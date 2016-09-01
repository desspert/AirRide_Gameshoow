#pragma once
#include "Obstacle.h"
#include "cinder/gl/Material.h"


namespace ar {
	class ObstaclePopArea : public ar::GameObject {
	public:

		// �N���ʒu �͈� �@���̔��a ��
		ObstaclePopArea(const ci::Vec3f& position_,
						float pop_range_,
						float radius_,
						int count_);
		ObstaclePopArea() {}

		void update()override;
		void draw()override;
		void transDraw()override;

		void setCameraPos(const ci::Vec3f& camera_pos_);

		std::list<std::shared_ptr<Obstacle>> getObstacles() {
			return obstacles;
		}

		// ��ԋ߂���Q����Ԃ�
		std::shared_ptr<Obstacle> getNearestObstacle(ci::Vec3f target_);

		// �����������ǂ���
		bool isHitObstacle(ci::Vec3f target_, float radius_);

		float getPopRange() {
			return pop_range;
		}

	private:

		std::list<std::shared_ptr<Obstacle>> obstacles;

		// �N���͈͒��a
		float pop_range;

		// �@���̑傫��
		float radius;
		// ��
		int count;
		void obstaclePop();


	private:


		void eraseObstacle();
		void respawnObstacle();

	private:// �f�o�b�O�p�͈̔�
		void testObstaclePopArea();
		ci::gl::Material test_mt;

	};
}