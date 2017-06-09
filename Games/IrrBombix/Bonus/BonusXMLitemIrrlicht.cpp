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

#include "BonusXMLitemIrrlicht.h"
#include "irrlicht.h"
#include "../GameItemIrrlicht.h"
#include "../ArenaItemIrrlicht.h"
#include "../ArenaIrrlicht.h"
#include "../GameIrrlicht.h"
#include "../Deserializer.h"
#include "../Serializer.h"
#include "../SoundManager.h"
#include "../Mainwindow.h"
#include "BonusIrrlicht.h"

#include <iostream>

#define PI 3.14159265

BonusXMLItemIrrlicht::BonusXMLItemIrrlicht(BonusIrrlicht* p_model) :
	ElementItemIrrlicht(p_model) {
	node = 0;
	pickupsound = "";
}

BonusXMLItemIrrlicht::BonusXMLItemIrrlicht(BonusIrrlicht* p_model,
		char* xmlfile) :
	ElementItemIrrlicht(p_model) {
	node = 0;
	pickupsound = "";
	readFrom(new Deserializer(xmlfile));
	//this->init();

}
;

BonusXMLItemIrrlicht::~BonusXMLItemIrrlicht() {

}

void BonusXMLItemIrrlicht::init() {

	if (!node) {
		std::cout << "enter BonusXMLItemIrrlicht::init() \n";
		//		irx = 0;
		//		iry = 90;
		//		irz = 0;
		//		ipx = 0;
		//		ipy = 0;
		//		ipz = 0;
		//		meshname = "./media/Items/CadBomb.obj";
		//		writeTo(new Serializer("./media/Items/Bonusbomb.xml"));

		irr::core::vector3df initrotation = irr::core::vector3df(irx, iry, irz);
		irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);

		ArenaItemIrrlicht
				* tmp =
						static_cast<ArenaItemIrrlicht*> (GameIrrlicht::arena->getView());
		dz = tmp->getCellSize();

		//dz=ArenaItemIrrlicht::getCellSize();
		//zstart=ArenaItemIrrlicht::getCellSize();
		zstart = tmp->getDrawPos().Z;

		IJElementIrrlicht* m =
				dynamic_cast<IJElementIrrlicht*> (this->getModel());
		cx = zstart + m->getI() * dz;
		cz = zstart + m->getJ() * dz;

		//std::cout << this->getModel()->getI() << ", "<< this->getModel()->getJ() <<"\n";
		//std::cout << cx << ", "<< cz <<"\n";
		irr::scene::IAnimatedMesh* mesh =
				GameIrrlicht::m_GameItemIrrlicht->smgr->getMesh(
						meshname.c_str());

		//Device->getSceneManager()->getMeshManipulator()->flipSurfaces( m->getMesh(0) );
		GameIrrlicht::m_GameItemIrrlicht->device->getSceneManager()->getMeshManipulator()->recalculateNormals(
				mesh->getMesh(0));
		node
				= GameIrrlicht::m_GameItemIrrlicht->smgr->addAnimatedMeshSceneNode(
						mesh);

		for (int i = 0; i < node->getMaterialCount(); i++) {
			node->getMaterial(i).AmbientColor
					= node->getMaterial(i).DiffuseColor;
			node->getMaterial(i).ColorMaterial = irr::video::ECM_NONE;
		}

		//std::cout << "BombItemIrrlicht::init() coords  "<< pl->getX()<<", "<<pl->getY()<<"\n";

		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);
		node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
		node->setRotation(initrotation);
		GameIrrlicht::m_GameItemIrrlicht->scaleNode(node, dz * 0.75);

		node->setPosition(initposition + irr::core::vector3df(cx, -dz, cz));


		bounce = ((rand() % 100) )/100.0;
		bouncedir = 1;



		setPosition();
		node->setVisible(false);

		std::cout << "leave BonusXMLItemIrrlicht::init() \n";
	} else {
		std::cout << "BonusXMLItemIrrlicht::init() already done\n";
	}
	lastItemAction = 0;
}

void BonusXMLItemIrrlicht::setPosition() {
	IJElementIrrlicht* m = dynamic_cast<IJElementIrrlicht*> (this->getModel());
	cx = zstart + m->getI() * dz;
	cz = zstart + m->getJ() * dz;
	irr::core::vector3df initrotation = irr::core::vector3df(irx, iry, irz);
	irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);
	node->setPosition(initposition * dz + irr::core::vector3df(cx, h, cz));
	//node->setPosition(initposition + irr::core::vector3df(cx, 0, cz));

}

void BonusXMLItemIrrlicht::setVisible(bool yorn) {
	// std::cout << "enter BonusXMLitemIrrlicht setVisible\n";
	if (this->node == 0) {
		init();
	}
	if (this->node) {
		//std::cout << "Drawing BonusXMLitemIrrlicht\n";
		setPosition();
		this->node->setVisible(yorn);
	} else {
		std::cout << "Could not initialise BonusXMLItemIrrlicht\n";
	}
	//std::cout << "leave BonusXMLItemIrrlicht::setVisible() \n";
}

void BonusXMLItemIrrlicht::update() {
	if (this->node == 0) {
		init();
	}

	if (this->node) {
		if (!lastItemAction) {
			lastItemAction = Mainwindow::device->getTimer()->getTime();
		}
		// loop abba
		float dt = Mainwindow::device->getTimer()->getTime() - lastItemAction;
		if (dt > 0) {

			//float l = (b - a) / 1000.0;
			float l = 1 / 2000.0;

			//tmp = node->getFrameNr();

			bounce = bounce + (bouncedir * dt * l);
			if (bounce >= 1.0) {
				bounce = 1.0;
				bouncedir = -1;
			}
			if (bounce <= 0.0) {
				bounce = 0.0;
				bouncedir = 1;
			}

		}

		h = (-dz + 0.25 * -dz * sin(PI * (bounce)));

		//node->setRotation(node->getRotation()+irr::core::vector3df(1, 1, 1));
		node->setRotation(node->getRotation() + irr::core::vector3df(0, 1, 0));

		BonusIrrlicht* m = dynamic_cast<BonusIrrlicht*> (this->getModel());

		if (!m->collected
				&& !GameIrrlicht::arena->getCell(m->getI(), m->getJ())->getBlock()) {
			setVisible(true);
		} else {
			setVisible(false);
		}

		if (m->playtakesound && pickupsound != "") {
			m->playtakesound = false;
			// play a sound
			cAudio::IListener* listener = SoundManager::manager->getListener();
			cAudio::IAudioSource* mysound = SoundManager::manager->create(
					"bling", pickupsound.c_str(), false);

			//Set the IAudio Sound to play3d and loop
			//play3d takes 4 arguments play3d(toloop,x,y,z,strength)
			if (mysound && listener) {

				listener->setPosition(
						cAudio::cVector3(
								GameIrrlicht::m_GameItemIrrlicht->cam->getPosition().X,
								GameIrrlicht::m_GameItemIrrlicht->cam->getPosition().Y,
								GameIrrlicht::m_GameItemIrrlicht->cam->getPosition().Z));
				mysound->setVolume(SoundManager::fxvolume);
				mysound->setMinDistance(1.0f);
				mysound->setMaxDistance(100.0f);

				mysound->play3d(cAudio::cVector3(node->getPosition().X,
						node->getPosition().Y, node->getPosition().Z), 2.0f,
						false);
			}

		}
	} else {
		std::cout << "Could not initialise BonusXMLItemIrrlicht\n";
	}
	lastItemAction = Mainwindow::device->getTimer()->getTime();
}

void BonusXMLItemIrrlicht::writeTo(Serializer* s) {
	//Serializer* s = new Serializer("/tmp/Bonusitem.xml");

	s->beginAttribute("BonusallitemIrrlicht");
	s->writeParameter("meshname", this->meshname);
	s->writeParameter("ipx", this->ipx);
	s->writeParameter("ipy", this->ipy);
	s->writeParameter("ipz", this->ipz);
	s->writeParameter("irx", this->irx);
	s->writeParameter("iry", this->iry);
	s->writeParameter("irz", this->irz);
	s->writeParameter("pickupsound", this->pickupsound);

	s->endAttribute();
	s->endAttribute();
	//std::cout << "write xml done \n";

}

void BonusXMLItemIrrlicht::readFrom(Deserializer* d) {

	d->beginAttribute("BonusallitemIrrlicht");
	d->readParameter("meshname", this->meshname);
	d->readParameter("ipx", this->ipx);
	d->readParameter("ipy", this->ipy);
	d->readParameter("ipz", this->ipz);
	d->readParameter("irx", this->irx);
	d->readParameter("iry", this->iry);
	d->readParameter("irz", this->irz);
	d->readParameter("pickupsound", this->pickupsound);

	//std::cout << "deserializble\n";

}
