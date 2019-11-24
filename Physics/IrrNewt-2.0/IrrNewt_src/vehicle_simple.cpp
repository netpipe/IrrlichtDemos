// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/vehicle_simple.hpp"
#include "../include/vehicle_tire.hpp"
#include "../include/world.hpp"
#include "../include/structures.hpp"
#include "../include/hidden.hpp"

void irr::newton::IVehicleSimple::Initialize() {

	this->tires_num=0;
	this->n_world=NULL;
	this->chassis=NULL;
	this->n_joint=NULL;
	this->class_type.Type=ECT_VEHICLE_SIMPLE;

}

irr::newton::IVehicleSimple::IVehicleSimple(const IVehicleSimple& v):irr::newton::IJoint() {
	this->tires_num=v.tires_num;
	this->n_world=v.n_world;
	this->chassis=v.chassis;
	this->n_joint=v.n_joint;
}

irr::newton::IVehicleSimple::IVehicleSimple(
	irr::newton::IWorld* world,
	irr::newton::IBody* chassis_body,
	irr::core::vector3df up_dir) {

	this->Initialize();

	float up_dir_array[3];
	irr::newton::Hidden::Vector3dfFillArray(up_dir,up_dir_array);

	this->n_world=const_cast<NewtonWorld*>(world->getNewtonWorld());
	this->chassis = chassis_body;

	/*this->n_joint = NewtonConstraintCreateVehicle (
		this->n_world,
		up_dir_array,
		chassis_body->getNewtonBody() );*/

	// set the user data
	NewtonJointSetUserData(this->n_joint, this );

	//NewtonJointSetDestructor(this->vehicle, newtonDestructor );

	//set the tire callback.
	//NewtonVehicleSetTireCallback(this->n_joint, irr::newton::Hidden::VehicleTireCallBack);
}

irr::newton::IVehicleSimple::~IVehicleSimple() {

}

void irr::newton::IVehicleSimple::reserved_destroy() {this->~IVehicleSimple();}

irr::newton::IVehicleTire* irr::newton::IVehicleSimple::createTire(irr::newton::STire tire) {
	return this->createTire(tire.Node,tire.Pin,tire.Mass,tire.Width,tire.Radius,
		tire.BodyOffsetFromSceneNode,
		tire.SuspensionShock,tire.SuspensionSpring,tire.SuspensionLength);
}


void irr::newton::IVehicleSimple::removeWheels()
{
    irr::core::array<irr::newton::IVehicleTire*> Tires = getAllTires();

    for(s32 i=0; i < Tires.size(); ++i)
    Tires[i]->remove();

    //this->tires_num = 0;
}

irr::newton::IVehicleTire* irr::newton::IVehicleSimple::createTire(
	irr::scene::ISceneNode* tire_node,
	irr::core::vector3df pin,
	irr::f32 mass,
	irr::f32 width,
	irr::f32 radius,
	irr::newton::SPosRot body_scene_node_offset,
	irr::f32 sus_shock,
	irr::f32 sus_spring,
	irr::f32 sus_length)
 {

	irr::newton::IVehicleTire* tire=new irr::newton::IVehicleTire(
		this,
		pin,
		mass,
		width,
		radius,
		sus_shock,
		sus_spring,
		sus_length,
		this->tires_num,
		tire_node,
		body_scene_node_offset
		);

	this->tires_num++;

	irr::newton::Hidden::AddObjectToDestroy(tire);

	return tire;
}

irr::newton::IVehicleTire* irr::newton::IVehicleSimple::getFirstTire() {
	irr::newton::IVehicleTire* t=NULL;
	/*void* tire_id=NewtonVehicleGetFirstTireID(this->n_joint);

	if(tire_id) {
		t=(irr::newton::IVehicleTire*)NewtonVehicleGetTireUserData(this->n_joint,tire_id);
	}*/

	return t;
}



irr::newton::IVehicleTire* irr::newton::IVehicleSimple::getNextTire(
			irr::newton::IVehicleTire* current_tire) {

	irr::newton::IVehicleTire* t=NULL;
	/*void* tire_id=NewtonVehicleGetNextTireID(
		this->n_joint,
		const_cast<void*>(current_tire->getNewtonTireId()));

	if(tire_id)
		t=(irr::newton::IVehicleTire*)NewtonVehicleGetTireUserData(this->n_joint,tire_id);
*/
	return t;
}

irr::core::array<irr::newton::IVehicleTire*> irr::newton::IVehicleSimple::getAllTires() {
	irr::core::array<irr::newton::IVehicleTire*> array;

	irr::newton::IVehicleTire* current_tire=this->getFirstTire();

	if(current_tire==NULL) return array;
	else array.push_back(current_tire);

	while(true) {
		current_tire=this->getNextTire(current_tire);
		if(current_tire==NULL) break;
		else
			array.push_back(current_tire);
	}

	return array;
}

void irr::newton::IVehicleSimple::reset() {
	//NewtonVehicleReset(this->n_joint);
}
