// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_MAPEDITOR_CPP

// Include the class definition
#include "mapeditor.hpp"

// Reference the factory class
#include "factory.hpp"
class dfactory;

// Using the vehe namespace
namespace vehe 
{
	using namespace ctypes;
	//Initializer
	mapeditor::mapeditor ()	
	{
		active = false;
		child  = false;
		picked = 666;
		lpicked = 666;

		// Store device pointers for use
		root = dfactory::instance()->getIrrDevice()->
				getGUIEnvironment()->getRootGUIElement();
		
		device = dfactory::instance()->getIrrDevice();

		smgr = dfactory::instance()->getIrrDevice()->getSceneManager();

		guienv = dfactory::instance()->getIrrDevice()->getGUIEnvironment();
		
		driver = dfactory::instance()->getIrrDevice()->getVideoDriver();

		audio =  dfactory::instance()->getSoundDevice();
	};

	//Show the map editor window
	void mapeditor::createEditor()
	{
		if (!active)
		{
			//Create the window
			window = guienv->addWindow(
			rect<s32>(10, 100, 225, 300), 
			false, 
			L"Map Editor Window",0,301);

			//Hide the close button
			IGUIButton * exitb = window->getCloseButton();
			exitb->setVisible(false);
			
			guienv->addStaticText(L"Functions:",  
			rect<s32>(15,25,140,50),
			false,
			false,
			window,302);

			guienv->addButton(rect<s32>(15 , 45, 15 + 80, 45 + 22),
			window , 303, L"Reset Camera", L"Reset Your Location!");

			guienv->addButton(rect<s32>(110 , 45, 110 + 80, 45 + 22),
			window , 304, L"Editor On", L"Toggle the Map Editor!");

			guienv->addButton(rect<s32>(15 , 75, 15 + 80, 75 + 22),
			window , 305, L"Add Object", L"Create a New Object!");

			guienv->addButton(rect<s32>(110 , 75, 110 + 80, 75 + 22),
			window , 306, L"Roaming Camera", L"Toggle the roaming camera!");

			guienv->addButton(rect<s32>(15 , 105, 15 + 80, 105 + 22),
			window , 307, L"Edit Object", L"Edit The Object!");

			guienv->addButton(rect<s32>(110 , 105, 110 + 80, 105 + 22),
			window , 308, L"Delete Object", L"Delete the selected object!");

			guienv->addButton(rect<s32>(15 , 135, 15 + 80, 135 + 22),
			window , 309, L"Move Object", L"Move The Object!");

			guienv->addButton(rect<s32>(110 , 135, 110 + 80, 135 + 22),
			window , 310, L"Clone Object", L"Clone the selected object!");
			
			active = true;
		}
		else
		{
			window->remove();
			active = false;
		}

		dfactory::instance()->getStateManager()->setState(1); // Reset GUI
	}

	//Create the object adding window
	void mapeditor::addObject()
	{
		if (!child)
		{
			child = true;

			//New Temporary
			irr::core::string <wchar_t> converter;

			//Create the window
			window_add = guienv->addWindow(
			rect<s32>(235, 100, 435, 580), 
			false, 
			L"Map Editor Window",0,311);

			//Hide the close button
			IGUIButton * exitb = window_add->getCloseButton();
			exitb->setVisible(false);
			
			guienv->addStaticText(L"Create an Object:",  
			rect<s32>(15,25,140,50),
			false,
			false,
			window_add);

			guienv->addStaticText(L"Name:", rect<s32>(15,40,175,60),
			false,false,window_add);
			guienv->addEditBox(L"My Object", rect<s32>(55, 40, 175, 60),true,window_add,312);
			
			guienv->addStaticText(L"Path:", rect<s32>(15,65,175,90),
			false,false,window_add);
			guienv->addEditBox(L"./data/graphics/models/", rect<s32>(55, 65, 175, 90),true,window_add,313);
			
			guienv->addStaticText(L"GFX:", rect<s32>(15,95,175,120),
			false,false,window_add);
			guienv->addEditBox(L"./data/graphics/models/", rect<s32>(55, 95, 175, 120),true,window_add,314);
				
			guienv->addStaticText(L"Script:", rect<s32>(15,125,175,150),
			false,false,window_add);
			guienv->addEditBox(L"./data/scripts/", rect<s32>(55, 125, 175, 150),true,window_add,315);
				
			guienv->addStaticText(L"Weight:", rect<s32>(15,155,175,180),
			false,false,window_add);
			guienv->addEditBox(L"0", rect<s32>(55, 155, 175, 180),true,window_add,316);
				
			guienv->addCheckBox(false,rect<s32>(15, 185, 175, 210),window_add,317,L" Vegetation");
			guienv->addCheckBox(false,rect<s32>(15, 215, 175, 250),window_add,318,L" is Static?");
			guienv->addCheckBox(false,rect<s32>(15, 245, 175, 280),window_add,319,L" Using Primitive?");
			guienv->addCheckBox(false,rect<s32>(15, 275, 175, 310),window_add,320,L" Single Texture?");
			guienv->addCheckBox(false,rect<s32>(15, 305, 175, 340),window_add,321,L" Using Alpha Channel?");
			guienv->addCheckBox(false,rect<s32>(15, 335, 175, 370),window_add,322,L" Scripted?");

			//Get Camera Pos
			vector3df position = dfactory::instance()->getCameraNode()->getPosition();
	
			guienv->addStaticText(L"Pos:", rect<s32>(15,375,175,400),
			false,false,window_add);
			converter = (int)position.X;
			guienv->addEditBox(converter.c_str(), rect<s32>(35, 375, 35 + 40, 375+20),true,window_add,323);
			converter = (int)position.Y;
			guienv->addEditBox(converter.c_str(), rect<s32>(80, 375, 80 + 40, 375+20),true,window_add,324);
			converter = (int)position.Z;
			guienv->addEditBox(converter.c_str(), rect<s32>(125, 375, 125 + 40, 375+20),true,window_add,325);
			
			//Get Camera Rot
			vector3df rotation = dfactory::instance()->getCameraNode()->getRotation();

			guienv->addStaticText(L"Rot:", rect<s32>(15,405,175,500),
			false,false,window_add);
			converter = (int)rotation.X;
			guienv->addEditBox(converter.c_str(), rect<s32>(35, 405, 35 + 40, 405+20),true,window_add,326);
			converter =(int) rotation.Y;
			guienv->addEditBox(converter.c_str(), rect<s32>(80, 405, 80 + 40, 405+20),true,window_add,327);
			converter = (int)rotation.Z;
			guienv->addEditBox(converter.c_str(), rect<s32>(125, 405, 125 + 40, 405+20),true,window_add,328);
			
			guienv->addStaticText(L"Scale:", rect<s32>(10,435,175,500),
			false,false,window_add);
			guienv->addEditBox(L"1", rect<s32>(35, 435, 35 + 40, 435+20),true,window_add,329);
			guienv->addEditBox(L"1", rect<s32>(80, 435, 80 + 40, 435+20),true,window_add,330);
			guienv->addEditBox(L"1", rect<s32>(125, 435, 125 + 40, 435+20),true,window_add,331);
			
			guienv->addButton(rect<s32>(15, 460, 15 + 80, 460 + 18),
				window_add , 332, L"Add Object", L"Create a New Object!");
			guienv->addButton(rect<s32>(100, 460, 115 + 80, 460 + 18),
				window_add , 333, L"Cancel", L"Cancel");
		}
	}

	
	//Create the object editing window
	void mapeditor::editObject()
	{
		if (picked!=666 && picked!=-1 && !child)
		{
			child = true;
			object				cobj = dfactory::instance()->getObject(picked);
		
			//New Temporary
			irr::core::string <wchar_t> converter;
			
			//Create the window
			window_edit = guienv->addWindow(
			rect<s32>(235, 100, 435, 580), 
			false, 
			L"Map Editor Window",0,341);

			//Hide the close button
			IGUIButton * exitb = window_edit->getCloseButton();
			exitb->setVisible(false);
			
			guienv->addStaticText(L"Edit an Object:",  
			rect<s32>(15,25,140,50),
			false,
			false,
			window_edit);

			guienv->addStaticText(L"Name:", rect<s32>(15,40,175,60),
			false,false,window_edit);
			converter = cobj.name;
			guienv->addEditBox(converter.c_str(), rect<s32>(55, 40, 175, 60),true,window_edit,342);
			
			guienv->addStaticText(L"Path:", rect<s32>(15,65,175,313),
			false,false,window_edit);
			converter = cobj.model;
			guienv->addEditBox(converter.c_str(), rect<s32>(55, 65, 175, 90),true,window_edit,343);
			
			guienv->addStaticText(L"GFX:", rect<s32>(15,95,175,120),
			false,false,window_edit);
			converter = cobj.gfx;
			guienv->addEditBox(converter.c_str(), rect<s32>(55, 95, 175, 120),true,window_edit,344);
				
			guienv->addStaticText(L"Script:", rect<s32>(15,125,175,150),
			false,false,window_edit);
			converter = cobj.script;
			guienv->addEditBox(converter.c_str(), rect<s32>(55, 125, 175, 150),true,window_edit,345);
				
			guienv->addStaticText(L"Weight:", rect<s32>(15,155,175,180),
			false,false,window_edit);
			converter = cobj.weight;
			guienv->addEditBox(converter.c_str(), rect<s32>(55, 155, 175, 180),true,window_edit,346);
				
			guienv->addCheckBox(cobj.vegetation,rect<s32>(15, 185, 175, 210),window_edit,347,L" Vegetation");
			guienv->addCheckBox(cobj.isStatic,rect<s32>(15, 215, 175, 250),window_edit,348,L" is Static?");
			guienv->addCheckBox(cobj.primitive,rect<s32>(15, 245, 175, 280),window_edit,349,L" Using Primitive?");
			guienv->addCheckBox(cobj.texture,rect<s32>(15, 275, 175, 310),window_edit,350,L" Single Texture?");
			guienv->addCheckBox(cobj.alpha,rect<s32>(15, 305, 175, 340),window_edit,351,L" Using Alpha Channel?");
			guienv->addCheckBox(cobj.scripted,rect<s32>(15, 335, 175, 370),window_edit,352,L" Scripted?");

			//Get Camera Pos
			vector3df position = cobj.location;
	
			guienv->addStaticText(L"Pos:", rect<s32>(15,375,175,400),
			false,false,window_edit);
			converter = (int)position.X;
			guienv->addEditBox(converter.c_str(), rect<s32>(35, 375, 35 + 40, 375+20),true,window_edit,353);
			converter = (int)position.Y;
			guienv->addEditBox(converter.c_str(), rect<s32>(80, 375, 80 + 40, 375+20),true,window_edit,354);
			converter = (int)position.Z;
			guienv->addEditBox(converter.c_str(), rect<s32>(125, 375, 125 + 40, 375+20),true,window_edit,355);
			
			//Get Camera Rot
			vector3df rotation = cobj.rotation;

			guienv->addStaticText(L"Rot:", rect<s32>(15,405,175,500),
			false,false,window_edit);
			converter = (int)rotation.X;
			guienv->addEditBox(converter.c_str(), rect<s32>(35, 405, 35 + 40, 405+20),true,window_edit,356);
			converter = (int)rotation.Y;
			guienv->addEditBox(converter.c_str(), rect<s32>(80, 405, 80 + 40, 405+20),true,window_edit,357);
			converter = (int)rotation.Z;
			guienv->addEditBox(converter.c_str(), rect<s32>(125, 405, 125 + 40, 405+20),true,window_edit,358);
			
			//Get Camera Scale
			vector3df scale = cobj.scale;

			guienv->addStaticText(L"Scale:", rect<s32>(10,435,175,500),
			false,false,window_edit);
			converter = (int)scale.X;
			guienv->addEditBox(converter.c_str(), rect<s32>(35, 435, 35 + 40, 435+20),true,window_edit,359);
			converter = (int)scale.Y;
			guienv->addEditBox(converter.c_str(), rect<s32>(80, 435, 80 + 40, 435+20),true,window_edit,360);
			converter = (int)scale.Z;
			guienv->addEditBox(converter.c_str(), rect<s32>(125, 435, 125 + 40, 435+20),true,window_edit,361);
			
			guienv->addButton(rect<s32>(15, 460, 15 + 80, 460 + 18),
				window_edit , 363, L"Edit Object", L"Edit the Object!");
			guienv->addButton(rect<s32>(100, 460, 115 + 80, 460 + 18),
				window_edit , 364, L"Cancel", L"Cancel");
		}
	}

	//! Create an object or edit one
	void mapeditor::makeObject ()
	{
		dfactory::instance()->saveMap("./data/maps/backup.cobj");

		//Temporary Variables
		irr::core::string <char> converter;
		IGUICheckBox * icheck;

		object myobject;
		/* Get all the text box information */
		converter = root->getElementFromId(312,true)->getText();
		strcpy	(myobject.name, converter.c_str());
		converter = root->getElementFromId(313,true)->getText();
		strcpy	(myobject.model, converter.c_str());
		converter = root->getElementFromId(314,true)->getText();
		strcpy	(myobject.gfx, converter.c_str());
		converter = root->getElementFromId(315,true)->getText();
		strcpy	(myobject.script, converter.c_str());
		converter = root->getElementFromId(316,true)->getText();
		myobject.weight = atoi(converter.c_str());

		//Check to see if the files exist
		if (!device->getFileSystem()->existFile(myobject.model) ||
			!device->getFileSystem()->existFile(myobject.gfx))
		{
			cout << "Error: One of the file names you entered was incorrect!" << endl;
			return;
		}

		/* Get all check box information */
		icheck = (IGUICheckBox*)root->getElementFromId(317,true);
		if (icheck->isChecked())
			myobject.vegetation=true;
		else
			myobject.vegetation=false;

		icheck =  (IGUICheckBox*)root->getElementFromId(318,true);
		if (icheck->isChecked())
			myobject.isStatic=true;
		else
			myobject.isStatic=false;

		icheck =  (IGUICheckBox*)root->getElementFromId(319,true);
		if (icheck->isChecked())
			myobject.primitive=true;
		else
			myobject.primitive=false;

		icheck = (IGUICheckBox*)root->getElementFromId(320,true);
		if (icheck->isChecked())
			myobject.texture=true;
		else
			myobject.texture=false;

		icheck = (IGUICheckBox*)root->getElementFromId(321,true);
		if (icheck->isChecked())
			myobject.alpha=true;
		else
			myobject.alpha=false;

		icheck = (IGUICheckBox*)root->getElementFromId(322,true);
		if (icheck->isChecked())
			myobject.scripted=true;
		else
			myobject.scripted=false;

		//Get the Position from the GUI
		vector3df newlocation;
		converter = root->getElementFromId(323,true)->getText(); 
		newlocation.X = (float)atoi(converter.c_str());
		converter = root->getElementFromId(324,true)->getText(); 
		newlocation.Y = (float)atoi(converter.c_str());
		converter = root->getElementFromId(325,true)->getText(); 
		newlocation.Z = (float)atoi(converter.c_str());
		myobject.location = newlocation;

		//Get the rotation from the GUI
		vector3df newrotation;
		converter = root->getElementFromId(326,true)->getText(); 
		newrotation.X = (float)atoi(converter.c_str());
		converter = root->getElementFromId(327,true)->getText(); 
		newrotation.Y = (float)atoi(converter.c_str());
		converter = root->getElementFromId(328,true)->getText(); 
		newrotation.Z = (float)atoi(converter.c_str());
		myobject.rotation = newrotation;

		//Get the Scale from the GUI
		vector3df newscale;
		converter = root->getElementFromId(329,true)->getText(); 
		newscale.X = (float)atoi(converter.c_str());
		converter = root->getElementFromId(330,true)->getText(); 
		newscale.Y = (float)atoi(converter.c_str());
		converter = root->getElementFromId(331,true)->getText(); 
		newscale.Z = (float)atoi(converter.c_str());
		myobject.scale = newscale;

		//Create the instance of the objects in the game world
		dfactory::instance()->buildObject(myobject);

		//Save the map
		irr::core::string <char> saveto = "./data/maps/";
		saveto.append(dfactory::instance()->getMapObject().file);
		dfactory::instance()->saveMap((char *) saveto.c_str());

		window_add->remove();
		child = false;
	}

	//! Edit an object
	void mapeditor::updateObject ()
	{
		dfactory::instance()->saveMap("./data/maps/backup.cobj");

		irr::core::string <char> converter;
		IGUICheckBox * icheck;

		object myobject;
		/* Get all the text box information */
		converter = root->getElementFromId(342,true)->getText(); 
		strcpy	(myobject.name, converter.c_str());
		converter = root->getElementFromId(343,true)->getText(); 
		strcpy	(myobject.model,  converter.c_str());
		converter = root->getElementFromId(344,true)->getText(); 
		strcpy	(myobject.gfx, converter.c_str());
		converter = root->getElementFromId(345,true)->getText(); 
		strcpy	(myobject.script,  converter.c_str());
		converter = root->getElementFromId(346,true)->getText(); 
		myobject.weight = atoi(converter.c_str());

		//Check to see if the files exist
		if (!device->getFileSystem()->existFile(myobject.model) ||
			!device->getFileSystem()->existFile(myobject.gfx))
		{
			cout << "Error: One of the file names you entered was incorrect!" << endl;
			return;
		}

		/* Get all check box information */
		icheck = (IGUICheckBox*)root->getElementFromId(347,true);
		if (icheck->isChecked())
			myobject.vegetation=true;
		else
			myobject.vegetation=false;

		icheck =  (IGUICheckBox*)root->getElementFromId(348,true);
		if (icheck->isChecked())
			myobject.isStatic=true;
		else
			myobject.isStatic=false;

		icheck =  (IGUICheckBox*)root->getElementFromId(349,true);
		if (icheck->isChecked())
			myobject.primitive=true;
		else
			myobject.primitive=false;

		icheck = (IGUICheckBox*)root->getElementFromId(350,true);
		if (icheck->isChecked())
			myobject.texture=true;
		else
			myobject.texture=false;

		icheck = (IGUICheckBox*)root->getElementFromId(351,true);
		if (icheck->isChecked())
			myobject.alpha=true;
		else
			myobject.alpha=false;

		icheck = (IGUICheckBox*)root->getElementFromId(352,true);
		if (icheck->isChecked())
			myobject.scripted=true;
		else
			myobject.scripted=false;

		//Get the Position from the GUI
		vector3df newlocation;
		converter = root->getElementFromId(353,true)->getText(); 
		newlocation.X = (float)atoi(converter.c_str());
		converter = root->getElementFromId(354,true)->getText(); 
		newlocation.Y = (float)atoi(converter.c_str());
		converter = root->getElementFromId(355,true)->getText(); 
		newlocation.Z = (float)atoi(converter.c_str());
		myobject.location = newlocation;

		//Get the rotation from the GUI
		vector3df newrotation;
		converter = root->getElementFromId(356,true)->getText(); 
		newrotation.X = (float)atoi(converter.c_str());
		converter = root->getElementFromId(357,true)->getText();  
		newrotation.Y = (float)atoi(converter.c_str());
		converter = root->getElementFromId(358,true)->getText(); 
		newrotation.Z = (float)atoi(converter.c_str());
		myobject.rotation = newrotation;

		//Get the Scale from the GUI
		vector3df newscale;
		converter = root->getElementFromId(359,true)->getText(); 
		newscale.X = (float)atoi(converter.c_str());
		converter = root->getElementFromId(360,true)->getText(); 
		newscale.Y = (float)atoi(converter.c_str());
		converter = root->getElementFromId(361,true)->getText(); 
		newscale.Z = (float)atoi(converter.c_str());
		myobject.scale = newscale;

		//Create the instance of the objects in the game world
		dfactory::instance()->updateObject(myobject,picked);

		//Save the map
		irr::core::string <char> saveto = "./data/maps/";
		saveto.append(dfactory::instance()->getMapObject().file);
		dfactory::instance()->saveMap((char *) saveto.c_str());

		window_edit->remove();
		child = false;
	}


		//Process GUI Inpit
	void mapeditor::processGUI(int x, int y)
	{
		//Process any GUI clicks
		IGUIElement * mygui = root->getElementFromPoint(position2d<s32>(x,y));

		if (mygui)
		if (active || mygui->getID() == 304 || mygui->getID() == 327 ) // CLOSE BUTTONS
		switch (mygui->getID())
		{
		case 303: // RESET LOCATION
			dfactory::instance()->resetCamera();
			dfactory::instance()->freeMouse();
			return;
			break;
		case 304: // TOGGLES MAP EDITOR MODE
			if (active==true)
			{
				active = false;
				root->getElementFromId(304,true)->setText(L"Editor Off");
			}
			else
			{
				active = true;
				root->getElementFromId(304,true)->setText(L"Editor On");
			}
			dfactory::instance()->getStateManager()->setState(1); // Reset GUI
			return;
			break;
		case 305: // OPEN ADD OBJECT WINDOW
			mapeditor::addObject();
			return;
			break;
		case 306: // SWITCH TO ROAMING CAMERA
			dfactory::instance()->roamingCamera();
			return;
			break;
		case 307: // OPEN EDIT OBJECT WINDOW
			mapeditor::editObject();
			return;
			break;
		case 308: // DELETE THE SELECTED OBJECT
			dfactory::instance()->deleteObject(picked);
			return;
			break;
		case 309: // MOVE THE SELECTED OBJECT
			dfactory::instance()->moveObject(picked);
			return;
			break;
		case 310: // CLONE THE SELECTED OBJECT
			dfactory::instance()->cloneObject(picked);
			return;
			break;
		case 332: // ADD A NEW OBJECT
			mapeditor::makeObject();
			return;
			break;
		case 333: // CLOSE ADD OBJECT WINDOW
			window_add->remove();
			child = false;
			return;
			break;
		case 363: // EDIT OBJECT
			mapeditor::updateObject();
			return;
			break;
		case 364: // CLOSE EDIT OBJECT WINDOW
			window_edit->remove();
			child = false;
			return;
			break;
		};
			
		//Get the node thats under the mouse cursor
		ISceneNode * mynode = smgr->getSceneCollisionManager()->
			getSceneNodeFromScreenCoordinatesBB(core::position2d<s32>(x,y)) ;
		if (active)
		{
			if (mynode && mynode->getType()==ESNT_ANIMATED_MESH  && !child)
			{
				dfactory::instance()->getBillBoardNode()->setVisible(true);
				dfactory::instance()->getBillBoardNode()->setPosition(mynode->getPosition());
				lpicked = picked;
				picked = (int)mynode->getID();				
				drawSelectedCollision();
			}
			else if (!child)
			{
				dfactory::instance()->getBillBoardNode()->setVisible(false);
					lpicked = picked;
					drawSelectedCollision();
				lpicked = 666;
				picked = 666;

			}	
		}
		else
		{
			dfactory::instance()->getBillBoardNode()->setVisible(false);
				lpicked = picked;
				drawSelectedCollision();
			lpicked = 666;
			picked = 666;
		}
		
	}

	//! Draw the selected objects collision debug infos
	void mapeditor::drawSelectedCollision()
	{
		if (active && picked != 666 && picked != -1 && 
			!dfactory::instance()->getObject(picked).vegetation)
			dfactory::instance()->getEntity(picked).mynode->setDebugDataVisible(-1);
		
		if (active && lpicked != 666 && lpicked != -1)
			dfactory::instance()->getEntity(lpicked).mynode->setDebugDataVisible(0);
		
	}

};