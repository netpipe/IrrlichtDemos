#ifndef CONFIGURECOMPANY_H_
#define CONFIGURECOMPANY_H_

class EditPlayerState : public State<Game, irr::SEvent>
{
private:

	///Just a label.
	irr::gui::IGUIStaticText* playerNameText;
	
	///An editbox to change the name of the player.
	irr::gui::IGUIEditBox* playerNameEditBox;



	irr::gui::IGUIStaticText* colorText;
	
	irr::gui::IGUIScrollBar* alphaScrollBar;

	irr::gui::IGUIScrollBar* redScrollBar;

	irr::gui::IGUIScrollBar* greenScrollBar;

	irr::gui::IGUIScrollBar* blueScrollBar;


	irr::gui::IGUIButton* acceptButton;

	irr::gui::IGUIButton* menuButton;

	///Updates the configuration object with the new values. Does some basic error checking too
	const void acceptChanges(Game* game) const;

	const void updateColorText() const;
	
public:

	EditPlayerState();

	virtual ~EditPlayerState();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual bool const onEvent(Game* entity, const irr::SEvent& event);
};

EditPlayerState::EditPlayerState() :
	playerNameText(0),
	playerNameEditBox(0),
	colorText(0),
	alphaScrollBar(0),
	redScrollBar(0),
	greenScrollBar(0),
	blueScrollBar(0),
	acceptButton(0),
	menuButton(0)
{
}

EditPlayerState::~EditPlayerState()
{

	if (this->menuButton) {
		this->menuButton->remove();
		this->menuButton->drop();
	}

	if (this->acceptButton) {
		this->acceptButton->remove();
		this->acceptButton->drop();
	}

	if (this->blueScrollBar) {
		this->blueScrollBar->remove();
		this->blueScrollBar->drop();
	}
	
	if (this->greenScrollBar) {
		this->greenScrollBar->remove();
		this->greenScrollBar->drop();
	}
	
	if (this->redScrollBar) {
		this->redScrollBar->remove();
		this->redScrollBar->drop();
	}
	
	if (this->alphaScrollBar) {
		this->alphaScrollBar->remove();
		this->alphaScrollBar->drop();
	}

	if (this->colorText) {
		this->colorText->remove();
		this->colorText->drop();
	}


	if (this->playerNameEditBox) {
		this->playerNameEditBox->remove();
		this->playerNameEditBox->drop();
	}
	
	if (this->playerNameText) {
		this->playerNameText->remove();
		this->playerNameText->drop();
	}
}

void EditPlayerState::onEnter(Game* game)
{
	irr::gui::IGUIEnvironment* guienv = game->getGuiEnvironment();

	//add player name static text
	this->playerNameText = guienv->addStaticText(
		L"Your name",
		irr::core::rect<irr::s32>(SX(50), SY(50), SX(250), SY(80)),
		true, //border
		true, //wordwrap
		0, //parent
		-1, //id
		true //fill background
	);
	this->playerNameText->grab();

	// add edit box for playerName
	this->playerNameEditBox = guienv->addEditBox(
		game->getConfiguration().getPlayerName().c_str(), //text
		irr::core::rect<irr::s32>(SX(300), SY(50), SX(500), SY(80)), //rect
		true, //border
		0, //parent
		-1 //id
	);
	this->playerNameEditBox->grab();
	
	

	//add scrollbars for color selection
	{
		this->alphaScrollBar = guienv->addScrollBar(
			true, //horizontal
			irr::core::rect<irr::s32>(SX(620), SY(50), SX(800), SY(70)), //rectangle
			0, //parent
			-1 //id
		);
		this->alphaScrollBar->grab();
	
		this->redScrollBar = guienv->addScrollBar(
			true, //horizontal
			irr::core::rect<irr::s32>(SX(620), SY(80), SX(800), SY(100)), //rectangle
			0, //parent
			-1 //id
		);
		this->redScrollBar->grab();
		
		this->greenScrollBar = guienv->addScrollBar(
			true, //horizontal
			irr::core::rect<irr::s32>(SX(620), SY(110), SX(800), SY(130)), //rectangle
			0, //parent
			-1 //id
		);
		this->greenScrollBar->grab();
		
		this->blueScrollBar = guienv->addScrollBar(
			true, //horizontal
			irr::core::rect<irr::s32>(SX(620), SY(140), SX(800), SY(160)), //rectangle
			0, //parent
			-1 //id
		);
		this->blueScrollBar->grab();


		//adjust scrollbars
		const irr::u8 max = 255;
		const irr::u8 small = 1;
		const irr::u8 large = 25;

		this->alphaScrollBar->setToolTipText(L"Alpha value of ship color");
		this->alphaScrollBar->setMax(max);
		this->alphaScrollBar->setSmallStep(small);
		this->alphaScrollBar->setLargeStep(large);

		this->redScrollBar->setToolTipText(L"Red value of ship color");
		this->redScrollBar->setMax(max);
		this->redScrollBar->setSmallStep(small);
		this->redScrollBar->setLargeStep(large);

		this->greenScrollBar->setToolTipText(L"Green value of ship color");
		this->greenScrollBar->setMax(max);
		this->greenScrollBar->setSmallStep(small);
		this->greenScrollBar->setLargeStep(large);

		this->blueScrollBar->setToolTipText(L"Blue value of ship color");
		this->blueScrollBar->setMax(max);
		this->blueScrollBar->setSmallStep(small);
		this->blueScrollBar->setLargeStep(large);


		// set scrollbar positions to values of company color
		const irr::video::SColor& shipColor = game->getConfiguration().getShipColor();
		this->alphaScrollBar->setPos(shipColor.getAlpha());
		this->redScrollBar->setPos(shipColor.getRed());
		this->greenScrollBar->setPos(shipColor.getGreen());
		this->blueScrollBar->setPos(shipColor.getBlue());


		//add the color text
		this->colorText = guienv->addStaticText(
			L"",irr::core::rect<irr::s32>(SX(850), SY(50), SX(1015), SY(150)), true, true, 0, -1, true);
		this->colorText->grab();
		this->updateColorText();

	}

	
	this->acceptButton = guienv->addButton(
		irr::core::rect<irr::s32>(SX(250), SY(350), SX(450), SY(400)),
		0, -1, L"[F1] APPLY CHANGES");
	this->acceptButton->grab();

	this->menuButton = guienv->addButton(
		irr::core::rect<irr::s32>(SX(550), SY(350), SX(750), SY(400)),
		0, -1, L"[ESC] BACK TO MENU");
	this->menuButton->grab();
}

void EditPlayerState::onLeave(Game* game)
{
	this->menuButton->remove();
	this->menuButton->drop();
	this->menuButton = 0;

	this->acceptButton->remove();
	this->acceptButton->drop();
	this->acceptButton = 0;

	this->blueScrollBar->remove();
	this->blueScrollBar->drop();
	this->blueScrollBar = 0;

	this->greenScrollBar->remove();
	this->greenScrollBar->drop();
	this->greenScrollBar = 0;

	this->redScrollBar->remove();
	this->redScrollBar->drop();
	this->redScrollBar = 0;
	
	this->alphaScrollBar->remove();
	this->alphaScrollBar->drop();
	this->alphaScrollBar = 0;

	this->colorText->remove();
	this->colorText->drop();
	this->colorText = 0;

	this->playerNameEditBox->remove();
	this->playerNameEditBox->drop();
	this->playerNameEditBox = 0;
	
	this->playerNameText->remove();
	this->playerNameText->drop();
	this->playerNameText = 0;
}

const bool EditPlayerState::onEvent(Game* game, const irr::SEvent& event)
{
	switch (event.EventType)
	{

	case irr::EET_KEY_INPUT_EVENT:
		if (!event.KeyInput.PressedDown)
			switch(event.KeyInput.Key)
			{
				case irr::KEY_F1: this->acceptChanges(game); return true;
				case irr::KEY_ESCAPE: game->setState(game->getPreviousState()); return true;
				default: return false;
			}
		break;

	case irr::EET_GUI_EVENT:
		if (event.GUIEvent.EventType == irr::gui::EGET_BUTTON_CLICKED)
		{
			if (event.GUIEvent.Caller == this->acceptButton)
			{
				this->acceptChanges(game);
				return true;
			}

			else if (event.GUIEvent.Caller == this->menuButton)
			{
				game->setState(game->getPreviousState());
				return true;
			}
		}

		else if (event.GUIEvent.EventType ==  irr::gui::EGET_SCROLL_BAR_CHANGED)
		{
			//a scrollbar has been moved. so construct a new color and update gui
			this->updateColorText();
			return true;
		}
		break;
		
	default: return false;
	}

	return false;
}

const void EditPlayerState::acceptChanges(Game* game) const
{
	irr::core::stringw newPlayerName(this->playerNameEditBox->getText());
	irr::video::SColor newShipColor(this->alphaScrollBar->getPos(),
		this->redScrollBar->getPos(), this->greenScrollBar->getPos(), this->blueScrollBar->getPos());


	//some basic error checking
	if (newPlayerName == L"" || newPlayerName == L"." || newPlayerName == L"..")
	{
		newPlayerName = L"Jack";
		this->playerNameEditBox->setText(newPlayerName.c_str()); //we corrected the new name, so update gui
	}


	Configuration& config = game->getConfiguration();

	//save player and company settings
	config.setPlayerName(newPlayerName);
	config.setShipColor(newShipColor);

	//write config to file
	config.write();
}

const void EditPlayerState::updateColorText() const
{
	const irr::video::SColor newColor(
		this->alphaScrollBar->getPos(),
		this->redScrollBar->getPos(),
		this->greenScrollBar->getPos(),
		this->blueScrollBar->getPos()
	);

	wchar_t ct[64];
	swprintf(ct, 64, L"Alpha = %u\nRed = %u\nGreen = %u\nBlue = %u",
			newColor.getAlpha(), newColor.getRed(), newColor.getGreen(), newColor.getBlue() 
	);

	this->colorText->setText(ct);
	this->colorText->setOverrideColor(newColor);
}

#endif /*CONFIGURECOMPANY_H_*/
