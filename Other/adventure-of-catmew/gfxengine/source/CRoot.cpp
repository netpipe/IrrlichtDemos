// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "Leeuwen" Library.
// For conditions of distribution and use, see copyright notice in Leeuwen.h

#include "gfx/Error.h"
#include "CRoot.h"
#include "CSceneManager.h"
#include "CVideoDriver.h"

namespace lew
{
	namespace gfx
	{
		CRoot::CRoot(const Vector pDimension, const bool pFullscreen,
		const bool pVsync, const bool pResizeable) : IRoot()
		{
			Dimension = pDimension;
			Fullscreen = pFullscreen;
			Vsync = pVsync;
			Resizeable = pResizeable;

			//SDL goes fullscreen when the resolution is 0,0 ,
			//so if we want a kind of 'null-device', we have to create
			//a window with 1,1 resolution
			if(Dimension == 0)
				Dimension = 1;

			LastTime = -1;
			StopTimer = false;
			Active = true;
			WarpCursor = true;
			MouseReleaseLeft = false;
			MouseReleaseRight = false;
			MouseReleaseMiddle = false;
			ChangedDimension = false;
			GrabInput = false;
			pVideoDriver = 0;
			pSceneManager = 0;
			RenderSurface = 0;


			#if defined(LINUX)
			Platform = "GNU/Linux";
			#elif defined(WINDOWS)
			Platform = "Microsoft Windows";
			#else
			Platform = "Unknown";
			#endif


			SDL_Init(SDL_INIT_EVERYTHING);
			SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			if(Vsync)
			{
				SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
			}


			createWindow();
			KeyState = SDL_GetKeyState(NULL);

			for(int i = 0; i != getJoystickCount(); ++i)
			{
				SDL_Joystick* joystick = SDL_JoystickOpen(i);
				JoystickList.push_back(joystick);

				JoystickMap[i] = joystick;
			}

			pVideoDriver = new CVideoDriver(this);
			pVideoDriver->lock();

			pSceneManager = new CSceneManager(this);
			pSceneManager->lock();
		}

		CRoot::~CRoot()
		{
			if(pSceneManager)
			{
				pSceneManager->unlock();
				pSceneManager->remove();
			}

			if(pVideoDriver)
			{
				pVideoDriver->unlock();
				pVideoDriver->remove();
			}

			for(unsigned int i = 0; i != JoystickList.size(); ++i)
				SDL_JoystickClose(JoystickList[i]);

			SDL_QuitSubSystem(SDL_INIT_EVERYTHING);
			SDL_Quit();
		}

		void CRoot::setWindowTitle(const std::string pWindowTitle)
		{
			WindowTitle = pWindowTitle;

			SDL_WM_SetCaption(WindowTitle.c_str(), 0);
		}

		void CRoot::setCursorVisible(const bool pFlag)
		{
			CursorVisible = pFlag;
			SDL_ShowCursor(CursorVisible);
		}

		void CRoot::setCursorPosition(const Vector pPosition)
		{
			if(WarpCursor)
			{
				SDL_WarpMouse(int(pPosition.x), int(pPosition.y));
			}

			RelativeCursorPosition.x = pPosition.x;
			RelativeCursorPosition.y = Dimension.y - pPosition.y;

			AbsoluteCursorPosition = RelativeCursorPosition +
				pSceneManager->getWorldPosition();

		}

		void CRoot::setGrabInput(const bool pFlag)
		{
			GrabInput = pFlag;

			if(GrabInput)
			{
				SDL_WM_GrabInput(SDL_GRAB_ON);
			}
			else
			{
				SDL_WM_GrabInput(SDL_GRAB_OFF);
			}
		}

		bool CRoot::getKeyPressed(const KeyCode pKeyCode) const
		{
			if(KeyState[pKeyCode] == SDL_PRESSED)
				return true;

			return false;
		}

		bool CRoot::getKeyReleased(const KeyCode pKeyCode) const
		{
			if(KeyState[pKeyCode] == SDL_RELEASED)
				return true;

			return false;
		}

		bool CRoot::getButtonPressed(const ButtonCode pButtonCode) const
		{
			if(SDL_GetMouseState(NULL, NULL) &SDL_BUTTON(pButtonCode))
				return true;

			return false;
		}

		bool CRoot::getButtonReleased(const ButtonCode pButtonCode) const
		{
			switch(pButtonCode)
			{
				case EBC_LEFT:
					return MouseReleaseLeft;
					break;

				case EBC_RIGHT:
					return MouseReleaseRight;
					break;

				case EBC_MIDDLE:
					return MouseReleaseMiddle;
					break;

				default:
					return false;
			}

			return false;
		}

		bool CRoot::getJoystickButtonPressed(const int pJoystickNumber,
		const int pButtonNumber)
		{
			if(SDL_JoystickOpened(pJoystickNumber))
			{
				if(SDL_JoystickGetButton(JoystickMap[pJoystickNumber],
				pButtonNumber) == SDL_PRESSED)
				{
					return true;
				}
			}

			return false;
		}

		bool CRoot::getJoystickButtonReleased(const int pJoystickNumber,
		const int pButtonNumber)
		{
			if(SDL_JoystickOpened(pJoystickNumber))
			{
				if(SDL_JoystickGetButton(JoystickMap[pJoystickNumber],
				pButtonNumber) == SDL_RELEASED)
				{
					return true;
				}
			}

			return false;
		}

		int CRoot::getJoystickAxisPosition(const int pJoystickNumber,
		const int pAxisNumber)
		{
			if(SDL_JoystickOpened(pJoystickNumber))
			{
				return SDL_JoystickGetAxis(JoystickMap[pJoystickNumber],
					pAxisNumber);
			}

			return 0;
		}

		HatCode CRoot::getJoystickHatState(const int pJoystickNumber,
		const int pHatNumber)
		{
			if(SDL_JoystickOpened(pJoystickNumber))
			{
				Uint8 state = SDL_JoystickGetHat(JoystickMap[pJoystickNumber],
					pHatNumber);

				switch(state)
				{
					case SDL_HAT_CENTERED:
						return EHC_CENTERED;
						break;

					case SDL_HAT_UP:
						return EHC_UP;
						break;

					case SDL_HAT_RIGHT:
						return EHC_RIGHT;
						break;

					case SDL_HAT_DOWN:
						return EHC_DOWN;
						break;

					case SDL_HAT_LEFT:
						return EHC_LEFT;
						break;

					case SDL_HAT_RIGHTUP:
						return EHC_RIGHTUP;
						break;

					case SDL_HAT_RIGHTDOWN:
						return EHC_RIGHTDOWN;
						break;

					case SDL_HAT_LEFTUP:
						return EHC_LEFTUP;
						break;

					case SDL_HAT_LEFTDOWN:
						return EHC_LEFTDOWN;
						break;

					default:
						break;
				}
			}

			return EHC_NONE;
		}

		WheelCode CRoot::getWheelState() const
		{
			return WheelState;
		}

		Vector CRoot::getRelativeCursorPosition() const
		{
			return RelativeCursorPosition;
		}

		Vector CRoot::getAbsoluteCursorPosition() const
		{
			return AbsoluteCursorPosition;
		}

		bool CRoot::getCursorVisible() const
		{
			return CursorVisible;
		}

		bool CRoot::getWindowActive() const
		{
			return Active;
		}

		bool CRoot::getFullscreen() const
		{
			return Fullscreen;
		}

		bool CRoot::getVSync() const
		{
			return Vsync;
		}

		bool CRoot::getResizeable() const
		{
			return Resizeable;
		}

		bool CRoot::getChangedDimension() const
		{
			return ChangedDimension;
		}

		bool CRoot::getGrabInput() const
		{
			return GrabInput;
		}

		bool CRoot::getSupportedDimension(const Vector pDimension) const
		{
			if(!Fullscreen)
			{
				if(SDL_VideoModeOK(int(pDimension.x), int(pDimension.y), 32,
				SDL_OPENGL))
				{
					return true;
				}
			}
			else
			{
				if(SDL_VideoModeOK(int(pDimension.x), int(pDimension.y), 32,
				SDL_FULLSCREEN | SDL_OPENGL))
				{
					return true;
				}
			}

			return false;
		}

		Vector CRoot::getDimension() const
		{
			return Dimension;
		}

		std::string CRoot::getWindowTitle() const
		{
			return WindowTitle;
		}

		std::string CRoot::getPlatform() const
		{
			return Platform;
		}

		std::string CRoot::getJoystickName(const int pJoystickNumber) const
		{
			if(SDL_JoystickOpened(pJoystickNumber))
				return SDL_JoystickName(pJoystickNumber);

			return "";
		}

		int CRoot::getTime() const
		{
			if(StopTimer)
				return LastTime;

			return SDL_GetTicks();
		}

		int CRoot::getJoystickAxisCount(const int pJoystickNumber)
		{
			if(SDL_JoystickOpened(pJoystickNumber))
				return SDL_JoystickNumAxes(JoystickMap[pJoystickNumber]);

			return 0;
		}

		int CRoot::getJoystickButtonCount(const int pJoystickNumber)
		{
			if(SDL_JoystickOpened(pJoystickNumber))
				return SDL_JoystickNumButtons(JoystickMap[pJoystickNumber]);

			return 0;
		}

		int CRoot::getJoystickHatCount(const int pJoystickNumber)
		{
			if(SDL_JoystickOpened(pJoystickNumber))
				return SDL_JoystickNumHats(JoystickMap[pJoystickNumber]);

			return 0;
		}

		int CRoot::getJoystickCount() const
		{
			return SDL_NumJoysticks();
		}

		ISceneManager* CRoot::getSceneManager() const
		{
			return pSceneManager;
		}

		IVideoDriver* CRoot::getVideoDriver() const
		{
			return pVideoDriver;
		}

		bool CRoot::loop()
		{
			WheelState = EWC_NONE;
			MouseReleaseLeft = false;
			MouseReleaseRight = false;
			MouseReleaseMiddle = false;
			ChangedDimension = false;

			SDL_Event Event;
			while(SDL_PollEvent(&Event))
			{
				if(Event.type == SDL_ACTIVEEVENT)
				{
					if(Event.active.state & SDL_APPACTIVE)
					{
						if(Event.active.gain == 0)
							Active = false;
						else
							Active = true;
					}
				}
				if(Event.type == SDL_VIDEORESIZE)
				{
					Vector oldDimension = Dimension;
					Dimension.x = Event.resize.w;
					Dimension.y = Event.resize.h;

					createWindow();

					//if viewport is set to use the whole screen
					if(pVideoDriver->getViewPortMin() == Vector(0,0) &&
						pVideoDriver->getViewPortMax() == oldDimension)
					{
						pVideoDriver->resetViewPort();
					}

					ChangedDimension = true;
				}
				if(Event.type == SDL_MOUSEBUTTONUP)
				{
					if(Event.button.button == SDL_BUTTON_LEFT)
						MouseReleaseLeft = true;

					if(Event.button.button == SDL_BUTTON_RIGHT)
						MouseReleaseRight = true;

					if(Event.button.button == SDL_BUTTON_MIDDLE)
						MouseReleaseMiddle = true;
				}
				if(Event.type == SDL_MOUSEBUTTONDOWN)
				{
					if(Event.button.button == SDL_BUTTON_WHEELUP)
						WheelState = EWC_UP;

					if(Event.button.button == SDL_BUTTON_WHEELDOWN)
						WheelState = EWC_DOWN;
				}

				if(Event.type == SDL_QUIT)
					return false;
		    }

			//update key information
			KeyState = SDL_GetKeyState(NULL);

			//get the cursor position
			int X = 0;
			int Y = 0;
			SDL_GetMouseState(&X, &Y);

			//apply it to our variables
			WarpCursor = false;
			setCursorPosition(Vector(X, Y));
			WarpCursor = true;

			//query joystick events
			SDL_JoystickUpdate();

			return true;
		}

		void CRoot::sleep(const int pMilliseconds) const
		{
			SDL_Delay(pMilliseconds);
		}

		void CRoot::startTimer()
		{
			StopTimer = false;
		}

		void CRoot::stopTimer()
		{
			LastTime = getTime();
			StopTimer = true;
		}

		void CRoot::createWindow()
		{
			if(!Fullscreen)
			{
				if(Resizeable)
				{
					RenderSurface = SDL_SetVideoMode((int)Dimension.x,
						(int)Dimension.y, 32, SDL_OPENGL| SDL_RESIZABLE);
				}
				else
				{
					RenderSurface = SDL_SetVideoMode((int)Dimension.x,
						(int)Dimension.y, 32, SDL_OPENGL);
				}
			}
			else
			{
				RenderSurface = SDL_SetVideoMode((int)Dimension.x,
					(int)Dimension.y, 32, SDL_FULLSCREEN | SDL_OPENGL);
			}

			if(!RenderSurface)
			{
				//VIDEO MODE NOT SUPPORTED
				throw(err::VideoMode(Dimension, Fullscreen));
			}
		}
	};
};

