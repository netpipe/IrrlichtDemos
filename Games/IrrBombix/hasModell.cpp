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

#include "hasModell.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "Mainwindow.h"

hasModell::hasModell() {
	node2=0;
	// TODO Auto-generated constructor stub

}

hasModell::~hasModell() {
	// TODO Auto-generated destructor stub
}

irr::scene::ISceneNode* hasModell::readModell(std::string file, irr::scene::ISceneManager* smgr){
	Deserializer* ds = new Deserializer(file.c_str());
	readFrom(ds);

	node2 = smgr->addEmptySceneNode();

	irr::core::vector3df initrotation = irr::core::vector3df(irx, iry, irz);
	irr::core::vector3df initposition = irr::core::vector3df(ipx, ipy, ipz);


	irr::scene::IAnimatedMesh* m = smgr->getMesh(meshname.c_str());

		if (!m) {
			std::cout << "The model could not be loaded. "
				L"Maybe it is not a supported file format.";
		}
	Mainwindow::device->getSceneManager()->getMeshManipulator()->recalculateNormals(
						m->getMesh(0));
	//animModel =	smgr->addAnimatedMeshSceneNode(m);
	animModel =	smgr->addAnimatedMeshSceneNode(m);
	//animModel =	GameIrrlicht::m_GameItemIrrlicht->smgr->addAnimatedMeshSceneNode(mesh);

			for (int i = 0; i < animModel->getMaterialCount(); i++) {
				animModel->getMaterial(i).AmbientColor
						= animModel->getMaterial(i).DiffuseColor;
				animModel->getMaterial(i).ColorMaterial = irr::video::ECM_NONE;
			}

	animModel->setPosition(initposition);
	animModel->setRotation(initrotation);
	animModel->setMaterialFlag(irr::video::EMF_ANISOTROPIC_FILTER, true);
	animModel->setMaterialFlag(irr::video::EMF_LIGHTING, true);
	node2->addChild(animModel);
	//node2->setVisible(false);
	return node2;
}

void hasModell::writeModell(std::string file){
	Serializer* s = new Serializer(file.c_str());
	writeTo(s);
}

void hasModell::writeTo(Serializer* s) {
	s->beginAttribute("Modell");
	s->writeParameter("meshname", this->meshname);
	s->writeParameter("ipx", this->ipx);
	s->writeParameter("ipy", this->ipy);
	s->writeParameter("ipz", this->ipz);
	s->writeParameter("irx", this->irx);
	s->writeParameter("iry", this->iry);
	s->writeParameter("irz", this->irz);

	s->endAttribute();
	s->endAttribute();
	std::cout << "write xml done \n";

}

void hasModell::readFrom(Deserializer* d) {

	d->beginAttribute("Modell");
	d->readParameter("meshname", this->meshname);
	d->readParameter("ipx", this->ipx);
	d->readParameter("ipy", this->ipy);
	d->readParameter("ipz", this->ipz);
	d->readParameter("irx", this->irx);
	d->readParameter("iry", this->iry);
	d->readParameter("irz", this->irz);

	std::cout << "deserializble\n";

}
