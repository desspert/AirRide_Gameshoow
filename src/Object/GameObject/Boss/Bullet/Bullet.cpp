#include "Bullet.h"
#include "../Boss.h"
#include "cinder/Rand.h"
#include "../../../../TaskManager/ObjDataManager.h"
#include "../../../../TaskManager/TextureManager.h"
#include "../../../GameObject/CharaBase/CharaBase.h"

void Bullets::setup()
{
	bullets.push_back(Bullet(ci::Vec3f::zero()));
}

void Bullets::update()
{
	//�̂��Ɋ֐���
	{
		if (boss->isPushBullet()) {
			ci::Vec2f direction = (ci::Matrix22f::createRotation(ci::randFloat(0, M_PI * 2))
				*ci::Vec2f::xAxis())
				*ci::randFloat(4.f);
			ci::Vec3f add = boss->getMatrcx().
				transformVec(ci::Vec3f(direction.x, direction.y, 0));
			bullets.push_back(Bullet(
				boss->getWorldpos() + add
				));
		}
	}
	{
		for (auto& it : bullets) {
			it.update();
		}
	}
	{
		bullets.remove_if([&](Bullet& _b) { return _b.isErase(); });
	}
	{
		float dif;
		for (auto& it : bullets) {
			dif = player->getWorldPoisition().distanceSquared(
				it.transform.position);
			if (dif < 2*2) {
				player->HitObstacle(0.5f);
			}
		}
	}



}

void Bullets::draw()
{

	for (auto& it : bullets) {
		it.draw();
	}
}

void Bullets::setBoss(std::shared_ptr<Boss> _boss)
{
	boss = _boss.get();
}

void Bullets::setPlayer(std::shared_ptr<CharaBase> _player)
{
	player = _player.get();
}

Bullet::Bullet(ci::Vec3f _pos)
{
	count = 60 * 3;
	transform.position = _pos;
	mesh = &ObjDataGet.find("obstacle");
}

void Bullet::update()
{
	count--;
}

void Bullet::draw()
{
	ci::gl::pushModelView();
	ci::gl::translate(transform.position);
	ci::gl::scale(0.03f, 0.03f, 0.03f);
	ci::gl::draw(*mesh);
	//ci::gl::drawColorCube(ci::Vec3f::zero(), ci::Vec3f::one());

	ci::gl::popModelView();

}

bool Bullet::isErase()
{
	return count < 0;
}