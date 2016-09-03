#include "ObstacleManager.h"
#include "../../../Share/Share.h"
#include "../GameObjectEntities.h"
//#include "../CharaBase/Player/Player.h"
#include "../CharaBase/Enemy/EnemyHolder/EnemyHolder.h"
#include "../Camera/Camera.h"


using namespace ci;
using namespace ci::app;

ar::ObstacleManager::ObstacleManager()
{
}

void ar::ObstacleManager::setup()
{
	loadObstacleArea();
}

void ar::ObstacleManager::update()
{
	std::for_each(pop_areas.begin(), pop_areas.end(),
				  [](ObstaclePopArea pop_areas_) {pop_areas_.update(); });


	auto start = std::chrono::system_clock::now();

	isPlayerHitObstacle();
	isEnemysInObstacleArea();
	auto end = std::chrono::system_clock::now();
	auto d = end - start;
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(d).count();
	//console() << msec << std::endl;

	setCameraPos(camera_pos);
}

void ar::ObstacleManager::draw()
{
	std::for_each(pop_areas.begin(), pop_areas.end(),
				  [&](ObstaclePopArea pop_areas_) {
		if (sphereToSphere(player->getWorldPoisition(), 200, pop_areas_.transform.position, 1))
			pop_areas_.draw();
	});

}

void ar::ObstacleManager::transDraw()
{
	/*std::for_each(pop_areas.begin(), pop_areas.end(),
				  [](ObstaclePopArea pop_areas_) {
		pop_areas_.transDraw();
	});*/
}

void ar::ObstacleManager::setCameraPos(const ci::Vec3f & camera_pos_)
{
	std::for_each(pop_areas.begin(), pop_areas.end(),
				  [&](ObstaclePopArea pop_areas_) {pop_areas_.setCameraPos(camera_pos_); });
}


std::shared_ptr<ar::Obstacle> ar::ObstacleManager::getNearestObstacle(ci::Vec3f target_)
{
	float minvec = std::numeric_limits<float>::max();
	ObstaclePopArea nearest_pop_area;

	std::for_each(pop_areas.begin(), pop_areas.end(),
				  [&](ObstaclePopArea pop_area)
	{
		Vec3f vec = target_ - pop_area.transform.position;
		float length = vec.length();

		if (length < minvec)
		{
			minvec = length;
			nearest_pop_area = pop_area;
		}
	});
	return nearest_pop_area.getNearestObstacle(target_);
}

void ar::ObstacleManager::isPlayerHitObstacle()
{
	std::for_each(pop_areas.begin(), pop_areas.end(),
				  [&](ObstaclePopArea pop_area_)
	{
		if (pop_area_.isHitObstacle(player->getWorldPoisition(), player->getCollisionCirclerad()))
		{
			player->HitObstacle(player->getClashSpeed());
		}
	});
}

void ar::ObstacleManager::isEnemysInObstacleArea()
{
	std::for_each(pop_areas.begin(), pop_areas.end(),
				  [&](ObstaclePopArea pop_area_)
	{
		Vec3f enemy_pos = (*enemy_holder->getActiveEnemys().begin())->getWorldPoisition();
		float enemy_range = (*enemy_holder->getActiveEnemys().begin())->getCollisionCirclerad();

		if (sphereToSphere(pop_area_.transform.position, pop_area_.getPopRange(),
						   enemy_pos, enemy_range))
		{
			(*enemy_holder->getActiveEnemys().begin())->inObstacleArea();
		}
	});
}



void ar::ObstacleManager::loadObstacleArea()
{
	std::ifstream ifstr(getAssetPath("StageInfo/ObstaclePoints.txt").string());

	if (!ifstr.is_open())
		assert(!"Obstacle text not find");

	while (!ifstr.eof())
	{
		Vec3f pos;
		float range;
		ifstr >> pos.x >> pos.y >> pos.z;
		ifstr >> range;
		pop_areas.push_back(ObstaclePopArea(pos, range, 1.0f, 1));
	}
}
