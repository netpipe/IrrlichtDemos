// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_menu_graphics.h"
#include "gui_menu_options.h"
#include "../gui.h"
#include "../main.h"
#include "../profiles.h"
#include "../string_table.h"
#include "../irrlicht_manager.h"
#include "../helper_str.h"
#include "../config.h"
#include "../tinyxml/tinyxml.h"

using namespace irr;
using namespace gui;

GuiMenuGraphics::GuiMenuGraphics(const Config& config)
    : GuiDialog(config)
    , mSliderResolution(0)
    , mSliderFilter(0)
    , mSliderAntialias(0)
    , mSliderLights(0)
    , mSliderTextureResolution(0)
    , mSliderParticles(0)
    , mSliderVsync(0)
    , mActiveProfile(0)
    , mStaticInfo(0)
    , mStaticRestartInfo(0)
{
    SetSuppressSceneRendering(true);
}

GuiMenuGraphics::~GuiMenuGraphics()
{
    RemoveFunctors();
}

bool GuiMenuGraphics::Load(const char* filename_, bool reloadLast_)
{
    RemoveFunctors();
    bool ok = GuiDialog::Load(filename_, reloadLast_);
    if ( ok )
    {
        AddGuiEventFunctor( GetIdForName(std::string("sld_resolution")), new EventFunctor<GuiMenuGraphics>(this, &GuiMenuGraphics::OnSliderResolution) );
        AddGuiEventFunctor( GetIdForName(std::string("sld_filter")), new EventFunctor<GuiMenuGraphics>(this, &GuiMenuGraphics::OnSliderFilter) );
        AddGuiEventFunctor( GetIdForName(std::string("sld_antialias")), new EventFunctor<GuiMenuGraphics>(this, &GuiMenuGraphics::OnSliderAntialias) );
        AddGuiEventFunctor( GetIdForName(std::string("sld_lights")), new EventFunctor<GuiMenuGraphics>(this, &GuiMenuGraphics::OnSliderLights) );
        AddGuiEventFunctor( GetIdForName(std::string("sld_texture_resolution")), new EventFunctor<GuiMenuGraphics>(this, &GuiMenuGraphics::OnSliderTextureResolution) );
        AddGuiEventFunctor( GetIdForName(std::string("sld_particles")), new EventFunctor<GuiMenuGraphics>(this, &GuiMenuGraphics::OnSliderParticles) );
        AddGuiEventFunctor( GetIdForName(std::string("sld_vsync")), new EventFunctor<GuiMenuGraphics>(this, &GuiMenuGraphics::OnSliderVsync) );
        AddGuiEventFunctor( GetIdForName(std::string("id_accept")), new EventFunctor<GuiMenuGraphics>(this, &GuiMenuGraphics::OnButtonAccept) );
        AddGuiEventFunctor( GetIdForName(std::string("id_cancel")), new EventFunctor<GuiMenuGraphics>(this, &GuiMenuGraphics::OnButtonCancel) );

        IGUIElement * root = GetDialogParent();
        if ( !root )
            return false;

        std::string errorMsg("GuiMenuGraphics::Load");
        mSliderResolution = static_cast<IGUITextSlider*>(GetElementByName(root, "sld_resolution", errorMsg));
        if ( mSliderResolution )
        {
            mSliderResolution->clearTexts();
            mSliderResolution->addText( APP.GetStringTable()->Get("id_res_auto").c_str() );
            const std::vector<VideoMode>& videoModes = APP.GetIrrlichtManager()->GetVideoModes();
            for ( unsigned int i=0; i<videoModes.size(); ++i )
            {
                VideoMode mode = videoModes[i];
                std::wstring resString;
                resString += ExtStr::int_to_wstr(mode.mResolution.Width);
                resString += L"X";
                resString += ExtStr::int_to_wstr(mode.mResolution.Height);
//                resString += L"X";
//                resString += ExtStr::int_to_wstr(mode.mDepth);
                mSliderResolution->addText( resString.c_str() );
            }
        }

        mSliderFilter = static_cast<IGUITextSlider*>(GetElementByName(root, "sld_filter", errorMsg));
        if ( mSliderFilter )
        {
            mSliderFilter->clearTexts();
            mSliderFilter->addText( APP.GetStringTable()->Get("id_filter_bilinear").c_str() );
            mSliderFilter->addText( APP.GetStringTable()->Get("id_filter_trilinear").c_str() );
            mSliderFilter->addText( APP.GetStringTable()->Get("id_filter_anisotropic").c_str() );
        }

        mSliderAntialias = static_cast<IGUITextSlider*>(GetElementByName(root, "sld_antialias", errorMsg));
        if ( mSliderAntialias )
        {
            mSliderAntialias->clearTexts();
            mSliderAntialias->addText( APP.GetStringTable()->Get("id_on").c_str() );
            mSliderAntialias->addText( APP.GetStringTable()->Get("id_off").c_str() );
        }

        mSliderLights = static_cast<IGUITextSlider*>(GetElementByName(root, "sld_lights", errorMsg));
        if ( mSliderLights )
        {
            mSliderLights->clearTexts();
            mSliderLights->addText( APP.GetStringTable()->Get("id_light_static").c_str() );
            mSliderLights->addText( APP.GetStringTable()->Get("id_light_dynamic").c_str() );
        }

        mSliderTextureResolution = static_cast<IGUITextSlider*>(GetElementByName(root, "sld_texture_resolution", errorMsg));
        if ( mSliderTextureResolution )
        {
			mSliderTextureResolution->clearTexts();
			mSliderTextureResolution->addText( APP.GetStringTable()->Get("id_tex_low").c_str() );
			mSliderTextureResolution->addText( APP.GetStringTable()->Get("id_tex_medium").c_str() );
			mSliderTextureResolution->addText( APP.GetStringTable()->Get("id_tex_high").c_str() );
        }

        mSliderParticles = static_cast<IGUITextSlider*>(GetElementByName(root, "sld_particles", errorMsg));
        if ( mSliderParticles )
        {
            mSliderParticles->clearTexts();
            mSliderParticles->addText( APP.GetStringTable()->Get("id_on").c_str() );
            mSliderParticles->addText( APP.GetStringTable()->Get("id_weather_off").c_str() );
            mSliderParticles->addText( APP.GetStringTable()->Get("id_all_off").c_str() );
        }

        mSliderVsync = static_cast<IGUITextSlider*>(GetElementByName(root, "sld_vsync", errorMsg));
        if ( mSliderVsync )
        {
            mSliderVsync->clearTexts();
            mSliderVsync->addText( APP.GetStringTable()->Get("id_on").c_str() );
            mSliderVsync->addText( APP.GetStringTable()->Get("id_off").c_str() );
        }

        mActiveProfile = static_cast<IGUIStaticText*>(GetElementByName(root, "act_profile2", errorMsg));
        mStaticInfo = static_cast<IGUIStaticText*>(GetElementByName(root, "info", errorMsg));
        mStaticRestartInfo = static_cast<IGUIStaticText*>(GetElementByName(root, "id_gpxinfo", errorMsg));
    }
    return ok;
}

void GuiMenuGraphics::RemoveFunctors()
{
    if ( !IsLoaded() )
        return;

    RemoveGuiEventFunctor( GetIdForName(std::string("sld_resolution")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("sld_filter")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("sld_antialias")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("sld_lights")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("sld_texture_resolution")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("sld_particles")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("sld_vsync")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_accept")) );
    RemoveGuiEventFunctor( GetIdForName(std::string("id_cancel")) );
}

void GuiMenuGraphics::AddNeedRestartCheck(irr::gui::IGUITextSlider* slider)
{
	if (slider)
	{
		NeedsRestart needRestart;
		needRestart.mTextSlider = slider;
		needRestart.mOldTextId = slider->getCurrentTextId();
		mNeedsRestart.push_back(needRestart);
	}
}

bool GuiMenuGraphics::CheckRestartNeeded(bool updateText)
{
	bool needRestart = false;
	for ( size_t i=0; i<mNeedsRestart.size(); ++i )
	{
		if ( mNeedsRestart[i].mTextSlider->getCurrentTextId() != mNeedsRestart[i].mOldTextId )
		{
			needRestart = true;
			break;
		}
	}

	if (updateText && mStaticRestartInfo)
	{
		mStaticRestartInfo->setVisible(needRestart);
	}

	return needRestart;
}

void GuiMenuGraphics::Show()
{
    GuiDialog::Show();

    if (!APP.GetConfig())
		return;
    const Config& config = *APP.GetConfig();

    mNeedsRestart.clear();
    if ( mSliderResolution )
    {
        if ( config.GetUseScreenResolution() )
        {
            mSliderResolution->setCurrentTextId(0);
        }
        else
        {
            int index = APP.GetIrrlichtManager()->GetVideoModeIndex( config.GetResolutionX(), config.GetResolutionY());
            if ( index < 0 )
            {
                mSliderResolution->setCurrentTextId(0);
            }
            else
            {
                mSliderResolution->setCurrentTextId(index+1);
            }
        }
        AddNeedRestartCheck(mSliderResolution);
    }
    if ( mSliderFilter )
    {
        if ( config.GetAnisotropic() )
        {
            mSliderFilter->setCurrentTextId(2);
        }
        else if ( config.GetTriLinear() )
        {
            mSliderFilter->setCurrentTextId(1);
        }
        else
        {
            mSliderFilter->setCurrentTextId(0);
        }
        AddNeedRestartCheck(mSliderFilter);
    }
    if ( mSliderAntialias )
    {
        if ( config.GetAntiAlias() )
        {
            mSliderAntialias->setCurrentTextId(0);
        }
        else
        {
            mSliderAntialias->setCurrentTextId(1);
        }
        AddNeedRestartCheck(mSliderAntialias);
    }
    if ( mSliderLights )
    {
		mSliderLights->setCurrentTextId((s32)config.GetLightMode());
    }
    if ( mSliderTextureResolution )
    {
		switch ( config.GetTextureDownscaleDivider() )
		{
			case 1: mSliderTextureResolution->setCurrentTextId(2); 	break;
			case 2: mSliderTextureResolution->setCurrentTextId(1);	break;
			case 4: mSliderTextureResolution->setCurrentTextId(0);	break;
			default:
				if ( config.GetTextureDownscaleDivider() < 1 )
					mSliderTextureResolution->setCurrentTextId(2);
				else
					mSliderTextureResolution->setCurrentTextId(0);
		}
		AddNeedRestartCheck(mSliderTextureResolution);
    }
    if ( mSliderParticles )
    {
		mSliderParticles->setCurrentTextId((s32)config.GetParticleMode());
    }
    if ( mSliderVsync )
    {
        if ( config.GetVSync() )
        {
            mSliderVsync->setCurrentTextId(0);
        }
        else
        {
            mSliderVsync->setCurrentTextId(1);
        }
        AddNeedRestartCheck(mSliderVsync);
    }

    if ( mActiveProfile )
    {
        PlayerProfile* profile = APP.GetProfileManager()->GetSelectedProfile();
        if ( profile )
        {
            mActiveProfile->setText( profile->mName.c_str() );
        }
        else
        {
            mActiveProfile->setText( APP.GetStringTable()->Get("id_showactiveprofile").c_str() );
        }
    }

    // Show text of top slider
    if (mSliderResolution)
		mStaticInfo->setText( APP.GetStringTable()->Get("id_gpxinfo_screenres").c_str() );
	else if (mSliderLights)
		mStaticInfo->setText( APP.GetStringTable()->Get("id_gpxinfo_lights").c_str() );

    CheckRestartNeeded();
}

bool GuiMenuGraphics::OnSliderResolution(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_ELEMENT_FOCUSED )
    {
        mStaticInfo->setText( APP.GetStringTable()->Get("id_gpxinfo_screenres").c_str() );
    }
    CheckRestartNeeded();
    return false;
}

bool GuiMenuGraphics::OnSliderFilter(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_ELEMENT_FOCUSED )
    {
        mStaticInfo->setText( APP.GetStringTable()->Get("id_gpxinfo_filter").c_str() );
    }
    CheckRestartNeeded();
    return false;
}

bool GuiMenuGraphics::OnSliderAntialias(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_ELEMENT_FOCUSED )
    {
        mStaticInfo->setText( APP.GetStringTable()->Get("id_gpxinfo_aa").c_str() );
    }
    CheckRestartNeeded();
    return false;
}

bool GuiMenuGraphics::OnSliderLights(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_ELEMENT_FOCUSED )
    {
        mStaticInfo->setText( APP.GetStringTable()->Get("id_gpxinfo_lights").c_str() );
    }
    CheckRestartNeeded();
    return false;
}

bool GuiMenuGraphics::OnSliderTextureResolution(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_ELEMENT_FOCUSED )
    {
        mStaticInfo->setText( APP.GetStringTable()->Get("id_gpxinfo_tex_resolution").c_str() );
    }
    CheckRestartNeeded();
    return false;
}

bool GuiMenuGraphics::OnSliderParticles(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_ELEMENT_FOCUSED )
    {
        mStaticInfo->setText( APP.GetStringTable()->Get("id_gpxinfo_particles").c_str() );
    }
    CheckRestartNeeded();
    return false;
}

bool GuiMenuGraphics::OnSliderVsync(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_ELEMENT_FOCUSED )
    {
        mStaticInfo->setText( APP.GetStringTable()->Get("id_gpxinfo_vsync").c_str() );
    }
    CheckRestartNeeded();
    return false;
}

bool GuiMenuGraphics::OnButtonAccept(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        TiXmlElement * eleGraphics = APP.GetConfig()->GetGraphicSettings();
        if ( eleGraphics )
        {
			if (mSliderResolution)
			{
				int index = mSliderResolution->getCurrentTextId();
				if ( index <= 0 )
				{
					eleGraphics->SetAttribute("use_screen_res", 1);
				}
				else
				{
					--index;
					VideoMode mode = APP.GetIrrlichtManager()->GetVideoModes()[index];
					eleGraphics->SetAttribute("use_screen_res", 0);
					eleGraphics->SetAttribute("res_x", mode.mResolution.Width);
					eleGraphics->SetAttribute("res_y", mode.mResolution.Height);
				}
			}
			if (mSliderFilter)
			{
				if ( mSliderFilter->getCurrentTextId() == 2 )
				{
					eleGraphics->SetAttribute("bi", 1);
					eleGraphics->SetAttribute("tri", 0);
					eleGraphics->SetAttribute("ani", 1);
				}
				else if ( mSliderFilter->getCurrentTextId() == 1 )
				{
					eleGraphics->SetAttribute("bi", 0);
					eleGraphics->SetAttribute("tri", 1);
					eleGraphics->SetAttribute("ani", 0);
				}
				else
				{
					eleGraphics->SetAttribute("bi", 1);
					eleGraphics->SetAttribute("tri", 0);
					eleGraphics->SetAttribute("ani", 0);
				}
			}
			if (mSliderAntialias)
			{
				if ( mSliderAntialias->getCurrentTextId() == 0 )
				{
					eleGraphics->SetAttribute("aa", 1);
				}
				else
				{
					eleGraphics->SetAttribute("aa", 0);
				}
			}

			if (mSliderLights)
				eleGraphics->SetAttribute("light", mSliderLights->getCurrentTextId());

			if (mSliderTextureResolution)
			{
				int divider = 1;
				switch ( mSliderTextureResolution->getCurrentTextId() )
				{
					case 0: divider = 4; break;
					case 1: divider = 2; break;
					case 2: divider = 1; break;
					default: divider = 1;
				}
				eleGraphics->SetAttribute("tex_div", divider);
			}

            if (mSliderParticles)
				eleGraphics->SetAttribute("particles", mSliderParticles->getCurrentTextId());

			if (mSliderVsync)
			{
				if ( mSliderVsync->getCurrentTextId() == 0 )
				{
					eleGraphics->SetAttribute("vsync", 1);
				}
				else
				{
					eleGraphics->SetAttribute("vsync", 0);
				}
			}

            APP.GetConfig()->Save();
            APP.GetConfig()->UpdateCache();
        }

        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuOptions() );
    }

    return false;
}

bool GuiMenuGraphics::OnButtonCancel(const irr::SEvent &event_)
{
    if ( event_.GUIEvent.EventType == EGET_BUTTON_CLICKED )
    {
        APP.GetGui()->PlayOverrideClickSound(GS_CLICK_CANCEL);
        APP.GetGui()->SetActiveDialog( APP.GetGui()->GetGuiMenuOptions() );
    }

    return false;
}
