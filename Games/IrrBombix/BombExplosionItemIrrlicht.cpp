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
#include "BombExplosionItemIrrlicht.h"
#include "BombExplosionIrrlicht.h"
#include "irrlicht.h"
#include "GameItemIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "GameIrrlicht.h"
#include <iostream>
#include <sstream>
#include "Mainwindow.h"

bombexplosionItemIrrlicht::bombexplosionItemIrrlicht(
		BombExplosionIrrlicht* p_model) :
	ElementItemIrrlicht(p_model) {
	crate = 0;
	framenr = 0;
}

bombexplosionItemIrrlicht::bombexplosionItemIrrlicht(
		BombExplosionIrrlicht* p_model, GameSceneIrrlicht* g_scene) :
	ElementItemIrrlicht(p_model) {
	crate = 0;
	framenr = 0;
	this->init();
}
;

bombexplosionItemIrrlicht::~bombexplosionItemIrrlicht() {

}

void bombexplosionItemIrrlicht::init() {
	GameSceneIrrlicht* g_scene = GameIrrlicht::m_GameItemIrrlicht;
	//std::cout << "enter bombexplosionItemIrrlicht::init() \n";

	this->framenr = 0;

	lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
	g_scene = GameIrrlicht::m_GameItemIrrlicht;

	ArenaItemIrrlicht
			* tmp =
					static_cast<ArenaItemIrrlicht*> (g_scene->getArenaIrrlicht()->getView());
	dz = tmp->getCellSize();
	zstart = tmp->getDrawPos().Z;

	IJElementIrrlicht* m = dynamic_cast<IJElementIrrlicht*> (this->getModel());
	cx = zstart + m->getI() * dz;
	cz = zstart + m->getJ() * dz;
	cy = -dz;
	//std::cout << this->getModel()->getI() << ", "<< this->getModel()->getJ() <<"\n";
	//std::cout << cx << ", "<< cz <<"\n";


	node = 0;
	crate = g_scene->smgr->addEmptySceneNode();

	for (int j = 20; j < 71; j++) {
		std::stringstream ss;
		ss << j;
		std::string* tmp = new std::string("./media/Items/explosion/p00"
				+ ss.str() + ".tm");
		bombtex[j - 20] = g_scene->driver->getTexture(tmp->c_str());
	}

	for (int j = 1; j < 11; j++) {
		std::stringstream ss;
		ss << j;
		std::string* tmp = new std::string("./media/Items/fire/f" + ss.str()
				+ ".tm");
		firetex[j - 1] = g_scene->driver->getTexture(tmp->c_str());
	}

	node = g_scene->smgr->addBillboardSceneNode(crate, irr::core::dimension2d<
			irr::f32>(2 * dz, 2 * dz));
	node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	//node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	node->setMaterialTexture(0, bombtex[0]);
	//node->setMaterialTexture(0, irr::driver->getTexture("../../media/particlewhite.bmp"));


	//	crate = g_scene->smgr->addCubeSceneNode(dz);
	//	crate->setMaterialTexture(0, g_scene->driver->getTexture("./media/fireball.bmp"));
	g_scene->scaleNode(node, dz * 2.5);
	//
	//	crate->setMaterialFlag(irr::video::EMF_LIGHTING, true);

	crate->setPosition(irr::core::vector3df(cx, -dz, cz));
	crate->setVisible(true);

}

void bombexplosionItemIrrlicht::update() {
	//std::cout << "enter bombexplosionItemIrrlicht::update() \n";
	if (Mainwindow::device->getTimer()->getTime() - lastInterfaceAction > 50) {
		lastInterfaceAction = Mainwindow::device->getTimer()->getTime();
		if (!crate) {
			init();
		}

		{
			BombExplosionIrrlicht* m =
					dynamic_cast<BombExplosionIrrlicht*> (this->getModel());
			if (!m->exploded) {
				framenr = 0;
			}
			if (m->exploded) {
				if (m->bombspot) {
					setPosition();

					framenr = (framenr + 1) % 50;

					node->setMaterialTexture(0, bombtex[framenr]);
					cy = -dz + 3 * (((dz * 100.0) / 50.0) * framenr / 100.0);
					if (framenr == 49) {
						setVisible(false);
						//m->bombspot = false;
						m->done = true;
						cy = -dz;
						return;
					}
					if (!m->done)
						setVisible(true);

				} else {
					if (!m->done) {
						setPosition();
						if (framenr == 9){
							return;
						}
						framenr = (framenr + 1) % 10;
						//std::cout << "bombexplosionitem framenr "<< framenr << "\n";
						node->setMaterialTexture(0, firetex[framenr]);
						setVisible(true);
						//setVisible(true);

					} else {
						setVisible(false);
					}
				}
			} else {
				setVisible(false);
				framenr = 0;
			}

		}
	}
	//std::cout << "leave bombexplosionItemIrrlicht::update() \n";
}

void bombexplosionItemIrrlicht::setPosition() {
	g_scene = GameIrrlicht::m_GameItemIrrlicht;

	ArenaItemIrrlicht
			* tmp =
					static_cast<ArenaItemIrrlicht*> (g_scene->getArenaIrrlicht()->getView());
	dz = tmp->getCellSize();
	zstart = tmp->getDrawPos().Z;

	IJElementIrrlicht* m = dynamic_cast<IJElementIrrlicht*> (this->getModel());
	cx = zstart + m->getI() * dz;
	cz = zstart + m->getJ() * dz;
	//std::cout << this->getModel()->getI() << ", "<< this->getModel()->getJ() <<"\n";
	//std::cout << cx << ", "<< cz <<"\n";

	crate->setPosition(irr::core::vector3df(cx, cy, cz));
}

void bombexplosionItemIrrlicht::setVisible(bool yorn) {
	crate->setVisible(yorn);
}
