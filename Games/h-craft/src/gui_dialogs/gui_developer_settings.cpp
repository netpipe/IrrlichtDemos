// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_developer_settings.h"
#include "../gui_ids.h"
#include "../config.h"
#include "../physics.h"
#include "../game.h"
#include "../main.h"
#include "../globals.h"
#include "../camera.h"
#include <cstdlib>

using namespace irr;
using namespace gui;

GuiDeveloperSettings::GuiDeveloperSettings(irr::gui::IGUIEnvironment* env_)
: mEnvironment(env_)
, mWindow(NULL)
, mLastOpenTab(0)
{
}

GuiDeveloperSettings::~GuiDeveloperSettings()
{
}

void GuiDeveloperSettings::CreateDeveloperSettings()
{
    if ( !mEnvironment )
        return;

	// remove tool box if already there
	IGUIElement* root = mEnvironment->getRootGUIElement();
	IGUIElement* e = root->getElementFromId(GUI_TOOLBOX, true);
	if (e)
	{
        e->remove();
        mWindow = NULL;
	}

	// create the toolbox window
	mWindow = mEnvironment->addWindow(core::rect<s32>(300,25,640,480),
		false, L"Toolset", 0, GUI_TOOLBOX);

	// create tab control and tabs
	IGUITabControl* tab = mEnvironment->addTabControl(core::rect<s32>(2,20,640-302,480-7),
                                                        mWindow, true, true, GUI_TAB_TOOLBOX);

    int editSize=1024;
    wchar_t editText[editSize+1];

    // steering
    IGUITab* tabSteering = tab->addTab(L"Steering");
    SteeringSettings steering;
    steering.ReadFromXml(APP.GetConfig()->GetSteeringSettings());

    int top = 20;
    int left = 10;
    int textRight = left + 200;
    mEnvironment->addStaticText(L"rotation speed", core::rect<s32>(left, top, textRight, top+10), false, false, tabSteering, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", steering.mRotationSpeed);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,150,top+20), true, tabSteering, EDIT_ROTATION_SPEED);
    top += 25;

    mEnvironment->addStaticText(L"scale rotation speed", core::rect<s32>(left, top, textRight, top+10), false, false, tabSteering, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", steering.mScaleRotationBySpeed);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,150,top+20), true, tabSteering, EDIT_SCALE_ROTATION_SPEED);
    top += 25;

//    mEnvironment->addStaticText(L"(scale rotation engine)", core::rect<s32>(left, top, textRight, top+10), false, false, tabSteering, -1, false);
//    top += 15;
//    swprintf_hc(editText, editSize, L"%.3f", steering.mScaleRotationByEngine);
//    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,150,top+20), true, tabSteering, EDIT_SCALE_ROTATION_ENGINE);
//    top += 25;

    mEnvironment->addStaticText(L"force factor", core::rect<s32>(left, top, textRight, top+10), false, false, tabSteering, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", steering.mForceFactor);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,150,top+20), true, tabSteering, EDIT_FORCE_FACTOR);
    top += 25;

    mEnvironment->addStaticText(L"brake factor", core::rect<s32>(left, top, textRight, top+10), false, false, tabSteering, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", steering.mBrakeFactor);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,150,top+20), true, tabSteering, EDIT_BRAKE_FACTOR);
    top += 25;

    mEnvironment->addStaticText(L"scale rolling", core::rect<s32>(left, top, textRight, top+10), false, false, tabSteering, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", steering.mScaleRolling);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,150,top+20), true, tabSteering, EDIT_SCALE_ROLLING);
    top += 25;

    mEnvironment->addStaticText(L"max rolling", core::rect<s32>(left, top, textRight, top+10), false, false, tabSteering, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", steering.mMaxRolling);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,150,top+20), true, tabSteering, EDIT_MAX_ROLLING);
    top += 25;

    mEnvironment->addStaticText(L"scale pitch flying", core::rect<s32>(left, top, textRight, top+10), false, false, tabSteering, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", steering.mScalePitchFlying);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,150,top+20), true, tabSteering, EDIT_SCALE_PITCH_FLYING);
    top += 25;

    mEnvironment->addStaticText(L"scale floor alignment", core::rect<s32>(left, top, textRight, top+10), false, false, tabSteering, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", steering.mScaleAlignment);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,150,top+20), true, tabSteering, EDIT_SCALE_ALIGNMENT);
    top += 25;

    mEnvironment->addButton(core::rect<s32>(160,300,220,330), tabSteering, BTN_SET_STEERING, L"set");

    // camera
    IGUITab* tabCamera = tab->addTab(L"Camera");
    CameraSettings camera;
    camera.ReadFromXml(APP.GetConfig()->GetCameraSettings(APP.GetGame()->GetActiveCameraIndex()));

    top = 20;

    core::rect< s32 > controllersRect(10, top, 150, top+80 );
    IGUIListBox * listCameras =	mEnvironment->addListBox(controllersRect, tabCamera, LIST_CAMERAS );
    for ( size_t i=0; i < APP.GetGame()->GetNumCameras(); ++i )
    {
        swprintf_hc(editText, editSize, L"%d", (int)i);
        std::wstring str( editText );
        listCameras->addItem(str.c_str());
    }
    listCameras->setSelected(APP.GetGame()->GetActiveCameraIndex());
    top += 90;

    textRight = left+140;
    int left2 = textRight + 10;
    int right2 = left2 + 140;

    mEnvironment->addStaticText(L"cam behind", core::rect<s32>(left, top, textRight, top+10), false, false, tabCamera, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", camera.mCamBehind);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,textRight,top+20), true, tabCamera, EDIT_CAM_BEHIND);
    top += 25;

    mEnvironment->addStaticText(L"cam height", core::rect<s32>(left, top, textRight, top+10), false, false, tabCamera, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", camera.mCamHeight);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,textRight,top+20), true, tabCamera, EDIT_CAM_HEIGHT);
    top += 25;

    mEnvironment->addStaticText(L"cam height upwards", core::rect<s32>(left, top, textRight, top+10), false, false, tabCamera, -1, false);
    mEnvironment->addStaticText(L"cam height downwards", core::rect<s32>(left2, top, right2, top+10), false, false, tabCamera, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", camera.mCamHeightUpwards);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,textRight,top+20), true, tabCamera, EDIT_CAM_UPWARDS);
    swprintf_hc(editText, editSize, L"%.3f", camera.mCamHeightDownwards);
    mEnvironment->addEditBox(editText, core::rect<s32>(left2,top,right2,top+20), true, tabCamera, EDIT_CAM_DOWNWARDS);
    top += 25;

    mEnvironment->addStaticText(L"cam target height", core::rect<s32>(left, top, textRight, top+10), false, false, tabCamera, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", camera.mCamTargetHeight);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,textRight,top+20), true, tabCamera, EDIT_CAM_TARGET_FRONT);
    top += 25;

    mEnvironment->addStaticText(L"cam trailing", core::rect<s32>(left, top, textRight, top+10), false, false, tabCamera, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", camera.mCamTrailing);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,textRight,top+20), true, tabCamera, EDIT_CAM_TRAILING);
    top += 25;

    mEnvironment->addStaticText(L"cam speed slow", core::rect<s32>(left, top, textRight, top+10), false, false, tabCamera, -1, false);
    mEnvironment->addStaticText(L"cam speed fast", core::rect<s32>(left2, top, right2, top+10), false, false, tabCamera, -1, false);
    top += 15;
    swprintf_hc(editText, editSize, L"%.3f", camera.mCamSpeedFactorSlow);
    mEnvironment->addEditBox(editText, core::rect<s32>(left,top,textRight,top+20), true, tabCamera, EDIT_CAM_SPEED_FACTOR_SLOW);
    swprintf_hc(editText, editSize, L"%.3f", camera.mCamSpeedFactorFast);
    mEnvironment->addEditBox(editText, core::rect<s32>(left2,top,right2,top+20), true, tabCamera, EDIT_CAM_SPEED_FACTOR_FAST);
    top += 25;

    mEnvironment->addButton(core::rect<s32>(10,top,100,top+30), tabCamera, BTN_SET_CAMERA, L"set");

    // physics
    IGUITab* tabPhysics = tab->addTab(L"Physic");	// sorry for "Physic" instead of "Physics" but xml is already out there
    PhysicsSettings physics;
    physics.ReadFromXml(APP.GetConfig()->GetPhysicsSettings());
    PhysicsObjectSettings hover;
    hover.ReadFromXml(APP.GetConfig()->GetPhysicsObjectSettings("hover"));

    mEnvironment->addStaticText(L"timestep", core::rect<s32>(10, 20, 150, 30), false, false, tabPhysics, -1, false);
    swprintf_hc(editText, editSize, L"%.3f", physics.mTimeStep);
    mEnvironment->addEditBox(editText, core::rect<s32>(10,35,150,55), true, tabPhysics, EDIT_PHYSICS_TIMESTEP);

    mEnvironment->addStaticText(L"rollfriction", core::rect<s32>(10, 60, 150, 70), false, false, tabPhysics, -1, false);
    swprintf_hc(editText, editSize, L"%.3f", physics.mRollFriction);
    mEnvironment->addEditBox(editText, core::rect<s32>(10,75,150,95), true, tabPhysics, EDIT_PHYSICS_ROLLFRICTION);

    mEnvironment->addStaticText(L"airfriction", core::rect<s32>(10, 100, 150, 110), false, false, tabPhysics, -1, false);
    swprintf_hc(editText, editSize, L"%.3f", physics.mAirConst);
    mEnvironment->addEditBox(editText, core::rect<s32>(10,115,150,135), true, tabPhysics, EDIT_PHYSICS_AIRCONST);

    mEnvironment->addStaticText(L"gravity", core::rect<s32>(10, 140, 150, 150), false, false, tabPhysics, -1, false);
    swprintf_hc(editText, editSize, L"%.3f", physics.mGravity);
    mEnvironment->addEditBox(editText, core::rect<s32>(10,155,150,175), true, tabPhysics, EDIT_PHYSICS_GRAVITY);

    mEnvironment->addStaticText(L"hover mass", core::rect<s32>(10, 180, 150, 190), false, false, tabPhysics, -1, false);
    swprintf_hc(editText, editSize, L"%.3f", hover.mMass);
    mEnvironment->addEditBox(editText, core::rect<s32>(10,195,150,215), true, tabPhysics, EDIT_PHYSICS_HOVER_MASS);

    mEnvironment->addStaticText(L"hover max speed", core::rect<s32>(10, 220, 150, 230), false, false, tabPhysics, -1, false);
    swprintf_hc(editText, editSize, L"%.3f", hover.mMaxSpeed);
    mEnvironment->addEditBox(editText, core::rect<s32>(10,235,150,255), true, tabPhysics, EDIT_PHYSICS_HOVER_MAX_SPEED);

    mEnvironment->addButton(core::rect<s32>(10,300,100,330), tabPhysics, BTN_SET_PHYSICS, L"set");

    tab->setActiveTab(mLastOpenTab);
}

bool GuiDeveloperSettings::OnEvent(const SEvent &event)
{
    if ( !mEnvironment )
        return false;

    switch ( event.EventType )
    {
        case EET_GUI_EVENT:
        {
            s32 id = event.GUIEvent.Caller->getID();
            gui::IGUIElement* root = mEnvironment->getRootGUIElement();
            switch(event.GUIEvent.EventType)
            {
                case EGET_BUTTON_CLICKED:
                {
                    switch(id)
                    {
                        case BTN_SET_STEERING:
                        {
                            SteeringSettings settings;
                            core::stringc s;

                            s = root->getElementFromId(EDIT_ROTATION_SPEED, true)->getText();
                            settings.mRotationSpeed = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_SCALE_ROTATION_SPEED, true)->getText();
                            settings.mScaleRotationBySpeed = (float)atof(s.c_str());
//                            s = root->getElementFromId(EDIT_SCALE_ROTATION_ENGINE, true)->getText();
//                            settings.mScaleRotationByEngine = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_FORCE_FACTOR, true)->getText();
                            settings.mForceFactor = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_BRAKE_FACTOR, true)->getText();
                            settings.mBrakeFactor = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_SCALE_ROLLING, true)->getText();
                            settings.mScaleRolling = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_MAX_ROLLING, true)->getText();
                            settings.mMaxRolling = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_SCALE_PITCH_FLYING, true)->getText();
                            settings.mScalePitchFlying = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_SCALE_ALIGNMENT, true)->getText();
                            settings.mScaleAlignment = (float)atof(s.c_str());

                            settings.WriteToXml(APP.GetConfig()->GetSteeringSettings());
                            APP.GetGame()->ReloadSettingsSteering();
                        }
                        break;
                        case BTN_SET_CAMERA:
                        {
                            CameraSettings camera;
                            core::stringc s;

                            s = root->getElementFromId(EDIT_CAM_BEHIND, true)->getText();
                            camera.mCamBehind = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_CAM_HEIGHT, true)->getText();
                            camera.mCamHeight = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_CAM_UPWARDS, true)->getText();
                            camera.mCamHeightUpwards = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_CAM_DOWNWARDS, true)->getText();
                            camera.mCamHeightDownwards = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_CAM_TARGET_FRONT, true)->getText();
                            camera.mCamTargetHeight = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_CAM_TRAILING, true)->getText();
                            camera.mCamTrailing = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_CAM_SPEED_FACTOR_SLOW, true)->getText();
                            camera.mCamSpeedFactorSlow = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_CAM_SPEED_FACTOR_FAST, true)->getText();
                            camera.mCamSpeedFactorFast = (float)atof(s.c_str());
                            camera.WriteToXml(APP.GetConfig()->GetCameraSettings(APP.GetGame()->GetActiveCameraIndex()));
                            APP.GetGame()->ReloadSettingsCamera();
                        }
                        break;
                        case BTN_SET_PHYSICS:
                        {
                            PhysicsObjectSettings hover;
                            PhysicsSettings physics;
                            core::stringc s;

                            s = root->getElementFromId(EDIT_PHYSICS_TIMESTEP, true)->getText();
                            physics.mTimeStep = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_PHYSICS_ROLLFRICTION, true)->getText();
                            physics.mRollFriction = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_PHYSICS_AIRCONST, true)->getText();
                            physics.mAirConst = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_PHYSICS_GRAVITY, true)->getText();
                            physics.mGravity = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_PHYSICS_HOVER_MASS, true)->getText();
                            hover.mMass = (float)atof(s.c_str());
                            s = root->getElementFromId(EDIT_PHYSICS_HOVER_MAX_SPEED, true)->getText();
                            hover.mMaxSpeed = (float)atof(s.c_str());

                            physics.WriteToXml(APP.GetConfig()->GetPhysicsSettings());
                            hover.WriteToXml(APP.GetConfig()->GetPhysicsObjectSettings("hover"));
                            APP.GetGame()->ReloadSettingsPhysics();
                        }
                        break;
                        default:
                        break;
                    }
                }
                break;
                case EGET_LISTBOX_CHANGED:
                {
                    switch ( id  )
                    {
                        case LIST_CAMERAS:
                        {
                            if ( !mWindow )
                                break;
                            IGUIElement * ele = mWindow->getElementFromId(LIST_CAMERAS, true);
                            if ( !ele )
                                break;
                            IGUIListBox * listBox = static_cast<IGUIListBox*>(ele);
                            int selected = listBox->getSelected();
                            if ( selected < 0 )
                                selected = 0;
                            if ( (size_t)selected >= APP.GetGame()->GetNumCameras() )
                            {
                                selected = (int)APP.GetGame()->GetNumCameras()-1;
                            }
                            APP.GetGame()->SetActiveCameraIndex((size_t)selected);
                            CreateDeveloperSettings();
                            return true;
                        }
                        break;
                        default:
                        break;
                    }
                }
                break;
                case EGET_TAB_CHANGED:
                {
                    switch ( id )
                    {
                        case GUI_TAB_TOOLBOX:
                        {
                            if ( !mWindow )
                                break;
                            IGUIElement * ele = mWindow->getElementFromId(GUI_TAB_TOOLBOX, true);
                            if ( !ele )
                                break;
                            IGUITabControl *tabCtrl = static_cast<IGUITabControl*>(ele);
                            mLastOpenTab = tabCtrl->getActiveTab();
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
        default:
        break;
    }

    return false;
}
