#include "stdafx.h"
#include "planet.h"

planet::planet(irr::IrrlichtDevice *graphics, IAnimatedMesh *model, core::vector3df position, core::vector3df scale)
{
	this->model = graphics->getSceneManager()->addAnimatedMeshSceneNode(model);
	this->model->setPosition(position);
	this->model->setScale(scale);
	this->model->getMaterial(0).NormalizeNormals=true;

	//remember to add a corona!
	//thats the whole purpose of creating a new planet class!
}

//This func is called by the gameManager loop
//TODO : proper planet cycles
void planet::rotate(f32 frameDeltaTime)
{
	core::vector3df rot = model->getRotation();
	rot.Y+=1*frameDeltaTime;
	model->setRotation(rot);
}

planet::~planet()
{
}
