#ifndef RUNNINGSTATE_H_
#define RUNNINGSTATE_H_

#include "../../scene/Ship.h"
#include "../../scene/Terrain.h"
#include "../../scene/NPC.h"
#include "../../scene/grass/CWindGenerator.h"
#include "../../scene/grass/CGrassGeneratorNode.h"
#include "../../gui/HUD.h"

class RunningState : public State<Game, irr::SEvent>
{

private:

	///Array of keys pressed.
	bool keys[irr::KEY_KEY_CODES_COUNT];

	const void checkKeys(Game* game) const;

	bool mouseButtons[3];

	irr::u32 lastEventTime;

	const void updateCamera(const Game* game) const;

	bool lockCamera;

	irr::scene::CGrassGeneratorNode* grassGeneratorNode;

	irr::scene::IWindGenerator* wind;

	Terrain* terrain;

	Ship* ship;

	irr::core::list<NPC*> npcList;

	irr::scene::ISceneNode* skybox;

	irr::scene::IBillboardSceneNode* cursor;

	HUD* hud;
	
public:

	RunningState();

	virtual ~RunningState();

	virtual void onEnter(Game* game);

	virtual void onLeave(Game* game);

	virtual const bool onEvent(Game* entity, const irr::SEvent& event);

	virtual void onUpdate(Game* game);
	
};

RunningState::RunningState() :
	lastEventTime(0),
	lockCamera(true),
	grassGeneratorNode(0),
	wind(0),
	terrain(0), ship(0),
	skybox(0), cursor(0),
	hud(0)
{
	
}

RunningState::~RunningState()
{
	if (grassGeneratorNode)
	{
		grassGeneratorNode->remove();
		grassGeneratorNode->drop();
	}

	if (wind)
		delete wind;
	
	if (hud)
		delete hud;
	
	if (cursor)
	{
		cursor->remove();
		cursor->drop();
	}

	if (skybox)
	{
		skybox->remove();
		skybox->drop();
	}

	if (terrain)
		delete terrain;
	
	if (ship)
		delete ship;

	//delete npc
    irr::core::list<NPC*>::Iterator it;
    for(it = this->npcList.begin(); it != this->npcList.end(); ++it)
    {
        NPC* oo = (*it);
        delete oo;
    }
    this->npcList.clear();
}

void RunningState::onEnter(Game* game)
{
	//init key array.
	irr::u32 k;
   	for (k = 0; k < sizeof(keys) / sizeof(*keys); ++k)
      	keys[k] = false;
   	for (k = 0; k < sizeof(mouseButtons) / sizeof(*mouseButtons); ++k)
   		mouseButtons[k] = false;

   	game->getDevice()->getCursorControl()->setVisible(false);

   	game->getSceneManager()->setAmbientLight(irr::video::SColorf(0.4f, 0.4f, 0.4f));
    game->getWeatherSystem()->setFog(true);
    game->getWeatherSystem()->setLight(true);


    //create terrain
	terrain = new Terrain(game);

    // create wind
    wind = irr::scene::createWindGenerator(30.0f, 3.0f);

    // load needed map files for the grass
    irr::video::IImage* heightMap = game->getVideoDriver()->createImageFromFile("media/images/terrain/default/terrain-heightmap.bmp");
    irr::video::IImage* textureMap = game->getVideoDriver()->createImageFromFile("media/images/grass/terrain-grasscol.bmp");
    irr::video::IImage* grassMap = game->getVideoDriver()->createImageFromFile("media/images/grass/terrain-grassmap.png");

    // add GrassGeneratorNode and add grass to terrain
    grassGeneratorNode = new irr::scene::CGrassGeneratorNode(game->getDevice());
   	grassGeneratorNode->addGrassToTerrain(game, wind, terrain->getTerrain(), heightMap, textureMap, grassMap); 

	// we don't need the IImages anymore
    if (heightMap) heightMap->drop();
    if (textureMap) textureMap->drop();
	if (grassMap) grassMap->drop();


	this->ship = new Ship(
		game,
		"Player",
		"media/models/spacefighter/fighter.3ds",
		"",
		irr::core::vector3df(1500, 170, 2400)
	);


	//add npc
	NPC* sydney = new NPC(
		game, // pointer to the game
		L"Sydney", // a name
		"media/models/sydney/sydney.md2", //mesh
		"media/models/sydney/sydney.bmp", //texture
		irr::core::vector3df(1480.0f, 61.0f, 2700.0f), //position
		irr::core::vector3df(0.0f, 45.0f, 0.0f), //rotation
		irr::core::vector3df(1.0f, 1.0f, 1.0f) //scale
	);
	sydney->getNode()->setMD2Animation(irr::scene::EMAT_WAVE);
	this->npcList.push_back(sydney);


	NPC* alienship = new NPC(game, L"Alien ship", "media/models/spacefighter/fighter.3ds",
		"", irr::core::vector3df(1490.0f, 80.0f, 2690.0f),
		irr::core::vector3df(0.0f, 0.0f, 0.0f), irr::core::vector3df(1.0f, 1.0f, 1.0f));
	this->npcList.push_back(alienship);
	
	
	
	
	cursor = game->getSceneManager()->addBillboardSceneNode(
		ship->getNode(), irr::core::dimension2df(4, 4), irr::core::vector3df(0.0f, 0.0f, 50.0f));
	cursor->setMaterialType(irr::video::EMT_TRANSPARENT_ADD_COLOR);
	cursor->setMaterialTexture(0, game->getVideoDriver()->getTexture("media/images/crosshair/cross.bmp"));
	cursor->setMaterialFlag(irr::video::EMF_LIGHTING, false);
	cursor->setMaterialFlag(irr::video::EMF_ZBUFFER, false);
	this->cursor->grab();

	this->hud = new HUD(game);
}

void RunningState::onLeave(Game* game)
{
	if (cursor)
	{
		cursor->remove();
		cursor->drop();
		cursor = 0;
	}

	if (skybox)
	{
		skybox->remove();
		skybox->drop();
		skybox = 0;
	}

	if (terrain)
	{
		delete terrain;
		terrain = 0;
	}

	if (ship)
	{
		delete ship;
		ship = 0;
	}


	//delete npc
    irr::core::list<NPC*>::Iterator it;
    for(it = this->npcList.begin(); it != this->npcList.end(); ++it)
    {
        NPC* npc = (*it);
        delete npc;
    }
    this->npcList.clear();

	
	if (hud)
	{
		delete hud;
		hud = 0;
	}

	if (wind)
	{
		delete wind;
		wind = 0;
	}

	if (grassGeneratorNode)
	{
		grassGeneratorNode->remove();
		grassGeneratorNode->drop();
		grassGeneratorNode = 0;
	}

	game->getDevice()->getCursorControl()->setVisible(true);
}

const bool RunningState::onEvent(Game* game, const irr::SEvent& event)
{
	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		if (!event.KeyInput.PressedDown)
		{
			switch (event.KeyInput.Key)
			{
				case irr::KEY_ESCAPE: game->setState(game->findGameState("intro")); return true;
				case irr::KEY_KEY_C: lockCamera = !lockCamera; return true;
				case irr::KEY_F12: game->makeScreenshot(); return true;
				
				default: ;
			}
		}

        keys[event.KeyInput.Key] = event.KeyInput.PressedDown;
    }


    // Handle mouse movement events only 
    if(irr::EET_MOUSE_INPUT_EVENT == event.EventType) 
    {
    	if (irr::EMIE_MOUSE_MOVED == event.MouseInput.Event)
    	{
	        // To allow continuous movement, we have to keep moving the cursor back to the middle
    		// of the screen (and compare it with that position) 
	        const irr::core::position2di screenCentre(
	        	game->getVideoDriver()->getScreenSize().Width/2, 
	            game->getVideoDriver()->getScreenSize().Height/2); 

	        // Ignore any event where the cursor is at the screen centre as that's likely generated by us moving it there 
	        if(screenCentre.X == event.MouseInput.X && screenCentre.Y == event.MouseInput.Y) 
	           return false;

	        // We need to know how fast the cursor is being moved 
	        const irr::u32 now = game->getDevice()->getTimer()->getTime(); 

	        if(0 != lastEventTime && now != lastEventTime) 
	        { 
	            // The less time that's passed since the last movement, the faster 
	            // the cursor is moving, so *divide* by the delta time. 
	        	const irr::f32 magnitude = game->getConfiguration().getCursorSensitivity() / (irr::f32)(now - lastEventTime); 

	            // Now work out how big a movement this represents
	        	irr::core::vector2df requiredMovement(
	        		(irr::f32)(event.MouseInput.X - screenCentre.X),
	        		(irr::f32)(event.MouseInput.Y - screenCentre.Y)
	        	); 
	            requiredMovement *= magnitude;

	            ship->turn(requiredMovement.X*game->getElapsed());
	            ship->pitch(requiredMovement.Y*game->getElapsed());
	        }

	        // Remember this movement time 
	        lastEventTime = now; 
	
	        // And move the cursor back to the screen centre 
	        game->getDevice()->getCursorControl()->setPosition(screenCentre);
	        
	        return true;
    	}

    	//check mouse buttons
    	if (irr::EMIE_LMOUSE_PRESSED_DOWN == event.MouseInput.Event) this->mouseButtons[0] = true;
    	else if (irr::EMIE_LMOUSE_LEFT_UP == event.MouseInput.Event) this->mouseButtons[0] = false;
    	if (irr::EMIE_MMOUSE_PRESSED_DOWN == event.MouseInput.Event) this->mouseButtons[1] = true;
    	else if (irr::EMIE_MMOUSE_LEFT_UP == event.MouseInput.Event) this->mouseButtons[1] = false;
    	if (irr::EMIE_RMOUSE_PRESSED_DOWN == event.MouseInput.Event) this->mouseButtons[2] = true;
    	else if (irr::EMIE_RMOUSE_LEFT_UP== event.MouseInput.Event) this->mouseButtons[2] = false;
    }

	return false;
}

void RunningState::onUpdate(Game* game)
{
	checkKeys(game);

	if (mouseButtons[0]) //left mouse button
		game->getWeaponSystem()->fire(ship, terrain);

	//update global camera
	updateCamera(game);
	
	this->ship->hover(this->terrain, mouseButtons[2]);


	this->hud->update(ship);
}

const void RunningState::updateCamera(const Game* game) const
{
	// get transformation matrix of node
	irr::core::matrix4 m;
	m.setRotationDegrees(ship->getNode()->getRotation());

    // transform forward vector of camera
    irr::core::vector3df frv(0.0f, 0.0f, 1.0f);
    m.transformVect(frv);

    // transform upvector of camera
    irr::core::vector3df upv(0.0f, 1.0f, 0.0f);
    m.transformVect(upv);

    // transform camera offset (thanks to Zeuss for finding it was missing)
    irr::core::vector3df offset(0.0f, 1.5f, -6.0f);
    m.transformVect(offset);

    game->getCamera()->setPosition(ship->getNode()->getPosition() + offset); //position camera behind the ship

    if (lockCamera)
    	game->getCamera()->setUpVector(upv); //set up vector of camera

    game->getCamera()->setTarget(ship->getNode()->getPosition() + frv); //set target of camera (look at point)

    game->getCamera()->updateAbsolutePosition();
}

const void RunningState::checkKeys(Game* game) const
{
    if (keys[irr::KEY_LEFT])
    {
    	if (keys[irr::KEY_KEY_S]) ship->turn(game->getElapsed());
    	else ship->turn(-game->getElapsed());
    }
    else if (keys[irr::KEY_RIGHT])
    {
    	if (keys[irr::KEY_KEY_S]) ship->turn(-game->getElapsed());
    	else ship->turn(game->getElapsed());
    }

    if (keys[irr::KEY_UP])
    	ship->pitch(game->getElapsed());
    else if (keys[irr::KEY_DOWN])
    	ship->pitch(-game->getElapsed());

    if (keys[irr::KEY_KEY_A])
    	ship->roll(game->getElapsed());
    else if (keys[irr::KEY_KEY_D])
    	ship->roll(-game->getElapsed());

    if (keys[irr::KEY_KEY_W])
    	ship->move(game->getElapsed());
    else if (keys[irr::KEY_KEY_S])
    	ship->move(-game->getElapsed());
    
    if (keys[irr::KEY_SPACE])
    	game->getWeaponSystem()->fire(ship, terrain);
}

#endif /*RUNNINGSTATE_H_*/
