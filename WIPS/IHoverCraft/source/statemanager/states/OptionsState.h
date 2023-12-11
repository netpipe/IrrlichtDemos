#ifndef OPTIONSSTATE_H_
#define OPTIONSSTATE_H_

#include <IGUIEnvironment.h>
#include <IGUIButton.h>
#include <IGUITabControl.h>
#include <IGUIComboBox.h>
#include <IGUIScrollBar.h>
#include <IGUICheckBox.h>

class OptionsState : public State<Game, irr::SEvent>
{

private:

	const void acceptChanges(Game* game) const;

	const void cancelChanges(Game* game) const;

	//gui elements for option state

	irr::gui::IGUITabControl* optionTabControl;
	irr::gui::IGUITab* videoTab;
#ifdef _SOUND
	irr::gui::IGUITab* audioTab;
#endif
	irr::gui::IGUITab* guiTab;

	//video tab
	irr::gui::IGUIComboBox* driverComboBox;
	irr::gui::IGUIComboBox* windowModeComboBox;
	irr::gui::IGUICheckBox* fullscreenCheckBox;
	irr::gui::IGUICheckBox* stencilbufferCheckBox;
	irr::gui::IGUICheckBox* vertSyncCheckBox;
#ifdef _SOUND
	//audio tab
	irr::gui::IGUIComboBox* soundDeviceComboBox;
	irr::gui::IGUICheckBox* soundCheckBox;
#endif
	//gui tab
	irr::gui::IGUIScrollBar* transparencyScrollBar;

	//buttons
	irr::gui::IGUIButton* applyButton;
	irr::gui::IGUIButton* closeButton;

	struct SVideoDriver
	{
		irr::s32 index;
		irr::video::E_DRIVER_TYPE Type;
	};

	irr::core::array<SVideoDriver> Drivers;

	struct SVideoMode
	{
		irr::s32 index;
		irr::core::dimension2di WindowSize;
		irr::u32 BitDepth;
	};

	irr::core::array<SVideoMode> Modes;

public:

	OptionsState();

	virtual ~OptionsState();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual const bool onEvent(Game* entity, const irr::SEvent& event);
};

OptionsState::OptionsState() :
	optionTabControl(0),
	videoTab(0),
#ifdef _SOUND
	audioTab(0),
#endif
	guiTab(0),

	//video tab
	driverComboBox(0),
	windowModeComboBox(0),
	fullscreenCheckBox(0),
	stencilbufferCheckBox(0),
	vertSyncCheckBox(0),
#ifdef _SOUND
	//audio tab
	soundDeviceComboBox(0),
	soundCheckBox(0),
#endif
	//gui tab
	transparencyScrollBar(0),

	applyButton(0),
	closeButton(0)
{
}

OptionsState::~OptionsState()
{
	//remove buttons
	if (closeButton) {
		closeButton->remove();
		closeButton->drop();
	}
	if (applyButton) {
		applyButton->remove();
		applyButton->drop();
	}

	//remove tabcontrol, tabs and children
	if (optionTabControl) {
		optionTabControl->remove();
		optionTabControl->drop();
	}
}

void OptionsState::onEnter(Game* game)
{
	//add tabcontrol
	this->optionTabControl = game->getGuiEnvironment()->addTabControl(
		irr::core::rect<irr::s32>(SX(200), SY(200), SX(824), SY(600)),
		0, true, true, -1);
	this->optionTabControl->grab();

	//add tabs
	this->videoTab = this->optionTabControl->addTab(L"Video", -1);
#ifdef _SOUND
	this->audioTab = this->optionTabControl->addTab(L"Audio", -1);
#endif
	this->guiTab = this->optionTabControl->addTab(L"Gui", -1);

	// drivers
	this->driverComboBox = game->getGuiEnvironment()->addComboBox(irr::core::rect<irr::s32>(SX(5), SY(23), SX(197), SY(43)), videoTab, 1);
	{
		SVideoDriver s;

		s.index = this->driverComboBox->addItem(L"OpenGL");
		s.Type  = irr::video::EDT_OPENGL;
		this->Drivers.push_back(s);

		s.index = this->driverComboBox->addItem(L"Burning Video");
		s.Type  = irr::video::EDT_BURNINGSVIDEO;
		this->Drivers.push_back(s);

		s.index = this->driverComboBox->addItem(L"Software");
		s.Type  = irr::video::EDT_SOFTWARE;
		this->Drivers.push_back(s);

		// set the current driver as the default
		irr::u32 n;
		for (n = 0; n < this->Drivers.size(); ++n)
		{
			if (game->getConfiguration().getIrrlichtParams().DriverType == this->Drivers[n].Type)
			{
				this->driverComboBox->setSelected(this->Drivers[n].index);
				break;
			}
		}
	}

	// resolution/bit depth
	this->windowModeComboBox = game->getGuiEnvironment()->addComboBox(irr::core::rect<irr::s32>(SX(5), SY(46), SX(197), SY(66)), videoTab, 2);
	{
		irr::video::IVideoModeList* modes = game->getDevice()->getVideoModeList();

		irr::s32 m;
		for (m = 0; m < modes->getVideoModeCount(); ++m)
		{
			const irr::core::dimension2di res = modes->getVideoModeResolution(m);
			if (res.Width < 640 || res.Height < 480)
				continue;

			const irr::s32 bits = modes->getVideoModeDepth(m);
			if (bits < 16)
				continue;

			wchar_t option[64];
			swprintf(option, 64, L"%u x %u @ %u bits", res.Width, res.Height, bits);

			SVideoMode s;

			s.index = this->windowModeComboBox->addItem(option);
			s.BitDepth = bits;
			s.WindowSize = res;

			this->Modes.push_back(s);
		}

		// set the current window mode as the default
		irr::u32 n;
		for (n = 0; n < Modes.size(); ++n)
		{
			if (game->getConfiguration().getIrrlichtParams().Bits == this->Modes[n].BitDepth &&
				game->getConfiguration().getIrrlichtParams().WindowSize == this->Modes[n].WindowSize)
			{
				this->windowModeComboBox->setSelected(this->Modes[n].index);
				break;
			}
		}
	}

	// full screen checkbox
	this->fullscreenCheckBox = game->getGuiEnvironment()->addCheckBox(
		game->getConfiguration().getIrrlichtParams().Fullscreen,
		irr::core::rect<irr::s32>(SX(5), SY(69), SX(199), SY(89)),
		this->videoTab, 3, L"Fullscreen"
	);

	// stencilbuffer checkbox
	this->stencilbufferCheckBox = game->getGuiEnvironment()->addCheckBox(
			game->getConfiguration().getIrrlichtParams().Stencilbuffer,
		irr::core::rect<irr::s32>(SX(5), SY(92), SX(199), SY(112)),
		this->videoTab, 4, L"Realtime shadows"
	);

	// vsync checkbox
	this->vertSyncCheckBox = game->getGuiEnvironment()->addCheckBox(
			game->getConfiguration().getIrrlichtParams().Vsync,
		irr::core::rect<irr::s32>(SX(5), SY(115), SX(199), SY(135)),
		this->videoTab, 5, L"Vertical sync"
	);

#ifdef _SOUND
	//audio devices combo box to lists all available audio devices.
	//irrlklang should automatically select the default device, so no effect, yet
	{
		this->soundDeviceComboBox = game->getGuiEnvironment()->addComboBox(
			irr::core::rect<irr::s32>(SX(5), SY(23), SX(197), SY(43)), this->audioTab, 1);

		irrklang::ISoundDeviceList* list = irrklang::createSoundDeviceList();
		for (irrklang::ik_s32 i = 0; i < list->getDeviceCount(); ++i)
		{
			this->soundDeviceComboBox->addItem(irr::core::stringw(list->getDeviceDescription(i)).c_str());
		}

	}

	// sound checkbox
	this->soundCheckBox = game->getGuiEnvironment()->addCheckBox(
			game->getConfiguration().isSoundEnabled(),
		irr::core::rect<irr::s32>(SX(5), SY(69), SX(199), SX(89)),
		this->audioTab, 6, L"Enable sound"
	);
#endif

	//transparency scroll bar
	{
		this->transparencyScrollBar = game->getGuiEnvironment()->addScrollBar(
			true, irr::core::rect<irr::s32>(SX(10), SY(10), SX(600), SY(30)), this->guiTab, -1);
		//adjust scrollbars
		const irr::u8 max = 255;
		const irr::u8 small = 1;
		const irr::u8 large = 25;

		this->transparencyScrollBar->setToolTipText(L"Alpha value of gui");
		this->transparencyScrollBar->setMax(max);
		this->transparencyScrollBar->setSmallStep(small);
		this->transparencyScrollBar->setLargeStep(large);
		this->transparencyScrollBar->setPos(game->getConfiguration().getGuiTransparency());
	}

	// apply button
	this->applyButton = game->getGuiEnvironment()->addButton(
		irr::core::rect<irr::s32>(SX(50), SY(50), SX(200), SY(100)), //dimension
		0, // parent
		7, // id
		L"[F1] APPLY" //text
	);
	this->applyButton->grab();

	// close button
	this->closeButton = game->getGuiEnvironment()->addButton(
		irr::core::rect<irr::s32>(SX(250), SY(50), SX(400), SY(100)),
		0,
		8,
		L"[ESC] BACK"
	);
	this->closeButton->grab();
}

void OptionsState::onLeave(Game* game)
{
	this->closeButton->remove();
	this->closeButton->drop();
	this->closeButton = 0;

	this->applyButton->remove();
	this->applyButton->drop();
	this->applyButton = 0;

	// remove our window and children
	this->optionTabControl->remove();
	this->optionTabControl->drop();
	this->optionTabControl = 0;

	this->videoTab = 0;
#ifdef _SOUND
	this->audioTab = 0;
#endif
	this->guiTab = 0;

	this->fullscreenCheckBox = 0;
	this->vertSyncCheckBox = 0;
	this->windowModeComboBox = 0;
	this->driverComboBox = 0;
#ifdef _SOUND
	this->soundDeviceComboBox = 0;
	this->soundCheckBox = 0;
#endif
	this->transparencyScrollBar = 0;
}

const bool OptionsState::onEvent(Game* game, const irr::SEvent& event)
{
	switch (event.EventType)
	{

	case irr::EET_GUI_EVENT:
		if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
		{
			if (event.GUIEvent.Caller == this->applyButton)
			{
				acceptChanges(game);
				return true;
			}

			else if (event.GUIEvent.Caller == this->closeButton)
			{
				cancelChanges(game);
				return true;
			}
		}

		else if (event.GUIEvent.EventType ==  irr::gui::EGET_SCROLL_BAR_CHANGED)
		{
			//update config object with new value
			const irr::s32 val = this->transparencyScrollBar->getPos();
			game->getConfiguration().setGuiTransparency(val);

			// change gui transparency
			irr::gui::IGUISkin* currentSkin = game->getGuiEnvironment()->getSkin();
			irr::u32 c;
			for (c = 0; c < irr::gui::EGDC_COUNT; ++c)
			{
				irr::video::SColor col = currentSkin->getColor(irr::gui::EGUI_DEFAULT_COLOR(c));
				col.setAlpha(val);
				currentSkin->setColor(irr::gui::EGUI_DEFAULT_COLOR(c), col);
			}

			return true;
		}

		else if (event.GUIEvent.EventType ==  irr::gui::EGET_CHECKBOX_CHANGED)
		{
#ifdef _SOUND
			if (event.GUIEvent.Caller == this->soundCheckBox)
			{
				//turn off all sounds if user unchecked the box
				if (!((irr::gui::IGUICheckBox*)event.GUIEvent.Caller)->isChecked())
				{
					if (game->getSoundEngine())
						game->getSoundEngine()->stopAllSounds();

					game->getConfiguration().setSoundEnabled(false);
					return true;
				}
				else
				{
					game->getConfiguration().setSoundEnabled(true);
					return true;
				}
			}
#endif
		}

		break;

	case irr::EET_KEY_INPUT_EVENT:
	{
		if (!event.KeyInput.PressedDown)
			if (event.KeyInput.Key == irr::KEY_ESCAPE)
			{
				cancelChanges(game);
				return true;
			}
			else if (event.KeyInput.Key == irr::KEY_F1)
			{
				acceptChanges(game);
				return true;
			}
	}
	break;

	default: return false;
	}

	return false;
}

const void OptionsState::acceptChanges(Game* game) const
{
	//get desired device configuration
	irr::SIrrlichtCreationParameters params;
	params.DriverType = this->Drivers[driverComboBox->getSelected()].Type;
	params.WindowSize = this->Modes[windowModeComboBox->getSelected()].WindowSize;
	params.Bits = this->Modes[windowModeComboBox->getSelected()].BitDepth;
	params.Fullscreen = this->fullscreenCheckBox->isChecked();
	params.Stencilbuffer = this->stencilbufferCheckBox->isChecked();
	params.Vsync = this->vertSyncCheckBox->isChecked();

	//update configuration object
	game->getConfiguration().setIrrlichtParams(params);
#ifdef _SOUND
	game->getConfiguration().setSoundEnabled(this->soundCheckBox->isChecked());
#endif

	//write config
	game->getConfiguration().write();
}

const void OptionsState::cancelChanges(Game* game) const
{
	game->setState(game->getPreviousState());
}

#endif /*OPTIONSSTATE_H_*/
