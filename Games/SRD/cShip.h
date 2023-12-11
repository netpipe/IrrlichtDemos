#ifndef SHIP_H
#define SHIP_H

#include <irrlicht.h>
#include "cObject.h"

using namespace irr;
using namespace core;
using namespace video;

class cLaser : public cObject
{
	public:
		float angle;		// angle of shots that are not circles (missles, laser)
		int dmg;			// damage this shot does
		int age;			// How long this shot has been alive
		int lifeSpan;		// how long this shot has been alive
		bool remove;		// Flag for removal
};

class cShip : public cObject
{
	public:
		cShip();
		void SubtractHealth(float energy);	// Takes health
		void SubtractEnergy(float energy);	// Takes energy

		void AddLaserShot();					// Fires single laser shot
		void RemoveLaserShot(cLaser *laser);	// Removes expired shots

		bool alive;					// Is player still alive
		float angle;		// angle
		float speed;		// speed
		float turnSpeed;	// turn speed
		SColor color;		// ship color
		bool shield;		// shield flag
		


		// Laser vars
		list<cLaser*> Laser;	// Linked list of laser shots
		bool laserReady;		// Laser is ready to fire
		int laserEnergyCost;	// Energy cost per laser shot
		float laserSpeed;		// Speed of laser shots
		int fireRateTimer;		// How much time between shots to controll fire rate


		// Ship stats
		float health;				// Current health
		float maxHealth;			// Max health
		
		float energy;				// Current energy
		float maxEnergy;			// Max amount of energy
		
		float energyRechargeRate;	// Rate at which energy recharges


		// NEW
		int xp;						// Experience points
		int score;					// Score
		int credits;				// Credits for buying new weapons, ships, etc.
	

};

#endif

cShip::cShip() : alive(true), angle(0), speed(0.0005), turnSpeed(0.0045), shield(false), laserReady(true), laserEnergyCost(30), laserSpeed(0.8),
				 health(100), maxHealth(100), energy(50), maxEnergy(100),
				 energyRechargeRate(0.05), fireRateTimer(0) {};

void cShip::SubtractHealth(float amount)
{
	// Take energy
	if(health - amount > 0)
		health -= amount;
	else
	{
		// die
		health = 0;
		alive = false;
		printf("You have died!\n");
	}
}

void cShip::SubtractEnergy(float amount)
{
	// Take energy
	if(energy - amount > 0)
		energy -= amount;
	else
	{
		energy = 0;
		shield = false;
	}

	if(amount >= 1)
		printf("You lose %d energy.\n", (int)amount);
}

void cShip::AddLaserShot()
{
	// Create new laser shot
	cLaser *temp = new cLaser;
		temp->pos = pos;
		temp->size = 1 + (energy/10);
		temp->mass = 0;
		temp->angle = angle;
		temp->dmg = energy*2;		// based on players current energy level
		temp->age = 0;
		temp->lifeSpan = 500;
		temp->remove = false;

		// Calculate vel
		vector2df laserAcc( cosf(angle), sinf(angle) );
		laserAcc.normalize() *= laserSpeed;
		temp->vel = laserAcc;

		// Add it to the list
		Laser.push_front(temp);

		printf("You fire your laser at %d power.\n", temp->dmg);
}
void cShip::RemoveLaserShot(cLaser *laser)
{

	// FIX THIS
	
	// currently the remove function is called in the update which breaks the update loop

}