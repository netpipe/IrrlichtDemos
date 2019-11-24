// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/hidden.hpp"
#include "../include/irrnewt_base.hpp"
#include "../include/material_collision_callback.hpp"
#include "../include/body.hpp"
#include "../include/ragdoll.hpp"
#include "../include/material_pair.hpp"
#include "../include/material_pair_and_contact.hpp"
#include "../include/contact.hpp"
#include "../include/body_controller.hpp"
#include "../include/vehicle_simple.hpp"
#include "../include/vehicle_tire.hpp"
#include "../include/pos_rot.hpp"
#include "../include/intersection_point.hpp"
#include "../include/utils.hpp"
#include "../include/world.hpp"
#include "../include/camera_fps.hpp"

//joints
#include "../include/joint.hpp"
#include "../include/joint_universal.hpp"
#include "../include/joint_upvector.hpp"
#include "../include/joint_hinge.hpp"
#include "../include/joint_corkscrew.hpp"
#include "../include/joint_slider.hpp"
#include "../include/joint_ball.hpp"

#include <cstdlib>

using namespace irr;

static IrrlichtDevice* hidden_device;
static irr::newton::Hidden::ERROR_CODE last_error=irr::newton::Hidden::NO_ERRORS;
static irr::core::array<irr::newton::IrrNewt_Base*> objs_to_destroy;


irr::core::array<irr::newton::IrrNewt_Base*>* irr::newton::Hidden::GetObjsList() {
	return &objs_to_destroy;
}

void irr::newton::Hidden::SetDevice(irr::IrrlichtDevice* device) {
	hidden_device=device;
}

irr::IrrlichtDevice* irr::newton::Hidden::GetDevice() {
	return hidden_device;
}

void irr::newton::Hidden::NewtonDebugCollision(
  void* body,
  int vertexCount,
  const float* faceVertec,
  int id)
{
	int i;
	core::vector3df line;

	i = vertexCount - 1;
	core::vector3df p0 (faceVertec[i * 3 + 0] * NewtonToIrr, faceVertec[i * 3 + 1] * NewtonToIrr, faceVertec[i * 3 + 2] * NewtonToIrr);

	for (i = 0; i < vertexCount; i ++) {
		core::vector3df p1 (faceVertec[i * 3 + 0] * NewtonToIrr, faceVertec[i * 3 + 1] * NewtonToIrr, faceVertec[i * 3 + 2] * NewtonToIrr);
		hidden_device->getVideoDriver()->draw3DLine (p0, p1,
			video::SColor(255, 255, 255, 0));
 		p0 = p1;
	}

}

void irr::newton::Hidden::NewtonDebugBody (const NewtonBody* body)
{
	core::matrix4 mat;
	video::SMaterial material;

	material.setTexture(0,NULL);
	material.Lighting = false;

	hidden_device->getVideoDriver()->setTransform(video::ETS_WORLD, mat);
	hidden_device->getVideoDriver()->setMaterial(material);

	// dray the collision in wire frame
	//NewtonBodyForEachPolygonDo(body, irr::newton::Hidden::NewtonDebugCollision);

	// draw all contact points
	/*
	for (int i = 0; i < m_contactCount; i ++) {
		driver->draw3DLine (m_contactNormals[i * 2], m_contactNormals[i * 2 + 1], video::SColor(255, 255, 0, 0));
	}
	*/
}

void irr::newton::Hidden::NewtonSerialize(
		void* serializeHandle,
		const void* buffer,
		size_t size) {
	fwrite (buffer, size, 1, (FILE*) serializeHandle);
}

void irr::newton::Hidden::NewtonDeserialize (
	void* serializeHandle,
	void* buffer,
	size_t size) {
	fread (buffer, size, 1, (FILE*) serializeHandle);
}

irr::newton::Hidden::ERROR_CODE irr::newton::Hidden::GetLastError() {
	irr::newton::Hidden::ERROR_CODE current_error=last_error;
	last_error=irr::newton::Hidden::NO_ERRORS;
	return current_error;
}
void irr::newton::Hidden::SetLastError(
	irr::newton::Hidden::ERROR_CODE error) {last_error=error;}

void irr::newton::Hidden::AddObjectToDestroy(
	irr::newton::IrrNewt_Base* obj) {

	objs_to_destroy.push_back(obj);
	objs_to_destroy.sort();
}

void irr::newton::Hidden::EraseFromDestroyList(irr::newton::IrrNewt_Base* obj) {
	int pos=objs_to_destroy.linear_search(obj);

	if(pos>=0)
		objs_to_destroy.erase(pos);
}

void irr::newton::Hidden::DestroyAll() {

	for(irr::u32 i=0;i<objs_to_destroy.size();i++) {

		if( objs_to_destroy[i] != NULL ) {

			objs_to_destroy[i]->reserved_destroy();
			if(objs_to_destroy[i]->call_delete())
				delete objs_to_destroy[i];

		}

	}

	objs_to_destroy.clear();

}

void irr::newton::Hidden::ClearObjsList() {

	for(irr::u32 i=0;i<objs_to_destroy.size();i++) {

		if(objs_to_destroy[i] != NULL) {

			objs_to_destroy[i]->reserved_destroy();
			//delete objs_to_destroy[i];
			//objs_to_destroy[i]=NULL;

		}
	}

	objs_to_destroy.clear();
}


void irr::newton::Hidden::DestroyAllOnExit() {

	atexit(irr::newton::Hidden::DestroyAll);
	objs_to_destroy.set_free_when_destroyed(false);

}

//-------------------------------------------------------------------

//chronometer functions
static irr::u32 last_time;
void irr::newton::Hidden::StartChronometer() {
	last_time=hidden_device->getTimer()->getRealTime();
}
irr::core::stringc irr::newton::Hidden::EndChronometer() {
	irr::u32 trascorred_time=hidden_device->getTimer()->getRealTime()-last_time;

	core::stringc time_string;
	time_string+=(int)trascorred_time;
	time_string+=" milliseconds equal to ";

	irr::f64 trascorred_time_d=(double)trascorred_time;
	trascorred_time_d/=1000;
	time_string+=trascorred_time_d;
	time_string+=" seconds";
	return time_string;
}

//-------------------------------------------------------------

int irr::newton::Hidden::NewtonMaterialCollisionContactBegin (
  const NewtonMaterial* material,
  const NewtonBody* body0,
  const NewtonBody* body1)
{
	irr::newton::IMaterialCollisionCallback* collision_callback=
		(irr::newton::IMaterialCollisionCallback*)NewtonMaterialGetMaterialPairUserData(material);

	irr::newton::IBody* body0_irrnewt=
		(irr::newton::IBody*)NewtonBodyGetUserData(body0);

	irr::newton::IBody* body1_irrnewt=
		(irr::newton::IBody*)NewtonBodyGetUserData(body1);

	irr::newton::IMaterialPair material_pair(const_cast<NewtonMaterial*>(material));

	return collision_callback->ContactBegin(
		&material_pair,
		body0_irrnewt,
		body1_irrnewt);

}

// this callback is called for every contact between the two bodies
int irr::newton::Hidden::NewtonMaterialCollisionContactProcess (
		const NewtonMaterial* material,
		const NewtonContactsProcess* contact)
{
	irr::newton::IMaterialCollisionCallback* collision_callback=
		(irr::newton::IMaterialCollisionCallback*)NewtonMaterialGetMaterialPairUserData (material);

	irr::newton::IMaterialPairAndContact material_pair_and_contact(
		const_cast<NewtonMaterial*>(material),
		const_cast<NewtonContactsProcess*>(contact)
		);

	return collision_callback->ContactProcess(
		&material_pair_and_contact);
}

// this function is call after all contacts for this pairs is processed
void  irr::newton::Hidden::NewtonMaterialCollisionContactEnd (
	const NewtonMaterial* material)
{
	irr::newton::IMaterialCollisionCallback* collision_callback=
		(irr::newton::IMaterialCollisionCallback*)NewtonMaterialGetMaterialPairUserData (material);

	irr::newton::IMaterialPair material_pair(const_cast<NewtonMaterial*>(material));

	collision_callback->ContactEnd(
		&material_pair);
	return;
}

irr::core::matrix4 irr::newton::Hidden::getIrrOffsetMatrix(
		irr::scene::ISceneNode* parent,
		irr::scene::ISceneNode* child)
		{

	child->updateAbsolutePosition();
	parent->updateAbsolutePosition();

	//get trans offset
	/*
	core::aabbox3d<f32> transf_box=child->getTransformedBoundingBox();
	core::vector3df child_trans_offset=transf_box.getCenter()-child->getAbsolutePosition();
	*/

	irr::core::matrix4 matrix;
	matrix.setTranslation(child->getPosition()-parent->getPosition());

	//matrix.setRotationDegrees(parent->getRotation()-child->getRotation());

	matrix.setRotationDegrees(core::vector3df(0,0,0));

	return matrix;
}


//----------------CALLBACKS---------------------------------------------
//---------BODY CALLBACKS----------------------
//------------------------------------------------------------------------

void irr::newton::Hidden::defBodyTransformCallback(const NewtonBody* body, const float* matrix, int shapeid) {

	//get irrlicht node and store current pos
	irr::newton::IBody* p_node  =(irr::newton::IBody*)NewtonBodyGetUserData(body);
	scene::ISceneNode* node     = p_node->getNode();

	//set node position
	//get matrix
	//realtime offset not implemented in newton
	core::matrix4 irr_mat;
	memcpy( GetMatrixPointer(irr_mat) , matrix, sizeof(float)*16);

	//newt_mat.setTranslation(newt_mat.getTranslation()+p_node->getRealtimeOffset().Position);
	//newt_mat.setRotationDegrees(newt_mat.getRotationDegrees()+p_node->getRealtimeOffset().Rotation);

    //if(irr_mat.getTranslation().X != irr_mat.getTranslation().X) { printf("QNAN\n"); return; }

	NewtonBodySetMatrix(body, matrix);

	//adjust translation to set with irrlicht
	irr_mat.setTranslation( irr_mat.getTranslation() * NewtonToIrr );
	core::vector3df new_pos = irr_mat.getTranslation();

	node->setPosition(new_pos);
	node->setRotation(irr_mat.getRotationDegrees());

    // set node rotation
	node->updateAbsolutePosition();
}


void irr::newton::Hidden::defBodyForceAndTorqueCallback(
														newton::IBody* body,
														const NewtonBody* n_body, int shapeid)  {

	//FORCE
	core::vector3df total_force = body->force + body->force_continuos;

    if(total_force.X != total_force.X) {

        body->newton_net_applied_force = core::vector3df(0,0,0);
        printf("qnan!!\n");
        return;
    }

	//reset force
	body->force = core::vector3df(0,0,0);

    if(total_force != irr::core::vector3df(0,0,0))
    {
        irr::f32 total_force_array[3];
        irr::newton::Hidden::Vector3dfFillArray(total_force, total_force_array);

        if( body->setting_force )
            NewtonBodySetForce(n_body, total_force_array);
        else
            NewtonBodyAddForce(n_body, total_force_array);
    }

	//TORQUE

    if(body->torque.X != body->torque.X) {
        body->newton_net_applied_force = core::vector3df(0,0,0);
        printf("qnan2!!\n");
        return;
    }


	if(body->torque != irr::core::vector3df(0,0,0))
	{
        float torque_array[3];
        irr::newton::Hidden::Vector3dfFillArray(body->torque, torque_array);
        NewtonBodyAddTorque(n_body,torque_array);
	}

    body->torque = core::vector3df(0,0,0);

	//get net applied force and torque
	float net_applied_force[3],net_applied_torque[3];
	NewtonBodyGetForce(n_body, net_applied_force);
	NewtonBodyGetTorque(n_body, net_applied_torque);

	//store net force and torque applied in the body
	body->newton_net_applied_force  = irr::newton::Hidden::Vector3dfFromArray(net_applied_force);
	body->newton_net_applied_torque = irr::newton::Hidden::Vector3dfFromArray(net_applied_torque);

	return;

}

void irr::newton::Hidden::defBodyForceAndTorqueCallback (const NewtonBody* n_body, float a, int b)
{

	irr::newton::IBody* body = (irr::newton::IBody*)NewtonBodyGetUserData(n_body);
	irr::newton::Hidden::defBodyForceAndTorqueCallback(body, n_body, b);

	return;
}

//---------END OF BODY CALLBACKS----------------------

//camera callbacks
void irr::newton::Hidden::defCameraBodyTransformCallback(const NewtonBody* body, const float* matrix, int shapeid) {
	irr::newton::Hidden::defBodyTransformCallback(body,matrix, shapeid);
	irr::newton::Hidden::correctCameraTarget(
		(scene::ICameraSceneNode*)((irr::newton::IBody*)NewtonBodyGetUserData(body))->getNode());
}

void irr::newton::Hidden::defCameraCharacterControllerTransformCallback(const NewtonBody* body, const float* matrix, int shapeid) {

	irr::newton::Hidden::defCharacterControllerTransformCallback(body,matrix, shapeid);

	scene::ICameraSceneNode* camera = (scene::ICameraSceneNode*)((irr::newton::IBody*)NewtonBodyGetUserData(body))->getNode();
	irr::newton::Hidden::correctCameraTarget(camera);
}

//character control callbacks
void irr::newton::Hidden::defCharacterControllerForceAndTorqueCallback (
								const NewtonBody* body, float something, int shapeid) {

	//core::vector3df velocity=p_node->getVelocity();
	//core::vector3df last_force_continuos=p_node->force_continuos;

	//INTERPOLATE VELOCITY WITH FORCE

	/*
	if(p_node->apply_force_if_not_air_borne) {

	core::vector3df last_force_continuous=p_node->force_continuos;
	p_node->force_continuos=core::vector3df();

	core::aabbox3d<f32> box=p_node->node->getTransformedBoundingBox();
	core::line3d<f32> line;
	line.start=box.MinEdge;

	core::vector3df force_continuous_for_line=last_force_continuous*99999;
	line.end=(line.start+force_continuous_for_line);

	irr::newton::IntersectionPoint p=
		p_node->world->getCollisionManager()->getCollisionFirstPoint(line);

	//debug info
	core::stringc message;
	message+="line start ";
	message+=(double)line.start.X;
	message+=",";
	message+=(double)line.start.Y;
	message+=",";
	message+=(double)line.start.Z;
	message+="  line.end";

	message+=(double)line.end.X;
	message+=",";
	message+=(double)line.end.Y;
	message+=",";
	message+=(double)line.end.Z;
	message+="  point";
	//end of debug info

	if(p.body!=NULL) {

	//debug infos
	message+=(double)p.point.X;
	message+=",";
	message+=(double)p.point.Y;
	message+=",";
	message+=(double)p.point.Z;
	//end of debug infos

	irr::f32 distance=fabs(box.MinEdge.getDistanceFrom(p.point));

	//a 29.40 dà ancora sgnali, a 29.45 non più
	//if(distance>32.0f) {

	if(distance>29.41f) {

		irr::f32 mass=p_node->getMass();
		irr::f32 deltaHeight=distance;

		dFloat timestep;
		dFloat timestepInv;

		// Get the current world timestep
		timestep = NewtonGetTimeStep(p_node->world->getNewtonWorld());
		timestepInv = 1.0f / timestep;

		// snap to floor only if the floor is lower than the character feet


		//irr::f32 accelY = - ((deltaHeight * timestepInv + velocity.Y) /10);
		//p_node->force_continuos.Y = mass * accelY;


		NewtonBodyAddForce(p_node->body,&last_force_continuous.X);
		return;

		//p_node->force_continuos=last_force_continuous;

			//debug infos
		message+=" force continuous: ";
		message+=(double)p_node->force_continuos.X;
		message+=",";
		message+=(double)p_node->force_continuos.Y;
		message+=",";
		message+=(double)p_node->force_continuos.Z;
	//end of debug infos

		hidden_device->getLogger()->log(message.c_str());
	}
	}//if(p.body!=NULL) {

	irr::newton::Hidden::defBodyForceAndTorqueCallback(body);
	p_node->force_continuos=last_force_continuous;

	}//if(p_node->apply_force_if_not_air_borne)
	*/
	//END OF INTERPOLATION

	//p_node->force_continuos=last_force_continuos;


	irr::newton::ICharacterController* p_node=(irr::newton::ICharacterController*)NewtonBodyGetUserData(body);

	irr::newton::Hidden::defBodyForceAndTorqueCallback(body, something, shapeid);

	//end of force and torque default

	//center the mouse
	if(p_node->center_mouse)
		hidden_device->getCursorControl()->setPosition(0.5f,0.5f);

	//for setting velocity
	//CAN SET VELOCITY
	if(p_node->can_set_velocity!=0){

		float plus=
			fabs(p_node->position_difference.X)+
			fabs(p_node->position_difference.Y)+
			fabs(p_node->position_difference.Z);

		const float minimum_value=+0.00001f;

		if(plus<=minimum_value) {

				switch(p_node->can_set_velocity) {
					case (1): p_node->can_set_velocity=2;break;
					case (2): p_node->can_set_velocity=3;break;
					case (3): p_node->can_set_velocity=4;break;
					case (4): p_node->can_set_velocity=5;break;
					case (5): p_node->can_set_velocity=6;break;
					case (6): p_node->can_set_velocity=7;break;
					case (7): p_node->can_set_velocity=8;break;
					case (8): p_node->can_set_velocity=0;break;
				}//switch

				//can set force

				switch(p_node->can_set_force) {
					case(0): break;
					case (1): p_node->can_set_force=2;break;
					case (2): p_node->can_set_force=3;break;
					case (3): p_node->can_set_force=4;break;
					case (4): p_node->can_set_force=5;break;
					case (5): p_node->can_set_force=6;break;
					case (6): p_node->can_set_force=7;break;
					case (7): p_node->can_set_force=8;break;
					case (8): p_node->can_set_force=0;break;
				}//switch

		}//if(plus<0.00001f&&p_node->can_set_velocity!=ESV_CAN) {

		//restore if move
		else if(plus>minimum_value) {
			p_node->can_set_velocity=1;

			//force
			if(p_node->can_set_force!=0)
				p_node->can_set_force=1;
		}//else if(plus>minimum_value) {

	}//	if(p_node->can_set_velocity!=0){

	p_node->position_difference=core::vector3df(0,0,0);


	//----------------------------------------------------------------------------
	//-------------STAIR CLIMBING-------------------------------------------------
	//----------------------------------------------------------------------------


	core::vector3df body_pos = p_node->getNode()->getPosition();
	body_pos.Y = 0;

	//avoid penetration
	p_node->getWorld()->getUtils()->round(body_pos.X,1);
	p_node->getWorld()->getUtils()->round(body_pos.Z,1);

	//CHECK IF WE CAN DO THAT
	static core::vector3df last_direction = core::vector3df();
	static core::vector3df last_body_position = core::vector3df();
	bool do_stair_climbing = true;

	//get net force
	irr::core::vector3df net_force = p_node->getNetAppliedForce();
	p_node->getWorld()->getUtils()->round(net_force.X,1);
	p_node->getWorld()->getUtils()->round(net_force.Z,1);
	//is the body moving?
    bool isMoving = (net_force.X != 0) || (net_force.Z != 0);

	//check if last direction
	/*
	if( do_stair_climbing && last_direction != (p_node->getLastDirectionParameter()+body_pos) )
		last_direction = p_node->getLastDirectionParameter()+body_pos;
	else do_stair_climbing = false;
	*/

	//END OF CHECKING

    //don't climb stair if moving, if clim stair force is 0, is max_stair_height is negative
	if(
    p_node->getClimbStairForce() != core::vector3df()
	/*&&  do_stair_climbing      */
    //isMoving &&
    //p_node->stair_max_height >= 0
    ) {


	//---------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------------

	core::vector3df point_body;

	//calculate AABB
	core::aabbox3d<f32> box = p_node->calculateAABB();
	core::vector3df minEdge = p_node->getPosition();

	core::vector3df lastDirectionParameter=p_node->getLastDirectionParameter();
	lastDirectionParameter.normalize();
	lastDirectionParameter*=50;


	//check ray cast point in body
	core::line3d<f32> line_trought_body;
	line_trought_body.start = line_trought_body.end = minEdge;
	line_trought_body.start += lastDirectionParameter;
	line_trought_body.end   -= lastDirectionParameter;

	newton::SIntersectionPoint i_point_body;
	p_node->getWorld()->getCollisionManager()->getCollisionPoint(
		p_node,
		line_trought_body,
		i_point_body);

	//reduce point position to avoid penetration
	//WORK CORECTLY, BUT IS THIS NECESARY?
	line_trought_body.start *= IrrToNewton;
	line_trought_body.end *= IrrToNewton;
	i_point_body.parametric_value += 0.05f;
    i_point_body.point = line_trought_body.start + i_point_body.parametric_value * (line_trought_body.end - line_trought_body.start);
	i_point_body.point *= NewtonToIrr;


	//set point and set y position as min edge
	point_body = i_point_body.point;
	point_body.Y = box.MinEdge.Y-5;


	//DEBUG
	/*
	video::IVideoDriver* driver = p_node->getWorld()->getIrrlichtDevice()->getVideoDriver();
	scene::ISceneNode* bill=p_node->getWorld()->getIrrlichtDevice()->getSceneManager()->addBillboardSceneNode();
	bill->setPosition(point_body);
	bill->setMaterialTexture(0,driver->getTexture("D:\\raffaele\\programmi\\phisic\\media\\wheel_texture.jpg"));
	bill->setMaterialFlag(video::EMF_LIGHTING,false);
	bill=p_node->getWorld()->getIrrlichtDevice()->getSceneManager()->addBillboardSceneNode();
	bill->setPosition(line_trought_body.start);
	bill->setMaterialFlag(video::EMF_LIGHTING,false);
	bill=p_node->getWorld()->getIrrlichtDevice()->getSceneManager()->addBillboardSceneNode();
	bill->setPosition(line_trought_body.end);
	bill->setMaterialFlag(video::EMF_LIGHTING,false);
	*/
	//END OF DEBUG

	//stair climbing
	//make direction
	lastDirectionParameter.normalize();
	lastDirectionParameter *= 13;

	//build direction
	core::line3d<f32> direction;
	direction.start = direction.end = point_body;
	direction.end += lastDirectionParameter;

	newton::SIntersectionPoint raycast_1=
		p_node->getWorld()->getCollisionManager()->getCollisionFirstPointEx(direction);

	if(raycast_1.body!=NULL) {

		//collide!! is a stair? or a wall?
		//distance
		irr::f32 distance_point1_body=raycast_1.point.getDistanceFrom(direction.start);

		//up the direction
		direction.start.Y+=p_node->stair_max_height;
		direction.end.Y+=p_node->stair_max_height;

		//second raycast
		newton::SIntersectionPoint raycast_2=
			p_node->getWorld()->getCollisionManager()->getCollisionFirstPointEx(direction);
		irr::f32 distance_point2_body=raycast_2.point.getDistanceFrom(direction.start);

		//conditional
		if(raycast_2.body==NULL||
			((raycast_2.body!=NULL)&&(distance_point1_body<distance_point2_body))
			) {

			//There is a stair!!! Make the body climb it!!!
			core::vector3df vel = p_node->getVelocity();
			//NEW CODE STAIR CLIMB
			//if(vel.Y < p_node->getClimbStairForce().Y)
				vel.Y += p_node->getClimbStairForce().Y;
			p_node->setVelocity(vel);

		}//if(raycast_2.body==NULL) {
	}//if(raycast_1.body!=NULL) {


	}//if gravity force != 0,0,0


	/*
	//DON'T ERASE. SET GRAVITY ONLY IN CERTAIN CIRCUMSTANCE

	if(p_node->getGravity()!=core::vector3df()) {

	core::aabbox3d<f32> box=p_node->calculateAABB();
	core::line3d<f32> line(box.MinEdge,box.MinEdge);
	line.end.Y-=99999.9f;

	newton::SIntersectionPoint i_point;
	i_point=p_node->getWorld()->getCollisionManager()->getCollisionFirstPointEx(line);

	if(i_point.body!=NULL) {

		irr::f32 distance = fabs(line.start.getDistanceFrom(i_point.point));

		if(distance >= p_node->stair_max_height) {
			p_node->addForce(p_node->getGravity());
		}
	}

	}//if gravity!=0,0,0
	*/


	//---------------------------------------------------------------
	//--------CAMERA FPS SPECIFIC------------------------------------
	//---------------------------------------------------------------

	if(p_node->class_type.Type==ECT_CAMERA_FPS_BODY) {

		newton::ICameraBodyFPS* cam_body=(newton::ICameraBodyFPS*)p_node;

		//move and rotate
		cam_body->getWorld()->getUtils()->updateCameraFPS(
			cam_body,
			cam_body->mouseSensitive,
			cam_body->moveSpeed,

			cam_body->isPaused?newton::SDirection():newton::SDirection(cam_body->up,cam_body->down,cam_body->left,cam_body->right),

			cam_body->verticalMovement,
			cam_body->onlyY,
			cam_body->isPaused?false:true   //stop if not moving. don't stop is it is in paused, else stop

		);

		//to unpause if time is elapsed
		cam_body->updatePause();

	}//camera fps
}

void irr::newton::Hidden::defCharacterControllerTransformCallback(const NewtonBody* body, const float* matrix, int shapeid) {

	//get irrlicht node
	irr::newton::ICharacterController* p_node=(irr::newton::ICharacterController*)NewtonBodyGetUserData(body);
	scene::ISceneNode* node=p_node->getNode();
	core::vector3df last_pos=node->getPosition();

	//set body position
	NewtonBodySetMatrix(body,matrix);

	//set node position
	//get matrix
	core::matrix4 irr_mat;
	memcpy( GetMatrixPointer(irr_mat) , matrix, sizeof(float)*16);

	//adjust translation to set with irrlicht
	irr_mat.setTranslation(irr_mat.getTranslation()*NewtonToIrr);
	core::vector3df new_pos=irr_mat.getTranslation();

	node->setPosition(irr_mat.getTranslation());

	if(p_node->getRotationUpdate())
		node->setRotation(irr_mat.getRotationDegrees());

	// set node rotation

	node->updateAbsolutePosition();

	p_node->position_difference=new_pos-last_pos;
}

void irr::newton::Hidden::defBodyLeaveWorldEvent(const NewtonBody* n_body, int shapeid) {
	irr::newton::IBody* body=(irr::newton::IBody*)NewtonBodyGetUserData(n_body);
	irr::newton::IWorld* world=body->getWorld();
	world->body_leave_world_event_callback(body);
}


//VEHICLE CALLBACKS

void irr::newton::Hidden::VehicleTireCallBack(const NewtonJoint* vehicle_joint) {
	irr::newton::IVehicleSimple* vehicle=
		(irr::newton::IVehicleSimple*)NewtonJointGetUserData(vehicle_joint);

	irr::core::array<irr::newton::IVehicleTire*> tires=vehicle->getAllTires();

	if(tires.empty()==true) return;

	for(irr::u32 i=0;i<tires.size();i++) {

		irr::f32 tire_torque = tires[i]->getTorque();
		irr::f32 tire_steer_angle = tires[i]->getSteerAngle();

		irr::f32 omega = NewtonHingeGetJointOmega(vehicle_joint);

		/*
		if(tire_torque!=0)
			tire_torque=fabs((tire_torque-30)) * fabs(omega);
			/*/

		//SIDE SLEEP
		float c_speed=0; //NewtonVehicleGetTireLongitudinalSpeed(vehicle_joint,tires[i]->tire_id);

		//NewtonVehicleSetTireMaxSideSleepSpeed (vehicle_joint, tires[i]->tire_id, c_speed *  1.0f);
		// The side slip is propostinal to the tire longitudilnal speed
		//NewtonVehicleSetTireSideSleepCoeficient (vehicle_joint, tires[i]->tire_id, c_speed * 0.4f);

		//END OF SIDE SLEEP

		//set torque
		if(tire_torque!=0) {

		/*NewtonVehicleSetTireTorque(
			vehicle_joint,
			const_cast<void*>(tires[i]->getNewtonTireId()),
			 tire_torque
			);*/
		}

		//set steer angle
		/*NewtonVehicleSetTireSteerAngle(
			vehicle_joint,
			const_cast<void*>(tires[i]->getNewtonTireId()),
			tire_steer_angle
			);*/

		//brakes
		irr::f32 total_brake_acceleration = tires[i]->brake_acceleration;

		//if(total_brake_acceleration>0) {

		//if(tires[i]->auto_calculate_max_brake_acceleration)
			//total_brake_acceleration += (irr::f32)NewtonVehicleTireCalculateMaxBrakeAcceleration(tires[i]->n_vehicle,tires[i]->tire_id);

		/*NewtonVehicleTireSetBrakeAcceleration(
			tires[i]->n_vehicle,
			tires[i]->tire_id,
			total_brake_acceleration,
			tires[i]->brake_max_friction_torque);*/

		// set some side slipe as funtion of the linear speed
		c_speed = 0; //NewtonVehicleGetTireLongitudinalSpeed (vehicle_joint, tires[i]->tire_id);
		//NewtonVehicleSetTireMaxSideSleepSpeed (vehicle_joint, tires[i]->tire_id, fabs(c_speed) *  0.4f);

		tires[i]->brake_max_friction_torque = 0.0f;

		//}

		//is air borne
		//warning if I cast an int into bool

		/*switch(NewtonVehicleTireIsAirBorne(
			tires[i]->n_vehicle,
			tires[i]->tire_id)) {

		case 1:

			tires[i]->is_air_borne = true;
			//forgot it. fix since 0.4
			break;

		default:

			tires[i]->is_air_borne = false;
			//forgot it. fix since 0.4
			break;

		}*/

		//upload node position
		irr::newton::SPosRot p=tires[i]->getPositionRotation();
		irr::newton::SPosRot offset=tires[i]->body_node_offset;
		p.add(tires[i]->body_node_offset);
		scene::ISceneNode* node=const_cast<scene::ISceneNode*>(tires[i]->getIrrlichtNode());

		if(node) {
			node->setPosition(p.Position);
			node->setRotation(p.Rotation);
			node->updateAbsolutePosition();
		}

	}//for

}//functions

//
//const float InvPi = 0.0174532f; /// PI / 180


//---------------------------------JOINTS--------------------------------------------------

//hinge
irr::u32 irr::newton::Hidden::defJointHingeCallback(const NewtonJoint* n_hinge, NewtonHingeSliderUpdateDesc* desc) {
	irr::newton::IJointHinge* p_hinge=(irr::newton::IJointHinge*)NewtonJointGetUserData(n_hinge);

	p_hinge->n_joint_angle=NewtonHingeGetJointAngle(n_hinge);
	p_hinge->n_joint_omega=NewtonHingeGetJointOmega(n_hinge);

	irr::f32 force_array[3];
	NewtonHingeGetJointForce(n_hinge,force_array);
	p_hinge->n_joint_force=irr::newton::Hidden::Vector3dfFromArray(force_array);

	return 1;

}

irr::u32 irr::newton::Hidden::defJointSliderCallback(const NewtonJoint* n_slider, NewtonHingeSliderUpdateDesc* desc) {

	irr::newton::IJointSlider* p_slider=(irr::newton::IJointSlider*)NewtonJointGetUserData(n_slider);

	p_slider->n_joint_posit=NewtonSliderGetJointPosit(n_slider);
	p_slider->n_joint_veloc=NewtonSliderGetJointVeloc(n_slider);

	irr::f32 force[3];
	NewtonSliderGetJointForce(n_slider,force);
	p_slider->n_joint_force=irr::newton::Hidden::Vector3dfFromArray(force);


	return 1;
}

