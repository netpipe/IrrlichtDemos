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

#include "ElementItemIrrlicht.h"
#include "TerrainIrrlicht.h"
#include "irrlicht.h"
#include <string>
#include "Serializable.h"

class PlayerIrrlicht;
class GameSceneIrrlicht;
//class TerrainIrrlicht;

#ifndef PlayerITEMIRRLICHT_H_
#define PlayerITEMIRRLICHT_H_

class PlayerItemIrrlicht: public ElementItemIrrlicht, public Serializable {
public:
	PlayerItemIrrlicht();
	virtual ~PlayerItemIrrlicht();
	PlayerItemIrrlicht(PlayerIrrlicht* p_model);
	PlayerItemIrrlicht(PlayerIrrlicht* p_model,  GameSceneIrrlicht* g_scene);

	void init();

	void update();
	float loosing();
	float winning();
	float running();
	float ufoing();

	bool setCharacterfile(std::string* string);
	std::string* getCharacterfile();

	void readFrom(Deserializer* id);
	void writeTo(Serializer* is);

	std::string iconname;

private:

	float loopABBA(int a, int b, float tmp);
	float loopABAB(int a, int b, float tmp);
	irr::scene::IAnimatedMeshSceneNode *node;
	irr::scene::IAnimatedMeshSceneNode* ufoModel;
	irr::video::IImage* sick;
	irr::video::ITexture* origTex;
	irr::video::ITexture** sickTex;
	int texturemat;
	int blinkdir;
	float sickblink;
	//irr::scene::ISceneNode *node;
	std::string* characterfile;


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


		irr::core::vector3df ufoHigh;



	long int standingstart;
	long int standingend;

	long int runningstart;
	long int runningend;
	long int winningstart;
	long int winningend;

	long int loosingstart;
	long int loosingend;
	float deadhigh;

	float animdirection;
	float animspeed;

	irr::u32 lastItemAction;
};

#endif /* PlayerITEMIRRLICHT_H_ */

