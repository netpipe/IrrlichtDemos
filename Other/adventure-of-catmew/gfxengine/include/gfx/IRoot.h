// Copyright (C) 2007 Kevin van Leeuwen
// This file is part of the "LeeuwenGfx" Library.
// For conditions of distribution and use, see copyright notice in LeeuwenGfx.h

#ifndef _IRootGfxHeader_
#define _IRootGfxHeader_

#include <string>

#include "gfx/Reference.h"
#include "gfx/Vector.h"
#include "gfx/KeyCodes.h"

namespace lew
{
	namespace gfx
	{
		class ISceneManager;
		class IVideoDriver;
		class IRoot : public Reference
		{
			public:
				IRoot() : Reference() {}

				virtual ~IRoot() {}

				virtual void setWindowTitle(const std::string pWindowTitle) = 0;
				virtual void setCursorVisible(const bool pFlag) = 0;
				virtual void setCursorPosition(const Vector pPosition) = 0;
				virtual void setGrabInput(const bool pFlag) = 0;

				virtual ISceneManager* getSceneManager() const = 0;
				virtual IVideoDriver* getVideoDriver() const = 0;
				virtual Vector getDimension() const = 0;
				virtual Vector getRelativeCursorPosition() const = 0;
				virtual Vector getAbsoluteCursorPosition() const = 0;
				virtual std::string getWindowTitle() const = 0;
				virtual std::string getPlatform() const = 0;
				virtual std::string getJoystickName(const int pJoystickNumber) const = 0;
				virtual int getTime() const = 0;
				virtual int getJoystickCount() const = 0;
				virtual int getJoystickAxisCount(const int pJoystickNumber) = 0;
				virtual int getJoystickButtonCount(const int pJoystickNumber) = 0;
				virtual int getJoystickHatCount(const int pJoystickNumber) = 0;
				virtual bool getCursorVisible() const = 0;
				virtual bool getWindowActive() const = 0;
				virtual bool getFullscreen() const = 0;
				virtual bool getVSync() const = 0;
				virtual bool getResizeable() const = 0;
				virtual bool getChangedDimension() const = 0;
				virtual bool getGrabInput() const = 0;
				virtual bool getSupportedDimension(const Vector pDimension) const = 0;
				virtual WheelCode getWheelState() const = 0;

				virtual bool getKeyReleased(const KeyCode
					pKeyCode) const = 0;

				virtual bool getKeyPressed(const KeyCode
					pKeyCode) const = 0;

				virtual bool getButtonReleased(const ButtonCode
					pButtonCode) const = 0;

				virtual bool getButtonPressed(const ButtonCode
					pButtonCode) const = 0;

				virtual HatCode getJoystickHatState(const int
					pJoystickNumber, const int pHatNumber) = 0;

				virtual bool getJoystickButtonPressed(const int pJoystickNumber,
					const int pButtonNumber) = 0;

				virtual bool getJoystickButtonReleased(const int pJoystickNumber,
					const int pButtonNumber) = 0;

				virtual int getJoystickAxisPosition(const int pJoystickNumber,
					const int pAxisNumber) = 0;

				virtual bool loop() = 0;
				virtual void startTimer() = 0;
				virtual void stopTimer() = 0;
				virtual void sleep(const int pMilliseconds) const = 0;
		};
	};
};

#endif

