// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _CRootGfxHeader_
#define _CRootGfxHeader_

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_main.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

#include "gfx/IRoot.h"
#include "gfx/ISceneManager.h"
#include "gfx/IVideoDriver.h"

namespace lew
{
	namespace gfx
	{
		class CRoot : public IRoot
		{
			public:
				CRoot(const Vector pDimension, const bool pFullscreen,
					const bool pVsync, const bool pResizeable);

				~CRoot();

				void setWindowTitle(const std::string pWindowTitle);
				void setCursorVisible(const bool pFlag);
				void setCursorPosition(const Vector pPosition);
				void setGrabInput(const bool pFlag);

				ISceneManager* getSceneManager() const;
				IVideoDriver* getVideoDriver() const;
				Vector getDimension() const;
				Vector getRelativeCursorPosition() const;
				Vector getAbsoluteCursorPosition() const;
				std::string getWindowTitle() const;
				std::string getPlatform() const;
				std::string getJoystickName(const int pJoystickNumber) const;
				int getTime() const;
				int getJoystickCount() const;
				int getJoystickAxisCount(const int pJoystickNumber);
				int getJoystickButtonCount(const int pJoystickNumber);
				int getJoystickHatCount(const int pJoystickNumber);
				bool getCursorVisible() const;
				bool getWindowActive() const;
				bool getFullscreen() const;
				bool getResizeable() const;
				bool getChangedDimension() const;
				bool getSupportedDimension(const Vector pDimension) const;
				bool getVSync() const;
				bool getGrabInput() const;
				bool getButtonReleased(const ButtonCode pButtonCode) const;
				bool getButtonPressed(const ButtonCode pButtonCode) const;
				bool getKeyReleased(const KeyCode pKeyCode) const;
				bool getKeyPressed(const KeyCode pKeyCode) const;
				WheelCode getWheelState() const;

				HatCode getJoystickHatState(const int pJoystickNumber,
					const int pHatNumber);

				bool getJoystickButtonPressed(const int pJoystickNumber,
					const int pButtonNumber);

				bool getJoystickButtonReleased(const int pJoystickNumber,
					const int pButtonNumber);

				int getJoystickAxisPosition(const int pJoystickNumber,
					const int pAxisNumber);

				bool loop();
				void startTimer();
				void stopTimer();
				void sleep(const int pMilliseconds) const;


			private:
				void createWindow();

				Vector Dimension;

				ISceneManager* pSceneManager;
				IVideoDriver* pVideoDriver;

				SDL_Surface* RenderSurface;
				std::string WindowTitle;
				std::string Platform;

				int LastTime;

				bool StopTimer;
				bool Active;
				bool Fullscreen;
				bool Vsync;
				bool Resizeable;
				bool ChangedDimension;
				bool GrabInput;

				Vector RelativeCursorPosition;
				Vector AbsoluteCursorPosition;

				WheelCode WheelState;
				bool CursorVisible;
				bool WarpCursor;

				bool MouseReleaseLeft;
				bool MouseReleaseRight;
				bool MouseReleaseMiddle;
				Uint8* KeyState;

				std::vector<SDL_Joystick*> JoystickList;
				std::map<int, SDL_Joystick*> JoystickMap;
		};
	};
};

#endif

