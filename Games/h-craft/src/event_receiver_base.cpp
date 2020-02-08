// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "event_receiver_base.h"
#include "compile_config.h"
#include "physics.h"
#include "gui_ids.h"
#include "config.h"
#include "gui.h"
#include "input_device_manager.h"
#include "gui_dialogs/gui_user_controls.h"
#include "gui_dialogs/gui_developer_settings.h"
#include "gui_dialogs/gui_levelmanager.h"
#include "gui_dialogs/gui_editor.h"
#include "gui_dialogs/gui_hud.h"
#include "game.h"
#include "editor.h"
#include "main.h"
#include "logging.h"
#include "profiles.h"
#include "string_table.h"
#include "sound.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#ifdef _IRR_ANDROID_PLATFORM_
#include <android_native_app_glue.h>
#endif

using namespace irr;
using namespace gui;

EventReceiverBase::EventReceiverBase(const Config& config)
: mConfig(config)
{
}

// Most events should be distributed to the Gui and/or the InputDeviceManager
// The rest which is in here is mostly used for debug/edit purposes and
// didn't quite fit anywhere else while developing it.
bool EventReceiverBase::OnEvent(const SEvent& event)
{
    if ( APP.GetGui() && APP.GetGui()->OnEvent(event) )
    {
        return true;
    }

#ifdef HC1_ENABLE_EDITOR
    if ( APP.GetEditor() && APP.GetEditor()->OnEvent(event) )
        return true;
#endif

    if ( APP.GetGame() && APP.GetGame()->OnEvent(event) )
        return true;

    switch ( event.EventType )
    {
        case EET_KEY_INPUT_EVENT:
        {
            if ( APP.GetInputDeviceManager() && APP.GetInputDeviceManager()->OnEvent(event) )
                return true;

            bool invalidFocus = false;
            IGUIElement * focusElement = APP.GetGui() ? APP.GetGui()->GetFocusElement() : NULL;
            if ( focusElement &&
                (   focusElement->getType() == EGUIET_EDIT_BOX
                ||  focusElement->getType() == EGUIET_FILE_OPEN_DIALOG ) )
            {
                invalidFocus = true;
            }

            if ( !event.KeyInput.PressedDown && !invalidFocus)
            {
                switch(event.KeyInput.Key)
                {
                    case KEY_ESCAPE:
                    {
                        if ( APP.GetMode() == MODE_GAME && APP.GetGame() )
                        {
                            const GameSettings& gameSettings = APP.GetGame()->GetSettings();
                            if ( gameSettings.mGameType == GT_HOTLAP )
                            {
								// You get a session-best display here which is not available in the pause screen
								// (thought I consider it irritating as it's the only mode where ESC doesn't enter
								// pause screen while pause button still does - no idea why we thought that a
								// good idea back then).
                                APP.GetGame()->Finish(false);
                            }
                            else
                            {
                                APP.GetGame()->Pause();
                            }
                        }
                        if ( APP.GetMode() == MODE_EDITOR || APP.GetMode() == MODE_FREEFLY)
                        {
                            APP.SetMode(MODE_GUI);
                        }
                        break;
                    }
                    case KEY_F12:
                    {
                        if ( APP.GetIrrlichtManager() && APP.GetIrrlichtManager()->GetVideoDriver() )
                        {
                            video::IImage * imageScreenShot = APP.GetIrrlichtManager()->GetVideoDriver()->createScreenShot();
                            if ( imageScreenShot )
                            {
                                time_t timeNow = time(0);
                                tm * timeNowTm = localtime(&timeNow);
                                std::ostringstream shotName;
                                shotName << mConfig.MakeFilenameUserData("shot");
                                if ( timeNowTm )
                                {
                                    shotName << 1900+timeNowTm->tm_year << "_" << timeNowTm->tm_mon+1 << "_" << timeNowTm->tm_mday;
                                    shotName << "_" << timeNowTm->tm_hour << "_" << timeNowTm->tm_min << "_" << timeNowTm->tm_sec;
                                }
                                shotName << ".bmp";

                                APP.GetIrrlichtManager()->GetVideoDriver()->writeImageToFile( imageScreenShot, shotName.str().c_str() );
                                imageScreenShot->drop();
                            }
                        }
                    }
                    break;
                    case KEY_F11:
                    {
                        if ( APP.GetGui() )
                        {
                            GuiHud* hud = APP.GetGui()->GetGuiHud();
                            if ( hud )
                            {
                                hud->SetFpsVisible( hud->GetFpsVisible() ? false : true );
                            }
                        }
                    }
                    break;

#if !defined(HOVER_RELEASE)
                    case KEY_KEY_F:
                    {
                        APP.SetMode(MODE_FREEFLY);
                        break;
                    }
#endif
#ifdef HC1_ENABLE_EDITOR
                    case KEY_KEY_G:
                    {
                        if ( APP.GetGui() && APP.GetGui()->GetTestDialog() && APP.GetStringTable())
                        {
                        	APP.GetStringTable()->Load(APP.GetStringTable()->GetFileName(), false);
                            APP.GetGui()->GetTestDialog()->Load(0, true);
                            APP.SetMode(MODE_GUI_DLG_TEST);
                        }
                        break;
                    }
#endif
#if !defined(HOVER_RELEASE)
                    case KEY_KEY_S:
                    {
                        if ( APP.GetEditGui() && APP.GetGame() )
                        {
                            APP.SetMode(MODE_GAME);
                            APP.GetGame()->PrepareStart();
                            APP.GetGame()->Start();
                        }
                        break;
                    }
                    case KEY_KEY_E:
                    {
                        if ( APP.GetEditGui() )
                        {
                            APP.SetMode(MODE_EDITOR);
                        }
                        break;
                    }

                    case KEY_KEY_1:
                    {
                        if ( APP.GetGame() )
                        {
                            APP.GetGame()->SetLocalPlayerStartTrack(0);
                        }
                        break;
                    }
                    case KEY_KEY_2:
                    {
                        if ( APP.GetGame() )
                        {
                            APP.GetGame()->SetLocalPlayerStartTrack(1);
                        }
                        break;
                    }
                    case KEY_KEY_3:
                    {
                        if ( APP.GetGame() )
                        {
                            APP.GetGame()->SetLocalPlayerStartTrack(2);
                        }
                        break;
                    }
                    case KEY_KEY_4:
                    {
                        if ( APP.GetGame() )
                        {
                            APP.GetGame()->SetLocalPlayerStartTrack(3);
                        }
                        break;
                    }

                    case KEY_KEY_M:
                    {
                        if ( APP.GetEditGui() )
                        {
                            APP.SetEditGui(false);
                        }
                        else
                        {
                            APP.SetEditGui(true);
                        }
                        break;
                    }
                    case KEY_F7:
                    {
                        if ( APP.GetIrrlichtManager() )
                        {
                            APP.GetIrrlichtManager()->LogAllTexturesInformation(LP_INFO_ESSENTIAL);
                        }
                        break;
                    };

                    case KEY_F6:
                    {
                        if ( APP.GetIrrlichtManager() )
                        {
                            APP.GetIrrlichtManager()->LogAllMeshInformation(LP_INFO_ESSENTIAL);
                        }
                        break;
                    };

#endif

#ifdef HC1_ENABLE_PROFILE
                    case KEY_F8:
                    {
						core::stringw irrstringw;
						core::stringc irrstringc;
						getProfiler().printAll(irrstringw, false, true);
						getProfiler().resetAll();
						irrstringc = irrstringw;
						std::ofstream streamProfile( mConfig.MakeFilenameBase("profile.txt").c_str() );
						streamProfile << irrstringc.c_str();
                        break;
                    }
#if !defined(HOVER_RELEASE)
                    case KEY_F1:
                    {
                        if ( APP.GetGui() )
                        {
                        	APP.GetGui()->SetShowProfiler(!APP.GetGui()->GetShowProfiler());
						}
                        break;
                    };
                    case KEY_F2:
                    {
                        if ( APP.GetGui() && APP.GetGui()->GetGuiProfiler() )
                        {
                            APP.GetGui()->GetGuiProfiler()->firstPage();
                            APP.GetGui()->SetShowProfiler(true);
                        }
                        break;
                    };
                    case KEY_F3:
                    {
                        if ( APP.GetGui() && APP.GetGui()->GetGuiProfiler() )
                        {
                            APP.GetGui()->GetGuiProfiler()->nextPage();
                            APP.GetGui()->SetShowProfiler(true);
                        }
                        break;
                    };
                    case KEY_F4:
                    {
                        if ( APP.GetGui() && APP.GetGui()->GetGuiProfiler() )
                        {
                            APP.GetGui()->GetGuiProfiler()->previousPage();
                            APP.GetGui()->SetShowProfiler(true);
                        }
                        break;
                    };
#endif
#endif // HC1_ENABLE_PROFILE
                    default:
                        break;
                }
            }
            else if ( event.KeyInput.PressedDown && event.KeyInput.Shift && !invalidFocus)
            {
                switch(event.KeyInput.Key)
                {
                    case KEY_DIVIDE:
                    {
                        if ( APP.GetProfileManager() )
                        {
                            PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
                            if ( profile )
                                profile->CheatChampionship(1);
                        }
                    }
                    break;
                    case KEY_MULTIPLY:
                    {
                        if ( APP.GetProfileManager() )
                        {
                            PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
                            if ( profile )
                                profile->CheatChampionship(2);
                        }
                    }
                    break;
                    case KEY_SUBTRACT:
                    {
                        if ( APP.GetProfileManager() )
                        {
                            PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
                            if ( profile )
                                profile->CheatChampionship(3);
                        }
                    }
                    break;
                    default:
                    break;
                }
            }
        }
        break;

        case EET_MOUSE_INPUT_EVENT:
			if ( event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN && mConfig.GetUseTouchInput() != ETI_NO_TOUCH && APP.GetGui() )
			{
				GuiHud* hud = APP.GetGui()->GetGuiHud();
				if ( hud )
				{
					hud->OnMouseClick( event.MouseInput.X, event.MouseInput.Y );
				}
			}

			if ( APP.GetInputDeviceManager() && APP.GetInputDeviceManager()->OnEvent(event) )
                return true;
		break;

#ifdef _IRR_ANDROID_PLATFORM_
		case EET_SYSTEM_EVENT:
		{
			if ( event.SystemEvent.EventType == ESET_ANDROID_CMD )
			{
				switch ( event.SystemEvent.AndroidCmd.Cmd )
				{
					case APP_CMD_PAUSE:
					{
						if ( APP.GetMode() == MODE_GAME && APP.GetGame() )
						{
							APP.GetGame()->Pause();
							APP.GetSound()->StopMusic();
							APP.GetSound()->StopAllSamples();
						}
					}
					break;
					default:
					break;
				}
			}
		}
		break;

		case EET_ACCELEROMETER_EVENT:
		case EET_GYROSCOPE_EVENT:
            if ( APP.GetInputDeviceManager() && APP.GetInputDeviceManager()->OnEvent(event) )
                return true;
		break;

		case EET_TOUCH_INPUT_EVENT:
        {
            if ( APP.GetInputDeviceManager() && APP.GetInputDeviceManager()->OnEvent(event) )
                return true;

			IrrlichtDevice * irrDevice = APP.GetIrrlichtManager() ? APP.GetIrrlichtManager()->GetIrrlichtDevice() : 0;
			if (!irrDevice)
				break;

			// TODO: We fake mouse-events. This should be handled in Irrlicht instead - but enough for a first test.
			// We only work with the first for now, but could be up to NUMBER_OF_MULTI_TOUCHES touches.

			SEvent fakeMouseEvent;
			fakeMouseEvent.EventType = EET_MOUSE_INPUT_EVENT;
			fakeMouseEvent.MouseInput.X = event.TouchInput.X;
			fakeMouseEvent.MouseInput.Y = event.TouchInput.Y;
			fakeMouseEvent.MouseInput.Shift = false;
			fakeMouseEvent.MouseInput.Control = false;
			fakeMouseEvent.MouseInput.ButtonStates = 0;

			switch ( event.TouchInput.Event)
			{
				case ETIE_PRESSED_DOWN:
				{
					//LOG.Debug(L"ETIE_PRESSED_DOWN");
					fakeMouseEvent.MouseInput.Event = EMIE_LMOUSE_PRESSED_DOWN;
					break;
				}
				case ETIE_MOVED:
				{
					//LOG.Debug(L"ETIE_MOVED");
					fakeMouseEvent.MouseInput.Event = EMIE_MOUSE_MOVED;
					fakeMouseEvent.MouseInput.ButtonStates = EMBSM_LEFT;
					break;
				}
				case ETIE_LEFT_UP:
				{
					//LOG.Debug(L"ETIE_LEFT_UP");
					fakeMouseEvent.MouseInput.Event = EMIE_LMOUSE_LEFT_UP;
					break;
				}
				default:
					break;
			}
			irrDevice->postEventFromUser(fakeMouseEvent);
        }
        break;
#endif

#ifdef HC1_ENABLE_EDITOR
        case EET_GUI_EVENT:
        {
            if ( !APP.GetIrrlichtManager() || !APP.GetIrrlichtManager()->GetIrrlichtDevice() )
                break;
            IGUIEnvironment* env = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getGUIEnvironment();
            if ( !env )
                break;

            if ( !event.GUIEvent.Caller )
                break;
            s32 id = event.GUIEvent.Caller->getID();

            switch(event.GUIEvent.EventType)
            {
                case EGET_MENU_ITEM_SELECTED:
                {
                    // a menu item was clicked

                    IGUIContextMenu* menu = (IGUIContextMenu*)event.GUIEvent.Caller;
                    if ( !menu )
                        break;
                    s32 id = menu->getItemCommandId(menu->getSelectedItem());

                    switch(id)
                    {
                        case GUI_OPEN_MODEL_FILE: // File -> Open Model
                            env->addFileOpenDialog(L"Please select a model file to open", /*modal*/false, /*parent*/0, GUI_FILEDLG_OPEN_MODEL);
                        break;

                        case GUI_OPEN_GUI_DLG:
                            if (  APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem() )
                            {
                                APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem()->changeWorkingDirectoryTo (mConfig.GetPathUI().c_str());
                                env->addFileOpenDialog(L"Please select a GUI XML", /*modal*/false, /*parent*/0, GUI_FILEDLG_OPEN_GUI);
                            }
                        break;

                        case GUI_LOAD_LEVEL:
                            if ( APP.GetGui() && APP.GetGui()->GetGuiLevelManager() )
                            {
								if ( APP.GetEditGui() )
								{
									APP.SetMode(MODE_EDITOR);
								}
                                APP.GetGui()->GetGuiLevelManager()->ShowLevelSelection();
                            }
                        break;

                        case GUI_QUIT: // File -> Quit
                            APP.GetIrrlichtManager()->GetIrrlichtDevice()->closeDevice();
                        break;

                        case GUI_TOGGLE_PHYSICS_DEBUG:
                            if ( APP.GetPhysics() )
                            {
                                if ( APP.GetPhysics()->IsDebuggingEnabled() )
                                    APP.GetPhysics()->EnableDebugging(false);
                                else
                                    APP.GetPhysics()->EnableDebugging(true);
                            }
                        break;

                        case GUI_SAVE_SETTINGS:
                            APP.SaveSettings();
                        break;

                        case GUI_LOAD_SETTINGS:
							APP.LoadSettings();
                        break;

                        case GUI_OPEN_USER_CONTROLS:
                        {
                            if ( APP.GetGui() && APP.GetGui()->GetUserControls() )
                                APP.GetGui()->GetUserControls()->CreateUserControls();
                        }
                        break;

                        case GUI_SAVE_LAST_RECORDING:
                        {
                            if ( APP.GetGame() )
                            {
                                APP.GetGame()->SaveLastRecording();
                            }
                        }
                        break;

                        case GUI_TOGGLE_AUTOSAVE_RECORDING:
                        {
                            if ( APP.GetGame() && APP.GetGui() )
                            {
                                if ( APP.GetGame()->GetAutosaveRecording() )
                                {
                                    APP.GetGui()->DisplayInfoText("Disable autosave recording", 3000);
                                    APP.GetGame()->SetAutosaveRecording(false);
                                }
                                else
                                {
                                    APP.GetGui()->DisplayInfoText("Enable autosave recording", 3000);
                                    APP.GetGame()->SetAutosaveRecording(true);
                                }
                            }
                        }
                        break;

                        case GUI_TOGGLE_AUTOLOAD_RECORDING:
                        {
                            if ( APP.GetGame() && APP.GetGui() )
                            {
                                if ( APP.GetGame()->GetAutoloadRecording() )
                                {
                                    APP.GetGui()->DisplayInfoText("Disable autoload recording", 3000);
                                    APP.GetGame()->SetAutoloadRecording(false);
                                }
                                else
                                {
                                    APP.GetGui()->DisplayInfoText("Enable autoload recording", 3000);
                                    APP.GetGame()->SetAutoloadRecording(true);
                                }
                            }
                        }
                        break;
                        default:
                        break;
                    }
                }
                break;
                case EGET_FILE_SELECTED:
                {
                    switch ( id )
                    {
                        case GUI_FILEDLG_OPEN_MODEL:
                        {
                            // load the model file, selected in the file open dialog
                            IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
                            if ( dialog && APP.GetIrrlichtManager() )
                            {
                                std::wstring str(dialog->getFileName());
                                APP.GetIrrlichtManager()->LoadAnimatedModel(mConfig, std::string(str.begin(), str.end()).c_str());
                            }
                        }
                        break;
                        case GUI_FILEDLG_OPEN_GUI:
                        {
                            // load and display the selected gui dialog
                            IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
                            if ( dialog && APP.GetGui() && APP.GetGui()->GetTestDialog() )
                            {
                                std::wstring str(dialog->getFileName());
                                APP.GetGui()->GetTestDialog()->Load(std::string(str.begin(), str.end()).c_str());
                                APP.SetMode(MODE_GUI_DLG_TEST);
                            }
                        }
                        break;
                    }
                }
                break;

                case EGET_BUTTON_CLICKED:
                {
                    switch(id)
                    {
                        case GUI_ICON_OPEN_FILE:
                            if ( APP.GetGui() && APP.GetGui()->GetGuiLevelManager() )
                            {
								if ( APP.GetEditGui() )
								{
									APP.SetMode(MODE_EDITOR);
								}
                                APP.GetGui()->GetGuiLevelManager()->ShowLevelSelection();
                            }
                            break;
                        case GUI_ICON_TOOLBAR:
                            if ( APP.GetGui() && APP.GetGui()->GetDeveloperSettings() )
                            {
                                APP.GetGui()->GetDeveloperSettings()->CreateDeveloperSettings();
                            }
                            break;
                        default:
                        break;
                    }
                }
                break;
                default:
                break;
            }
        }
        break;
#endif
        default:
        break;
    }

    return false;
}
