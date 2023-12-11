#ifndef __COSMIC_GAMEGLOBALS_H_INCLUDED__
#define __COSMIC_GAMEGLOBALS_H_INCLUDED__


// Windows-only defs
#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64) || defined(_WIN32_WCE)

// No "deprecate" warnings for MS VS2005
#define _CRT_SECURE_NO_DEPRECATE

// No console for Release build
#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#endif // end of Windows-only defs


#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <irrlicht.h>


using namespace irr;


#pragma comment(lib, "irrlicht.lib")


enum GameObjectTypeEnum {
	GAME_OBJECT_TYPE_UNKNOWN,
	GAME_OBJECT_TYPE_LIGHT,
	GAME_OBJECT_TYPE_BILLBOARD,
	GAME_OBJECT_TYPE_ANIMATEDMESH,
	GAME_OBJECT_TYPE_SKYBOX,
	GAME_OBJECT_TYPE_POINTCLOUD,
	GAME_OBJECT_TYPE_MESH
};


#include "GameObject.h"
#include "GameLightObject.h"
#include "GameSkyboxObject.h"
#include "GameBillboardObject.h"
#include "GamePointcloudObject.h"
#include "GameAnimatedMeshObject.h"
#include "GameMeshObject.h"
#include "GameMapDescription.h"
#include "GameMap.h"
#include "GameEventReceiver.h"
#include "GameCore.h"
#include "GameConfig.h"
#include "GameMeasurer.h"


#endif
