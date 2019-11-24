// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/vehicle_car.hpp"
#include "../include/structures.hpp"
#include "../include/world.hpp"
#include "../include/utils.hpp"


void irr::newton::ICar::createWheels(irr::newton::IWorld* world, SCar car)
{
	irr::newton::STire tire_data;

	//build tire data
	tire_data.Width=car.TiresWidth;
	tire_data.Radius=car.TiresRadius;
	tire_data.SuspensionSpring=car.SuspensionSpring;
	tire_data.SuspensionShock=car.SuspensionShock;
	tire_data.SuspensionLength=car.SuspensionLength;
	tire_data.Mass=car.TiresMass;
	tire_data.Pin=car.TiresPin;
	tire_data.BodyOffsetFromSceneNode=car.TiresBodyOffsetFromSceneNode;

	//positiong tires
	if(car.TiresOffsetFromChassisFront != core::vector3df() && car.TiresOffsetFromChassisBack != core::vector3df())
		world->getUtils()->calculateCarTiresPosition(car, car.TiresOffsetFromChassisFront, car.TiresOffsetFromChassisBack);

	//create tires
	tire_data.Node=car.TireNodeFR;
	this->fr_tire=this->createTire(tire_data);

	tire_data.Node=car.TireNodeFL;
	this->fl_tire=this->createTire(tire_data);

	tire_data.Node=car.TireNodeBR;
	this->br_tire=this->createTire(tire_data);

	tire_data.Node=car.TireNodeBL;
	this->bl_tire=this->createTire(tire_data);

}

irr::newton::ICar::ICar(irr::newton::IWorld* world,SCar car):IVehicleSimple(world,car.ChassisBody,car.UpVector) {

	this->class_type.Type = ECT_CAR;
	this->torque_increment = 0.01f;
	this->steer_angle_increment = 0.01f;

	if(car.TiresWidth == UNDEFINED_PARAMETER)
		world->getUtils()->calculateTireWidth(car);

	if(car.TiresRadius == UNDEFINED_PARAMETER)
		world->getUtils()->calculateTireRadius(car);

	this->createWheels(world, car);
}

