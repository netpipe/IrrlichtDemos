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

#ifndef HASMODELL_H_
#define HASMODELL_H_
#include "irrlicht.h"
#include <string>
#include "Serializable.h"
#include "Deserializer.h"

class hasModell: public Serializable{
public:
	hasModell();
	virtual ~hasModell();
	irr::scene::ISceneNode* readModell(std::string file, irr::scene::ISceneManager* smgr);
	void writeModell(std::string file);

	irr::scene::ISceneNode* node2;
	irr::scene::IAnimatedMeshSceneNode* animModel;
private:

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
	void readFrom(Deserializer* id);
	void writeTo(Serializer* is);

};

#endif /* HASMODELL_H_ */
