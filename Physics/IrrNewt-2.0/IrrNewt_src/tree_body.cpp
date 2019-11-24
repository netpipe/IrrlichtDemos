// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/tree_body.hpp"
#include "../include/world.hpp"
#include "../include/irrtonewt.hpp"
#include "../include/hidden.hpp"
#include "../include/utils.hpp"
using namespace irr;

//------------------------------------------------------------------
//------------------------OCT TREE SCENE NODE CLASS--------------------------
//------------------------------------------------------------------

namespace irr{
namespace newton {
	bool getLogStatus();
}
}

irr::newton::ITreeBody::ITreeBody(
				irr::newton::IWorld* iWorld,
				irr::scene::ISceneNode* iNode,
				core::stringc file
				) {

	//deserialize collision from file
	this->deserialize(iWorld, file);

	//create body from deserialized collision
	ITreeBody::ITreeBody(iWorld, iNode, this->collision);

	//release collision object
	this->collision->release();
}

irr::newton::ITreeBody::ITreeBody(
				irr::newton::IWorld* iWorld,
				irr::scene::ISceneNode* iNode,
				irr::newton::ICollision* iCollision
				) {

	irr::newton::Hidden::StartChronometer();

	this->collision = iCollision;
	this->body      = NULL;

	this->node        = iNode;

	this->class_type.Type = ECT_TREE_BODY;

	core::vector3df scale_value = node->getScale();

	this->node->updateAbsolutePosition();

	// create a ridid body to represent the world
	this->body = NewtonCreateBody(
		iWorld->getNewtonWorld(),
		this->collision->getNewtonCollision() );

	//now we can call this->getNewtonWorld() and this->getWorld()

//  don't release collision!!!
//	this->collision->release();

	this->setNodeMatrixToBody();

	NewtonBodySetMaterialGroupID (this->body, this->getWorld()->getNewtonDefaultMaterialGroupID());
	NewtonBodySetUserData(this->body,this);

//  set id
//	NewtonConvexCollisionSetUserID(this->collision,this->node->getID());

	NewtonBodySetTransformCallback(
		this->body,
		irr::newton::Hidden::defBodyTransformCallback);


	if(irr::newton::getLogStatus()) {
		core::stringc text="IrrNewt: tree body created in ";
		text += irr::newton::Hidden::EndChronometer();
		text += " number of polys ";
		text += (int)this->getPolyCount();
		text += " face skipped ";
		text += (int)this->getSkippedFaceCount();
		this->getWorld()->getIrrlichtDevice()->getLogger()->log(text.c_str());
	}

}



irr::newton::ITreeBody::~ITreeBody() {}

void irr::newton::ITreeBody::serialize(newton::IWorld* world, irr::core::stringc file) {
	/*FILE* handle=fopen(file.c_str(),"wb");

	NewtonCollisionSerialize(
    world->getNewtonWorld(),
		this->collision->getNewtonCollision(),
		irr::newton::Hidden::NewtonSerialize,
		handle);

	fclose(handle);*/
}

bool irr::newton::ITreeBody::deserialize(newton::IWorld* world, irr::core::stringc file) {

	FILE* handle = fopen(file.c_str(),"rb");

	if( handle == NULL ) {
		irr::newton::Hidden::SetLastError(irr::newton::Hidden::FILE_NOT_FOUND);
		return false;
	}

	NewtonCollision* nCollision;

	/*nCollision = NewtonCreateTreeCollisionFromSerialization(
		world->getNewtonWorld(),  //world
		NULL,//tree collision callback
		irr::newton::Hidden::NewtonDeserialize,
		handle
		);

	fclose(handle);

	this->collision = world->createCollision(nCollision);*/

	return true;
}

