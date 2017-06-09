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

#ifndef BLOCKITEMIRRLICHT_H_
#define BLOCKITEMIRRLICHT_H_


#endif /* BLOCKITEMIRRLICHT_H_ */

#include <irrlicht.h>
//#include "GameItemIrrlicht.h"
#include "ElementItemIrrlicht.h"
class BlockIrrlicht;
class GameSceneIrrlicht;


/**
 * @brief This class is the graphical representation of a Block.
 */
class BlockItemIrrlicht: public ElementItemIrrlicht
{
private:
	int cx;
	int cz;
	int dz;
	int zstart;
	irr::scene::ISceneNode* crate;

public:

    /**
     * Creates a new BlockItem instance.
     * @param p_model the Block model
     * @param renderer the KGameRenderer
     */
    BlockItemIrrlicht(BlockIrrlicht* p_model, GameSceneIrrlicht* g_scene);

    BlockItemIrrlicht(BlockIrrlicht* p_model);


    /**
     * Deletes the BlockItem instance.
     */
    ~BlockItemIrrlicht();

    void init();
    void setVisible(bool yorn);
};
