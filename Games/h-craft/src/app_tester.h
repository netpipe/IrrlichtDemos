// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef APP_TESTER_H
#define APP_TESTER_H

#include "logging_priorities.h"
#include <irrlicht.h>
#include <map>
#include <vector>
#include <string>

class TiXmlDocument;
class TiXmlElement;
class App;

//! Automatic runs through the game and does stuff like screenshots.
//! It's controlled by a xml-file with the commands.
class AppTester
{
public:
	AppTester();

	void Init(App * app);

	//! filesystem only needed on Android
	bool Load(const char* filenameXml, irr::io::IFileSystem * fs=0);

	bool HasTest() const
	{
		return (!mCommandStack.empty() && mApp != 0) ? true : false;
	}
	bool Update();

protected:
	void Log(const char * text, bool testLabel=true);
    void Log(const wchar_t * text, bool testLabel=true);
    void LogLn(const char * text, const char * text2);
    void LogLn(const char * text, int value);
    void LogLn(const char * text, float value);

	irr::gui::IGUIElement* getElementByName(const char* name) const;
	void SendLMouseEvent(const irr::core::position2di& pos, bool down);
	void SendKeyEvent(const char * key, bool down);
	void PushNextSibling(const TiXmlElement* element);

private:
	LogPriority mLogPriority;

	// So why xml? Because I already got a loader for it, speed for this
	// doesn't matter and it's easy to use.
	TiXmlDocument * mXmlDocument;

	// current command to runs
	std::vector<const TiXmlElement*> mCommandStack;

	std::map<std::string, const TiXmlElement*> mFunctions;

	// don't run an update until this time is reached
	irr::u32 mWaitTimeTarget;

	App * mApp;
};

#endif // APP_TESTER_H

