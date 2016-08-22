#pragma once
#include "../CharaBase.h"
#include "../../../../Input/LeapMotion/LeapHands/LeapHands.h"
#include "../../../../Input/InputEvent.h"


class Player : public CharaBase
{
public:

	Player();
	~Player();

	void setup() override;
	void update() override;
	void draw() override;

private:

	void changeOperationType();

	void operationKey();

	void debugMove();
	void debugRoll();
	void debugDash();
	

	void operationLeap();

	void moveDestination();
	void UpdateLeapHands();
	void handNormalRotation();
	void handPosZDistance();

	enum class OperationType
	{
		LEAPMOTION,
		KEY
	};

	OperationType operation_type;

	ci::Vec2f move_direction;

	// camera���猩���v���C���[�̉f��͈�
	ci::Vec2f window_size_camera_to_player;
	ci::Vec2f pos_to_ratio;
	int interval_frame;
	ci::Vec2f before_pos_to_ratio;
	ci::Vec3f before_hand_normal;
	ci::Vec3f before_hand_pos;

	// roll�Ŏg���͈͎w��̕ϐ�
	float min_hand_normal_z_range;
	float max_hand_normal_z_range;
	float min_hand_normal_xy_range;
	float max_hand_normal_xy_range;
	float min_dot_product_range;
	float max_dot_product_range;

	// dash�Ŏg���͈͎w��̕ϐ�
	float dash_range;
};