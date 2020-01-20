
// CONFIGURATION UTILITY
// This application shows a GUI for adjusting Litha Engine settings.

#include "Litha.h"
#include <vector>

// This should be changed to match the other Litha Engine project's hard coded default settings,
// those passed to CreateEngine.
VariantMap PROJECT_DEFAULT_SETTINGS()
{
	VariantMap settings;
	settings["appName"] = "Puzzle Moppet";
	settings["screenWidth"] = 1024;
	settings["screenHeight"] = 768;
	settings["fullScreen"] = true;
	return settings;
}

namespace gui_ids
{
	enum
	{
		BUTTON_CANCEL = 100,
		BUTTON_SAVE_AND_EXIT = 101,
		
		COMBOBOX_SCREEN_RES = 200,
		CHECKBOX_FULL_SCREEN = 201,
		CHECKBOX_SHADERS = 202,
		CHECKBOX_POST_PROCESSING = 203
	};
	
	// Should only be called after engine has been created.
    gui::IGUIElement *getElementFromID(s32 id, gui::IGUIElement *startElement = NULL)
    {
        if (startElement == NULL)
            startElement = GetEngine()->GetIrrlichtDevice()->getGUIEnvironment()->getRootGUIElement();
        
        if (startElement->getID() == id)
            return startElement;
        
        core::list<gui::IGUIElement *>::ConstIterator begin = startElement->getChildren().begin();
        core::list<gui::IGUIElement *>::ConstIterator end = startElement->getChildren().end();
        
        for (; begin != end; begin ++)
        {
            if (gui::IGUIElement *found = getElementFromID(id, *begin))
                return found;
        }
        
        return NULL;
    }
}

gui::IGUIComboBox *comboboxScreenRes;
gui::IGUICheckBox *checkboxFullScreen;
gui::IGUICheckBox *checkboxShaders;
gui::IGUICheckBox *checkboxPostProcessing;

// Find the pointers based on IDs from the xml file.
void FindGUIElementPointers()
{
	comboboxScreenRes		= (gui::IGUIComboBox *)gui_ids::getElementFromID(gui_ids::COMBOBOX_SCREEN_RES);
	checkboxFullScreen		= (gui::IGUICheckBox *)gui_ids::getElementFromID(gui_ids::CHECKBOX_FULL_SCREEN);
	checkboxShaders			= (gui::IGUICheckBox *)gui_ids::getElementFromID(gui_ids::CHECKBOX_SHADERS);
	checkboxPostProcessing	= (gui::IGUICheckBox *)gui_ids::getElementFromID(gui_ids::CHECKBOX_POST_PROCESSING);
}


// Current state
VariantMap projectSettings;

// List of all available video modes.
Set<core::dimension2du> videoModes; // using a set avoids duplicates (different bit depths)


void InitSettings()
{
	// This is the same process used in Engine constructor
	// to determine the final settings for an app.
	
	// Find the other project's default settings.
	// It's the default engine settings overridden by the default project settings.
	projectSettings = GetEngine()->GetDefaultSettings();
	override_variantmap(projectSettings, PROJECT_DEFAULT_SETTINGS());
	
	// And then override by the settings from the config file...
	VariantMap configFileSettings = file::loadsettings(
			os::path::concat(os::getcustomappdata(projectSettings["appName"]), projectSettings["appName"] + ".ini") );
	
	if (configFileSettings.size())
		override_variantmap(projectSettings, configFileSettings);
}

// Should only be called after InitGUI has set up the videoMode list.
bool selectVideoModeInComboBox(const core::dimension2du &dim)
{
	// Search for listed video mode in combo box that matches current settings.
	for (u32 i = 0; i < videoModes.size(); i ++)
	{
		if (videoModes[i] == dim)
		{
			comboboxScreenRes->setSelected(i);
			return true;
		}
	}
	return false;
}

void InitGUI()
{
	// Find available video modes.
	
	video::IVideoModeList *vmList = GetEngine()->GetIrrlichtDevice()->getVideoModeList();
	
	// We ignore the bit depth, as the maximum available is always chosen.
	// (32 bits, when creating the Irrlicht device).
	
	for (s32 i = 0; i < vmList->getVideoModeCount(); i ++)
		videoModes.Insert( vmList->getVideoModeResolution(i) );
	
	for (u32 i = 0; i < videoModes.size(); i ++)
	{
		core::stringw resStr;
		resStr += videoModes[i].Width;
		resStr += "x";
		resStr += videoModes[i].Height;
		comboboxScreenRes->addItem(resStr.c_str());
	}
	
	
	// Now we set up the GUI depending on the previously saved settings.
	
	ASSERT( projectSettings["screenWidth"] > 0 );
	ASSERT( projectSettings["screenHeight"] > 0 );
	
	
	if (videoModes.size() == 0)
	{
		WARN << "No video modes were found...";
	}
	else
	{
		if (!selectVideoModeInComboBox( core::dimension2du(projectSettings["screenWidth"],
				projectSettings["screenHeight"]) ))
		{
			// Not found!? Fall back to the project default...
			// This would presumably only occur if the user changed their monitor or something
			// and a previously set video mode was no longer supported.
			NOTE << "VideoMode [" << projectSettings["screenWidth"] << " by " << projectSettings["screenHeight"]
					<< "] not found, falling back to project default...";
			
			// (still need to override the engine settings, to get the engine's default,
			// since the project might not have specified a video mode)
			VariantMap defaultProjectSettings = GetEngine()->GetDefaultSettings();
			override_variantmap(defaultProjectSettings, PROJECT_DEFAULT_SETTINGS());
			
			ASSERT( defaultProjectSettings["screenWidth"] > 0 );
			ASSERT( defaultProjectSettings["screenHeight"] > 0 );
			
			// Attempt to set the default.
			if (!selectVideoModeInComboBox( core::dimension2du(defaultProjectSettings["screenWidth"],
					defaultProjectSettings["screenHeight"]) ))
			{
				NOTE << "Default VideoMode [" << projectSettings["screenWidth"] << " by "
						<< projectSettings["screenHeight"] << "] not found, will use the first in list... ["
						<< videoModes[0].Width << " by " << videoModes[0].Height << "]";
				
				
				// Default is not present either!?
				// SO, just select the first in the list...
				comboboxScreenRes->setSelected(0);
			}
		}
	}
	
	checkboxFullScreen->setChecked( projectSettings["fullScreen"] );
	checkboxShaders->setChecked( projectSettings["shadersEnabled"] );
	checkboxPostProcessing->setChecked( projectSettings["postProcessingEnabled"] );
}

void SaveSettings()
{
	// First, we must update the old state (projectSettings) with newly set GUI state.
	
	if (comboboxScreenRes->getSelected() != -1)
	{
		s32 selected = comboboxScreenRes->getSelected();
		
		ASSERT( selected < videoModes.size() );
		
		projectSettings["screenWidth"] = videoModes[selected].Width;
		projectSettings["screenHeight"] = videoModes[selected].Height;
	}
	else
	{
		WARN << "no screen resolution selected...";
	}
	
	projectSettings["fullScreen"] = checkboxFullScreen->isChecked();
	projectSettings["shadersEnabled"] = checkboxShaders->isChecked();
	projectSettings["postProcessingEnabled"] = checkboxPostProcessing->isChecked();
	
	// Now write back the changes to .ini file.
	// Note that this writes *all* settings: project specific settings plus any
	// default engine settings that were not overridden by project.
	
	// Also, we don't want to store appName in the settings file.
	
	core::stringc savePath = os::path::concat(os::getcustomappdata(projectSettings["appName"]),
			projectSettings["appName"] + ".ini");
	
	projectSettings.erase("appName");
	
	if (!file::savesettings(savePath, projectSettings))
	{
		WARN << "Could not save settings: " << savePath;
	}
}

class EventReceiver : public IEventReceiver
{
	IEngine *engine;
	
public:
	EventReceiver(IEngine *engine)
	{
		this->engine = engine;
	}
	
	bool OnGUIEvent(const SEvent::SGUIEvent &event)
	{
		
		// Button clicks
		if (event.EventType == gui::EGET_BUTTON_CLICKED)
		{
			switch (event.Caller->getID())
			{
			case gui_ids::BUTTON_CANCEL:
				engine->Exit();
				return true;
			case gui_ids::BUTTON_SAVE_AND_EXIT:
				SaveSettings();
				engine->Exit();
				return true;
			}
		}
		
		return false;
	}
	
	bool OnEvent(const SEvent &event)
	{
		if (event.EventType == EET_GUI_EVENT)
			return OnGUIEvent(event.GUIEvent);
		else
			return false;
	}
};

int main(int argc, const char **argv)
{
	VariantMap settings;
	settings["appName"] = "Puzzle Moppet Configuration Utility";
	settings["windowTitle"] = "Puzzle Moppet Configuration";
	settings["screenWidth"] = 250;
	settings["screenHeight"] = 330;
#ifndef __APPLE__
	// Software renderer doesn't seem to work on Mac...
	settings["softwareMode"] = true;
#endif
	
	IEngine *engine = CreateEngine(argc, argv, &settings);
	
	IrrlichtDevice *device = engine->GetIrrlichtDevice();
	
	// Filesystem...
	// Irrlicht needs this so it can load textures from same dir as font xml.
	engine->GetIrrlichtDevice()->getFileSystem()->addFileArchive("../projects/ConfigApp/");
	
	// Show the cursor
	device->getCursorControl()->setVisible(true);
	
	// Pause World, not needed for GUI app.
	// (pausing World also disables mouse centring)
	engine->GetWorld()->Pause();
	
	// Load GUI
	gui::IGUIEnvironment *guienv = device->getGUIEnvironment();
	ASSERT( guienv->loadGUI("gui.xml") );
	
	// Get pointers to important elements within the loaded GUI.
	// Ideally in the future we'll have a layout manager and won't need the GUI
	// defined in xml.
	FindGUIElementPointers();
	
	// Default font
	guienv->getSkin()->setFont( guienv->getFont("lucida.xml") );
	
	// Change background colour...
	engine->GetRenderSystem()->SetBackgroundCol(video::SColor(0, 200,200,200));
	
	// Receive GUI events.
	// This prevents Litha Engine from receiving GUI events, but that doesn't matter.
	IEventReceiver *eventReceiver = new EventReceiver(engine);
	guienv->setUserEventReceiver(eventReceiver);
	
	// Init
	InitSettings();
	InitGUI();
	
	engine->Run();
	engine->drop();
	delete eventReceiver;
	
	return 0;
}

