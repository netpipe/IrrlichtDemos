#ifndef MYGLOBALSTATE_H_
#define MYGLOBALSTATE_H_

#include <IGUIEnvironment.h>

class GlobalState : public State<Game, irr::SEvent>
{

private:

	irr::s32 lastFPS;

public:

	GlobalState();

	virtual ~GlobalState();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual void onUpdate(Game* game);

	virtual const bool onEvent(Game* entity, const irr::SEvent& event);
};

GlobalState::GlobalState() :
	lastFPS(0)
{
}

GlobalState::~GlobalState()
{
}

void GlobalState::onEnter(Game* game)
{
	
}

void GlobalState::onLeave(Game* game)
{
}

void GlobalState::onUpdate(Game* game)
{
	// render the scene
	if (game->getDevice()->isWindowActive())
	{
		game->getPhysics()->update();

		game->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 0, 0, 0));
		game->getSceneManager()->drawAll();
		game->getGuiEnvironment()->drawAll();
		game->getVideoDriver()->endScene();

		const irr::s32 fps = game->getVideoDriver()->getFPS();
		if (this->lastFPS != fps)
		{
			this->lastFPS = fps;

			wchar_t fpsString[32];
			swprintf(fpsString, 32, L" [FPS=%u]", fps);

			irr::core::stringw fullTitle(game->getConfiguration().getProjectName());
			fullTitle.append(fpsString);

			game->getDevice()->setWindowCaption(fullTitle.c_str());
		}
	}
}

const bool GlobalState::onEvent(Game* game, const irr::SEvent& event)
{
	return false;
}

#endif /*MYGLOBALSTATE_H_*/
