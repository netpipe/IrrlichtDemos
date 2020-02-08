#ifndef WEAPONSYSTEM_H_
#define WEAPONSYSTEM_H_

#include "../scene/Weapon.h"
#include "../scene/Laser.h"

class WeaponSystem
{

private:
	
	const Game* game;
	
	irr::core::list<Weapon*> availableWeapons;

	Weapon* currentWeapon;
	
public:

	WeaponSystem(const Game* game) :
		game(game), currentWeapon(0)
	{
		Laser* laser = new Laser(game);
		availableWeapons.push_back(laser);

		this->currentWeapon = laser;
	}

	~WeaponSystem()
	{
	    irr::core::list<Weapon*>::Iterator it;
	    for(it = this->availableWeapons.begin(); it != this->availableWeapons.end(); ++it)
	    {
	        Weapon* oo = (*it);
	        delete oo;
	    }
	    this->availableWeapons.clear();
	}

	const void fire(const Ship* ship, const Terrain* terrain) const
	{
		if (this->currentWeapon)
			this->currentWeapon->fire(ship, terrain);
	}
	
	void nextWeapon()
	{
		irr::core::list<Weapon*>::Iterator it;
		for(it = this->availableWeapons.begin(); it != this->availableWeapons.end(); ++it)
		{
			Weapon* w = (*it);
			if (w == this->currentWeapon)
			{
				this->currentWeapon = *it++;
				break;
			}
		}
	}

	void lastWeapon()
	{
		
	}
};

#endif /*WEAPONSYSTEM_H_*/
