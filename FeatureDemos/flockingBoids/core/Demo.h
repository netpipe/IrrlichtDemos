#ifndef DEMO_H_
#define DEMO_H_

#include <IrrlichtDevice.h>
#include <IMetaTriangleSelector.h>
#include "statemanager/StateManager.h"

namespace irr
{
	namespace scene
	{
		class ICameraSceneNode;
	}

	namespace gui
	{
		class IGUIFont;
	}
}

#ifdef _SOUND
namespace irrklang
{
	class ISoundEngine;
}
#endif

class Configuration;

// these macros will return the scaled value based on current screen size
// (i assume 1024x768 is the normal res) to provide a resolution independent gui.
#define SX(val) ( (irr::s32)(demo->getVideoDriver()->getScreenSize().Width*(irr::f32)(val)/1024.0f) )
#define SY(val) ( (irr::s32)(demo->getVideoDriver()->getScreenSize().Height*(irr::f32)(val)/768.0f) )

class Demo : public irr::IEventReceiver, public StateManager<Demo, irr::SEvent>
{

public:

	Demo(Configuration* const configuration, irr::IrrlichtDevice* const device
#ifdef _SOUND
		, irrklang::ISoundEngine* const soundEngine
#endif
	);

	~Demo();

	///Adapt irrlicht event handling to ours.
	bool OnEvent(const irr::SEvent& event)
	{
		return onEvent(event);
	}

	void addGameState(const char* const name, State<Demo, irr::SEvent>* state);

	/**
	 * Finds a game state by its name.
	 * \return The game state. 0 if no such game state exits.
	 */
	State<Demo, irr::SEvent>* findGameState(const char* const name);

	inline void tick()
	{
		//compute time since last frame
		this->now = this->timer->getTime();
		this->elapsed = ((irr::f32)(this->now - this->then))*0.001f;
		this->then = this->now;
	}

	void makeScreenshot() const;

	void addSelector(irr::scene::ITriangleSelector* const selector) const;

	void removeSelectors() const;

	//getter

	inline Configuration* const getConfiguration() const { return this->configuration; }

	///\return The Irrlicht device.
	inline irr::IrrlichtDevice* getDevice() const { return this->device; }
#ifdef _SOUND
	inline irrklang::ISoundEngine* const getSoundEngine() const { return this->soundEngine; }
#endif
	///\return The Irrlicht scene manager.
	inline irr::scene::ISceneManager* const getSceneManager() const { return this->sceneManager; }

	///\return The Irrlicht gui environment.
	inline irr::gui::IGUIEnvironment* const getGuiEnvironment() const { return this->guiEnvironment; }

	///\return The Irrlicht video driver.
	inline irr::video::IVideoDriver* const getVideoDriver() const { return this->videoDriver; }

	inline irr::gui::IGUIFont* const getFont() const { return this->font; }

	inline const irr::f32 getElapsed() const { return this->elapsed; }

	inline irr::scene::ICameraSceneNode* const getCamera() const { return this->camera; }

	inline irr::scene::IMetaTriangleSelector* const getSelector() const { return this->selector; }

	inline const bool isRunning() const { return this->device->run(); }

	//setter

	inline void setCamera(irr::scene::ICameraSceneNode* const camera) { this->camera = camera; }

private:

	///Defines a state for the state machine.
	struct SState
	{
		SState(const irr::c8* name = 0, State<Demo, irr::SEvent>* state = 0) :
			name(name), state(state)
		{

		}

		bool operator<(const SState& other) const
		{
			return strcmp(name, other.name) < 0;
		}

		const irr::c8* name;

		State<Demo, irr::SEvent>* state;
	};

	///Holds all game states.
	irr::core::array<SState> gameStates;

	///A reference to the configuation object
	Configuration* configuration;

	///A pointer to the Irrlicht device.
	irr::IrrlichtDevice* const device;
#ifdef _SOUND
	irrklang::ISoundEngine* const soundEngine;
#endif
	///A pointer to the Irrlicht scene manager.
	irr::scene::ISceneManager* const sceneManager;

	///A pointer to the Irrlicht video driver.
	irr::video::IVideoDriver* const videoDriver;

	///A pointer to the Irrlicht gui environment.
	irr::gui::IGUIEnvironment* const guiEnvironment;

	///Default font.
	irr::gui::IGUIFont* font;

	///A pointer to the Irrlicht timer.
	irr::ITimer* const timer;
	irr::u32 then;
	irr::u32 now;
	irr::f32 elapsed;

	irr::scene::ICameraSceneNode* camera;

	irr::scene::IMetaTriangleSelector* selector;

	void setFont(const irr::c8* const filename);
};

#endif /* DEMO_H_ */
