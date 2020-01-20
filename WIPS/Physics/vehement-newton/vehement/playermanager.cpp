// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_PLAYERMANAGER_CPP

// Include the class definition
#include "playermanager.hpp"

// Reference the factory class
#include "factory.hpp"
class dfactory;

// Using the vehe namespace
namespace vehe 
{
	using namespace ctypes;

	//Initializer
	playermanager::playermanager ()	
	{
		// Store device pointers for use
		device = dfactory::instance()->getIrrDevice();
		
		guienv = dfactory::instance()->getIrrDevice()->getGUIEnvironment();
		
		driver = dfactory::instance()->getIrrDevice()->getVideoDriver();

		smgr  = dfactory::instance()->getIrrDevice()->getSceneManager();

		audio =  dfactory::instance()->getSoundDevice();
	};

	//Set up a basic new user
	void playermanager::newPlayer() 
	{
		//Setup a new player
		player newplayer;
		strcpy(newplayer.name ,"Player1");
		strcpy(newplayer.currentmap, "map1.cobj");
		newplayer.level = 1;
		newplayer.health = 100;
		newplayer.power = 100;
		newplayer.position = vector3df(3000,300,3000);
		newplayer.rotation = vector3df(0,0,0);
		myplayer = newplayer;

		//Save this player
		playermanager::savePlayer("./data/saves/player1.cobj");

		//Wipe out map states
		cout << _rmdir("./data/states/") << endl;
		_mkdir("./data/states");

		//Load the current map
		dfactory::instance()->loadMap(myplayer.currentmap);
		//dfactory::instance()->newMap(myplayer.currentmap);

		//Make a sword item, add it to the inventory
		item newitem;
		strcpy(newitem.name ,"sword");
		strcpy(newitem.model ,"./data/graphics/models/weapons/sword.3ds");
		strcpy(newitem.gfx ,"./data/graphics/models/weapons/smach.jpg");
		newitem.scale = vector3df(1,1,1);
		newitem.rotation = vector3df(-65,-50,-15);
		newitem.location = vector3df(20,-15,45);
		inventory.push_back(newitem);
	};

	//Update the player manager
	void playermanager::updatePlayerManager()
	{
		dfactory * myfactory = dfactory::instance();
		if (!myfactory->getIsPaused())
		{	
			//Reset the player location if they have fallen off the map
			playermanager::isInGameArea();

			//Update the player structure
			player newplayer = myplayer;
			newplayer.position = myfactory->getCameraNode()->getPosition();
			newplayer.rotation = myfactory->getCameraNode()->getRotation();
			myplayer = newplayer;
		}
	};
	
	//Check if the player has fallen off of the map
	void playermanager::isInGameArea()
	{
		dfactory * myfactory = dfactory::instance();
		if (myfactory->getCameraNode()->getPosition().Y < 0)
			myfactory->getCameraNode()->setPosition(myfactory->getMapObject().playerstart);
	}
	
	//Load a player from an XML file
	void playermanager::loadPlayer (char * filename)
	{
		//Temporary
		irr::core::string <char> mystring, x, y, z;
		player newplayer;

		// read configuration from xml file
		io::IXMLReader* xml =
			device->getFileSystem()->createXMLReader(filename);

			while(xml && xml->read())
			{
				switch(xml->getNodeType())
				{
					case io::EXN_ELEMENT:
						if (core::stringw("flag") == xml->getNodeName())
						{
							if(core::stringw("name") == xml->getAttributeName(0))
							{
								mystring = xml->getAttributeValue(L"name");
								strcpy(newplayer.name, mystring.c_str());	
							}
							else if(core::stringw("currentmap") == xml->getAttributeName(0))
							{
								mystring = xml->getAttributeValue(L"currentmap");
								strcpy(newplayer.currentmap, mystring.c_str());	
							}
							else if(core::stringw("level") == xml->getAttributeName(0))
							{
								mystring = xml->getAttributeValue(L"level");
								newplayer.level = atoi(mystring.c_str());	
							}
							else if(core::stringw("health") == xml->getAttributeName(0))
							{
								mystring = xml->getAttributeValue(L"health");
								newplayer.health = atoi(mystring.c_str());	
							}
							else if(core::stringw("power") == xml->getAttributeName(0))
							{
								mystring = xml->getAttributeValue(L"power");
								newplayer.power = atoi(mystring.c_str());	
							}
						}
						else if (core::stringw("position") == xml->getNodeName())
						{
							x = xml->getAttributeValue(L"X");
							y = xml->getAttributeValue(L"Y");
							z = xml->getAttributeValue(L"Z");
							newplayer.position = vector3df((float)atoi(x.c_str()),
										(float)atoi(y.c_str()),
										(float)atoi(z.c_str()));
						}
						else if (core::stringw("rotation") == xml->getNodeName())
						{
							x = xml->getAttributeValue(L"X");
							y = xml->getAttributeValue(L"Y");
							z = xml->getAttributeValue(L"Z");
							newplayer.rotation = vector3df((float)atoi(x.c_str()),
															(float)atoi(y.c_str()),
															(float)atoi(z.c_str()));
						}
					break;
				};
			}
			if (xml)
			xml->drop(); // don't forget to delete the xml reader 

			//Set the current player stuctue
			myplayer = newplayer;

			//Load the current map
			dfactory::instance()->loadMap(myplayer.currentmap);
	}

	//Save a player to an xml file
	void playermanager::savePlayer(char * filename)
	{
		//Temporary
		irr::core::string <wchar_t> mystring, x, y, z;

		io::IXMLWriter* xml =
		device->getFileSystem()->createXMLWriter(filename);
		
		xml->writeXMLHeader();
		xml->writeComment(L"This is a player file for Confined");
		xml->writeLineBreak();

		xml->writeElement(L"config",false);
		xml->writeLineBreak();

		//Strings	
		mystring = myplayer.name;
		xml->writeElement(L"flag",true, L"name", mystring.c_str() );
		xml->writeLineBreak();
		mystring = myplayer.currentmap;
		xml->writeElement(L"flag",true, L"currentmap", mystring.c_str() );
		xml->writeLineBreak();
		
		//integers
		mystring = myplayer.level;
		xml->writeElement(L"flag",true, L"level", mystring.c_str() );
		xml->writeLineBreak();
		mystring = myplayer.health;
		xml->writeElement(L"flag",true, L"health", mystring.c_str() );
		xml->writeLineBreak();
		mystring = myplayer.power;
		xml->writeElement(L"flag",true, L"power", mystring.c_str() );
		xml->writeLineBreak();

		//positions
		x = myplayer.position.X;
		y = myplayer.position.Y;
		z = myplayer.position.Z;
		xml->writeElement(L"position",true, L"X", x.c_str(), L"Y", y.c_str(), L"Z", z.c_str());
		xml->writeLineBreak();

		x = myplayer.rotation.X;
		y = myplayer.rotation.Y;
		z = myplayer.rotation.Z;
		xml->writeElement(L"rotation",true, L"X", x.c_str(), L"Y", y.c_str(), L"Z", z.c_str());
		xml->writeLineBreak();

		xml->writeClosingTag(L"config");
		xml->writeLineBreak();

		if (xml)
			xml->drop(); 

		//Save the state of this map
		irr::core::string <char> mystring2 = "./data/states/";
		mystring2.append(dfactory::instance()->getMapObject().file);
		mystring2.append(".state");

		dfactory::instance()->saveMapState((char *)mystring2.c_str());
	}
	

	
};