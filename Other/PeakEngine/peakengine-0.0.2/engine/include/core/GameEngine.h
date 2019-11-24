/*
Copyright (C) 2008  Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the
Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_

#include "core/Config.h"

#include <string>

#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
   #include <time.h>
   #include <winsock2.h>
   #include <winbase.h>
   #define usleep(x) Sleep(x/1000)

#else
   #include <sys/time.h>
#endif
#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
   int gettimeofday(struct timeval* tv, void *timezone);
#endif

namespace peak {
/**
 * \mainpage PeakEngine
 *
 * \section about About
 *
 * The PeakEngine is a multiplatform, scriptable, easy-to use game engine with
 * shader-based graphics, automatic networking, 3D sound and networked physics.
 * The engine is completely controlled in Lua and XML. It is based on Horde3D,
 * FreeImagePlus, guichan, Bullet, enet, tolua++, Lua, glfw, OpenAL, freealut,
 * libogg and libvorbis.
 *
 * \section design Design
 * \subsection overview Overview
 * The engine is controlled completely through Lua and XML, with the possibility
 * of binary plugins being planned in the near future. The game logic itself is
 * splitted up into two parts: The level (which mostly holds static objects),
 * and entities which are all dynamic objects.
 * \subsection entity Entity
 * Entities can contain physics, 2D and 3D graphics and are defined in a XML file
 * like this (very simple example which just creates a cube with physics):
 * \code
 * <?xml version="1.0" standalone="yes"?>
 * <entity>
 * 	<name>Test</name>
 * 	<property name="position" type="alias" value="testbody.position" />
 * 	
 * 	<body name="testbody">
 * 		<shape name="testshape" type="box" size="1/1/1" />
 * 		<scenenode name="testnode" type="mesh" file="cube.scene.xml" scale="1/1/1" />
 * 	</body>
 * 	
 * 	<link type="direct" source="testbody.rotation.y" target="global.compass" />
 * </entity>
 * \endcode
 * This creates a cube which has a physics body ("testbody"), a collision shape
 * ("testshape") and a scene node ("testnode").
 *
 * Additionally Entities and all other objects derived from GameObject can have
 * properties which can be used via their name. In this case only "position" is
 * defined, liked to the position of the body, but it is possible to define
 * other properties. Example for a simple integer property:
 * \code
 * <property name="hitpoints" type="integer" value="100" />
 * \endcode
 * The properties which belong to entities are synchronized automatically via
 * the network, this does not apply to other game objects, like SceneNode.
 *
 * Additionally one can create links between properties so that one property
 * automatically gets changed whenever another changes. Links can also be created
 * between a Property and a script function ("Call script whenever property "x"
 * changes") or an Event and a script function ("Call script whenever
 * input.KeyDown is triggered").
 *
 * The following is an example for the latter:
 * \code
 * <link name="zlink" type="script" source="input.KeyPressed" filter="FORWARD/BACK" place="server">
 * 	local speed = (FORWARD - BACK) * 5;
 * 	velocity = testbody:getLinearVelocity();
 * 	velocity.z = speed;
 * 	testbody:setLinearVelocity();
 * </link>
 * \endcode
 * \subsection networking Networking
 * The engine provides a at the moment still rather simple client-server networking
 * system. Physics and entities are automatically updated, the same applies to
 * links which for example are triggered by a client input event, but execute a
 * server script.
 *
 * This software is still in its early stages of development, so there might be
 * and possible are still security leaks in it. Also it doesn't yet have network
 * encryption or compression and also still breaks quickly when it runs out of
 * bandwidth.
 * \subsection examples Examples
 * There exist 2 examples for the engine: There is a simple test for some functions
 * of it in tests/test1, and there is a simple helicopter game in tests/gunships
 * which shows how to write a simple game using the engine.
 */
}

/**
 * \brief Namespace which holds all the classes of the engine
 */
namespace peak
{
	/**
	 * \brief Main game engine class. Calls the other parts of the engine.
	 */
	class PEAK_API GameEngine
	{
		public:
			/**
			 * \brief Returns pointer to the GameEngine class.
			 *
			 * \return Pointer to the game engine
			 */
			static GameEngine *get(void);
			
			/**
			 * \brief Initializes the other parts of the engine and starts the game.
			 *
			 * Returns when the engine has been closed.
			 * \param root Path to the data of the game
			 * \return Returns false if there have been errors, else returns true.
			 */
			bool run(std::string root);
			
			/**
			 * \brief Stops the game forcefully at the next frame
			 */
			void stopGame(void);
			
			/**
			 * \brief Sets the root directory.
			 *
			 * Do not use this function!
			 */
			void setRootDirectory(std::string dir);
			/**
			 * \brief Returns the root directory specified by run().
			 *
			 * \return Engine's root directory
			 */
			std::string getRootDirectory(void);
		private:
			GameEngine();
			
			std::string rootdir;
			
			bool stopengine;

			double lastframe;
	};
}

#endif

