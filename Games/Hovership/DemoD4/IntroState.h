#ifndef INTROSTATE_H_
#define INTROSTATE_H_

#include <IAnimatedMeshSceneNode.h>
#include <ILightSceneNode.h>
#include <ICameraSceneNode.h>
#include <IGUIEnvironment.h>
#include <IGUIButton.h>

class IntroState : public State<Game, irr::SEvent>
{

private:


	irr::scene::ILightSceneNode* light;


	irr::gui::IGUIButton* startButton;

	irr::gui::IGUIButton* editPlayerButton;
	
	irr::gui::IGUIButton* optionsButton;

	irr::gui::IGUIButton* exitButton;

public:

	IntroState();

	virtual ~IntroState();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual const bool onEvent(Game* game, const irr::SEvent& event);

	virtual void onUpdate(Game* game);
};

IntroState::IntroState() :
	light(0), 
	startButton(0), editPlayerButton(0), optionsButton(0), exitButton(0)
{

}

IntroState::~IntroState()
{

	if (light)
	{
		light->remove();
		light->drop();
	}

	if (optionsButton)
	{
		optionsButton->remove();
		optionsButton->drop();
	}

	if (startButton)
	{
		startButton->remove();
		startButton->drop();
	}

	if (editPlayerButton)
	{
		editPlayerButton->remove();
		editPlayerButton->drop();
	}

	if (exitButton)
	{
		exitButton->remove();
		exitButton->drop();
	}
}

void IntroState::onEnter(Game* game)
{
	
	this->light = game->getSceneManager()->addLightSceneNode();
	this->light->setPosition(irr::core::vector3df(0.0f, 10.0f, 0.0f));
	this->light->grab();

	irr::video::SLight& lightData = this->light->getLightData();
	lightData.AmbientColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	lightData.DiffuseColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	lightData.SpecularColor.set(1.0f, 1.0f, 1.0f, 1.0f);
	lightData.Radius = 20.0f;

	//setup global camera
	{
		game->getCamera()->setPosition(irr::core::vector3df(0.0f, 0.0f, -6.0f));
	
		irr::core::matrix4 m;
		m.setRotationDegrees(irr::core::vector3df(0.0f, 0.0f, 0.0f));
		irr::core::vector3df upv(0.0f, 1.0f, 0.0f);
		m.transformVect(upv);
		game->getCamera()->setUpVector(upv);
	}

	//add gui
	{
		this->startButton = game->getGuiEnvironment()->addButton(
			irr::core::rect<irr::s32>(SX(120), SY(120), SX(250), SY(150)),
			0, -1, L"Start [F1]", L"Start the demo");
		this->startButton->grab();

		this->editPlayerButton = game->getGuiEnvironment()->addButton(
			irr::core::rect<irr::s32>(SX(120), SY(170), SX(250), SY(200)),
			0, -1, L"Player [F2]", L"Edit player settings");
		this->editPlayerButton->grab();		
		
		this->optionsButton = game->getGuiEnvironment()->addButton(
			irr::core::rect<irr::s32>(SX(120), SY(220), SX(250), SY(250)),
			0, -1, L"Options [F3]", L"Setup demo");
		this->optionsButton->grab();

		this->exitButton = game->getGuiEnvironment()->addButton(
			irr::core::rect<irr::s32>(SX(120), SY(270), SX(250), SY(300)),
			0, -1, L"Exit [ESC]", L"Exit the demo");
		this->exitButton->grab();
	}

}

void IntroState::onLeave(Game* game)
{
	this->light->remove();
	this->light->drop();
	this->light = 0;
	
	startButton->remove();
	startButton->drop();
	startButton = 0;
	
	optionsButton->remove();
	optionsButton->drop();
	optionsButton = 0;

	editPlayerButton->remove();
	editPlayerButton->drop();
	editPlayerButton = 0;

	if (exitButton)
	{
		exitButton->remove();
		exitButton->drop();
		exitButton = 0;
	}
}

const bool IntroState::onEvent(Game* game, const irr::SEvent& event)
{
	switch(event.EventType)
	{
		case irr::EET_GUI_EVENT:
			if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
			{				
				if (event.GUIEvent.Caller == this->exitButton)
				{
					game->getDevice()->closeDevice();
					return true;
				}
			}
		break;
		
		
		case irr::EET_KEY_INPUT_EVENT:
		{
			if (!event.KeyInput.PressedDown)
			{
				if (event.KeyInput.Key == irr::KEY_ESCAPE)
				{
					game->getDevice()->closeDevice();
					return true;
				}
			}
	    }
		break;

		default: return false;
	}

	return false;
}

void IntroState::onUpdate(Game* game)
{
	//game->drawBoundingBoxes(fighterNode, true, false);
}

#endif /*INTROSTATE_H_*/
