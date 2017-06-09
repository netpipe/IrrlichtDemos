#ifndef _STYPES_H_
#define _STYPES_H_

#include "stdafx.h"
#include "irrlicht.h"


//TODO: MAKE LIST OF DIFFERENT SHIPS
//GIVE THEM VALUES ACCORDING TO STRUCT
//MAKE THEM DIFFERENT

enum ship_class
{
	TERR_IMPERIUM_FRIGATE,					// imperium class terran frigate
	TERR_LEGION_DESTROYER,					// legion class terran destroyer
	TERR_PRAETORIAN_CRUISER,
	TERR_DOMINION_DREAD,
	TERR_TIBERIUS_CARRIER,
	PROV_HAWK_FRIGATE,
	PROV_EAGLE_DESTROYER,
	PROV_VULTURE_CRUISER,
	PROV_CONDOR_DREAD,
	PROV_TALON_CARRIER,
	PIRATE_RAIDER
};

enum ship_faction
{
	FACTION_TERRAN_ALLIANCE,
	FACTION_PROVIAN_CONSORTIUM,
	FACTION_PIRATE
};
enum ship_types 
{
	TYPE_SHIP,
	TYPE_STATION,
};

enum turret_class
{
	TYPE_TURRET_RAIL,						// railgun				- primary
	TYPE_TURRET_PHOTON,						// photon cannon		- primary
	TYPE_TURRET_PD,							// point defense		- light
	TYPE_TURRET_FLAK,						// flak cannons			- light
	TYPE_TURRET_PARTICLE,					// particle disruptor	- secondary
	TYPE_TURRET_PLASMA,						// plasma cannon		- secondary
	TYPE_TURRET_ION_CANNON,					// ion cannon			- heavy
	TYPE_TURRET_KRYON_TORPEDO,				// kryon torpedo		- heavy
	TYPE_TURRET_KINETIC_CANNON				// kinetic cannon		- crap
};


#endif _STYPES_H_