/****************************************************
// PLAYER CLASS
****************************************************/

class playerClass
{
	public:
	void moveInventory(int location1, int location2);
	void equip(int slot, int loc);
	void deequip(int slot, int loc);
	bool checkReqs(int slot, int loc);
	int getAttackDamage();
	void loadFromFile(stringc dataFile);
	void sendStart(int sock);
	void sendInventory(int sock);
	void sendPlayer(int sock, int from);
	int savePlayer();
	playerClass();
	stringc email, inGameName, userName, password, title;
	double  speed, posx, posy, posz, rotation, qsecposx, qsecposy, qsecposz, lastAttackTime, lastSkillTime, attackRadius, attackDelay, damage, defense;
	float movx, movy, movz;
	int level, clan, str, dex, con, curhp, maxhp, intel, expOn, expTot, skillLevel, unusedSkillPoints, equipmentList[19], inventory[40], mountId, raceId, modelId,pvp, showeq, visibility,mapx, mapy, mapz, rotationDir, id, targetType,targetId, targetIndex, targetKey;
	bool criminal, jump, loaded, mounted, updateMov[maxconnections], updatePos[maxconnections];
	vector<int> localPlayersR, skillList, faction[2];
	};

int playerClass::getAttackDamage()
{
	//return damage
	return damage;
}

playerClass::playerClass()
{
	//set to 0
	attackRadius=1000;
	targetId=-1;
	damage=0;
	defense=0;
}
void playerClass::equip(int slot, int loc)
{
	//add bonuses and stats from item located in loc
	if(loc==0)
	{
		//equipmentList
		double damageOut=0, defenseOut=0;
		//get item data from its position
		itemSys.getData(itemSys.findItem(equipmentList[slot])).copyData(damageOut, defenseOut);
		//add data to vars
		damage+=damageOut;
		defense+=defenseOut;
	}
	else if(loc==1)
	{
		//inventory
		double damageOut=0, defenseOut=0;
		//get item data from its position
		itemSys.getData(itemSys.findItem(inventory[slot])).copyData(damageOut, defenseOut);
		//add data to vars
		damage+=damageOut;
		defense+=defenseOut;
	}
}
void playerClass::deequip(int slot, int loc)
{
	//remove bonuses and stats from item in loc
	if(loc==0)
	{
		//equipmentList
		double damageOut=0, defenseOut=0;
		//get item data from its position
		itemSys.getData(itemSys.findItem(equipmentList[slot])).copyData(damageOut, defenseOut);
		//subtract data from vars
		damage-=damageOut;
		defense-=defenseOut;
	}
	else if(loc==1)
	{
		//inventory
		double damageOut=0, defenseOut=0;
		//get item data from its position
		itemSys.getData(itemSys.findItem(inventory[slot])).copyData(damageOut, defenseOut);
		//subtract data from vars
		damage-=damageOut;
		defense-=defenseOut;
	}
}
bool playerClass::checkReqs(int slot, int loc)
{
	//check reqs for item in loc
	//needs to be made
	return true;
}
void playerClass::moveInventory(int location1, int location2)
{
	//don't think this is used
	//move slot1 to slot2, swap if needed, combine if possible
	if(inventory[location2]!=-1)
	{
		//item in slot 2
		if(inventory[location1]==inventory[location2])
		{
			//same id, combineable
		}
		else
		{
			//swap
			int holder=inventory[location2];
			inventory[location2]=inventory[location1];
			inventory[location1]=holder;
		}
	}
	else
	{
		//no item in slot 2 just move
		inventory[location2]=inventory[location1];
		inventory[location1]=-1;
	}
} 

//builds player class from given file
void playerClass::loadFromFile(stringc dataFile)
{
	//open file from data file string
	ifstream playerFile;
	playerFile.open(dataFile.c_str());
	if(playerFile.is_open())
	{
		//if file is open load its data link below for var list
		//http://rofhonline.com/forums/viewtopic.php?f=8&t=205
		std::string infromfile;
		getline(playerFile, infromfile);
		id=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		password.append(infromfile.c_str());
		getline(playerFile, infromfile);
		email.append(infromfile.c_str());
		getline(playerFile, infromfile);
		userName.append(infromfile.c_str());
		getline(playerFile, infromfile);
		inGameName.append(infromfile.c_str());
		getline(playerFile, infromfile);
		title.append(infromfile.c_str());
		getline(playerFile, infromfile);
		clan=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		str=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		dex=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		con=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		intel=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		level=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		pvp=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		expOn=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		expTot=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		skillLevel=atoi(infromfile.c_str());
		int skillCount=0;
		getline(playerFile, infromfile);
		skillCount=atoi(infromfile.c_str());
		skillList.resize(skillCount);
		for(int x=0;x<skillCount;x++)
		{
				getline(playerFile, infromfile);
				skillList[x,0]=atoi(infromfile.c_str());
				getline(playerFile, infromfile);
				skillList[x,1]=atoi(infromfile.c_str());
		}
		int eqCount=0;
		getline(playerFile, infromfile);
		eqCount=atoi(infromfile.c_str());
		for(int x=0;x<eqCount;x++)
		{
			int eqPos=0;
			getline(playerFile, infromfile);
			eqPos=atoi(infromfile.c_str());
			getline(playerFile, infromfile);
			equipmentList[eqPos]=atoi(infromfile.c_str());
		}
		int invCount=0;
		getline(playerFile, infromfile);
		invCount=atoi(infromfile.c_str());
		for(int x=0;x<invCount;x++)
		{
			int invPos=0;
			getline(playerFile, infromfile);
			invPos=atoi(infromfile.c_str());
			getline(playerFile, infromfile);
			inventory[invPos]=atoi(infromfile.c_str());
		}
		getline(playerFile, infromfile);
		mountId=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		raceId=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		modelId=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		showeq=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		visibility=atoi(infromfile.c_str());;
		getline(playerFile, infromfile);
		speed=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		mapx=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		mapy=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		mapz=atoi(infromfile.c_str());
		getline(playerFile, infromfile);
		posx=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		posy=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		posz=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		curhp=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		maxhp=atof(infromfile.c_str());
		getline(playerFile, infromfile);
		int factionTot=atoi(infromfile.c_str());
		for(int x=0;x<factionTot;x++)
		{
			getline(playerFile, infromfile);
			faction[0].push_back(atoi(infromfile.c_str()));
			getline(playerFile, infromfile);
			faction[1].push_back(atoi(infromfile.c_str()));
		}
		//set jump to false and loaded to true
		jump=false;
		loaded=true;
	}
}

//send start data to client code 13
void playerClass::sendStart(int sock)
{
	//send player info, you can read the vars below i think
	//this is sent to the player, loads all of their data into the client
	//NOT used to send to other players use sendPlayer for that
	stringc sendData;
	sendData="13|";
	sendData.append(inGameName.c_str());
	sendData.append('|');
	sendData.append(title);
	sendData.append('|');
	char thebuffer[100];
	sendData.append(itoa(id,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mapx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mapy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mapz,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(posx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(posy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(posz,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(level,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(intel,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(con,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(dex,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(str,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(rotation,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(raceId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mountId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(modelId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(expOn,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(expTot,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(skillLevel,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(unusedSkillPoints,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(pvp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(showeq,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(visibility,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(criminal,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(maxhp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(curhp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mounted,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(speed,thebuffer,10));
	sendData.append('|');
	addMessageHeader(sendData);
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
	//skill list

	//equipment and inventory list
	sendInventory(sock);
}

//send inventory data to client code 25 and 26
void playerClass::sendInventory(int sock)
{
	//send inventory and equipment
	stringc sendData;
	sendData="25|";
	//loop through equipment
	for(int x=0;x<19;x++)
	{
		//add equipment to sendData
		char item[400];
		sprintf(item,"%i|", equipmentList[x]);
		cout <<"e" << equipmentList[x]<< endl;
		sendData.append(item);
	}
	//add header
	addMessageHeader(sendData);
	//send equipment
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);

	//set to inventory
	sendData="26|";
	//loop through inventory
	for(int x=0;x<40;x++)
	{
		//add inventory to sendData
		char item[800];
		sprintf(item,"%i|", inventory[x]);
		cout <<"i" << inventory[x]<< endl;
		sendData.append(item);
	}
	//add header
	addMessageHeader(sendData);
	//send inventory
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

//send player data to client code 27
void playerClass::sendPlayer(int sock, int from)
{
	//send data to clients
	//adds them as another char
	printf("@sendPlayer %i to %i @ ", from, sock);
	stringc sendData;
	sendData="27|";
	sendData.append(inGameName.c_str());
	sendData.append('|');
	sendData.append(title);
	sendData.append('|');
	char thebuffer[100];
	sendData.append(itoa(id,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mapx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mapy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mapz,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(posx,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(posy,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(posz,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(level,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(rotation,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(raceId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mountId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(modelId,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(pvp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(showeq,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(visibility,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(criminal,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(maxhp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(curhp,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(mounted,thebuffer,10));
	sendData.append('|');
	sendData.append(itoa(speed,thebuffer,10));
	sendData.append('|');
	//add header
	addMessageHeader(sendData);
	//send data
	send(clientsock[sock], sendData.c_str(), sendData.size(),0);
}

int playerClass::savePlayer()
{
	//saves player data
	printf("opening file ");
	ofstream playerFileS;
	//get file loc
	stringc filetoopen="database/players/";
	filetoopen.append(userName);
	playerFileS.open(filetoopen.c_str());
	//open file
	if(playerFileS.is_open())
	{
		//if file is open start saving vars on each line
		printf("saving ");
		playerFileS << id;
		playerFileS << '\n';
		playerFileS << password.c_str();
		playerFileS << '\n';
		playerFileS << email.c_str();
		playerFileS << '\n';
		playerFileS << userName.c_str();
		playerFileS << '\n';
		playerFileS << inGameName.c_str();
		playerFileS << '\n';
		playerFileS << title.c_str();
		playerFileS << '\n';
		playerFileS << clan;
		playerFileS << '\n';
		playerFileS << str;
		playerFileS << '\n';
		playerFileS << dex;
		playerFileS << '\n';
		playerFileS << con;
		playerFileS << '\n';
		playerFileS << intel;
		playerFileS << '\n';
		playerFileS << level;
		playerFileS << '\n';
		playerFileS << pvp;
		playerFileS << '\n';
		playerFileS << expOn;
		playerFileS << '\n';
		playerFileS << expTot;
		playerFileS << '\n';
		playerFileS << skillLevel;
		playerFileS << '\n';
		playerFileS << skillList.size();
		playerFileS << '\n';
		for(int x=0;x<skillList.size();x++)
		{
			playerFileS << skillList[x,0];
			playerFileS << '\n';
			playerFileS << skillList[x,1];
			playerFileS << '\n';
		}
		int eqCount=19;
		playerFileS << eqCount;
		playerFileS << '\n';
		for(int x=0;x<eqCount;x++)
		{
			int eqPos=x;
			playerFileS << eqPos;
			playerFileS << '\n';
			playerFileS << equipmentList[eqPos];
			playerFileS << '\n';
		}
		int invCount=40;
		playerFileS << invCount;
		playerFileS << '\n';
		for(int x=0;x<invCount;x++)
		{
			int invPos=x;
			playerFileS << invPos;
			playerFileS << '\n';
			playerFileS << inventory[invPos];
			playerFileS << '\n';
		}
		playerFileS << mountId;
		playerFileS << '\n';
		playerFileS << raceId;
		playerFileS << '\n';
		playerFileS << modelId;
		playerFileS << '\n';
		playerFileS << showeq;
		playerFileS << '\n';
		playerFileS << visibility;
		playerFileS << '\n';
		playerFileS << speed;
		playerFileS << '\n';
		playerFileS << mapx;
		playerFileS << '\n';
		playerFileS << mapy;
		playerFileS << '\n';
		playerFileS << mapz;
		playerFileS << '\n';
		playerFileS << posx;
		playerFileS << '\n';
		playerFileS << posy;
		playerFileS << '\n';
		playerFileS << posz;
		playerFileS << '\n';
		playerFileS << curhp;
		playerFileS << '\n';
		playerFileS << maxhp;
	}
}
