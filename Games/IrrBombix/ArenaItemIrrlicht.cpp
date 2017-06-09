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

#include "ArenaItemIrrlicht.h"
#include "ArenaIrrlicht.h"
#include "GameItemIrrlicht.h"
#include "GameIrrlicht.h"
#include "PlayerIrrlicht.h"
#include "BombIrrlicht.h"
#include <math.h>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <algorithm>

ArenaItemIrrlicht::ArenaItemIrrlicht(ArenaIrrlicht* p_model) :
	ElementItemIrrlicht(p_model) {

	//setCellSize(6);
	//setCellSize(40);
	setDrawPos(irr::core::vector3df(0, 0, 0));
	now = true;
}

ArenaItemIrrlicht::~ArenaItemIrrlicht() {
	// TODO Auto-generated destructor stub
}

irr::f32 ArenaItemIrrlicht::CellSize = 40;

irr::f32 ArenaItemIrrlicht::getCellSize() {
	return ArenaItemIrrlicht::CellSize;
}

irr::core::vector3d<irr::f32> ArenaItemIrrlicht::getDrawPos() const {
	return drawPos;
}

void ArenaItemIrrlicht::setCellSize(irr::f32 CellSize) {
	this->CellSize = CellSize;
}

void ArenaItemIrrlicht::setDrawPos(irr::core::vector3d<irr::f32> drawPos) {
	this->drawPos = drawPos;
}

void ArenaItemIrrlicht::UpdateDynamicCam() {
	ArenaIrrlicht* m = dynamic_cast<ArenaIrrlicht*> (this->getModel());
	irr::f32 z = std::max<irr::f32>(m->getNbColumns() * CellSize,
			m->getNbRows() * CellSize);
	//std::cout << "max z = " << z << "\n";
	//irr::core::vector3d<irr::f32> npos = (irr::core::vector3d<irr::f32>(m->getNbColumns() * CellSize ,z,m->getNbRows() * CellSize)*irr::core::vector3d<irr::f32>(0.5,0.6,0.15)) + getDrawPos();
	//irr::core::vector3d<irr::f32> target = (irr::core::vector3d<irr::f32>(m->getNbColumns() * CellSize,0,m->getNbRows() * CellSize)*irr::core::vector3d<irr::f32>(0.5,1,0.4))+ getDrawPos();

	irr::core::vector3d<irr::f32> npos = (irr::core::vector3d<irr::f32>(
			m->getNbColumns() * CellSize, 1 * z, m->getNbRows() * CellSize)
			* irr::core::vector3d<irr::f32>(0.5, 0.55, 0.1)) + getDrawPos();
	irr::core::vector3d<irr::f32> target = (irr::core::vector3d<irr::f32>(
			m->getNbColumns() * CellSize, 0, m->getNbRows() * CellSize)
			* irr::core::vector3d<irr::f32>(0.5, 1, 0.35)) + getDrawPos();

	g_scene->dyncam->setAspectRatio(16.0 / 9.0);

	// float rectlx=-17;

	// gameArea boundary
	float rectlx = -0.5 * CellSize;
	//float rectrx=(m->getNbColumns()+1) * CellSize+getDrawPos().X;
	float rectrx = (m->getNbColumns() - 0.5) * CellSize + getDrawPos().X;
	//float rectoy=(m->getNbRows()+1) * CellSize+getDrawPos().Y;
	float rectoy = (m->getNbRows() - 0.5) * CellSize + getDrawPos().Y;
	float rectuy = -0.5 * CellSize;

	// Calculate rect need to view every player and every bomb

	float maxx = GameIrrlicht::players[0]->getX();
	float maxy = GameIrrlicht::players[0]->getY();

	float minx = GameIrrlicht::players[0]->getX();
	float miny = GameIrrlicht::players[0]->getY();

	for (int i = 0; i < GameIrrlicht::players.size(); i++) {
		if (GameIrrlicht::players[i]->isAlive() || GameIrrlicht::players[i]->isUfo) {
			maxx = std::max(maxx, GameIrrlicht::players[i]->getX());
			maxy = std::max(maxy, GameIrrlicht::players[i]->getY());

			minx = std::min(minx, GameIrrlicht::players[i]->getX());
			miny = std::min(miny, GameIrrlicht::players[i]->getY());
		}
	}

	for (int i = 0; i < GameIrrlicht::bombs.size(); i++) {
		maxx = std::max(maxx, GameIrrlicht::bombs[i]->getX());
		maxy = std::max(maxy, GameIrrlicht::bombs[i]->getY());

		minx = std::min(minx, GameIrrlicht::bombs[i]->getX());
		miny = std::min(miny, GameIrrlicht::bombs[i]->getY());
	}

	float minrect = 0.5f;
	// space to the side execept at gamearea boundary
	maxx = std::max(maxx, rectlx + minrect * (rectrx - rectlx));
	maxy = std::max(maxy, rectuy + minrect * (rectoy - rectuy));

	minx = std::min(minx, rectlx + (1 - minrect) * (rectrx - rectlx));
	miny = std::min(miny, rectuy + (1 - minrect) * (rectoy - rectuy));

	rectrx = std::min(maxx + 2 * CellSize, rectrx + 1 * CellSize);
	rectoy = std::min(maxy + 2 * CellSize, rectoy + 1 * CellSize);

	rectlx = std::max(minx - 2 * CellSize, rectlx - 1 * CellSize);
	rectuy = std::max(miny - 3 * CellSize, rectuy - 2 * CellSize);

	//std::cout << rectlx << ", " << rectrx	<< " rectlx, rectrx ArenaItemIrrlicht\n";
	//std::cout << rectuy << ", " << rectoy	<< " rectuy, rectoy ArenaItemIrrlicht\n";

	//irr::core::vector3df tmpv = g_scene->cam->getViewFrustum()->getFarLeftDown();
	//std::cout << "FarLeftDown";
	//std::cout << "Vector3df ("<< tmpv.X <<", "<< tmpv.Y <<", "<< tmpv.Z <<")\n";


	//tmpv = g_scene->cam->getViewFrustum()->getFarRightUp();
	//std::cout << "FarRightUp";
	//std::cout << "Vector3df ("<< tmpv.X <<", "<< tmpv.Y <<", "<< tmpv.Z <<")\n";

	//g_scene->cam->set
	float alphah = g_scene->dyncam->getFOV();
	float beta = -3.14f * 0.0f / 180.0f;

	//std::cout << alphah << ", "<< beta <<" aplhah, beta ArenaItemIrrlicht\n";

	float mx = 0.5 * (rectrx - rectlx);
	float my = 0.5 * (rectoy - rectuy);

	// std::cout << mx << ", " << my << " mx, my ArenaItemIrrlicht\n";

	float maxd = std::max(mx, my);
	// std::cout << maxd << " maxd ArenaItemIrrlicht\n";

	float h = cos(beta) * maxd;
	// std::cout << h << " h  ArenaItemIrrlicht\n";

	//fov = Mathf.Atan((0.5 * height) / distance) * 2.0 * Mathf.Rad2Deg;

	//float alphaV= asin((1/g_scene->cam->getAspectRatio())*sin(alphah/2))*2;

	float alphaV = asin(sin(alphah / 2) * 1 / g_scene->dyncam->getAspectRatio())
			* 2;

	// std::cout << alphaV * 180 / 3.14 << " alphaV  ArenaItemIrrlicht\n";
	// std::cout << alphah * 180 / 3.14 << " alphah  ArenaItemIrrlicht\n";
	// std::cout << g_scene->cam->getAspectRatio()	<< " aspectRatio  ArenaItemIrrlicht\n";

	// std::cout << alphah << ", " << alphaV	<< " alphah, alphav ArenaItemIrrlicht\n";

	float d1;
	//d1 = h/tan(alphah/2);
	float d2 = sin(beta) * maxd;

	float dh = h / tan(alphah / 2);

	float dv = h / cos(alphaV / 2);

	float dvo = dv / cos(alphah / 2);

	float dho = dh / cos(alphaV / 2);

	d1 = std::max(dvo, dho);

	//d1 = 400;
	//d2 = 0;

	// std::cout << d1 << ", " << d2 << " d1, d2 ArenaItemIrrlicht\n";

	irr::core::vector3d<irr::f32> camv = irr::core::vector3d<irr::f32>(0, (d1
			+ d2) * 1.0, 0);
	camv.rotateYZBy(beta / (2 * 3.14) * 360.0f, irr::core::vector3d<irr::f32>(
			0, 0, 0));
	//camv=camv/-sin(beta);


	//std::cout << "camv "<<camv.X << " -> " << target.X << "\n";
	//std::cout << "camv "<<camv.Y << " -> " << target.Y << "\n";
	//std::cout << "camv "<<camv.Z << " -> " << target.Z << "\n";

	npos = irr::core::vector3d<irr::f32>(mx + rectlx, -CellSize * 1.5, my
			+ rectuy) + camv + getDrawPos();

	//npos = irr::core::vector3d<irr::f32>(mx,0*(-CellSize)+d1+d2,my);
	//npos.rotateYZBy(beta ,npos);

	target = irr::core::vector3d<irr::f32>(mx + rectlx, -CellSize * 1.5, my
			+ rectuy) + getDrawPos();

	irr::core::vector3df tmpv = g_scene->dyncam->getPosition();

	// std::cout << "cam Position";
	// std::cout << "Vector3df (" << tmpv.X << ", " << tmpv.Y << ", " << tmpv.Z	<< ")\n";

	tmpv = g_scene->dyncam->getTarget();
	// std::cout << "cam Target";
	// std::cout << "Vector3df (" << tmpv.X << ", " << tmpv.Y << ", " << tmpv.Z << ")\n";

	if (!now) {
		tmpv = target - oldcameratarget;
		irr::core::vector3df tmpv2 = npos - oldcamerapos;
		if (tmpv.getLength() > CellSize / 10) {
			target = oldcameratarget + (CellSize / 20) / tmpv.getLength()
					* (tmpv);

			npos = oldcamerapos + (CellSize / 20) / tmpv.getLength() * (tmpv2);
		}
	} else {
		now = false;
	}
	g_scene->dyncam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));
	g_scene->dyncam->setTarget(target);
	g_scene->dyncam->setPosition(npos + irr::core::vector3d<irr::f32>(0, 0, -0.3f
			* (d1 + d2)));
	g_scene->dyncam->setNearValue((d1 + d2) * 2.0 / 3.0);

	oldcameratarget = target;
	oldcamerapos = npos;
	//g_scene->cam->setFOV(fov);


	//		std::cout << npos.X << " -> " << target.X << "\n";
	//		std::cout << npos.Y << " -> " << target.Y << "\n";
	//		std::cout << npos.Z << " -> " << target.Z << "\n";


}

void ArenaItemIrrlicht::setCamera(GameSceneIrrlicht* g_scene) {
	ArenaIrrlicht* m = dynamic_cast<ArenaIrrlicht*> (this->getModel());
	irr::f32 z = std::max<irr::f32>(m->getNbColumns() * CellSize,
			m->getNbRows() * CellSize);
	// std::cout << "max z = " << z << "\n";

	irr::core::vector3d<irr::f32> npos = (irr::core::vector3d<irr::f32>(
			m->getNbColumns() * CellSize, 1 * z, m->getNbRows() * CellSize)
			* irr::core::vector3d<irr::f32>(0.5, 0.55, 0.1)) + getDrawPos();
	irr::core::vector3d<irr::f32> target = (irr::core::vector3d<irr::f32>(
			m->getNbColumns() * CellSize, 0, m->getNbRows() * CellSize)
			* irr::core::vector3d<irr::f32>(0.5, 1, 0.35)) + getDrawPos();

	g_scene->cam = g_scene->smgr->addCameraSceneNode(0, npos);
	g_scene->dyncam = g_scene->smgr->addCameraSceneNode(0, npos);
	g_scene->beautycam = g_scene->smgr->addCameraSceneNode(0, npos);
	g_scene->overheadcam = g_scene->smgr->addCameraSceneNode(0, npos);

	g_scene->fpscam = g_scene->smgr->addCameraSceneNodeFPS();


	irr::core::vector3d<irr::f32> overheadtarget = (irr::core::vector3d<irr::f32>(m->getNbColumns() * CellSize, 0, m->getNbRows() * CellSize)
				* irr::core::vector3d<irr::f32>(0.45, 1, 0.45)) + getDrawPos();
	irr::core::vector3d<irr::f32> overheadnpos = (irr::core::vector3d<irr::f32>(m->getNbColumns() * CellSize, z, m->getNbRows() * CellSize)
					* irr::core::vector3d<irr::f32>(0.45, 0.65, 0.45)) + getDrawPos();

	g_scene->overheadcam->setAspectRatio(16.0 / 9.0);
		//g_scene->cam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));
	g_scene->overheadcam->setTarget(overheadtarget);
	g_scene->overheadcam->setPosition(overheadnpos);
	g_scene->overheadcam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));

	g_scene->cam->setAspectRatio(16.0 / 9.0);
	//g_scene->cam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));
	g_scene->cam->setTarget(target);
	g_scene->cam->setPosition(npos);
	//g_scene->cam->setFOV(fov);

	g_scene->cam->setAspectRatio(16.0 / 9.0);
	//g_scene->cam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));
	g_scene->cam->setTarget(target);
	g_scene->cam->setPosition(npos);

	g_scene->dyncam->setAspectRatio(16.0 / 9.0);
	//g_scene->cam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));
	g_scene->dyncam->setTarget(target);
	g_scene->dyncam->setPosition(npos);


	irr::core::vector3d<irr::f32> beautytarget = (irr::core::vector3d<irr::f32>(m->getNbColumns() * CellSize, 0, m->getNbRows() * CellSize)
					* irr::core::vector3d<irr::f32>(0.45, 1, 0.45)) + getDrawPos();
		irr::core::vector3d<irr::f32> beautypos = (irr::core::vector3d<irr::f32>(m->getNbColumns() * CellSize, z, m->getNbRows() * CellSize)
						* irr::core::vector3d<irr::f32>(0.45, 0.45, -0.45)) + getDrawPos();

	g_scene->beautycam->setAspectRatio(16.0 / 9.0);
		//g_scene->cam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));
	g_scene->beautycam->setTarget(beautytarget);
	g_scene->beautycam->setPosition(beautypos);

	g_scene->fpscam->setAspectRatio(16.0 / 9.0);
			//g_scene->cam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));
	g_scene->fpscam->setTarget(beautytarget);
	g_scene->fpscam->setPosition(beautypos);

	//g_scene->fpscam->setAspectRatio(16.0 / 9.0);
	//g_scene->cam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));
	//g_scene->fpscam->setTarget(target);
	//g_scene->fpscam->setPosition(npos);
	//g_scene->fpscam->setUpVector(irr::core::vector3d<irr::f32>(0, 0, 1));

	std::cout << npos.X << " -> " << target.X << "\n";
	std::cout << npos.Y << " -> " << target.Y << "\n";
	std::cout << npos.Z << " -> " << target.Z << "\n";
	// TODO move init


	//g_scene->cam = g_scene->smgr->addCameraSceneNodeFPS();

	init();
}

void ArenaItemIrrlicht::init() {
	ArenaIrrlicht* m = dynamic_cast<ArenaIrrlicht*> (this->getModel());
	bool UseLight = true;
	//meshname="./media/Arena/RNuclear.obj";
	//meshname="./media/Arena/RLegoTxtSC.obj";
	//irx=90;
	//iry=270;
	//irz=0;

	//ipx=7.5;
	//ipy=-1.5;
	//ipz=6;
	//modellfieldsize=2;


	irr::scene::IAnimatedMesh* mesh = GameIrrlicht::m_GameItemIrrlicht->smgr->getMesh(
				meshname.c_str());

	  //Device->getSceneManager()->getMeshManipulator()->flipSurfaces( m->getMesh(0) );
	GameIrrlicht::m_GameItemIrrlicht->device->getSceneManager()->getMeshManipulator()->recalculateNormals( mesh->getMesh(0) );

	irr::scene::ISceneNode* node =
			GameIrrlicht::m_GameItemIrrlicht->smgr->addAnimatedMeshSceneNode(mesh);


	for (int i=0;i<node->getMaterialCount();i++){
		node->getMaterial(i).AmbientColor=node->getMaterial(i).DiffuseColor;
		node->getMaterial(i).ColorMaterial = irr::video::ECM_NONE;
	}



	node->setMaterialType(irr::video::EMT_SOLID);
	//node->setMaterialType(irr::video::EMT_PARALLAX_MAP_SOLID);
	//node->setMaterialFlag(irr::video::EMF_GOURAUD_SHADING, true);
	//node->setMaterialFlag(irr::video::EMF_NORMALIZE_NORMALS, UseLight);
	node->setMaterialFlag(irr::video::EMF_LIGHTING, UseLight);
	node->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);


	node->setRotation(irr::core::vector3df(irx, iry, irz));
	float dz = ArenaItemIrrlicht::getCellSize();

	irr::core::vector3df extent = node->getTransformedBoundingBox().getExtent();
	std::cout << "ArenaIrrlicht " << extent.X << " " << extent.Y << " "
			<< extent.Z << "\n";
	irr::f32 factor = (dz / modellfieldsize);
	node->setScale(irr::core::vector3d<irr::f32>(factor, factor, factor));

	//g_scene->scaleNode(node, (m->getNbColumns()+12)*dz );


	node->setPosition(irr::core::vector3df(ipx * dz, ipy * dz, (ipz * dz)));
	//node->setPosition(irr::core::vector3df(dz*12.42,-dz , (dz*14.18)));
	//animModel->setAnimationSpeed(30);
	node->setVisible(true);

	//GameIrrlicht::m_GameItemIrrlicht->smgr->setAmbientLight(irr::video::SColorf(0.0,0.0,0.0));

}
