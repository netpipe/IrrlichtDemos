#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <iomanip>
#include <iostream>

using namespace std;

//going to be used for drops
int random();

//Exp required for next level up
//variable need is level
double EXP100(int);

//Damage function prototype
//will pass it min and max damage from weapons/armor and strength.
int damage(int, int, int);

/*int main()
{
	srand(time(NULL));
	cout << " NUMBER: " << random() << endl;
	double total = 0;

	//listing all exp needed for every level
	for(int level = 1; level < 101; level++)
	{

		cout << "It takes " << setprecision(8) << EXP100(level) << " EXP to get from level " << level << " to level " << level + 1 << endl;
		total += EXP100(level);
		cout << "Total EXP up to this point: " <<total << endl;
	}

	
	cout << "Damage with Varying Strength and Weapon Damage" << endl;

	for(int str = 100; str <=200; str += 20)
	{
		cout << "Damage with Strength = " << str << endl;
		for(int x = 200; x <= 1000; x += 200)
		{
			cout << endl;
			cout << "Weapon Damage = " << x << "-" << x + 100 << endl;
			cout << "Damage during battle:" << endl;
			for (int battle = 0; battle < 5; battle++)
				cout << damage(x, x+100, str) << endl;
		}
	}


	for(int x = 0; x <25; x++)
		cout << damage(1000,1001,200) << endl;
	return 0;
}*/

int random()
{
	int random = 0;
	
	for(int garbage = 0; garbage < 30; garbage++)
		rand();

	random = rand();

	return random;
}

double EXP100(int level)
{
	return (50*exp(0.1303 * level));
}

int damage(int wepdmgmin, int wepdmgmax, int str)
{
	int dmg = 0, difference = 0, actualdmg = 0;

	difference = abs(wepdmgmax - wepdmgmin);
	if(wepdmgmin == wepdmgmax)
		difference = 1;

	actualdmg = wepdmgmin + random()%difference;

	dmg = actualdmg + random()%(str);
	
	if(dmg < wepdmgmin)
		dmg = wepdmgmin;

	return dmg;
}