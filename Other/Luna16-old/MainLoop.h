 #ifdef NDEBUG
    if ( !device->run() ) return 0;
    guienv->clear();
    smgr->clear();

    camera = smgr->addCameraSceneNodeFPS(0, 100, .1f, -1, keyMap, 8);
    camera->setFOV(PI/2);

    // Scene, setup for lights.
     m_cScene = new Scene();
     m_cScene->registerIrrDevice(*device);
     m_cScene->setupLights();
 //   scene::ILightSceneNode* light = smgr->addLightSceneNode( 0, core::vector3df(100,100,100),
 //                                                       video::SColorf(1,1,1,1), 1000.0f );
    device->getCursorControl()->setVisible(false);

    //Vehicle Init
     m_cVehicle = new Vehicle;
     m_cVehicle->registerIrrDevice(*device);
     m_cVehicle->registerPhysics(*m_cPhysics);
     m_cVehicle->registerScene(*m_cScene);
     stringc track = "data/t4.3ds";
     m_cVehicle->loadLevel(track);
     m_cVehicle->initPhysics();

    m_cVehicle2 = new Vehicle;
     m_cVehicle2->registerIrrDevice(*device);
     m_cVehicle2->registerPhysics(*m_cPhysics);
     m_cVehicle2->registerScene(*m_cScene);
    // m_cVehicle2->reverse(1223);
    // m_cVehicle2->initPhysics(); //noneed to call this

    //Create player.
     m_cPlayer = new Player();
     m_cPlayer->registerIrrlichtDevice(*device);
     m_cPlayer->initialize();
     m_cPlayer->setParent(camera);

    device->getGUIEnvironment()->
            addImage( driver->getTexture("data/textures/crosshairs/crosshair1.png"),
                      core::position2d<s32>((resolution[0]/2)-64,(resolution[1]/2)-64));
    //Timers
    int lastFPS = -1;
    u32 timeStamp = device-> getTimer()-> getRealTime(),deltaTime = 0;

 //   AddShader();


    while ( device->run() && !m_cInGameEvents.Quit )
    {

            CheckKeyStates();

                //Physics Timers
                    deltaTime = device->getTimer()->getRealTime() - timeStamp;
                    timeStamp = device->getTimer()->getRealTime();
                    m_cPhysics->updatePhysics(deltaTime);
                    m_cVehicle->renderme();
                   // m_cVehicle2->renderme();
                   // adevice->playAll();
                smgr->drawAll();
                guienv->drawAll();
                driver->endScene();

                btVector3 pos = m_cVehicle->getVehiclePosition();
                vector3df ha = vector3df(pos[0], pos[1]+5, pos[2]);

                if (netManager->getConnectionStatus() != net::EICS_FAILED)
                {
                    netManager->update(0);
                    net::SOutPacket packet;
                	if (login){
                                packet << "hello world";
                                packet << ha;
                                netManager->sendOutPacket(packet);
                                std::cout << "Sending coordse \n " << ha.X << ha.Y << ha.Z ;
                                // return 0;
                     }else{
                          packet << "handshake";  // seems this has to be sent first or nothing works
                          netManager->sendOutPacket(packet);
                          return 3;  // nothing loop around again
                     }
                     packet.compressPacket();
                     packet.encryptPacket("hushthisissecret"); //16 char max
                     netManager->sendOutPacket(packet);
                }
        device->sleep(1,0);
    }

#endif
