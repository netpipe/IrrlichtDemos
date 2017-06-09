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

#include "WallItemIrrlicht.h"
#include "WallIrrlicht.h"
#include "irrlicht.h"
#include "GameItemIrrlicht.h"
#include "ArenaItemIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "GameIrrlicht.h"
#include <iostream>

WallItemIrrlicht::WallItemIrrlicht(WallIrrlicht* p_model) :
	ElementItemIrrlicht(p_model) {
}

WallItemIrrlicht::WallItemIrrlicht(WallIrrlicht* p_model,
		GameSceneIrrlicht* g_scene) :
	ElementItemIrrlicht(p_model) {

	this->init();
}
;

WallItemIrrlicht::~WallItemIrrlicht() {

}

void WallItemIrrlicht::init() {
	GameSceneIrrlicht* g_scene = GameIrrlicht::m_GameItemIrrlicht;
	//std::cout << "enter WallItemIrrlicht::init() \n";

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

	irr::scene::ISceneNode* brick = 0;
	/*
	brick = g_scene->smgr->addMeshSceneNode(g_scene->smgr->getMesh("./media/brick2x2.obj"));

	g_scene->scaleNode(brick, dz);
	//std::cout << " Brick nx "<<<<"\n";
	brick->setScale(brick->getScale() * irr::core::vector3df(1, 2, 1));


	brick->setPosition(irr::core::vector3df(cx, -dz*8.5/6, cz));
	brick->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	//brick->setMaterialFlag(irr::video::EMT_LIGHTING, true);
	//brick->setMaterialType(irr::video::EMT_SOLID);
	//brick->setMaterialType(irr::video::EMT_DETAIL_MAP );
	//brick->setMaterialType(irr::video::EMT_SOLID );
	//brick->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	//std::cout << brick->getMaterialCount() << "\n";

	int i=m->getI();
	int j=m->getJ();
//	if ((i + j) % 3 == 0) {
//		//brick->getMaterial(0).AmbientColor.set(255, 0, 255, 0);
//		brick->getMaterial(0).DiffuseColor.set(255, 0, 255, 0);
//		brick->getMaterial(0).EmissiveColor.set(255, 0, 255, 0);
//		brick->getMaterial(0).Lighting=false;
//	} else if ((i + j) % 3 == 1) {
//		brick->getMaterial(0).AmbientColor.set(255, 255, 0, 0);
//		brick->getMaterial(0).DiffuseColor.set(255, 255, 0, 0);
//		brick->getMaterial(0).EmissiveColor.set(255, 255, 0, 0);
//	} else if ((i + j) % 3 == 2) {
//		brick->getMaterial(0).AmbientColor.set(255, 255, 255, 0);
//		brick->getMaterial(0).DiffuseColor.set(255, 255, 255, 0);
//		brick->getMaterial(0).EmissiveColor.set(255, 255, 255, 0);
//	}

	brick->setVisible(false);

	//std::cout<< "WallItemI "<<brick->getPosition().X<<", "<<brick->getPosition().Y<<", "<<brick->getPosition().Z<<"\n";
*/
}
