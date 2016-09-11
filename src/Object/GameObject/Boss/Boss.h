#pragma once
#include "../GameObject.h"

class CharaBase;
class EnemyHolder;
namespace ar {
	class SignPostManager;
}


class Boss : public ar::GameObject
{
public:
	void setup() override;
	void update()override;
	void draw()override;

	//�{�X���o�Ă���Ƃ���true��Ԃ��܂�
	bool getIsExist();

	void setPlayer(std::shared_ptr<CharaBase>);
	void setEnemyHolder(std::shared_ptr<EnemyHolder>);
	void setSignPostManager(std::shared_ptr<ar::SignPostManager>);


	bool isPushBullet();
	ci::Vec3f getWorldpos();
	ci::Matrix44f getMatrcx();


	bool isDead() { return HP <= 0; }
	int getHp() { return HP; }
	int getMaxHp() { return HP_max; }

private:


	void damage();

	void keepDifference();
	void entry();



	EnemyHolder* enemyholder;
	ar::SignPostManager* signpostmanager;
	CharaBase* player;
	bool is_active;
	ci::Matrix44f matrix;

	int pushcount;

	int HP;
	int HP_max;
	int difference;
	float maxspeed;
	float minspeed;

	bool is_hit;
};



