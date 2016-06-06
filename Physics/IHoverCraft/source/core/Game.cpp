#include "Game.h"
#include <IGUIEnvironment.h>
#include <ICameraSceneNode.h>
#include "GameObject.h"

Game::Game(
	Configuration& configuration,
	irr::IrrlichtDevice* device
#ifdef _SOUND
	, irrklang::ISoundEngine* soundEngine
#endif
) :
	StateManager<Game, irr::SEvent>(this),

	configuration(configuration), device(device),
	sceneManager(device->getSceneManager()),
	videoDriver(device->getVideoDriver()),
	guiEnvironment(device->getGUIEnvironment()),
#ifdef _SOUND
	soundEngine(soundEngine),
#endif
	font(0),

	timer(device->getTimer()),
	then(timer->getTime()),
	now(0),
	elapsed(0.0f),

	weatherSystem(0),
	camera(sceneManager->addCameraSceneNode()),

	playerHealth(100),

	weaponSystem(0)
{
	this->device->grab();
	this->device->setEventReceiver(this);
#ifdef _SOUND
	this->soundEngine->grab();
#endif

	this->weatherSystem = new WeatherSystem(this);
	camera->setFarValue(25000.0f);

	this->physics = new Physics(this);

	this->weaponSystem = new WeaponSystem(this);

	// change gui transparency
	irr::gui::IGUISkin* currentSkin = this->guiEnvironment->getSkin();

	irr::u32 c;
	for (c = 0; c < irr::gui::EGDC_COUNT; ++c)
	{
		irr::video::SColor col = currentSkin->getColor(irr::gui::EGUI_DEFAULT_COLOR(c));
		col.setAlpha(configuration.getGuiTransparency());
		currentSkin->setColor(irr::gui::EGUI_DEFAULT_COLOR(c), col);
	}

	//setup default game font
	setFont("media/fonts/fontlucida.png");
}

Game::~Game()
{
	delete this->physics;
	delete this->weatherSystem;
	delete this->weaponSystem;

	this->device->drop();
#ifdef _SOUND
	this->soundEngine->drop();
#endif
}

const void Game::addGameState(const char* name, State<Game, irr::SEvent>* state)
{
	this->gameStates.push_back(SState(name, state));
}

State<Game, irr::SEvent>* Game::findGameState(const char* name)
{
	const irr::s32 f = this->gameStates.binary_search(SState(name));
	if (f != -1)
		return this->gameStates[f].State;

	return 0;
}

/*
* You can write screenshots in the following file formats:
* PNG, BMP, TGA, PPM, JPG
* Just change the extension of the filename. (really easy, isn't it?)
*/
const void Game::makeScreenshot() const
{
	//get video::IImage from the last rendered frame
	irr::video::IImage* image = 0;
	image = this->videoDriver->createScreenShot();

	if (image) //should always be true, but you never know. ;)
	{
		//construct a filename, consisting of local time and file extension
		irr::c8 filename[64];
		snprintf(filename, 64, "capture/screenshot_%u.png", this->timer->getRealTime());

		//write screenshot to file
		if (this->videoDriver->writeImageToFile(image, filename))
			this->device->getLogger()->log(L"Screenshot taken.");
		else
			this->device->getLogger()->log(L"Failed to take screenshot. Maybe you need to create the capture folder.");

		//Don't forget to drop image since we don't need it anymore.
		image->drop();
	}
}

const void Game::setFont(const irr::c8* filename)
{
	this->font = this->getGuiEnvironment()->getFont(filename);
	if (this->font)
		this->getGuiEnvironment()->getSkin()->setFont(this->font);
/*
	//leave built in font as tooltip font
	this->getGuiEnvironment()->getSkin()->setFont(
		this->getGuiEnvironment()->getBuiltInFont(),
		irr::gui::EGDF_TOOLTIP
	);
*/
}

const void Game::tick()
{
	//compute time since last frame
	this->now = this->timer->getTime();
	this->elapsed = (this->now - this->then) / 1000.f;
	this->then = this->now;
}

irr::video::ITexture* Game::scaleTexture(irr::video::ITexture* SrcTexture, const irr::core::dimension2du& destSize)
{
	static irr::u32 counter = 0;
	irr::video::IImage* SrcImage;
	irr::video::IImage* DestImage;
	irr::video::ITexture* IntTexture;
	irr::core::stringc SrcName = "NewTexture";
	SrcName += counter;

	IntTexture = this->videoDriver->addTexture(destSize, "IntermediateTexture");
	SrcImage = this->videoDriver->createImageFromData(SrcTexture->getColorFormat(), SrcTexture->getSize(), SrcTexture->lock());
	DestImage = this->videoDriver->createImageFromData(SrcTexture->getColorFormat(), destSize, IntTexture->lock());

	IntTexture->unlock();
	SrcImage->copyToScaling(DestImage);
	SrcTexture->unlock();

	this->videoDriver->removeTexture(IntTexture);
	IntTexture = this->videoDriver->addTexture(SrcName.c_str(), DestImage);
	SrcImage->drop();
	DestImage->drop();

	++counter;

	return IntTexture;
}
