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

#include "egModules.h"
#include "utDebug.h"

const char *versionString = "Horde3D 1.0.0 Beta1";

EngineConfig		*Modules::_engineConfig = 0x0;
EngineLog			*Modules::_engineLog = 0x0;
SceneManager		*Modules::_sceneManager = 0x0;
ResourceManager		*Modules::_resourceManager = 0x0;
Renderer			*Modules::_renderer = 0x0;
ExtensionManager	*Modules::_extensionManager = 0x0;


void Modules::init()
{
	if( _extensionManager == 0x0 ) _extensionManager = new ExtensionManager();
	if( _sceneManager == 0x0 ) _sceneManager = new SceneManager();
	if( _resourceManager == 0x0 ) _resourceManager = new ResourceManager();
	if( _renderer == 0x0 ) _renderer = new Renderer();
	if( _engineLog == 0x0 ) _engineLog = new EngineLog();
	if( _engineConfig == 0x0 ) _engineConfig = new EngineConfig();
}


void Modules::release()
{
	// Remove overlays since they reference resources and resource manager is removed before renderer
	if( _renderer ) _renderer->clearOverlays();
	
	// Order of destruction is important
	delete _extensionManager; _extensionManager = 0x0;
	delete _sceneManager; _sceneManager = 0x0;
	delete _resourceManager; _resourceManager = 0x0;
	delete _renderer; _renderer = 0x0;
	delete _engineLog; _engineLog = 0x0;
	delete _engineConfig; _engineConfig = 0x0;
}
