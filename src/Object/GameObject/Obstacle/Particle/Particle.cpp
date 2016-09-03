#include "Particle.h"
#include "../../../Component/Components/Texture.h"
#include "../../../Component/Components/Material.h"
#include <random>

using namespace ci;
using namespace ci::app;



ar::Particle::Particle(float v_)
{
	addComponent<ar::Texture>(ar::Texture("Particle"));

	addComponent<ar::Material>(ar::Material(
		gl::Material(ColorA(1.0f, 1.0f, 1.0f, 1.0f),					  // Ambient
					 ColorA(1.0f * v_, 1.0f * v_, 1.0f * v_, 1.0f),       // Diffuse
					 ColorA(1.0f, 1.0f, 1.0f, 1.0f),					  // Specular
					 80.0f,												  // Shininess
					 ColorA(0.7f * v_, 0.7f * v_, 0.7f * v_, 1.0f))));	  // Emission


	std::random_device rand;
	std::mt19937 mt(rand());
	std::uniform_real_distribution<float> vec_rand(-1.0f, 1.0f);

	Vec3f vec_ = Vec3f(vec_rand(mt), vec_rand(mt), vec_rand(mt));
	vec = vec_;

	std::uniform_int_distribution<int> exist_time_rand(10, 70);
	exist_time = exist_time_rand(mt);

	is_erase = false;
	//camera_pos = ci::Vec3f(0, 0, 1);
}

void ar::Particle::update()
{
	transform.position += vec;

	time++;
	if (time > exist_time)
	{
		is_erase = true;
	}
}

void ar::Particle::draw()
{
	drawBegin();
	pushModelView();

	Quatf quat(Vec3f(0, 0, 1), camera_pos);

	gl::rotate(quat.toMatrix44());

	gl::drawCube(Vec3f::zero(), Vec3f(1, 1, 0));

	popModelView();
	drawEnd();
}



ar::ParticleController::ParticleController(const ci::Vec3f& position_)
{
	std::random_device rand;
	std::mt19937 mt(rand());

	std::uniform_real_distribution<float> material_rand(0.5f, 1.0f);

	int pop_num = 10;

	for (int i = 0; i < pop_num; i++)
	{
		particles.push_back(std::make_shared<Particle>(Particle(material_rand(mt))));
	}
}

void ar::ParticleController::update()
{
	std::for_each(particles.begin(), particles.end(),
				  [](std::shared_ptr<Particle> pt_)
	{
		pt_->update();
	});

	erase();
}

void ar::ParticleController::draw()
{
	std::for_each(particles.begin(), particles.end(),
				  [&](std::shared_ptr<Particle> pt_)
	{
		pt_->draw();
	});
}

void ar::ParticleController::erase()
{
	std::remove_if(particles.begin(), particles.end(),
				   [](std::shared_ptr<Particle> pt_)
	{
		return pt_->is_erase;
	});
}

void ar::ParticleController::setCameraPos(const ci::Vec3f & camera_pos_)
{
	std::for_each(particles.begin(), particles.end(),
				  [&](std::shared_ptr<Particle> pt_) {pt_->setCameraPos(camera_pos_); });
}

