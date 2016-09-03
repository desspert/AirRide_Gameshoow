#pragma once
#include "../SceneManager/SceneBase.h"

#include "../../Object/GameObject/GameObjectEntities.h"
#include "../../UI/UIPlate/UIPlate.h"


class GameMain :public SceneBase {
public:

	GameMain();
	void setup() override;
	void draw() override;
	void update() override;
	void shift() override;
	void shutdown() override;

private:
	ar::GameObjectEntities entities;

	bool is_setup = false;
	UIPlate ui;
};


