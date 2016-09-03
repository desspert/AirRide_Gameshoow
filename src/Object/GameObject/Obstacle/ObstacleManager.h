#pragma once
#include "ObstaclePopArea.h"
#include "Obstacle.h"
#include "../GameObject.h"
//#include "../CharaBase/Player/Player.h"
//#include "../CharaBase/Enemy/EnemyHolder/EnemyHolder.h"
//#include "../Camera/Camera.h"

class Player;
class EnemyHolder;
//class Camera;

namespace ar {
	class ObstacleManager : public GameObject {
	public:

		ObstacleManager();

		void setup()override;
		void update()override;
		void draw()override;
		void transDraw()override;

		void setCameraPos(const ci::Vec3f& camera_pos_);

	private:
		std::list<ObstaclePopArea> pop_areas;


		void loadObstacleArea();

	public:
		std::shared_ptr<ar::Obstacle> getNearestObstacle(ci::Vec3f target_);

		void setPlayer(std::shared_ptr<Player> player_) { player = player_; }
		void setEnemyHolder(std::shared_ptr<EnemyHolder> enemy_holder_) { enemy_holder = enemy_holder_; }


		void isPlayerHitObstacle();
		void isEnemysInObstacleArea();

		ci::Vec3f camera_pos;
	private:
		std::shared_ptr<Player> player;
		std::shared_ptr<EnemyHolder> enemy_holder;

		//std::shared_ptr<Camera> camera;
	};
}