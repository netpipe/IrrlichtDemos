//**********************************
// ITEM CLASS, holds item data
//**********************************

class itemClass
{
	public:
	void loadFromFile(int fileId);
	void sendData(int sock);
	void saveFile();
	int getId();
	double getLastUsed();
	void unloadItem();
	itemClass();
	void copyData(double &damage, double &defense);
	private:
	int id, type, eqslot, iconId, modelId, openSlots, quality, setId, cost;
	stringc name, description;
	double damage, defense, lastUsed;
	int lastUsedI;
	bool usable, stackable, quest, multiOwner, loaded;
	//orbs
	//reqs
	vector<int> reqs[2];
};

void itemClass::copyData(double &damageOut, double &defenseOut)
{
	//copies data for updating player information
	damageOut=damage;
	defenseOut=defense;
}

itemClass::itemClass()
{
	//constructor, sets everything to 0 or "" or false
	lastUsed=0;
	name="";
	description="";
	id=-1;
	type=0;
	eqslot=0;
	iconId=0;
	modelId=0;
	openSlots=0;
	quality=0;
	setId=0;
	cost=0;
	damage=0;
	defense=0;
	usable=false;
	stackable=false;
	quest=false;
	multiOwner=false;
	loaded=false;
	lastUsedI=1;
}
void itemClass::unloadItem()
{
	//save item
	saveFile();
	//remove from active memory
	type=0;eqslot=-1;iconId=-1;modelId=-1;openSlots=0;quality=0;setId=0;cost=0;
	name="DNE";description="DNE";
	damage=0; defense=0;lastUsed=0;
	usable=false;stackable=false;quest=false;multiOwner=false;
	//mark as open slot
	id=-1;
	loaded=false;
}

void itemClass::sendData(int sock)
{
	//get time
	timeval newTimea;
	gettimeofday(&newTimea, NULL);
	double time= ((.001*newTimea.tv_usec)+(newTimea.tv_sec*1000));
	cout <<id<<endl;
	//reset lastUsed
	lastUsed=time;
	if(loaded)
	{
		//make sure item is loaded
		stringc sendData="";
		//populate sendData with all needed data
		char charbuffer[500];
		sprintf(charbuffer,"30|%i|%i|%i|%i|%i|%i|%i|%i|%s|%f|%f|%i|", id, type,  modelId, iconId, openSlots, setId,cost,quality,name.c_str(), damage, defense, quest);
		sendData=charbuffer;
		//add header info
		addMessageHeader(sendData);
		//send data
		send(clientsock[sock], sendData.c_str(), sendData.size(),0);
		//get description
		sprintf(charbuffer,"31|%i|%s|", id, description.c_str());
		sendData=charbuffer;
		//add header info
		addMessageHeader(sendData);
		//send data
		send(clientsock[sock], sendData.c_str(), sendData.size(),0);
	}
}

void itemClass::saveFile()
{
	//save file
	ofstream itemFile;
	stringc dataFileS;
	char dataFile[200];
	//get file location from id
	sprintf(dataFile,"database/items/%i.rofh", id);
	dataFileS=dataFile;
	//open file
	itemFile.open(dataFileS.c_str());
	if(itemFile.is_open())
	{
		//if file is open
		//save data one var per line
		itemFile << type;
		itemFile << '\n';
		itemFile << eqslot;
		itemFile << '\n';
		itemFile << iconId;
		itemFile << '\n';
		itemFile << modelId;
		itemFile << '\n';
		itemFile << openSlots;
		itemFile << '\n';
		itemFile << quality;
		itemFile << '\n';
		itemFile << setId;
		itemFile << '\n';
		itemFile << cost;
		itemFile << '\n';
		itemFile << name.c_str();
		itemFile << '\n';
		itemFile << description.c_str();
		itemFile << '\n';
		itemFile << damage;
		itemFile << '\n';
		itemFile << defense;
		itemFile << '\n';
		itemFile << usable;
		itemFile << '\n';
		itemFile << stackable;
		itemFile << '\n';
		itemFile << quest;
		itemFile << '\n';
		itemFile << multiOwner;
		itemFile << '\n';
		//reqs	
		itemFile << reqs[0].size();
		itemFile << '\n';
		for(int x=0;x<reqs[0].size();x++)
		{
		itemFile << reqs[0][x];
		itemFile << '\n';
		itemFile << reqs[1][x];
		itemFile << '\n';
		}
		//orbs
		itemFile << '0';
		itemFile << '\n';
		//close file
		itemFile.close();
	}
}

double itemClass::getLastUsed()
{
	//get last Used time
	return lastUsed;
}

void itemClass::loadFromFile(int fileId)
{
	//load data from file
	cout << "loading item "<< fileId <<endl;
	//get time
	timeval newTime;
	gettimeofday(&newTime, NULL);
	double time= ((.001*newTime.tv_usec)+(newTime.tv_sec*1000));
	//set last used to time
	lastUsed=time;
	//load from file
	ifstream itemFile;
	char dataFile[200];
	//get file loc from id
	sprintf(dataFile, "database/items/%i.rofh", fileId);
	stringc dataFileS=dataFile;
	//open file
	itemFile.open(dataFileS.c_str());
	//set id
	id=fileId;
	if(itemFile.is_open())
	{
		//if file is open
		cout << "file open"<<endl;
		//load data from file, file list posted on forums, link below
		//http://rofhonline.com/forums/viewtopic.php?f=8&t=204
		std::string infromfile;
		getline(itemFile, infromfile);
		type=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		eqslot=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		iconId=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		modelId=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		openSlots=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		quality=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		setId=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		cost=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		name=infromfile.c_str();
		getline(itemFile, infromfile);
		description=infromfile.c_str();
		getline(itemFile, infromfile);
		damage=atof(infromfile.c_str());
		getline(itemFile, infromfile);
		defense=atof(infromfile.c_str());
		getline(itemFile, infromfile);
		usable=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		stackable=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		quest=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		multiOwner=atoi(infromfile.c_str());
		getline(itemFile, infromfile);
		int numReqs=atoi(infromfile.c_str());
		for(int x=0;x<numReqs;x++)
		{
			getline(itemFile, infromfile);
			reqs[0].push_back(atoi(infromfile.c_str()));
			getline(itemFile, infromfile);
			reqs[1].push_back(atoi(infromfile.c_str()));
		}
		//close file and state loaded
		itemFile.close();
		loaded=true;
	}
	else
	{
		//file does not exist default options
		type=0;eqslot=-1;iconId=-1;modelId=-1;openSlots=0;quality=0;setId=0;cost=0;
		name="DNE";description="DNE";
		damage=0; defense=0;lastUsed=0;
		usable=false;stackable=false;quest=false;multiOwner=false;
		loaded=true;
	}
}

int itemClass::getId()
{
	//return item id
	return id;
}

