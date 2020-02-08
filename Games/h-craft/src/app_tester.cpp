// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "app_tester.h"
#include "logging.h"
#include "main.h"
#include "game.h"
#include "gui.h"
#include "gui_dialog.h"
#include "keycode_strings.h"
#include "config.h"
#include "tinyxml/tinyxml.h"
#include <sstream>
#include <string>

AppTester::AppTester()
: mLogPriority(LP_INFO_ESSENTIAL)
, mXmlDocument(0)
, mWaitTimeTarget(0)
, mApp(0)
{
}

void AppTester::Init(App * app)
{
	mApp = app;
}

bool AppTester::Load(const char* filenameXml, irr::io::IFileSystem * fs)
{
    delete mXmlDocument;
    mFunctions.clear();
    mCommandStack.clear();

    mXmlDocument = new TiXmlDocument(filenameXml);
    mXmlDocument->SetIrrFs(fs, TiXmlDocument::E_ON_READ_FAIL_ANDROID);

	if ( !mXmlDocument || !mXmlDocument->LoadFile() )
	{
		if ( mXmlDocument )
		{
			LogLn("AppTester::Load - TiXmlDocument::ErrorDesc: ", mXmlDocument->ErrorDesc());
			LogLn("file: ", filenameXml);
			delete mXmlDocument;
			mXmlDocument = 0;
		}

		return false;
	}

	TiXmlElement* root = mXmlDocument->RootElement();
	if ( !root )
		return false;

	LogLn("testfile loaded: ", filenameXml);
	mCommandStack.push_back(root->FirstChildElement());

	return true;
}

void AppTester::PushNextSibling(const TiXmlElement* element)
{
	const TiXmlElement* sibling = element->NextSiblingElement();
	if ( sibling )
	{
		mCommandStack.push_back(sibling);
	}
}

bool AppTester::Update()
{
	irr::IrrlichtDevice* irrDevice = mApp->GetIrrlichtManager()->GetIrrlichtDevice();

	irr::u32 timeTick = irrDevice->getTimer()->getTime();

	if ( timeTick < mWaitTimeTarget )
		return true;

	while ( !mCommandStack.empty() )
	{
		const TiXmlElement* activeCommand = mCommandStack.back();
		mCommandStack.pop_back();

		irr::core::stringc value(activeCommand->Value());
		if ( value == "frame_end" )
		{
			PushNextSibling(activeCommand);
			return true;
		}
		else if ( value == "wait" )
		{
			int timeMs = 0;
			activeCommand->Attribute("timeMs", &timeMs);
			mWaitTimeTarget = timeTick + (irr::u32)timeMs;
			PushNextSibling(activeCommand);
			return true;
		}
		else if ( value == "lmouse_click" )
		{
			irr::gui::IGUIElement * element = getElementByName( activeCommand->Attribute("element") );
			if ( element )
			{
				SendLMouseEvent( element->getAbsolutePosition().getCenter(), true );
				SendLMouseEvent( element->getAbsolutePosition().getCenter(), false );
			}
			else
			{
				LogLn("lmouse_click element not found: ", activeCommand->Attribute("element"));
				return false;
			}
		}
		else if ( value == "lmouse_down" )
		{
			irr::gui::IGUIElement * element = getElementByName( activeCommand->Attribute("element") );
			if ( element )
			{
				SendLMouseEvent( element->getAbsolutePosition().getCenter(), true );
			}
			else
			{
				LogLn("lmouse_down element not found: ", activeCommand->Attribute("element"));
				return false;
			}
		}
		else if ( value == "lmouse_up" )
		{
			irr::gui::IGUIElement * element = getElementByName( activeCommand->Attribute("element") );
			if ( element )
			{
				SendLMouseEvent( element->getAbsolutePosition().getCenter(), false );
			}
			else
			{
				LogLn("lmouse_up element not found: ", activeCommand->Attribute("element"));
				return false;
			}
		}
		else if ( value == "key_down" )
		{
			SendKeyEvent( activeCommand->Attribute("key"), true);
		}
		else if ( value == "key_up" )
		{
			SendKeyEvent( activeCommand->Attribute("key"), false);
		}
		else if ( value == "screenshot" )
		{
			const char* name = activeCommand->Attribute("name");
			irr::video::IVideoDriver * videoDriver = irrDevice->getVideoDriver();
			irr::video::IImage * imageScreenShot = videoDriver->createScreenShot();

			if ( imageScreenShot )
			{
				// figure out some nice name
				static irr::s32 num_shot = 0;
				std::string shotName(irr::core::stringc(num_shot).c_str());
				++num_shot;

				bool needsName = true;
				if ( name )
				{
					std::string stdName(name);
					if ( !stdName.empty() )
					{
						needsName = false;
						shotName += "_";
						shotName += stdName;
					}
				}
				if ( needsName && mApp->GetMode() == MODE_GAME )
				{
					// set screenshot name to levelname if the game is running and no other name was given
					shotName += "_";
					shotName += std::string( mApp->GetGame()->GetSettings().mLevelName.begin(), mApp->GetGame()->GetSettings().mLevelName.end() );
				}
				shotName += ".bmp";
				shotName = mApp->GetConfig()->MakeFilenameTestResults( std::string(shotName) );

  				// write it
				videoDriver->writeImageToFile( imageScreenShot, shotName.c_str() );
				imageScreenShot->drop();
			}
		}
		else if ( value == "write_fps" )
		{
			const char* name = activeCommand->Attribute("name");
			irr::video::IVideoDriver * videoDriver = irrDevice->getVideoDriver();

			std::ostringstream strstream;
			strstream << "FPS";
			if ( name )
				strstream << name;
			strstream << ": " << videoDriver->getFPS() << "\n";
			Log(strstream.str().c_str());
		}
		else if ( value == "reset_profile" )
		{
#if IRR_REV >= 3
			irr::getProfiler().resetAll();
#endif
		}
		else if ( value == "write_profile" )
		{
#if IRR_REV >= 3
			irr::core::stringw profileResult;
			irr::getProfiler().printAll(profileResult);
			const char* name = activeCommand->Attribute("name");
			LogLn("Profile ", name);
			Log(profileResult.c_str(), false);
#endif
		}
		else if ( value == "show_profile" )
		{
#if IRR_REV >= 4
			if ( mApp->GetGui() && mApp->GetGui()->GetGuiProfiler() )
			{
				mApp->GetGui()->GetGuiProfiler()->setShowGroupsTogether(true);
				mApp->GetGui()->GetGuiProfiler()->firstPage(false);
				mApp->GetGui()->GetGuiProfiler()->setFrozen(true);

				if (mApp->GetGui()->GetGuiProfiler2())
				{
					mApp->GetGui()->GetGuiProfiler2()->setShowGroupsTogether(true);
					mApp->GetGui()->GetGuiProfiler2()->firstPage(false);
					mApp->GetGui()->GetGuiProfiler2()->nextPage(false);	// amount of nextPage's is try&error
					mApp->GetGui()->GetGuiProfiler2()->nextPage(false);
					mApp->GetGui()->GetGuiProfiler2()->nextPage(false);
					mApp->GetGui()->GetGuiProfiler2()->setFrozen(true);
				}

				mApp->GetGui()->SetShowProfiler(true);
			}
#endif
		}
		else if ( value == "write_scenemanager_parameters" )
		{
			const char* name = activeCommand->Attribute("name");
			LogLn("SceneManager parameters (debug only): ", name);

			irr::scene::ISceneManager* smgr = irrDevice->getSceneManager();
			if ( smgr )
			{
				irr::io::IAttributes* attribs = smgr->getParameters();
				if ( attribs )
				{
					LogLn("culled", attribs->getAttributeAsInt("culled", 0));
					LogLn("calls", attribs->getAttributeAsInt("calls", 0));
					LogLn("drawn_solid", attribs->getAttributeAsInt("drawn_solid", 0));
					LogLn("drawn_transparent", attribs->getAttributeAsInt("drawn_transparent", 0));
					LogLn("drawn_transparent_effect", attribs->getAttributeAsInt("drawn_transparent_effect", 0));
				}
			}
		}
		else if ( value == "call" )
		{
			PushNextSibling(activeCommand);

			const char* name = activeCommand->Attribute("name");
			if ( name )
			{
				std::map<std::string, const TiXmlElement*>::const_iterator it = mFunctions.find( std::string(name) );
				if ( it != mFunctions.end() )
				{
					const TiXmlElement* child = it->second->FirstChildElement();
					if ( child )
					{
						mCommandStack.push_back(child);
					}
				}
				else
				{
					LogLn("Function declaration not found for: ", name);
				}
			}
			continue;
		}
		else if ( value == "func" )
		{
			const char* name = activeCommand->Attribute("name");
			if ( name )
			{
				mFunctions[std::string(name)] = activeCommand;
			}
			else
			{
				Log("func without name is invalid\n");
			}
		}

		PushNextSibling(activeCommand);
	}

	return false;
}

void AppTester::SendLMouseEvent(const irr::core::position2di& pos, bool down)
{
	irr::SEvent event;
	event.EventType = irr::EET_MOUSE_INPUT_EVENT;
	event.MouseInput.Event = down ? irr::EMIE_LMOUSE_PRESSED_DOWN : irr::EMIE_LMOUSE_LEFT_UP;
	event.MouseInput.Control = 0;
	event.MouseInput.Shift = 0;
	event.MouseInput.X = pos.X;
	event.MouseInput.Y = pos.Y;

	mApp->GetIrrlichtManager()->GetIrrlichtDevice()->postEventFromUser(event);
}

void AppTester::SendKeyEvent(const char * key, bool down)
{
	if ( key )
	{
		irr::SEvent event;
		event.EventType = irr::EET_KEY_INPUT_EVENT;
		event.KeyInput.Char = 0;
		event.KeyInput.Key = StringToIrrKey( std::string(key) );
		event.KeyInput.PressedDown = down;
		event.KeyInput.Shift = false;
		event.KeyInput.Control = false;

		mApp->GetIrrlichtManager()->GetIrrlichtDevice()->postEventFromUser(event);
	}
}

irr::gui::IGUIElement* AppTester::getElementByName(const char* name) const
{
	if ( name )
	{
		GuiDialog * dialog = mApp->GetGui()->GetActiveDialog();
		if ( dialog )
		{
			std::string error;
			return dialog->GetElementByName( mApp->GetIrrlichtManager()->GetIrrlichtDevice()->getGUIEnvironment()->getRootGUIElement(), std::string(name), error);
		}
	}

	return 0;
}

void AppTester::Log(const char * text, bool testLabel)
{
	std::string str( testLabel ? "TEST: " : "");
	str += text;
	LOG.Log(mLogPriority, str);
}

void AppTester::Log(const wchar_t * text, bool testLabel)
{
	std::wstring str(testLabel ? L"TEST: " : L"");
	str += text;
	LOG.Log(mLogPriority, str);
}

void AppTester::LogLn(const char * text, const char * text2)
{
	std::string str("TEST: ");
	str += text;
	LOG.LogLn(mLogPriority, str.c_str(), text2);
}

void AppTester::LogLn(const char * text, int value)
{
	std::string str("TEST: ");
	str += text;
	LOG.LogLn(mLogPriority, str.c_str(), value);
}

void AppTester::LogLn(const char * text, float value)
{
	std::string str("TEST: ");
	str += text;
	LOG.LogLn(mLogPriority, str.c_str(), value);
}
