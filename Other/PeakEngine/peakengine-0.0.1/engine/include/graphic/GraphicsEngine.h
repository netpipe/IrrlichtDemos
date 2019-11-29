/*
Copyright (C) 2008   Lukas Kropatschek

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

#include <string>

#ifdef _USE_IRRLICHT_
namespace irr
{
	class IrrlichtDevice;
	namespace scene
	{
		class ISceneManager;
	}
	namespace video
	{
		class IVideoDriver;
	}
	namespace gui
	{
		class ICursorControl;
		class IGUIEnvironment;
	}
}
#endif

//tolua_begin
namespace peak
{
	class CameraSceneNode;
	
	/**
	 * \brief Class used for 3D graphics and scene management.
	 */
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
			
			CameraSceneNode *getStandardCamera(void);
			
			//tolua_end
			#ifdef _USE_IRRLICHT_
			
			/**
			 * \brief Returns the Irrlicht scene manager.
			 * \return pointer to the Irrlicht scene manager class.
			 */
			irr::scene::ISceneManager *getSceneManager(void);
			/**
			 * \brief Returns the Irrlicht video driver.
			 * \return pointer to the Irrlicht video driver class.
			 */
			irr::video::IVideoDriver *getDriver(void);
			/**
			 * \brief Returns the Irrlicht GUI environment.
			 * \return pointer to the Irrlicht GUIEnvironment class.
			 */
			irr::gui::IGUIEnvironment *getGUI(void);
			/**
			 * \brief Returns the Irrlicht cursor control.
			 */
			irr::gui::ICursorControl *getCursorControl(void);
			#endif
			
		private:
			GraphicsEngine();
			
			#ifdef _USE_IRRLICHT_
			irr::scene::ISceneManager *scenemgr;
			irr::IrrlichtDevice *device;
			irr::video::IVideoDriver *driver;
			irr::gui::IGUIEnvironment *guienv;
			std::wstring caption;
			CameraSceneNode *stdcam;
			#endif
			int lastfps;
			//tolua_begin
	};

}
//tolua_end

#endif /*GRAPHICSENGINE_H_*/

