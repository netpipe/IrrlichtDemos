int attack(int damage, int defense)
{
	//calculate attack damage from given damage from something and defense of target
	int attack=0;
	attack=damage-defense;
	//make sure its positive
	if(attack<0)
	{
		attack=0;
	}
	return attack;
}
