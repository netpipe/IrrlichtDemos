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
#include "BlockItemIrrlicht.h"
#include "BlockIrrlicht.h"
#include "irrlicht.h"
#include "GameItemIrrlicht.h"
#include "GameIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "ArenaIrrlicht.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


BlockItemIrrlicht::BlockItemIrrlicht(BlockIrrlicht* p_model) : ElementItemIrrlicht(p_model) {
}


BlockItemIrrlicht::BlockItemIrrlicht(BlockIrrlicht* p_model,  GameSceneIrrlicht* g_scene) : ElementItemIrrlicht(p_model) {

	this->init();
};

BlockItemIrrlicht::~BlockItemIrrlicht() {

}

void BlockItemIrrlicht::init() {
	//std::cout << "enter BlockItemIrrlicht::init() \n";

	g_scene=GameIrrlicht::m_GameItemIrrlicht;
		  /* generate secret number: */
    irr::f32 rot = ((rand() % 200) -100) / 20.0;

	ArenaItemIrrlicht* tmp = static_cast<ArenaItemIrrlicht*> (g_scene->getArenaIrrlicht()->getView());
	dz=tmp->getCellSize(); zstart=tmp->getDrawPos().Z;
	BlockIrrlicht* m = dynamic_cast<BlockIrrlicht*> (this->getModel());
	cx = zstart + m->getI() * dz;
	cz = zstart + m->getJ() * dz;
	//std::cout << this->getModel()->getI() << ", "<< this->getModel()->getJ() <<"\n";
	//std::cout << cx << ", "<< cz <<"\n";
	crate = g_scene->smgr->addCubeSceneNode(dz);
	crate->setMaterialTexture(0, g_scene->driver->getTexture("./media/crate.jpg"));
	g_scene->scaleNode(crate, dz*0.75);
	crate->setPosition(irr::core::vector3df(cx, -dz, cz));
	crate->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	crate->setRotation(irr::core::vector3df(0.0f, rot, 0.0f));
	crate->setVisible(true);
}

void BlockItemIrrlicht::setVisible(bool yorn) {
	crate->setVisible(yorn);
}
