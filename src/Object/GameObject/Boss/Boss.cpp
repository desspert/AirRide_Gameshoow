#include "Boss.h"
#include "../../GameObject/CharaBase/CharaBase.h"
#include "../../GameObject/CharaBase/Enemy/EnemyHolder/EnemyHolder.h"
#include "../../GameObject/SignPost/SignPostManager.h"
#include "../../../Share/Easing/Easing.h"
#include "../../../Share/Interface/Interface.h"

void Boss::setup()
{
	is_active = false;
	pushcount = 0;
	HP = 10;
	is_hit = false;

	difference = 22.f;
	maxspeed = 1.3f;
	minspeed = 0.5f;
	Params->addParam("Boss difference", &difference).group("BOSS");
	Params->addParam("Boss maxspeed", &maxspeed).step(0.1f).group("BOSS");
	Params->addParam("Boss minspeed", &minspeed).step(0.1f).group("BOSS");
	Params->addParam("Boss HP", &HP).group("BOSS");
	//Params->addParam("Boss minspeed", &minspeed).step(0.1f).group("BOSS");

}

void Boss::update()
{
	if (!is_active) {
		entry();
		return;
	}
	damage();
	pushcount++;
	pushcount %= 60;
	keepDifference();
	matrix = signpostmanager->getMatrix(transform.position);

}

void Boss::draw()
{
	if (!is_active)return;
	ci::gl::popModelView();
	ci::gl::pushModelView();


	ci::gl::multModelView(matrix);
	ci::gl::scale(ci::Vec3f(7, 7, 1));
	ci::gl::drawColorCube(ci::Vec3f::zero(), ci::Vec3f::one());


	ci::gl::popModelView();
	ci::gl::pushModelView();

}

void Boss::setPlayer(std::shared_ptr<CharaBase> _player)
{
	player = _player.get();
}

void Boss::setEnemyHolder(std::shared_ptr<EnemyHolder> _eh)
{
	enemyholder = _eh.get();
}

void Boss::setSignPostManager(std::shared_ptr<ar::SignPostManager> _spm)
{
	signpostmanager = _spm.get();
}

void Boss::entry()
{
	if (is_active)return;
	if (!enemyholder->isEndLasstEnemy()) return;

	transform.position.z = player->transform.position.z + 100;

	is_active = true;
}


bool Boss::getIsExist() {
	return is_active;
}

bool Boss::isPushBullet()
{
	if (!is_active)return false;
	return pushcount == 0;

}

ci::Vec3f Boss::getWorldpos()
{
	return  matrix * ci::Vec3f::zero();
}

ci::Matrix44f Boss::getMatrcx()
{
	return matrix;
}

void Boss::damage()
{
	
	if (player->transform.position.z > transform.position.z){
		if (is_hit) return;
		HP--;
		is_hit = true;
		return;
	}


	is_hit = false;
}

void Boss::keepDifference()
{
	float _difference = -player->transform.position.z + transform.position.z;

	if (_difference > difference) {
		transform.position.z += minspeed;
	}
	if (_difference < difference) {
		transform.position.z += maxspeed;
	}

}
