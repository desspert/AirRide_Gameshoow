#include "Player.h"
#include "../../../Component/Components/Texture.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::setup()
{
	LEAPHANDS.Setup();

	init();

	transform.scale = ci::Vec3f(0.07f, 0.07f, 0.07f);

	operation_type = OperationType::KEY;

	move_direction = ci::Vec2f::zero();

	window_size_camera_to_player = ci::Vec2f(40.0f, 40.0f);
	pos_to_ratio = ci::Vec2f::zero();
	interval_frame = 6;
	before_pos_to_ratio = ci::Vec2f::zero();
	before_hand_normal = ci::Vec3f::zero();
	before_hand_pos = ci::Vec3f::zero();


	min_hand_normal_z_range = -0.5;
	max_hand_normal_z_range = 0.5f;
	min_hand_normal_xy_range = (float)-M_PI / 3.0f;
	max_hand_normal_xy_range = (float)M_PI / 3.0f;
	min_dot_product_range = 1.0f;
	max_dot_product_range = 0.5f;
	dash_range = 50.0f;

	max_clash_count = 1.0f;
	clash_speed = 0.5f;
	start_clash_speed = 0.5f;
	end_clash_speed = 0.5f;


//	addComponent<ar::Texture>(ar::Texture("Player"));
}

void Player::update()
{
	changeOperationType();

	if (operation_type == OperationType::LEAPMOTION)
		operationLeap();
	else if (operation_type == OperationType::KEY)
		operationKey();

	move();

	// cameraからはみ出ないようにする
	debugCourseOutStop();

	// stage行列に変換
	updateStageMatrix();
}

void Player::draw()
{
	ci::gl::popMatrices();

	ci::gl::pushMatrices();
	ci::gl::multModelView(matrix);
	ci::Matrix44f mtranslate = ci::Matrix44f::createTranslation(ci::Vec3f(0.0f, 1.0f, 0.0f));
	ci::gl::multModelView(mtranslate);
	ci::Matrix44f mrotate = ci::Matrix44f::createRotation(transform.angle);
	ci::gl::multModelView(mrotate);
	mtranslate = ci::Matrix44f::createTranslation(ci::Vec3f(0.0f, -1.0f, 0.0f));
	ci::gl::multModelView(mtranslate);
	ci::Matrix44f mscale = ci::Matrix44f::createScale(transform.scale);
	ci::gl::multModelView(mscale);

	ci::gl::draw(ObjDataGet.find("Player"));

	ci::gl::popMatrices();
	ci::gl::pushMatrices();
}

void Player::changeOperationType()
{
	if (LEAPHANDS.IsHandExist() && operation_type == OperationType::KEY)
	{
		operation_type = OperationType::LEAPMOTION;
		return;
	}

	if (!LEAPHANDS.IsHandExist())
	{
		operation_type = OperationType::KEY;
		return;
	}
}

void Player::operationKey()
{
	debugMove();

	if (env.isPush(ci::app::KeyEvent::KEY_SPACE))
		debugRoll();

	if (env.isPush(ci::app::KeyEvent::KEY_b))
		debugDash();

	if (env.isPush(ci::app::KeyEvent::KEY_RETURN))
		HitObstacle(clash_speed);
}

void Player::debugMove()
{
	if (status != CharaStatus::NORMAL)
		return;

	ci::Vec2f _direction = ci::Vec2f::zero();
	if (env.isPress(ci::app::KeyEvent::KEY_j)) {
		_direction.x = 1;
	}
	if (env.isPress(ci::app::KeyEvent::KEY_l)) {
		_direction.x = -1;
	}
	if (env.isPress(ci::app::KeyEvent::KEY_i)) {
		_direction.y = 1;
	}
	if (env.isPress(ci::app::KeyEvent::KEY_k)) {
		_direction.y = -1;
	}

	if (_direction.lengthSquared() > 0) {
		_direction.normalize();
		start_move_pos = ci::Vec2f(transform.position.x, transform.position.y);
		end_move_pos = ci::Vec2f(transform.position.x, transform.position.y) + _direction * 2.0f;
		move_count = 0.0f;

		move_direction = _direction / 10.0f;
	}


}

void Player::debugRoll()
{
	if (status != CharaStatus::NORMAL)
		return;

	if (move_direction.lengthSquared() <= 0)
		return;

	isRolling(move_direction * 50.0f);
}

void Player::debugDash()
{
	if (status != CharaStatus::NORMAL)
		return;

	isAttacking();
}

void Player::operationLeap()
{
	// leapmotionの更新処理
	UpdateLeapHands();

	// 行き先の更新
	moveDestination();

	// Rollの確認処理
	handNormalRotation();

	// Dashの確認処理
	handPosZDistance();
}

void Player::UpdateLeapHands()
{
	pos_to_ratio = LEAPHANDS.GetHandCenterPosToRatio();

	if (TIME.isIntervalTime(interval_frame))
	{
		before_hand_normal = LEAPHANDS.GetHandNormal();
		before_hand_pos = LEAPHANDS.GetHandCenterPos();
		before_pos_to_ratio = LEAPHANDS.GetHandCenterPosToRatio();
	}
}

void Player::moveDestination()
{
	/* destination : 移動先 */
	ci::Vec2f destination_pos =
		ci::Vec2f(window_size_camera_to_player.x * pos_to_ratio.x,
				  window_size_camera_to_player.y * pos_to_ratio.y);

	// 前のフレームのプレイヤーの行き先と今現在の行き先を比べて違った場合、更新する
	if (end_move_pos == destination_pos)
		return;

	moving(destination_pos);
}

void Player::handNormalRotation()
{
	// LeapMotion が hand を認識していないときはじく
	if (!LEAPHANDS.IsHandExist())
		return;

	// 掌の法線が真下からz方向に [－４５度より小さい]、[４５度より大きい場合] はじく
	if (LEAPHANDS.GetHandNormal().z > max_hand_normal_z_range ||
		LEAPHANDS.GetHandNormal().z < min_hand_normal_z_range)
		return;

	float start_theta = std::atan2(LEAPHANDS.GetHandNormal().x - 0.0f, LEAPHANDS.GetHandNormal().y - (-1.0f));

	if (start_theta < min_hand_normal_xy_range &&
		start_theta > max_hand_normal_xy_range)
		return;

	// 6frame前の法線と今現在の法線との内積
	float dot_product = (before_hand_normal.x * LEAPHANDS.GetHandNormal().x) + (before_hand_normal.y * LEAPHANDS.GetHandNormal().y);

	// 6frame前の手の法線と今の法線との内積が [０以上～９０度未満] の場合はじく
	if (dot_product <= min_dot_product_range && dot_product > max_dot_product_range)
		return;

	// 6frame前の法線と今の法線との角度
	float theta = std::atan2(LEAPHANDS.GetHandNormal().x - before_hand_normal.x, LEAPHANDS.GetHandNormal().y - before_hand_normal.y);

	// |a| * |b|
	float vec_a_to_b = std::sqrtf((before_hand_normal.x * before_hand_normal.x) + (before_hand_normal.y * before_hand_normal.y)) *
		std::sqrtf((LEAPHANDS.GetHandNormal().x * LEAPHANDS.GetHandNormal().x) + (LEAPHANDS.GetHandNormal().y * LEAPHANDS.GetHandNormal().y));

	// 外積公式 |a| * |b| * sinθ
	//float cross_product = vec_a_to_b * std::sin(theta);

	// ロールする方向
	ci::Vec2f distance_vec_normal = pos_to_ratio - before_pos_to_ratio;

	// とりまノーマライズ
	(distance_vec_normal).normalize();

	//// 今現在の位置と移動先の位置のノーマライズしたベクトルと真下のベクトルとの角度
	//float direction_theta = std::atan2(distance_vec_normal.x - 0.0f, distance_vec_normal.y - (-1.0f));

	//// 手が反時計回りしたとき
	//if (cross_product < 0.0f)
	//{
	//	// 移動先が右方向の場合はじく
	//	if (direction_theta > 0.0f)
	//		return;

	//	end_roll_angle = max_roll_angle;
	//}
	//// 手が時計回りしたとき
	//else if (cross_product >= 0.0f)
	//{
	//	// 移動先が左方向の場合はじく
	//	if (direction_theta <= 0.0f)
	//		return;

	//	end_roll_angle = -max_roll_angle;
	//}


	isRolling(distance_vec_normal * 10.0f);
}

void Player::handPosZDistance()
{
	float distance_z = LEAPHANDS.GetHandCenterPos().z - before_hand_pos.z;

	// 手のz軸に対しての移動量が満たなかった場合はじく
	if (distance_z < dash_range)
		return;

	isAttacking();
}
