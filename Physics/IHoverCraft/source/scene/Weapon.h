#ifndef IWEAPON_H_
#define IWEAPON_H_

class Game;
class Ship;
class Terrain;

class Weapon
{

protected:
	
	const Game* game;
	
public:

	Weapon(const Game* game) :
		game(game)
	{
		
	}
	
	virtual ~Weapon()
	{
		
	}
	
	virtual const void fire(const Ship* ship, const Terrain* terrain) const = 0;
};

#endif /*IWEAPON_H_*/
