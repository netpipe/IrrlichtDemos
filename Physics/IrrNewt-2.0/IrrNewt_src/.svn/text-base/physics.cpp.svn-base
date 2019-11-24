// Copyright (C) 2006 Mancuso Raffaele
// This file is part of the "IrrNewt library".
// For conditions of distribution and use, see copyright notice in IrrNewt.hpp

#include "../include/IrrNewt.hpp"
#include "../include/world.hpp"

//to create the world
irr::newton::IWorld* irr::newton::createPhysicsWorld(irr::IrrlichtDevice* device) {
	irr::newton::Hidden::DestroyAllOnExit();

	irr::newton::IWorld* world = new irr::newton::IWorld(device);

	//register for destroy at exit
	irr::newton::Hidden::AddObjectToDestroy(world);

	return world;
}

static bool log_stat=true;
void irr::newton::setLogStatus(bool value) {log_stat=value;}
bool irr::newton::getLogStatus() {return log_stat;}
