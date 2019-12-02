/*class Character
{
	protected:
		int ID;
		stringc Name;

		int Constitution;
		int Strength;
		int Dexterity;
		int Intelligence;
		int StatPts;

		int Level;
		double EXP_total;

		char Race;

		time_t Anniversary;
		stringc Lastlogin;
		double OnlineTime;
		struct tm * STime;
		time_t ETime;

		int location[2];

	public:
		int getID();
		void setID(int);

		int mapX, mapY;
		float posx, posy, posz;

		irr::scene::ISceneNode* playerModel;
		
		stringc getName();
		wchar_t* getWideName();
		void setName(stringc);

		int getCon();
		void setCon(int);
		int getStr();
		void setStr(int);
		int getDex();
		void setDex(int);
		int getInt();
		void setInt(int);
		int getStatPts();
		void setStatPts(int);

		int getLvl();
		void setLvl(int);

		double getEXP();
		void setEXP(double);

		char getRace();
		void setRace(char);

		time_t  getAnn();
		void setAnn(time_t);

		stringc getLL();
		void setLL(stringc);

		double getOnline();
		void setOnline(double);

		struct tm * getStime();
		void setStime(struct tm *);

		time_t getEtime();
		void setEtime(time_t);

		void display();

		Character(int, stringc, time_t);

		int getLocation();
		void setLocation(int x, int y);
};

//CONSTRUCTOR
Character::Character(int id, stringc name, time_t start)
{
	ID = id;
	Name = name;
	Anniversary = start;
	STime = localtime ( &start );

	Constitution = 0;
	Strength = 0;
	Dexterity = 0;
	Intelligence = 0;
	StatPts = 10;

	Level = 1;
	EXP_total = 0;

	Race = 'H';

	location[0]=0;
	location[1]=0;
}


//all the Gets, to return it to somewhere people can see it.

int Character::getLocation(){
	return *location;
}
int Character::getID()
{
	return ID;
}

stringc Character::getName()
{
	return Name;
}
wchar_t* Character::getWideName()
{
	//convert char array -> wchar_t*
	size_t alloc_len = strlen(Name.c_str()) + 1;
	wchar_t* WideName = new wchar_t[alloc_len];
	size_t result = mbstowcs(WideName, Name.c_str(), alloc_len);
	//end conversion
	return WideName;
}
int Character::getCon()
{
	return Constitution;
}

int Character::getStr()
{
	return Strength;
}

int Character::getDex()
{
	return Dexterity;
}

int Character::getInt()
{
	return Intelligence;
}

int Character::getStatPts()
{
	return StatPts;
}

int Character::getLvl()
{
	return Level;
}

double Character::getEXP()
{
	return EXP_total;
}

char Character::getRace()
{
	return Race;
}

time_t  Character::getAnn()
{
	return Anniversary;
}

stringc Character::getLL()
{
	return Lastlogin;
}

double Character::getOnline()
{
	return OnlineTime;
}

struct tm * Character::getStime()
{
	return STime;
}

time_t Character::getEtime()
{
	return ETime;
}


//SETS::

void Character::setLocation(int x, int y)
{
	location[0]=x;
	location[1]=y;
}


void Character::setID(int id)
{
	ID = id;
}

void Character::setName(stringc name)
{
	Name = name;
}

void Character::setCon(int con)
{
	Constitution = con;
}

void Character::setStr(int str)
{
	Strength = str;
}

void Character::setDex(int dex)
{
	Dexterity = dex;
}

void Character::setInt(int inte)
{
	Intelligence = inte;
}

void Character::setStatPts(int stat)
{
	StatPts = stat;
}

void Character::setLvl(int lvl)
{
	Level = lvl;
}

void Character::setEXP(double exp)
{
	EXP_total = exp;
}

void Character::setRace(char r)
{
	Race = r;
}

void Character::setAnn(time_t  anniv)
{
	Anniversary = anniv;
}

void Character::setLL(stringc ll)
{
	Lastlogin = ll;
}

void Character::setStime(struct tm * timeas)
{
	STime = timeas;
}

void Character::display()
{
	printf("This is the Characters information:  endl");
/*	cout << "Name: " << getName() << " Level: " << getLvl() << endl
		 << "Race: " << getRace() << " ID: " << getID() << endl
		 << "Str: " << getStr() << " Dex: " << getDex() << endl
		 << "Con: " << getCon() << " Int: " << getInt() << endl
		 << "EXP total: " << getEXP() << " Anniversary: " << getAnn() << endl
		 << "Last Login: " << getLL() << " Start Time: " << asctime(getStime()) 
		 << "End Time: " << getEtime() << endl;*
}*/

class Character
{
public:
	void setEquipment(int loc, int id);
	void setInventory(int loc, int id);
	irr::scene::ISceneNode* playerModel;
	stringc inGameName, title;
	vector3df movVec;
	double startMovTime, posx, posy, posz, mapX, mapY, mapZ, id,nodeId, level, intel, con, dex, str, rotation, raceId, mountId, modelId, expOn, expTot, skillLevel, unusedSkillPoints, rotateSpeed, maxhp, curhp, speed, modelHeight; 
	int pvp, showeq, visibility, rotationDir, type, equipmentList[19], inventoryList[40]; 
	bool criminal, mounted, loaded, mov1, mov2, mov3, mov4, jump, buildModel, inUse, flying;
	Character();
};
void Character::setInventory(int loc, int id)
{
	//point to location in item list
	inventoryList[loc]=id;
	if(id!=0)
	{
		printf("loc %i", loc);
	}
}
void Character::setEquipment(int loc, int id)
{
	//point to location in item list
	equipmentList[loc]=id;
}
Character::Character()
{
	for(int x=0;x<19;x++)
	{
		equipmentList[x]=-1;
	}
	for(int x=0;x<40;x++)
	{
		inventoryList[x]=-1;
	}
	flying=false;
}