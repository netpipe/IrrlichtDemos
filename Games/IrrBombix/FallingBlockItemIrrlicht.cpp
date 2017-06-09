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
#include "FallingBlockIrrlicht.h"
#include "irrlicht.h"
#include "GameItemIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "GameIrrlicht.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "FallingBlockItemIrrlicht.h"
#include "GameTimerIrrlicht.h"
#include "SoundManager.h"

#include <iostream>

FallingBlockItemIrrlicht::FallingBlockItemIrrlicht(
		FallingBlockIrrlicht* p_model) :
	ElementItemIrrlicht(p_model) {
	node = 0;
}

FallingBlockItemIrrlicht::FallingBlockItemIrrlicht(
		FallingBlockIrrlicht* p_model, char* xmlfile) :
	ElementItemIrrlicht(p_model) {
	node = 0;
	readFrom(new Deserializer(xmlfile));
	//this->init();
}

FallingBlockItemIrrlicht::~FallingBlockItemIrrlicht() {

}

void FallingBlockItemIrrlicht::init() {

	if (!node) {
		//std::cout << "enter FallingBlockItemIrrlicht::init() \n";
		inithigh = 3.5;
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
		ch = -dz * 1.5 + inithigh * dz;

		//std::cout << this->getModel()->getI() << ", "<< this->getModel()->getJ() <<"\n";
		//std::cout << cx << ", "<< cz <<"\n";

		irr::scene::IAnimatedMesh* mesh =
				GameIrrlicht::m_GameItemIrrlicht->smgr->getMesh(
						meshname.c_str());

		//Device->getSceneManager()->getMeshManipulator()->flipSurfaces( m->getMesh(0) );
		GameIrrlicht::m_GameItemIrrlicht->device->getSceneManager()->getMeshManipulator()->recalculateNormals(
				mesh->getMesh(0));

		node =	GameIrrlicht::m_GameItemIrrlicht->smgr->addAnimatedMeshSceneNode(mesh);

		for (int i = 0; i < node->getMaterialCount(); i++) {
			node->getMaterial(i).AmbientColor
					= node->getMaterial(i).DiffuseColor;
			node->getMaterial(i).ColorMaterial = irr::video::ECM_NONE;
		}

		//std::cout << "BombItemIrrlicht::init() coords  "<< pl->getX()<<", "<<pl->getY()<<"\n";
		node->setMaterialFlag(irr::video::EMF_LIGHTING, true);

		//node->getMaterial(0).DiffuseColor.set(55,55,55,55);
		//node->getMaterial(0).EmissiveColor.set(50,50,50,50);

		//node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, true);
		//node->setMaterialType(irr::video::EMT_SOLID);
		//node->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
		//node->setMaterialFlag(irr::video::EMF_BACK_FACE_CULLING, false);
		node->setRotation(initrotation);
		GameIrrlicht::m_GameItemIrrlicht->scaleNode(node, dz * 0.9);

		node->setPosition(initposition + irr::core::vector3df(cx, ch, cz));
		node->setVisible(false);

		donesound = false;

		//std::cout << "leave FallingBlockItemIrrlicht::init() \n";
	}
}

void FallingBlockItemIrrlicht::setPosition() {
	IJElementIrrlicht* m = dynamic_cast<IJElementIrrlicht*> (this->getModel());
	cx = zstart + m->getI() * dz;
	cz = zstart + m->getJ() * dz;
	irr::core::vector3df initrotation = irr::core::vector3df(irx, iry, irz);
	irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);
	node->setPosition(initposition * dz + irr::core::vector3df(cx, ch, cz));

}

void FallingBlockItemIrrlicht::setVisible(bool yorn) {
	//std::cout << "enter FallingBlockItemIrrlicht setVisible\n";
	if (this->node == 0) {
		init();
	}
	if (this->node) {
		//std::cout << "Drawing FallingBlockItemIrrlicht\n";
		setPosition();
		this->node->setVisible(yorn);
	} else {
		std::cout << "Could not initialise FallingBlockItemIrrlicht\n";
	}
	//std::cout << "leave FallingBlockItemIrrlicht::setVisible() \n";
}

void FallingBlockItemIrrlicht::update() {
	if (this->node == 0) {
		init();
	}
	int falltime = 750;
	FallingBlockIrrlicht* modell =
			dynamic_cast<FallingBlockIrrlicht*> (this->getModel());

	int dland = modell->landtime - GameIrrlicht::m_gametimerIrrlicht->getTime();

	//int ddie=modell->dietime-GameIrrlicht::m_gametimerIrrlicht->getTime();


	if (modell->dietime - modell->showtime
			< GameIrrlicht::m_gametimerIrrlicht->getTime()) {
		setVisible(true);
		setPosition();
	}

	if (dland < falltime) {
		if (dland > 0) {
			// let it rain

			ch = -dz * 1.5 + (inithigh * dz * dland) / falltime;
			//std::cout <<"FallingBlockItems dland "<< dland<< "ch "<<ch<<"\n";
			setPosition();
			//} else {
			if (!donesound) {
				donesound = true;
				// play a sound
				cAudio::IListener* listener =
						SoundManager::manager->getListener();
				cAudio::IAudioSource* mysound = SoundManager::manager->create(
						"bling", "media/Sounds/FallingBlock.wav", false);

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
					mysound->play3d(cAudio::cVector3(cx, ch, cz), 2.0f, false);
				}
			}
		}

	}
	//node->setRotation(node->getRotation()+irr::core::vector3df(1, 1, 1));
	//node->setRotation(node->getRotation()+irr::core::vector3df(0, 1, 0));

}

void FallingBlockItemIrrlicht::writeTo(Serializer* s) {
	//Serializer* s = new Serializer("/tmp/Bonusitem.xml");

	s->beginAttribute("BonusallitemIrrlicht");
	s->writeParameter("meshname", this->meshname);
	s->writeParameter("ipx", this->ipx);
	s->writeParameter("ipy", this->ipy);
	s->writeParameter("ipz", this->ipz);
	s->writeParameter("irx", this->irx);
	s->writeParameter("iry", this->iry);
	s->writeParameter("irz", this->irz);

	s->endAttribute();
	s->endAttribute();
	std::cout << "write xml done \n";

}

void FallingBlockItemIrrlicht::readFrom(Deserializer* d) {

	d->beginAttribute("BonusallitemIrrlicht");
	d->readParameter("meshname", this->meshname);
	d->readParameter("ipx", this->ipx);
	d->readParameter("ipy", this->ipy);
	d->readParameter("ipz", this->ipz);
	d->readParameter("irx", this->irx);
	d->readParameter("iry", this->iry);
	d->readParameter("irz", this->irz);

	std::cout << "deserializble\n";

}
