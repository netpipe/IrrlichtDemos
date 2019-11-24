// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/vehicle_tire.hpp"
#include "../include/pos_rot.hpp"
#include "../include/utils.hpp"
#include "../include/hidden.hpp"

using namespace irr::newton::Hidden;


void irr::newton::IVehicleTire::Initialize() {

	this->class_type.Type=ECT_VEHICLE_TIRE;
	this->tire_id=NULL;
	this->world = NULL;
	this->torque=0.0f;
	this->steer_angle=0.0f;
	this->auto_calculate_max_brake_acceleration=false;
	this->brake_acceleration=0.0f;
	this->brake_max_friction_torque=0.0f;
	this->is_air_borne=false;

	this->max_steer_angle = 3.14159f * 2;
	this->min_steer_angle = -this->max_steer_angle;
}

irr::newton::IVehicleTire::IVehicleTire(const IVehicleTire& t):IrrNewt_Base() {

	this->tire_id=t.tire_id;
	this->torque=t.torque;
	this->steer_angle=t.steer_angle;
	this->tire_node=NULL;
	this->auto_calculate_max_brake_acceleration=t.auto_calculate_max_brake_acceleration;

	this->world = t.world;
}

irr::newton::IVehicleTire::IVehicleTire(
				irr::newton::IVehicleSimple* vehicle,
				irr::core::vector3df pin,
				irr::f32 mass,
				irr::f32 width,
				irr::f32 radius,
				irr::f32 sus_shock,
				irr::f32 sus_spring,
				irr::f32 sus_length,
				int collision_ID,
				irr::scene::ISceneNode* tire_scene_node,
				irr::newton::SPosRot body_scene_node_offset) {

	this->Initialize();

	this->body_node_offset=body_scene_node_offset;
	this->tire_node=tire_scene_node;
	this->n_vehicle=const_cast<NewtonJoint*>(vehicle->getNewtonJoint());

	this->world = vehicle->getWorld();

	//instanziate utils
	irr::newton::IUtils* utils=new irr::newton::IUtils(
		((irr::newton::IWorld*)NewtonWorldGetUserData(vehicle->n_world))
		);

	if(width==-1.0f)
		width=utils->calculateTireWidth(tire_scene_node);
	if(radius==-1.0f)
		radius=utils->calculateTireWidth(tire_scene_node);

	delete utils;


	//get offset matrix
	core::matrix4 offset_relative_chassis = irr::newton::Hidden::getIrrOffsetMatrix(vehicle->getChassis()->getNode(),tire_scene_node);
	offset_relative_chassis.setTranslation(
		offset_relative_chassis.getTranslation()*IrrToNewton
		);

	//interpolate with body node offset
	//offset_relative_chassis.setTranslation(offset_relative_chassis.getTranslation()+this->body_node_offset.Position);
	//offset_relative_chassis.setRotationDegrees(offset_relative_chassis.getRotationDegrees()+this->body_node_offset.Position);

	irr::f32 pin_array[3];
	irr::newton::Hidden::Vector3dfFillArray(pin,pin_array);

	/*this->tire_id=NewtonVehicleAddTire(
		vehicle->getNewtonJoint(),
		GetMatrixPointer(offset_relative_chassis),
		pin_array,
		mass,
		width,
		radius,
		sus_shock,
		sus_spring,
		sus_length,
		this,
		collision_ID);*/
}

irr::newton::IVehicleTire::~IVehicleTire() {

}

void irr::newton::IVehicleTire::reserved_destroy() {
	this->~IVehicleTire();
}

irr::core::vector3df irr::newton::IVehicleTire::getPosition() {
	irr::core::matrix4 mat;
	/*NewtonVehicleGetTireMatrix(this->n_vehicle,this->tire_id, GetMatrixPointer(mat));
	mat.setTranslation(mat.getTranslation()*NewtonToIrr);*/
	return mat.getTranslation();
}

irr::core::vector3df irr::newton::IVehicleTire::getRotation() {
	irr::core::matrix4 mat;
	//NewtonVehicleGetTireMatrix(this->n_vehicle,this->tire_id, GetMatrixPointer(mat));
	return mat.getRotationDegrees();
}

irr::newton::SPosRot irr::newton::IVehicleTire::getPositionRotation() {
	irr::core::matrix4 mat;
	/*NewtonVehicleGetTireMatrix(this->n_vehicle,this->tire_id, GetMatrixPointer(mat));
	mat.setTranslation(mat.getTranslation()*NewtonToIrr);*/
	return irr::newton::SPosRot(mat.getTranslation(),mat.getRotationDegrees());
}

irr::core::matrix4 irr::newton::IVehicleTire::getMatrix() {
	irr::core::matrix4 mat;
	//NewtonVehicleGetTireMatrix(this->n_vehicle,this->tire_id,GetMatrixPointer(mat));
	//mat.setTranslation(mat.getTranslation()*NewtonToIrr);
	return mat;
}

void irr::newton::IVehicleTire::remove() {
	//NewtonVehicleRemoveTire(this->n_vehicle,this->tire_id);
}
