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
#include "GroundItemIrrlicht.h"
#include "GroundIrrlicht.h"
#include "irrlicht.h"
#include "GameItemIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "GameIrrlicht.h"
#include <iostream>

GroundItemIrrlicht::GroundItemIrrlicht(GroundIrrlicht* p_model) :
	ElementItemIrrlicht(p_model) {
}

GroundItemIrrlicht::GroundItemIrrlicht(GroundIrrlicht* p_model,
		GameSceneIrrlicht* g_scene) :
	ElementItemIrrlicht(p_model) {

	this->init();
}
;

GroundItemIrrlicht::~GroundItemIrrlicht() {

}

void GroundItemIrrlicht::init() {
	GameSceneIrrlicht* g_scene = GameIrrlicht::m_GameItemIrrlicht;
	//std::cout << "enter GroundItemIrrlicht::init() \n";

	ArenaItemIrrlicht
			* tmp =
					static_cast<ArenaItemIrrlicht*> (g_scene->getArenaIrrlicht()->getView());
	irr::f32 dz = tmp->getCellSize();
	irr::f32 zstart = tmp->getDrawPos().Z;

	irr::f32 cz;// = zstart + this->getModel()->getI() * dz;
	irr::f32 cx;// = zstart + this->getModel()->getJ() * dz;


	IJElementIrrlicht* m = dynamic_cast<IJElementIrrlicht*> (this->getModel());
		cx = zstart + m->getI() * dz;
		cz = zstart + m->getJ() * dz;
	/*
	irr::scene::ISceneNode* plate = 0;
	plate = g_scene->smgr->addMeshSceneNode(g_scene->smgr->getMesh(	"./media/plate2x2.obj"));

	 plate->getMaterial(0).Shininess = 0.0f;
	 plate->getMaterial(0).AmbientColor.set(255,255,255,255);
	 plate->getMaterial(0).DiffuseColor.set(255,255,255,255);
	 plate->getMaterial(0).EmissiveColor.set(0,0,0,0);

	g_scene->scaleNode(plate, dz);
	plate->setPosition(irr::core::vector3df(cx, -dz *(8.5/6), cz));
	plate->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	plate->setMaterialType(irr::video::EMT_DETAIL_MAP );
	plate->setMaterialType(irr::video::EMT_SOLID );

	//plate->setMaterialType(irr::video::EMT_LIGHTMAP_M2);
	plate->getMaterial(0).DiffuseColor.set(0,255,255,255);
	plate->getMaterial(0).AmbientColor.set(255, 90, 140, 140);
	plate->setVisible(false);
	*/

}
