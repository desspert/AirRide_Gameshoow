#pragma once
#include "cinder/app/AppNative.h"
#include "../../../GameObject/GameObject.h"



namespace ar
{
	class Particle : public ar::GameObject {
	public:

		Particle(float randvalue_);

		void update();
		void draw();

		void setCameraPos(const ci::Vec3f& camera_pos_) {
			camera_pos = camera_pos_;
		}

	private:
		ci::Vec3f camera_pos;
	public:

		ci::Vec3f vec;

		int time;
		int exist_time;
		bool is_erase;

	};
	class ParticleController :public ar::GameObject
	{
	public:
		ParticleController(const ci::Vec3f& position_);

		void update();
		void draw();
		void erase();

		void setCameraPos(const ci::Vec3f& camera_pos_);

	private:

		std::list<std::shared_ptr<Particle>> particles;
	};


}


