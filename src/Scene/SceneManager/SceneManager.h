#pragma once
#include "CreateScene.h"
#include "../SceneCategory/Title.h"
#include "../SceneCategory/Select.h"
#include "../SceneCategory/GameMain.h"
#include "../SceneCategory/Result.h"

class SceneManager
{
public:
	//static SceneManager& get() {
	//	static SceneManager scenemanager;
	//	return scenemanager;
	//}
	SceneManager() {
		Scene::createScene<Title>(new Title());
		//Scene::createScene<GameMain>(new GameMain());
	}
	void update() {
		Scene::get().update();
	}
	void draw() {
		Scene::get().draw();
	}
	void setup() {
		Scene::get().setup();
	}
	void shift() {
		Scene::get().shift();
	}
	void shutdown() {
		Scene::get().shutdown();
	}
};