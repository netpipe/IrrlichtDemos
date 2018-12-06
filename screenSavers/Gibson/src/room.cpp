/* A Gibson room, including the towers */

/*
    Copyright © 2011 John Serafino
    This file is part of The Gibson Screensaver.

    The Gibson Screensaver is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Gibson Screensaver is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with The Gibson Screensaver If not, see <http://www.gnu.org/licenses/>.
*/

#include "room.h"

void Room::init()
{
	mesh.createMesh();
	mesh.loadMesh("room.3ds", false, false);
	mesh.loadTex("room.png");
	//mesh.setScale(-1,-1,-1);
	mesh.setLit(false);
	mesh.setPosition(0,0,0);
	mesh.setRotation(0,0,180);
	mesh.sceneNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
	mesh.sceneNode->setMaterialFlag(EMF_TRILINEAR_FILTER, true);

	towers[0].createMesh();
	towers[0].loadMesh("towers.obj", false, false);
	towers[0].setLit(false);
	towers[0].sceneNode->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
	towers[0].sceneNode->setMaterialFlag(EMF_TRILINEAR_FILTER, true);
	towers[0].sceneNode->setMaterialFlag(EMF_ANISOTROPIC_FILTER, true);
	towers[0].setScale(1.3,1.1,1.3);
	//towers[0].sceneNode->setMaterialFlag(EMF_BACK_FACE_CULLING, false);

	f32 x,z;
	//x = -9.125;
	//z = 9.125;

	x = -((TOWER_XSIZE/2) * TOWER_DIST);
	z = ((TOWER_YSIZE/2) * TOWER_DIST);

	towers[0].setPosition(x + 9.125,14,z);

	for(int j=0; j < TOWER_TEXTURE_COUNT; j++)
	{
		char filename[100];

		sprintf(filename, "towers1-%d.png", j+1);

		towerTex[j] = Video->getTexture(filename);
	}

	for(int j=0; j < TOWER_TEXTURE_COUNT; j++)
	{
		char filename[100];

		sprintf(filename, "towers2-%d.png", j+1);

		towerTex[j + TOWER_TEXTURE_COUNT] = Video->getTexture(filename);
	}

	cout << "Loaded textures\n";

	for(int i=1; i < TOWER_COUNT; i++)
	{
		x += TOWER_DIST;

		if((i+1)%(TOWER_XSIZE) == 0)
		{
			z -= TOWER_DIST;
			x = -((TOWER_XSIZE/2) * TOWER_DIST);
		}

		towers[i].sceneNode = towers[0].sceneNode->clone(0);
		//towers[i].copyFrom(towers[0]);
		towers[i].setPosition(x + TOWER_DIST/2,14,z);

		if(int(getRand(0,2)) > 0)
		{
			towers[i].sceneNode->setMaterialTexture(0,towerTex[TOWER_TEXTURE_COUNT]);
			type[i] = 2;
		}
		else
		{
			towers[i].sceneNode->setMaterialTexture(0,towerTex[0]);
			type[i] = 1;
		}

	}

	light.createLight(DIRECTIONAL, 0.8,0.8,0.8,1);
	light.setPosition(0,0,0);
	light.setRotation(-45,45,0);

	currentTowerTexture = 1;
	timeSinceTowerUpdate = Ray.getTime();
}

void Room::update()
{
	if((timeSinceTowerUpdate + TEXT_ANIM_WAIT_TIME) <= Ray.getTime())
	{
		currentTowerTexture++;
		if(currentTowerTexture >= TOWER_TEXTURE_COUNT)
			currentTowerTexture = 0;

		for(int i=0; i < TOWER_COUNT; i++)
		{
			if(type[i] == 1)
			{
				towers[i].sceneNode->setMaterialTexture(0, towerTex[currentTowerTexture]);
			}
			if(type[i] == 2)
			{
				towers[i].sceneNode->setMaterialTexture(0, towerTex[TOWER_TEXTURE_COUNT + currentTowerTexture]);
			}
		}


		timeSinceTowerUpdate = Ray.getTime();
	}
}
