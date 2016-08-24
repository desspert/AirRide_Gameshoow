#pragma once
#include "cinder/app/AppNative.h"
#include "../GameObject.h"


namespace ar {
	class Obstacle : public ar::GameObject {
	public:
		// �@���̔��a
		Obstacle(const ci::Vec3f& position_, float radius_);

		Obstacle() {}

		void setup();
		void update();
		void draw();

	private:
		// ���a
		float radius;

	};
}