// *************************************************************************************************
//
// Horde3D
//   Next-Generation Graphics Engine
// --------------------------------------
// Copyright (C) 2006-2008 Nicolas Schulz
//
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#ifndef _egModules_H_
#define _egModules_H_

#include "egPrerequisites.h"
#include "egCom.h"
#include "egScene.h"
#include "egResource.h"
#include "egRenderer.h"
#include "egPipeline.h"
#include "egExtensions.h"


extern const char *versionString;


class Modules
{
private:

	static EngineConfig		*_engineConfig;
	static EngineLog		*_engineLog;
	static SceneManager		*_sceneManager;
	static ResourceManager	*_resourceManager;
	static Renderer			*_renderer;
	static ExtensionManager	*_extensionManager;

public:

	static void init();
	static void release();
	
	static EngineConfig &config() { return *_engineConfig; }
	static EngineLog &log() { return *_engineLog; }
	static SceneManager &sceneMan() { return *_sceneManager; }
	static ResourceManager &resMan() { return *_resourceManager; }
	static Renderer &renderer() { return *_renderer; }
	static ExtensionManager &extMan() { return *_extensionManager; }
};

#endif // _egModules_H_
