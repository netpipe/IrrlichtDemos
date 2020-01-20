// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_FACTORY_MAP_CPP

// Inlude the class definition
#include "factory.hpp"

// Using the vehe namespace
namespace vehe 
{
	//! Create the terrain node
	void dfactory::createTerrain (mapobj data)
	{
		//Load compressed graphics
		device->getFileSystem()->addZipFileArchive(data.gfx);

		terrain = smgr->addTerrainSceneNode( 
				data.heightmap,
				0,										// parent node
				-1,										// node id
				vector3df(0,0,0),						// position
				vector3df(0,0,0),						// rotation
				vector3df(40,4.0f,40),					// scale
				video::SColor ( 0, 255, 255,255 ),		// vertexColor,
				5,										// maxLOD
				scene::ETPS_17,							// patchSize
				4										// smoothFactor
				);
			terrain->setMaterialFlag(video::EMF_LIGHTING, false);
			terrain->setMaterialTexture(0, driver->getTexture(data.texture));
			terrain->setMaterialTexture(1, driver->getTexture(data.detail));
			terrain->setMaterialType(video::EMT_DETAIL_MAP);
			terrain->scaleTexture(35.0f, 35.0f);
			terrain->setMaterialFlag(EMF_FOG_ENABLE, true);

			//Triangle Selector Code
			ITriangleSelector* selector =
			smgr->createTerrainTriangleSelector(terrain, 0);
			terrain->setTriangleSelector(selector);
			meta->addTriangleSelector(selector);
			selector->drop();

			//Physics
			eterrain.bData.Node=terrain;
			eterrain.bData.TerrainLOD = 3;
			eterrain.body = p_world->createBody(eterrain.bData);
			eterrain.material = p_world->createMaterial();
			eterrain.body->setMaterial(eterrain.material);
	
	}

	//! New Map
	void dfactory::newMap(char * filename)
	{
		//Unload things from memory
		smgr->clear();
		meta->removeAllTriangleSelectors();
		if(!firstload)
		{
			p_world->destroyWorld();
			firstload = false;
		}
		entities.clear();
		objects.clear();
		p_world = newton::createPhysicsWorld(device);

		//Fill a terrain structure
		mapobj newmap;
		strcpy(newmap.file,"map1.cobj");
		strcpy(newmap.heightmap, "terrain2.bmp");
		strcpy(newmap.texture, "texture.jpg");
		strcpy(newmap.detail, "grass.jpg");
		strcpy(newmap.gfx, "./data/graphics/map.dat");
		newmap.playerstart = vector3df(3000,800,3000);
		newmap.iobj = 0;

		/* Temporary new object details */
		object myobject;
		strcpy(myobject.name, "chapel");
		strcpy(myobject.model, "./data/graphics/models/chapel/kosciol.3ds");
		strcpy(myobject.gfx, "./data/graphics/models/chapel/kosciol.jpg");
		strcpy(myobject.script, "./data/scripts/objects/test.sc");
		myobject.location = vector3df(5398,425,1903);
		myobject.rotation = vector3df(0,200,0);
		myobject.scale = vector3df(75,45,75);
		myobject.isStatic = true;
		myobject.primitive = false;
		myobject.texture = true;
		myobject.alpha = false;
		myobject.scripted = false;
		myobject.vegetation = false;
		myobject.weight = 0;
		objects.push_back(myobject);

		//Update the map object structure
		newmap.iobj = objects.size();
		currentmap = newmap;

		//Create a terrain node
		dfactory::createTerrain(currentmap);

		//Create all the maps objects
		dfactory::populateObjects();

		//Create the camera and skydome and bill board
		dfactory::createSkyDome();	
		dfactory::createBillBoard();	
		dfactory::createCamera(currentmap.playerstart);
		
		//Save the map
		irr::core::string <char> saveto = "./data/maps/";
		saveto.append(filename);
		dfactory::instance()->saveMap((char *) saveto.c_str());
	}

	// Load each object in the object array
	void dfactory::populateObjects() 
	{
		for (unsigned int e=0;e<objects.size();e++)
		{
				//Load package of textures if needed
				if (!objects.at(e).texture)
					device->getFileSystem()->addZipFileArchive(objects.at(e).gfx);
							
				//Load the model
				entity  newent; 
				IAnimatedMesh *mymesh = smgr->getMesh(objects.at(e).model);

				newent.mynode =  smgr->addAnimatedMeshSceneNode(mymesh,0,-1,
				objects.at(e).location,objects.at(e).rotation, objects.at(e).scale);
				newent.mynode->setID((s32)e);

				//Alpha Code if needed
				if (objects.at(e).alpha) 
				{
					 ITexture * image  = driver->getTexture(objects.at(e).gfx);
					 driver->makeColorKeyTexture(image, SColor(0,0,0,0));
					 newent.mynode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
					 newent.mynode->setMaterialTexture(0, image); 
				}
				//No alpha? Single Texture if needed
				else if (objects.at(e).texture)
					newent.mynode->setMaterialTexture( 0, driver->getTexture(objects.at(e).gfx));

				//Transfer settings
				newent.isStatic = objects.at(e).isStatic;
				newent.primitive = objects.at(e).primitive;
				newent.weight = objects.at(e).weight;

				//If it is not vegetetation, add the node to the physics world
				if (!objects.at(e).vegetation)
				{
					//Triangle Selector Code
					ITriangleSelector *selector = smgr->createTriangleSelector(mymesh->getMesh(0),newent.mynode);
					meta->addTriangleSelector(selector);
					selector->drop();
					newent.Start(p_world);
				}

				//Push onto the stack
				entities.push_back(newent);

				//Turn off lighting add fog
				entities.at(e).mynode->setMaterialFlag(EMF_FOG_ENABLE, true);
				entities.at(e).mynode->setMaterialFlag(EMF_LIGHTING, false);
						
		}
	}

	// Create the entity for the last object in the array
	void dfactory::buildObject(object newobject) 
	{
		//Load package of textures if needed
		if (!newobject.texture)		
			device->getFileSystem()->addZipFileArchive(newobject.gfx);
						
		//Load the model
		entity  newent; 
		IAnimatedMesh *mymesh = smgr->getMesh(newobject.model);

		newent.mynode =  smgr->addAnimatedMeshSceneNode(mymesh,0,-1,
		newobject.location,newobject.rotation, newobject.scale);
		newent.mynode->setID((s32)objects.size());
		
		//Alpha Code if needed
		if (newobject.alpha) 
		{
			 ITexture * image  = driver->getTexture(newobject.gfx);
			 driver->makeColorKeyTexture(image, SColor(0,0,0,0));
			 newent.mynode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			 newent.mynode->setMaterialTexture(0, image); 
		}
		//No alpha? Single Texture if needed
		else if (newobject.texture)
			newent.mynode->setMaterialTexture( 0, driver->getTexture(newobject.gfx));

		//Transfer settings
		newent.isStatic = newobject.isStatic;
		newent.primitive = newobject.primitive;
		newent.weight = newobject.weight;

		//If it is not vegetetation, add the node to the physics world
		if (!newobject.vegetation)
		{
			//Triangle Selector Code
			newent.selector = smgr->createTriangleSelector(mymesh->getMesh(0),newent.mynode);
			meta->addTriangleSelector(newent.selector);
			newent.Start(p_world);
		}

		//Push onto the stack
		objects.push_back(newobject);
		entities.push_back(newent);

		//Turn off lighting add fog
		entities.at(entities.size()-1).mynode->setMaterialFlag(EMF_FOG_ENABLE, true);
		entities.at(entities.size()-1).mynode->setMaterialFlag(EMF_LIGHTING, false);
	}

	//! Update an objects entity
	void dfactory::updateObject(object newobj, int myid)
	{
		if (myid != 666 && myid != -1)
		{
			dfactory::deleteObject(myid);
			dfactory::buildObject(newobj);
		}
	}

	//! Move an object
	void dfactory::moveObject(int myid)
	{
		if (myid != 666 && myid != -1)
		{
			dfactory::saveMap("./data/maps/backup.cobj");
			objects.at(myid).location = camera->getAbsolutePosition();
			objects.at(myid).location.Z = objects.at(myid).location.Z + 100;
			objects.at(myid).rotation = camera->getRotation();
			
			//Reset all the node ID's
			for (unsigned int i = 0; i < objects.size(); i++)
					entities.at(i).mynode->setID((s32)i);

			if (!objects.at(myid).vegetation)
			{
				entities.at(myid).body->setPosition(objects.at(myid).location);
				entities.at(myid).body->setRotation(objects.at(myid).rotation);
			}
			else
			{
				entities.at(myid).mynode->setPosition(objects.at(myid).location);
				entities.at(myid).mynode->setRotation(objects.at(myid).rotation);
			}

			//Clear the editor's marker
			editor->setSelectedEditorNode(666);
			dfactory::instance()->getBillBoardNode()->setVisible(false);

			irr::core::string <char> saveto = "./data/maps/";
			saveto.append(currentmap.file);
			dfactory::instance()->saveMap((char *) saveto.c_str());
		}
	}

	//! Clone an objects entity
	void dfactory::cloneObject( int myid)
	{
		if (myid != 666 && myid != -1)
		{
			dfactory::saveMap("./data/maps/backup.cobj");
			object newobj = objects.at(myid);
			newobj.location = camera->getAbsolutePosition();
			newobj.location.Z = newobj.location.Z + 100;
			dfactory::buildObject(newobj);
			
			//Clear the editor's marker
			editor->setSelectedEditorNode(666);
			dfactory::instance()->getBillBoardNode()->setVisible(false);

			//Reset all the node ID's
			for (unsigned int i = 0; i < objects.size(); i++)
					entities.at(i).mynode->setID((s32)i);

			irr::core::string <char> saveto = "./data/maps/";
			saveto.append(currentmap.file);
			dfactory::instance()->saveMap((char *) saveto.c_str());
		}
	}
	
	//! Delete a object entity and refresh their ID's
	void dfactory::deleteObject(int myid)
	{
		if (myid != 666 && myid != -1)
		{
				//Erase the object from the array, collision manager, physics and memory
				dfactory::saveMap("./data/maps/backup.cobj");

				if (!objects.at(myid).vegetation)
					meta->removeTriangleSelector(entities.at(myid).selector);

				if (!objects.at(myid).vegetation)
					entities.at(myid).body->remove();
				else
					entities.at(myid).mynode->remove();
				
				objects.erase(objects.begin()+myid);
				entities.erase(entities.begin()+myid);
				
				//Clear the editor's marker
				editor->setSelectedEditorNode(666);
				dfactory::instance()->getBillBoardNode()->setVisible(false);

				//Reset all the node ID's
				for (unsigned int i = 0; i < objects.size(); i++)
					entities.at(i).mynode->setID((s32)i);
				
				irr::core::string <char> saveto = "./data/maps/";
				saveto.append(currentmap.file);
				dfactory::instance()->saveMap((char *) saveto.c_str());
		}
	}

		

	//Load a map from a file
	void dfactory::loadMap(char * filename)
	{
		//Temporary
		irr::core::string <char> mystring, x, y, z;
		mapobj newmap; object newobject;

		//Unload things from memory
		smgr->clear();
		meta->removeAllTriangleSelectors();
		if(!firstload)
		{
			p_world->destroyWorld();
			firstload = false;
		}
		entities.clear();
		objects.clear();
		p_world = newton::createPhysicsWorld(device);

		// read configuration from xml file
		
		mystring = "./data/maps/";
		mystring.append(filename);
		io::IXMLReader* xml =
			device->getFileSystem()->createXMLReader((char*)mystring.c_str());

		while(xml && xml->read())
		{
			switch(xml->getNodeType())
			{
			case io::EXN_ELEMENT_END:
				{
					if (core::stringw("config") == xml->getNodeName())		
						currentmap = newmap;
					else if (core::stringw("object") == xml->getNodeName())
						objects.push_back(newobject);
					break;
				}
			case io::EXN_ELEMENT:
				{
					if (core::stringw("map") == xml->getNodeName())
					{
						if(core::stringw("file") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(L"file");
							strcpy(newmap.file, mystring.c_str());	
						}
						else if (core::stringw("gfx") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(L"gfx");
							strcpy(newmap.gfx , mystring.c_str());	
						}
						else if (core::stringw("heightmap") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(L"heightmap");
							strcpy(newmap.heightmap , mystring.c_str());	
						}
						else if (core::stringw("detail") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(L"detail");
							strcpy(newmap.detail , mystring.c_str());	
						}
						else if (core::stringw("texture") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(L"texture");
							strcpy(newmap.texture , mystring.c_str());	
						}
						else if (core::stringw("iobj") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(L"iobj");
							newmap.iobj = atoi(mystring.c_str());	
						}
						else if (core::stringw("X") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							newmap.playerstart.X = (float)atoi(mystring.c_str());	
							mystring = xml->getAttributeValue(1);
							newmap.playerstart.Y = (float)atoi(mystring.c_str());	
							mystring = xml->getAttributeValue(2);
							newmap.playerstart.Z = (float)atoi(mystring.c_str());	
						}
					}
					else if (core::stringw("object") == xml->getNodeName())
					{
						object myobject;
						newobject = myobject;
					}		
					else if (core::stringw("flag") == xml->getNodeName())
					{
						if(core::stringw("name") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							strcpy(newobject.name , mystring.c_str());	
						}
						else if(core::stringw("model") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							strcpy(newobject.model , mystring.c_str());	
						}
						else if(core::stringw("gfx") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							strcpy(newobject.gfx, mystring.c_str());	
						}
						else if(core::stringw("script") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							strcpy(newobject.script , mystring.c_str());	
						}
						else if(core::stringw("vegetation") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							if (atoi(mystring.c_str()) == 0)
								newobject.vegetation = false;
							else
								newobject.vegetation = true;
						}
						else if(core::stringw("isStatic") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							if (atoi(mystring.c_str()) == 0)
								newobject.isStatic = false;
							else
								newobject.isStatic = true;
						}
						else if(core::stringw("primitive") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							if (atoi(mystring.c_str()) == 0)
								newobject.primitive= false;
							else
								newobject.primitive = true;
						}
						else if(core::stringw("texture") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							if (atoi(mystring.c_str()) == 0)
								newobject.texture = false;
							else
								newobject.texture = true;
						}
						else if(core::stringw("alpha") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							if (atoi(mystring.c_str()) == 0)
								newobject.alpha = false;
							else
								newobject.alpha = true;
						}
						else if(core::stringw("scripted") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							if (atoi(mystring.c_str()) == 0)
								newobject.scripted = false;
							else
								newobject.scripted = true;
						}
						else if(core::stringw("weight") == xml->getAttributeName(0))
						{
							mystring = xml->getAttributeValue(0);
							newobject.weight = atoi(mystring.c_str());
						}
					}
					else if (core::stringw("location") == xml->getNodeName())
					{
							mystring = xml->getAttributeValue(0);
							newobject.location.X = (float)atoi(mystring.c_str());
							mystring = xml->getAttributeValue(1);
							newobject.location.Y = (float)atoi(mystring.c_str());
							mystring = xml->getAttributeValue(2);
							newobject.location.Z = (float)atoi(mystring.c_str());
					}
					else if (core::stringw("rotation") == xml->getNodeName())
					{
							mystring = xml->getAttributeValue(0);
							newobject.rotation.X = (float)atoi(mystring.c_str());
							mystring = xml->getAttributeValue(1);
							newobject.rotation.Y = (float)atoi(mystring.c_str());
							mystring = xml->getAttributeValue(2);
							newobject.rotation.Z = (float)atoi(mystring.c_str());
					}
					else if (core::stringw("scale") == xml->getNodeName())
					{
							mystring = xml->getAttributeValue(0);
							newobject.scale.X = (float)atoi(mystring.c_str());
							mystring = xml->getAttributeValue(1);
							newobject.scale.Y = (float)atoi(mystring.c_str());
							mystring = xml->getAttributeValue(2);
							newobject.scale.Z = (float)atoi(mystring.c_str());
					}	
				}	
					
			break;
			}
		}
		if (xml)
			xml->drop(); // don't forget to delete the xml reader 

		//Create a terrain node
		dfactory::createTerrain(currentmap);

		//Create all the maps objects
		dfactory::populateObjects();

		//Load this games state if it exists
		irr::core::string <char> mystring2 = "./data/states/";
		mystring2.append(currentmap.file);
		mystring2.append(".state");
		if (device->getFileSystem()->existFile(mystring2.c_str()))
			dfactory::loadMapState((char *)mystring2.c_str());
		else
			dfactory::saveMapState((char *)mystring2.c_str());

		//Create the camera and skydome
		dfactory::createSkyDome();	
		dfactory::createBillBoard();	

		dfactory::createCamera(dfactory::getPlayerManager()->getPlayerObject().position);

	}
	
	//Save the current map to a file
	void dfactory::saveMap(char * filename)
	{
		currentmap.iobj = objects.size();

		//Temporary
		irr::core::string <wchar_t> mystring, x, y, z;

		io::IXMLWriter* xml =
			device->getFileSystem()->createXMLWriter(filename);
		
		xml->writeXMLHeader();
		xml->writeComment(L"This is a map file for Confined");
		xml->writeLineBreak();

		xml->writeElement(L"config",false);
		xml->writeLineBreak();

		mystring = currentmap.file;
		xml->writeElement(L"map",true, L"file", mystring.c_str());
		xml->writeLineBreak();

		mystring = currentmap.gfx;
		xml->writeElement(L"map",true, L"gfx", mystring.c_str());
		xml->writeLineBreak();

		mystring = currentmap.heightmap;
		xml->writeElement(L"map",true, L"heightmap", mystring.c_str());
		xml->writeLineBreak();

		mystring = currentmap.detail;
		xml->writeElement(L"map",true, L"detail", mystring.c_str());
		xml->writeLineBreak();
	
		mystring = currentmap.texture;
		xml->writeElement(L"map",true, L"texture", mystring.c_str());
		xml->writeLineBreak();

		mystring = currentmap.iobj;
		xml->writeElement(L"map",true, L"iobj", mystring.c_str());
		xml->writeLineBreak();

		x = currentmap.playerstart.X;
		y = currentmap.playerstart.Y;
		z = currentmap.playerstart.Z;
		xml->writeElement(L"map",true, L"X", x.c_str(), L"Y", y.c_str(), L"Z", z.c_str());
		xml->writeLineBreak();
		xml->writeClosingTag(L"config");
		xml->writeLineBreak();

		xml->writeElement(L"objects",false);
		xml->writeLineBreak();


		for (int i = 0; i < currentmap.iobj; i++)
		{
			object myobject = objects.at(i);
			
			mystring = i;
			xml->writeElement(L"object",false, L"id", mystring.c_str());
			xml->writeLineBreak();

			//Strings	
			mystring = myobject.name;
			xml->writeElement(L"flag",true, L"name", mystring.c_str() );
			xml->writeLineBreak();
			mystring = myobject.model;
			xml->writeElement(L"flag",true, L"model", mystring.c_str() );
			xml->writeLineBreak();
			mystring = myobject.gfx;
			xml->writeElement(L"flag",true, L"gfx", mystring.c_str() );
			xml->writeLineBreak();
			mystring = myobject.script;
			xml->writeElement(L"flag",true, L"script", mystring.c_str() );
			xml->writeLineBreak();
			xml->writeLineBreak();

			//flags
			mystring = myobject.vegetation;
			xml->writeElement(L"flag",true, L"vegetation", mystring.c_str() );
			xml->writeLineBreak();
			mystring = myobject.isStatic;
			xml->writeElement(L"flag",true, L"isStatic", mystring.c_str() );
			xml->writeLineBreak();
			mystring = myobject.primitive;
			xml->writeElement(L"flag",true, L"primitive", mystring.c_str() );
			xml->writeLineBreak();
			mystring = myobject.texture;
			xml->writeElement(L"flag",true, L"texture", mystring.c_str() );
			xml->writeLineBreak();
			mystring = myobject.alpha;
			xml->writeElement(L"flag",true, L"alpha", mystring.c_str() );
			xml->writeLineBreak();
			mystring = myobject.scripted;
			xml->writeElement(L"flag",true, L"scripted", mystring.c_str() );
			xml->writeLineBreak();
			xml->writeLineBreak();

			//integers
			mystring = myobject.weight;
			xml->writeElement(L"flag",true, L"weight", mystring.c_str() );
			xml->writeLineBreak();
			xml->writeLineBreak();

			//positions
			x = myobject.location.X;
			y = myobject.location.Y;
			z = myobject.location.Z;
			xml->writeElement(L"location", true, L"x", x.c_str(), L"y", y.c_str(), L"z", z.c_str());
			xml->writeLineBreak();

			x = myobject.rotation.X;
			y = myobject.rotation.Y;
			z = myobject.rotation.Z;
			xml->writeElement(L"rotation", true, L"x", x.c_str(), L"y", y.c_str(), L"z", z.c_str());
			xml->writeLineBreak();

			x = myobject.scale.X;
			y = myobject.scale.Y;
			z = myobject.scale.Z;
			xml->writeElement(L"scale", true, L"x", x.c_str(), L"y", y.c_str(), L"z", z.c_str());
			xml->writeLineBreak();

			xml->writeClosingTag(L"object");
			xml->writeLineBreak();

		}

		xml->writeLineBreak();
		xml->writeClosingTag(L"objects");
		
		if (xml)
			xml->drop(); 

		
		//Delete this maps state because it has been changed
		irr::core::string <char> mystring2 = "./data/states/";
		mystring2.append(currentmap.file);
		mystring2.append(".state");
		remove(mystring2.c_str());

	}

	//! Load the map state
	void dfactory::loadMapState(char *filename)
	{
		objectstate.clear();
		currentmap.iobj = objects.size();

		fstream iofile(filename, ios::binary | ios::in );
		for (int i=0;i<currentmap.iobj;i++){
			cpos newpos;
			iofile.read(reinterpret_cast<char *>(&newpos),sizeof(newpos));
			objectstate.push_back(newpos);
		}
		iofile.close();

		for (int x=0;x<currentmap.iobj;x++){
			entities.at(x).body->setPosition(objectstate.at(x).location);
			entities.at(x).body->setRotation(objectstate.at(x).rotation);
		}
		objectstate.clear();
	}

	//! Save the map state
	void dfactory::saveMapState(char *filename)
	{
		objectstate.clear();
		currentmap.iobj = objects.size();

		for (unsigned int i=0;i<(objects.size());i++){
			cpos savedstate;
			savedstate.location = entities.at(i).body->getPosition();
			savedstate.rotation = entities.at(i).body->getRotationBody();
			objectstate.push_back(savedstate);
		}

		fstream iofile(filename, ios::binary | ios::out);
		for (unsigned int x=0;x<(objects.size());x++){
			iofile.write((char*)&objectstate.at(x), sizeof(objectstate.at(x)));
		}
		iofile.close();
		objectstate.clear();

	}


};