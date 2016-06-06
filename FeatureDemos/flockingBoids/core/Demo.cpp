#include "Demo.h"
#include "ISceneManager.h"
#include <IGUIEnvironment.h>
#ifdef _SOUND
#include <irrKlang.h>
#endif

Demo::Demo(Configuration* const configuration, irr::IrrlichtDevice* const device
#ifdef _SOUND
		, irrklang::ISoundEngine* const soundEngine
#endif
) :
	StateManager<Demo, irr::SEvent>(this),

	configuration(configuration), device(device),
#ifdef _SOUND
	soundEngine(soundEngine),
#endif
	sceneManager(device->getSceneManager()),
	videoDriver(device->getVideoDriver()),
	guiEnvironment(device->getGUIEnvironment()),

	font(0),

	timer(device->getTimer()),
	then(timer->getTime()),
	now(0),
	elapsed(0.0f),

	camera(0), selector(sceneManager->createMetaTriangleSelector())
{
	device->grab();
	this->device->setEventReceiver(this);
#ifdef _SOUND
	soundEngine->grab();
#endif
	//setup default game font
	setFont("media/font/fontlucida.png");
}

Demo::~Demo()
{
	this->selector->drop();
	this->device->drop();
#ifdef _SOUND
	this->soundEngine->drop();
#endif
}

void Demo::addGameState(const char* const name, State<Demo, irr::SEvent>* state)
{
	this->gameStates.push_back(SState(name, state));
}

State<Demo, irr::SEvent>* Demo::findGameState(const char* const name)
{
	const irr::s32 f = this->gameStates.binary_search(SState(name));
	if (f != -1)
		return this->gameStates[f].state;

	return 0;
}

void Demo::setFont(const irr::c8* const filename)
{
	this->font = this->guiEnvironment->getFont(filename);
	if (this->font)
		this->guiEnvironment->getSkin()->setFont(this->font);
/*
	//leave built in font as tooltip font
	this->guiEnvironment->getSkin()->setFont(
		this->guiEnvironment->getBuiltInFont(), irr::gui::EGDF_TOOLTIP);
*/
}

void Demo::makeScreenshot() const
{
	//get video::IImage from the last rendered frame
	irr::video::IImage* const image = this->videoDriver->createScreenShot();

	if (image) //should always be true, but you never know. ;)
	{
		//construct a filename, consisting of local time and file extension
		irr::c8 filename[64];
		snprintf(filename, 64, "capture/screenshot_%u.png", this->timer->getRealTime());

		//write screenshot to file
		if (this->videoDriver->writeImageToFile(image, filename))
			this->device->getLogger()->log(L"Screenshot taken.");
		else
			this->device->getLogger()->log(L"Failed to take screenshot. Maybe you need to create the capture folder.", irr::ELL_WARNING);

		//Don't forget to drop image since we don't need it anymore.
		image->drop();
	}
}

void Demo::addSelector(irr::scene::ITriangleSelector* const selector) const
{
	this->selector->addTriangleSelector(selector);
}

void Demo::removeSelectors() const
{
	this->selector->removeAllTriangleSelectors();
}
