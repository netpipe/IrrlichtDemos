// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

namespace irr{
namespace newton {
	bool getLogStatus();
}
}

#include "../include/world.hpp"
#include "../include/irrtonewt.hpp"
//#include "../include/body.hpp"
#include "../include/camera_fps.hpp"
#include "../include/tree_body.hpp"
#include "../include/intersection_point.hpp"
#include "../include/hidden.hpp"
#include "../include/material.hpp"
#include "../include/body_controller.hpp"
//#include "../include/vehicle_simple.hpp"
#include "../include/ragdoll.hpp"

#include "../include/vehicle_car.hpp"
//#include "../include/vehicle_tire.hpp"
//#include "../include/vehicle_simple.hpp"

#include "../include/structures.hpp"
#include "../include/utils.hpp"

//joints
//#include "../include/joint.hpp"
#include "../include/joint_universal.hpp"
#include "../include/joint_upvector.hpp"
#include "../include/joint_hinge.hpp"
#include "../include/joint_corkscrew.hpp"
#include "../include/joint_slider.hpp"
#include "../include/joint_ball.hpp"

using namespace irr;

namespace irr{
	namespace newton{

		IRRNEWT_API irr::newton::IWorld* createPhysicsWorld(irr::IrrlichtDevice* device);
	}
}


//IRRNEWT BASE
irr::newton::IrrNewt_Base::IrrNewt_Base() {}

//WORLD CLASS
irr::newton::IWorld::IWorld(irr::IrrlichtDevice* Device) {

	this->class_type.Type = ECT_WORLD;
	this->world = NewtonCreate();
	this->group_material_ID = NewtonMaterialGetDefaultGroupID(this->world);
	this->device = Device;
	this->time_elapsed = 0.0f;
	this->accumulated_time = 0.0f;


	this->utils.world = this;
	this->collision_manager.n_world = this->world;


	//set newton world user data to this class
	NewtonWorldSetUserData(this->world, this);

	//call newton update 30 times per seconds for default
	this->update_fps = 30.0f;

	irr::newton::Hidden::SetDevice(this->device);

	//to avoid problems expecially for beginners we set a big world size
	const irr::f32 world_size_value=9999999;
	this->setWorldSize(core::aabbox3d<f32>(
		core::vector3df(-world_size_value,-world_size_value,-world_size_value),
		core::vector3df(world_size_value,world_size_value,world_size_value)));
	//end of set world size

	core::stringc log_text = "IrrNewt version ";
	log_text += IRRNEWT_VER;
	device->getLogger()->log(log_text.c_str());

	if(newton::getLogStatus())
		device->getLogger()->log("IrrNewt: physics world created");
}

irr::newton::IWorld::~IWorld() {

	if(this->world != NULL) {

		NewtonDestroyAllBodies(this->world);
		NewtonMaterialDestroyAllGroupID(this->world);
		NewtonDestroy(this->world);
		this->world = NULL;

	}//	if(this->world != NULL) {

}

void irr::newton::IWorld::reserved_destroy() {
	this->~IWorld();
}


void irr::newton::IWorld::destroyWorld() {

	//clear array (and call indirectly IWorld destrutor wich detroy body, materials and world)
	irr::newton::Hidden::ClearObjsList();

	//set pointer to NULL
	this->world = NULL;
	this->group_material_ID = -1;
}

void irr::newton::IWorld::update(irr::f32 time_step) {
	NewtonUpdate(this->world,time_step);
}


//#define PHYSICS_UPDATE_FPS         300.0f
//#define PHYSICS_THREAD_SLEEPTIME   (1000.0f / PHYSICS_UPDATE_FPS)

void irr::newton::IWorld::update() {

	//time releated
	static bool first_time=true;
	static irr::f32 last_time=0;

	if(first_time) {
		first_time=false;
		last_time=(irr::f32)this->device->getTimer()->getRealTime();
		return;
	}

	this->time_elapsed=((irr::f32)this->device->getTimer()->getRealTime())-last_time;
	irr::f32 milliseconds=this->time_elapsed;

	this->time_elapsed/=1000.0f; //convert to seconds

	last_time=this->device->getTimer()->getRealTime();
	//------------------------------------------------------------------
	//-------------end----------------------------------------------
	//----------------------------------------------------------------


	//update
	//for too slow machine fix
	if (milliseconds>100.0f) milliseconds=100.0f;

	irr::f32 newton_time_step = (1000.0f /(irr::f32)this->update_fps);

	 // add the number of millisconds passed to our accumlative total
	this->accumulated_time += milliseconds;

	while (this->accumulated_time > newton_time_step)
   {
      NewtonUpdate(this->world, (newton_time_step*1.0e-3f));   // convert to seconds
      this->accumulated_time -= newton_time_step;
   }


	//if(time_elapsed_ms>50.0f) time_elapsed_ms=50.0f;

	/*
     this->accumulated_time += time_elapsed_ms;
	 irr::f32 newton_time_step =

     frame_time = this->device->getTimer()->getRealTime();

     while(accumulated_time >= newton_time_step )
     {
         NewtonUpdate( this->world, (newton_time_step * 0.001f));
         accumulated_time -= newton_time_step;
     }
	 */


	//NewtonUpdate(this->world,this->time_elapsed);
	//NewtonUpdate(this->world,1.0f/this->time_elapsed);

			/*
	float fps = (float) device->getVideoDriver()->getFPS();
	if (fps > 0.0f) {
		NewtonUpdate(this->world, fps);
	}
	//else
		//NewtonUpdate(this->world, 0.01f);
		*/

	//NewtonUpdate(this->world, this->timer.GetElapsedSeconds());

	//NewtonUpdate(this->world, 100.0f);

	//NewtonUpdate(this->world, 0.01f);
}

irr::newton::IBody* irr::newton::IWorld::createBodyAuto(
	irr::scene::ISceneNode* node,
	irr::scene::IMesh* mesh/*=NULL*/,
	irr::f32 mass,irr::u32 terrain_lod,
	irr::core::matrix4 body_offset_from_scene_node,
	core::vector3df scale_offset,
	E_CALCULATE_OFFSET autoCalcOffset,
	E_CALCULATE_SIZE calcSizeMode) {


	newton::SBodyFromNode data;

	data.Node = node;
	data.Mesh = mesh;
	data.Type = newton::EBT_AUTODETECT;
	data.Mass = mass;
	data.BodyOffsetFromNode = body_offset_from_scene_node;
	data.BodyScaleOffsetFromNode = scale_offset;
	data.CalculateOffsetMode = autoCalcOffset;
	data.CalculateSizeMode = calcSizeMode;

    return this->createBody(data);

}

irr::core::array<irr::newton::IBody*> irr::newton::IWorld::createBodies(
	irr::core::array<irr::scene::ISceneNode*> nodes,
	irr::newton::SBodyFromNode data) {

	irr::core::array<irr::newton::IBody*> bodies;

	for(irr::u32 i=0;i<nodes.size();i++) {
		data.Node=nodes[i];
		bodies.push_back(this->createBody(data));
	}
	return bodies;
}


irr::newton::ICollision* irr::newton::IWorld::createCollision(irr::newton::SBodyFromNode data)
{
	irr::newton::ICollision* p_node = new irr::newton::ICollision(this);
	p_node->create(data);

	//register it for destroying at exit
	irr::newton::Hidden::AddObjectToDestroy(p_node);

	//this->scene_nodes.push_back(p_node);
	return p_node;
}

irr::newton::ICollision* irr::newton::IWorld::createCollision( core::array<irr::newton::ICollision*> collisions) {

	irr::newton::ICollision* p_node = new irr::newton::ICollision(this);
	p_node->create(collisions);

	//register it for destroying at exit
	irr::newton::Hidden::AddObjectToDestroy(p_node);

	//this->scene_nodes.push_back(p_node);
	return p_node;

}

irr::newton::ICollision* irr::newton::IWorld::createCollision(NewtonCollision* nCollision) {

	irr::newton::ICollision* p_node = new irr::newton::ICollision(this);
	p_node->n_collision = nCollision;

	//register it for destroying at exit
	irr::newton::Hidden::AddObjectToDestroy(p_node);

	//this->scene_nodes.push_back(p_node);
	return p_node;

}

irr::newton::IBody*  irr::newton::IWorld::createBody(
	scene::ISceneNode* node,
	newton::ICollision* collision) {

	newton::E_BODY_TYPE type = collision->getCreationData().Type;

	irr::newton::IBody* body = NULL;

	//isn't not a tree collision
	if( type != newton::EBT_TREE &&
		type != newton::EBT_TREE_TERRAIN) {
		body = new irr::newton::IBody(
			this, node, collision, 1 );
	}


	//is a tree collision
	else{

		body = new irr::newton::ITreeBody(
			this, node, collision);
	}

	//register it for destroying at exit
	irr::newton::Hidden::AddObjectToDestroy(body);

	return body;
}

irr::newton::IBody*  irr::newton::IWorld::createBody(SBodyFromNode bodyData) {
	//create the collision of the body
	irr::newton::ICollision*  collision = this->createCollision(bodyData);

	//create the body from collision
	irr::newton::IBody* body = this->createBody(bodyData.Node, collision);

	//set mass
	body->setMass(bodyData.Mass);

	//calculate tensor of inertia
	body->calculateMomentOfInertia();

	//release collision object
	collision->release();

	return body;
}


irr::newton::IBody*  irr::newton::IWorld::createBody(

				irr::scene::ISceneNode* node,
				irr::scene::IMesh* mesh,
				E_BODY_TYPE bodyType,
				irr::f32 mass,
				irr::core::matrix4 body_offset_from_scene_node,
				core::vector3df body_scale_offset,
				E_CALCULATE_OFFSET autoCalcOffset,
				E_CALCULATE_SIZE calcSizeMode

				) {

	newton::SBodyFromNode data;

	data.Node = node;
	data.Mesh = mesh;
	data.Type = bodyType;
	data.Mass = mass;
	data.BodyOffsetFromNode = body_offset_from_scene_node;
	data.BodyScaleOffsetFromNode = body_scale_offset;
	data.CalculateOffsetMode = autoCalcOffset;
	data.CalculateSizeMode = calcSizeMode;

	return this->createBody(data);

}



irr::newton::ITreeBody*  irr::newton::IWorld::createTreeBody(
														   irr::scene::ISceneNode* node,
														   irr::scene::IMesh* iMeshBase){

	SBodyFromNode data;
	data.Type = EBT_TREE;
	data.Node = node;
	data.Mesh = iMeshBase;

	irr::newton::ITreeBody* oct_node = (irr::newton::ITreeBody*)this->createBody(data);

	return oct_node;
}

void irr::newton::IWorld::setBodyLeaveWorldEvent( void (*new_leave_world_event_callback) (irr::newton::IBody* body))
{
	this->body_leave_world_event_callback=new_leave_world_event_callback;
	NewtonSetBodyLeaveWorldEvent(this->world,irr::newton::Hidden::defBodyLeaveWorldEvent);
}

irr::newton::ITreeBody*  irr::newton::IWorld::createTerrainTreeBody(
			 irr::scene::ITerrainSceneNode* node,
			irr::s32 LOD) {

	newton::SBodyFromNode data;

	data.Node = node;
	data.TerrainLOD = LOD;
	data.Type = newton::EBT_TREE_TERRAIN;

	return (irr::newton::ITreeBody*)this->createBody(data);
}

void irr::newton::IWorld::drawAllDebugInfos() {
	//NewtonWorldForEachBodyInAABBDo
	//NewtonWorldForEachBodyDo (this->world, irr::newton::Hidden::NewtonDebugBody);
}

irr::newton::ICharacterController* irr::newton::IWorld::createCharacterController(
				irr::newton::IBody* body,
				bool erase/*=true*/) {

	if( body == NULL) return NULL;

	irr::newton::ICharacterController* c = new irr::newton::ICharacterController(
		(irr::newton::ICharacterController*)body);
	c->setRotationUpdate(true);

	if(erase) {
		delete body;
	}

	irr::newton::Hidden::EraseFromDestroyList(body);
	irr::newton::Hidden::AddObjectToDestroy(c);

	return c;
}

irr::newton::IMaterial* irr::newton::IWorld::createMaterial() {
	irr::newton::IMaterial* material=new irr::newton::IMaterial(this->world);

	//register for destroy at exit
	irr::newton::Hidden::AddObjectToDestroy(material);

	return material;
}

irr::newton::ITreeBody* irr::newton::IWorld::deserializeTreeBody(
		irr::scene::ISceneNode* node,
		irr::core::stringc file
		) {

	irr::newton::ITreeBody* oct_node = new irr::newton::ITreeBody(
		this,
		node,
		file   //file for deserialization
		);

	//register it for destroying at exit
	irr::newton::Hidden::AddObjectToDestroy(oct_node);

	if(irr::newton::Hidden::GetLastError() != irr::newton::Hidden::NO_ERRORS)
		oct_node = NULL;

	return oct_node;
}

irr::newton::IVehicleSimple* irr::newton::IWorld::createVehicleSimple(
			irr::newton::IBody* chassis,
			irr::core::vector3df up_dir) {

	irr::newton::IVehicleSimple* v = new irr::newton::IVehicleSimple(
		this,
		chassis,
		up_dir
		);
	irr::newton::Hidden::AddObjectToDestroy(v);
	return v;
}

irr::newton::ICar* irr::newton::IWorld::createCar(irr::newton::SCar car) {

	irr::newton::ICar* v=new irr::newton::ICar(
		this,
		car
		);
	irr::newton::Hidden::AddObjectToDestroy(v);
	return v;
}

//create camera fps
irr::newton::ICameraBodyFPS* irr::newton::IWorld::createCameraFPS(
																  irr::newton::SBodyFromNode data
																  ) {

	irr::newton::ICharacterController* base=this->createCharacterController(this->createBody(data));
	irr::newton::ICameraBodyFPS* v=new irr::newton::ICameraBodyFPS(
		base
		);

	irr::newton::Hidden::AddObjectToDestroy(v);
	return v;
}



//-------------------------------------------------------------------------------

irr::newton::ICollisionManager* irr::newton::IWorld::getCollisionManager() {
	return &this->collision_manager;
}

irr::newton::IUtils* irr::newton::IWorld::getUtils() {
	return &this->utils;
}

irr::newton::IInternalBehaviour* irr::newton::IWorld::getInternalBehaviour() {
	return &this->behaviour;
}

//--------------------------------------------------------------------------


void irr::newton::IWorld::setWorldSize(irr::core::aabbox3d<irr::f32> size) {
	size.MaxEdge*=IrrToNewton;
	size.MinEdge*=IrrToNewton;

	float min[3],max[3];
	irr::newton::Hidden::Vector3dfFillArray(size.MaxEdge,max);
	irr::newton::Hidden::Vector3dfFillArray(size.MinEdge,min);

	NewtonSetWorldSize(
		this->world,
		min,max);

}


irr::newton::IJointUpVector* irr::newton::IWorld::createJoint(
	irr::newton::SJointUpVector up_vector
	) {

	irr::newton::IJointUpVector* v=new irr::newton::IJointUpVector(
		this,
		up_vector
		);

	irr::newton::Hidden::AddObjectToDestroy(v);

	return v;

}

irr::newton::IJointHinge* irr::newton::IWorld::createJoint(irr::newton::SJointHinge jointData) {

	irr::newton::IJointHinge* v=new irr::newton::IJointHinge(
		this,
		jointData
		);

	irr::newton::Hidden::AddObjectToDestroy(v);

	return v;
}

irr::newton::IJointBall* irr::newton::IWorld::createJoint(irr::newton::SJointBall jointData) {

	irr::newton::IJointBall* v=new irr::newton::IJointBall(
		this,
		jointData
		);

	irr::newton::Hidden::AddObjectToDestroy(v);

	return v;
}

irr::newton::IJointUniversal* irr::newton::IWorld::createJoint(irr::newton::SJointUniversal jointData) {

	irr::newton::IJointUniversal* v=new irr::newton::IJointUniversal(
		this,
		jointData
		);

	irr::newton::Hidden::AddObjectToDestroy(v);

	return v;
}

irr::newton::IJointCorkscrew* irr::newton::IWorld::createJoint(irr::newton::SJointCorkscrew jointData) {
	irr::newton::IJointCorkscrew* v=new irr::newton::IJointCorkscrew(
		this,
		jointData
		);

	irr::newton::Hidden::AddObjectToDestroy(v);

	return v;
}

irr::newton::IJointSlider* irr::newton::IWorld::createJoint(irr::newton::SJointSlider jointData) {
	irr::newton::IJointSlider* v=new irr::newton::IJointSlider(
		this,
		jointData
		);

	irr::newton::Hidden::AddObjectToDestroy(v);

	return v;
}



irr::newton::IJoint* irr::newton::IWorld::createJoint(irr::newton::SGenericJoint c) {

	//UP VECTOR
	if(c.Type == newton::EJT_UPVECTOR) {

		newton::SJointUpVector h;

		h.ParentBody = c.ParentBody;
		h.PinDir     = c.Pin0;

		return this->createJoint(h);

	}

	//HINGE
	if(c.Type == newton::EJT_HINGE) {

		newton::SJointHinge h;

		h.ChildBody = c.ChildBody;
		h.ParentBody = c.ParentBody;
		h.Pivot = c.Pivot;
		h.PinDir = c.Pin0;

		return this->createJoint(h);

	}

	//UNIVERSAL
	if(c.Type == newton::EJT_UNIVERSAL) {

		newton::SJointUniversal h;

		h.ParentBody = c.ParentBody;
		h.ChildBody  = c.ChildBody;
		h.Pin0       = c.Pin0;
		h.Pin1       = c.Pin1;
		h.Pivot      = c.Pivot;

		return this->createJoint(h);

	}

	//CORKSCREW
	if(c.Type == newton::EJT_CORKSCREW) {

		newton::SJointCorkscrew h;

		h.ParentBody = c.ParentBody;
		h.ChildBody  = c.ChildBody;
		h.PinDir     = c.Pin0;
		h.Pivot      = c.Pivot;

		return this->createJoint(h);

	}

	//SLIDER
	if(c.Type == newton::EJT_SLIDER) {

		newton::SJointSlider h;

		h.ParentBody = c.ParentBody;
		h.ChildBody  = c.ChildBody;
		h.PinDir     = c.Pin0;
		h.Pivot      = c.Pivot;

		return this->createJoint(h);

	}

	//BALL
	if(c.Type == newton::EJT_BALL) {

		newton::SJointBall h;

		h.ParentBody = c.ParentBody;
		h.ChildBody  = c.ChildBody;
		h.Pivot      = c.Pivot;

		return this->createJoint(h);

	}


	//UNKNOW JOINT TYPE
	else return NULL;

}

void irr::newton::IWorld::setSolverModel(irr::newton::E_OPTIMIZATION_MODE mode) {

	switch(mode) {
	case EOM_FAST: NewtonSetSolverModel(this->world,2); break;
	case EOM_MEDIUM: NewtonSetSolverModel(this->world,1); break;
	case EOM_PRECISE: NewtonSetSolverModel(this->world,0); break;
	}

}

void irr::newton::IWorld::setFrictionModel(irr::newton::E_OPTIMIZATION_MODE mode) {

	switch(mode) {
	case EOM_FAST: NewtonSetFrictionModel(this->world,2); break;
	case EOM_MEDIUM: NewtonSetFrictionModel(this->world,1); break;
	case EOM_PRECISE: NewtonSetFrictionModel(this->world,0); break;
	}

}

void irr::newton::IWorld::setPlatformArchitecture(irr::newton::E_USE_HARDWARE_MODE mode) {
	switch(mode) {
	case EUHM_MINIMUM: NewtonSetFrictionModel(this->world,2); break;
	case EUHM_MEDIUM: NewtonSetFrictionModel(this->world,1); break;
	case EUHM_MAXIMUM: NewtonSetFrictionModel(this->world,0); break;
	}
}

void irr::newton::IWorld::setMinimumFrameRate(irr::f32 minimum_frame_rate) {
	NewtonSetMinimumFrameRate(this->world,minimum_frame_rate);
}
irr::f32 irr::newton::IWorld::getTimeStep() {
	return 0.f; //return NewtonGetTimeStep(this->world);
}

irr::s32 irr::newton::IWorld::getNewtonVersion() {
	return NewtonWorldGetVersion();
}

//get all bodies
irr::core::array<irr::newton::IBody*> all_bodies;

void NewtonGetAllBodiesCallback (const NewtonBody* body)
{
	all_bodies.push_back(
		(irr::newton::IBody*)(NewtonBodyGetUserData(body))
		);
}

irr::core::array<irr::newton::IBody*> irr::newton::IWorld::getAllBodies() {
	all_bodies.clear();
	//NewtonWorldForEachBodyDo(this->world,NewtonGetAllBodiesCallback);
	return all_bodies;
}
//end of get all bodies

irr::core::array<irr::newton::IBody*> irr::newton::IWorld::getAllPrimitiveCollisionBodies() {
	irr::core::array<irr::newton::IBody*> to_return;
	irr::core::array<irr::newton::IrrNewt_Base*> all_objs = *irr::newton::Hidden::GetObjsList();

	for(irr::u32 i=0;i<all_objs.size();i++) {
		if(all_objs[i]->class_type.Type==ECT_BODY)
			to_return.push_back((irr::newton::IBody*)all_objs[i]);
	}

	return to_return;

}

irr::core::array<irr::newton::ITreeBody*> irr::newton::IWorld::getAllTreeCollisionBodies() {
	irr::core::array<irr::newton::ITreeBody*> to_return;
	irr::core::array<irr::newton::IrrNewt_Base*> all_objs = *irr::newton::Hidden::GetObjsList();

	for(irr::u32 i=0;i<all_objs.size();i++) {
		if(all_objs[i]->class_type.Type==ECT_TREE_BODY)
			to_return.push_back((irr::newton::ITreeBody*)all_objs[i]);
	}

	return to_return;
}

irr::core::array<irr::newton::IMaterial*> irr::newton::IWorld::getAllMaterials() {
	irr::core::array<irr::newton::IMaterial*> to_return;
	irr::core::array<irr::newton::IrrNewt_Base*> all_objs = *irr::newton::Hidden::GetObjsList();

	for(irr::u32 i=0;i<all_objs.size();i++) {
		if(all_objs[i]->class_type.Type==ECT_MATERIAL)
			to_return.push_back((irr::newton::IMaterial*)all_objs[i]);
	}

	return to_return;
}

irr::core::array<irr::newton::IVehicleSimple*> irr::newton::IWorld::getAllSimpleVehicles() {
	irr::core::array<irr::newton::IVehicleSimple*> to_return;
	irr::core::array<irr::newton::IrrNewt_Base*> all_objs = *irr::newton::Hidden::GetObjsList();

	for(irr::u32 i=0;i<all_objs.size();i++) {
		if(all_objs[i]->class_type.Type==ECT_VEHICLE_SIMPLE)
			to_return.push_back((irr::newton::IVehicleSimple*)all_objs[i]);
	}

	return to_return;
}

irr::core::array<irr::newton::ICar*> irr::newton::IWorld::getAllCars() {
	irr::core::array<irr::newton::ICar*> to_return;
	irr::core::array<irr::newton::IrrNewt_Base*> all_objs = *irr::newton::Hidden::GetObjsList();

	for(irr::u32 i=0;i<all_objs.size();i++) {
		if(all_objs[i]->class_type.Type==ECT_CAR)
			to_return.push_back((irr::newton::ICar*)all_objs[i]);
	}

	return to_return;
}

irr::core::array<irr::newton::IVehicleTire*> irr::newton::IWorld::getAllVehicleTires() {
	irr::core::array<irr::newton::IVehicleTire*> to_return;
	irr::core::array<irr::newton::IrrNewt_Base*> all_objs = *irr::newton::Hidden::GetObjsList();

	for(irr::u32 i=0;i<all_objs.size();i++) {
		if(all_objs[i]->class_type.Type==ECT_VEHICLE_TIRE)
			to_return.push_back((irr::newton::IVehicleTire*)all_objs[i]);
	}

	return to_return;
}

void irr::newton::IWorld::clear() {

	/*
	IrrlichtDevice* device=this->getIrrlichtDevice();
	irr::newton::Hidden::DestroyAll();
	irr::newton::IWorld* world=this;
	world=irr::newton::createPhysicsWorld(device);
	*/

	//destroy all related dynamic objects
	irr::core::array<irr::newton::IrrNewt_Base*>* objs = irr::newton::Hidden::GetObjsList();

	for(irr::u32 i=0; i<objs->size(); i++) {

		if(objs->operator[](i)->class_type.Type != irr::newton::ECT_WORLD) {

			objs->operator[](i)->reserved_destroy();
			delete objs->operator[](i);

		}

	}

	objs->clear();
	objs->push_back( this );

	//destroy all
	NewtonDestroyAllBodies(this->world);
	NewtonMaterialDestroyAllGroupID(this->world);

}

