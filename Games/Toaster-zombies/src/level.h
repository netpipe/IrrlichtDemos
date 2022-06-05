//creates floor
    IAnimatedMesh* floormesh = smgr->getMesh("scene.obj");
    IAnimatedMeshSceneNode* floornode = smgr->addAnimatedMeshSceneNode(floormesh,0,-1,vector3df(0,-2,0));

    IAnimatedMesh* bulletmesh = smgr->getMesh("bullet6.obj");
    IAnimatedMesh* locketmesh = smgr->getMesh("rocketz.obj");
    IAnimatedMesh* devilshotmesh = smgr->getMesh("rocketz.obj");
    IAnimatedMesh* bodymesh = smgr->getMesh("zombiebody.obj");
    IAnimatedMesh* headmesh = smgr->getMesh("zombiehead.obj");
    IAnimatedMesh* armrmesh = smgr->getMesh("zombiearmr.obj");
    IAnimatedMesh* armlmesh = smgr->getMesh("zombiearml.obj");
    IAnimatedMesh* footrmesh = smgr->getMesh("zombiefootr.obj");
    IAnimatedMesh* footlmesh = smgr->getMesh("zombiefootl.obj");
    IAnimatedMesh* dbodymesh = smgr->getMesh("dbody.obj");
    IAnimatedMesh* dheadmesh = smgr->getMesh("dhead.obj");
    IAnimatedMesh* darmrmesh = smgr->getMesh("darmr.obj");
    IAnimatedMesh* darmlmesh = smgr->getMesh("darml.obj");
    IAnimatedMesh* dfootrmesh = smgr->getMesh("dfootr.obj");
    IAnimatedMesh* dfootlmesh = smgr->getMesh("dfootl.obj");
    IAnimatedMesh* roofmesh = smgr->getMesh("roofs.obj");
    IAnimatedMeshSceneNode *Roofnode = smgr->addAnimatedMeshSceneNode(roofmesh,0,-1, vector3df(0,2,0));

    ITexture* crosshair = driver->getTexture("crosshair.png");
    driver->makeColorKeyTexture(crosshair, core::position2d<s32>(0,0));
    ITexture* ammo = driver->getTexture("ammo3.png");
    driver->makeColorKeyTexture(ammo, core::position2d<s32>(0,0));
    ITexture* hp = driver->getTexture("hp.png");
    driver->makeColorKeyTexture(hp, core::position2d<s32>(0,0));
    ITexture* hud = driver->getTexture("hud2.png");
    driver->makeColorKeyTexture(hud, core::position2d<s32>(0,0));
    ITexture* skull = driver->getTexture("skulls.png");
    driver->makeColorKeyTexture(skull, core::position2d<s32>(0,0));
    ITexture* Numbers[10];
    Numbers[0] = driver->getTexture("ZERO.png");
    driver->makeColorKeyTexture(Numbers[0], core::position2d<s32>(0,0));
    Numbers[1] = driver->getTexture("ONE.png");
    driver->makeColorKeyTexture(Numbers[1], core::position2d<s32>(0,0));
    Numbers[2] = driver->getTexture("TWO.png");
    driver->makeColorKeyTexture(Numbers[2], core::position2d<s32>(0,0));
    Numbers[3] = driver->getTexture("THREE.png");
    driver->makeColorKeyTexture(Numbers[3], core::position2d<s32>(0,0));
    Numbers[4] = driver->getTexture("FOUR.png");
    driver->makeColorKeyTexture(Numbers[4], core::position2d<s32>(0,0));
    Numbers[5] = driver->getTexture("FIVE.png");
    driver->makeColorKeyTexture(Numbers[5], core::position2d<s32>(0,0));
    Numbers[6] = driver->getTexture("SIX.png");
    driver->makeColorKeyTexture(Numbers[6], core::position2d<s32>(0,0));
    Numbers[7] = driver->getTexture("SEVEN.png");
    driver->makeColorKeyTexture(Numbers[7], core::position2d<s32>(0,0));
    Numbers[8] = driver->getTexture("EIGHT.png");
    driver->makeColorKeyTexture(Numbers[8], core::position2d<s32>(0,0));
    Numbers[9] = driver->getTexture("NINE.png");
    driver->makeColorKeyTexture(Numbers[9], core::position2d<s32>(0,0));
    ITexture* DPMessage[10];
    ITexture* EventMessage[20];
    ITexture* Splash;
    Splash = driver->getTexture("inthetoaster.bmp");





    for(c = 0; c < 200; c++)
    {
        bullet[c].hit = true;
        bullet[c].Bulletnode = smgr->addAnimatedMeshSceneNode(bulletmesh,0,-1,vector3df(0,10000,0));
        bullet[c].Locketnode = smgr->addAnimatedMeshSceneNode(locketmesh,0,-1,vector3df(0,0,0));
        bullet[c].Locketnode->setParent(bullet[c].Bulletnode);
        bullet[c].Locketnode->setVisible(false);
        bullet[c].Devilnode = smgr->addAnimatedMeshSceneNode(devilshotmesh,0,-1,vector3df(0,0,0));
        bullet[c].Devilnode->setParent(bullet[c].Bulletnode);
        bullet[c].Devilnode->setVisible(false);
        bullet[c].timer = 0;
        zombies[c].Bodynode = smgr-> addAnimatedMeshSceneNode(bodymesh,0,-1,vector3df(0,500,0));
        zombies[c].Headnode = smgr-> addAnimatedMeshSceneNode(headmesh,zombies[c].Bodynode,-1,vector3df(0,0,0));
        zombies[c].Armrnode = smgr-> addAnimatedMeshSceneNode(armrmesh,zombies[c].Bodynode,-1,vector3df(0,11,-5));
        zombies[c].Armlnode = smgr-> addAnimatedMeshSceneNode(armlmesh,zombies[c].Bodynode,-1,vector3df(0,11,-5));
        zombies[c].Footrnode = smgr->addAnimatedMeshSceneNode(footrmesh,zombies[c].Bodynode,-1,vector3df(0,0,0));
        zombies[c].Footlnode = smgr->addAnimatedMeshSceneNode(footlmesh,zombies[c].Bodynode,-1,vector3df(0,0,0));
        zombies[c].redeye = smgr->addLightSceneNode(zombies[c].Bodynode, vector3df(0,27,13), SColorf(1.f, 0.f, 0.f, 1.f), 10.f);
        enemysetup(zombies[c]);
    }

//import gun meshes here
    IAnimatedMesh* pistol = smgr->getMesh("pistol4.obj");
    IAnimatedMesh* locket = smgr->getMesh("rocket_launcher3.obj");
    IAnimatedMesh* shotgun = smgr->getMesh("shotgun4.obj");
    IAnimatedMesh* uzi = smgr->getMesh("uzi3.obj");
    IAnimatedMesh* claymoregun = smgr->getMesh("mine_gun.obj");
    IAnimatedMesh* barrelgun = smgr->getMesh("barrel_gun.obj");
    IAnimatedMesh* minegun = smgr->getMesh("mine_gun.obj");
    IAnimatedMesh* chargegun = smgr->getMesh("chargegun.obj");
    IAnimatedMesh* fakewallgun = smgr->getMesh("fakewallgun.obj");
    IAnimatedMesh* railgun = smgr->getMesh("railgun4.obj");

    gun[PISTOL].Gunnode = smgr->addAnimatedMeshSceneNode(pistol,0,-1,vector3df(0,-20,5));
    gun[SHOTGUN].Gunnode = smgr->addAnimatedMeshSceneNode(shotgun,0,-1,vector3df(0,-20,5));
    gun[UZI].Gunnode = smgr->addAnimatedMeshSceneNode(uzi,0,-1,vector3df(0,-20,5));
    gun[BARREL].Gunnode = smgr->addAnimatedMeshSceneNode(barrelgun,0,-1,vector3df(0,-20,5));
    gun[GRENADE].Gunnode = smgr->addAnimatedMeshSceneNode(pistol,0,-1,vector3df(0,-20,5));
    gun[FAKEWALL].Gunnode = smgr->addAnimatedMeshSceneNode(fakewallgun,0,-1,vector3df(0,-20,5));
    gun[ROCKET].Gunnode = smgr->addAnimatedMeshSceneNode(locket,0,-1,vector3df(0,-20,5));
    gun[CLAYMORE].Gunnode = smgr->addAnimatedMeshSceneNode(claymoregun,0,-1,vector3df(0,-20,5));
    gun[RAILGUN].Gunnode = smgr->addAnimatedMeshSceneNode(railgun,0,-1,vector3df(0,-20,5));
    gun[CHARGE].Gunnode = smgr->addAnimatedMeshSceneNode(chargegun,0,-1,vector3df(0,-20,5));
//dfg
    for(c=0; c<200; c++)
        for(k=0; k<10; k++)
        {
            events[c].Eventnode[k] = smgr->addAnimatedMeshSceneNode(footrmesh,0,-1, vector3df(0,2000,0));
            events[c].Eventnode[k]->setVisible(false);
        }
    IAnimatedMesh* hammernails = smgr->getMesh("hammer.obj");
    events[HAMMER_NAILS].Eventnode[0] = smgr->addAnimatedMeshSceneNode(hammernails,0,-1,vector3df(0,0,0));
    events[HAMMER_NAILS].Eventnode[0]->setVisible(true);
    IAnimatedMesh* fridge0 = smgr->getMesh("AFRIDGE0.obj");
    events[STEAL_FRIDGE].Eventnode[0] = smgr->addAnimatedMeshSceneNode(fridge0,0,-1,vector3df(0,0,0));
    events[STEAL_FRIDGE].Eventnode[0]->setVisible(true);
    IAnimatedMesh* fridge1 = smgr->getMesh("AFRIDGE1.obj");
    events[STEAL_FRIDGE].Eventnode[1] = smgr->addAnimatedMeshSceneNode(fridge1,0,-1,vector3df(0,0,0));
    events[STEAL_FRIDGE].Eventnode[1]->setVisible(true);
    IAnimatedMesh* fridge2 = smgr->getMesh("AFRIDGE2.obj");
    events[STEAL_FRIDGE].Eventnode[2] = smgr->addAnimatedMeshSceneNode(fridge2,0,-1,vector3df(0,0,0));
    events[STEAL_FRIDGE].Eventnode[2]->setVisible(true);
    IAnimatedMesh* fridge3 = smgr->getMesh("AFRIDGE3.obj");
    events[STEAL_FRIDGE].Eventnode[3] = smgr->addAnimatedMeshSceneNode(fridge3,0,-1,vector3df(0,0,0));
    events[STEAL_FRIDGE].Eventnode[3]->setVisible(true);
    IAnimatedMesh* fridge4 = smgr->getMesh("AFRIDGE4.obj");
    events[STEAL_FRIDGE].Eventnode[4] = smgr->addAnimatedMeshSceneNode(fridge4,0,-1,vector3df(0,0,0));
    events[STEAL_FRIDGE].Eventnode[4]->setVisible(true);
//ENGINES HERE
    IAnimatedMesh* engine0 = smgr->getMesh("car1.obj");
    events[STEAL_ENGINE].Eventnode[0] = smgr->addAnimatedMeshSceneNode(engine0,0,-1,vector3df(0,0,0));
    events[STEAL_ENGINE].Eventnode[0]->setVisible(true);
    IAnimatedMesh* engine1 = smgr->getMesh("car2.obj");
    events[STEAL_ENGINE].Eventnode[1] = smgr->addAnimatedMeshSceneNode(engine1,0,-1,vector3df(0,0,0));
    events[STEAL_ENGINE].Eventnode[1]->setVisible(true);
    IAnimatedMesh* bathtub = smgr->getMesh("bathtub.obj");
    events[STEAL_TUB].Eventnode[0] = smgr->addAnimatedMeshSceneNode(bathtub,0,-1,vector3df(0,0,0));
    events[STEAL_TUB].Eventnode[0]->setVisible(true);
    IAnimatedMesh* vacuum = smgr->getMesh("vacuum.obj");
    events[STEAL_VACUUM].Eventnode[0] = smgr->addAnimatedMeshSceneNode(vacuum,0,-1,vector3df(0,0,0));
    events[STEAL_VACUUM].Eventnode[0]->setVisible(true);
    IAnimatedMesh* stand = smgr->getMesh("tvstand.obj");
    events[STEAL_STAND].Eventnode[0] = smgr->addAnimatedMeshSceneNode(stand,0,-1,vector3df(0,0,0));
    events[STEAL_STAND].Eventnode[0]->setVisible(true);
    IAnimatedMesh* bed = smgr->getMesh("bedposts.obj");
    events[STEAL_BED].Eventnode[0] = smgr->addAnimatedMeshSceneNode(bed,0,-1,vector3df(0,0,0));
    events[STEAL_BED].Eventnode[0]->setVisible(true);
    IAnimatedMesh* shutter0 = smgr->getMesh("SHUTTER0.obj");
    events[STEAL_SHUTTERS].Eventnode[0] = smgr->addAnimatedMeshSceneNode(shutter0,0,-1,vector3df(0,0,0));
    events[STEAL_SHUTTERS].Eventnode[0]->setVisible(true);
    IAnimatedMesh* shutter1 = smgr->getMesh("SHUTTER1.obj");
    events[STEAL_SHUTTERS].Eventnode[1] = smgr->addAnimatedMeshSceneNode(shutter1,0,-1,vector3df(0,0,0));
    events[STEAL_SHUTTERS].Eventnode[1]->setVisible(true);
    IAnimatedMesh* shutter2 = smgr->getMesh("SHUTTER2.obj");
    events[STEAL_SHUTTERS].Eventnode[2] = smgr->addAnimatedMeshSceneNode(shutter2,0,-1,vector3df(0,0,0));
    events[STEAL_SHUTTERS].Eventnode[2]->setVisible(true);
    IAnimatedMesh* shutter3 = smgr->getMesh("SHUTTER3.obj");
    events[STEAL_SHUTTERS].Eventnode[3] = smgr->addAnimatedMeshSceneNode(shutter3,0,-1,vector3df(0,0,0));
    events[STEAL_SHUTTERS].Eventnode[3]->setVisible(true);
    IAnimatedMesh* shutter4 = smgr->getMesh("SHUTTER4.obj");
    events[STEAL_SHUTTERS].Eventnode[4] = smgr->addAnimatedMeshSceneNode(shutter4,0,-1,vector3df(0,0,0));
    events[STEAL_SHUTTERS].Eventnode[4]->setVisible(true);
    IAnimatedMesh* shutter5 = smgr->getMesh("SHUTTER5.obj");
    events[STEAL_SHUTTERS].Eventnode[5] = smgr->addAnimatedMeshSceneNode(shutter5,0,-1,vector3df(0,0,0));
    events[STEAL_SHUTTERS].Eventnode[5]->setVisible(true);
    IAnimatedMesh* shutter6 = smgr->getMesh("SHUTTER6.obj");
    events[STEAL_SHUTTERS].Eventnode[6] = smgr->addAnimatedMeshSceneNode(shutter6,0,-1,vector3df(0,0,0));
    events[STEAL_SHUTTERS].Eventnode[6]->setVisible(true);
    IAnimatedMesh* shutter7 = smgr->getMesh("SHUTTER7.obj");
    events[STEAL_SHUTTERS].Eventnode[7] = smgr->addAnimatedMeshSceneNode(shutter7,0,-1,vector3df(0,0,0));
    events[STEAL_SHUTTERS].Eventnode[7]->setVisible(true);
    IAnimatedMesh* gate = smgr->getMesh("gate.obj");
    events[STEAL_GATE].Eventnode[0] = smgr->addAnimatedMeshSceneNode(gate,0,-1,vector3df(0,0,0));
    events[STEAL_GATE].Eventnode[0]->setVisible(true);
    IAnimatedMesh* control = smgr->getMesh("panel.obj");
    events[STEAL_CONTROL].Eventnode[0] = smgr->addAnimatedMeshSceneNode(control,0,-1,vector3df(0,0,0));
    events[STEAL_CONTROL].Eventnode[0]->setVisible(true);



//adds lights
//smgr->addLightSceneNode(0,vector3df(1000,400,1000),SColorf(1.0f,1.0f,1.0f,1.0f), 1000.0f);
//smgr->addLightSceneNode(0,vector3df(1000,400,-1000),SColorf(1.0f,1.0f,1.0f,1.0f), 1000.0f);
//smgr->addLightSceneNode(0,vector3df(-1000,400,-1000),SColorf(1.0f,1.0f,1.0f,1.0f), 1000.0f);
//smgr->addLightSceneNode(0,vector3df(-1000,400,1000),SColorf(1.0f,1.0f,1.0f,1.0f), 1000.0f);
//smgr->addLightSceneNode(0,vector3df(-250,210,-170),SColorf(1.0f,1.0f,0.9f,1.0f), 100.0f);
    smgr->addLightSceneNode(0,vector3df(200,210,350),SColorf(1.0f,1.0f,0.9f,1.0f), 160.0f);
//smgr->addLightSceneNode(0,vector3df(-250,210,1073),SColorf(1.0f,1.0f,0.9f,1.0f), 100.0f);
    smgr->addLightSceneNode(0,vector3df(-1420, 210, 2000),SColorf(1.0f,1.0f,0.9f,1.0f), 120.0f);
    smgr->addLightSceneNode(0,vector3df(-1450, 210, -1550),SColorf(1.0f,1.0f,0.9f,1.0f), 160.0f);
//smgr->addLightSceneNode(0,vector3df(-2180, 210, -80),SColorf(1.0f,1.0f,0.9f,1.0f), 100.0f);
//smgr->addLightSceneNode(0,vector3df(-2900, 0, 950),SColorf(1.0f,1.0f,0.9f,1.0f), 100.0f);
    smgr->addLightSceneNode(0,vector3df(2366, 0, -584),SColorf(1.0f,1.0f,0.9f,1.0f), 120.0f);
//smgr->addLightSceneNode(0,vector3df(1794, 0, 1460),SColorf(1.0f,1.0f,0.9f,1.0f), 100.0f);

//adds skybox

    smgr->addSkyBoxSceneNode(
        driver->getTexture("Starscape.png"),
        driver->getTexture("Starscape.png"),
        driver->getTexture("Starscape.png"),
        driver->getTexture("Starscape.png"),
        driver->getTexture("Starscape.png"),
        driver->getTexture("Starscape.png"));
