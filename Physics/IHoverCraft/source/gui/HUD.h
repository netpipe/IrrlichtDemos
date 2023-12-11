#ifndef HUD_H_
#define HUD_H_

#include "Healthbar.h"
#include "Compass.h"

/**
 * Class to display a head up display.
 * 
 * Currently is displays
 *  - the positon of the ship
 *  - a compass
 * 
 * 
 */
class HUD
{
	
private:

	Healthbar* healtbar;
	
	Compass* pgCompass;

	irr::gui::IGUIStaticText* infoText;
	
public:

	HUD(Game* game) :
		healtbar(0), pgCompass(0), infoText(0)
	{
		// add a compass
		const irr::core::dimension2di compassSize(128, 128);
		
		irr::core::rect<irr::s32> gCompassRect;
		gCompassRect.UpperLeftCorner.X = SX(880);
		gCompassRect.UpperLeftCorner.Y = SY(20);
		gCompassRect.LowerRightCorner.X = SX(880) + compassSize.Width;
		gCompassRect.LowerRightCorner.Y = SY(20) + compassSize.Height;

		pgCompass = new Compass(gCompassRect, game->getGuiEnvironment(), game->getGuiEnvironment()->getRootGUIElement());
		pgCompass->SetCompassBodyTexture(game->getVideoDriver()->getTexture("media/images/compass/compass_body.tga"));
		pgCompass->SetCompassNeedleTexture(game->getVideoDriver()->getTexture("media/images/compass/compass_needle.tga"));

		//add info text
		this->infoText = game->getGuiEnvironment()->addStaticText(
			L"", irr::core::rect<irr::s32>(SX(20), SY(20), SX(300), SY(80)),
			true, true, 0, -1, true);
		this->infoText->setTextAlignment(irr::gui::EGUIA_CENTER, irr::gui::EGUIA_CENTER);
		this->infoText->grab();
		
		//add healthbar
		this->healtbar = new Healthbar(game);
	}

	~HUD()
	{
		if (infoText)
		{
			infoText->remove();
			infoText->drop();
			infoText = 0;
		}

		if (pgCompass)
		{
			pgCompass->remove();
			pgCompass->drop();
		}

		if (healtbar)
			delete healtbar;
	}

	inline Healthbar* getHealtbar() const { return this->healtbar; }
	
	const void update(const Ship* ship) const
	{
		//update compass
		irr::core::vector3df fore(0, 0, 1);
		ship->getNode()->getAbsoluteTransformation().rotateVect(fore);
		irr::core::vector3df rot1 = fore.getHorizontalAngle();
		pgCompass->SetCompassHeading(rot1.Y);

		//update info text
		const irr::core::vector3df& pos = ship->getNode()->getAbsolutePosition();
		irr::core::stringw text(L"Position: X ");
		text.append(irr::core::stringw((irr::s32)pos.X));
		text.append(L" | Y "); text.append(irr::core::stringw((irr::s32)pos.Y));
		text.append(L" | Z "); text.append(irr::core::stringw((irr::s32)pos.Z));
		
		text.append(L"\nUnits above ground: ");
		text.append(irr::core::stringw((irr::s32)ship->getAltitude()));

		this->infoText->setText(text.c_str());
		
		//update healthbar
		if (this->healtbar)
			this->healtbar->update();
	}
};

#endif /*HUD_H_*/
