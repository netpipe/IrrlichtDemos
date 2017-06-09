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

#include "irrlicht.h"
#include "GameItemIrrlicht.h"
#include "GameIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "XYElement.h"
#include "BombIrrlicht.h"
#include "FlyBombIrrlicht.h"
#include "BombItemIrrlicht.h"
#include <iostream>
#include "Serializer.h"
#include "Deserializer.h"
#include "Mainwindow.h"
#include <math.h>
#include "GameTimerIrrlicht.h"
#include "SoundManager.h"

#define PI 3.14159265

BombItemIrrlicht::BombItemIrrlicht(BombIrrlicht* p_model) :
	ElementItemIrrlicht(p_model) {
	node = 0;
}

BombItemIrrlicht::BombItemIrrlicht(BombIrrlicht* p_model,
		GameSceneIrrlicht* g_scene) :
	ElementItemIrrlicht(p_model) {
	node = 0;
	this->init();
}
;

BombItemIrrlicht::~BombItemIrrlicht() {

}

void BombItemIrrlicht::init() {
	//std::cout << "enter BombItemIrrlicht::init() \n";
	GameSceneIrrlicht* g_scene = GameIrrlicht::m_GameItemIrrlicht;

	irr::f32 dz = ArenaItemIrrlicht::getCellSize();
	//node = GameIrrlicht::m_GameItemIrrlicht->smgr->addMeshSceneNode(GameIrrlicht::m_GameItemIrrlicht->smgr->getMesh("./media/bombc.obj"));
	node = GameIrrlicht::m_GameItemIrrlicht->smgr->addMeshSceneNode(
			GameIrrlicht::m_GameItemIrrlicht->smgr->getMesh(
					"./media/bombtext.obj"));
	//node = GameIrrlicht::m_GameItemIrrlicht->smgr->addMeshSceneNode(GameIrrlicht::m_GameItemIrrlicht->smgr->getMesh("./media/Items/CadBomb.obj"));

	GameIrrlicht::m_GameItemIrrlicht->scaleNode(node, dz * 0.75);
	normalscale = node->getScale();

	BombIrrlicht* pl = dynamic_cast<BombIrrlicht*> (this->getModel());
	//std::cout << "BombItemIrrlicht::init() coords  "<< pl->getX()<<", "<<pl->getY()<<"\n";

	node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
	node->setRotation(irr::core::vector3df(0, 0, 0));
	this->node->setPosition(irr::core::vector3df(pl->getX() - 0.5 * dz,
			1 * -dz, pl->getY() - 0.5 * dz));

	//animModel->setAnimationSpeed(30);
	node->setVisible(false);

	lastInterfaceAction = Mainwindow::device->getTimer()->getTime();

	// std::cout << "BombItemIrrlicht::init()" << pl->getX() - 0.5 * dz << ", "<< pl->getY() - 0.5 * dz << "\n";
}

void BombItemIrrlicht::update() {
	if (!node) {
		init();
	}
	irr::f32 dz = ArenaItemIrrlicht::getCellSize();

	FlyBombIrrlicht* pl = dynamic_cast<FlyBombIrrlicht*> (this->getModel());

	if (pl->playsound) {
		pl->playsound = false;
		// play a sound
		cAudio::IListener* listener = SoundManager::manager->getListener();
		cAudio::IAudioSource* mysound = SoundManager::manager->create("bling",
				"media/Sounds/explosion1.wav", false);

		//Set the IAudio Sound to play3d and loop
		//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
		if (mysound && listener) {

			listener->setPosition(cAudio::cVector3(
					GameIrrlicht::m_GameItemIrrlicht->cam->getPosition().X,
					GameIrrlicht::m_GameItemIrrlicht->cam->getPosition().Y,
					GameIrrlicht::m_GameItemIrrlicht->cam->getPosition().Z));
			mysound->setVolume(SoundManager::fxvolume);
			mysound->setMinDistance(1.0f);
			mysound->setMaxDistance(100.0f);
			mysound->play3d(cAudio::cVector3(pl->getX(), pl->getY(), pl->getZ()), 2.0f,
					false);
		}

	}

	if (pl->getExploded()) {
		node->setVisible(false);
		node->setRotation(irr::core::vector3df(0, 0, 00));
		//node->remove();
		return;

	}

	if (!pl->landed) {
		// works only for shortways distance < arena.size/2
		float waydone = pl->flydonex + pl->flydoney;

		float dzh = ((-dz) - pl->flystartz);
		float h = pl->flystartz + dzh * (waydone / pl->flylength);
		h = h + fabs(0.5 * pl->flylength * sin(PI * (waydone / pl->flylength)));
		float x = waydone / pl->flylength;
		//h = h + fabs(0.5*pl->flylength* ((2*x-1)*(2*x-1)*(2*x-1)*(2*x-1)*(-1)+1));
		h = h + 0.0 * dz;
		//std::cout << "bombitem h" << h <<"\n";
		this->node->setPosition(irr::core::vector3df(pl->getX() - 0.5 * dz, h,
				pl->getY() - 0.5 * dz));
		pl->setZ(h);

	} else {

		if (Mainwindow::device->getTimer()->getTime() - lastInterfaceAction
				> 50) {
			lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
			//this->node->setPosition(irr::core::vector3df(pl->getX(),-ArenaItemIrrlicht::getCellSize(), pl->getY()));

			//irr::core::vector3df cp = node->getRotation();
			irr::core::vector3df cp = node->getRotation();

			//int tmp = ((int)(node->getFrameNr())+1);

			// standing
			//node->setFrameLoop(0,55);
			irr::core::vector3df nr;
			if (pl->getXSpeed() < 0) {
				node->setRotation(irr::core::vector3df(pl->getXSpeed(), 0, 0)
						+ cp);
			}
			if (pl->getXSpeed() > 0) {
				node->setRotation(irr::core::vector3df(pl->getXSpeed(), 0, 2
						* pl->getXSpeed()) + cp);
			}

			irr::u32 timetoexplode = pl->laytime + pl->ignitetime
					- GameIrrlicht::m_gametimerIrrlicht->getTime();

			int bouncer = pl->ignitetime / 1000;

			double param, result;
			param = 360 * timetoexplode / pl->ignitetime;
			result = cos(param * bouncer * PI / 180);
			node->setScale(0.125 * normalscale * irr::core::vector3df(result,
					result, result) + normalscale);

			if (pl->getYSpeed() > 0) {

				nr = (irr::core::vector3df(pl->getYSpeed(), 0, 0) + cp);
				std::cout << "bombitem rotate up cp " << cp.X << " " << cp.Y
						<< " " << cp.Z << "\n";
				std::cout << "bombitem rotate up " << nr.X << " " << nr.Y
						<< " " << nr.Z << "\n";
				node->setRotation(nr);
			}
			if (pl->getYSpeed() < 0) {
				node->setRotation(irr::core::vector3df(pl->getYSpeed(), 0, 0)
						+ cp);
				nr = (irr::core::vector3df(pl->getYSpeed(), 0, 0) + cp);
				std::cout << "bombitem rotate down cp " << cp.X << " " << cp.Y
						<< " " << cp.Z << "\n";
				std::cout << "bombitem rotate down " << nr.X << " " << nr.Y
						<< " " << nr.Z << "\n";
				node->setRotation(nr);
				std::cout << "bombitem rotste down \n";
			}

			this->node->setPosition(irr::core::vector3df(pl->getX() - 0.5 * dz,
					1 * -dz, pl->getY() - 0.5 * dz));

		}
	}
}

void BombItemIrrlicht::setVisible(bool yorn) {
	if (!node) {
		init();
	}
	node->setVisible(yorn);

}

