/*
 * Based on
 * ARKANOID - Breakoutclone Game 4 Wizzard of OZ4
 * Autor: Dirk Hoeschen alias RubbelDieK@tz
 * Email: rubbel@c-base.org
 * This Game is public doMain.
 */

#include <stdio.h>
#include <iostream>
#include <sstream>

#include <wchar.h>
#include <irrlicht.h>
#include <stdlib.h>
#include <time.h>
#include "GameItemIrrlicht.h"
#include "BlockItemIrrlicht.h"
#include "BlockIrrlicht.h"
#include "WallItemIrrlicht.h"
#include "WallIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "GroundItemIrrlicht.h"
#include "GroundIrrlicht.h"
#include "BombExplosionIrrlicht.h"
#include "BombExplosionItemIrrlicht.h"
#include "PlayerIrrlicht.h"
#include "PlayerItemIrrlicht.h"

#include "ElementItemIrrlicht.h"
#include "IrrlichtEventReceiver.h"

#include "FallingBlockIrrlicht.h"
#include "FallingBlockItemIrrlicht.h"

#include "GameTimerIrrlicht.h"

#include "GameIrrlicht.h"
#include "Cell.h"

#include "States/StateOptionsboard.h"

#include "Mainwindow.h"

/*

 In the Irrlicht Engine, everything can be found in the namespace
 'irr'. So if you want to use a class of the engine, you have to
 write an irr:: before the name of the class.
 */
using namespace irr;

/*
 There are 5 sub namespaces in the Irrlicht Engine. Take a look
 at them, you can read a detailed description of them in the
 documentation by clicking on the top menu item 'Namespace List'
 or using this link: http://irrlicht.sourceforge.net/docu/namespaces.html.
 Like the irr Namespace, we do not want these 5 sub namespaces now,
 to keep this example simple. Hence we tell the compiler again
 that we do not want always to write their names:
 */
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


GameSceneIrrlicht::GameSceneIrrlicht(GameIrrlicht* p_game) {
	/* public variables */
	screenwidth = 1680;
	//screenwidth = 800;
	//int   screenwidth = 320;
	screenheight = 1050;
	//screenheight = 600;
	//int   screenheight =200;
	dz = 6;
	zstart = 0;
	m_game = p_game;
}

bool GameSceneIrrlicht::InitDriver() {

	/*
	 Get a pointer to the video driver, the SceneManager and the
	 graphical user interface environment, so that
	 we do not always have to write device->getVideoDriver(),
	 device->getSceneManager() and device->getGUIEnvironment().
	 */
	device = Mainwindow::device;
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	env = device->getGUIEnvironment();
	ElementItemIrrlicht::g_scene = this;
}

/*
 The Arkanoid Game Functions
 */
void GameSceneIrrlicht::CamStats() // Init Game Menu
{
	if (!device)
		return;

	IGUISkin* skin = env->getSkin();
	IGUIFont* font = env->getFont("./media/fontoffi.png");
	if (font)
		skin->setFont(font);

	skin->setColor(EGDC_BUTTON_TEXT, video::SColor(255, 255, 220, 100));

	// env->addImage(driver->getTexture("./media/woslogo.tga"), core::position2d<s32>(5,5));

	// Statustext
	const int lwidth = 450;
	const int lheight = 50;
	//core::dimension2d<unsigned int> size =device->getVideoDriver()->getScreenSize();
	//core::rect<int> pos(10, size.Height - lheight - 10, 10 + lwidth,	size.Height - 10);
	//env->addImage(pos);

	//statusText = env->addStaticText(L"IrrBombIx", pos, true, false);
	//statusText->setOverrideColor(video::SColor(255, 205, 200, 200));

	/* Buttons	*/
	int lastFPS = -1;
	int halbe = (screenwidth / 2);

	ArenaItemIrrlicht* aii =
			static_cast<ArenaItemIrrlicht*> (m_ArenaIrrlicht->getView());

	aii->setCamera(this);


}

void GameSceneIrrlicht::cleanup() {
	players.clear();
	bombs.clear();
}

void GameSceneIrrlicht::StartGame() {
}

void GameSceneIrrlicht::InitLevel()
{

}

void GameSceneIrrlicht::init() {

	/* initialize random seed: */
	srand(time(NULL));


	/* initialize the playfield */
	setLight();

	/* show the menu */
	CamStats();
	//receiver.InitLevel();


	Cell* tmp40;
	ElementIrrlicht* tmp20;
	std::cout << "GameItemIrrlicht (i,j)=(" << m_ArenaIrrlicht->getNbRows()
			<< "," << m_ArenaIrrlicht->getNbColumns() << ")\n";

	// imax=nbcolumns
	for (int i = 0; i < m_ArenaIrrlicht->getNbColumns(); ++i) {
		for (int j = 0; j < m_ArenaIrrlicht->getNbRows(); ++j) {
			//DrawPlate(arenaRows - i, j, true);
			//DrawBrick(arenaRows - i, j, true);
			tmp40 = m_ArenaIrrlicht->getCell(i, j);
			tmp20 = tmp40->getTerrain();
			if (tmp20) {
				if (tmp20->getView()) {
					tmp20->getView()->init();
				}
			}
			tmp20 = tmp40->getBlock();
			if (tmp20) {
				//std::cout << "has block \n";
				if (tmp20->getView()) {
					tmp20->getView()->init();
				}
			}
			tmp20 = tmp40->getBonus();
			if (tmp20) {
				//std::cout << "has Bonus \n";
				if (tmp20->getView()) {
					tmp20->getView()->init();
					tmp20->getView()->update();
				}
			}

		}
	}

	// Display each PlayerItem

	for (int j = 0; j < GameIrrlicht::players.size(); j++) {

		PlayerItemIrrlicht
				* tmp =
						static_cast<PlayerItemIrrlicht*> (GameIrrlicht::players[j]->getView());
		tmp->init();
		tmp->update();
	}

	DrawLoop();

}

void GameSceneIrrlicht::setLight() // Draw Playfield
{

	scene::ISceneNode* bottom = 0;
	int dz = ArenaItemIrrlicht::getCellSize();

	int nr = m_ArenaIrrlicht->getNbRows();
	int nc = m_ArenaIrrlicht->getNbColumns();

	irr::core::vector2df llcorner = m_ArenaIrrlicht->getCoords(0, 0);
	irr::core::vector2df urcorner = m_ArenaIrrlicht->getCoords(nr, nc);

	float power = 0.5;
	//float power = 10.0;

	irr::video::SColorf* lcolor = new irr::video::SColorf(1.0f, 1.0f, 1.0f,
			1.0f);
	irr::video::SLight* lighttmp = new irr::video::SLight();

	irr::scene::ILightSceneNode* tmp;

	lighttmp->DiffuseColor = *lcolor;

	//lighttmp->AmbientColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
	lighttmp->SpecularColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);

	tmp = smgr->addLightSceneNode(0, core::vector3df(llcorner.X - dz, 80,
			llcorner.Y - dz), video::SColorf(0.5f, 0.5f, 0.5f, 1.0f), power);
	tmp->setLightType(ELT_SPOT);
	tmp->setLightData(*lighttmp);
	lighttmp->CastShadows = true;
	tmp->getLightData().Radius = power;

	//tmp=smgr->addLightSceneNode(0,  core::vector3df(llcorner.X-dz, 80, urcorner.Y+dz), video::SColorf(0.5f, 0.5f, 0.5f,1.0f),  power);
	//tmp->setLightType(ELT_SPOT);
	//tmp->setLightData(*lighttmp);

	tmp=smgr->addLightSceneNode(0, core::vector3df(urcorner.X+dz, 80, urcorner.Y+dz), video::SColorf(0.5f, 0.5f, 0.5f,1.0f), power);
	tmp->setLightType(ELT_SPOT);
	tmp->setLightData(*lighttmp);

	//tmp = smgr->addLightSceneNode(0, core::vector3df(urcorner.X + dz, 80,	llcorner.Y - dz), video::SColorf(0.5f, 0.5f, 0.5f, 1.0f), power);
	//tmp->setLightType(ELT_SPOT);
	//tmp->setLightData(*lighttmp);

	smgr->setAmbientLight(video::SColorf(0.2, 0.2, 0.2, 1));

}

void GameSceneIrrlicht::drawClock() {
	irr::gui::IGUIFont* font = device->getGUIEnvironment()->getFont(
			"./media/fontoffi.png");
	std::stringstream ss1;
	ss1.str(StateOptionsboard::stateoptionsboard->getValue("Party Time"));
	// std::cout << "GameSceneIrrlicht drawclock " <<StateOptionsboard::stateoptionsboard->getValue("Party Time")<<"\n";
	int roundtime;
	ss1 >> roundtime;
	int displaytime = roundtime * 1000
			- GameIrrlicht::m_gametimerIrrlicht->getTime();
	std::stringstream ss;
	ss << displaytime / 1000 << "." << (displaytime % 1000) / 100;
	core::dimension2d<unsigned int> size =
			device->getVideoDriver()->getScreenSize();
	core::rect<int> pos(size.Width / 2 * 0.9, size.Height * 0.05, size.Width
			/ 2 * 1.1, size.Height * 0.1);

	font->draw(ss.str().c_str(), pos, irr::video::SColor(255, 255, 255, 255));

}

void GameSceneIrrlicht::SoundFx() {
}

void GameSceneIrrlicht::scaleNode(scene::ISceneNode* mesh, f32 desiredX) {
	core::vector3df extent = mesh->getTransformedBoundingBox().getExtent();
	f32 factor = desiredX / extent.X;
	core::vector3d<f32> factorEscalate(factor, factor, factor);
	mesh->setScale(factorEscalate);
}

void GameSceneIrrlicht::DrawLoop() {
	//while (device->run() && driver) {


	//if (!m_game->isPaused()) {
	//driver->beginScene(true, true, SColor(255, 150, 201, 240));
	smgr->drawAll();
	env->drawAll();
	drawClock();
	//driver->endScene();
	//}
	//}
	//device->drop();
}

void GameSceneIrrlicht::updatePlayers() {
	for (int j = 0; j < m_game->getPlayersIrrlicht().size(); j++) {
		PlayerItemIrrlicht
				* tmp =
						static_cast<PlayerItemIrrlicht*> (m_game->getPlayersIrrlicht()[j]->getView());
		tmp->update();
	}

	for (int j = 0; j < GameIrrlicht::fallingblocks.size(); j++) {
		FallingBlockItemIrrlicht
				* tmp =
						static_cast<FallingBlockItemIrrlicht*> (GameIrrlicht::fallingblocks[j]->getView());
		tmp->update();
	}

	for (int j = 0; j < GameIrrlicht::bombexplosions.size(); j++) {
		bombexplosionItemIrrlicht
				* tmp =
						static_cast<bombexplosionItemIrrlicht*> (GameIrrlicht::bombexplosions[j]->getView());
		tmp->update();
	}

}

void GameSceneIrrlicht::update() {
	updatePlayers();
	ArenaItemIrrlicht* tmp =
			dynamic_cast<ArenaItemIrrlicht*> (m_ArenaIrrlicht->getView());

	if (StateOptionsboard::stateoptionsboard->getValue("Camera") == "dynamic") {
		smgr->setActiveCamera(dyncam);
		tmp->UpdateDynamicCam();
	}
	if (StateOptionsboard::stateoptionsboard->getValue("Camera") == "static") {
		smgr->setActiveCamera(cam);
	}
	if (StateOptionsboard::stateoptionsboard->getValue("Camera") == "fly") {
		smgr->setActiveCamera(fpscam);
	}
	if (StateOptionsboard::stateoptionsboard->getValue("Camera") == "overhead") {
			smgr->setActiveCamera(overheadcam);
		}
	if (StateOptionsboard::stateoptionsboard->getValue("Camera") == "beauty") {
				smgr->setActiveCamera(beautycam);
			}

	DrawLoop();
}

irr::f32 GameSceneIrrlicht::xytoxy(irr::f32 gx, bool isy) {
	int arenaRows = m_ArenaIrrlicht->getNbRows();
	int arenaCols = m_ArenaIrrlicht->getNbColumns();
	ArenaItemIrrlicht * tmp =
			static_cast<ArenaItemIrrlicht*> (m_ArenaIrrlicht->getView());
	if (isy) {
		irr::f32 maxy = arenaRows * m_ArenaIrrlicht->getCell(0, 0)->SIZE;
		return (maxy - gx) / m_ArenaIrrlicht->getCell(0, 0)->SIZE
				* tmp->getCellSize();
	} else {
		return gx / m_ArenaIrrlicht->getCell(0, 0)->SIZE * tmp->getCellSize();
	}
}

ArenaIrrlicht* GameSceneIrrlicht::getArenaIrrlicht() {
	return m_ArenaIrrlicht;
}

void GameSceneIrrlicht::setArenaIrrlicht(ArenaIrrlicht *m_ArenaIrrlicht) {
	this->m_ArenaIrrlicht = m_ArenaIrrlicht;

}
