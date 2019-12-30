btLogicManager* logicManager = new btLogicManager();
static std::vector<RagDoll*> v_RagDolls;
static std::vector<btRigidBody*> v_Boxes;
irr::scene::ITriangleSelector* selecta = 0;

//DevLoop
    if ( !device->run() ) return 0;

    #include "Scene/particles.h"
    #include "Scene/extras.h"
    #include "Scene/Flag/FlagManager.h"
//Scene, setup for lights.
    guienv->clear();
    smgr->clear();
    //  camera = smgr->addCameraSceneNode(0, vector3df(0.0f,10.0f,0.0f) , vector3df(0.0f,0.0f,0.0f), -1);
    camera = smgr->addCameraSceneNodeFPS(0, 100, .50f, -1, keyMap, 8);
      camera->setFarValue(10000);
    //camera = smgr->addCameraSceneNodeFPS();
    //  camera->setFOV(PI/2);
     m_cScene = new Scene();
     m_cScene->registerIrrDevice(*device);
     m_cScene->setupLights();
    //scene::ILightSceneNode* light = smgr->addLightSceneNode( 0, core::vector3df(100,100,100),video::SColorf(1,1,1,1), 10.0f );
    //debug light smgr->addLightSceneNode(NULL, core::vector3df(0,100,0), video::SColorf(1.f, 1.f, 1.f), 2500, -1);
    device->getCursorControl()->setVisible(false);
    #ifdef IRRc
     device->getCursorControl()->setVisible(true);
     //device->setEventReceiver ( &events );//testing
    #endif
    //!Testing
    Elevator::Instance()->Instance();
    ///Player::Instance()->Instance();//obsolete
    //    Collision::Instance()->Instance();
    ///Player::Instance()->setDevice(device); //obsolete
    //   Collision::Instance()->setDevice(device);

    #ifdef ReflectiveWater
     //Reflective Water
	water = new CReflectedWater("ReflectedWater", device, smgr, -1, 1000, 100, dimension2du(512,512));
	 ISceneNode *waternode = water->m_waternode;
	 waternode->setPosition(vector3df(0, -20, 0));
	 water->m_WaveDisplacement /= 1.5f;
	 water->m_WaveHeight *= 1.0f;
	 water->m_WaveSpeed *= 2.0f;
	 water->m_RefractionFactor = 0.51f;
    #endif

    #ifdef occlusion
        Renderer  RenderMan(device, SColor(0, 10,10,10));
        RenderMan.addSky("data/models/occlusion/gradient3.bmp");
        RenderMan.addLight();
        //a.addWater("042eau.jpg");
        //a.addTerrain();
        RenderMan.loadModel("data/models/occlusion/etage.b3d", vector3df(0,0,0));
        RenderMan.addCam(.1f, vector3df(40,70,40), vector3df(0,0,1000), true);
        //a.addMy3DScene("models/bedroom1/bedroom.my3d", 0,0,0);
        RenderMan.initGUI();
    #endif

/// PHYSICS
    #ifdef BULLETCAR
    //Vehicle Init
        m_cVehicle = new Vehicle;
         m_cVehicle->registerIrrDevice(*device);
         m_cVehicle->registerPhysics(*m_cPhysics);
         m_cVehicle->registerScene(*m_cScene);
         m_cVehicle->registerCamera(camera);
        // m_cVehicle->createTerrain(vector3df(0,10.0f,0),"data/textures/heightmaps/ctown.png");
     //!testing second vehicle
     // m_cVehicle2 = new Vehicle;
     //  m_cVehicle2->registerIrrDevice(*device); m_cVehicle2->registerPhysics(*m_cPhysics);
     //  m_cVehicle2->registerScene(*m_cScene); m_cVehicle2->reverse(1223);
     //  m_cVehicle2->initPhysics();
     //  m_cVehicle2->setVehiclePosition(vector3df(10,0,10));
     //  ISceneNode* nodee = smgr->addCubeSceneNode(1);
     //  btVector3 btCarScale = m_cVehicle2->btCarScale;
     //   node->setScale(vector3df(btCarScale[0],btCarScale[1],btCarScale[2]));
     //   node->setMaterialFlag(video::EMF_LIGHTING, false);
    #endif
    #ifdef BULLETBLEND
    	int verboseDumpAllTypes = 0;
        int verboseDumpAllBlocks = 0;
        const char* fileName = "./data/clubsilo_packed.blend";
        FILE* file = fopen(fileName,"rb");
    	int fileLen;
        char*memoryBuffer =  btReadBuffer(file,&fileLen);

		IrrBlendNew	bulletBlendReaderNew(device ,smgr,m_cPhysics->getDynamicsWorld(),logicManager);
		if (!bulletBlendReaderNew.readFile(memoryBuffer,fileLen,verboseDumpAllTypes))//if (!bulletBlendReader.openFile("./data/huge.blend"))
		{
			fclose(file);
			exit(0);
		}
		bulletBlendReaderNew.convertAllObjects(verboseDumpAllBlocks);
		fseek(file, 0, SEEK_SET); /* seek to start */
		//		m_cPhysics->dynamicsWorld->setGravity(btVector3(0,-60,0));
    #endif
    #ifdef RAG
        v_RagDolls.push_back(new RagDoll(m_cPhysics->getDynamicsWorld(),smgr, btVector3(0,55,0), btScalar(5.0)));
    #endif
    #ifdef BULLETCAR
        m_cVehicle->initPhysics();  //if its loaded before the bullet level it crashes
    #endif

    #ifdef LOADLEVELS
        //stringc track = "aa.irrmesh";
        stringc track = "huge.3ds";
        device->getFileSystem()->changeWorkingDirectoryTo("data/Stage3/");
        device->getSceneManager()->loadScene("Stage3.irr");  // LOADSCENE not LOADTRACK
        device->getFileSystem()->changeWorkingDirectoryTo("../../");
        device->getFileSystem()->changeWorkingDirectoryTo("data/3/");
        m_cVehicle->loadLevel(track.c_str());
        device->getFileSystem()->changeWorkingDirectoryTo("../../");

//        // Create a meta triangle selector to hold several triangle selectors.
//        scene::IMetaTriangleSelector * metaSelector = smgr->createMetaTriangleSelector();
//        m_cVehicle->recursiveFillMetaSelector( smgr->getRootSceneNode(), metaSelector );
//
//        scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(
//                metaSelector, camera, core::vector3df(5,5,5),
//                core::vector3df(0,0,0));
//        metaSelector->drop(); // I'm done with the meta selector now
//        camera->addAnimator(anim);
//        anim->drop(); // I'm done with the animator now
//
//        //reads xml model names then removes the node
//        ISceneNode *node = 0;
//        u32 i = 0;
//
//        while((node = device->getSceneManager()->getSceneNodeFromName("powercell", 0)))
//         {
//
//            if(node)
//             {
//                //   PowerCell::Instance()->Add(node->getPosition());
//                node->remove();
//                printf("powercell Node removed");
//                i++;
//             }
//        while((node = device->getSceneManager()->getSceneNodeFromName("playerstart2", 0)))
//         {
//            if(node)
//             {
//                //   PowerCell::Instance()->Add(node->getPosition());
//                node->remove();
//                printf("playerStart Node removed");
//                i++;
//             }
//         }
//         }
    #endif

    #ifdef TERRAIN
     terr = new Terrain;
        terr->registerIrrDevice(*device);
        terr->registerScene(*m_cScene);
        terr->registerPhysics(*m_cPhysics);
        terr->Init();
        terr->Render(vector3df(0,0.0f,0)); // position, rotation, scale
        #ifdef TREES
            terr->MakeTrees();
        #endif
    #endif



        device->getGUIEnvironment()->addImage( driver->getTexture("data/textures/crosshairs/crosshair1.png"),
                                            core::position2d<s32>((resolution[0]/2)-64,(resolution[1]/2)-64));
        #ifdef HUD
         CHUD2->RegisterDevice(device);
         CHUD2->LoadHUD("data/HUD.xml");

        CVideoMaster    *vidmaster;
        vidmaster = new CVideoMaster(device, true, 1);
        ITexture* cubeDiffuse = vidmaster->addVideoClip("./media/bunny.ogg", "TV", ESM_NONE, true);
        IMeshSceneNode *TV = smgr->addCubeSceneNode(50);
        TV->setScale(vector3df(1.0f, 1.0f, 0.001f));
        // Set video texture as diffuse
        TV->setMaterialTexture(0, cubeDiffuse);
        TV->setMaterialFlag(EMF_LIGHTING, false);
        //vidmaster->addVideoClip("./media/YUVTest.ogg", "YUV", dimension2du(128,128), true);
        #endif

        #ifdef ATMOSPHERE
         ATMOsphere *atmo;
         atmo=new ATMOsphere;
         atmo->start(device,driver,smgr->getRootSceneNode(),smgr,666);
         atmo->setDaysPerDay(500);
        #endif

    #ifdef FPSWEAPON
        IAnimatedMesh*   gunmesh = smgr->getMesh("./data/models/weapons/M4/3d_person/M4A1d.3ds");
          scene::IAnimatedMeshSceneNode* agun;
          agun = smgr->addAnimatedMeshSceneNode(gunmesh);
          if (agun)
          {
            agun->setMaterialTexture(0, driver->getTexture("./data/models/weapons/M4/1st_person/M4A1.jpg"));
            agun->setScale(core::vector3df(1.2f, 1.2f, 1.2f));
            agun->setPosition(core::vector3df(180.f,1.5f,0.f));
            agun->setRotation(core::vector3df(0.f,0.f,90.f));
            //agun->apply_light2node(agun);
        	agun->addShadowVolumeSceneNode();
          }
          gunmesh->drop();
       // scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100.0f, .5f,-1,keyMap,8);
                                  // cam->setPosition(core::vector3df(-200,100,200));
                                 //  cam->setTarget(core::vector3df(0,0,0));
        camera->setFOV(PI/2);
        camera->setFarValue(7000);

        firstPersonWeapon* M4;
        M4 = new firstPersonWeapon(device, camera);
        //  apply_light2node(M4->getNode());
        device->setEventReceiver(M4);
    #endif
    #ifdef COMPASS
        ///Compass
           core::rect<s32> CompassRect;
            CompassRect.UpperLeftCorner.X = 50;
            CompassRect.UpperLeftCorner.Y = 50;
            CompassRect.LowerRightCorner.X = 150;
            CompassRect.LowerRightCorner.Y = 150;

            gui::IGUIElement* pgRootGuiElement = guienv->getRootGUIElement();

           Compass * Compass1 = new Compass( CompassRect, guienv, pgRootGuiElement );
            video::ITexture * CompassBodyTexture =   driver->getTexture("media/compass/compass_body.png");
            video::ITexture * CompassNeedleTexture =   driver->getTexture("media/compass/compass_needle.png");
            Compass1->SetCompassBodyTexture( CompassBodyTexture );
            Compass1->SetCompassNeedleTexture( CompassNeedleTexture );
    #endif

    #ifdef DSOUND
    manager = cAudio::createAudioManager(false);
	if(manager)
	{
		//Initialize the manager with the user settings
		manager->initialize(manager->getAvailableDeviceName(5));
		//Create a IAudio object and load a sound from a file
		mysound = manager->create("bling","./media/bling.ogg",false);
	}
    #endif

    #ifdef PostProcess
     //PostProcessing
        IPostProc* ppRenderer = new CRendererPostProc( smgr, dimension2du( 1024, 512 ),
                                                    true, true, SColor( 255u, 100u, 101u, 140u ) );

        CEffectPostProc* ppBlurDOF = new CEffectPostProc( ppRenderer, dimension2du( 1024, 512 ), PP_BLURDOF );
        CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer, dimension2du( 1024, 512 ), PP_BLUR, 0.00081f );
                            ppBlur->setQuality( PPQ_GOOD );
    #endif

    #ifdef PYTHON
    //Python
        Py_Initialize();            //Initialize Python
        Python::init_irr();         //Initialize our module
        Python::registerScene(*smgr);
        Python::ExecuteScript("./media/ss.pys"); //Using our handy dandy script execution function
    #endif


    #ifdef FORMATIONDEMO
    // FORMATION DEMO
    WrapperClass::Formation * formation =
        new WrapperClass::Formation(camera->getAbsolutePosition(), 100.f, 5, 30, 100.f, 100.f, smgr);
    	///const irr::core::aabbox3df& tb = terrain->getBoundingBox();
    #endif


    #ifdef BOIDS
        const irr::f32 borders[4] = { 1.0f, 222, 1.0f, 222}; //Xmin, Xmax, Zmin, Zmax
        Flock* flock;

        flock = new Flock( irr::core::vector3df(0.0f, 0.0f, 0.0f), borders);
        flock->registerScene(*smgr);
        	//create flock
        //	flock->boidSelector = smgr->createMetaTriangleSelector();
        irr::scene::IMesh* meshy = smgr->getMesh("./media/dwarf.x");
        //add boids to flock
        for (irr::u32 i = 0; i < 30; ++i)
		flock->addBoid(meshy);

        irr::video::SMaterial mat;
        mat.TextureLayer[0].Texture = driver->getTexture("media/boids/star.bmp");
        mat.EmissiveColor = irr::video::SColor(255, 255, 0, 0);
        mat.SpecularColor = irr::video::SColor(0, 0, 0, 0);
        mat.DiffuseColor =  irr::video::SColor(0, 0, 0, 0);
        mat.Shininess = 128.0f;
        mat.NormalizeNormals = true;
        irr::scene::IMeshSceneNode* const cube = smgr->addCubeSceneNode(50.0f, 0, -1, irr::core::vector3df(0, 100, 0.0f));
        cube->getMesh()->setHardwareMappingHint(irr::scene::EHM_STATIC);
        cube->getMaterial(0) = mat;

        selecta = smgr->createTriangleSelector(cube->getMesh(), cube);
        cube->setTriangleSelector(selecta);

        flock->setTarget(irr::core::vector3df(0.0f, 0.0f, 0.0f));
        flock->setPaused(false);
        //selector->addTriangleSelector(selecta);
        //	selecta->drop();
    #endif


    #ifdef PLAYER
        //Player.
        m_cPlayer = new Player();
         m_cPlayer->registerIrrlichtDevice(*device);
         m_cPlayer->initialize();
         m_cPlayer->setParent(camera);
         //Collision::Instance()->createPlayerCollision();
            //m_cPlayer->createShadow();
        //m_cPlayer->CheckFalling();

        scene::ILightSceneNode* light1 =
            smgr->addLightSceneNode(0, core::vector3df(0,60,0),
            video::SColorf(0.5f, 1.0f, 0.5f, 0.0f), 800.0f);
    #endif

    #ifdef Image2D
        cImage* image = new cImage(driver->getTexture("data/sydney.bmp"),
                                    rect<s32>(0,256,512,512),rect<s32>(0,0,512,256));
        image->SetTransparent(true);
        image->SetTransparentColor(SColor(0,255,0,255),driver);
        image->SetBilinearFilter(true);
    #endif

    #ifdef IRRc
        Application app(device);
        if (app.init())
    #endif

    #ifdef EDITOR
        CGUIEditBoxIRB * codeEditor = new CGUIEditBoxIRB(L"",true,true,guienv,guienv->getRootGUIElement(),-1,rect<s32>(20,60,600,400),device);
    #endif

    #ifdef DECALS
        //! Create decal manager
        DecalManager* decalManager = new DecalManager(smgr);
        // Create a decal
        irr::core::vector3df position = irr::core::vector3df(80, 80, 80);
        irr::core::vector3df dimension = irr::core::vector3df(10, 10, 100);
        irr::core::vector3df normal = irr::core::vector3df(1, 1, 0);
        irr::f32 textureRotation = 40;
        //irr::scene::ISceneNode* parent = 0;
        irr::f32 lifeTime = 0;
        irr::f32 distance = 11;
        //Position to place the decal        //Dimension of decal        //Orientation of the decal        //Rotation in degrees        //Parent        //Time to life        //Max viewing distance
            scene::IMeshSceneNode* sphere = smgr->addSphereSceneNode(25.f, 100);
            sphere->setPosition(core::vector3df(80, 80, 80));
            sphere->setScale(core::vector3df(20, 20, 20));
            decalManager->addMesh(sphere);
        //decalManager->addDecal("media/sydney.bmp", position, dimension, normal, textureRotation, sphere, lifeTime, distance);
    #endif

    video::ITexture* image = driver->getTexture("data/textures/bullet.png");

    for(int i=0; i<MAX_DECALS; i++){
        decals[i] = new DecalSceneNode(smgr->getRootSceneNode(), smgr, image, 15.2f);
    //decals[i]->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    //decals[i]->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
    //decals->setMaterialFlag(video::EMF_LIGHTING, true);
    //decals[i]->getMaterial(0)->setFlag();

    }
        int nextDecal=0;


    //Timers
        int lastFPS = -1;
        u32 timeStamp = device-> getTimer()-> getRealTime(),deltaTime = 0;
        u32 then = device->getTimer()->getTime();
/**
////////////////////////////////////////////////////////////////////////
//      MAINLOOP  //////////////////////////////////
///////////////////////////////////
**/

    while ( device->run() && !m_cInGameEvents.Quit )
    {

        driver->beginScene ( true, true, SColor ( 0, 0, 0, 0 ) );

		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

        deltaTime = device->getTimer()->getRealTime() - timeStamp;
        timeStamp = device->getTimer()->getRealTime();

        #ifdef PostProcess
        //f32 p = sinf( device->getTimer( )->getTime( ) * 0.0005f ) * 0.5f - 0.2f;
        //ppBlurDOF->setParameters( p * 100.0f + 80.0f, p * 100.0f + 110.0f, p * 100.0f + 160.0f, p * 100.0f + 240.0f, 0.01f );
        #endif

        #ifdef SPARKA
		// lightmap effect
      //  if(deltaTime >= 0.05f)
      //  {

            float decal = SPK::random(0.95f,1.05f);

            for(int y=0; y<2; y++)
                for(int x=0; x<2; x++)
                    ((video::S3DVertex2TCoords*)(buff->getVertices()))[x+2*y].TCoords2 = core::vector2df((x-0.5f)*decal+0.5f,(y-0.5f)*decal+0.5f);
//            lastLightTime = time;

       // }
        #endif

        CheckKeyStates();

        #ifdef DPHYSICS
                    m_cPhysics->updatePhysics(deltaTime);
                    #ifdef BULLETCAR
                   ///  btVector3 pos = m_cVehicle->getVehiclePosition();
                     // m_cVehicle2->renderme(); // no need to update unless running 2 players from same computer
                     m_cVehicle->renderme();
                     // m_cVehicle2->setVehiclePosition(vector3df(pos[0]+20,pos[1],pos[2])); //ghostCar
                     // camera->setPosition( vector3df(pos[0],pos[1]+4,pos[2]));  //sticking the camera to the car
                    #endif
                #ifdef BULLETBLEND
                    logicManager->processLogicBricks(deltaTime);
                #endif
          #ifdef RAG
            for (std::vector<RagDoll*>::iterator it = v_RagDolls.begin(); it != v_RagDolls.end(); ++it)
            (*it)->Update();
          #endif
        #endif

                #ifdef ReflectiveWater
                    #ifdef ReflectWater-Main
                     water->updateRendertarget(smgr);
                    #endif
                #endif

                #ifdef COMPASS
                //Compass --BRJ correct way to get heading posted by Vitek
                  core::vector3df fore(0, 0, 1);
                  camera->getAbsoluteTransformation().rotateVect(fore);
                  core::vector3df rot1 = fore.getHorizontalAngle();
                  Compass1->SetCompassHeading( rot1.Y );
                #endif
              //  m_cPlayer->CheckFalling();
//                Obstacle::Instance()->CheckCollisionWithPlayer();
        //        Elevator::Instance()->CheckCollisionWithPlayer();
        //        Elevator::Instance()->UpdatePlayerPosition();

            #ifdef DSOUND
            if(mysound && !mysound->isPlaying())
            {
                mysound->setVolume(0.5);
                //Set the IAudio Sound to play2d and loop
                mysound->play2d(false);
            }
            #endif

            smgr->drawAll();


            #ifdef PostProcess
             ppBlur->render( NULL );
             //	ppBlurDOF->render( NULL );
            #endif

            guienv->drawAll();

            #ifdef FPSWEAPON
             M4->update(device->getTimer()->getTime());
             if (M4->isKeyDown(KEY_ESCAPE)) break;
            #endif


            #ifdef HUD
                CHUD2->DisplayHUD(); // for displaying text
            //Video
                //  videoPlayer->refresh();
                //  videoPlayer->drawVideoTexture();
              ITexture* temp = vidmaster->getTextureByName("Titan");
              temp = vidmaster->getTextureByName("YUV");
              if (temp)
              {
                driver->draw2DImage(temp, vector2di(driver->getScreenSize().Width - 128, driver->getScreenSize().Height - 128));
              }
              vidmaster->update();
            #endif

            #ifdef FORMATIONDEMO
            // vector3df pos2 = camera->getPosition();
           //  SpeedMultiplier=.00001f;
            // formation->DesiredPosition = pos2; // desiredPosition;
            // formation->process(deltaTime);
            #endif

            #ifdef BOIDS
             flock->update(selecta,deltaTime*0.0001f,0);
            #endif

            #ifdef COMPASS
            Compass1->draw();
            #endif

            #ifdef ATMOSPHERE
             atmo->update(driver);//update all sky/sun
            #endif

            #ifdef occlusion
             RenderMan.drawGUI();
            #endif

            #ifdef Image2D
            // IMAGE TESTING
            image->Draw(smgr);
            #endif
        driver->endScene();

            #ifdef occlusion
            RenderMan.updateCulling();
            #endif

        int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Luna Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
       device->sleep(5);

    }

#ifdef HUD
      delete vidmaster;
#endif
    #ifdef BOIDS
     delete flock;
    #endif

    #ifdef PostProcess
	 delete ppBlurDOF;
	 delete ppBlur;
	 delete ppRenderer;
    #endif

	#ifdef ATMOSPHERE
     delete atmo;
    #endif

	#ifdef ReflectiveWater
	 delete water;
	#endif

	#ifdef RAG
		for (std::vector<RagDoll*>::iterator it = v_RagDolls.begin(); it != v_RagDolls.end(); ++it)
            (*it)->~RagDoll();
	#endif

	//delete CHUD2;
	//delete m_cVehicle;

	#ifdef COMPASS
	 delete Compass1;
	#endif

	#ifdef FLAG     // should be the flagmanager
	delete irrFlagNode;
	#endif

	#ifdef FLAG2     // should be the flagmanager
	delete flag;
	#endif

	#ifdef PYTHON
     Py_Finalize();
    #endif

    #ifdef DSOUND
		manager->releaseAllSources();
		manager->shutDown();
        cAudio::destroyAudioManager(manager);
    #endif

#ifdef SPARKA
	cout << "\nSPARK FACTORY BEFORE DESTRUCTION :" << endl;
	SPKFactory::getInstance().traceAll();
	SPKFactory::getInstance().destroyAll();
	cout << "\nSPARK FACTORY AFTER DESTRUCTION :" << endl;
	SPKFactory::getInstance().traceAll();
	device->drop();
#endif

//	delete videoPlayer;
