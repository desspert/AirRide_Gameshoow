#include "UIPlate.h"

void UIPlate::titleSetup()
{
	
	for (auto it = UIObjects::get().begin(); it != UIObjects::get().end(); it++) {
		TEX.set((*it), ui_data[(*it)]->getTexturePath());
		if (ui_data[(*it)]->isGaugeUI()) {
			TEX.set(ui_data[(*it)]->getGaugeTexturePath(), ui_data[(*it)]->getGaugeTexturePath());
		}
	}
	ui_data["�v���C���[�|�W�V����"]->Active();
	ui_data["�_�b�V���Q�[�W"]->Active();
	ui_data["�|�W�V�����o�["]->Active();
	
	ci::gl::enableAlphaBlending();
}
float a = 0;
void UIPlate::titleUpdate() {
	for (auto it = UIObjects::get().begin(); it != UIObjects::get().end(); it++) {
		ui_data[(*it)]->update();
	}
	a++;
	ui_data["�_�b�V���Q�[�W"]->changeGauge(a,500.f);
	/*if (ui_data["�I�v�V����"]->NextFlag() || ui_data["�X�^�[�g"]->NextFlag()) {
	if (ui_data["�^�C�g��"]->collisionToUI(des::Vec2f(event.getX(), event.getY()))) {
	ui_data["�^�C�g��"]->setEnd();
	ui_data["�I�v�V����"]->Active();
	ui_data["�X�^�[�g"]->Active();
	}
	}

	if (ui_data["�^�C�g��"]->NextFlag()) {
	if (ui_data["�I�v�V����"]->collisionToUI(des::Vec2f(event.getX(), event.getY())) ||
	ui_data["�X�^�[�g"]->collisionToUI(des::Vec2f(event.getX(), event.getY()))) {
	ui_data["�I�v�V����"]->setEnd();
	ui_data["�X�^�[�g"]->setEnd();
	ui_data["�^�C�g��"]->Active();
	}
	}*/
}

void UIPlate::titleDraw() {
	
	
	ci::gl::setMatrices(camera_o);
	for (auto it = UIObjects::get().begin(); it != UIObjects::get().end(); it++) {
	

		ci::gl::pushModelView();
		
		ci::gl::color(ui_data[(*it)]->getColorR(),ui_data[(*it)]->getColorG(),ui_data[(*it)]->getColorB(),ui_data[(*it)]->getColorA());
		ci::gl::translate(0.0f, 0.0f, -3.0f);
		
		
		
		

		TEX.get((*it)).enableAndBind();
		ci::gl::drawSolidRect(
			ci::Rectf(ci::Vec2f(
				ui_data[(*it)]->getPosX(),
				ui_data[(*it)]->getPosY()),
				ci::Vec2f(
					ui_data[(*it)]->getPosX() + ui_data[(*it)]->getSizeX(),
					ui_data[(*it)]->getPosY() + ui_data[(*it)]->getSizeY())));
		TEX.get((*it)).disable();
		
		ci::gl::popModelView();

		ci::gl::pushModelView();

		ci::gl::color(ui_data[(*it)]->getColorR(), ui_data[(*it)]->getColorG(), ui_data[(*it)]->getColorB(), ui_data[(*it)]->getColorA());
		ci::gl::translate(0.0f, 0.0f, -1.0f);

		if (ui_data[(*it)]->isGaugeUI()) {
			TEX.get(ui_data[(*it)]->getGaugeTexturePath()).enableAndBind();
			ci::gl::drawSolidRect(
				ci::Rectf(ci::Vec2f(
					ui_data[(*it)]->getGaugePosX(),
					ui_data[(*it)]->getGaugePosY()),
					ci::Vec2f(
						ui_data[(*it)]->getGaugePosX() + ui_data[(*it)]->getGaugeSizeX(),
						ui_data[(*it)]->getGaugePosY() + ui_data[(*it)]->getGaugeSizeY())));
			TEX.get(ui_data[(*it)]->getGaugeTexturePath()).disable();
		
		}
		ci::gl::popModelView();
		
	}
	
}














void UIPlate::gameMainSetup()
{
}

void UIPlate::gameMainUpdate()
{
}

void UIPlate::gameMainDraw()
{
}
