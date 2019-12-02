class itemClass
{
public:
	void setInfo(stringc serverString);
	void setDescription(stringc serverString);
	void setItemId(int uid);
	int getIcon();
	int getId();
	itemClass();
	bool getLoaded();
private:
	int id, type, modelId, iconId, openSlots, setId, cost, quality;
	stringc name, description;
	double damage, defense;
	bool loaded, quest;
};
int itemClass::getIcon()
{
	return iconId;
}
void itemClass::setInfo(stringc serverString)
{
	printf("\nGOT INFO!\n");
	stringc brokenString[13];
	breakCode(serverString, brokenString,13);
	type=atoi(brokenString[2].c_str());
	modelId=atoi(brokenString[3].c_str());
	iconId=atoi(brokenString[4].c_str());
	openSlots=atoi(brokenString[5].c_str());
	setId=atoi(brokenString[6].c_str());
	cost=atoi(brokenString[7].c_str());
	quality=atoi(brokenString[8].c_str());
	name=brokenString[9];
	damage=atof(brokenString[10].c_str());
	defense=atof(brokenString[11].c_str());
	quest=atoi(brokenString[12].c_str());
	loaded=true;
}
void itemClass::setDescription(stringc serverString)
{
	stringc brokenString[3];
	breakCode(serverString, brokenString,3);
	description=brokenString[2];
}
itemClass::itemClass()
{
	//initialize variables
	id=-1;
	iconId=0;
	loaded=false;
}
bool itemClass::getLoaded()
{
	return loaded;
}
int itemClass::getId()
{
	return id;
}
void itemClass::setItemId(int uid)
{
	id=uid;
}

//*******************************************************
//********************* ITEM SYSTEM *********************
//*******************************************************
class itemSys
{
public:
	void update();
	void addItem(int uid);
	void setInfo(int id, stringc serverString);
	int findItem(int uid);
	void setDescription(int id, stringc serverString);
	itemSys();
	int getIcon(int id);
private:
	vector<itemClass> items;
	void requestItem(int uid);
	void removeItem(int uid);
	double lastUpdate;
};
int itemSys::getIcon(int id)
{
	int loc = findItem(id);
	return items[loc].getIcon();
}
itemSys::itemSys()
{
	lastUpdate=0;
}
void itemSys::setInfo(int id, stringc serverString)
{
	printf("\nSETTING INFO\n");
	int loc=findItem(id);
	items[loc].setInfo(serverString);
}
void itemSys::setDescription(int id, irr::core::stringc serverString)
{
	printf("\nSETTING DESCR\n");
	int loc=findItem(id);
	items[loc].setDescription(serverString);
}
void itemSys::update()
{
	//load items that aren't loaded
	double time = (f32)timer->getTime() / 1000.0f;
	if(lastUpdate+1<time)
	{printf("\nupdate\n");
		lastUpdate=time;
		for(int x=0;x<items.size();x++)
		{
			if(!items[x].getLoaded()&&items[x].getId()!=0)
			{
				requestItem(items[x].getId());
			}
		}
	}
}
void itemSys::requestItem(int uid)
{

	char sendChar[200];
	stringc sendCode;
	sprintf(sendChar,"16|%i|",uid);
	sendCode=sendChar;
	send(serverSocket,sendCode.c_str(), sendCode.size(),NULL);
}
int itemSys::findItem(int uid)
{
	//loop till you find the item
	for(int x=0;x<items.size();x++)
	{
		if(items[x].getId()==uid)
		{
			//return position
			return x;
		}
	}
	//did not find it, readd
	addItem(uid);
	//loop till you find the item, reverse since you just added it
	for(int x=items.size()-1;x>-1;x--)
	{
		if(items[x].getId()==uid)
		{
			//return position
			return x;
		}
	}
	printf("OH NO!!! BIG ITEM ISSUE!");
	return -1;
}
void itemSys::addItem(int uid)
{
	bool found=false;
	for(int x=0;x<items.size();x++)
	{
		if(items[x].getId()==uid)
		{
			found=true;
			x=items.size();
		}
	}
	if(!found)
	{
		itemClass newItem;
		newItem.setItemId(uid);
		items.push_back(newItem);
	}
}