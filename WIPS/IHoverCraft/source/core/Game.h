#ifndef GAME_H_
#define GAME_H_

#include <IrrlichtDevice.h>
#ifdef _SOUND
#include <irrKlang.h>
#endif

#include "../statemanager/StateManager.h"
#include "Configuration.h"
#include "../physics/Physics.h"
#include "../scene/WeatherSystem.h"
#include "WeaponSystem.h"
#include <irrlicht.h>

class GameObject;

// these macros will return the scaled value based on current screen size
// (i assume 1024x768 is the normal res) to provide a resolution independent gui.
#define SX(val) ( (irr::s32)(game->getVideoDriver()->getScreenSize().Width*(irr::f32)(val)/1024.0f) )
#define SY(val) ( (irr::s32)(game->getVideoDriver()->getScreenSize().Height*(irr::f32)(val)/768.0f) )

class Game : public irr::IEventReceiver, public StateManager<Game, irr::SEvent>
{

private:

	///A reference to the configuration object created in the Application object.
	Configuration& configuration;

	///A pointer to the Irrlicht device.
	irr::IrrlichtDevice* device;

	///A pointer to the Irrlicht scene manager.
	irr::scene::ISceneManager* sceneManager;

	///A pointer to the Irrlicht video driver.
	irr::video::IVideoDriver* videoDriver;

	///A pointer to the Irrlicht gui environment.
	irr::gui::IGUIEnvironment* guiEnvironment;
#ifdef _SOUND
	///A pointer to the irrKlang sound engine.
	irrklang::ISoundEngine* soundEngine;
#endif
	///Default font.
	irr::gui::IGUIFont* font;

	//Timer
	irr::ITimer* timer;
	irr::u32 then;
	irr::u32 now;
	irr::f32 elapsed;

	///Defines a state for the state machine.
	struct SState
	{
//		SState(const irr::c8* name = 0, State<Game, irr::SEvent>* state = 0) :
//			name(name),
//			State(state)
//		{
//		}

		bool operator<(const SState& other) const
		{
			return strcmp(name, other.name) < 0;
		}

		const irr::c8* name;

//		State<Game, irr::SEvent>* State;
	};

	///Holds all game states.
	irr::core::array<SState> gameStates;


	//3d stuff
	WeatherSystem* weatherSystem;

	///The irrlicht camera. All states use this one. You can call it 'the global camera'.
	irr::scene::ICameraSceneNode* camera;


	irr::u32 playerHealth;

	Physics* physics;

	WeaponSystem* weaponSystem;

public:

	Game(Configuration& configuration, irr::IrrlichtDevice* device
#ifdef _SOUND
		, irrklang::ISoundEngine* soundEngine
#endif
	);

	~Game();

	///Adapt irrlicht event handling to ours.
	virtual bool OnEvent(const irr::SEvent& event)
	{
		return onEvent(event);
	}

	const void addGameState(const char* name, State<Game, irr::SEvent>* state);

	/**
	 * Finds a game state by its name.
	 * \return The game state. 0 if no such game state exits.
	 */
	State<Game, irr::SEvent>* findGameState(const char* name);

	///\return The Configuration object.
	inline Configuration& getConfiguration() const { return this->configuration; }

	///\return The Irrlicht device.
	inline irr::IrrlichtDevice* getDevice() const { return this->device; }

	///\return The Irrlicht scene manager.
	inline irr::scene::ISceneManager* getSceneManager() const { return this->sceneManager; }

	///\return The Irrlicht gui environment.
	inline irr::gui::IGUIEnvironment* getGuiEnvironment() const { return this->guiEnvironment; }

	///\return The Irrlicht video driver.
	inline irr::video::IVideoDriver* getVideoDriver() const { return this->videoDriver; }
#ifdef _SOUND
	///\return A pointer to the irrKlang engine.
	inline irrklang::ISoundEngine* getSoundEngine() const { return this->soundEngine; }
#endif
	inline WeatherSystem* getWeatherSystem() const { return this->weatherSystem; }

	inline irr::scene::ICameraSceneNode* getCamera() const { return this->camera; }

	inline WeaponSystem* getWeaponSystem() const { return this->weaponSystem; }

	inline irr::u32 getPlayerHealth() const { return this->playerHealth; }

	inline void setPlayerHealth(const irr::u32 newHealth)
	{
		if (newHealth <= 0)
		{
			//die
			return;
		}

		this->playerHealth = newHealth;
	}


	inline Physics* getPhysics() const { return this->physics; }

	inline const bool isRunning() const { return this->device->run(); }

	const void tick();

	inline const irr::f32 getElapsed() const { return this->elapsed; }

	const void makeScreenshot() const;

	const void setFont(const irr::c8* filename);

	///Does what the name says.
	irr::video::ITexture* scaleTexture(irr::video::ITexture* SrcTexture, const irr::core::dimension2di& destSize);
};

#endif /*GAME_H_*/
