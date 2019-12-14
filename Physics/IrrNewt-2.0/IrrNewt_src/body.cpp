// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include <stddef.h>

//#include "../include/body.hpp"
//#include "../include/newton/dMatrix.h"

#include <Newton.h>

#include "../include/body.hpp"
#include "../include/world.hpp"
#include "../include/irrtonewt.hpp"
#include "../include/hidden.hpp"
#include "../include/utils.hpp"
#include "../include/collision.hpp"

#include "../include/material.hpp"
#include <string>

using namespace irr;
using namespace irr::newton::Hidden;

#pragma warning(4:4800) //forcing value to bool 'true' or 'false' (performance warning)

namespace irr{
namespace newton {
	bool getLogStatus();
}
}


//------------------------------------------------------------------
//------------------------BODY CLASS--------------------------
//------------------------------------------------------------------


void irr::newton::IBody::Create(newton::IWorld* world, f32 mass) {


	//backup rotation and set it to 0
	this->node->updateAbsolutePosition();

	core::vector3df old_rot = this->node->getRotation();
	core::vector3df old_pos = this->node->getPosition();


	this->node->setRotation ( core::vector3df() );
	this->node->setPosition ( core::vector3df() );


	this->node->updateAbsolutePosition();
	//end of annull position and rotation


	//OFFSET

	/*

	//debug
	//core::vector3df rot_offset=body_offset_from_scene_node.getRotationDegrees();
	core::matrix4 offset;
	offset.setScale(core::vector3df(1,1,1));

	//apply scale offset and backup box size
	core::vector3df old_box_size = this->box_size;
	this->box_size *= data.BodyScaleOffsetFromNode;

	//translation offset
	core::vector3df trans_offset;

	//automatic calculation of translation offset
	if (this->calcOffsetMode == ECO_MESH && this->mesh == NULL) this->calcOffsetMode = ECO_NODE;
	if (this->calcOffsetMode == ECO_NODE && this->node == NULL) this->calcOffsetMode = ECO_NONE;


	if( this->shape_type  != newton::EBT_CONVEX_HULL          &&
		this->shape_type  != newton::EBT_CONVEX_HULL_MODIFIER &&
		this->calcOffsetMode != ECO_NONE) {

		//if mesh is NULL try to calculate the offset from the node (not so good)

		if( this->calcOffsetMode == ECO_NODE ) {

			//calculate translation offset
			core::aabbox3df transf_box = this->node->getTransformedBoundingBox();

			core::vector3df t_center = transf_box.getCenter();
			core::vector3df ut_center = this->node->getBoundingBox().getCenter();

			core::vector3df position = this->node->getPosition();
			core::vector3df abs_position = this->node->getAbsolutePosition();

			trans_offset = this->node->getTransformedBoundingBox().getCenter() - this->node->getAbsolutePosition();

		}

		//calculate the offset from the mesh (better?)
		//this->calcOffsetMode is ECO_MESH
		else {
			trans_offset = mesh->getBoundingBox().getCenter();
		}


		//convert in newton unit
		trans_offset *= IrrToNewton;

		//set translation offset
		offset.setTranslation(trans_offset);


  	}//if(this->shape_type!=newton::EST_CONVEX_HULL&&


	//convert user defind translation in newton unit
	data.BodyOffsetFromNode.setTranslation( data.BodyOffsetFromNode.getTranslation() * IrrToNewton);
	data.BodyOffsetFromNode.setScale(core::vector3df(1,1,1));

	//add user offset with automatic calculated offset
	offset = irr::newton::Hidden::AddMatrix(offset, data.BodyOffsetFromNode);

  */

	//END OF OFFSET

//   create collision
//	this->CreateCollision(this->box_size, offset, mesh);
//	this->collision = this->getWorld->createCollision(data);

#define test //testing newer 2.35 newton
#ifdef test

const dFloat* const matrix;
	//create body
	this->body = NewtonCreateBody(world->getNewtonWorld(), this->collision->getNewtonCollision(),matrix );
	NewtonBodySetUserData(this->body, this);
#else
	//create body
	this->body = NewtonCreateBody(world->getNewtonWorld(), this->collision->getNewtonCollision() );
	NewtonBodySetUserData(this->body, this);
#endif
//      don't release collision object!!!
//		this->collision->release();

	irr::f32 initial_force[3] = {0,0,0};
	NewtonBodySetForce(this->body,initial_force);

	//restoring
	//this->box_size=old_box_size;
	this->node->setRotation(old_rot);
	this->node->setPosition(old_pos);
	this->node->updateAbsolutePosition();
	this->setNodeMatrixToBody();

	//for camera
	if(this->node->getType()==scene::ESNT_CAMERA||
		this->node->getType()==scene::ESNT_CAMERA_MAYA||
		this->node->getType()==scene::ESNT_CAMERA_FPS) {

		NewtonBodySetTransformCallback(this->body,
			irr::newton::Hidden::defCameraBodyTransformCallback);
		this->camera = true;
	}

	else
		NewtonBodySetTransformCallback(this->body,
			irr::newton::Hidden::defBodyTransformCallback);

	NewtonBodySetForceAndTorqueCallback(this->body,
			irr::newton::Hidden::defBodyForceAndTorqueCallback);

	NewtonBodySetMaterialGroupID(this->body, world->getNewtonDefaultMaterialGroupID() );
	NewtonBodySetAutoSleep(this->body, 0);
	//NewtonWorldUnfreezeBody(world->getNewtonWorld(), this->body);


//      set id
//		NewtonConvexCollisionSetUserID(this->collision,this->node->getID());


//		this->applied_offset = offset;

	//set mass and calculate moments of inertia
	this->setMass(mass);
	this->calculateMomentOfInertia();

	//this->getWorld->getIrrlichtDevice()->getLogger()->log("IrrNewt: body with primitive collision or a convex hull body has created");

}

void irr::newton::IBody::init() {
	this->node  = NULL;
	this->camera = false;
	this->user_data = NULL;
	this->class_type.Type = ECT_BODY;
	this->setting_force = true;
}

irr::newton::IBody::IBody(
		irr::newton::IWorld* iWorld,
		scene::ISceneNode* node,
		newton::ICollision* iCollision,
		f32 mass) {

	this->init();

	/*
	this->body_offset_from_node = data.BodyOffsetFromNode;
	this->mesh = data.Mesh;
	this->calcOffsetMode = data.calculateOffsetMode;
	this->calcSizeMode   = data.calculateSizeMode;
	this->body_scale_offset_from_node = data.BodyScaleOffsetFromNode;

  */

//	this->bodyData = data;

	this->collision = iCollision;

	this->node  = node;

	this->Create(iWorld, mass);
}

void irr::newton::IBody::restoreCallbacks() {
	NewtonBodySetTransformCallback(this->body,
			irr::newton::Hidden::defBodyTransformCallback);

	NewtonBodySetForceAndTorqueCallback(this->body,
		irr::newton::Hidden::defBodyForceAndTorqueCallback);
}

void irr::newton::IBody::removeBody() {

	if(	this->getNewtonWorld() &&
		this->body) {

		//set callbacks to NULL
		NewtonBodySetTransformCallback(this->body,NULL);
		NewtonBodySetForceAndTorqueCallback(this->body,NULL);

		//destroy body
		NewtonDestroyBody(this->getNewtonWorld(),this->body);

		//set body pointer to NULL
		this->body=NULL;

		/*
		irr::newton::Hidden::EraseFromDestroyList(this);
		delete this;
		*/
	}

}

void irr::newton::IBody::remove() {
	if(	this->getNewtonWorld()&&
		this->body) {

		NewtonBodySetTransformCallback(this->body,NULL);
		NewtonBodySetForceAndTorqueCallback(this->body,NULL);
		NewtonDestroyBody(this->getNewtonWorld(),this->body);

		irr::newton::Hidden::EraseFromDestroyList(this);
		this->node->remove();
		delete this;
	}
}

void irr::newton::IBody::setCoriolisForcesMode(bool value) {
	//NewtonBodyCoriolisForcesMode(this->body,value);
}

//various function
void irr::newton::IBody::setNodeMatrixToBody() {
	core::matrix4 irr_mat = this->node->getRelativeTransformation();
	irr_mat.setTranslation(irr_mat.getTranslation()*IrrToNewton);

	NewtonBodySetMatrix(
		this->body,
		GetMatrixPointer(irr_mat));
}

void irr::newton::IBody::setMomentOfInertia(core::vector3df inertia) {

	irr::f32 mass,inx,iny,inz;

	NewtonBodyGetMassMatrix(this->body,&mass,&inx,&iny,&inz);
	NewtonBodySetMassMatrix(
		this->body,
		mass,
		inertia.X,
		inertia.Y,
		inertia.Z);
}

void irr::newton::IBody::setMaterial(irr::newton::IMaterial* new_material) {
	this->material=new_material;
	NewtonBodySetMaterialGroupID(this->body,new_material->getID());
}

void irr::newton::IBody::setAutoFreeze(bool value) {
	NewtonBodySetAutoSleep(this->body, value);
}

void irr::newton::IBody::setCentreOfMass(irr::core::vector3df centre) {
	irr::f32 mass_center_array[3];
	irr::newton::Hidden::Vector3dfFillArray(centre,mass_center_array);
	NewtonBodySetCentreOfMass(this->body,mass_center_array);
}

irr::core::vector3df irr::newton::IBody::getCentreOfMass() {
	irr::f32 mass_center_array[3];
	NewtonBodyGetCentreOfMass(this->body,mass_center_array);
	return irr::newton::Hidden::Vector3dfFromArray(
		mass_center_array);
}

void irr::newton::IBody::setMass(irr::f32 mass) {
	irr::f32 current_mass,inx,iny,inz;
	NewtonBodyGetMassMatrix(this->body,&current_mass,&inx,&iny,&inz);
	NewtonBodySetMassMatrix(this->body,mass/*no current_mass*/,inx,iny,inz);
}

void irr::newton::IBody::setLinearDamping(irr::f32 value) {
	NewtonBodySetLinearDamping(this->body,value);
}
irr::core::vector3df irr::newton::IBody::getMomentOfInertia() {
	irr::f32 mass,inx,iny,inz;
	NewtonBodyGetMassMatrix(this->body,&mass,&inx,&iny,&inz);
	return core::vector3df(inx,iny,inz);
}

void irr::newton::IBody::setFreeze(bool value) {
  NewtonBodySetFreezeState(this->body, value);
}

void irr::newton::IBody::drawDebugInfo() {
	//int i;
	core::matrix4 mat;
	video::SMaterial material;

	material.setTexture(0,NULL);
	material.Lighting = false;

	irr::IrrlichtDevice* device = const_cast<irr::IrrlichtDevice*>(this->getWorld()->getIrrlichtDevice());
	device->getVideoDriver()->setTransform(video::ETS_WORLD, mat);
	device->getVideoDriver()->setMaterial(material);

	// dray the collision in wire frame
	 NewtonCollisionForEachPolygonDo(
    this->collision->getNewtonCollision(),
    GetMatrixPointer(getNewtonMatrix()),
    irr::newton::Hidden::NewtonDebugCollision,
    getUserData());

	// draw all contact points
	/*
	for (i = 0; i < m_contactCount; i ++) {
		driver->draw3DLine (m_contactNormals[i * 2], m_contactNormals[i * 2 + 1], video::SColor(255, 255, 0, 0));
	}
	*/
}

irr::f32 irr::newton::IBody::getMass(){
	irr::f32 o_mass,inx,iny,inz;
	NewtonBodyGetMassMatrix(this->body,&o_mass,&inx,&iny,&inz);
	return o_mass;
}
void irr::newton::IBody::addForce(irr::core::vector3df force) {
	this->force += force;
}

void irr::newton::IBody::addForceContinuous(irr::core::vector3df force) {
	this->force_continuos += force;
}

void irr::newton::IBody::addTorque(irr::core::vector3df torque) {
	this->torque+=torque;
}

void irr::newton::IBody::setOmega(irr::core::vector3df omega) {
	irr::f32 omega_array[3];
	irr::newton::Hidden::Vector3dfFillArray(omega,omega_array);

	NewtonBodySetOmega(this->body,omega_array);
}

void irr::newton::IBody::setVelocity(irr::core::vector3df velocity) {
	irr::f32 velocity_array[3];
	irr::newton::Hidden::Vector3dfFillArray(velocity,velocity_array);

	NewtonBodySetVelocity(this->body,velocity_array);
}

irr::core::matrix4 irr::newton::IBody::getNewtonMatrix() {

	/*
	irr::core::matrix4 mat;
	NewtonBodyGetMatrix(this->body,&mat.M[0]);
	return mat;
	*/

	irr::core::matrix4 mat;
	mat.setTranslation(this->node->getPosition()*IrrToNewton);
	mat.setRotationDegrees(this->node->getRotation());
	mat.setScale(core::vector3df(1,1,1));
	return mat;
}

void irr::newton::IBody::calculateMomentOfInertia() {

	irr::f32 mass = this->getMass();
	core::vector3df inertia,origin;
	irr::f32 inertia_array[3],origin_array[3];

	NewtonConvexCollisionCalculateInertialMatrix(
		this->collision->getNewtonCollision(),
		inertia_array,
		origin_array);

	inertia = irr::newton::Hidden::Vector3dfFromArray(inertia_array);
	origin = irr::newton::Hidden::Vector3dfFromArray(origin_array);

	inertia *= mass;

	this->setMomentOfInertia(inertia);
	this->setCentreOfMass(origin);
}

//set function
void irr::newton::IBody::setPosition(irr::core::vector3df pos) {
	//set node position
	this->node->setPosition(pos);

	//set physics geometry position
	irr::core::matrix4 irr_mat;
	NewtonBodyGetMatrix(this->body,GetMatrixPointer(irr_mat));
	irr_mat.setTranslation(pos*IrrToNewton);
	NewtonBodySetMatrix(this->body,GetMatrixPointer(irr_mat));
}

void irr::newton::IBody::setRotation(irr::core::vector3df rot) {
	this->node->setRotation(rot);

	core::matrix4 temp_mat;
	NewtonBodyGetMatrix(this->body,GetMatrixPointer(temp_mat));
	temp_mat.setRotationDegrees(rot);
	NewtonBodySetMatrix(this->body,GetMatrixPointer(temp_mat));

	/*
	if(this->camera)
		irr::newton::Hidden::correctCameraTarget((scene::ICameraSceneNode*)this->node);
		*/
}

void irr::newton::IBody::setRotationBody(irr::core::vector3df rot) {
	core::matrix4 temp_mat;
	NewtonBodyGetMatrix(this->body,GetMatrixPointer(temp_mat));
	temp_mat.setRotationDegrees(rot);
	NewtonBodySetMatrix(this->body,GetMatrixPointer(temp_mat));
}

irr::core::vector3df irr::newton::IBody::getOmega() {
	irr::f32 omega_array[3];
	NewtonBodyGetOmega(this->body,omega_array);
	return core::vector3df(
		omega_array[0],
		omega_array[1],
		omega_array[2]);
}

irr::core::aabbox3d<irr::f32> irr::newton::IBody::calculateAABB() {
	float matrix[16];
	NewtonBodyGetMatrix(this->body,matrix);

	float p0_array[3],p1_array[3];
	core::vector3df p0,p1;
	NewtonCollisionCalculateAABB(this->collision->getNewtonCollision(), matrix,p0_array,p1_array);

	p0=irr::newton::Hidden::Vector3dfFromArray(p0_array);
	p1=irr::newton::Hidden::Vector3dfFromArray(p1_array);

	p0*=NewtonToIrr;
	p1*=NewtonToIrr;

	core::aabbox3d<f32> box(p0,p1);

	return box;
}

void irr::newton::IBody::setAngularDamping(core::vector3df value) {
	float angular_damping_array[3];
	irr::newton::Hidden::Vector3dfFillArray(value,angular_damping_array);

	NewtonBodySetAngularDamping(this->body,angular_damping_array);
}

irr::core::vector3df irr::newton::IBody::getVelocity() {
	irr::f32 velocity_array[3]={0,0,0};
	NewtonBodyGetVelocity(this->body,velocity_array);
	return irr::newton::Hidden::Vector3dfFromArray(velocity_array);
}

/*
irr::core::vector3df irr::newton::IBody::getForce() {
	irr::f32 force_array[3]={0,0,0};
	NewtonBodyGetForce(this->body,force_array);
	return irr::newton::Hidden::Vector3dfFromArray(force_array);
}
*/

void irr::newton::IBody::setContinuousCollisionMode(bool value) {
	if(value)
		NewtonBodySetContinuousCollisionMode(this->body,1);
	else
		NewtonBodySetContinuousCollisionMode(this->body,0);
}

bool irr::newton::IBody::getContinuousCollisionMode() {
	return static_cast<bool>(NewtonBodyGetContinuousCollisionMode(this->body));
}

core::vector3df irr::newton::IBody::getRotationBody() {
	core::matrix4 temp_mat;
	NewtonBodyGetMatrix(this->body,GetMatrixPointer(temp_mat));
	return temp_mat.getRotationDegrees();
}

core::vector3df irr::newton::IBody::getPositionBody() {
	core::matrix4 temp_mat;
	NewtonBodyGetMatrix(this->body,GetMatrixPointer(temp_mat));
	core::vector3df pos=temp_mat.getTranslation()*NewtonToIrr;
	return pos;
}

/*
void irr::newton::IBody::setScale(irr::core::vector3df scale) {
	this->node->setScale(scale);

	core::matrix4 temp_mat;
	NewtonBodyGetMatrix(this->body,&temp_mat.M[0]);
	temp_mat.setScale(scale);
	NewtonBodySetMatrix(this->body,&temp_mat.M[0]);
}
*/

void irr::newton::IBody::setScale(irr::core::vector3df new_box_size) {
		node->setScale(new_box_size);
		node->updateAbsolutePosition();
		this->setScaleBody(new_box_size);
}

void irr::newton::IBody::setScaleBody(irr::core::vector3df new_box_size) {

	if(this->getType() != newton::EBT_CONVEX_HULL_MODIFIER) {

		core::vector3df node_backup_scale = this->getNode()->getScale();

		node->setScale(new_box_size);
		node->updateAbsolutePosition();

	  //backup data
		/*
		irr::core::vector3df inertia=this->getMomentOfInertia();
		irr::core::vector3df mass_center=this->getCentreOfMass();
		irr::f32 mass=this->getMass();
		irr::newton::IMaterial* material=this->getMaterial();
		irr::core::vector3df position=this->getPositionBody();
		irr::core::vector3df rotation=this->getRotationBody();
		void* user_data=this->getUserData();
		bool continuous_collision_mode=this->getContinuousCollisionMode();

  */



		//change collision geometry
		this->collision->create( this->collision->getCreationData() );
		this->setCollision(this->collision);
		this->collision->release();




		//restore body data
		/*
		this->setCentreOfMass(mass_center);
		this->setMomentOfInertia(inertia);
		this->setMaterial(material);
		this->setPosition(position);
		this->setRotationBody(rotation);
		this->setUserData(user_data);
		this->setContinuousCollisionMode(continuous_collision_mode);
		*/

		//restore node scale
		this->getNode()->setScale(node_backup_scale);

	}//	if(this->shape_type != newton::EST_CONVEX_HULL_MODIFIER) {


	//if body type is convex hull modifier
	else {
		core::matrix4 mat;
		mat = this->getMatrixNewton();
		mat.setScale(new_box_size);
		this->setMatrixNewton(mat);
	}


}

void irr::newton::IBody::addImpulse(
	irr::core::vector3df delta_velocity,
	irr::core::vector3df impulse_center) {

	//impulse_center*=IrrToNewton;
	//delta_velocity*=IrrToNewton;

	float delta_velocity_array[3],impulse_center_array[3];
	irr::newton::Hidden::Vector3dfFillArray(delta_velocity,delta_velocity_array);
	irr::newton::Hidden::Vector3dfFillArray(impulse_center,impulse_center_array);

	NewtonBodyAddImpulse(
		this->body,
		delta_velocity_array,
		impulse_center_array);
}


//get all poligons

irr::core::array<irr::newton::SPolygon> all_poligons;

void NewtonGetAllPoligonsCallback(
											   const NewtonBody* body,
											   int vertexCount,
											   const float* faceVertec,
											   int id)
{
	irr::newton::SPolygon this_poly;

	for (irr::s32 i = 0; i < vertexCount; i ++) {

		this_poly.Vertices.push_back(core::vector3df(
			faceVertec[i*3+0]*NewtonToIrr,
			faceVertec[i*3+1]*NewtonToIrr,
			faceVertec[i*3+2]*NewtonToIrr
			));

		/*
		core::vector3df p1 (faceVertec[i * 3 + 0] * NewtonToIrr, faceVertec[i * 3 + 1] * NewtonToIrr, faceVertec[i * 3 + 2] * NewtonToIrr);
	//hidden_device->getVideoDriver()->draw3DLine (
		//p0, p1, video::SColor(255, 255, 255, 0));
 		p0 = p1;
		*/

	}

	all_poligons.push_back(this_poly);
}

irr::core::array<irr::newton::SPolygon> irr::newton::IBody::getAllPoligons()
{
	all_poligons.clear();
	//NewtonCollisionForEachPolygonDo(this->collision->getNewtonCollision(), NewtonGetAllPoligonsCallback);
	return all_poligons;

}

//end of get all poligons

irr::core::matrix4 irr::newton::IBody::getMatrixNewton() {
	core::matrix4 mat;
	NewtonBodyGetMatrix(this->body,GetMatrixPointer(mat));
	return mat;
}

void irr::newton::IBody::setMatrixIrrlicht(core::matrix4 mat) {
	mat.setTranslation( mat.getTranslation() * IrrToNewton );
	this->setMatrixNewton(mat);
}

void irr::newton::IBody::setMatrixNewton(core::matrix4 mat) {
	NewtonBodySetMatrix(this->body, irr::newton::Hidden::GetMatrixPointer(mat));
}

void irr::newton::IBody::setMatrixRecursive(core::matrix4 mat) {
	mat.setTranslation(mat.getTranslation()*IrrToNewton);
	NewtonBodySetMatrixRecursive(this->body,GetMatrixPointer(mat));
}

void irr::newton::IBody::setFreezeTreshold(
	irr::f32 freezeSpeedMag2,
	irr::f32 freezeOmegaMag2,
	irr::s32 framesCount) {

	//NewtonBodySetFreezeTreshold(this->body,freezeSpeedMag2,freezeOmegaMag2,framesCount);
}

void irr::newton::IBody::getFreezeTreshold(
	irr::f32* freezeSpeedMag2,
	irr::f32* freezeOmegaMag2) {

	//NewtonBodyGetFreezeTreshold(this->body,freezeSpeedMag2,freezeOmegaMag2);
}


//----utility functions---------------

void irr::newton::IBody::addGlobalForce(core::vector3df force, core::vector3df point)
{
   point*=IrrToNewton;

   core::vector3df R = point - this->getMatrixNewton().getTranslation();
   core::vector3df torque = R.crossProduct(force);
   this->addForce(force);
   this->addTorque(torque);
}

void irr::newton::IBody::addLocalForce (core::vector3df localForce, core::vector3df localPoint)
{
	//localPoint is in irrlicht coordinate so this function operate in irrlicht coordinate system
	core::matrix4 bodyMatrix=this->getMatrixIrrlicht();
	bodyMatrix.rotateVect(localForce);
	bodyMatrix.transformVect(localPoint);

	core::vector3df GlobaForce = localForce;
	core::vector3df GlobalPoint = localPoint;
   this->addGlobalForce (GlobaForce, GlobalPoint);
}

void irr::newton::IBody::setCollision(newton::ICollision* newCollision) {

	NewtonBodySetCollision(this->body, newCollision->getNewtonCollision());
	this->collision = newCollision;

}
