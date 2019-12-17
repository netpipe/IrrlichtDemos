#pragma once

#include <irrlicht.h>

class CWorld;
class CCar;
class CEventReceiver;
class CGUI;

class CGameContext : public irr::IEventReceiver
{
public:
	CGameContext(irr::IrrlichtDevice * device);
	virtual ~CGameContext(void);

	void gameStep();

	void startGame(const irr::io::path &level, const irr::io::path &car);
	void continueGame();
	void mainMenu();
	void pause();
	void exit();

protected:	
	void animatePlayer(CCar * car);
	void animateCamera(irr::scene::ICameraSceneNode * camera, CCar * car);
	
	void clear();

	void resetCar();

	virtual bool OnEvent(const irr::SEvent& event);

	virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const {
		return m_key_is_down[keyCode];
	}
private:
	enum {
		STATE_MAIN_MENU,		//! renders world & main menu
		STATE_PAUSE_MENU,		//! renders paused game view & pause menu
		STATE_PLAYGING,			//! world & car
		STATE_CHOOSING_CAR,		//! renders world and selected car
		STATE_CHOOSING_RACE		//! renders world and race map
	} m_state;					//! current state

	CGUI						*m_gui;

	irr::io::path				m_selected_race_path,	
								m_selected_car_path;		

	irr::IrrlichtDevice			*m_device;
	irr::video::IVideoDriver	*m_driver;
	irr::scene::ISceneManager	*m_smgr;
	irr::gui::IGUIEnvironment	*m_guienv;

	irr::scene::ICameraSceneNode *m_camera;		

	CWorld						*m_world;		//! world instance

	irr::core::list<CCar*>		m_npc_cars;			
	CCar						*m_player_car;		
	
	bool						m_key_is_down[irr::KEY_KEY_CODES_COUNT];	
};

