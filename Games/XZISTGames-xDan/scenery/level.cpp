
#include <level.h>

void Level::Start(irrInfo *info)
{
    this->info = info;

    camera = info->smgr->addCameraSceneNode();
    //info->smgr->setShadowColor(SColor(100,100,100,0));

    // don't want same level each time...or do we?
    srand(info->timer->getRealTime());

    // set up a test level

    f32 farValue = 16200;

	camera->setFarValue(farValue);

///	info->driver->setFog(SColor(0,125,125,125), true, farValue-800, farValue-200, 0, true, true);

	// light
	// need terrain normals for correct lighting (solution is NOT to decrease light distance)
	mainLight = info->smgr->addLightSceneNode(camera, vector3df(10000,10000,0), SColorf(1.0f, 1.0f, 0.0f, 1.0f), 10000.0f);

	SLight &slight = mainLight->getLightData();
	slight.AmbientColor = SColorf(0.5,0.5,0.5,0.5);

	// FIND NEW SKY BOX, OLD ONE IS COPYRIGHTED!!
    addSky();

	// ODE physics
	dWorld = dWorldCreate();
	dWorldSetERP(dWorld, 0.2);
	dWorldSetContactSurfaceLayer(dWorld, 0.001);
	dWorldSetCFM(dWorld, 0.00001);
	dSpace = dSimpleSpaceCreate(0);
	dJointGroup = dJointGroupCreate(0);
	dWorldSetGravity(dWorld, 0, -500, 0);

	// setup event handler. players one needs info struct for game pausing etc
	playerEvent.setIrrInfo(info);

	cameraDistance = 160;
	cameraDistanceDelta = 0;

	textures[0] = info->driver->getTexture("tex/dirt17.bmp");
	textures[1] = info->driver->getTexture("tex/tree.bmp");
	tilemap = new TileMap(info->smgr, dSpace, textures);

	// important stuff. entities need this, so must set up before any are created!
	info->dWorld = dWorld;
	info->dSpace = dSpace;
	info->entities = &entities;

	// camera can currently only be set to a spaceship entity as it does not check if is controllable (goForwards etc are used)
	// (Entities are cast to (SpaceShip *) with no type checking!!)
	cameraEntity = new HoverCraft(info);
	cameraEntity->setPosition(-50,4000,-140);

	// test enemies
	Entity *enemy = new HoverCraft(info);
	enemy->setPosition(-100,900,100);
	// AI will default to non-hostile to all others
	// set entity as autonamous (will control itself)
	enemy->setAuto(true);
	// increases hostility towards another entity
	enemy->attack(cameraEntity);


	for (i = 0; i < 5; i ++)
	{
		// this appears to be a memory leak
        Entity *enemy = new HoverCraft(info);
        enemy->setPosition( rand()%400 - 200, 4000 + rand()%200, rand()%400 - 200);
        enemy->setAuto(true);
        enemy->attack(cameraEntity);
    }


	unPause();

	info->gameRunning = true;
}

void Level::unPause()
{
    info->gamePaused = false;
    info->cursor->setVisible(false);
    playerEvent.Reset();
    info->device->setEventReceiver(&playerEvent);
    info->centreMouse();
    if (info->timer->isStopped()) info->timer->start();
}

void Level::Finish()
{
    DestroyEntities();
    delete tilemap;
    dWorldDestroy(dWorld);
    dSpaceDestroy(dSpace);
    dJointGroupDestroy(dJointGroup);
    info->smgr->getRootSceneNode()->removeAll();

    info->gamePaused = false;
    info->gameRunning = false;
}

void Level::refresh(float delta_t)
{
    // physics stepping
    dSpaceCollide(dSpace, this, dNearCallback);
    //dWorldStep(ode_world, delta_t);
    dWorldQuickStep(dWorld, delta_t);
    dJointGroupEmpty(dJointGroup);

    // now through entities setting orientation and whatnot from physics bodies (entity->refresh())
    for (i = 0; i < entities.size(); i ++)
    {
        if (entities[i])
        {
            entities[i]->RefreshAI();
            entities[i]->Refresh();
            if (entities[i] == cameraEntity)
            {
                // position camera and tilemap at this entity
                dReal *odePos = entities[i]->getPosition();//(dReal *)dGeomGetPosition(entities[i]->getMainGeom());

                // calc position based on mouse x,y
                // should use ray??
                static float yawAngle = 0, pitchAngle = 0;
                yawAngle += -playerEvent.mouseDX/60.0;
                pitchAngle += playerEvent.mouseDY/60.0;

                if (pitchAngle > 1) pitchAngle = 1;
                if (pitchAngle < -1.5) pitchAngle = -1.5;

                float cdist = cameraDistance;
                float adist = cos(pitchAngle) * cdist;
                vector3df camPos(odePos[0], odePos[1]+60, odePos[2]);
                camPos.X += cos(yawAngle + 90000) * adist;
                camPos.Y += sin(pitchAngle) * cdist;
                camPos.Z += sin(yawAngle + 90000) * adist;

                info->centreMouse();

                camera->setPosition(camPos);
                camera->setTarget(vector3df(odePos[0], odePos[1], odePos[2]));
                camera->updateAbsolutePosition();

                tilemap->setPos(odePos[0], 0, odePos[2]);

                // player control. should check that entity is player controlled!
                if (playerEvent.goForwards)
                {
                    ( (SpaceShip *)entities[i] )->goForwards();
                }
                if (playerEvent.goLeft)
                {
                    ( (SpaceShip *)entities[i] )->goLeft();
                }
                if (playerEvent.goRight)
                {
                    ( (SpaceShip *)entities[i] )->goRight();
                }

                f32 mouseWheel = playerEvent.getMouseWheel();

                // need smoother mouse wheel. like in guild wars. or maybe it's my mouse ^_^
                if (mouseWheel > 0.5)
                {
                    cameraDistanceDelta -= 60;
                    cameraDistance += cameraDistanceDelta;
                }
                else if (mouseWheel < -0.5)
                {
                    cameraDistanceDelta += 60;
                    cameraDistance += cameraDistanceDelta;
                }
                else
                {
                    cameraDistanceDelta = 0;
                }
                // limit camera distance. will need to use a ray to avoid geometry intersecting.
                if (cameraDistance < 100)
                {
                    cameraDistance = 100;
                }
                if (cameraDistance > 1000)
                {
                    cameraDistance = 1000;
                }

            }
        }
    }

    // drawing
    info->driver->beginScene(true, true, SColor(0,200,200,200));
    info->smgr->drawAll();
    info->guienv->drawAll();
    info->driver->endScene();
}

void Level::dNearCallback(void *data, dGeomID o1, dGeomID o2)
{
    // perhaps put MAX_CONTACTS and dContact structs in collision class. problem is which geom gets the data? o1 or o2?
    Level *me = (Level *)data;

    if (dGeomIsSpace (o1) || dGeomIsSpace (o2)) return;

    const int MAX_CONTACTS = 12; // per body

    // exit without doing anything if the two bodies are connected by a joint
    dBodyID b1 = dGeomGetBody(o1);
    dBodyID b2 = dGeomGetBody(o2);
    //if (b1 && b2 && dAreConnected (b1, b2))return;
    if(b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact)) return;

    dContact contact[MAX_CONTACTS];
    for (int i = 0; i < MAX_CONTACTS; i++)
    {
        contact[i].surface.mode = dContactBounce | dContactSoftCFM;

        contact[i].surface.mu     =   10000;//n1->surface_mu;

        contact[i].surface.bounce = 0;//n1->bounce;
        contact[i].surface.bounce_vel = 0;//n1->bounce_vel;
        contact[i].surface.soft_cfm = 0;//n1->soft_cfm;
    }

    int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact));

    if (numc == 0) return;

    for (int i = 0; i < 2; i ++)
    {
        dGeomID geom;
        Collision *col;
        if (i == 0) geom = o1;
        if (i == 1) geom = o2;
        col = (Collision *)dGeomGetData(geom);
        if (col)
        {
            col->add();
            if (dGeomGetClass(geom) == dRayClass) col->setRayContact(contact[0].geom);
        }
    }

    // if either are rays, we don't need contact joints
    if ( (dGeomGetClass(o1) == dRayClass) || (dGeomGetClass(o2) == dRayClass)) {
        return;
    }

    for (int i = 0; i < numc; i++) {
        dJointID c = dJointCreateContact(me->dWorld, me->dJointGroup, contact+i);
        dJointAttach (c, b1, b2);
    }
}

void Level::DestroyEntities()
{
    for (int i = 0; i < entities.size(); i ++) { delete entities[i]; }
    entities.clear();
}

void Level::addSky()
{
   	info->smgr->addSkyDomeSceneNode(info->driver->getTexture("tex/panorama/HiCloudExampleFull.jpg"),
        60,16,0.96f,1.1f, NULL);
}

