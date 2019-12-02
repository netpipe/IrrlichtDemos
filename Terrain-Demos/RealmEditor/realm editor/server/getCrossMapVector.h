//get vector between to points across maps from a to b
void getCrossMapVector(double aposx,double aposy,double aposz,double bposx,double bposy,double bposz, int amapx, int amapy, int amapz, int bmapx, int bmapy, int bmapz, double &vecx, double &vecy, double &vecz)
{
//get map differences
	if(amapx!=bmapx)
	{
		if(amapx>bmapx)
		{
			//correct position, a is more maps away, add position to it
			aposx+=mapSize*abs(amapz-bmapz);
		}
		else
		{
			//correct position, b is more maps away, add position to it
			bposx+=mapSize*abs(amapz-bmapz);
		}
	}
	if(amapy!=bmapy)
	{
		if(amapy>bmapy)
		{
			//correct position, a is more maps away, add position to it
			aposy+=mapSize*abs(amapz-bmapz);
		}
		else
		{
			//correct position, b is more maps away, add position to it
			bposy+=mapSize*abs(amapz-bmapz);
		}
	}
	if(amapz!=bmapz)
	{
		if(amapz>bmapz)
		{
			//correct position, a is more maps away, add position to it
			aposz+=mapSize*abs(amapz-bmapz);
		}
		else
		{
			//correct position, b is more maps away, add position to it
			bposz+=mapSize*abs(amapz-bmapz);
		}
	}
	//get differecne in positions and store as vectors
	vecx=bposx-aposx;
	vecy=bposy-aposy;
	vecz=bposz-aposz;
}
