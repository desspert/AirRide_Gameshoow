#pragma once
#include "cinder/app/AppNative.h"
#include "../GameObject.h"
#include "Particle/Particle.h"


namespace ar {
	class Obstacle : public ar::GameObject {
	public:
		// ‹@—‹‚Ì”¼Œa
		Obstacle(const ci::Vec3f& position_, float radius_);

		Obstacle() {}

		void setup();
		void update();
		void draw();


		void setCameraPos(const ci::Vec3f& camera_pos_);
	public:
		// ”¼Œa
		float radius;

	public:

		void bomb();

		bool is_erase;
	private:

		int respawn_count;
		int count;
		bool is_bomb;

		std::list<std::shared_ptr<ParticleController>> particle;



	};
}