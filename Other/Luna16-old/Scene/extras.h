#ifdef EXTRAS

/*    //BitPlaneClouds
        scene::CCloudSceneNode* clouds = new scene::CCloudSceneNode(
                smgr->getRootSceneNode(), smgr,
                    device->getTimer(), 666, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(1,1,1));

        video::ITexture * txture = driver->getTexture("data/cloud.tga");
        srand(time(NULL));
        clouds->setLOD(4);
        clouds->setMaxDepth(4);
        clouds->setMaterialFlag(video::EMF_FOG_ENABLE, true);
        clouds->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        srand(rand());
        clouds->makeRandomCloud(22); //cloudseed
        clouds->setMaterialTexture( 0, txture );
        clouds->setPosition(core::vector3df(0,4000,0));

        scene::ISceneNodeAnimator* cloudsCycle = smgr->createFlyCircleAnimator(core::vector3df(100.0f,0.0f,100.0f), 15000.0f, 0.000006f, core::vector3df(0.f, 1.f, 1.f), 0.4f);
            clouds->addAnimator(cloudsCycle);
            cloudsCycle->drop();
*/

        //Omare's CloudGen
        int nClouds =0;
        int max =10;
        while (nClouds<max)
            {
                int	cloud_x=rand()%2000;
                int cloud_y=rand()%2000;
                int cloud_z=rand()%2000;
                IBillboardSceneNode* cloudgen = smgr->addBillboardSceneNode(0,core::dimension2d<f32>(200, 100));
                cloudgen->setPosition(core::vector3df(cloud_x,cloud_y,cloud_z));
                cloudgen->setMaterialFlag(video::EMF_LIGHTING, false);
                cloudgen->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
                 cloudgen->setMaterialTexture(0,	driver->getTexture("./media/cloudgen/cloud.jpg"));
                 printf("cloud generated");
                nClouds=nClouds+1;
            }

        //Beams
        //origin Cube
        ISceneNode* cube = smgr->addCubeSceneNode();
        cube->setPosition(vector3df(10,0,10));
        cube->setMaterialFlag(EMF_LIGHTING,false);
        cube->setMaterialTexture(0,driver->getTexture("./data/textures/sceneNodes/laserlf7.png"));

        scene::CBeamNode* beam = new scene::CBeamNode(smgr->getRootSceneNode(), smgr, -1, "./data/textures/sceneNodes/laserlf6.png" , "laserlf6.png" );
        //now set the beam
        beam->setLine(core::vector3df(0,0,0), core::vector3df(100,100,100), 5.0f);
        beam->drop();
        scene::CBeamNode* beam1 = new scene::CBeamNode(smgr->getRootSceneNode(), smgr, -1, "./data/textures/sceneNodes/laserlf7.png" , "laserlf6.png" );
        //now set the beam
        beam1->setLine(core::vector3df(0,52,0), core::vector3df(100,0,100), 120.0f);
        beam1->drop();
        scene::CBeamNode* beam2 = new scene::CBeamNode(smgr->getRootSceneNode(), smgr, -1, "./data/textures/sceneNodes/laserlf6.png" , "laserlf6.png" );
        //now set the beam
        beam2->setLine(core::vector3df(0,20,0), core::vector3df(100,100,100), 25.0f);
        beam2->drop();

    //lightning
        ISceneNode* sphere = smgr->addSphereSceneNode(10);
        sphere->setPosition(vector3df(50,50,50));
        sphere->setMaterialFlag(EMF_LIGHTING,false);
        sphere->setMaterialTexture(0,driver->getTexture("./data/textures/sceneNodes/water006.jpg"));

        irr::scene::CBoltSceneNode* lightning = new irr::scene::CBoltSceneNode(smgr->getRootSceneNode(), smgr, -1,"./data/textures/sceneNodes/light01_1.bmp");
        lightning->setLine(irr::core::vector3df(50,50,50), irr::core::vector3df(0,0,0), 100, 5,10,3, false,10.0f, irr::video::SColor(255,0,0,255));
        lightning->drop();

    //GUI/HUD CODE
     //   smgr->addSkyDomeSceneNode(driver->getTexture( "data/textures/skydomes/skydome_1_2048x512.jpg" ), 60,60,1,2);

    //VideoPlayer
        //cVideoPlayer* videoPlayer;
        //videoPlayer = new cVideoPlayer(driver, device->getTimer(), "./media/123.avi",800,600);
        //videoPlayer->setLoop(false);
        //   videoPlayer->play();

       ///end of dont run list
#endif
