

    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
    std::cout << "IMP - IRRLICHT MUSIC PLAYER" << std::endl;
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;

    /// Fill this array: (Can click again?)
    int arrCASize = sizeof(canClickAgain)/sizeof(int);       // Count array items
    for(signed int i=0; i<arrCASize; i++){ canClickAgain[i] = 1; }  // Fill array

    /// Ok, now lets get all settings from the external settings file:
    readSettingsFromFile();

	/// Loop to chose driver from all available:
    video::E_DRIVER_TYPE driverType;
    for (int i=0; i<irr::video::EDT_COUNT; i++){
    if (irr::IrrlichtDevice::isDriverSupported(irr::video::E_DRIVER_TYPE(i))){ driverType = irr::video::E_DRIVER_TYPE(i); };
    };


    /// Create device from the above found driver:
	device = createDevice(EDT_OPENGL, core::dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), 16, FULLSCREEN, STENCIL_SHADOWS, VSYNC, 0); /// OK!... Or using the next code...
	/// device = createDevice(deviceType = video::EDT_SOFTWARE, core::dimension2d< u32 >(640, 480), u32 bits = 16, bool fullscreen = false, bool stencilbuffer = false, bool vsync = false, IEventReceiver* receiver = 0

    /*
	SIrrlichtCreationParameters params = SIrrlichtCreationParameters();
	params.DriverType  = driverType;
	params.WindowSize  = core::dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT);
	params.ZBufferBits = 16;
	params.Fullscreen  = FULLSCREEN;
	params.StencilBuffer = false;
	params.Vsync = false;
    params.AntiAlias   = true;
    params.Event = 0;
    device = createDeviceEx(params);
    */

    if (device == 0){ return 1; }; /// If device fails... Do nothing...

	driver  = device->getVideoDriver();     // Create Driver
	filesys = device->getFileSystem();
    guienv  = device->getGUIEnvironment();  // For GUI texts
	smgr    = device->getSceneManager();    // Create Scene Manager
	// video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices(); // Not used, by now...


	/// For STENCIL_SHADOWS:
	/// Set shadows colors:
	smgr->setShadowColor(video::SColor(150,0,0,0)); // a (0 = transparent | 255 = opaque), r, g, b
	/// Some tweaks to accelerate the shadows process:
	// device->IsUseStencilBuffer();
    // scene->setShadowDelayerMax(2);
    // scene->setAutoShadowDelayer(true);


	// Add an events receiver: (keyboard)
	MyEventReceiver receiver;
    device->setEventReceiver(&receiver);

    /// Device: Other Stuff: (Window)
    device->setWindowCaption(windowtexts.c_str());
	device->getCursorControl()->setVisible(true);  // Mouse cursor visibility
	device->setResizable(true);                    // Resizable window

	/// Driver: Other Stuff: (for transparencies)
	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_ALWAYS_32_BIT, true);
	driver->setTextureCreationFlag(E_TEXTURE_CREATION_FLAG::ETCF_CREATE_MIP_MAPS, true);

    /// Materials:
	driver->getMaterialRenderer(video::EMT_PARALLAX_MAP_SOLID);

    /// Driver: Other Stuff: (for smoothness)
	driver->getMaterial2D().TextureLayer[0].BilinearFilter = true;
	driver->getMaterial2D().TextureLayer[1].BilinearFilter = true;
	driver->getMaterial2D().TextureLayer[2].BilinearFilter = true;
	driver->getMaterial2D().TextureLayer[3].BilinearFilter = true;
    driver->getMaterial2D().AntiAliasing = video::EAAM_FULL_BASIC;



    std::cout << "\n\nInternal Datas:" << std::endl;
	/// Load all needed files: (package is a ZIP file, in encryption level: AES-128)
	// http://irrlicht.sourceforge.net/docu/classirr_1_1io_1_1_i_file_system.html#a05de4e45b4d36c779b9b8f7cb1dccf7c
    // device->getFileSystem()->addFileArchive("data/world.dll", true, false, EFAT_ZIP, ""); // String filename, ignoreCase, ignorePaths, fileType, password
    device->getFileSystem()->addFileArchive("world/world.zip", true, true, EFAT_ZIP, "none"); // String filename, ignoreCase, ignorePaths, fileType, password
    std::cout << "file \"world.zip\"... Ok" << std::endl;
	// Load all from... (deprecated: addZipFileArchive)
	// device->getFileSystem()->addZipFileArchive("data/world.zip");

	// applyICON();

    /// Loading Bar: #1 (Initialize)
    /// driver->removeTexture(backgroundImageTexture);
    /// backgroundImageTexture = driver->getTexture("courtain.jpg");
    LoadBar = new CLoadingBar(device, driver, guienv, NULL, rect<s32>(300, SCREEN_HEIGHT/2, SCREEN_WIDTH-300, SCREEN_HEIGHT/2+15), true, 7);  // posX, posY, bar width, bar height
    LoadBar->setPercent(0); // 0 to 100
    LoadBar->draw();

    /// ##########################################################################################
    /// ############# AUDIO: (USING AUDIERE LIBRARY) - END #######################################
    /// ##########################################################################################

    std::cout << "\n\nInternal Audios:" << std::endl;

    /// Load .OGG sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFile00 = device->getFileSystem()->createAndOpenFile("audio_intro.ogg");
    void* buffer00 = malloc(readFile00->getSize());
    readFile00->read(buffer00, readFile00->getSize());
    // Create memory file:
    FilePtr targetFile00 = CreateMemoryFile(buffer00, readFile00->getSize()); // targetFile11 is the file (path) that we want !!!
    free(buffer00);
    readFile00->drop();

    /// Load .OGG sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFile11 = device->getFileSystem()->createAndOpenFile("audio_ooo.ogg");
    void* buffer11 = malloc(readFile11->getSize());
    readFile11->read(buffer11, readFile11->getSize());
    // Create memory file:
    FilePtr targetFile11 = CreateMemoryFile(buffer11, readFile11->getSize()); // targetFile11 is the file (path) that we want !!!
    free(buffer11);
    readFile11->drop();

    /// Load .OGG sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFile22 = device->getFileSystem()->createAndOpenFile("audio_applause.ogg");
    void* buffer22 = malloc(readFile22->getSize());
    readFile22->read(buffer22, readFile22->getSize());
    // Create memory file:
    FilePtr targetFile22 = CreateMemoryFile(buffer22, readFile22->getSize()); // targetFile22 is the file (path) that we want !!!
    free(buffer22);
    readFile22->drop();

    /// Load .OGG sounds (valid too, to any other type of file) get a file from the ZIP into memory (buffer):
    irr::io::IReadFile* readFile33 = device->getFileSystem()->createAndOpenFile("audio_click.ogg");
    void* buffer33 = malloc(readFile33->getSize());
    readFile33->read(buffer33, readFile33->getSize());
    // Create memory file:
    FilePtr targetFile33 = CreateMemoryFile(buffer33, readFile33->getSize()); // targetFile33 is the file (path) that we want !!!
    free(buffer33);
    readFile33->drop();

    /// OK!... Now lets load Sounds to AUDIERE:
    if(!audiereAudioDevice){ std::cout << "ERROR: Audiere: Cant open audio device! \n" << std::endl; exit(1); };
    // if(!audiereMidiDevice ){ std::cout << "ERROR: Audiere: Cant open midi device! \n"  << std::endl; exit(1); };

    soundIntro = audiere::OpenSound(audiereAudioDevice.get(), targetFile00, true); // Load music into memory? true / false
    soundIntro->setVolume(GLOBAL_VOLUME/2);
    soundIntro->setPan(0);    // Left, 1 Right
    soundIntro->setRepeat(0); // 1 loop, 0 don't loop
    // soundIntro->play();
    std::cout << "Audio \"audio_intro.ogg\"... Ok" << std::endl;

    soundOoo = audiere::OpenSound(audiereAudioDevice.get(), targetFile11, true); // Load music into memory? true / false
    soundOoo->setVolume(GLOBAL_VOLUME/3);
    soundOoo->setPan(0);    // Left, 1 Right
    soundOoo->setRepeat(0); // 1 loop, 0 don't loop
    // soundOoo->play();
    std::cout << "Audio \"audio_booo.ogg\"... Ok" << std::endl;

    soundClaps = audiere::OpenSound(audiereAudioDevice.get(), targetFile22, true); // Load music into memory? true / false
    soundClaps->setVolume(GLOBAL_VOLUME/2);
    soundClaps->setPan(0);    // Left, 1 Right
    soundClaps->setRepeat(0); // 1 loop, 0 don't loop
    // soundClaps->play();
    std::cout << "Audio \"audio_applause.ogg\"... Ok" << std::endl;

    soundClick = audiere::OpenSound(audiereAudioDevice.get(), targetFile33, true); // Load music into memory? true / false
    soundClick->setVolume(GLOBAL_VOLUME/2);
    soundClick->setPan(0);    // Left, 1 Right
    soundClick->setRepeat(0); // 1 loop, 0 don't loop
    // soundClick->play();    // 1 loop, 0 don't loop
    std::cout << "Audio \"audio_click.ogg\"... Ok" << std::endl;


    /// ##########################################################################################
    /// ############# AUDIO: (USING AUDIERE LIBRARY) - END #######################################
    /// ##########################################################################################




    /// Loading Bar: #2
    LoadBar->setPercent(10); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);





    /// ############# DEVELOPER NOTES ################################################################

    /// .3DS files exported from 3DMax 2017 loads OK. Don't support animations
    /// .blend files cant load
    /// .dae files (Collada) handles lights very well. Don't support animations

    /*
    /// Create (Load Animated Object) for INTERNAL TESTS PURPOSES only:
	scene::IAnimatedMeshSceneNode* nodeTests = smgr->addAnimatedMeshSceneNode(smgr->getMesh("stage.obj"));
	/// if(nodeTests){
	scene::IMeshBuffer* buffer92 = smgr->getMesh("stage.obj")->getMesh(0)->getMeshBuffer(0);
    int vc92 = buffer92->getVertexCount();
    std::cout << "Vertex count: " << vc92 << std::endl;
	nodeTests->setPosition(vector3df(0, 100, 0));
	/// nodeTests->setRotation(vector3df(0, 180, 0));
	nodeTests->setScale(core::vector3df(1.0,1.0,1.0));
	nodeTests->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeTests->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeTests->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    // nodeTests->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeTests->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeTests->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Set animation frames limits:
    f32 framestartb = nodeTests->getStartFrame();
    f32 frameendb = nodeTests->getEndFrame()-1;
    /// nodeTests->setFrameLoop(framestartb, frameendb);
    /// nodeTests->setCurrentFrame(framestartb);
    nodeTests->setAnimationSpeed(1);
    nodeTests->setLoopMode(true);
    nodeTests->setVisible(true);
	/// };
	*/

    /*
    for(u32 i=0; i<nodeTests->getMaterialCount(); i++){ // nodeTests->getMaterialCount(); // 40
	nodeTests->getMaterial(i).setTexture(0, driver->getTexture("stage_floor.jpg"));
	nodeTests->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeTests->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeTests->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeTests->getMaterial(i).Lighting = true;
	nodeTests->getMaterial(i).Shininess = 10; // Set size of specular highlights (20.0f == Metallic)
	nodeTests->getMaterial(i).SpecularColor.set(10,10,10,10); /// The lights reflexes
    // nodeTests->getMaterial(i).Shininess = 0.0f; // Set size of specular highlightss (20.0f == Metallic)
	// nodeTests->getMaterial(i).SpecularColor.set(155,155,155,155);
	// nodeTests->getMaterial(i).DiffuseColor.set(255,255,255,255);
	// nodeTests->getMaterial(i).EmissiveColor.set(0,0,0,0);
	// nodeTests->getMaterial(i).getTextureMatrix(1).buildTextureTransform(1, core::vector2df(-0.5f, -0.5f*5), core::vector2df(0.5f,0.5f), core::vector2df(5, 5));
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeTests->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeTests->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeTests->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};
	*/







    /// #############################################################################################







    /// ############# LOAD NODES ################################################################

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Load MESHES:" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    /// Create (Loads Object) STAGE:
	scene::IAnimatedMeshSceneNode* nodeStage = smgr->addAnimatedMeshSceneNode(smgr->getMesh("stage.obj")); // For an animated node
	// scene::ISceneNode* nodeStage = smgr->addMeshSceneNode(smgr->getMesh("stage.obj")->getMesh(0));

	scene::IMeshBuffer* buffer1 = smgr->getMesh("stage.obj")->getMesh(0)->getMeshBuffer(0);
    int vc1 = buffer1->getVertexCount();
    std::cout << "Stage vertex count: " << vc1 << std::endl;
	// scene::IMeshSceneNode* nodeStage = smgr->getMeshManipulator()->createMeshWithTangents(smgr->getMesh("cena.obj")->getMesh(0));
	// scene::IMeshSceneNode* nodeStage = smgr->addMeshSceneNode(smgr->getMesh("cena.obj")->getMesh(0));
	nodeStage->setPosition(vector3df(0, 100, 0));
	nodeStage->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeStage->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStage->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeStage->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStage->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    for(u32 i=0; i<nodeStage->getMaterialCount(); i++){ // nodeStage->getMaterialCount(); // 40
	nodeStage->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStage->getMaterial(i).getTextureMatrix(1).setTextureScale(2.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeStage->getMaterial(i).Lighting = true;
	nodeStage->getMaterial(i).Shininess = 70.0f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStage->getMaterial(i).SpecularColor = video::SColor(200,100,100,100); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStage->getMaterial(i).AmbientColor  = video::SColor(200,200,200,200); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStage->getMaterial(i).DiffuseColor  = video::SColor(200,200,200,200); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStage->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors


    /*
    /// Start Colors:
	/// Set size of specular highlights
	nodeStage->getMaterial(i).Lighting = true;
	nodeStage->getMaterial(i).Shininess = 70.0f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStage->getMaterial(i).SpecularColor = video::SColor(150,100,100,100); /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Tweak all the light related aspects of the material:
    nodeStage->getMaterial(i).AmbientColor  = video::SColor(200,150,150,150); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStage->getMaterial(i).DiffuseColor  = video::SColor(150,100,100,100); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStage->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors
    */

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStage->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStage->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
    nodeStage->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust    nodeStage->getMaterial(i).Shininess = 12.0f; // Set size of specular highlights (20.0f == Metallic)
	};


	// nodeStage->setMaterialType(video::EMT_DETAIL_MAP); // Needed for multitexturing (Apply (mix) texture to my specified material)
	/// Material Type --> http://irrlicht.sourceforge.net/docu/namespaceirr_1_1video.html#ac8e9b6c66f7cebabd1a6d30cbc5430f1
	/// Material Type --> http://webcache.googleusercontent.com/search?q=cache:r8-qBiunQboJ:irrlicht.sourceforge.net/tut011.html+irrlicht+animator+rotation&cd=2&hl=pt-PT&ct=clnk&gl=pt&source=www.google.pt
    nodeStage->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER
    nodeStage->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)

    std::cout << "Mesh file \"stage.obj\"... Ok" << std::endl;

    std::cout << "\n\nMeshes:" << std::endl;


    /// Create (Loads Object) STAGE:
	/// scene::ISceneNode* nodeStageFloor = smgr->addMeshSceneNode(smgr->getMesh("stage_floor.obj")->getMesh(0));
	scene::IMeshSceneNode* nodeStageFloor = smgr->addMeshSceneNode(smgr->getMesh("stage_floor.obj"));
	scene::IMeshBuffer* buffersf = smgr->getMesh("stage_floor.obj")->getMesh(0)->getMeshBuffer(0);
    int vcsf = buffersf->getVertexCount();
    std::cout << "Stage floor vertex count: " << vcsf << std::endl;
	// scene::IMeshSceneNode* nodeStageFloor = smgr->getMeshManipulator()->createMeshWithTangents(smgr->getMesh("cena.obj")->getMesh(0));
	// scene::IMeshSceneNode* nodeStageFloor = smgr->addMeshSceneNode(smgr->getMesh("cena.obj")->getMesh(0));
	nodeStageFloor->setPosition(vector3df(0, 100, 0));
	nodeStageFloor->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeStageFloor->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStageFloor->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeStageFloor->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStageFloor->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeStageFloor->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeStageFloor->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER --> The reflection map should be set as first texture!!
    // nodeStageFloor->setMaterialType(video::EMT_LIGHTMAP_LIGHTING_M4); /// EMT_SPHERE_MAP: Look like a reflection of the environment around it

    for(u32 i=0; i<nodeStageFloor->getMaterialCount(); i++){ // nodeStageFloor->getMaterialCount(); // 40
	nodeStageFloor->getMaterial(i).setTexture(0, driver->getTexture("stage_floor_default.jpg"));
	nodeStageFloor->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	// nodeStageFloor->getMaterial(i).setTexture(1, driver->getTexture("stage_floor_reflexes.jpg"));
	// nodeStageFloor->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	/// nodeStageFloor->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStageFloor->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStageFloor->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	/*
	nodeStageFloor->getMaterial(i).BlendOperation = irr::video::EBO_ADD;
	nodeStageFloor->getMaterial(i).Lighting = true;
	nodeStageFloor->getMaterial(i).ZWriteEnable = false;
	nodeStageFloor->getMaterial(i).ZBuffer = false;
	*/
    /// Start Colors:
	nodeStageFloor->getMaterial(i).Lighting = true;
	nodeStageFloor->getMaterial(i).Shininess = 128.0f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStageFloor->getMaterial(i).SpecularColor = video::SColor(250,250,250,250); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStageFloor->getMaterial(i).AmbientColor  = video::SColor(250,250,250,250); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStageFloor->getMaterial(i).DiffuseColor  = video::SColor(250,250,250,250); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStageFloor->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	// nodeStageFloor->getMaterial(i).getTextureMatrix(1).buildTextureTransform(1, core::vector2df(-0.5f, -0.5f*5), core::vector2df(0.5f,0.5f), core::vector2df(5, 5));
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStageFloor->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStageFloor->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeStageFloor->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};

    // Create texture animator
    if(nodeStageFloor){
    // Load textures for animation:
    core::array<video::ITexture*> texturesFloor;
    for(s32 n=1; n<23; ++n){ // Total images (minus) - 1 !
    core::stringc tmp = "";
    tmp = "stage_floor_"; tmp += n; tmp += ".jpg"; // Path to images directory
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    if(tx!=NULL){ texturesFloor.push_back(tx); }; // image found?... Add it!
    };
    scene::ISceneNodeAnimator* animatorFloor = smgr->createTextureAnimator(texturesFloor, 550, true); // Time between images (textures)
    nodeStageFloor->addAnimator(animatorFloor);
    animatorFloor->drop();
    };



    /// Create (Loads Object) STAGE PLAKARD: (texts "irrlicht music player")
	scene::IAnimatedMeshSceneNode* nodeStagePlackard = smgr->addAnimatedMeshSceneNode(smgr->getMesh("stage_plackard.obj")); // For an animated node
	// scene::ISceneNode* nodeStagePlackard = smgr->addMeshSceneNode(smgr->getMesh("stage.obj")->getMesh(0));

	scene::IMeshBuffer* buffer101 = smgr->getMesh("stage_plackard.obj")->getMesh(0)->getMeshBuffer(0);
    int vc101 = buffer101->getVertexCount();
    std::cout << "Stage Plackard vertex count: " << vc101 << std::endl;
	// scene::IMeshSceneNode* nodeStagePlackard = smgr->getMeshManipulator()->createMeshWithTangents(smgr->getMesh("cena.obj")->getMesh(0));
	// scene::IMeshSceneNode* nodeStagePlackard = smgr->addMeshSceneNode(smgr->getMesh("cena.obj")->getMesh(0));
	nodeStagePlackard->setPosition(vector3df(0, 100, 0));
	nodeStagePlackard->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeStagePlackard->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStagePlackard->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeStagePlackard->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStagePlackard->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    for(u32 i=0; i<nodeStagePlackard->getMaterialCount(); i++){ // nodeStagePlackard->getMaterialCount(); // 40
	nodeStagePlackard->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStagePlackard->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeStagePlackard->getMaterial(i).Lighting = true;
	nodeStagePlackard->getMaterial(i).Shininess = 6.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeStagePlackard->getMaterial(i).SpecularColor = video::SColor(150,50,50,50); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStagePlackard->getMaterial(i).AmbientColor  = video::SColor(150,150,150,150); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStagePlackard->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStagePlackard->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	// nodeStagePlackard->getMaterial(i).getTextureMatrix(1).buildTextureTransform(1, core::vector2df(-0.5f, -0.5f*5), core::vector2df(0.5f,0.5f), core::vector2df(5, 5));
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStagePlackard->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStagePlackard->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
    nodeStagePlackard->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust    nodeStagePlackard->getMaterial(i).Shininess = 12.0f; // Set size of specular highlights (20.0f == Metallic)
	};


	// nodeStagePlackard->setMaterialType(video::EMT_DETAIL_MAP); // Needed for multitexturing (Apply (mix) texture to my specified material)
	/// Material Type --> http://irrlicht.sourceforge.net/docu/namespaceirr_1_1video.html#ac8e9b6c66f7cebabd1a6d30cbc5430f1
	/// Material Type --> http://webcache.googleusercontent.com/search?q=cache:r8-qBiunQboJ:irrlicht.sourceforge.net/tut011.html+irrlicht+animator+rotation&cd=2&hl=pt-PT&ct=clnk&gl=pt&source=www.google.pt
    nodeStagePlackard->setMaterialType(video::EMT_REFLECTION_2_LAYER); // EMT_REFLECTION_2_LAYER
    nodeStagePlackard->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)

    /*
    // Create render target
    video::ITexture* rttr = 0;
    rttr = driver->addRenderTargetTexture(core::dimension2d<u32>(512,512), "RTTR");
    for(u32 i=0; i<nodeStageFloor->getMaterialCount(); i++){
    nodeStageFloor->setMaterialTexture(i, rttr); // set material of cube to render target
    };
    */


    /// ##########################################################################################

	// smgr->getMeshManipulator()->makePlanarTextureMapping(smgr->getMesh("cena.obj")->getMesh(0), 0.001f);

    /// Create disco balls spot light effects: (Needed for multitexturing)
    // ITexture* texture1 = driver->getTexture("reflexes.jpg");
    // rect<s32> sourceRect = rect<s32>(0,0,256,201);


    /// Create (Loads Object) PLACKARD:
	scene::ISceneNode* nodeFlyingPlackard = smgr->addMeshSceneNode(smgr->getMesh("plackard.obj")->getMesh(0));
	scene::IMeshBuffer* buffer12 = smgr->getMesh("plackard.obj")->getMesh(0)->getMeshBuffer(0);
    int vc12 = buffer12->getVertexCount();
    std::cout << "Flying Plackard vertex count: " << vc12 << std::endl;
	nodeFlyingPlackard->setPosition(vector3df(0, 115, 0));
	nodeFlyingPlackard->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeFlyingPlackard->setMaterialFlag(video::EMF_LIGHTING, false);          // Node is affected by LIGHT?
    nodeFlyingPlackard->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeFlyingPlackard->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeFlyingPlackard->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /*
    for(u32 i=0; i<nodeFlyingPlackard->getMaterialCount(); i++){ // nodeFlyingPlackard->getMaterialCount(); // 40
	nodeFlyingPlackard->getMaterial(i).setTexture(1, driver->getTexture("plackard.jpg"));
	/// nodeFlyingPlackard->getMaterial(i).getTextureMatrix(0).setTextureScale(1,1);
	nodeFlyingPlackard->getMaterial(i).Lighting = true;
	// nodeFlyingPlackard->getMaterial(i).getTextureMatrix(1).buildTextureTransform(1, core::vector2df(-0.5f, -0.5f*5), core::vector2df(0.5f,0.5f), core::vector2df(5, 5));
	};
	*/

    nodeFlyingPlackard->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeFlyingPlackard->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)

    /// Start Colors:
	/// Set size of specular highlights
	nodeFlyingPlackard->getMaterial(0).Lighting = true;
	nodeFlyingPlackard->getMaterial(0).Shininess = 8.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeFlyingPlackard->getMaterial(0).SpecularColor = video::SColor(150,50,50,50); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeFlyingPlackard->getMaterial(0).AmbientColor  = video::SColor(150,150,150,150); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFlyingPlackard->getMaterial(0).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFlyingPlackard->getMaterial(0).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

    std::cout << "Mesh file \"plackard.obj\"... Ok" << std::endl;

	/// Since driver + nodeFlyingPlackard are already created, now we can now initialize "Text to texture" stuff:
    rtt = driver->addRenderTargetTexture(core::dimension2d<u32>(512, 512), "rtt", ECF_R8G8B8);
    font_texture = guienv->getFont("font2.bmp"); // Get my text font

    /// And write some text to texture too:
    doTextToTexture(nodeFlyingPlackard, L"Irrlicht Music Player");





	/// Staff: STAFF #1
	scene::IAnimatedMeshSceneNode* nodeStaff1;
	nodeStaff1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("staff.MD3"));
	scene::IMeshBuffer* bufferS = smgr->getMesh("staff.MD3")->getMesh(0)->getMeshBuffer(0);
    int vcS = bufferS->getVertexCount();
    std::cout << "Staff n.1 vertex count: " << vcS << std::endl;
	nodeStaff1->setPosition(vector3df(180, 57, -379));
	nodeStaff1->setRotation(vector3df(0, 180-40, 0));
	nodeStaff1->setScale(core::vector3df(0.65,0.65,0.65));
	nodeStaff1->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStaff1->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeStaff1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStaff1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeStaff1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeStaff1->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	for(u32 i=0; i<nodeStaff1->getMaterialCount(); i++){
	nodeStaff1->getMaterial(i).setTexture(0, driver->getTexture("staff.jpg")); // Apply texture to my specified material
	nodeStaff1->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeStaff1->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStaff1->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

	/// Start Colors:
	/// Set size of specular highlights
	nodeStaff1->getMaterial(i).Lighting = true;
	nodeStaff1->getMaterial(i).Shininess = 0.5f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStaff1->getMaterial(i).SpecularColor = video::SColor(1,1,1,1); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStaff1->getMaterial(i).AmbientColor  = video::SColor(255,40,40,40); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStaff1->getMaterial(i).DiffuseColor  = video::SColor(255,15,15,15); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStaff1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStaff1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStaff1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeStaff1->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};


	/// Set animation frames limits:
    f32 framestart9 = nodeStaff1->getStartFrame();
    f32 frameend9 = nodeStaff1->getEndFrame()-1;
    nodeStaff1->setFrameLoop(framestart9,frameend9);
    nodeStaff1->setCurrentFrame(framestart9);
    nodeStaff1->setAnimationSpeed(0);
    nodeStaff1->setLoopMode(false);

    std::cout << "Mesh file \"staff.MD3\"... Ok" << std::endl;

    /// Staff: STAFF #2
	scene::IAnimatedMeshSceneNode* nodeStaff2;
	nodeStaff2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("staff.MD3"));
	scene::IMeshBuffer* bufferS2 = smgr->getMesh("staff.MD3")->getMesh(0)->getMeshBuffer(0);
    int vcS2 = bufferS2->getVertexCount();
    std::cout << "Staff n.2 vertex count: " << vcS2 << std::endl;
	nodeStaff2->setPosition(vector3df(-180, 57, -379));
	nodeStaff2->setRotation(vector3df(0, 180+40, 0));
	nodeStaff2->setScale(core::vector3df(0.65,0.65,0.65));
	nodeStaff2->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeStaff2->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeStaff2->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeStaff2->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeStaff2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeStaff2->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	for(u32 i=0; i<nodeStaff2->getMaterialCount(); i++){
	nodeStaff2->getMaterial(i).setTexture(0, driver->getTexture("staff.jpg")); // Apply texture to my specified material
	nodeStaff2->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeStaff2->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeStaff2->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

	/// Start Colors:
	/// Set size of specular highlights
	nodeStaff2->getMaterial(i).Lighting = true;
	nodeStaff2->getMaterial(i).Shininess = 0.5f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeStaff2->getMaterial(i).SpecularColor = video::SColor(1,1,1,1); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeStaff2->getMaterial(i).AmbientColor  = video::SColor(255,40,40,40); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStaff2->getMaterial(i).DiffuseColor  = video::SColor(255,15,15,15); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeStaff2->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeStaff2->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeStaff2->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeStaff2->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};


	/// Set animation frames limits:
    f32 framestart10 = nodeStaff2->getStartFrame();
    f32 frameend10 = nodeStaff2->getEndFrame()-1;
    nodeStaff2->setFrameLoop(framestart10,frameend10);
    nodeStaff2->setCurrentFrame(framestart10);
    nodeStaff2->setAnimationSpeed(0);
    nodeStaff2->setLoopMode(false);

    std::cout << "Mesh file \"staff.md3\"... Ok" << std::endl;


    /// Loading Bar: #3
    LoadBar->setPercent(20); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);


    /// ##########################################################################################


    /// Create (Load Animated Object) SPECTATORS:
    /*
    // Fill array with random numbers:
    vector<int> randUniquesChairsRow1;
    vector<int> randUniquesChairsRow2;
    vector<int> randUniquesChairsRow3;
    vector<int> randUniquesChairsRow4;

    for(int i=0; i<10; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesChairsRow1.push_back(x1);
    randUniquesChairsRow1.push_back(x2);
    };
    for(int i=0; i<10; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesChairsRow2.push_back(x1);
    randUniquesChairsRow2.push_back(x2);
    };
    for(int i=0; i<10; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesChairsRow3.push_back(x1);
    randUniquesChairsRow3.push_back(x2);
    };
    for(int i=0; i<10; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesChairsRow4.push_back(x1);
    randUniquesChairsRow4.push_back(x2);
    };
    // Remove duplicated random numbers in array:
    // sort(randUniques.begin(), randUniques.end());
    // randUniques.erase(makeUnique(randUniques.begin(), randUniques.end()), randUniques.end());
doingCamTransOnce[5]
    int totRandoms = 0;
    cout << endl;
    for(vector<int>::iterator iter = randUniquesChairsRow1.begin(); iter != randUniquesChairsRow1.end(); i++ter){
    totRandoms+= 1; cout << totRandoms <<") Spectator random is: " << *iter << endl;
    };
    cout << endl;
    */


    int chairInX[48]={ // Range: -20 to -340
    -290, -240, -220, -180, -120, -100, -60, -20, // first row right
    -290, -240, -220, -180, -120, -100, -60, -30, // second row right
    -290, -240, -220, -180, -120, -100, -60, -35, // third row right
     290,  240,  220,  180,  120,  100,  60,  20,
     290,  240,  220,  180,  120,  100,  60,  30,
     290,  240,  220,  180,  120,  100,  60,  35
    };

    /// Create (CLONE Objects) SPECTATORS:
    // scene::ISceneNode* nodeSpectators[15]={};
    // scene::IAnimatedMeshSceneNode* nodeSpectators[15]={};

    scene::IAnimatedMeshSceneNode* nodeSpectators[48]={};
    for(int i=0; i<48; i++){
    // Clone node:
    // nodeSpectators[i] = node9->clone(); // Clone only works with ISceneNode
    /// This mesh has 3 animations: 1 to 30 -> stand up + 30 to 60 -> hands up + 60 to 100 -> clapping
    nodeSpectators[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("spectators.MD3"));
	// nodeSpectators[i]->setPosition(vector3df(50, 70, -320));
	// nodeSpectators[i]->setRotation(vector3df(0, 0, 0));
	nodeSpectators[i]->setScale(core::vector3df(0.5,0.5,0.5));
	nodeSpectators[i]->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeSpectators[i]->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeSpectators[i]->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeSpectators[i]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeSpectators[i]->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeSpectators[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	/// http://irrlicht.sourceforge.net/docu/classirr_1_1scene_1_1_i_animated_mesh_scene_node.html#aff1c1e2270f4d3d94e58e7c130c575a4
	// nodeSpectators[i]->setCurrentFrame(1);
	// nodeSpectators[i]->setFrameLoop(30,60);     // Settings also set in main loop
    // nodeSpectators[i]->setAnimationSpeed(0);    // Settings also set in main loop (0, starts statuis is: stop)
    // nodeSpectators[i]->setLoopMode(false);      // Settings also set in main loop
    // nodeSpectators[i]->setAnimationEndCallback(0);

    /// Set in right chairs rows (X):
    if(i>=0  && i<8 ){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -320)); };
    if(i>=8  && i<16){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -270)); };
    if(i>=16 && i<24){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -220)); };

    if(i>=24 && i<32){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -320)); };
    if(i>=32 && i<40){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -270)); };
    if(i>=40 && i<48){ nodeSpectators[i]->setPosition(vector3df(chairInX[i], 70, -220)); };

    /*
    if(i>=10 && i<20){ nodeSpectators[i]->setPosition(vector3df(randUniquesChairsRow2[i], 70, -265)); };
    if(i>=20 && i<30){ nodeSpectators[i]->setPosition(vector3df(randUniquesChairsRow3[i], 70, -220)); };
    if(i>=30 && i<40){ nodeSpectators[i]->setPosition(vector3df(randUniquesChairsRow4[i], 70, -175)); };
    */

    // Rotate nodes (spectator) to face target Node (band):
    vector3df nodePos = nodeSpectators[i]->getPosition();
    vector3df targetPos = vector3df(0,0,0); // nodeBand->getPosition();
    vector3df diff = -(targetPos - nodePos); // Normal calculation is: vector3df diff = targetPos - nodePos;
    nodeSpectators[i]->setRotation(diff.getHorizontalAngle());
    nodeSpectators[i]->updateAbsolutePosition();
    };


    /// Aplly (random) textures:
    for(int x=0; x<48; x++){
    int k = rand()%4; // random from (0 to 3)
    for(u32 i=0; i<nodeSpectators[x]->getMaterialCount(); i++){
    if(k == 0){ nodeSpectators[x]->getMaterial(i).setTexture(0, driver->getTexture("spectators_1.jpg")); }; // Apply texture to my specified material
    if(k == 1){ nodeSpectators[x]->getMaterial(i).setTexture(0, driver->getTexture("spectators_2.jpg")); }; // Apply texture to my specified material
    if(k == 2){ nodeSpectators[x]->getMaterial(i).setTexture(0, driver->getTexture("spectators_3.jpg")); }; // Apply texture to my specified material
    if(k == 3){ nodeSpectators[x]->getMaterial(i).setTexture(0, driver->getTexture("spectators_4.jpg")); }; // Apply texture to my specified material
	nodeSpectators[x]->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeSpectators[x]->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeSpectators[x]->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture

	/// Start Colors:
	/// Set size of specular highlights
	nodeSpectators[x]->getMaterial(i).Lighting = true;
	nodeSpectators[x]->getMaterial(i).Shininess = 0.5f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeSpectators[x]->getMaterial(i).SpecularColor = video::SColor(1,1,1,1); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeSpectators[x]->getMaterial(i).AmbientColor  = video::SColor(255,40,40,40); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeSpectators[x]->getMaterial(i).DiffuseColor  = video::SColor(255,15,15,15); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeSpectators[x]->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeSpectators[x]->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeSpectators[x]->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeSpectators[x]->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};
    };


    /// Aplly animations:
    for(int x=0; x<48; x++){
    /// Set animation frames limits:
    f32 framestart = nodeSpectators[x]->getStartFrame();
    f32 frameend = nodeSpectators[x]->getEndFrame()-1;
    nodeSpectators[x]->setFrameLoop(framestart,frameend);
    nodeSpectators[x]->setCurrentFrame(framestart);
    nodeSpectators[x]->setAnimationSpeed(0);
    nodeSpectators[x]->setLoopMode(false);
    };

    std::cout << "Mesh file \"spectators.MD3\"... Ok" << std::endl;

    /// Create Objects for SPECTATORS: (PHOTO FLASHES placed at spectator´s hand)
    scene::ISceneNode* flash[48]; // Photos Flashes Array
    scene::ISceneNode* billf[48]; // Bills Array

    for(int i=0; i<48; i++){
	flash[i] = smgr->addEmptySceneNode();              // Photo Flashes
	flash[i]->setParent(nodeSpectators[i]);            // Attach to audiance
	flash[i]->setPosition(core::vector3df(0,117,-15)); // Attach to hand
	flash[i]->setVisible(false);                       // Starts hidden
	billf[i] = smgr->addBillboardSceneNode(flash[i], core::dimension2d<f32>(60,30)); // Attach billboard to node.
	billf[i]->setMaterialFlag(video::EMF_LIGHTING, false);   // Bill Node is affected by LIGHT?
	billf[i]->setMaterialFlag(video::EMF_FOG_ENABLE, false); // Bill Node is affected by FOG?
	billf[i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	billf[i]->setMaterialTexture(0, driver->getTexture("estrela.png"));
    };

    /// Only 1 REAL (dynamic) light for each flash (and only 1 will be visible at a time)
    scene::ILightSceneNode* fLight[48]={};
    for(int i=0; i<48; i++){
    fLight[i] = smgr->addLightSceneNode(flash[i], vector3df(0,0,0), video::SColorf(1.0,1.0,1.0,1.0), 100); // SColorf: 0.0f to 1.0f full color
    fLight[i]->enableCastShadow(false);
    };

    std::cout << "Photo Flashes... Ok" << std::endl;


    /// Create (Load Animated Object) BALLONS:
    // Fill array with random numbers:
    vector<int> randUniquesBalloons;

    for(int i=0; i<48; i++){
    /// Formula is --> int i = (MIN + rand()%(MAX - MIN + 1)) * INTERVAL;
    int x1 = (1+rand()%(10-1))*20;    // From 30 to 200
    int x2 = (1+rand()%(-10-1))*-20;  // From -30 to -200
    randUniquesBalloons.push_back(x1);
    randUniquesBalloons.push_back(x2);
    };
    // Remove duplicated random numbers in array:
    randUniquesBalloons.erase(makeUnique(randUniquesBalloons.begin(), randUniquesBalloons.end()), randUniquesBalloons.end());
    // for(vector<int>::iterator iter = randUniques.begin(); iter != randUniques.end(); i++ter){ cout << "--> " << *iter << endl; }

    /// Create (CLONE Objects) BALLONS:
    scene::IAnimatedMeshSceneNode* nodeBalloons[48]={};

    for(int i=0; i<48; i++){
    // Clone node:
    // nodeBalloons[i] = node9->clone(); // Clone only works with ISceneNode
    /// This mesh has 3 animations: 1 to 30 -> stand up + 30 to 60 -> hands up + 60 to 100 -> clapping
    nodeBalloons[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("balloon.MD3"));
	// nodeBalloons[i]->setPosition(vector3df(50, 70, -320));
	// nodeBalloons[i]->setRotation(vector3df(0, 0, 0));
	nodeBalloons[i]->setScale(core::vector3df(0.0,0.0,0.0));                // Starts too small - ANIM will change this to new scales!
	nodeBalloons[i]->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeBalloons[i]->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeBalloons[i]->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    nodeBalloons[i]->setParent(nodeSpectators[i]);                          // Attach it to spectator
    nodeBalloons[i]->setPosition(core::vector3df(0, 80, -3));               // Position relative to spectator
    nodeBalloons[i]->setVisible(false);                                     // Starts hidden - ANIM will change this!
    nodeBalloons[i]->setMaterialType(video::EMT_REFLECTION_2_LAYER);        // EMT_REFLECTION_2_LAYER
    nodeBalloons[i]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeBalloons[i]->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    };

    /// Aplly (random) textures:
    for(int x=0; x<48; x++){
    int k = rand()%5; // random from (0 to x)
    for(u32 i=0; i<nodeBalloons[x]->getMaterialCount(); i++){
    if(k == 0){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_1.jpg")); }; // Apply texture to my specified material
    if(k == 1){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_2.jpg")); }; // Apply texture to my specified material
    if(k == 2){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_3.jpg")); }; // Apply texture to my specified material
    if(k == 3){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_4.jpg")); }; // Apply texture to my specified material
    if(k == 4){ nodeBalloons[x]->getMaterial(i).setTexture(0, driver->getTexture("balloon_5.jpg")); }; // Apply texture to my specified material
    nodeBalloons[x]->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeBalloons[x]->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeBalloons[x]->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeBalloons[x]->getMaterial(i).Lighting = true;
	nodeBalloons[x]->getMaterial(i).Shininess = 3.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeBalloons[x]->getMaterial(i).SpecularColor = video::SColor(50,50,50,50); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeBalloons[x]->getMaterial(i).AmbientColor  = video::SColor(150,150,150,150); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeBalloons[x]->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeBalloons[x]->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeBalloons[x]->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeBalloons[x]->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeBalloons[x]->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};
    };

    /// Aplly animations:
    for(int x=0; x<48; x++){
    /// Set animation frames limits:
    f32 framestart = nodeBalloons[x]->getStartFrame();
    f32 frameend = nodeBalloons[x]->getEndFrame()-1;
    nodeBalloons[x]->setFrameLoop(framestart,frameend);
    nodeBalloons[x]->setCurrentFrame(framestart);
    nodeBalloons[x]->setAnimationSpeed(0);
    nodeBalloons[x]->setLoopMode(false);
    };





    /// Loading Bar: #4
    LoadBar->setPercent(30); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);


	/// Create (Load Animated Object) COURTAIN BACKGROUND:
	nodeCourtainBackground = smgr->addAnimatedMeshSceneNode(smgr->getMesh("courtain.MD3"));
	scene::IMeshBuffer* buffer8 = smgr->getMesh("courtain.MD3")->getMesh(0)->getMeshBuffer(0);
    int vc8 = buffer8->getVertexCount();
    std::cout << "Courtain n.1 vertex count: " << vc8 << std::endl;
	nodeCourtainBackground->setPosition(vector3df(0, 100, -1190));
	nodeCourtainBackground->setRotation(vector3df(0, 180, 0));
	nodeCourtainBackground->setScale(core::vector3df(1.0,1.0,1.0));
	nodeCourtainBackground->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeCourtainBackground->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeCourtainBackground->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    // nodeCourtainBackground->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeCourtainBackground->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeCourtainBackground->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	for(u32 i=0; i<nodeCourtainBackground->getMaterialCount(); i++){
	nodeCourtainBackground->getMaterial(i).setTexture(0, driver->getTexture("courtain_background.jpg")); /// JPG - Apply texture to my specified material
	nodeCourtainBackground->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeCourtainBackground->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeCourtainBackground->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeCourtainBackground->getMaterial(i).Lighting = true;
	nodeCourtainBackground->getMaterial(i).Shininess = 1.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeCourtainBackground->getMaterial(i).SpecularColor = video::SColor(5,5,5,5); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeCourtainBackground->getMaterial(i).AmbientColor  = video::SColor(100,100,100,100); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtainBackground->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtainBackground->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeCourtainBackground->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeCourtainBackground->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeCourtainBackground->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};



	/// Create (Load Animated Object) COURTAIN #2 - HALF-LEFT:
	nodeCourtain2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("courtain_left.obj"));
	scene::IMeshBuffer* buffer82 = smgr->getMesh("courtain_left.obj")->getMesh(0)->getMeshBuffer(0);
    int vc82 = buffer82->getVertexCount();
    std::cout << "Courtain n.2 vertex count: " << vc82 << std::endl;
	nodeCourtain2->setPosition(vector3df(587, 239, -450));
	nodeCourtain2->setRotation(vector3df(-90, 0, 0));
	nodeCourtain2->setScale(core::vector3df(1.0,1.0,1.0));
	nodeCourtain2->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeCourtain2->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeCourtain2->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    // nodeCourtain2->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeCourtain2->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeCourtain2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	for(u32 i=0; i<nodeCourtain2->getMaterialCount(); i++){
	nodeCourtain2->getMaterial(i).setTexture(0, driver->getTexture("courtain_left.jpg")); /// JPG (Was a PNG) - Apply texture to my specified material
	nodeCourtain2->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeCourtain2->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeCourtain2->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeCourtain2->getMaterial(i).Lighting = true;
	nodeCourtain2->getMaterial(i).Shininess = 1.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeCourtain2->getMaterial(i).SpecularColor = video::SColor(5,5,5,5); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeCourtain2->getMaterial(i).AmbientColor  = video::SColor(100,100,100,100); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtain2->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtain2->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

    nodeCourtain2->getMaterial(i).BackfaceCulling = false;
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeCourtain2->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeCourtain2->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
    nodeCourtain2->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
    /// nodeCourtain2->getMaterial(i).MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;  /// EMT_TRANSPARENT_ALPHA_CHANNEL_REF
	};


	/// Create (Load Animated Object) COURTAIN #3 - HALF-RIGHT:
	nodeCourtain3 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("courtain_right.obj"));
	scene::IMeshBuffer* buffer83 = smgr->getMesh("courtain_right.obj")->getMesh(0)->getMeshBuffer(0);
    int vc83 = buffer83->getVertexCount();
    std::cout << "Courtain n.3 vertex count: " << vc83 << std::endl;
	nodeCourtain3->setPosition(vector3df(-587, 239, -450));
	nodeCourtain3->setRotation(vector3df(-90, 0, 0));
	nodeCourtain3->setScale(core::vector3df(1.0,1.0,1.0));
	nodeCourtain3->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeCourtain3->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeCourtain3->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    // nodeCourtain3->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER
    nodeCourtain3->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeCourtain3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	for(u32 i=0; i<nodeCourtain3->getMaterialCount(); i++){
	nodeCourtain3->getMaterial(i).setTexture(0, driver->getTexture("courtain_right.jpg")); /// JPG (Was a PNG) - Apply texture to my specified material
	nodeCourtain3->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeCourtain3->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeCourtain3->getMaterial(i).getTextureMatrix(1).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture

    /// Start Colors:
	/// Set size of specular highlights
	nodeCourtain3->getMaterial(i).Lighting = true;
	nodeCourtain3->getMaterial(i).Shininess = 1.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeCourtain3->getMaterial(i).SpecularColor = video::SColor(5,5,5,5); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeCourtain3->getMaterial(i).AmbientColor  = video::SColor(100,100,100,100); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtain3->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeCourtain3->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

    nodeCourtain3->getMaterial(i).BackfaceCulling = false;
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeCourtain3->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeCourtain3->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
    nodeCourtain3->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
    /// nodeCourtain3->getMaterial(i).MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;  /// EMT_TRANSPARENT_ALPHA_CHANNEL_REF
	};

	// nodeStage->setMaterialType(video::EMT_DETAIL_MAP); // Needed for multitexturing (Apply (mix) texture to my specified material)
    // Create disco balls spot light effects:
	// node8->setMaterialTexture(0, driver->getTexture("chao.png"));  // The default, loaded texture
	// node8->setMaterialTexture(1, driver->getTexture("reflexes.jpg"));
	// node8->setMaterialType(video::EMT_DETAIL_MAP);                    // Needed for multitexturing



	/// Create (Loads Object) DISCO BALL N1:
	scene::IAnimatedMeshSceneNode* nodeDiscoBall1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("disco_ball.obj"));
	nodeDiscoBall1->setPosition(vector3df(110,280,-601));
	// nodeDiscoBall1->setRotation(vector3df(0, 180, 0));
	nodeDiscoBall1->setScale(core::vector3df(0.7,0.7,0.7));
	nodeDiscoBall1->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeDiscoBall1->setMaterialFlag(video::EMF_FOG_ENABLE, false);        // Node is affected by FOG?
    nodeDiscoBall1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Render both sides?
    nodeDiscoBall1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    nodeDiscoBall1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeDiscoBall1->setMaterialType(video::EMT_REFLECTION_2_LAYER);       // EMT_REFLECTION_2_LAYER

	for(u32 i=0; i<nodeDiscoBall1->getMaterialCount(); i++){
	nodeDiscoBall1->getMaterial(i).setTexture(0, driver->getTexture("disco_ball.jpg")); // Apply texture to my specified material
	nodeDiscoBall1->getMaterial(i).getTextureMatrix(0).setTextureScale(3.0f,1.0f);   /// Repeat (tile) the texture
	nodeDiscoBall1->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nodeDiscoBall1->getMaterial(i).getTextureMatrix(1).setTextureScale(0.015f,0.015f); /// Repeat (tile) the texture

	/// Start Colors:
	/// Set size of specular highlights
	nodeDiscoBall1->getMaterial(i).Lighting = true;
	nodeDiscoBall1->getMaterial(i).Shininess = 100.0f; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeDiscoBall1->getMaterial(i).SpecularColor = video::SColor(255,250,250,250); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeDiscoBall1->getMaterial(i).AmbientColor  = video::SColor(200,200,200,200); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeDiscoBall1->getMaterial(i).DiffuseColor  = video::SColor(200,200,200,200); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeDiscoBall1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeDiscoBall1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeDiscoBall1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	nodeDiscoBall1->getMaterial(i).MaterialTypeParam = 1.f / 64.f; /// Adjust height for parallax effect
	};

    scene::ISceneNodeAnimator* anim11 = smgr->createRotationAnimator(core::vector3df(0,1.0f,0));
    nodeDiscoBall1->addAnimator(anim11);
    anim11->drop();

    /// Create (CLONE Object) DISCO BALL N2:
    scene::ISceneNode* nodeDiscoBall2 = nodeDiscoBall1->clone();
	nodeDiscoBall2->setPosition(vector3df(-114,280,-601));
	nodeDiscoBall2->setScale(core::vector3df(0.7,0.7,0.7));
	nodeDiscoBall2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Create (CLONE Object) DISCO BALL N3:
    scene::ISceneNode* nodeDiscoBall3 = nodeDiscoBall1->clone();
	nodeDiscoBall3->setPosition(vector3df(-344,320,-518));
	nodeDiscoBall3->setScale(core::vector3df(0.9,0.9,0.9));
	nodeDiscoBall3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Create (CLONE Object) DISCO BALL N4:
    scene::ISceneNode* nodeDiscoBall4 = nodeDiscoBall1->clone();
	nodeDiscoBall4->setPosition(vector3df(354,320,-518));
	nodeDiscoBall4->setScale(core::vector3df(0.9,0.9,0.9));
	nodeDiscoBall4->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);




	/// Create (Loads Object) FOCUS:
	scene::IAnimatedMeshSceneNode* nodeFocus1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("focus_top.obj"));
	nodeFocus1->setPosition(vector3df(0,300,-495));
	// nodeFocus1->setRotation(vector3df(0, 180, 0));
	nodeFocus1->setScale(core::vector3df(1.0,1.0,1.0));
	nodeFocus1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeFocus1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeFocus1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    nodeFocus1->setMaterialType(video::EMT_REFLECTION_2_LAYER);        // EMT_REFLECTION_2_LAYER
    nodeFocus1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeFocus1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    // nodeFocus1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// Transparent material (for the focus PNG lights)

	for(u32 i=0; i<nodeFocus1->getMaterialCount(); i++){
	/// nodeFocus1->getMaterial(i).setTexture(0, driver->getTexture("focus.jpg")); // Apply texture to my specified material

	/// Start Colors:
	/// Set size of specular highlights
	nodeFocus1->getMaterial(i).Lighting = true;
	nodeFocus1->getMaterial(i).Shininess = 100.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeFocus1->getMaterial(i).SpecularColor = video::SColor(255,200,200,200); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeFocus1->getMaterial(i).AmbientColor  = video::SColor(255,255,255,255); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFocus1->getMaterial(i).DiffuseColor  = video::SColor(255,255,255,255); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFocus1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeFocus1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeFocus1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};

    scene::ISceneNodeAnimator* anim20 = smgr->createRotationAnimator(core::vector3df(0,1.5f,0));
    nodeFocus1->addAnimator(anim20);
    anim20->drop();

    /// Create (CLONE Object) FOCUS:
    scene::ISceneNode* nodeFocus2 = nodeFocus1->clone();
	nodeFocus2->setPosition(vector3df(-200,300,-495));
	nodeFocus2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Create (CLONE Object) FOCUS:
    scene::ISceneNode* nodeFocus3 = nodeFocus1->clone();
	nodeFocus3->setPosition(vector3df(200,300,-495));
	nodeFocus3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);




	/// Create (Loads Object) FOCUS SMOKE LIGHT:
	scene::IAnimatedMeshSceneNode* nodeFocusSmokeLight1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("focus_top_light.obj"));
	nodeFocusSmokeLight1->setPosition(vector3df(0,300,-495));
	// nodeFocus1->setRotation(vector3df(0, 180, 0));
	nodeFocusSmokeLight1->setScale(core::vector3df(1.0,1.0,1.0));
	nodeFocusSmokeLight1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeFocusSmokeLight1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeFocusSmokeLight1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    // nodeFocusSmokeLight1->setMaterialType(video::EMT_REFLECTION_2_LAYER);        // EMT_REFLECTION_2_LAYER
    nodeFocusSmokeLight1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeFocusSmokeLight1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeFocusSmokeLight1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// Transparent material (for the focus PNG lights)

	for(u32 i=0; i<nodeFocusSmokeLight1->getMaterialCount(); i++){
	/// nodeFocusSmokeLight1->getMaterial(i).setTexture(0, driver->getTexture("focus.jpg")); // Apply texture to my specified material

	/// Start Colors:
	/// Set size of specular highlights
	nodeFocusSmokeLight1->getMaterial(i).Lighting = true;
	nodeFocusSmokeLight1->getMaterial(i).Shininess = 0.5; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeFocusSmokeLight1->getMaterial(i).SpecularColor = video::SColor(200,200,200,200); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeFocusSmokeLight1->getMaterial(i).AmbientColor  = video::SColor(200,200,200,200); /// How much ambient light (a global light) is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFocusSmokeLight1->getMaterial(i).DiffuseColor  = video::SColor(200,200,200,200); /// How much diffuse light coming from a light source is reflected by this material (a (0 = transparent | 255 = opaque), r, g, b)
    nodeFocusSmokeLight1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// Light emitted by this material. Default is to emit no light (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeFocusSmokeLight1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeFocusSmokeLight1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};

    scene::ISceneNodeAnimator* anim22 = smgr->createRotationAnimator(core::vector3df(0,1.5f,0));
    nodeFocusSmokeLight1->addAnimator(anim22);
    anim22->drop();

    /// Create (CLONE Object) FOCUS:
    scene::ISceneNode* nodeFocusSmokeLight2 = nodeFocusSmokeLight1->clone();
	nodeFocusSmokeLight2->setPosition(vector3df(-200,300,-495));
	nodeFocusSmokeLight2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    /// Create (CLONE Object) FOCUS:
    scene::ISceneNode* nodeFocusSmokeLight3 = nodeFocusSmokeLight1->clone();
	nodeFocusSmokeLight3->setPosition(vector3df(200,300,-495));
	nodeFocusSmokeLight3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);






	/// Create (Loads Object) FOCUS FLOOR:
	scene::IAnimatedMeshSceneNode* nodeFocusFloor1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("focus_floor.obj"));
	nodeFocusFloor1->setPosition(vector3df(0,300,-495));
	// nodeFocusFloor1->setRotation(vector3df(0, 180, 0));
	nodeFocusFloor1->setScale(core::vector3df(1.0,1.0,1.0));
	nodeFocusFloor1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeFocusFloor1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeFocusFloor1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    /// nodeFocusFloor1->setMaterialType(video::EMT_REFLECTION_2_LAYER);    // EMT_REFLECTION_2_LAYER
    nodeFocusFloor1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeFocusFloor1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeFocusFloor1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// Transparent material (for the focus PNG lights)

	for(u32 i=0; i<nodeFocusFloor1->getMaterialCount(); i++){
	/// nodeFocusFloor1->getMaterial(i).setTexture(0, driver->getTexture("focus.jpg")); // Apply texture to my specified material
	nodeFocusFloor1->getMaterial(i).Lighting = true;
    // nodeFocusFloor1->getMaterial(i).Shininess = 50.0f; // Set size of specular highlights (20.0f == Metallic)
	// nodeFocusFloor1->getMaterial(i).SpecularColor.set(20,255,255,255);
	// nodeFocusFloor1->getMaterial(i).DiffuseColor.set(255,255,255,255);
	// nodeFocusFloor1->getMaterial(i).EmissiveColor.set(255,255,255,255);
	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeFocusFloor1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeFocusFloor1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};

    scene::ISceneNodeAnimator* animff = smgr->createRotationAnimator(core::vector3df(0,2.0f,0));
    nodeFocusFloor1->addAnimator(animff);
    animff->drop();

    /// Create (CLONE Object) FOCUS FLOOR:
    scene::ISceneNode* nodeFocusFloor2 = nodeFocusFloor1->clone();
	nodeFocusFloor2->setPosition(vector3df(-200,300,-495));

    /// Create (CLONE Object) FOCUS FLOOR:
    scene::ISceneNode* nodeFocusFloor3 = nodeFocusFloor1->clone();
	nodeFocusFloor3->setPosition(vector3df(200,300,-495));


    /// Loading Bar: #5
    LoadBar->setPercent(40); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);



	/// Create (Loads Object) TV BACK:
	// scene::ISceneNode* nodeTVback1 = smgr->addSceneNode(smgr->getMesh("tv.obj"));
	// scene::IAnimatedMesh* nodeTVback1 = smgr->getMesh("tv.obj");
	scene::IMeshSceneNode* nodeTVback1 = smgr->addMeshSceneNode(smgr->getMesh("tv_back.obj")->getMesh(0));
	scene::IMeshBuffer* bufferTV1 = smgr->getMesh("tv_back.obj")->getMesh(0)->getMeshBuffer(0);
    int vc2 = bufferTV1->getVertexCount();
    std::cout << "TV-Back1 vertex count: " << vc2 << std::endl;
    nodeTVback1->setName("TV-Back");
	nodeTVback1->setPosition(vector3df(0, 100, 0.0));
	nodeTVback1->setRotation(vector3df(0, 0, 0));
	nodeTVback1->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVback1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeTVback1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeTVback1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    nodeTVback1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeTVback1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    // nodeTVback1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// EMT_TRANSPARENT_ALPHA_CHANNEL_REF

    /// Static texture:
    /// video::ITexture* textureTVbackStopped = driver->getTexture("datas/imp-tv/0.jpg");

    /// Animated textures for TV 1:
    if(nodeTVback1){
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Load TV1 images:" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    // Load textures for animation:
    core::array<video::ITexture*> texturesTVbackPlaying;
    for(s32 n=1; n<50; ++n){ // Total images - 1!
    core::stringc tmp = "";
    tmp = "television_N1_"; tmp += n; tmp += ".jpg"; // Path to images: included in the "world.zip" file
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    if(tx!=NULL){ texturesTVbackPlaying.push_back(tx); }; // image found?... Add it!
    };

    scene::ISceneNodeAnimator* animatorTVback = smgr->createTextureAnimator(texturesTVbackPlaying, 1500, true); // Time between TV images (textures)
    nodeTVback1->addAnimator(animatorTVback);
    animatorTVback->drop();

 	for(u32 i=0; i<nodeTVback1->getMaterialCount(); i++){

	/// Start Colors:
	/// Set size of specular highlights
	nodeTVback1->getMaterial(i).Lighting = true;
	nodeTVback1->getMaterial(i).Shininess = 125; /// Reflex type: 20.0f == Metallic (Range: 0.5 to 128)
    /// Affect the colour of the reflected light:
    nodeTVback1->getMaterial(i).SpecularColor = video::SColor(200,150,150,150); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeTVback1->getMaterial(i).AmbientColor  = video::SColor(200,150,150,150); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeTVback1->getMaterial(i).DiffuseColor  = video::SColor(200,150,150,150); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeTVback1->getMaterial(i).EmissiveColor = video::SColor(200,255,255,255); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeTVback1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeTVback1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};
	};





    // nodeTVback->getMaterial(0).setTexture(0, textureTVbackStopped); // Apply texture to my specified material

    /// Create (CLONE Object) TV BACK:
    scene::ISceneNode* nodeTVback2 = nodeTVback1->clone();
	nodeTVback2->setPosition(vector3df(-258, 120, -390.0));
	nodeTVback2->setRotation(vector3df(0, -67, 0));
	nodeTVback2->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVback2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	/// Animated textures for TV 2:
    if(nodeTVback2){
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Load TV2 images:" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    // Load textures for animation:
    core::array<video::ITexture*> texturesTVbackPlaying;
    for(s32 n=1; n<10; ++n){ // Total images - 1!
    core::stringc tmp = "";
    tmp = "television_N2_"; tmp += n; tmp += ".jpg"; // Path to images: included in the "world.zip" file
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    if(tx!=NULL){ texturesTVbackPlaying.push_back(tx); }; // image found?... Add it!
    };
    scene::ISceneNodeAnimator* animatorTVback = smgr->createTextureAnimator(texturesTVbackPlaying, 5000, true); // Time between TV images (textures)
    nodeTVback2->addAnimator(animatorTVback);
    animatorTVback->drop();
    };

    /// Create (CLONE Object) TV BACK:
    scene::ISceneNode* nodeTVback3 = nodeTVback1->clone();
	nodeTVback3->setPosition(vector3df(269, 120, -390.0));
	nodeTVback3->setRotation(vector3df(0, 67, 0));
	nodeTVback3->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVback3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	/// Animated textures for TV 3:
    if(nodeTVback3){
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Load TV3 images:" << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;
    // Load textures for animation:
    core::array<video::ITexture*> texturesTVbackPlaying;
    for(s32 n=1; n<10; ++n){ // Total images - 1!
    core::stringc tmp = "";
    tmp = "television_N3_"; tmp += n; tmp += ".jpg"; // Path to images: included in the "world.zip" file
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    if(tx!=NULL){ texturesTVbackPlaying.push_back(tx); }; // image found?... Add it!
    };
    scene::ISceneNodeAnimator* animatorTVback = smgr->createTextureAnimator(texturesTVbackPlaying, 5000, true); // Time between TV images (textures)
    nodeTVback3->addAnimator(animatorTVback);
    animatorTVback->drop();
    };





	/// Create (Loads Object) TV FRONT: (The TV Hardware: buttons, leds, logo, etc)
	// scene::ISceneNode* nodeTVfront1 = smgr->addSceneNode(smgr->getMesh("tv.obj"));
	// scene::IAnimatedMesh* nodeTVfront1 = smgr->getMesh("tv.obj");
	scene::IMeshSceneNode* nodeTVfront1 = smgr->addMeshSceneNode(smgr->getMesh("tv_front.obj")->getMesh(0));
	scene::IMeshBuffer* buffer2f = smgr->getMesh("tv_front.obj")->getMesh(0)->getMeshBuffer(0);
    int vc2f = buffer2f->getVertexCount();
    std::cout << "TV-Front vertex count: " << vc2f << std::endl;
    nodeTVfront1->setName("TV-Front");
	nodeTVfront1->setPosition(vector3df(0, 100, 0.2));
	nodeTVfront1->setRotation(vector3df(0, 0, 0));
	nodeTVfront1->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVfront1->setMaterialFlag(video::EMF_LIGHTING, true);            // Node is affected by LIGHT?
    nodeTVfront1->setMaterialFlag(video::EMF_FOG_ENABLE, false);         // Node is affected by FOG?
    nodeTVfront1->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);  // Render both sides?
    nodeTVfront1->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true);  // Increase view distance quality (similar to sharpness)
    nodeTVfront1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeTVfront1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); /// EMT_TRANSPARENT_ALPHA_CHANNEL_REF

    // Create texture animator
    if(nodeTVfront1){
    // Load textures for animation:
    core::array<video::ITexture*> texturesTVfront;
    for(s32 n=1; n<2; ++n){ // Total images (minus) - 1 !
    core::stringc tmp = "";
    tmp = "tv_"; tmp += n; tmp += ".png"; // Path to images: "tv" directory, included in the "world.zip" file
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    texturesTVfront.push_back(tx);
    };
    scene::ISceneNodeAnimator* animatorTVfront = smgr->createTextureAnimator(texturesTVfront, 300, true); // Time between TV images (textures)
    nodeTVfront1->addAnimator(animatorTVfront);
    animatorTVfront->drop();
    };

	for(u32 i=0; i<nodeTVfront1->getMaterialCount(); i++){
	nodeTVfront1->getMaterial(i).setTexture(0, driver->getTexture("tv_1.png")); // Apply texture to my specified material

	/// Start Colors:
	/// Set size of specular highlights
	nodeTVfront1->getMaterial(i).Lighting = true;
	nodeTVfront1->getMaterial(i).Shininess = 5.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    nodeTVfront1->getMaterial(i).SpecularColor = video::SColor(150,10,10,10); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    nodeTVfront1->getMaterial(i).AmbientColor  = video::SColor(50,10,10,10); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    nodeTVfront1->getMaterial(i).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    nodeTVfront1->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors

	for(u32 t=0; t<_IRR_MATERIAL_MAX_TEXTURES_; t++){
    nodeTVfront1->getMaterial(i).TextureLayer[t].AnisotropicFilter = 8;
    nodeTVfront1->getMaterial(i).TextureLayer[t].LODBias = 128;
    };
	};


	/// Create (CLONE Object) TV FRONT:
    scene::ISceneNode* nodeTVfront2 = nodeTVfront1->clone();
	nodeTVfront2->setPosition(vector3df(-258.2, 120, -390.0));
	nodeTVfront2->setRotation(vector3df(0, -67, 0));
	nodeTVfront2->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVfront2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	/// Create (CLONE Object) TV FRONT:
    scene::ISceneNode* nodeTVfront3 = nodeTVfront1->clone();
	nodeTVfront3->setPosition(vector3df(269.2, 120, -390.0));
	nodeTVfront3->setRotation(vector3df(0, 67, 0));
	nodeTVfront3->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeTVfront3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);


    std::cout << "Get other meshes... Ok" << std::endl;



    /// Loading Bar: #6
    LoadBar->setPercent(50); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);




    /// ############# CREATE LIGHTS ################################################################


    /// Create main LIGHT:
    /// smgr->setAmbientLight(video::SColorf(200,200,255,0)); // Ambient Light. SColorf == r,g,b,a
    smgr->setAmbientLight(video::SColorf(0.50,0.50,0.50,0.0)); // Ambient Light. SColorf == r,g,b,a

    /// Create FOG: (E_FOG_TYPE ---> EFT_FOG_EXP or EFT_FOG_LINEAR or EFT_FOG_EXP2)
    // SYNTAX: driver->setFog(SColor, E_FOG_TYPE, f32, f32, f32, bool, bool);
    driver->setFog(video::SColor(0,0.0,0.0,0.0), video::EFT_FOG_LINEAR, 250, 1000, .003f, true, false);
    // node->setMaterialFlag(video::EMF_FOG_ENABLE, true);


	/// Create DECAL manager:
	/*
    // Create decal manager
    DecalManager* decalManager = new DecalManager(smgr);
    // Set the target node that will have decals:
    decalManager->addMesh(nodeStageFloor);
    // nodeBand->setPosition(vector3df(600, 96, -520));  /// Out of stage
    // Create a decal
    irr::core::vector3df position = irr::core::vector3df(0,110,-510); // Position to place the decal
    irr::core::vector3df dimension = irr::core::vector3df(20,20,20);  // Dimension of decal
    irr::core::vector3df normal = irr::core::vector3df(0,1,0);        // Orientation of the decal
    irr::f32 textureRotation = 0;                                     // Rotation in degrees
    irr::scene::ISceneNode* parent = 0;                               // Parent
    irr::f32 lifeTime = 160;                                          // Time to life (in seconds)
    irr::f32 distance = 1000;                                         // Max viewing distance

    decalManager->addDecal("decal_01.jpg", position, dimension, normal, textureRotation, parent, lifeTime, distance);
    */


    /// Create Spot LIGHT:
    /*
    scene::ISceneNode* node_SP1 = 0;
    scene::IBillboardSceneNode* billboard_SP1 = smgr->addBillboardSceneNode(node_SP1);
    billboard_SP1->setPosition(core::vector3df(0,200,-350));
    billboard_SP1->setMaterialTexture(0, driver->getTexture("dummy.png")); /// Invisible: "dummy.png" - Visible: "estrela.png"
    billboard_SP1->setMaterialFlag(video::EMF_LIGHTING, false);
    billboard_SP1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
    billboard_SP1->setScale(core::vector3df(450,450,450));
    */
    light_SP1 = smgr->addLightSceneNode(0, core::vector3df(0,200,-350), video::SColorf(1.0,1.0,1.0,0.0), 1000); // SColorf: 0.0f to 1.0f full color. SColorf = r,g,b,a
    // light_SP1->setPosition(vector3df(600, 96, -520));  /// Out of stage
	light_SP1->setRotation(vector3df(160.0,0.0,0.0)); // X90 = Middle Stage & Y90 = left, Y-90 = right
	light_SP1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    data_SP1.Type = ELT_SPOT; // ELT_POINT | ELT_SPOT | ELT_DIRECTIONAL
    data_SP1.InnerCone = 20.0f;
    data_SP1.OuterCone = 30.0f;
    data_SP1.Falloff   =  5.0f;
    // data_SP1.Direction     = core::vector3df(0.0, 0.0, -1.0);
    data_SP1.AmbientColor  = video::SColorf(1.0, 0.0, 0.0, 0.0);
    data_SP1.DiffuseColor  = video::SColorf(1.0, 0.0, 0.0, 0.0);
    data_SP1.SpecularColor = video::SColorf(1.0, 0.0, 0.0, 0.0);
    light_SP1->setLightData(data_SP1); // Apply data to the light
    light_SP1->enableCastShadow(true);
    light_SP1->setVisible(true);



    /// Create Main Colored LIGHT, does cast shadows:
    scene::ISceneNode* node_ML1 = 0;
    scene::IBillboardSceneNode* billboard_ML1 = smgr->addBillboardSceneNode(node_ML1);
    billboard_ML1->setPosition(core::vector3df(0,190,-420));
    billboard_ML1->setMaterialTexture(0, driver->getTexture("dummy.png")); /// Invisible: "dummy.png" - Visible: "estrela.png"
    billboard_ML1->setMaterialFlag(video::EMF_LIGHTING, false);
    billboard_ML1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
    billboard_ML1->setScale(core::vector3df(450,450,450));

    // Add animator:
    scene::ISceneNodeAnimator* anim_ML1 = smgr->createFlyCircleAnimator(core::vector3df(0,190,-420), 3.0f, 0.0030f);
    billboard_ML1->addAnimator(anim_ML1);
    anim_ML1->drop();

    /// Create the light: (This is the main light responsible for all the stage colors)
    light_ML1 = smgr->addLightSceneNode(billboard_ML1, vector3df(0,0,0), video::SColorf(1.0,1.0,1.0,0.0), 3000); // SColorf: 0.0f to 1.0f full color. SColorf = r,g,b,a
    light_ML1->enableCastShadow(true);
    light_ML1->setVisible(true);
    data_ML1.Type = video::ELT_POINT; // ELT_POINT | ELT_SPOT | ELT_DIRECTIONAL
    // data_ML1.Direction = vector3df(0,-1,0);
    data_ML1.AmbientColor  = video::SColorf(0,0,0,0); // Will change in main loop
    data_ML1.SpecularColor = video::SColorf(0,0,0,0); // Will change in main loop
    data_ML1.DiffuseColor  = video::SColorf(0,0,0,0); // Will change in main loop
    light_ML1->setLightData(data_ML1); // Apply data to the light (Will change in main loop)




    /// Create White Flash Strobe LIGHT:
    scene::ISceneNode* node_FL1 = 0;
    scene::IBillboardSceneNode* billboard_FL1 = smgr->addBillboardSceneNode(node_FL1);
    billboard_FL1->setPosition(vector3df(0,180,-420));
    billboard_FL1->setMaterialTexture(0, driver->getTexture("dummy.png")); /// Invisible: "dummy.png" - Visible: "estrela.png"
    billboard_FL1->setMaterialFlag(video::EMF_LIGHTING, false);
    billboard_FL1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
    billboard_FL1->setScale(core::vector3df(450,450,450));
    // Add animator:
    scene::ISceneNodeAnimator* anim_FL1 = smgr->createFlyCircleAnimator(core::vector3df(0,180,-420), 5.0f, 0.0030f);
    billboard_FL1->addAnimator(anim_FL1);
    anim_FL1->drop();

    /// Create the light: (This is the main light responsible for all the stage colors)
    light_FL1 = smgr->addLightSceneNode(billboard_FL1, vector3df(0,0,0), video::SColorf(1.0,1.0,1.0,0.0), 3000); // SColorf: 0.0f to 1.0f full color. SColorf = r,g,b,a
    light_FL1->enableCastShadow(true);
    light_FL1->setVisible(false);
    data_FL1.Type = video::ELT_POINT; // ELT_POINT | ELT_SPOT | ELT_DIRECTIONAL
    // data_FL1.Direction = vector3df(0,45,0);
    data_FL1.AmbientColor  = video::SColorf(0.9,0.9,0.9,1.0); // SColorf = r,g,b,a
    data_FL1.SpecularColor = video::SColorf(0.9,0.9,0.9,1.0); // SColorf = r,g,b,a
    data_FL1.DiffuseColor  = video::SColorf(0.9,0.9,0.9,1.0); // SColorf = r,g,b,a
    light_FL1->setLightData(data_FL1); // Apply data to the light




    /// Create small FAKE LIGHTS: (6 columns x 8 lights = 48 lights per column)
    /// scene::ILightSceneNode* light[48];
    scene::ISceneNode* light[48]; /// Fake Lights Array
    scene::ISceneNode* bill[48];  // Bills Array

    // Lights coordinates arrays, per column:
    irr::f32 aX[48] = { // Lights X axis:
     258, 258, 258, 258, 258, 258,   // Column 1
     210, 210, 210, 210, 210, 210,   // Column 2
     158, 158, 158, 158, 158, 158,   // Column 3
     110, 110, 110, 110, 110, 110,   // Column 4
    -112,-112,-112,-112,-112,-112,   // Column 5
    -161,-161,-161,-161,-161,-161,   // Column 6
    -213,-213,-213,-213,-213,-213,   // Column 7
    -261,-261,-261,-261,-261,-261 }; // Column 8

    irr::f32 aY[48] = { // Lights Y axis:
    243, 225, 207, 189, 171, 153,    // Column 1
    243, 225, 207, 189, 171, 153,    // Column 2
    243, 225, 207, 189, 171, 153,    // Column 3
    243, 225, 207, 189, 171, 153,    // Column 4
    243, 225, 207, 189, 171, 153,    // Column 5
    243, 225, 207, 189, 171, 153,    // Column 6
    243, 225, 207, 189, 171, 153,    // Column 7
    243, 225, 207, 189, 171, 153 };  // Column 8

    irr::f32 aZ[48] = { // Lights Z axis:
    -593,-592,-590,-589,-588,-587,   // Column 1
    -593,-592,-590,-589,-588,-587,   // Column 2
    -593,-592,-590,-589,-588,-587,   // Column 3
    -593,-592,-590,-589,-588,-587,   // Column 4
    -593,-592,-590,-589,-588,-587,   // Column 5
    -593,-592,-590,-589,-588,-587,   // Column 6
    -593,-592,-590,-589,-588,-587,   // Column 7
    -593,-592,-590,-589,-588,-587 }; // Column 8


    /// Add 6 FAKE lights per column, loop:
    for(int i=0; i<48; i++){ // Loop trought lights (6 Lights X 8 columns = 48 Lights)
	light[i] = smgr->addEmptySceneNode(); /// Fake Lights
	light[i]->setPosition(core::vector3df(aX[i],aY[i],aZ[i]));
	// bill[column][i] = smgr->addBillboardSceneNode(light[column][i], core::dimension2d<f32>(60,30)); // Attach billboard to light.
	bill[i] = smgr->addBillboardSceneNode(light[i], core::dimension2d<f32>(85,300)); // Attach billboard to light.
	bill[i]->setMaterialFlag(video::EMF_LIGHTING, false);   // Bill Node is affected by LIGHT?
	bill[i]->setMaterialFlag(video::EMF_FOG_ENABLE, false); // Bill Node is affected by FOG?
	bill[i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	bill[i]->setMaterialTexture(0, driver->getTexture("estrela_vertical.png"));
    /*
	/// Start Colors:
	/// Set size of specular highlights
	bill[i]->getMaterial(0).Lighting = true;
	bill[i]->getMaterial(0).Shininess = 50.0f; /// Reflex type: 20.0f == Metallic
    /// Affect the colour of the reflected light:
    bill[i]->getMaterial(0).SpecularColor = video::SColor(255,255,255,255); /// The lights reflexes (a (0 = transparent | 255 = opaque), r, g, b)
    /// Tweak all the light related aspects of the material:
    bill[i]->getMaterial(0).AmbientColor  = video::SColor(255,255,255,255); /// Texture visibility (a (0 = transparent | 255 = opaque), r, g, b)
    bill[i]->getMaterial(0).DiffuseColor  = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    bill[i]->getMaterial(0).EmissiveColor = video::SColor(0,0,0,0); /// (a (0 = transparent | 255 = opaque), r, g, b)
    /// End Colors
    */
    };

    std::cout << "Lights... Ok" << std::endl;



    /// Loading Bar: #7
    LoadBar->setPercent(60); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);




    /// Create LASER LIGHTS:
    // Get images:
    core::array<irr::video::ITexture*> texturesLaser;
    for(s32 n=0; n<13; n++){
    core::stringc tmp; tmp = "laser_light_"; tmp += n; tmp += ".jpg";
    video::ITexture* tx = driver->getTexture(tmp.c_str());
    texturesLaser.push_back(tx);
    };
    /// Create LASER LIGHTS group for left:
    // Light nodes:
    for(int i=0; i<8; i++){
    nodeLaserA[i] = smgr->addVolumeLightSceneNode(0, -1, 4, 4, video::SColor(0, 255, 255, 255), video::SColor(0, 0, 0, 0)); // Set: | ? | ? | Subdivisions on U axis | Subdivisions on V axis | foot color | tail color
    // nodeLaserA[i]->enableCastShadow(false);
    nodeLaserA[i]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    nodeLaserA[i]->setScale(core::vector3df(0.35f,900.0f,0.1f));
    nodeLaserA[i]->setPosition(core::vector3df(150,384.1f,-583)); /// Position Y = 384.1f is perfect!
    // Create texture animator
    scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(texturesLaser, 250, true); // Time between images (textures)
    // Add the animator: G/// Create LASER LIGHTS at the left:low
    nodeLaserA[i]->addAnimator(glow);
    // Drop the animator because it was created with a create() function
    glow->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[0]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,1.0f));
    nodeLaserA[0]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[1]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,-1.0f));
    nodeLaserA[1]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[2]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,0.2f));
    nodeLaserA[2]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[3]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,-0.2f));
    nodeLaserA[3]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[4]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,0.7f));
    nodeLaserA[4]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[5]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,-0.7f));
    nodeLaserA[5]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[6]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,1.7f));
    nodeLaserA[6]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserA[7]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,-1.7f));
    nodeLaserA[7]->addAnimator(animLaser);
    animLaser->drop();
    };
    /// Hide lasers:
    for(int i=0; i<8; i++){ if(nodeLaserA[i]){ nodeLaserA[i]->setVisible(false); }; };

    /// Create LASER LIGHTS group for right:
    for(int i=0; i<8; i++){
    nodeLaserB[i] = smgr->addVolumeLightSceneNode(0, -1, 4, 4, video::SColor(0, 255, 255, 255), video::SColor(0, 0, 0, 0)); // Set: | ? | ? | Subdivisions on U axis | Subdivisions on V axis | foot color | tail color
    // nodeLaserB[i]->enableCastShadow(false);
    nodeLaserB[i]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    nodeLaserB[i]->setScale(core::vector3df(0.35f,900.0f,0.1f));
    nodeLaserB[i]->setPosition(core::vector3df(-150,384.1f,-583)); /// Position Y = 384.1f is perfect!
    // Create texture animator
    scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(texturesLaser, 250, true); // Time between images (textures)
    // Add the animator: Glow
    nodeLaserB[i]->addAnimator(glow);
    // Drop the animator because it was created with a create() function
    glow->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[0]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,1.0f));
    nodeLaserB[0]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[1]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,-1.0f));
    nodeLaserB[1]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[2]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,0.2f));
    nodeLaserB[2]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[3]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,-0.2f));
    nodeLaserB[3]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[4]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,0.7f));
    nodeLaserB[4]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[5]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(0.1f,0,-0.7f));
    nodeLaserB[5]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[6]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,1.7f));
    nodeLaserB[6]->addAnimator(animLaser);
    animLaser->drop();
    };
    // Add the animator: Rotation:
    if(nodeLaserB[7]){
    scene::ISceneNodeAnimator* animLaser = smgr->createRotationAnimator(core::vector3df(-0.1f,0,-1.7f));
    nodeLaserB[7]->addAnimator(animLaser);
    animLaser->drop();
    };
    /// Hide lasers:
    for(int i=0; i<8; i++){ if(nodeLaserB[i]){ nodeLaserB[i]->setVisible(false); }; };

    std::cout << "Lasers... Ok" << std::endl;




    /// ##########################################################################################
    /// ############# PARTICLES ##################################################################
    /// ##########################################################################################

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Creating particles system..." << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    /// Create particle system: (DISCO BALL N1 - reflexed lights)
	scene::IParticleSystemSceneNode* ps60 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em60 = ps60->createSphereEmitter( /// SPHERE Emitter (Same geometry as the mesh is)
	core::vector3df(0.0f,0.0f,0.0f),  // emitter position
    0.282f,                           // f32 radius
	core::vector3df(0.0f,0.0f,0.0f),  // initial direction & speed (all set to 0.0f == particles stopped)
	0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),      // darkest color
	video::SColor(0,0,0,0),           // darkest color
	video::SColor(255,255,255,255),   // brightest color
	90,200,0,                         // min age, max age, angle
	core::dimension2df( 5.0f, 5.0f),  // min size
	core::dimension2df(25.0f,25.0f)); // max size

	ps60->setEmitter(em60); // Grabs the emitter
	em60->drop(); // We can drop it here, without deleting it

	ps60->setParent(nodeDiscoBall1); /// Attach these reflexed lights to the Disco Ball N1 (Match to the same position & rotation)
	// ps60->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps60->setScale(core::vector3df(100,100,100));
	ps60->setScale(core::vector3df(40,40,40));
	ps60->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps60->setMaterialTexture(0, driver->getTexture("estrela_squared.png"));
	// ps60->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps60->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps60->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps60->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps60);
	// ps60->getMaterial(0).Lighting = false;
    // ps60->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps60->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps60->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps60->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em60->setMinParticlesPerSecond(0);
	em60->setMaxParticlesPerSecond(0);


    /// Create particle system: (DISCO BALL N2 - reflexed lights)
	scene::IParticleSystemSceneNode* ps61 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em61 = ps61->createSphereEmitter( /// SPHERE Emitter (Same geometry as the mesh is)
	core::vector3df(0.0f,0.0f,0.0f),  // emitter position
    0.282f,                           // f32 radius
	core::vector3df(0.0f,0.0f,0.0f),  // initial direction & speed (all set to 0.0f == particles stopped)
	0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),      // darkest color
	video::SColor(0,0,0,0),           // darkest color
	video::SColor(255,255,255,255),   // brightest color
	90,200,0,                         // min age, max age, angle
	core::dimension2df( 5.0f, 5.0f),  // min size
	core::dimension2df(25.0f,25.0f)); // max size

	ps61->setEmitter(em61); // Grabs the emitter
	em61->drop(); // We can drop it here, without deleting it

	ps61->setParent(nodeDiscoBall2); /// Attach these reflexed lights to the Disco Ball N2 (Match to the same position & rotation)
	// ps61->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps61->setScale(core::vector3df(100,100,100));
	ps61->setScale(core::vector3df(40,40,40));
	ps61->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps61->setMaterialTexture(0, driver->getTexture("estrela_squared.png"));
	// ps61->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps61->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps61->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps61->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps61);
	// ps61->getMaterial(0).Lighting = false;
    // ps61->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps61->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps61->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps61->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em61->setMinParticlesPerSecond(0);
	em61->setMaxParticlesPerSecond(0);


    /// Create particle system: (DISCO BALL N3 - reflexed lights)
	scene::IParticleSystemSceneNode* ps62 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em62 = ps62->createSphereEmitter( /// SPHERE Emitter (Same geometry as the mesh is)
	core::vector3df(0.0f,0.0f,0.0f),  // emitter position
    0.282f,                           // f32 radius
	core::vector3df(0.0f,0.0f,0.0f),  // initial direction & speed (all set to 0.0f == particles stopped)
	0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),      // darkest color
	video::SColor(0,0,0,0),           // darkest color
	video::SColor(255,255,255,255),   // brightest color
	90,200,0,                         // min age, max age, angle
	core::dimension2df( 5.0f, 5.0f),  // min size
	core::dimension2df(25.0f,25.0f)); // max size

	ps62->setEmitter(em62); // Grabs the emitter
	em62->drop(); // We can drop it here, without deleting it

	ps62->setParent(nodeDiscoBall3); /// Attach these reflexed lights to the Disco Ball N3 (Match to the same position & rotation)
	// ps62->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps62->setScale(core::vector3df(100,100,100));
	ps62->setScale(core::vector3df(40,40,40));
	ps62->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps62->setMaterialTexture(0, driver->getTexture("estrela_squared.png"));
	// ps62->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps62->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps62->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps62->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps62);
	// ps62->getMaterial(0).Lighting = false;
    // ps62->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps62->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps62->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps62->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em62->setMinParticlesPerSecond(0);
	em62->setMaxParticlesPerSecond(0);


    /// Create particle system: (DISCO BALL N4 - reflexed lights)
	scene::IParticleSystemSceneNode* ps63 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em63 = ps63->createSphereEmitter( /// SPHERE Emitter (Same geometry as the mesh is)
	core::vector3df(0.0f,0.0f,0.0f),  // emitter position
    0.282f,                           // f32 radius
	core::vector3df(0.0f,0.0f,0.0f),  // initial direction & speed (all set to 0.0f == particles stopped)
	0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),      // darkest color
	video::SColor(0,0,0,0),           // darkest color
	video::SColor(255,255,255,255),   // brightest color
	90,200,0,                         // min age, max age, angle
	core::dimension2df( 5.0f, 5.0f),  // min size
	core::dimension2df(25.0f,25.0f)); // max size

	ps63->setEmitter(em63); // Grabs the emitter
	em63->drop(); // We can drop it here, without deleting it

	ps63->setParent(nodeDiscoBall4); /// Attach these reflexed lights to the Disco Ball N4 (Match to the same position & rotation)
	// ps63->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps63->setScale(core::vector3df(100,100,100));
	ps63->setScale(core::vector3df(40,40,40));
	ps63->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps63->setMaterialTexture(0, driver->getTexture("estrela_squared.png"));
	// ps63->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps63->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps63->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps63->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps63);
	// ps63->getMaterial(0).Lighting = false;
    // ps63->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps63->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps63->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps63->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em63->setMinParticlesPerSecond(0);
	em63->setMaxParticlesPerSecond(0);


    /// Create particle system: (Falling STARS)
	scene::IParticleSystemSceneNode* ps2 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em2 = ps2->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(0,0,0,30,30,8),  // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                         // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,-0.02f,0.0f),   // initial direction. Y = speed
	0,0,                                 // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),         // darkest color
	video::SColor(0,0,0,0),              // darkest color
	video::SColor(255,255,255,255),      // brightest color
	16000,30000,40,                      // min age, max age, angle
	core::dimension2df(0.5f,0.5f),       // min size
	core::dimension2df(6.0f,6.0f));      // max size

	ps2->setEmitter(em2); // Grabs the emitter
	em2->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf2 = ps2->createRotationAffector(core::vector3df(3,3,3), core::vector3df(0,0,0)); /// Rotates our particles (speed, pivot point)
	ps2->addAffector(paf2); // Same goes for the affector
	paf2->drop();

	ps2->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	// ps2->setScale(core::vector3df(100,100,100));
	ps2->setScale(core::vector3df(40,40,40));
	ps2->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps2->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps2->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps2->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps2->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps2);
	// ps2->getMaterial(0).Lighting = false;
    // ps2->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps2->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps2->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps2->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em2->setMinParticlesPerSecond(0);
	em2->setMaxParticlesPerSecond(0);


    /// Create particle system: (RIGHT FIRE)
	scene::IParticleSystemSceneNode* ps3 = smgr->addParticleSystemSceneNode(false);
    // ps3->setParent(nodeBand[0]);

	scene::IParticleEmitter* em3 = ps3->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-7,0,-7,7,1,7),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.2f,0.0f),      // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,255,255,255),         // darkest color
	video::SColor(0,255,255,255),         // brightest color
	400,1000,0,                           // min age, max age, angle
	core::dimension2df(3.f,3.f),          // min size
	core::dimension2df(17.f,17.f));       // max size

	ps3->setEmitter(em3); // Grabs the emitter
	em3->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf3 = ps3->createFadeOutParticleAffector(); // Fade out particles
	ps3->addAffector(paf3); // Same goes for the affector
	paf3->drop();

	ps3->setPosition(core::vector3df(-297,170,-598)); // x, y, z: profundidade (+25)
	// ps3->setScale(core::vector3df(100,100,100));
	ps3->setScale(core::vector3df(1.0f,1.0f,1.0f));
	ps3->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps3->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps3->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps3->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps3->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps3->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps3);
	/// Disabled:
    em3->setMinParticlesPerSecond(0);
	em3->setMaxParticlesPerSecond(0);


    /// Create particle system: (LEFT FIRE)
	scene::IParticleSystemSceneNode* ps4 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em4 = ps4->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-7,0,-7,7,1,7),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.2f,0.0f),      // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,255,255,255),         // darkest color
	video::SColor(0,255,255,255),         // brightest color
	400,1000,0,                           // min age, max age, angle
	core::dimension2df(3.f,3.f),          // min size
	core::dimension2df(17.f,17.f));       // max size

	ps4->setEmitter(em4); // Grabs the emitter
	em4->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf4 = ps4->createFadeOutParticleAffector(); // Fade out particles
	ps4->addAffector(paf4); // Same goes for the affector
	paf4->drop();

	ps4->setPosition(core::vector3df(292,170,-598)); // x, y, z: profundidade
	// ps4->setScale(core::vector3df(100,100,100));
	ps4->setScale(core::vector3df(1.0f,1.0f,1.0f));
	ps4->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps4->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps4->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps4->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps4->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps4->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps4->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps4);
	/// Disabled:
    em4->setMinParticlesPerSecond(0);
	em4->setMaxParticlesPerSecond(0);
	// TODO stop emiters, if no song playing:
	// ps->getEmitter()->setMaxParticlesPerSecond(10);
    // ps->getEmitter()->setMinParticlesPerSecond(10);
    // ps->setEmitter(0);


	/// Create particle system: (CENTERED FIREWORKS) - Only visible when music stops
	scene::IParticleSystemSceneNode* ps5 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em5 = ps5->createRingEmitter( /// RING Emitter
    vector3df(0.0f,0.0f,0.0f),          // center
    1,10,                               // radius / ring thickness
    vector3df(0.0f,0.1f,0.0f),          // direction
    0,0,                                // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
    SColor(255,255,255,255),            // darkest color
    SColor(0,0,0,0),                    // brightest color
    8000,10000,180,                     // lifeTimeMin / lifeTimeMax / maxAngleDegrees
    core::dimension2df(1.0f, 1.0f),     // minStartSize
    core::dimension2df(5.0f, 5.0f));    // maxStartSize

	ps5->setEmitter(em5); // Grabs the emitter
	em5->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf5 = ps5->createFadeOutParticleAffector(); // Fade out particles
	ps5->addAffector(paf5); // Same goes for the affector
	paf5->drop();

	ps5->setPosition(core::vector3df(0,380,-390)); // x, y, z: profundidade
	// ps5->setScale(core::vector3df(100,100,100));
	// ps5->setScale(core::vector3df(10,10,10));
	ps5->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps5->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps5->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps5->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps5->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps5->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps5->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps5);
	/// Disabled:
    em5->setMinParticlesPerSecond(0);
	em5->setMaxParticlesPerSecond(0);

	// TODO stop emiters, if no song playing:
	// ps->getEmitter()->setMaxParticlesPerSecond(10);
    // ps->getEmitter()->setMinParticlesPerSecond(10);
    // ps5->setEmitter();

    /// Create particle system: (falling flames RIGHT)
	scene::IParticleSystemSceneNode* ps7 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em7 = ps7->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(0,0,0,0.4,0.4,0.4), // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                            // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,-0.03f,0.0f),      // initial direction. Y = speed
	0,0,                                    // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	video::SColor(0,0,0,0),                 // darkest color
	video::SColor(255,255,255,255),         // brightest color
	500,1000,1,                             // min age, max age, angle
	core::dimension2df(1.0f,1.0f),          // min size
	core::dimension2df(7.0f,7.0f));         // max size

	ps7->setEmitter(em7); // Grabs the emitter
	em7->drop(); // We can drop it here, without deleting it

	// scene::IParticleAffector* paf7 = ps7->createFadeOutParticleAffector(); // Fade out particles
	// ps7->addAffector(paf7); // Same goes for the affector
	// paf7->drop();

	ps7->setPosition(core::vector3df(-306,160,-608)); // x, y, z: profundidade
	// ps7->setScale(core::vector3df(100,100,100));
	ps7->setScale(core::vector3df(40,40,40));
	ps7->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps7->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps7->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps7->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps7->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps7->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps7);
	// ps7->getMaterial(0).Lighting = false;
    // ps7->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps7->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps7->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps7->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em7->setMinParticlesPerSecond(0);
	em7->setMaxParticlesPerSecond(0);


	/// Create particle system: (falling flames LEFT)
	scene::IParticleSystemSceneNode* ps8 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em8 = ps8->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(0,0,0,0.4,0.4,0.4), // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                            // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,-0.03f,0.0f),      // initial direction. Y = speed
	0,0,                                    // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	video::SColor(0,0,0,0),                 // darkest color
	video::SColor(255,255,255,255),         // brightest color
	500,1000,1,                             // min age, max age, angle
	core::dimension2df(1.0f,1.0f),          // min size
	core::dimension2df(7.0f,7.0f));         // max size

	ps8->setEmitter(em8); // Grabs the emitter
	em8->drop(); // We can drop it here, without deleting it

	// scene::IParticleAffector* paf8 = ps8->createFadeOutParticleAffector(); // Fade out particles
	// ps8->addAffector(paf8); // Same goes for the affector
	// paf8->drop();

	ps8->setPosition(core::vector3df(286,160,-608)); // x, y, z: profundidade
	// ps8->setScale(core::vector3df(100,100,100));
	ps8->setScale(core::vector3df(40,40,40));
	ps8->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps8->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps8->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps8->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent texture.
	ps8->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps8->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps8);
	// ps8->getMaterial(0).Lighting = false;
    // ps8->getMaterial(0).Shininess = 100.0f; // Set size of specular highlights
	// ps8->getMaterial(0).SpecularColor.set(255,255,255,255);
	// ps8->getMaterial(0).DiffuseColor.set(255,255,255,255);
	// ps8->getMaterial(0).EmissiveColor.set(255,255,255,255);
	/// Disabled:
    em8->setMinParticlesPerSecond(0);
	em8->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMALL UP FIRE N1)
	scene::IParticleSystemSceneNode* ps30 = smgr->addParticleSystemSceneNode(false);
    // ps30->setParent(nodeBand[0]);

	scene::IParticleEmitter* em30 = ps30->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.15f,0.0f),     // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,0,0,0),               // darkest color
	video::SColor(0,255,255,255),         // brightest color
	100,1000,0,                           // min age, max age, angle
	core::dimension2df(1.0f,1.0f),        // min size
	core::dimension2df(13.f,13.f));       // max size

	ps30->setEmitter(em30); // Grabs the emitter
	em30->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf30a = ps30->createFadeOutParticleAffector(); // Affector: Fade out particles
	ps30->addAffector(paf30a);
	paf30a->drop();

	ps30->setPosition(core::vector3df(147,105,-428)); // x, y, z: profundidade (+25)
	// ps30->setRotation(core::vector3df(-2.5,0,-2.5)); /// Rotation needed, its to compensate emitter angle
	// ps30->setScale(core::vector3df(100,100,100));
	ps30->setScale(core::vector3df(1,1,1));
	ps30->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps30->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps30->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps30->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps30->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps30->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps30->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps30);
	/// Disabled:
    em30->setMinParticlesPerSecond(0);
	em30->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMALL UP FIRE N2)
	scene::IParticleSystemSceneNode* ps31 = smgr->addParticleSystemSceneNode(false);
    // ps30->setParent(nodeBand[0]);

	scene::IParticleEmitter* em31 = ps31->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.15f,0.0f),     // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,0,0,0),               // darkest color
	video::SColor(0,255,255,255),         // brightest color
	100,1000,0,                           // min age, max age, angle
	core::dimension2df(1.0f,1.0f),        // min size
	core::dimension2df(13.f,13.f));       // max size

	ps31->setEmitter(em31); // Grabs the emitter
	em31->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf31a = ps31->createFadeOutParticleAffector(); // Affector: Fade out particles
	ps31->addAffector(paf31a);
	paf31a->drop();

	ps31->setPosition(core::vector3df(49,105,-428)); // x, y, z: profundidade (+25)
	// ps31->setRotation(core::vector3df(-2.5,0,-2.5)); /// Rotation needed, its to compensate emitter angle
	// ps31->setScale(core::vector3df(100,100,100));
	ps31->setScale(core::vector3df(1,1,1));
	ps31->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps31->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps31->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps31->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps31->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps31->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps31->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps31);
	/// Disabled:
    em31->setMinParticlesPerSecond(0);
	em31->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMALL UP FIRE N3)
	scene::IParticleSystemSceneNode* ps32 = smgr->addParticleSystemSceneNode(false);
    // ps32->setParent(nodeBand[0]);

	scene::IParticleEmitter* em32 = ps32->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.15f,0.0f),     // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,0,0,0),               // darkest color
	video::SColor(0,255,255,255),         // brightest color
	100,1000,0,                           // min age, max age, angle
	core::dimension2df(1.0f,1.0f),        // min size
	core::dimension2df(13.f,13.f));       // max size

	ps32->setEmitter(em32); // Grabs the emitter
	em32->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf32a = ps32->createFadeOutParticleAffector(); // Affector: Fade out particles
	ps32->addAffector(paf32a);
	paf32a->drop();

	ps32->setPosition(core::vector3df(-49,105,-428)); // x, y, z: profundidade (+25)
	// ps32->setRotation(core::vector3df(-2.5,0,-2.5)); /// Rotation needed, its to compensate emitter angle
	// ps32->setScale(core::vector3df(100,100,100));
	ps32->setScale(core::vector3df(1,1,1));
	ps32->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps32->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps32->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps32->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps32->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps32->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps32->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps31);
	/// Disabled:
    em32->setMinParticlesPerSecond(0);
	em32->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMALL UP FIRE N4)
	scene::IParticleSystemSceneNode* ps33 = smgr->addParticleSystemSceneNode(false);
    // ps33->setParent(nodeBand[0]);

	scene::IParticleEmitter* em33 = ps33->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                          // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,0.15f,0.0f),     // initial direction. Y = speed
	0,0,                                  // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),          // darkest color
	video::SColor(0,0,0,0),               // darkest color
	video::SColor(0,255,255,255),         // brightest color
	100,1000,0,                           // min age, max age, angle
	core::dimension2df(1.0f,1.0f),        // min size
	core::dimension2df(13.f,13.f));       // max size

	ps33->setEmitter(em33); // Grabs the emitter
	em33->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf33a = ps33->createFadeOutParticleAffector(); // Affector: Fade out particles
	ps33->addAffector(paf33a);
	paf33a->drop();

	ps33->setPosition(core::vector3df(-147,105,-428)); // x, y, z: profundidade (+25)
	// ps33->setRotation(core::vector3df(-2.5,0,-2.5)); /// Rotation needed, its to compensate emitter angle
	// ps33->setScale(core::vector3df(100,100,100));
	ps33->setScale(core::vector3df(1,1,1));
	ps33->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps33->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps33->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps33->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps33->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps33->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps33->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps33);
	/// Disabled:
    em33->setMinParticlesPerSecond(0);
	em33->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMOKE MACHINE N1 - LEFT)
	scene::IParticleSystemSceneNode* ps50 = smgr->addParticleSystemSceneNode(false);
    // ps50->setParent(nodeBand[0]);

	scene::IParticleEmitter* em50 = ps50->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),     // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                            // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(-0.035f,0.010f,0.010f), // initial direction & speed
	0,0,                                    // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	video::SColor(0,0,0,0),                 // darkest color
	video::SColor(0,255,255,255),           // brightest color
	10000,30000,30,                         // min age, max age, angle
	core::dimension2df(5.0f,5.0f),          // min size
	core::dimension2df(20.f,20.f));         // max size

	ps50->setEmitter(em50); // Grabs the emitter
	em50->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf50a = ps50->createFadeOutParticleAffector(); /// Affector: Fade out particles
	ps50->addAffector(paf50a);
	paf50a->drop();

	scene::IParticleAffector* paf50b = ps50->createGravityAffector(core::vector3df(0.020f,  0.030f, 0.0f), 70000); /// Affector: Apply gravity to particles (when at age of 5000)
	ps50->addAffector(paf50b);
	paf50b->drop();

	ps50->setPosition(core::vector3df(357,125,-650)); // x, y, z
	// ps50->setRotation(core::vector3df(0,0,74));
	// ps50->setScale(core::vector3df(100,100,100));
	ps50->setScale(core::vector3df(1,1,1));
	ps50->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps50->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps50->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps50->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps50->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps50->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps50->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps50);
	/// Disabled:
    em50->setMinParticlesPerSecond(0);
	em50->setMaxParticlesPerSecond(0);


    /// Create particle system: (STAGE SMOKE MACHINE N2 - RIGHT)
	scene::IParticleSystemSceneNode* ps51 = smgr->addParticleSystemSceneNode(false);
    // ps51->setParent(nodeBand[0]);

	scene::IParticleEmitter* em51 = ps51->createBoxEmitter( /// BOX Emitter
	core::aabbox3d<f32>(-1,0,-1,1,1,1),      // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                             // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.035f,-0.010f,-0.010f), // initial direction & speed
	0,0,                                     // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
	// video::SColor(0,0,0,255),             // darkest color
	video::SColor(0,0,0,0),                  // darkest color
	video::SColor(0,255,255,255),            // brightest color
	10000,30000,20,                          // min age, max age, angle
	core::dimension2df(5.0f,5.0f),           // min size
	core::dimension2df(20.f,20.f));          // max size

	ps51->setEmitter(em51); // Grabs the emitter
	em51->drop(); // We can drop it here, without deleting it

	scene::IParticleAffector* paf51a = ps51->createFadeOutParticleAffector(); /// Affector: Fade out particles
	ps51->addAffector(paf51a);
	paf51a->drop();

	scene::IParticleAffector* paf51b = ps51->createGravityAffector(core::vector3df(-0.020f,  0.030f, 0.0f), 70000); /// Affector: Apply gravity to particles (when at age of 5000)
	ps51->addAffector(paf51b);
	paf51b->drop();

	ps51->setPosition(core::vector3df(-357,125,-650)); // x, y, z
	// ps51->setRotation(core::vector3df(0,0,-74));
	// ps51->setScale(core::vector3df(100,100,100));
	ps51->setScale(core::vector3df(1,1,1));
	ps51->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps51->setMaterialTexture(0, driver->getTexture("particles_bolinhas.png"));
	// ps51->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps51->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	// ps51->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);  // Transparent texture.
	ps51->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps51->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	// effect->excludeNodeFromLightingCalculations(ps51);
	/// Disabled:
    em51->setMinParticlesPerSecond(0);
	em51->setMaxParticlesPerSecond(0);


	std::cout << "All Particles... Ok" << std::endl;




    /// ##########################################################################################
    /// ############# PARTICLES - END ############################################################
    /// ##########################################################################################



    /// Loading Bar: #8
    LoadBar->setPercent(70); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);



    /// ############# CREATE CAMERAS ############################################################

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Creating Cameras..." << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    /// Cameras:
    ICameraSceneNode* camera[30];
    f32 SPEED_CAMERA = 0.0017f; // Default: 0.0010f;

    // Cameras target is: (Selected band position)
    cameraTarget = vector3df(0,0,0); /// core::vector3df(nodeBand[BAND_SELECTED]->getPosition().X, nodeBand[BAND_SELECTED]->getPosition().Y+50, nodeBand[BAND_SELECTED]->getPosition().Z);

    // Camera 1:
    camera[1] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[1]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[1]->setPosition(core::vector3df(0,0,0));         // Set Camera Position
    camera[1]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[1]->setInputReceiverEnabled(false);
    camera[1]->setFarValue(3000.0f); /// this increases the shadows visible range
    camera[1]->setFOV(0.55f); /// Default, will change in main loop at start (Just for a cool zoom effect)
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc0 = smgr->createFlyCircleAnimator(core::vector3df(0,110,-100), 2.0f, SPEED_CAMERA);
    camera[1]->addAnimator(animc0);
    animc0->drop();

    // Camera 2:
    camera[2] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[2]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[2]->setPosition(core::vector3df(0,0,0));         // Set Camera Position
    camera[2]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[2]->setInputReceiverEnabled(false);
    camera[2]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc1 = smgr->createFlyCircleAnimator(core::vector3df(0,350,-150), 130.0f, SPEED_CAMERA);
    camera[2]->addAnimator(animc1);
    animc1->drop();

    // Camera 3:
    camera[3] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[3]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[3]->setPosition(core::vector3df(-300,110,-100));         // Set Camera Position
    camera[3]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[3]->setInputReceiverEnabled(false);
    camera[3]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc2 = smgr->createFlyCircleAnimator(core::vector3df(-300,120,-100), 5.0f, SPEED_CAMERA);
    camera[3]->addAnimator(animc2);
    animc2->drop();

    // Camera 4:
    camera[4] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[4]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[4]->setPosition(core::vector3df(300,110,-100));         // Set Camera Position
    camera[4]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[4]->setInputReceiverEnabled(false);
    camera[4]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc3 = smgr->createFlyCircleAnimator(core::vector3df(300,120,-100), 5.0f, SPEED_CAMERA);
    camera[4]->addAnimator(animc3);
    animc3->drop();

   // Camera 5:
    camera[5] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[5]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[5]->setPosition(core::vector3df(0,0,0));         // Set Camera Position
    camera[5]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[5]->setInputReceiverEnabled(false);
    camera[5]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc4 = smgr->createFlyCircleAnimator(core::vector3df(0,260,-40), 5.0f, SPEED_CAMERA);
    camera[5]->addAnimator(animc4);
    animc4->drop();

    // Camera 6:
    camera[6] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[6]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[6]->setPosition(core::vector3df(0,0,0));         // Set Camera Position
    camera[6]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[6]->setInputReceiverEnabled(false);
    camera[6]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc5 = smgr->createFlyCircleAnimator(core::vector3df(0,150,-200), 170.0f, SPEED_CAMERA);
    camera[6]->addAnimator(animc5);
    animc5->drop();

    // Camera 7:
    camera[7] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[7]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[7]->setPosition(core::vector3df(0,240,-650));    // Set Camera Position
    camera[7]->setTarget(core::vector3df(0,-20,50));        /// Set Camera Target (spectators)
    camera[7]->setInputReceiverEnabled(false);
    camera[7]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc7 = smgr->createFlyCircleAnimator(core::vector3df(0,240,-650), 3.0f, SPEED_CAMERA);
    camera[7]->addAnimator(animc7);
    animc7->drop();

    // Camera 8:
    camera[8] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[8]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[8]->setPosition(core::vector3df(300,150,-250));  // Set Camera Position
    camera[8]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[8]->setInputReceiverEnabled(false);
    camera[8]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc8 = smgr->createFlyCircleAnimator(core::vector3df(300,150,-250), 5.0f, SPEED_CAMERA);
    camera[8]->addAnimator(animc8);
    animc8->drop();

    // Camera 9:
    camera[9] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[9]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[9]->setPosition(core::vector3df(-300,150,-250)); // Set Camera Position
    camera[9]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[9]->setInputReceiverEnabled(false);
    camera[9]->setFarValue(3000.0f); /// this increases the shadows visible range
    // Add fly circle animator to this camera:
    scene::ISceneNodeAnimator* animc9 = smgr->createFlyCircleAnimator(core::vector3df(-300,150,-250), 5.0f, SPEED_CAMERA);
    camera[9]->addAnimator(animc9);
    animc9->drop();

    // Camera 10:
    camera[10] = smgr->addCameraSceneNode(0, vector3df(0,0,0), vector3df(0,0,0)); // Auto animated Camera
    camera[10]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[10]->setPosition(core::vector3df(300,105,-50));   // Set Camera Position
    camera[10]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[10]->setInputReceiverEnabled(false);
    camera[10]->setFarValue(3000.0f); /// this increases the shadows visible range

    // Initialize follow spiline to this camera (needs two dummy nodes):
    scene::ISceneNode* CameraPositionNode = smgr->addEmptySceneNode();

    // Camera -> createFollowSplineAnimator():
    irr::core::array<core::vector3df> camPositions;
    // Fill position array:
    camPositions.push_back(core::vector3df( 300, 105,  -50)); // Corner
    camPositions.push_back(core::vector3df(   0, 445,  -40)); // Note: 445 max height
    camPositions.push_back(core::vector3df(-300, 105,  -50)); // Corner
    camPositions.push_back(core::vector3df(-300, 400, -300)); // Corner
    camPositions.push_back(core::vector3df(   0, 102, -270));
    camPositions.push_back(core::vector3df( 300, 400, -300)); // Corner

    // Add follow positions spline animator to this node:
    scene::ISceneNodeAnimator* animCam = smgr->createFollowSplineAnimator(0, camPositions, 0.2f, 0.2f, true, true); // CreateFollowSplineAnimator(startTime,points,speed,tightness,loop,pingpong);
    CameraPositionNode->addAnimator(animCam);
    animCam->drop();


    /// Camera 20: FPS (for DEBUG only)
    /// Set FPS camera keys:
    SKeyMap keyMap[10];
    keyMap[0].Action  = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action  = EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = KEY_DOWN;
    keyMap[2].Action  = EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = KEY_LEFT;
    keyMap[3].Action  = EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = KEY_RIGHT;

    camera[20] = smgr->addCameraSceneNodeFPS(0, 100, 0.1, -1, keyMap, 2);
    camera[20]->setMaterialFlag(video::EMF_LIGHTING, false); // Node is affected by LIGHT?
    camera[20]->setPosition(core::vector3df(0,350,-50)); // Set Camera Position
    camera[20]->setTarget(cameraTarget);                     // Set Camera Target (band)
    camera[20]->setInputReceiverEnabled(true);
    camera[20]->setFarValue(10000.0f); /// this increase a shadow visible range


    std::cout << "Cameras... Ok" << std::endl;




    /// Loading Bar: #9
    LoadBar->setPercent(80); // 0 to 100
    LoadBar->draw();
    // LoadBar->setVisible(false);



    /// ############# SPECIAL EFFECTS ##############################################################


    /// EFFECTS (1/2): ( From: "libraries\PostProcessor\..." ) -- HELP at --> http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=34830
    // Starting with effects (Post processing):
    ppRenderer = new CRendererPostProc(smgr, dimension2du(1024,1024), true, true, SColor(255,100,101,140));
    // When setting up the effect, the parameters are: Input, size of output, effect ID (see CEffectPostProc.h for full list), effect parameters (in this case, blur size):
    /// All effects at "libraries\PostProcessor\CEffectPostProc.cpp". (eg: PP_BLUR, PP_BLURDOFFAR, PP_BLURDOFNEAR, PP_BLURDOF, PP_BLOOM, PP_GLOOM,...)


    ppEffects[0] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_GREYSCALE, 1.0f);   ppEffectName[0]="Greyscale";
    ppEffects[1] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_BLOOM, 1.0f);       ppEffectName[1]="Bloom";
    ppEffects[2] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_GLOOM, 10.0f);      ppEffectName[2]="Gloom";
    ppEffects[3] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_BLUR, 0.01f );      ppEffectName[3]="Blur";         /// Ugly!...
    ppEffects[4] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_RADIALBLUR, 1.0f);  ppEffectName[4]="Radial Blur";  /// Ugly!...
    ppEffects[5] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_MONITOR, 2.0f );    ppEffectName[5]="Monitor";      /// Ugly!...
    ppEffects[6] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_WATERCOLOR, 1.5f ); ppEffectName[6]="Water Color";  /// Ugly!...
    ppEffects[7] = new CEffectPostProc(ppRenderer, dimension2du(1024,1024), PP_SATURATE, 1.0f);    ppEffectName[7]="Saturate";     /// Ugly!...

    for(int i=0; i<ppeCount; i++){ ppEffects[i]->setQuality(PPQ_GOOD); }; // Change to a better quality effects - not all shaders will respect these, but they can be used to hint the rendering standard required.

    bool ppeffectsEnable = false;           // Post prosessing effects
    int ppEffectsRand = rand()%ppeCount;    // Post prosessing (default) random selected effect

    std::cout << "Effects... Ok" << std::endl;




    /// ##########################################################################################
    /// ############# USER INTERFACE (GUI) #######################################################
    /// ##########################################################################################

    std::cout << "------------------------------------------------------------------------------" << std::endl;
    std::cout << "Creating GUI..." << std::endl;
    std::cout << "------------------------------------------------------------------------------" << std::endl;

    /// Initialize GUI stuff:
    skin  = guienv->getSkin();
    fontMain = guienv->getFont("font1.bmp"); // Get Texts Font
    // IGUIFont* font2 = guienv->getFont("font2.bmp"); // Get Texts Font


    /// Fonts for lyrics:
    font_lyrics[1] = guienv->getFont("font_lyrics_14.bmp");
    font_lyrics[2] = guienv->getFont("font_lyrics_18.bmp");
    font_lyrics[3] = guienv->getFont("font_lyrics_22.bmp");
    font_lyrics[4] = guienv->getFont("font_lyrics_26.bmp");
    font_lyrics[5] = guienv->getFont("font_lyrics_36.bmp");
    font_lyrics[6] = guienv->getFont("font_lyrics_68.bmp");


    /// Apply font to GUI:
    if(fontMain){ skin->setFont(fontMain); };
    skin->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,210,235,255)); // Set Texts Font Color
    // IGUIStaticText->setTextAlignment - EGUIA_CENTER

    // SCREEN_WIDTH  = driver->getScreenSize().Width;
    // SCREEN_HEIGHT = driver->getScreenSize().Height;
    // s32 pos = ((IGUIScrollBar*)GUIEvent.Caller)->getPos();

    /// Set GUI Colors: (transparencies, etc...)
    std::cout << "Total GUI Elements: " << EGDC_COUNT << std::endl; // Write log
    /// --> Paint some GUI elements...
    for(s32 i=0; i<8; i++){ guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, SColor(200,25,25,40)); }; /// --> Paint some GUI elements... (opacity, red, green, blue)
    /// ID:0 - Windows right,bottom borders (shadows) + cursors
    /// ID:1 - Windows right,bottom borders + cursors
    /// ID:2 - Cursors + window fills
    /// ID:3 - Windows left,top borders + cursors
    /// ID:4 - Input text right,bottom borders
    /// ID:5 - Windows Toolbar
    /// ID:6 - Unknown...
    /// ID:7 - Unknown...
    /// ID:8 - GUI Texts

    /// --> ...Override this... (Painted in the above loop)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)2,  SColor(200,15,15,25));    /// --> ID:2  - Cursors + window fills (opacity, red, green, blue)

    /// --> ...Then, these ones...
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)9,  SColor(150,255,255,255)); /// --> ID:9  - Text Fields Colors (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)10, SColor(200,50,50,100));   /// --> ID:10 - Selectors Background Color (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)11, SColor(255,255,0,0));  /// --> ID:11 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)12, SColor(255,255,0,0));  /// --> ID:12 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)13, SColor(255,255,0,0));  /// --> ID:13 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)14, SColor(255,255,0,0));  /// --> ID:14 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)15, SColor(255,255,0,0));  /// --> ID:15 - Unknown... (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)16, SColor(100,200,200,250)); /// --> ID:16 - Scrollbars (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)17, SColor(255,255,0,0));  /// --> ID:17 - Unknown... (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)18, SColor(200,200,200,200)); /// --> ID:18 - Cursors and Window Close button (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)19, SColor(200,200,200,200)); /// --> ID:19 - Directories Icons Colors (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)20, SColor(255,255,0,0));  /// --> ID:20 - Unknown... (opacity, red, green, blue)
    // guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)21, SColor(255,255,0,0));  /// --> ID:21 - Unknown... (opacity, red, green, blue)
    guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)22, SColor(80,0,0,0));        /// --> ID:22 - Text Path Background Color (opacity, red, green, blue)


    /// --> ...Finally...
    for(s32 i=23; i<gui::EGDC_COUNT; i++){ guienv->getSkin()->setColor((EGUI_DEFAULT_COLOR)i, SColor(255,255,255,255)); }; /// --> Paint any other GUI unknown elements... (opacity, red, green, blue)



    /// GUI Colors:
    /// http://irrlicht.convextech.ca/IrrlichtDocument/html/namespaceirr_1_1gui.html#abd15860fde29833c48daff5f95d5467a
    // guienv->getSkin()->setColor(EGDC_ICON_HIGH_LIGHT, SColor(255, 255,0,0));
    // guienv->getSkin()->setColor(EGDC_3D_HIGH_LIGHT, SColor(255, 255,0,0));
    // guienv->getSkin()->setColor(EGDC_3D_DARK_SHADOW, SColor(255, 255,0,0));
    // guienv->getSkin()->setColor(EGDC_3D_SHADOW, SColor(255, 255,0,0));


    /// Create a window: (elements container)
    win1 = guienv->addWindow(core::rect<s32>(5,5,180,SCREEN_HEIGHT-5), false, L"Win1");
    // win1->setRelativePosition(core::rect<s32>(5,5,180,SCREEN_HEIGHT-5));
    win1->getCloseButton()->setVisible(false); // Hides the irrlicht window´s default close (X) button
    win1->setDraggable(true);
    win1->setDrawTitlebar(false);
    win1->setVisible(true);
    int win1Width  = 180;
    int win1Height = SCREEN_HEIGHT-5;

    /// Create a window: (elements container)
    win2 = guienv->addWindow(core::rect<s32>(185,5,400,65), false, L"Win2");
    // win2->setRelativePosition(core::rect<s32>(5,5,180,SCREEN_HEIGHT-5));
    win2->getCloseButton()->setVisible(false); // Hides the irrlicht window´s default close (X) button
    win2->setDraggable(true);
    win2->setDrawTitlebar(false);
    win2->setVisible(true);
    int win2Width  = 400;
    int win2Height = 65;

    /// Add an icon to that window:
    video::ITexture* img = driver->getTexture("drag.png");
    // IGUIImage* drag = guienv->addImage(img, core::position2d<s32>(347,38), true, win2, -1, 0);
    guienv->addImage(img, core::position2d<s32>(347,38), true, win2, -1, 0);





    /// Disable window (is visible, but cant be used)
    // guienv->getSkin()->setColor(EGDC_WINDOW, SColor(255, 255,0,0));
    // IGUIInOutFader* fader;
    // fader = device->getGUIEnvironment()->addInOutFader();
    // fader->setColor(video::SColor(0,255,255,255));
    // fader->fadeIn(900);

    /// Window N1 - Add texts:
    // IGUIStaticText* texts[50]={};
    int sy = 10; // Texts starting y position

    sy=10;
    // texts[16] = guienv->addStaticText(L"IMP - Irrlicht Music Player", core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win1); /// border? - text wrap? - parent name
    // texts[16]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=0;
    texts[0] = guienv->addStaticText(L"Show/Hide Menus: M", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[0]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[15] = guienv->addStaticText(L"Display Infos: None (I)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[15]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[11] = guienv->addStaticText(L"Spectators Sounds: 1, 2", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[11]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[1] = guienv->addStaticText(L"Camera: Auto (C)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[1]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    /*
    sy+=14;
    texts[10] = guienv->addStaticText(L"Zoom: Z, X", core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win1); // border? wordwrap?
    texts[10]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue
    */


    /// External settings: --->

    sy+=28;
    texts[14] = guienv->addStaticText(L"Band: Man (B)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[14]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[22] = guienv->addStaticText(L"Show: Normal (S)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[22]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[19] = guienv->addStaticText(L"Lyrics Color: White (L)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[19]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[18] = guienv->addStaticText(L"Lyrics Size: 3 (K)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[18]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[23] = guienv->addStaticText(L"Sounds: On (O)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[23]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[24] = guienv->addStaticText(L"Clock: Off (H)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[24]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    /// ---------------------->

    sy+=28;
    texts[8] = guienv->addStaticText(L"Play Mode: Single (P)", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[8]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    // sy+=14;
    // texts[21] = guienv->addStaticText(L"Pitch: Normal (7, 8)", core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win); // border? wordwrap?
    // texts[21]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    // sy+=14;
    // volume 	0.0 = silence, 1.0 = maximum volume (default)
    // stringw volt = L""; volt += L"Volume: "; for(f32 i=0.0; i<GLOBAL_VOLUME; i+=0.1){ volt += L"|"; }; volt += L" (9, 0)";
    // texts[20] = guienv->addStaticText(volt.c_str(), core::rect<s32>(10,sy,200,SCREEN_HEIGHT), false, false, win); // border? wordwrap?
    // texts[20]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14;
    texts[13] = guienv->addStaticText(L"Play/Pause: Space", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[13]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    /// ListBox texts:
    sy+=55;
    texts[2] = guienv->addStaticText(L"\nSelect Directory: D\nSelect: Click or Arrow Keys", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[2]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue

    sy+=14; /// Empty - Not used by now.
    texts[17] = guienv->addStaticText(L"", core::rect<s32>(10,sy,win1Width,win1Height), false, false, win1); /// border? - text wrap? - parent name
    texts[17]->setOverrideColor(video::SColor(255,210,235,255)); // opacity, red, green, blue



    /// Other texts:
    texts[3] = guienv->addStaticText(L"Loading Music...", core::rect<s32>(SCREEN_WIDTH/2-50,SCREEN_HEIGHT/2,SCREEN_WIDTH,SCREEN_HEIGHT), false, false, 0); /// border? - text wrap? - parent name
    texts[3]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[3]->setVisible(false);



    /// Window N2 - Add texts:
    texts[4] = guienv->addStaticText(L"Position:\nVolume:\nPitch:", core::rect<s32>(10,10,50,50), false, true, win2); /// border, wrap, parent
    texts[4]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[4]->setTextAlignment(EGUIA_LOWERRIGHT, EGUIA_LOWERRIGHT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE

    /// Timmers:
    texts[5] = guienv->addStaticText(L"00:00:00 - 00:00:00", core::rect<s32>(220,10,win2Width,win2Height), false, false, win2); /// border? - text wrap? - parent name
    texts[5]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue
    texts[5]->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    /// Volume:
    stringw volumet = L""; for(float i=0.0; i<GLOBAL_VOLUME; i+=0.05){ volumet += L"|"; }; volumet += L" 50%";
    texts[26] = guienv->addStaticText(volumet.c_str(), core::rect<s32>(220,23,win2Width,win2Height), false, false, win2); /// border? - text wrap? - parent name
    texts[26]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue
    texts[26]->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    /// Pitch:
    stringw pitcht = L""; for(float i=0.0; i<MUSIC_PITCH; i+=0.1){ pitcht += L"|"; }; pitcht += L" 1.0 Normal";
    texts[27] = guienv->addStaticText(pitcht.c_str(), core::rect<s32>(220,37,win2Width,win2Height), false, false, win2); /// border? - text wrap? - parent name
    texts[27]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue
    texts[27]->setTextAlignment(EGUIA_UPPERLEFT, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    /// Music has lyrics?
    texts[28] = guienv->addStaticText(L"Lyrics: No", core::rect<s32>(315,10,win2Width-12,win2Height), false, false, win2); /// border? - text wrap? - parent name
    texts[28]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue
    texts[28]->setTextAlignment(EGUIA_LOWERRIGHT, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE



    /// Display LOADING text: (Please standby, the Show will continue in a moment)
    texts[20] = guienv->addStaticText(L"", core::rect<s32>(0,0,SCREEN_WIDTH,SCREEN_HEIGHT), false, true, 0); /// border? - text wrap? - parent name
    texts[20]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[20]->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE

    /// Display PAUSED text: (show intermission)
    texts[12] = guienv->addStaticText(L"", core::rect<s32>(0,0,SCREEN_WIDTH,SCREEN_HEIGHT), false, true, 0); /// border? - text wrap? - parent name
    texts[12]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[12]->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE

    /// Display: Infos: Music Tags, Help, About, Nothing.
    texts[6] = guienv->addStaticText(L"", core::rect<s32>(190,5,SCREEN_WIDTH,SCREEN_HEIGHT), false, false, 0); /// border? - text wrap? - parent name
    // texts[6]->setOverrideColor(video::SColor(200,180,180,180)); // opacity, red, green, blue
    texts[6]->setOverrideColor(video::SColor(200,150,150,200)); // opacity, red, green, blue

    /// Display LYRICS:
    texts[7] = guienv->addStaticText(L"", core::rect<s32>(50,450,SCREEN_WIDTH-50,SCREEN_HEIGHT), false, true, 0); /// border? - text wrap? - parent name
    // texts[7]->setTextAlignment(EGUIA_CENTER, EGUIA_UPPERLEFT); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    texts[7]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
    texts[7]->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE
    /// texts[7]->setTextRestrainedInside(true);
    /// texts[7]->getParent()->bringToFront(texts[7]); // ---> Brings text to front (placed in main loop), but when over ListBox some elements dont accept clicks!


    /// The Loading Bars texts: (Loading...)
    texts[25] = guienv->addStaticText(L"", core::rect<s32>(50,SCREEN_HEIGHT-130,SCREEN_WIDTH,SCREEN_HEIGHT), false, false, 0); /// border? - text wrap? - parent name
    texts[25]->setOverrideColor(video::SColor(255,150,150,200));
    texts[25]->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER); // ALIGN: horizontal, vertical -> Enumerator: EGUIA_UPPERLEFT, EGUIA_LOWERRIGHT, EGUIA_CENTER, EGUIA_SCALE



    std::cout << "GUI texts... Ok" << std::endl;

    /*
    /// Add buttons, one for each found song: (loop)
    int mc = musicList.size()-1;  // Get total songs count
    int sPos = 185;                // Buttons starting Y position
    IGUIButton* button[10]={};
    for(int i=0; i<=mc; i++){
    stringw songPath = musicList[i].c_str(); // eg: stringw text("Hello");
    stringw songNameOnly = musicListNamesOnly[i].c_str();
    stringw songExtensionOnly = musicExtensionsOnly[i].c_str();
    stringw allWords = ""; allWords += "["; allWords += songExtensionOnly; allWords += "] "; allWords += songNameOnly;
    button[i] = guienv->addButton(rect<s32>(position2di(10,sPos), dimension2di(155,20)), win, -1, allWords.c_str());
    // button[i] = guienv->addButton(rect<s32>(position2di(10,sPos), dimension2di(155,20)), win, 50, songPath.c_str());
    button[i]->setID(i+100); // set IDs, IDs counting from 100, in this case
    sPos += 20;
    }
    */


    /// Create GUI ListBox:
    /// Customize --> http://irrlicht.convextech.ca/IrrlichtDocument/html/namespaceirr_1_1gui.html#a7da705f0a0b4aa5385e6842adf409cb6
    /// gui::IGUIListBox* musicListBox;
    musicListBox = guienv->addListBox(core::rect<s32>(10,300,165,SCREEN_HEIGHT-20), win1, -1, true);
    musicListBox->setAutoScrollEnabled(true); // Auto scroll to selected item?
    musicListBox->setDrawBackground(false);
    musicListBox->setItemHeight(19);
    /*
    musicListBox->setItemOverrideColor(musicID, video::SColor(255,210,235,255));
    musicListBox->setItemOverrideColor(musicID, EGUI_LBC_TEXT, video::SColor(255,210,235,255)); // Color for selected item
    musicListBox->setItemOverrideColor(musicID, EGUI_LBC_TEXT_HIGHLIGHT, video::SColor(255,210,235,255)); // Color for selected item
    musicListBox->setItemOverrideColor(musicID, EGUI_LBC_ICON, video::SColor(255,210,235,255)); // Color for selected item
    musicListBox->setItemOverrideColor(musicID, EGUI_LBC_ICON_HIGHLIGHT, video::SColor(255,210,235,255)); // Color for selected item
    */

    /// Fill ListBox:
    if(musicListBox){ fillListBox(); };

    /*
    // Customize ListBox: (TODO)
    for (s32 i=0; i<musicList.size(); i++){ // for (s32 i=0; i<EGDC_COUNT; i++){
    SColor col = guienv->getSkin()->getColor((EGUI_DEFAULT_COLOR)i);
    col.setAlpha(30); // Alpha: 0 (transparent) to 255 (opaque)
    musicListBox->setItemOverrideColor((EGUI_LISTBOX_COLOR)i, col);
    }
    */

    /// Create GUI FileOpenDialog:
    /// http://irrlicht.sourceforge.net/docu/classirr_1_1gui_1_1_i_g_u_i_environment.html
    /// gui::IGUIFileOpenDialog* musicFileDialog;
    // IGUIFileOpenDialog * addFileOpenDialog (const wchar_t *title=0, bool modal=true, IGUIElement *parent=0, s32 id=-1, bool restoreCWD=false, io::path::char_type *startDir=0)=0
    /// musicFileDialog = guienv->addFileOpenDialog(L"SELECT THE NEW TARGET MUSIC FOLDER", true, 0, -1, false, 0);
    // musicFileDialog->setVisible(false); /// Starts hidden
    // musicFileDialog->getFileName();
    // musicFileDialog->getDirectoryName();

    /// Create GUI music scrollbar: (Position)
    // IGUIScrollBar (IGUIEnvironment *environment, IGUIElement *parent, s32 id, core::rect< s32 > rectangle)
    // musicPositionScrollbar = guienv->addScrollBar(true, rect<s32>(58, 12, 410, 22), win2, ID_SCROLLBAR_POSITION);
    musicPositionScrollbar = guienv->addScrollBar(true, rect<s32>(58, 12, 210, 22), win2, ID_SCROLLBAR_POSITION);
	musicPositionScrollbar->setMin(0);
	musicPositionScrollbar->setPos(0);
	musicPositionScrollbar->setMax(0);
    musicPositionScrollbar->setSmallStep(0);
    musicPositionScrollbar->setLargeStep(0);

    /// Create GUI music scrollbar: (Volume)
    musicVolumeScrollbar = guienv->addScrollBar(true, rect<s32>(58, 26, 210, 36), win2, ID_SCROLLBAR_VOLUME);
	musicVolumeScrollbar->setMin(0);
	musicVolumeScrollbar->setPos(10);
	musicVolumeScrollbar->setMax(20);
    musicVolumeScrollbar->setSmallStep(1);
    musicVolumeScrollbar->setLargeStep(20); /// setLargeStep(20) == setMax(20)

    /// Create GUI music scrollbar: (Pitch)
    musicPitchScrollbar = guienv->addScrollBar(true, rect<s32>(58, 40, 210, 50), win2, ID_SCROLLBAR_PITCH);
	musicPitchScrollbar->setMin(0);
	musicPitchScrollbar->setPos(10);
	musicPitchScrollbar->setMax(20);
    musicPitchScrollbar->setSmallStep(1);
    musicPitchScrollbar->setLargeStep(20); /// setLargeStep(20) == setMax(20)

	// ((IGUIListBox*)guienv->getRootGUIElement()->getElementFromId(100, true))->getSelected();
	// ((IGUIScrollBar*)guienv->getRootGUIElement()->getElementFromId(100, true))->getPos(); // setColor((EGUI_DEFAULT_COLOR)9, SColor(150,255,255,255));


	/// Create 3D CLOCK:
    scene::ISceneNode* nBody   = NULL;
    scene::ISceneNode* nHour   = NULL;
    scene::ISceneNode* nMinute = NULL;
    scene::ISceneNode* nSecond = NULL;
    CClockSceneNode* nodeClock = NULL;

    /// Create 3D CLOCK:
	nBody = smgr->addMeshSceneNode(smgr->getMesh("clock_body.obj"));
	nBody->setPosition(vector3df(0, 193, -691.5));
	for(u32 i=0; i<nBody->getMaterialCount(); i++){
    // nBody->getMaterial(i).setTexture(0, driver->getTexture(""));
    // nBody->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nBody->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nBody->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture
	nBody->getMaterial(i).Lighting = true;
	nBody->getMaterial(i).Shininess = 0.01f; // Set size of specular highlights (20.0f == Metallic)
	nBody->getMaterial(i).SpecularColor.set(3,3,3,3); /// The lights reflexes
	// nBody->getMaterial(i).DiffuseColor.set(0,0,0,0);
	// nBody->getMaterial(i).EmissiveColor.set(255,255,255,255);
	};
	nHour = smgr->addMeshSceneNode(smgr->getMesh("clock_hours.obj"));
	nHour->setPosition(vector3df(0, 193, -691.5));
	for(u32 i=0; i<nHour->getMaterialCount(); i++){
    // nHour->getMaterial(i).setTexture(0, driver->getTexture(""));
    // nHour->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nHour->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nHour->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture
	nHour->getMaterial(i).Lighting = true;
	nHour->getMaterial(i).Shininess = 0.01f; // Set size of specular highlights (20.0f == Metallic)
	nHour->getMaterial(i).SpecularColor.set(3,3,3,3); /// The lights reflexes
	// nHour->getMaterial(i).DiffuseColor.set(0,0,0,0);
	// nHour->getMaterial(i).EmissiveColor.set(255,255,255,255);
	};
	nMinute = smgr->addMeshSceneNode(smgr->getMesh("clock_minutes.obj"));
	nMinute->setPosition(vector3df(0, 193, -691.5));
	for(u32 i=0; i<nMinute->getMaterialCount(); i++){
	// nMinute->getMaterial(i).setTexture(0, driver->getTexture(""));
    // nMinute->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nMinute->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nMinute->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture
	nMinute->getMaterial(i).Lighting = true;
	nMinute->getMaterial(i).Shininess = 0.01f; // Set size of specular highlights (20.0f == Metallic)
	nMinute->getMaterial(i).SpecularColor.set(3,3,3,3); /// The lights reflexes
	// nMinute->getMaterial(i).DiffuseColor.set(0,0,0,0);
	// nMinute->getMaterial(i).EmissiveColor.set(255,255,255,255);
	};
	nSecond = smgr->addMeshSceneNode(smgr->getMesh("clock_seconds.obj"));
	nSecond->setPosition(vector3df(0, 193, -691.5));
	for(u32 i=0; i<nSecond->getMaterialCount(); i++){
	// nSecond->getMaterial(i).setTexture(0, driver->getTexture(""));
    // nSecond->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nSecond->getMaterial(i).setTexture(1, driver->getTexture("reflexes.jpg"));
	nSecond->getMaterial(i).getTextureMatrix(1).setTextureScale(0.1f,0.1f); /// Repeat (tile) the texture
	nSecond->getMaterial(i).Lighting = true;
	nSecond->getMaterial(i).Shininess = 0.01f; // Set size of specular highlights (20.0f == Metallic)
	nSecond->getMaterial(i).SpecularColor.set(3,3,3,3); /// The lights reflexes
	// nSecond->getMaterial(i).DiffuseColor.set(0,0,0,0);
	// nSecond->getMaterial(i).EmissiveColor.set(255,255,255,255);
	};
	nodeClock = new CClockSceneNode(device, smgr, nHour, nMinute, nSecond);
	nodeClock->drop();
	ITimer *real_time = device->getTimer();
	int curTimeH = real_time->getRealTimeAndDate().Hour;
	int curTimeM = real_time->getRealTimeAndDate().Minute;
	int curTimeS = real_time->getRealTimeAndDate().Second;
	nodeClock->setTime(curTimeH, curTimeM, curTimeS);
	nodeClock->setUpdatesPerSecond(20);
	nodeClock->setAutoUpdate(true);
	// nodeClock->setShowSeconds(true);
	nodeClock->setUseRealTime(true);

	nBody->setVisible(false);        // Clock body
	nodeClock->setVisibility(false); // Clock pointers



	/// Add GUI images:
    IGUIImage* image[10]={};
	image[0] = guienv->addImage(driver->getTexture("logotipo_1.png"), position2d<int>(SCREEN_WIDTH-145,10));
	// image[1] = guienv->addImage(driver->getTexture("logotipo_2.png"), position2d<int>(SCREEN_WIDTH-145,10));
	// image[2] = guienv->addImage(driver->getTexture("logotipo_3.png"), position2d<int>(SCREEN_WIDTH-145,10));
	// guienv->addGUIElement("myButton", IGUIButton(core::rect< s32> (0,0,200,50), 0, 101, wchar_t "mybutton", wchar_t "clickhere")  )



	std::cout << "GUI images... Ok" << std::endl;

	/// ##########################################################################################
    /// ############# USER INTERFACE (GUI) - END #################################################
    /// ##########################################################################################

