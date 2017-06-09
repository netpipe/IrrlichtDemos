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
#ifndef BonusXMLITEMIRRLICHT_H_
#define BonusXMLITEMIRRLICHT_H_



#endif /* BonusXMLITEMIRRLICHT_H_ */

#include <irrlicht.h>
//#include "../GameItemIrrlicht.h"
#include "../ElementItemIrrlicht.h"
#include "../Serializable.h"
#include <string>
class BonusXMLIrrlicht;
class GameSceneIrrlicht;
class BonusIrrlicht;


/**
 * @brief This class is the graphical representation of a BonusXML.
 */
class BonusXMLItemIrrlicht: public ElementItemIrrlicht, public Serializable
{
private:
	int cx;
	int cz;
	int dz;
	int zstart;
	irr::scene::ISceneNode* node;

	std::string meshname;
	// initial position
	float ipx;
	float ipy;
	float ipz;
	// initial rotation
	float irx;
	float iry;
	float irz;
	// use lightning
	int lightning;

	std::string pickupsound;

	float h;
	float bounce;
	float bouncedir;

	irr::u32 lastItemAction;

public:

    /**
     * Creates a new BonusXMLItem instance.
     * @param p_model the BonusXML model
     * @param renderer the KGameRenderer
     */
    BonusXMLItemIrrlicht(BonusIrrlicht* p_model, char* xmlfile);

    BonusXMLItemIrrlicht(BonusIrrlicht* p_model);


    /**
     * Deletes the BonusXMLItem instance.
     */
    ~BonusXMLItemIrrlicht();

    void setPosition();
    void init();
    void setVisible(bool yorn);
    void update();
    void readFrom(Deserializer* id);
    void writeTo(Serializer* is);
};
