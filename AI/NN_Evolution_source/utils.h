
// returns a random floating point value between 0 and 1.0
float fRand()
{
	return ((float)(rand()%10000)) * 0.0001;
}

// returns a random floating point value between -1 and 1
float fRand2()
{
	if (rand()%2)
		return fRand();
	else
		return -1.f*fRand();
}

// Checks that a string only contains a number, and no letters. (and is not empty)
bool strIsNumber(const core::stringw str)
{
	if (str.size() == 0)
		return false;
	
	for (int i = 0; i < str.size(); i ++)
	{
		if (str[i] < 46 || str[i] > 57)
			return false;
	}
	
	return true;
}
