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

#ifndef bombexplosionITEMIRRLICHT_H_
#define bombexplosionITEMIRRLICHT_H_


#endif /* bombexplosionITEMIRRLICHT_H_ */

#include <irrlicht.h>
//#include "GameItemIrrlicht.h"
#include "ElementItemIrrlicht.h"
class BombExplosionIrrlicht;
class GameSceneIrrlicht;


/**
 * @brief This class is the graphical representation of a bombexplosion.
 */
class bombexplosionItemIrrlicht: public ElementItemIrrlicht
{
private:
	float cx;
	float cz;
	float cy;
	int dz;
	int zstart;
	irr::scene::ISceneNode* crate;
	irr::video::ITexture* bombtex[51];
	irr::video::ITexture* firetex[11];
	int framenr;
	irr::scene::ISceneNode* node;
	irr::u32 lastInterfaceAction;
public:

    /**
     * Creates a new bombexplosionItem instance.
     * @param p_model the bombexplosion model
     * @param renderer the KGameRenderer
     */
    bombexplosionItemIrrlicht(BombExplosionIrrlicht* p_model, GameSceneIrrlicht* g_scene);

    bombexplosionItemIrrlicht(BombExplosionIrrlicht* p_model);


    /**
     * Deletes the bombexplosionItem instance.
     */
    ~bombexplosionItemIrrlicht();

    void init();
    void update();
    void setVisible(bool yorn);
    void setPosition();
};
