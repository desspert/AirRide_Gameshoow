#include "Obstacle.h"
#include "../../Component/Components/Material.h"
#include "../../../TaskManager/ObjDataManager.h"
#include "../../Component/Components/Texture.h"

using namespace ci;
using namespace ci::app;

ar::Obstacle::Obstacle(const ci::Vec3f& position_, float radius_)
{
	radius = radius_;
	transform.position = position_;
	setup();

	is_bomb = false;
	is_erase = false;
	respawn_count = 200;
	count = 0;
}

void ar::Obstacle::setup()
{
	addComponent<ar::Material>(ar::Material(
		gl::Material(ColorA(1.0f, 1.0f, 1.0f, 1.0f),      // Ambient
					 ColorA(1.0f, 1.0f, 1.0f, 1.0f),      // Diffuse
					 ColorA(1.0f, 1.0f, 1.0f, 1.0f),      // Specular
					 80.0f,                               // Shininess
					 ColorA(0.5f, 0.5f, 0.5f, 1.0f))));	  // Emission
	addComponent<ar::Texture>(ar::Texture("Obstacle"));
}

void ar::Obstacle::update()
{
	if (is_bomb)
	{
		if (count == 0)
			particle.push_back(std::make_shared<ParticleController>(ParticleController(transform.position)));
		count++;
		if (count > respawn_count)
		{
			is_erase = true;
			particle.clear();
		}
		std::for_each(particle.begin(), particle.end(),
					  [](std::shared_ptr<ParticleController> ptc_) {ptc_->update(); });
	}
}

void ar::Obstacle::draw()
{
	drawBegin();
	pushModelView();

	if (is_bomb)
		std::for_each(particle.begin(), particle.end(),
					  [&](std::shared_ptr<ParticleController> ptc_) {ptc_->draw(); });
	else
	{
		gl::scale(0.04, 0.04, 0.04);

		gl::draw(ObjDataGet.find("obstacle"));
	}


	popModelView();
	drawEnd();
}

void ar::Obstacle::setCameraPos(const ci::Vec3f & camera_pos_)
{
	std::for_each(particle.begin(), particle.end(),
				  [&](std::shared_ptr<ParticleController> ptc_) {ptc_->setCameraPos(camera_pos_); });
}

void ar::Obstacle::bomb()
{
	is_bomb = true;
}
