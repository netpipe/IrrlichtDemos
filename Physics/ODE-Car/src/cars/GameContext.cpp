#include "GameContext.h"

#include "World.h"
#include "Car.h"
#include "GUI.h"

using namespace irr;
using namespace core;
using namespace gui;


CGameContext::CGameContext(irr::IrrlichtDevice * device) : m_device(device), m_player_car(NULL)
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
		m_key_is_down[i] = false;

	device->setEventReceiver(this);

	m_gui = new CGUI(device);

	m_device->grab();

	m_driver = m_device->getVideoDriver();
	m_smgr = m_device->getSceneManager();
	m_guienv = m_device->getGUIEnvironment();

	m_camera = m_smgr->addCameraSceneNode();

	m_world = CWorld::create(m_smgr, device->getFileSystem());
	m_world->loadScene("./races/street.xml");
}

CGameContext::~CGameContext(void)
{
	if (m_player_car)
		m_player_car->drop();

	m_world->drop();
	m_device->drop();
	delete m_gui;
}

void CGameContext::gameStep()
{
	if (m_state == STATE_PLAYGING)
	{
		animatePlayer(m_player_car);
		animateCamera(m_camera, m_player_car);
	}

	m_driver->beginScene(true, true, video::SColor(255,100,101,140));

	m_smgr->drawAll();

	if (m_state != STATE_PLAYGING)
		m_gui->drawAll();

	m_driver->endScene();

	m_gui->showFPS(m_driver->getFPS());
}

void CGameContext::startGame(const irr::io::path &level, const irr::io::path &car)
{
	clear();

	//m_world->loadScene(level);
	//m_player_car = m_world->createCar(car, vector3df(0.0f, 2.0f, 0.0f));
    m_world->loadScene("./races/street.xml");
	m_player_car = m_world->createCar("./cars/car5.xml", vector3df(0.0f, 2.0f, 0.0f));
	m_world->startSimulation();
	m_state = STATE_PLAYGING;
}

void CGameContext::continueGame()
{
	m_world->startSimulation();
	m_state = STATE_PLAYGING;
}

void CGameContext::mainMenu()
{
	clear();

	m_camera->setPosition(m_world->getDefaultCameraPos());
	m_camera->setTarget(m_world->getDefaultCameraTarget());

	m_gui->showMainMenu();
	m_world->stopSimulation();

	m_state = STATE_MAIN_MENU;
}

void CGameContext::pause()
{
	m_world->stopSimulation();
	m_gui->showPauseMenu();
	m_state = STATE_PAUSE_MENU;
}

void CGameContext::exit()
{
	m_device->closeDevice();
}

void CGameContext::animatePlayer(CCar * car)
{
	// process keyboard clicks
	if (IsKeyDown(irr::KEY_KEY_W))
	{
		car->accelerate();
	}
	if (IsKeyDown(irr::KEY_KEY_S))
	{
		car->slowdown();
	}
	if (IsKeyDown(irr::KEY_KEY_A))
	{
		car->turnLeft();
	}
	if (IsKeyDown(irr::KEY_KEY_D))
	{
		car->turnRight();
	}
	if (IsKeyDown(irr::KEY_KEY_R))
	{
		resetCar();
	}
}

void CGameContext::animateCamera(irr::scene::ICameraSceneNode * camera, CCar * car)
{
	if (!car) return;

	const matrix4 &car_transform = car->getChassis()->getAbsoluteTransformation();

	vector3df new_camera_position,
				car_target(0.0f, 0.0f, -2.0f),
				behind_car(0.0f, 0.0f, 3.0f);
	car_transform.transformVect(car_target);
	car_transform.transformVect(behind_car);

	new_camera_position = car_target + (behind_car - car->getPosition()) * 2.0f + vector3df(0.0f, 1.0f, 0.0f);

	camera->setPosition(new_camera_position);
	camera->setTarget(car->getPosition());
}

bool CGameContext::OnEvent(const SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		m_key_is_down[event.KeyInput.Key] = event.KeyInput.PressedDown;

		if (event.KeyInput.Key == irr::KEY_ESCAPE && event.KeyInput.PressedDown)
		{
			if (m_state == STATE_PLAYGING)
				pause();
			else if (m_state == STATE_PAUSE_MENU)
				continueGame();
		}
	}
	else
	if (event.EventType == irr::EET_GUI_EVENT)
	{
		s32 id = event.GUIEvent.Caller->getID();

		switch (event.GUIEvent.EventType)
		{
		case EGET_LISTBOX_CHANGED:
			if (m_state == STATE_CHOOSING_CAR)
				m_selected_car_path = m_gui->getSelectedFile();
			else if (m_state == STATE_CHOOSING_RACE)
				m_selected_race_path = m_gui->getSelectedFile();

			break;

		case EGET_BUTTON_CLICKED:
			switch (id)
			{
			case CMD_ID_EXIT_BUTTON:
				m_device->closeDevice();
				return true;

			case CMD_ID_CONTINUE_BUTTON:
				continueGame();
				m_state = STATE_PLAYGING;
				return true;

			case CMD_ID_EXIT_MAIN_MENU_BUTTON:
				mainMenu();
				return true;

			case CMD_ID_CHOOSE_CAR_BUTTON:
				m_gui->showCarBrowser();
				m_state = STATE_CHOOSING_CAR;
				return true;

			case CMD_ID_START_BUTTON:
			case CMD_ID_CHOOSE_RACE_BUTTON:
				m_gui->showRaceBrowser();
				m_state = STATE_CHOOSING_RACE;
				return true;

			case CMD_ID_PLAY_BUTTON:
				startGame(m_selected_race_path, m_selected_car_path);
				return true;

			default:
				return false;
			}

			break;
		}
	}

	return false;
}

void CGameContext::clear()
{
	if (m_player_car)
	{
		m_player_car->drop();
		m_player_car = 0;
	}

	irr::core::list<CCar*>::Iterator it = m_npc_cars.begin();
	while (it != m_npc_cars.end())
	{
		(*it)->drop();

		it++;
	}
}

void CGameContext::resetCar()
{
	m_player_car->reset();
	m_player_car->setPosition(vector3df(0.0f, 2.0f, 0.0f));
}
