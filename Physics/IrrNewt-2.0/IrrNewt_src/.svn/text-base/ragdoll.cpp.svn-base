// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

/*#include "../include/ragdoll.hpp"
#include "../include/world.hpp"
#include "../include/body.hpp"
#include "../include/hidden.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

using namespace irr::newton::Hidden;
using namespace irr;

irr::newton::IRagDoll::IRagDoll(irr::newton::IWorld* world) {
	this->world = world;
	this->last_id = 0;
	this->head_bone_id = -1;
}

irr::newton::IRagDoll::~IRagDoll(){
	//NewtonDestroyRagDoll(this->world->getNewtonWorld(),this->n_ragdoll);
}

using namespace irr::scene;
using namespace irr::core;

void irr::newton::IRagDoll::build(
								  scene::ISceneNode* ragdollNode,
								 float bonesMass) {

	core::vector3df veloc (0, 0, 0);

	NewtonWorld* nWorld = this->world->getNewtonWorld();

	//create a ragdoll container
	this->n_ragdoll = NewtonCreateRagDoll (nWorld);


	// begin adding bones to the rad doll
	NewtonRagDollBegin (this->n_ragdoll);

	// set the force function
	NewtonRagDollSetForceAndTorqueCallback (this->n_ragdoll, irr::newton::Hidden::defRagDollForceCallback);

	// Set the trnasform function
	NewtonRagDollSetTransformCallback (this->n_ragdoll, irr::newton::Hidden::defRagDollTransformCallback);

	//build the ragdoll
	this->BuildRadDoll (this->n_ragdoll, NULL, ragdollNode, veloc, bonesMass);


	// finalize the rag doll
	NewtonRagDollEnd (this->n_ragdoll);

}


void irr::newton::IRagDoll::BuildRadDoll(
										 NewtonRagDoll* nRagDoll,
										 NewtonRagDollBone* ParentDollBone,
										 scene::ISceneNode* BoneNode,
										 core::vector3df veloc,
										 irr::f32 boneMass) {

	//get the newton world
	NewtonWorld* nWorld = this->world->getNewtonWorld();

	scene::ISceneNode* child;
	NewtonBody* BoneBody;
	NewtonRagDollBone* nRagDollBone;

	// get the box of the bone
	core::aabbox3d<f32> box (BoneNode->getBoundingBox());
	vector3df size (box.getExtent () * IrrToNewton);

	// get the bone local matrix and comver to newton units
	matrix4 matrix (BoneNode->getRelativeTransformation());
	matrix.setTranslation (matrix.getTranslation() * IrrToNewton);

	//create rag doll irrnewt obj
	irr::newton::IRagDollBone* CurrBone = new irr::newton::IRagDollBone();
	irr::newton::Hidden::AddObjectToDestroy(CurrBone);

	//build IrrNewt collision
	newton::SBodyFromNode CollisionData;
	CollisionData.Node = BoneNode;
	CollisionData.Type = EBT_PRIMITIVE_BOX;
	CollisionData.CalculateOffsetMode = newton::ECO_NONE;
	newton::ICollision* collision = this->world->createCollision( CollisionData );

	// create a rag doll bone for this bone
    nRagDollBone = NewtonRagDollAddBone (
		nRagDoll,    //ragdoll
		ParentDollBone,  //parent
		CurrBone,  //user data (ragdoll bone irrnewt)
		boneMass,   //bone mass
		newton::Hidden::GetMatrixPointer(matrix) ,  //matrix
		collision->getNewtonCollision(),  //collision shape
		&size.X   //size
		);

	//set bone id incrementally
	int IDbone = BoneNode->getID(); //id of the scene node
	NewtonRagDollBoneSetID(nRagDollBone, this->last_id);
	this->last_id++;

	// get the bone rigid body and freeze it
	BoneBody = NewtonRagDollBoneGetBody (nRagDollBone);


	//build Ragdoll bone data
	//newton bone
	CurrBone->n_bone = nRagDollBone;
	//newton ragdoll
	CurrBone->n_ragdoll = this->n_ragdoll;

	//create IBody
	CurrBone->ragdoll_bone_body = new IBody();
	irr::newton::Hidden::AddObjectToDestroy(CurrBone->ragdoll_bone_body);

	//body collision
	CurrBone->ragdoll_bone_body->collision = collision;
	//body obj
	CurrBone->ragdoll_bone_body->body = BoneBody;
	//body node
	CurrBone->ragdoll_bone_body->node = BoneNode;


	//release collision
	CurrBone->ragdoll_bone_body->collision->release();


	//-------------------------------------------------------------------------

	//set limits

	// set the rad doll bone limits
	// the x axis is usually the box main axis
	vector3df coneDir (1.0f, 0.0f, 0.0f);
	// the z axis is usually the box plane
	vector3df lateralDir (0.0f, 0.0f, 1.0f);

	// the axis limits are specifiied in global space, rotate by the joint matrix
	matrix4 BoneMatrix (BoneNode->getAbsoluteTransformation());
	BoneMatrix.rotateVect (coneDir);
	BoneMatrix.rotateVect (lateralDir);

	// set the limis
	NewtonRagDollBoneSetLimits (nRagDollBone, &coneDir.X, -PI * 0.25f, PI * 0.25f, -PI * 0.5f, &lateralDir.X, 0.0f, 0.0f);


	//------------------------------------------------------------------------

	//ORIGINAL
//  NewtonWorldFreezeBody (nWorld, body);

	//NEW
	NewtonWorldUnfreezeBody (nWorld, BoneBody);
	//NEW
	NewtonBodySetAutoFreeze (BoneBody, true);



	// recusivelly add old bones to the rag doll
	const list<ISceneNode*>& children = BoneNode->getChildren();
	for (list<ISceneNode*>::ConstIterator it = children.begin(); it != children.end(); ++it) {
		child = *it;
		this->BuildRadDoll (nRagDoll, nRagDollBone, child, veloc, boneMass);
	}
}

irr::newton::IRagDollBone* irr::newton::IRagDoll::findBone(int id) {
	NewtonRagDollBone* bone = NewtonRagDollFindBone(this->n_ragdoll,id);
	if(bone == NULL) return NULL;

	return (irr::newton::IRagDollBone*)NewtonRagDollBoneGetUserData(bone);

}

core::array<irr::newton::IRagDollBone*> irr::newton::IRagDoll::getBones() {

	core::array<irr::newton::IRagDollBone*> bones;

	for(int i=0;;i++) {

		irr::newton::IRagDollBone* bone = this->findBone(i);
		if(bone != NULL)
			bones.push_back(bone);
		else break;
	}

	return bones;
}

void irr::newton::IRagDoll::setForceToBones(core::vector3df value) {
	core::array<irr::newton::IRagDollBone*> bones = this->getBones();
	for(int i=0; i<bones.size();i++) bones[i]->getBody()->setForce(value);
}

void irr::newton::IRagDoll::addForceContinuousToBones(core::vector3df value) {
	core::array<irr::newton::IRagDollBone*> bones = this->getBones();
	for(int i=0; i<bones.size(); i++) bones[i]->getBody()->addForceContinuous(value);
}

void irr::newton::IRagDoll::setTorqueToBones(core::vector3df value) {
	core::array<irr::newton::IRagDollBone*> bones = this->getBones();
	for(int i=0; i<bones.size();i++) bones[i]->getBody()->setTorque(value);
}

void irr::newton::IRagDoll::setFreezeToBones(bool value) {
	core::array<irr::newton::IRagDollBone*> bones = this->getBones();
	for(int i=0; i<bones.size();i++) bones[i]->getBody()->setFreeze(value);
}

void irr::newton::IRagDoll::setAutoFreezeToBones(bool value) {
	core::array<irr::newton::IRagDollBone*> bones = this->getBones();
	for(int i=0; i<bones.size();i++) bones[i]->getBody()->setAutoFreeze(value);
}

void irr::newton::IRagDoll::setMaterialToBones(newton::IMaterial* material) {
	core::array<irr::newton::IRagDollBone*> bones = this->getBones();
	for(int i=0; i<bones.size();i++) bones[i]->getBody()->setMaterial(material);
}

//---------------------------------------------------------------------------------------------
//ragdoll bone implementation
//---------------------------------------------------------------------------------------------

irr::newton::IBody* irr::newton::IRagDollBone::getBody() {
	return this->ragdoll_bone_body;
}

irr::scene::ISceneNode* irr::newton::IRagDollBone::getNode() {
	return this->getBody()->getNode();
}*/
