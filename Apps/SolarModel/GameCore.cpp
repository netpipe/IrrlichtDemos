#include "GameGlobals.h"


GameStruct Game;


scene::ICameraSceneNode* GameFPSCamera(const core::vector3df & position, float rotate_speed, float move_speed, scene::ISceneNode* parent)
{
	SKeyMap keyMap[8];

	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_UP;
	keyMap[1].Action = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;

	keyMap[2].Action = EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = KEY_DOWN;
	keyMap[3].Action = EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = KEY_KEY_S;

	keyMap[4].Action = EKA_STRAFE_LEFT;
	keyMap[4].KeyCode = KEY_LEFT;
	keyMap[5].Action = EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = KEY_KEY_A;

	keyMap[6].Action = EKA_STRAFE_RIGHT;
	keyMap[6].KeyCode = KEY_RIGHT;
	keyMap[7].Action = EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = KEY_KEY_D;

	scene::ICameraSceneNode* cam =
		Game.Scene->addCameraSceneNodeFPS(
			parent, rotate_speed, move_speed, -1, keyMap, sizeof(keyMap), false);

	cam->setPosition(position);

	return cam;
}


void GameLog(char *str, ...)
{
	va_list args;
	char buf[4096];

	va_start(args, str);
	vsprintf(buf, str, args);
	va_end(args);

	if (Game.Logger) Game.Logger->log(buf);
	else printf("%s\n", buf);
}


bool GameScreenshot(GameStruct *pGame, core::stringc &filename)
{
	filename = pGame->FS->getWorkingDirectory();

#ifdef _IRR_WINDOWS_
	filename += "\\";
#else
	filename += "/";
#endif

	filename += Game.Timer->getRealTime();
	filename += "-";
	filename += (rand()%9000) + 1000; // random 4 digit value
	filename += ".jpg";

	if (pGame->ScreenshotWithoutGUI)
	{
		pGame->Device->run();

		pGame->Video->beginScene(Game.ClearBackBuffer, Game.ClearZBuffer, Game.ClearColor);
		pGame->Scene->drawAll();
		GamePost();
		pGame->Video->endScene();
	}

	return pGame->Video->writeImageToFile(
		pGame->Video->createScreenShot(),
		filename.c_str(),
		pGame->ScreenshotQuality);
}


bool GameIsMaterialSupported(video::E_MATERIAL_TYPE material)
{
	video::IMaterialRenderer* mr = Game.Video->getMaterialRenderer(material);
	return mr && !mr->getRenderCapability();
}


void GameCheckCompatibilities()
{
	switch (Game.PerPixelLighting)
	{
	case 0:
		// not using per pixel lighting, so nothing to check
		break;

	case 1:
		if (!GameIsMaterialSupported(video::EMT_NORMAL_MAP_SOLID))
		{
			GameLog("WARNING: Bump mapping not supported :(");
		}
		break;

	case 2:
		if (!GameIsMaterialSupported(video::EMT_PARALLAX_MAP_SOLID))
		{
			GameLog("WARNING: Parallax mapping not supported :(");
			return;
		}
		break;

	default:
		// any other value just ignoring
		break;
	}
}


void GameCreateIrrLicht(GameStruct *pGame)
{
	pGame->Device = createDevice(
		pGame->DriverType,
		core::dimension2du( pGame->Width, pGame->Height ),
		pGame->BitsPerPixel,
		pGame->Fullscreen,
		false, // Game.StencilBuffer
		pGame->Vsync,
		pGame->EventReceiver
	);

	pGame->Scene = pGame->Device->getSceneManager();
	pGame->SceneCollision = pGame->Scene->getSceneCollisionManager();
	pGame->MeshManipulator = pGame->Scene->getMeshManipulator();
	pGame->Video = pGame->Device->getVideoDriver();
	pGame->Logger = pGame->Device->getLogger();
	pGame->Timer = pGame->Device->getTimer();
	pGame->GUI = pGame->Device->getGUIEnvironment();
	pGame->FS = pGame->Device->getFileSystem();
	pGame->OS = pGame->Device->getOSOperator();
	pGame->Cursor = pGame->Device->getCursorControl();
	pGame->Skin = pGame->GUI->getSkin();
}


void GameDefaults(GameStruct *pGame)
{
	memset((void*)pGame, 0, sizeof(*pGame));

	pGame->Width = 800;
	pGame->Height = 600;
	pGame->BitsPerPixel = 32;
	pGame->Fullscreen = false;

	pGame->AntiAlias = false;
	pGame->ClearZBuffer = true;
	pGame->ClearBackBuffer = true;
	pGame->BackgroundRendering = false;
	pGame->ClearColor = video::SColor(255,100,120,150);
	pGame->Vsync = true;
	pGame->PerPixelLighting = 0;
	pGame->ScreenshotQuality = 100;

	pGame->Device = NULL;
	pGame->Video = NULL;
	pGame->DriverType = video::EDT_OPENGL;
	pGame->GUI = NULL;
	pGame->Logger = NULL;
	pGame->Scene = NULL;
	pGame->SceneCollision = NULL;
	pGame->MeshManipulator = NULL;
	pGame->Timer = NULL;
	pGame->FS = NULL;
	pGame->OS = NULL;
	pGame->Cursor = NULL;
	pGame->Skin = NULL;

	pGame->EventReceiver = NULL;
	pGame->Map = NULL;
}


int main()
{
	GameDefaults(&Game);

	GameSetup();

	Game.EventReceiver = new GameEventReceiver();
	Game.Map = new GameMap();

	GameCreateIrrLicht(&Game);
	GameCheckCompatibilities();

	GameInit();

	while (Game.Device->run())
	{
		if (Game.Device->isWindowActive() || Game.BackgroundRendering)
		{
			Game.Video->beginScene(
				Game.ClearBackBuffer,
				Game.ClearZBuffer,
				Game.ClearColor
			);

			if (GameRun())
			{
				Game.Scene->drawAll();
				GamePost();
				Game.GUI->drawAll();
				Game.Video->endScene();
			}
			else
			{
				break;
			}
		}
		else
		{
			Game.Device->yield();
		}
	}

	GameDone();

	if (Game.Map) delete Game.Map;
	if (Game.EventReceiver) delete Game.EventReceiver;

	Game.Device->closeDevice();
	Game.Device->drop();

	return 0;
}
