/*
 * Copyright 2011
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "PlayerItemIrrlicht.h"
#include "PlayerIrrlicht.h"
#include "irrlicht.h"
#include "GameItemIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "XYElement.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "GameIrrlicht.h"
#include "Mainwindow.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sstream>

PlayerItemIrrlicht::PlayerItemIrrlicht(PlayerIrrlicht* p_model) :
	ElementItemIrrlicht(p_model) {
	//this->init(g_scene);
	sickTex = 0;
}

PlayerItemIrrlicht::PlayerItemIrrlicht(PlayerIrrlicht* p_model,
		GameSceneIrrlicht* g_scene) :
	ElementItemIrrlicht(p_model) {
	sickTex = 0;
	this->init();
}
;

PlayerItemIrrlicht::~PlayerItemIrrlicht() {

}

float PlayerItemIrrlicht::loopABBA(int a, int b, float tmp2) {
	float tmp;
	float newtime = Mainwindow::device->getTimer()->getTime();
	float dt = newtime - lastItemAction;
	//if (dt > 0)
	// lastItemAction = newtime;
	//float l = (b - a) / 1000.0;
	float l = animspeed / 1000.0;

	//tmp = node->getFrameNr();
	tmp = tmp2;
	tmp = tmp + (animdirection * dt * l);
	if (tmp >= b) {
		tmp = b;
		animdirection = -1;
	}
	if (tmp <= a) {
		tmp = a;
		animdirection = 1;
	}
	return tmp;

}

float PlayerItemIrrlicht::loopABAB(int a, int b, float in) {
	float tmp;
	float newtime = Mainwindow::device->getTimer()->getTime();
	float dt = newtime - lastItemAction;
	//if (dt > 0)
	//	lastItemAction = newtime;
	//float l = (b - a) / 1000.0;
	float l = animspeed / 1000.0;

	tmp = in;
	tmp = tmp + animdirection * dt * l;

	if (tmp >= b) {
		tmp = a;
		animdirection = 1;
	}
	if (tmp <= a) {
		tmp = a;
		animdirection = 1;
	}
	return tmp;
}

std::string* PlayerItemIrrlicht::getCharacterfile() {
	return characterfile;
}
;

bool PlayerItemIrrlicht::setCharacterfile(std::string* string) {
	characterfile = string;
	Deserializer* ds;
	// std::cout << "PlayerItemIrrlicht characterfile" << characterfile->c_str()	<< "\n";
	ds = new Deserializer(characterfile->c_str());

	if(ds->fileok){
			readFrom(ds);
			return true;
		}
	free(ds);
	return false;
}

void PlayerItemIrrlicht::init() {
	GameSceneIrrlicht* g_scene = GameIrrlicht::m_GameItemIrrlicht;
	//std::cout << "enter PlayerItemIrrlicht::init() \n";

	irr::f32 dz = ArenaItemIrrlicht::getCellSize();

	bool UseLight = true;
	//	meshname="./media/Characters/0072.x";
	//
	//	standingstart = 0;
	//	standingend = 55;
	//
	//	runningstart = 60;
	//	runningend = 100;
	//
	//	winningstart = 150;
	//	winningend = 220;
	//
	//	loosingstart = 0;
	//	loosingend = 0;


	//	meshname = "./media/Characters/0012.x";
	//	Serializer* s = new Serializer("./media/Characters/0012.xml");
	//
	//	standingstart = 0;
	//	standingend = 65;
	//
	//	runningstart = 65;
	//	runningend = 110;
	//
	//	winningstart = 110;
	//	winningend = 178;
	//
	//	loosingstart = 0;
	//	loosingend = 0;

	//writeTo(s);

	//	Deserializer* ds;
	//	std::cout << "PlayerItemIrrlicht characterfile" << characterfile->c_str()
	//			<< "\n";
	//	ds = new Deserializer(characterfile->c_str());
	//	readFrom(ds);

	irr::core::vector3df initrotation = irr::core::vector3df(irx, iry, irz);
	irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);

	irr::scene::IAnimatedMesh* m = g_scene->smgr->getMesh(meshname.c_str());

	irr::scene::IAnimatedMesh* ufom = g_scene->smgr->getMesh(
			"./media/Characters/oni.x");

	if (!m) {
		//			// model could not be loaded
		//				Device->getGUIEnvironment()->addMessageBox(
		//				Caption.c_str(), L"The model could not be loaded. " \
//				L"Maybe it is not a supported file format.");
		//				return;
		std::cout << "The model could not be loaded. "
			L"Maybe it is not a supported file format.\n";
	}
	if (!ufom) {
		//			// model could not be loaded
		//				Device->getGUIEnvironment()->addMessageBox(
		//				Caption.c_str(), L"The model could not be loaded. " \
	//				L"Maybe it is not a supported file format.");
		//				return;
		std::cout << "The ufo model could not be loaded. "
			L"Maybe it is not a supported file format.\n";
	}

	// set default material properties
	//
	//		if (Octree)
	//			Model = Device->getSceneManager()->addOctreeSceneNode(m->getMesh(0));
	//		else
	//		{


	ufoModel = g_scene->smgr->addAnimatedMeshSceneNode(ufom);
	g_scene->scaleNode(ufoModel, 1.5 * dz);
	ufoModel->setMaterialFlag(irr::video::EMF_LIGHTING, UseLight);
	ufoModel->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, UseLight);
	ufoModel->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, true);
	ufoHigh = irr::core::vector3df(0, 0.25 * -dz, 0);

	irr::scene::IAnimatedMeshSceneNode* animModel =
			g_scene->smgr->addAnimatedMeshSceneNode(m);

	node = animModel;

	//}
	node->setMaterialFlag(irr::video::EMF_LIGHTING, UseLight);
	node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, UseLight);
	node->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, true);
	//	Model->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	//node->setDebugDataVisible(irr::scene::EDS_OFF);
	node->setRotation(initrotation);

	PlayerIrrlicht* pl = dynamic_cast<PlayerIrrlicht*> (this->getModel());
	//node->setPosition(irr::core::vector3df(pl->getX(),-dz,pl->getY()));
	node->setPosition(irr::core::vector3df(50, 1.5 * -dz, 50) + initposition);

	irr::core::vector3df extent = node->getTransformedBoundingBox().getExtent();
	irr::f32 factor = (dz + 2) / extent.X;
	factor = 0.033;
	// std::cout << "PlayerItemIrrlicht::scale() " << factor << "\n";
	irr::core::vector3d<irr::f32> factorEscalate(factor, factor, factor);
	node->setScale(factorEscalate);

	//g_scene->scaleNode(node, dz + 2);

	//animModel->setAnimationSpeed(30);
	node->setVisible(true);
	//node->setFrameLoop(standingstart, standingend);
	//node->setAnimationSpeed(30);
	//node->setLoopMode(true);

	//node->addChild(ufoModel);
	ufoModel->setPosition(node->getPosition() + ufoHigh + initposition);
	ufoModel->setVisible(true);

	deadhigh = 1.5 * -dz;

	animdirection = 1;
	animspeed = node->getAnimationSpeed() * 1.25;

	node->setAnimationSpeed(0);

	float newtime = Mainwindow::device->getTimer()->getTime();
	lastItemAction = newtime;

	blinkdir = 1;
	sickblink = 0;
	// create sickTextures
	sick = 0;

	if (sickTex) {
		for (int i = 0; i < 256; i++) {
			Mainwindow::device->getVideoDriver()->removeTexture(sickTex[i]);
		}
		// free(sickTex);
		sickTex=0;
	}
	{

		sickTex = new irr::video::ITexture*[257];

		for (int j = 0; j < node->getMaterialCount(); j++) {
			irr::video::SMaterial tmp = node->getMaterial(j);
			irr::video::ITexture* texture = 0;
			texture = tmp.getTexture(0);

			// std::cout << "PlayerItemIrrlicht sicktex creation!["<< pl->getPlayerName().c_str() << " " << texture << "\n";

			if (texture) {
				sick
						= Mainwindow::device->getVideoDriver()->createImageFromData(
								texture->getColorFormat(), texture->getSize(),
								texture->lock(), false //copy mem
						);
				texture->unlock();

				origTex = texture;
				texturemat = j;

			}

			if (sick) {
				for (int i = 0; i < 25; i++) {
					sick->lock();
					for (int x = 0; x < sick->getDimension().Width; x++) {
						for (int y = 0; y < sick->getDimension().Height; y++) {
							int c = i * 10;
							sick->setPixel(x, y, irr::video::SColor(255, c, c,
									c), false);
						}
					}
					sick->unlock();
					std::string* stmp = new std::string(
							pl->getPlayerName().c_str());
					std::string* stmp2 = new std::string("sicktex" + i);
					std::string stmp3 = stmp->append(stmp2->c_str());
					sickTex[i]
							= Mainwindow::device->getVideoDriver()->addTexture(
									stmp3.c_str(), sick);
					// std::cout << "PlayerItemIrrlicht sicktex[" << i	<< "] created " << "\n";
					//node->setMaterialTexture(j, sickTex[0]);
					//node->getMaterial(j).setTexture(0, sickTex[i]);
				}
				break;
			}
		}
	}
}

float PlayerItemIrrlicht::ufoing() {
	irr::f32 dz = ArenaItemIrrlicht::getCellSize();

	PlayerIrrlicht* pl = dynamic_cast<PlayerIrrlicht*> (this->getModel());
	//this->node->setPosition(irr::core::vector3df(pl->getX(),-ArenaItemIrrlicht::getCellSize(), pl->getY()));
	irr::core::vector3df initrotation = irr::core::vector3df(irx, iry, irz);
	irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);
	this->node->setPosition(irr::core::vector3df(pl->getX() - (dz * 0.5),
			pl->getZ(), pl->getY() - (dz * 0.5)) + initposition);
	//irr::core::vector3df cp = node->getRotation();
	irr::core::vector3df cp = initrotation;

	// standing

	// look to the middle only
	int horizonly = GameIrrlicht::arena->getColFromX(pl->getX()) != 0
			&& GameIrrlicht::arena->getColFromX(pl->getX())
					!= GameIrrlicht::arena->getNbColumns() - 1;
	int verticonly = GameIrrlicht::arena->getRowFromY(pl->getY()) != 0
			&& GameIrrlicht::arena->getRowFromY(pl->getY())
					!= GameIrrlicht::arena->getNbRows() - 1;

	int dx = pl->lastXSpeed == 0;
	int dy = pl->lastYSpeed == 0;

	dx = dx && !horizonly;
	dy = dy || horizonly;

	dx = dx || verticonly;
	dy = dy && !verticonly;

	int dmx = (GameIrrlicht::arena->getNbColumns() * Cell::SIZE / 2)
			- pl->getX();
	int dmy = (GameIrrlicht::arena->getNbRows() * Cell::SIZE / 2) - pl->getY();

	//bombonCell->throwBomb(dx * dmx, dy * dmy, 2);

	//int tmp = ((int) (node->getFrameNr()) + 1);

	float tmp = loopABBA(standingstart, standingend, node->getFrameNr());
	//node->setFrameLoop(standingstart, standingend);

	if (dx * dmx < 0) {
		node->setRotation(irr::core::vector3df(0, -90.0, 0) + cp);
		ufoModel->setRotation(irr::core::vector3df(0, -90.0, 0));
	}
	if (dx * dmx > 0) {
		node->setRotation(irr::core::vector3df(0, 90.0, 0) + cp);
		ufoModel->setRotation(irr::core::vector3df(0, 90.0, 0));
	}
	if (dy * dmy > 0) {
		node->setRotation(irr::core::vector3df(0, 0.0, 0) + cp);
		ufoModel->setRotation(irr::core::vector3df(0, 0.0, 0));

	}
	if (dy * dmy < 0) {
		node->setRotation(irr::core::vector3df(0, 180, 0) + cp);
		ufoModel->setRotation(irr::core::vector3df(0, 180, 0));

	}
	//node->setFrameLoop(runningstart, runningstart);
	return tmp;
}

float PlayerItemIrrlicht::running() {
	irr::f32 dz = ArenaItemIrrlicht::getCellSize();

	PlayerIrrlicht* pl = dynamic_cast<PlayerIrrlicht*> (this->getModel());
	//this->node->setPosition(irr::core::vector3df(pl->getX(),-ArenaItemIrrlicht::getCellSize(), pl->getY()));
	irr::core::vector3df initrotation = irr::core::vector3df(irx, iry, irz);
	irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);
	this->node->setPosition(irr::core::vector3df(pl->getX() - (dz * 0.5),
			pl->getZ(), pl->getY() - (dz * 0.5)) + initposition);
	//irr::core::vector3df cp = node->getRotation();
	irr::core::vector3df cp = initrotation;

	// int tmp = ((int) (node->getFrameNr()) + animdirection);
	float tmp;
	// standing
	if (pl->getXSpeed() == 0 && pl->getYSpeed() == 0) {
		tmp = loopABBA(standingstart, standingend, node->getFrameNr());
	} else {
		// running
		if (pl->lastXSpeed < 0) {
			node->setRotation(irr::core::vector3df(0, -90.0, 0) + cp);
		}
		if (pl->lastXSpeed > 0) {
			node->setRotation(irr::core::vector3df(0, 90.0, 0) + cp);
		}
		if (pl->lastYSpeed > 0) {
			node->setRotation(irr::core::vector3df(0, 0.0, 0) + cp);

		}
		if (pl->lastYSpeed < 0) {
			node->setRotation(irr::core::vector3df(0, 180, 0) + cp);

		}
		float dspeed = animspeed * float(pl->getSpeed())
				/ float(pl->m_normalSpeed);
		animspeed = animspeed + dspeed;
		tmp = loopABAB(runningstart, runningend, node->getFrameNr());
		animspeed = animspeed - dspeed;
	}
	return tmp;
}

float PlayerItemIrrlicht::winning() {
	irr::f32 dz = ArenaItemIrrlicht::getCellSize();

	PlayerIrrlicht* pl = dynamic_cast<PlayerIrrlicht*> (this->getModel());
	//this->node->setPosition(irr::core::vector3df(pl->getX(),-ArenaItemIrrlicht::getCellSize(), pl->getY()));

	irr::core::vector3df initrotation = irr::core::vector3df(irx, iry, irz);
	irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);
	this->node->setPosition(irr::core::vector3df(pl->getX() - (dz * 0.5), 1.5
			* -dz, pl->getY() - (dz * 0.5)) + initposition);

	//int tmp = ((int) (node->getFrameNr()) + 1);

	// standing
	float tmp = loopABAB(winningstart, winningend, node->getFrameNr());
	return tmp;
}

float PlayerItemIrrlicht::loosing() {
	PlayerIrrlicht* pl = dynamic_cast<PlayerIrrlicht*> (this->getModel());
	//float newtime = Mainwindow::device->getTimer()->getTime();
	//float dt = newtime - lastItemAction;
	irr::f32 dz = ArenaItemIrrlicht::getCellSize();
	if (deadhigh > dz * 5) {
		if (!pl->isUfo) {
			node->setVisible(false);
		}
		return 0;
	}

	deadhigh = deadhigh + (dz / 40);

	//this->node->setPosition(irr::core::vector3df(pl->getX(),-ArenaItemIrrlicht::getCellSize(), pl->getY()));
	irr::core::vector3df initrotation = irr::core::vector3df(irx, iry, irz);
	irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);
	this->node->setPosition(irr::core::vector3df(pl->getX() - (dz * 0.5), (0.0
			* dz) + deadhigh, pl->getY() - (dz * 0.5)) + initposition);
	//irr::core::vector3df cp = node->getRotation();
	irr::core::vector3df cp = irr::core::vector3df(-90, deadhigh * 5, 0);
	node->setRotation(irr::core::vector3df(0, 180, 0) + cp);
	//int tmp = ((int) (node->getFrameNr()) + 1);

	// standing
	float tmp = loopABBA(loosingstart, loosingend, node->getFrameNr());
	//node->setFrameLoop(loosingstart, loosingend);
	return tmp;
}

void PlayerItemIrrlicht::update() {
	float tmpnext = 0;
	PlayerIrrlicht* pl = dynamic_cast<PlayerIrrlicht*> (this->getModel());

	if (pl->isAlive()) {

		if (pl->getWonRound()) {
			tmpnext = winning();
		} else {
			tmpnext = running();
		}
	} else {
		if (!pl->isUfo) {
			tmpnext = loosing();
		}
	}

	if (pl->isUfo) {
		irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);
		ufoModel->setPosition(node->getPosition() + ufoHigh - initposition);
		node->setVisible(true);
		ufoModel->setVisible(true);
		tmpnext = ufoing();
	} else {
		ufoModel->setVisible(false);
	}

	float newtime = Mainwindow::device->getTimer()->getTime();
	if (pl->hasBadBonus() && !pl->isUfo) {
		float dt = newtime - lastItemAction;
		if (dt > 0) {

			//float l = (b - a) / 1000.0;
			float l = 50 / 1000.0;

			//tmp = node->getFrameNr();

			sickblink = sickblink + (blinkdir * dt * l);
			if (sickblink >= 24.0) {
				sickblink = 24.0;
				blinkdir = -1;
			}
			if (sickblink <= 0.0) {
				sickblink = 0.0;
				blinkdir = 1;
			}

			int texnr = sickblink;
			/*
			std::cout << "PlayerItemIrrlicht::blink()" << sickblink
					<< " texturemat " << texturemat << " texnr " << texnr
					<< " sickTex[texnr] " << sickTex[texnr]
					<< "\n";
					*/
			node->getMaterial(texturemat).setTexture(0, sickTex[texnr]);
		}

	} else {
		node->getMaterial(texturemat).setTexture(0, origTex);
	}

	node->setCurrentFrame(tmpnext);
	//std::cout << "PlayerItemIrrlicht::update()" << meshname << " getFrameNr "<< tmpnext << "\n";
	//std::cout << "PlayerItemIrrlicht::update()" << pl->getAskedXSpeed() << ", "	<< pl->getAskedYSpeed() << "\n";

	lastItemAction = newtime;
}

void PlayerItemIrrlicht::writeTo(Serializer* s) {
	//Serializer* s = new Serializer("./media/Characters/0072.xml");

	s->beginAttribute("playeritemIrrlicht");
	s->writeParameter("meshname", this->meshname);
	s->writeParameter("iconname", this->iconname);
	s->writeParameter("ipx", this->ipx);
	s->writeParameter("ipy", this->ipy);
	s->writeParameter("ipz", this->ipz);
	s->writeParameter("irx", this->irx);
	s->writeParameter("iry", this->iry);
	s->writeParameter("irz", this->irz);

	s->writeParameter("standingstart", this->standingstart);
	s->writeParameter("standingend", this->standingend);

	s->writeParameter("runningstart", this->runningstart);
	s->writeParameter("runningend", this->runningend);

	s->writeParameter("winningstart", this->winningstart);
	s->writeParameter("winningend", this->winningend);

	s->writeParameter("loosingstart", this->loosingstart);
	s->writeParameter("loosingend", this->loosingend);

	s->endAttribute();
	s->endAttribute();
	std::cout << "write xml done \n";

}

void PlayerItemIrrlicht::readFrom(Deserializer* d) {

	d->beginAttribute("playeritemIrrlicht");
	d->readParameter("meshname", this->meshname);
	d->readParameter("iconname", this->iconname);
	d->readParameter("ipx", this->ipx);
	d->readParameter("ipy", this->ipy);
	d->readParameter("ipz", this->ipz);
	d->readParameter("irx", this->irx);
	d->readParameter("iry", this->iry);
	d->readParameter("irz", this->irz);

	d->readParameter("standingstart", this->standingstart);
	d->readParameter("standingend", this->standingend);

	d->readParameter("runningstart", this->runningstart);
	d->readParameter("runningend", this->runningend);

	d->readParameter("winningstart", this->winningstart);
	d->readParameter("winningend", this->winningend);

	d->readParameter("loosingstart", this->loosingstart);
	d->readParameter("loosingend", this->loosingend);

	//std::cout << "deserializble\n";

}

