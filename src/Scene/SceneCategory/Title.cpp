#include "Title.h"
#include "../SceneCategory/GameMain.h"
#include "../../Input/InputEvent.h"
#include "../../Input/LeapMotion/LeapHands/LeapHands.h"

using namespace ci;
using namespace ci::app;

Title::Title()
{
}

void Title::setup()
{
	color_r = 0.0f;
	color_g = 0.0f;
	color_b = 0.0f;
	c_Easing::apply(color_r, 1.0f, EasingFunction::ExpoIn, 60);
	c_Easing::apply(color_g, 1.0f, EasingFunction::ExpoIn, 60);
	c_Easing::apply(color_b, 1.0f, EasingFunction::ExpoIn, 60);
	update_count = 0;
	end_count = 0;
	end_flag = false;
}

void Title::update()
{
	if (c_Easing::isEnd(color_b)) {
		update_count++;
		if (update_count == 1) {
			ui.titleSetup();
			entities.setupGameObject();
			if (SoundGet.find("TitleBGM")->isEnabled())
				SoundGet.find("TitleBGM")->stop();
			else
				SoundGet.find("TitleBGM")->start();
			SoundGet.find("TitleBGM")->setLoopEnabled(true);
		}
		ui.titleUpdate();
		entities.updateGameObject();
	}
	if (end_flag == true &&
		end_count == 1) {
		c_Easing::apply(color_r, 0.0f, EasingFunction::ExpoIn, 60);
		c_Easing::apply(color_g, 0.0f, EasingFunction::ExpoIn, 60);
		c_Easing::apply(color_b, 0.0f, EasingFunction::ExpoIn, 60);
	}

}

void Title::draw()
{
	ci::gl::clear(ColorA(color_r, color_g, color_b, 1.0f));
	ui.titleDraw();
	entities.drawGameObject();
}

void Title::shift()
{
	if (env.isPush(KeyEvent::KEY_RETURN))
	{
		ui.ui_data["R"]->setEnd();
		ui.ui_data["I"]->setEnd();
		ui.ui_data["D"]->setEnd();
		ui.ui_data["E"]->setEnd();
		ui.ui_data["O"]->setEnd();
		ui.ui_data["N"]->setEnd();
		ui.ui_data["T"]->setEnd();
		ui.ui_data["H"]->setEnd();
		ui.ui_data["E2"]->setEnd();
		ui.ui_data["S"]->setEnd();
		ui.ui_data["K"]->setEnd();
		ui.ui_data["Y"]->setEnd();
		ui.ui_data["!"]->setEnd();
		ui.ui_data["スタート"]->setEnd();
		SoundGet.find("TitleBGM")->disable();
		SoundGet.find("Start")->start();

		end_flag = true;
	}
	if (end_flag == true) {
		end_count++;
	}
	if (end_count >= 60) {
		//gl::popMatrices();
		Scene::createScene<GameMain>(new GameMain());
	}
}

void Title::shutdown()
{
	entities.Alldestroy();
}
