/*
Copyright (C) 2009  Mathias Gottschlag

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

//#include "core/Config.h"

#include <string>

#if defined(_MSC_VER) || defined(_WINDOWS_) || defined(_WIN32)
   #include <time.h>
   #include <winsock2.h>
   #include <winbase.h>
   #define usleep(x) Sleep(x/1000)
#else
   #include <sys/time.h>
#endif

/**
 * \brief Namespace which holds all the classes of the engine
 */
namespace peak
{
	/**
	 * \brief Main game engine class. Calls the other parts of the engine.
	 */
	class GameEngine
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

