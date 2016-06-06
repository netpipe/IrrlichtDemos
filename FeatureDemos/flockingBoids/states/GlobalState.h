#ifndef MYGLOBALSTATE_H_
#define MYGLOBALSTATE_H_

class GlobalState : public State<Demo, irr::SEvent>
{

public:

	GlobalState();

	virtual ~GlobalState();

	virtual void onEnter(Demo* const demo);

	virtual void onLeave(Demo* const demo);

	virtual void onUpdate(Demo* const demo);

	virtual const bool onEvent(Demo* const demo, const irr::SEvent& event);

private:

	irr::s32 lastFPS;
};

GlobalState::GlobalState() :
	lastFPS(0)
	{
	}

GlobalState::~GlobalState()
{
}

void GlobalState::onEnter(Demo* const demo)
{

}

void GlobalState::onLeave(Demo* const demo)
{
}

void GlobalState::onUpdate(Demo* const demo)
{
	if (demo->getDevice()->isWindowActive())
	{

		// render the scene
		demo->getVideoDriver()->beginScene(true, true, irr::video::SColor(255, 0, 0, 0));
		demo->getSceneManager()->drawAll();
		demo->getGuiEnvironment()->drawAll();
		demo->getVideoDriver()->endScene();

		if (!demo->getDevice()->isFullscreen())
		{
			const irr::s32 fps = demo->getVideoDriver()->getFPS();
			if (this->lastFPS != fps)
			{
				this->lastFPS = fps;

				wchar_t fpsString[32];
				swprintf(fpsString, 32, L" [FPS=%u]", fps);

				irr::core::stringw fullTitle(demo->getConfiguration()->getProjectName());
				fullTitle.append(fpsString);

				demo->getDevice()->setWindowCaption(fullTitle.c_str());
			}
		}
	}
}

const bool GlobalState::onEvent(Demo* const demo, const irr::SEvent& event)
{
	if (event.EventType == irr::EET_LOG_TEXT_EVENT)
	{
		//get text
		//		const irr::c8* message = event.LogEvent.Text;

		//get logLevel.
		irr::ELOG_LEVEL logLevel = event.LogEvent.Level;
		switch (logLevel)
		{
		case irr::ELL_INFORMATION: break; //High log level, warnings, errors and important information texts are printed out.
		case irr::ELL_WARNING: return false; break; //Default log level, warnings and errors are printed out.
		case irr::ELL_ERROR: return false; break; //Low log level, only errors are printed into the log.
		case irr::ELL_NONE: break; //Nothing is printed out to the log.
		}

		return true; //return true, to suppress console output
	}

	return false;
}

#endif /*MYGLOBALSTATE_H_*/
