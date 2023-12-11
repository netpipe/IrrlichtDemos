#ifndef HEALTHBAR_H_
#define HEALTHBAR_H_

class Healthbar
{

private:

	Game* game;

	irr::core::dimension2di iconSize;

	irr::core::position2di heartPosition;
	irr::video::ITexture* heartTexture;
	irr::gui::IGUIImage* heart;

	irr::gui::IGUIStaticText* healtText;

	irr::video::ITexture* createScaledTexture(irr::video::IImage* origImage, const irr::core::dimension2du& desiredDimension) const
	{
		//make a texture
		irr::video::ITexture* origTexture = game->getVideoDriver()->addTexture("another texture", origImage);
	
		irr::video::ITexture* scaledTexture = game->scaleTexture(origTexture,
			irr::core::dimension2du(desiredDimension.Width, desiredDimension.Height));

		//remove original texture
		this->game->getVideoDriver()->removeTexture(origTexture);
		
		
		return scaledTexture;
	}
	
public:

	Healthbar(Game* game) :
		game(game),
		iconSize(64, 64),
		heartPosition(SX(50), SY(650)), heartTexture(0), heart(0), healtText(0)
	{

		//load image
		irr::video::IImage* heartImage = 0;
		heartImage = game->getVideoDriver()->createImageFromFile("media/images/healthbar/heart.tga");

		if (heartImage)
		{
			//make a scaled texture out of image
			const irr::core::dimension2du desiredDimension(this->iconSize.Width, this->iconSize.Height);
			this->heartTexture = createScaledTexture(heartImage, desiredDimension);
			this->heartTexture->grab();
			
			//add gui image wich uses scaled texture
			this->heart = game->getGuiEnvironment()->addImage(heartTexture, heartPosition);
			this->heart->grab();

			heartImage->drop();
		}

		const irr::s32 xPos = SX(50 + this->iconSize.Width + 10);
		const irr::s32 yPos = SY(650 + this->iconSize.Height/2);

		this->healtText = game->getGuiEnvironment()->addStaticText(
			irr::core::stringw(game->getPlayerHealth()).c_str(),
			irr::core::rect<irr::s32>(xPos, yPos, xPos + 80, yPos + 30), false, false, 0, -1, false);
		this->healtText->grab();
	}

	~Healthbar()
	{
		if (heart)
		{
			this->heart->remove();
			this->heart->drop();
		}
		
		if (this->heartTexture)
		{
			game->getVideoDriver()->removeTexture(heartTexture);
			heartTexture->drop();
		}
		
		if (this->healtText)
		{
			this->healtText->remove();
			this->healtText->drop();
		}
	}

	const void update() const
	{
		const irr::u32 currentHealth = game->getPlayerHealth();

		this->healtText->setText(irr::core::stringw(currentHealth).c_str());
		
		if (currentHealth <10)
			this->healtText->setOverrideColor(irr::video::SColor(255, 255, 0, 0));
		else if (currentHealth >= 10 && currentHealth < 100/2) //max player health / 2
			this->healtText->setOverrideColor(irr::video::SColor(255, 255, 128, 0));
		else
			this->healtText->setOverrideColor(irr::video::SColor(255, 0, 255, 64));
	}
};

#endif /*HEALTHBAR_H_*/
