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

#ifndef _GRAPHICSENGINE_H_
#define _GRAPHICSENGINE_H_

#include "core/Keycode.h"

#include <string>

namespace peak
{
	class SceneNode;
	
	class GraphicsEngine
	{
		public:
			/**
			 * \brief Returns a pointer to the GraphicsEngine class.
			 * \return pointer to the GraphicsEngine class.
			 */
			static GraphicsEngine * get(void);

			/**
			 * \brief Initiates the graphics engine and creates a render window.
			 * \return false if an error occured.
			 */
			bool init(void);

			/**
			 * \brief Shuts down the graphics engine.
			 * \return false if not everything could be cleaned up.
			 */
			bool shutdown(void);
			/**
			 * \brief Render one frame.
			 * \return false if the window was closed or any other error occured.
			 */
			bool renderFrame(float msecs);
			
			/**
			 * \brief Saves all unsaved log messages to the log file.
			 */
			void saveMessages(void);
			
			SceneNode *getDefaultCamera(void);
			
			void setKeyCallback(void (*cb)(Keycode key, bool pressed));
		private:
			GraphicsEngine();
			
			int camera;
			SceneNode *cameranode;
			
			int lastfps;
			float timesincefps;
			std::string caption;
	};
}

#endif

