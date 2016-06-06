#ifndef INTROSTATE_H_
#define INTROSTATE_H_

class IntroState : public State<Demo, irr::SEvent>
{

public:

	IntroState();

	virtual ~IntroState();

	virtual void onEnter(Demo* const game);

	virtual void onLeave(Demo* const game);

	virtual const bool onEvent(Demo* const game, const irr::SEvent& event);

	virtual void onUpdate(Demo* const game);

private:

	irr::gui::IGUIImage* irrlichtImage;

	irr::scene::ISceneNode* skybox;

	irr::scene::IBillboardTextSceneNode* introText;

	irr::scene::IBillboardTextSceneNode* copyRightText;

	irr::scene::ISceneNodeAnimator* anim1;

	irr::gui::IGUIStaticText* infoText;

	irr::f32 pauseTimer;

	bool wait;
};

IntroState::IntroState() :
	irrlichtImage(0),
	skybox(0),
	introText(0), copyRightText(0), anim1(0), infoText(0), pauseTimer(4.0f),
	wait(false)
	{

	}

IntroState::~IntroState()
{
	if (this->irrlichtImage)
	{
		this->irrlichtImage->remove();
		this->irrlichtImage->drop();
	}

	if (this->skybox)
	{
		this->skybox->remove();
		this->skybox->drop();
	}

	if (this->introText)
	{
		this->introText->remove();
		this->introText->drop();
	}

	if (this->copyRightText)
	{
		this->copyRightText->remove();
		this->copyRightText->drop();
	}

	if (this->infoText)
	{
		this->infoText->remove();
		this->infoText->drop();
	}
}

void IntroState::onEnter(Demo* const demo)
{
	demo->setCamera(demo->getSceneManager()->addCameraSceneNode());
	demo->getCamera()->setPosition(irr::core::vector3df(0.0f, 0.0f, 0.0f));
	demo->getCamera()->setTarget(irr::core::vector3df(0.0f, 0.0f, 1.0f));

	pauseTimer = 4.0f;
	wait = false;

	// add irrlicht logo
	this->irrlichtImage = demo->getGuiEnvironment()->addImage(demo->getVideoDriver()->getTexture("media/images/irrlichtlogo.jpg"),
			irr::core::position2d<irr::s32>(SX(900), SY(20)));
	this->irrlichtImage->grab();


	//create skybox
	demo->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, false);
	this->skybox = demo->getSceneManager()->addSkyBoxSceneNode(
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_up.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_dn.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_lf.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_rt.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_ft.png"),
			demo->getVideoDriver()->getTexture("media/images/skybox/starfield/starfield_bk.png"));
	demo->getVideoDriver()->setTextureCreationFlag(irr::video::ETCF_CREATE_MIP_MAPS, true);
	this->skybox->grab();

	irr::scene::ISceneNodeAnimator* skyAnim = demo->getSceneManager()->createRotationAnimator(irr::core::vector3df(0.0f, 0.005f, 0.0f));
	this->skybox->addAnimator(skyAnim);
	skyAnim->drop();

	//add introText
	this->introText = demo->getSceneManager()->addBillboardTextSceneNode(
			demo->getFont(),
			L"Irrlicht In Motion 08/09",
			0,
			irr::core::dimension2d<irr::f32>(20.0f, 5.0f),
			irr::core::vector3df(0.0f, 0.0f, 150.0f),
			-1,
			irr::video::SColor(255, 255, 0, 0),
			irr::video::SColor(255, 255, 233, 0));
	this->introText->grab();

	this->anim1 = demo->getSceneManager()->createFlyStraightAnimator(
			irr::core::vector3df(0.0f, 0.0f, 150.0f), //start
			irr::core::vector3df(0.0f, 0.0f, 15.0f), //end
			4000, //time for way
			false); // loop mode

	this->introText->addAnimator(anim1);
	this->anim1->drop();




	//add copyright text
	this->copyRightText = demo->getSceneManager()->addBillboardTextSceneNode(
			demo->getFont(),
			L"by randomMesh",
			0,
			irr::core::dimension2d<irr::f32>(4.0f, 2.0f),
			irr::core::vector3df(0.0f, -20.0f, 15.0f),
			-1,
			irr::video::SColor(255, 0, 0, 255),
			irr::video::SColor(255, 255, 255, 255));
	this->copyRightText->grab();





	irr::scene::IParticleSystemSceneNode* ps = demo->getSceneManager()->addParticleSystemSceneNode(false, this->introText);
	ps->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	ps->setMaterialTexture(0, demo->getVideoDriver()->getTexture("media/images/particle.bmp"));
	ps->setMaterialType(irr::video::EMT_TRANSPARENT_VERTEX_ALPHA);
//	ps->setScale(irr::core::vector3df(2.0f, 1.0f, 2.0f));

	irr::scene::IParticleEmitter* em = ps->createPointEmitter(
			irr::core::vector3df(0.0f, 0.003f, 0.0f),
			500,600,
			irr::video::SColor(0, 20, 20, 20), irr::video::SColor(0, 70, 70, 70),
			900, 1800,
			45);
	em->setMinStartSize(irr::core::dimension2d<irr::f32>(1.0f, 1.0f));
	em->setMaxStartSize(irr::core::dimension2d<irr::f32>(2.0f, 2.0f));
	ps->setEmitter(em);
	em->drop();

	irr::scene::IParticleAffector* paf = ps->createFadeOutParticleAffector(irr::video::SColor(0, 0, 0, 0), 500);
	ps->addAffector(paf);
	paf->drop();

	paf = ps->createGravityAffector(irr::core::vector3df(0.0f, 0.006f, 0.0f), 2000);
	ps->addAffector(paf);
	paf->drop();




	this->infoText = demo->getGuiEnvironment()->addStaticText(
			L"Press ESC to skip intro",
			irr::core::rect<irr::s32>(SX(20), SY(20), SX(200), SY(60)),
			false, true, 0, -1, false);
	this->infoText->setOverrideColor(irr::video::SColor(255, 255, 255, 255));
	this->infoText->grab();

#ifdef _SOUND
	if (demo->getSoundEngine() && demo->getConfiguration()->isSoundEnabled())
		demo->getSoundEngine()->play2D("media/sounds/runwithme.ogg", true);
#endif
}

void IntroState::onLeave(Demo* const demo)
{
	demo->getCamera()->remove();
	demo->setCamera(0);

	this->irrlichtImage->remove();
	this->irrlichtImage->drop();
	this->irrlichtImage = 0;

	this->skybox->remove();
	this->skybox->drop();
	this->skybox = 0;

	this->introText->removeAnimators();
	this->introText->remove();
	this->introText->drop();
	this->introText = 0;

	this->copyRightText->removeAnimators();
	this->copyRightText->remove();
	this->copyRightText->drop();
	this->copyRightText = 0;

	this->infoText->remove();
	this->infoText->drop();
	this->infoText = 0;
}

const bool IntroState::onEvent(Demo* const game, const irr::SEvent& event)
{
	switch(event.EventType)
	{
	case irr::EET_KEY_INPUT_EVENT:
	{
		if (!event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
			case irr::KEY_ESCAPE: game->setState(game->findGameState("running")); return true;
			default: return false;
			}
		}
	}
	break;

	default: return false;
	}

	return false;
}

void IntroState::onUpdate(Demo* const demo)
{
	if (this->wait)
	{
		this->pauseTimer -= demo->getElapsed();
		if (this->pauseTimer <= 0)
		{
			//change gamestate
			demo->setState(demo->findGameState("running"));
		}
	}
	else
	{
	    /*
		if (this->anim1->hasFinished())
		{
			this->wait = true;

			irr::scene::ISceneNodeAnimator* const anim2 = demo->getSceneManager()->createFlyStraightAnimator(
					this->copyRightText->getAbsolutePosition(), //start
					irr::core::vector3df(0.0f, -2.0f, 15.0f), //end
					2000, //time for way
					false); // loop mode

			this->copyRightText->addAnimator(anim2);
			anim2->drop();
		}
		*/
	}
}

#endif /*INTROSTATE_H_*/
