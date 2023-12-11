// Jorge Rosa (1999 - 2018 Portugal)
// Email: jorge.bigarte@gmail.com
// Portfolio: https://sites.google.com/site/jorgerosaportfolio
// Facebook: https://www.facebook.com/jorge.bigarte
// Youtube: https://www.youtube.com/user/jorgeerosa/videos

/// ------------------------------------------------------------------------------------------------------

/// Sky:
scene::ISceneNode* nodeSky;
/// Plane:
scene::IAnimatedMeshSceneNode* nodePlane;
/// Helice:
scene::IAnimatedMeshSceneNode* nodeHelice;
/// Camera:
scene::ICameraSceneNode* camera;
/// GUI (2D Images):
IGUIImage* image;


/// Camera:
void createCamera(){
    /// Add a camera:
    // camera = smgr->addCameraSceneNode();
    // virtual ICameraSceneNode * addCameraSceneNodeMaya (ISceneNode *parent=0, f32 rotateSpeed=-1500.f, f32 zoomSpeed=200.f, f32 translationSpeed=1500.f, s32 id=-1, f32 distance=70.f, bool makeActive=true)=0
    camera = smgr->addCameraSceneNodeMaya(0, -200.0f, 5000.0f, 200.0f, -1, 800.0f, true);
    // camera->setPosition(core::vector3df(0,0,-1700));
    camera->setPosition(vector3df(0,0,0));
    camera->setRotation(core::vector3df(0,0,0));
    camera->setTarget(core::vector3df(0,0,0));
    camera->setFOV(0.40f); /// To make the (almost) 2D view effect
    camera->setFarValue(15000.0f);

    /// Add a light to the camera:
    scene::ILightSceneNode* light = smgr->addLightSceneNode(camera);
    SLight lightdata;
    lightdata.DiffuseColor = video::SColorf(1.0f, 1.0f, 1.0f, 0.5f);
    lightdata.Radius = 1000.0f;
    lightdata.CastShadows = true;
    light->setLightData(lightdata);
};


/// Logo: (The TV)
void createTV(){
    image = guienv->addImage(driver->getTexture("TV.png"), position2d<int>(0,0));
    image->setScaleImage(true);
    // image->setMinSize(dimension2du(driver->getScreenSize().Width, driver->getScreenSize().Height));
    image->setMaxSize(dimension2du(driver->getScreenSize().Width, driver->getScreenSize().Height));
};


/// Sky:
void createSky(){
    nodeSky = smgr->addSkyDomeSceneNode(driver->getTexture("World-Map.jpg"), 16, 8, 1.0f, 2.0f, 50.f, 0, -1);
    scene::ISceneNodeAnimator* anim = smgr->createRotationAnimator(core::vector3df(0.0,0.06,0.0));
    nodeSky->addAnimator(anim);
    anim->drop();
};


/// Nodes Plane and Helice:
void createPlane(){
	/// Node Plane:
	nodePlane = smgr->addAnimatedMeshSceneNode(smgr->getMesh("FAIREY-III.obj")); // For an animated node
	/// nodePlane->setParent(nodePlane); /// Child of...
	nodePlane->setPosition(vector3df(0.0f,0.0f,0.0f));
	nodePlane->setRotation(vector3df(0.0f,0.0f,0.0f));
	// nodePlane->setRotation(vector3df(-90.0f,0.0f,0.0f));
	nodePlane->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodePlane->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodePlane->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodePlane->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Hide render on the other side?
    nodePlane->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodePlane->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    /// Shadows:
    nodePlane->addShadowVolumeSceneNode();
    /// Textures:
    for(u32 i=0; i<nodePlane->getMaterialCount(); i++){
    nodePlane->getMaterial(i).setTexture(0, driver->getTexture("Plane-Fairey-IIID-Greys.jpg"));
	nodePlane->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodePlane->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0);
	nodePlane->getMaterial(i).Shininess = 250.0f; /// Set size of specular highlights
	};
	/// Animator: (Gentle Plane Movement)
	ISceneNodeAnimator* anim1 = smgr->createFlyStraightAnimator(core::vector3df(0,0,0), core::vector3df(10,10,10), 5000, true, true);
    nodePlane->addAnimator(anim1);
    anim1->drop();
	/// Node Helice:
	nodeHelice = smgr->addAnimatedMeshSceneNode(smgr->getMesh("FAIREY-III-HELICE.obj")); // For an animated node
	nodeHelice->setParent(nodePlane); /// Child of...
	nodeHelice->setPosition(vector3df(0.0f,-8.0f,54.0f));
	nodeHelice->setRotation(vector3df(0.0f,0.0f,0.0f));
	// nodeHelice->setRotation(vector3df(-90.0f,0.0f,0.0f));
	nodeHelice->setScale(core::vector3df(1.0f,1.0f,1.0f));
	nodeHelice->setMaterialFlag(video::EMF_LIGHTING, true);           // Node is affected by LIGHT?
    nodeHelice->setMaterialFlag(video::EMF_FOG_ENABLE, true);         // Node is affected by FOG?
    nodeHelice->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false); // Hide render on the other side?
    nodeHelice->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    nodeHelice->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    /// Shadows:
    nodeHelice->addShadowVolumeSceneNode();
    /// Textures:
    for(u32 i=0; i<nodeHelice->getMaterialCount(); i++){
    nodeHelice->getMaterial(i).setTexture(0, driver->getTexture("Plane-Fairey-IIID-Greys.jpg"));
	nodeHelice->getMaterial(i).getTextureMatrix(0).setTextureScale(1.0f,1.0f); /// Repeat (tile) the texture
	nodeHelice->getMaterial(i).EmissiveColor = video::SColor(0,0,0,0);
	nodeHelice->getMaterial(i).Shininess = 250.0f; /// Set size of specular highlights
	};
	/// Animator: (Helice Rotation)
	ISceneNodeAnimator* anim2 = smgr->createRotationAnimator(vector3df(0,0,350));
    nodeHelice->addAnimator(anim2);
    anim2->drop();
};


/// Nodes Smoke Particles:
void createSmokeParticles(){
    /// Particles: Smoke for exhaust N1:
	scene::IParticleSystemSceneNode* ps1 = smgr->addParticleSystemSceneNode(false);
	scene::IParticleEmitter* em1 = ps1->createRingEmitter( /// Ring Emitter
    vector3df(0.0f,0.0f,0.0f),        // center
    1,0,                              // radius / ring thickness
    vector3df(0.0f,0.0f,-0.40f),      // direction
    0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
    SColor(0,60,60,60),               // darkest color (a,r,g,b)
    SColor(0,0,0,0),                  // brightest color
    2000,8000,1,                      // lifeTimeMin / lifeTimeMax / maxAngleDegrees
    core::dimension2df(2.0f, 2.0f),   // minStartSize
    core::dimension2df(20.0f, 20.0f));  // maxStartSize
	ps1->setEmitter(em1); // Grabs the emitter
	em1->drop(); // We can drop it here, without deleting it
	scene::IParticleAffector* paf1 = ps1->createFadeOutParticleAffector(); // Fade out particles
	ps1->addAffector(paf1); // Same goes for the affector
	paf1->drop();
    ps1->setParent(nodePlane);
	ps1->setPosition(core::vector3df( 16,2.8,46)); // x, y, z: profundidade
	ps1->setRotation(vector3df(0.0f,0.0f,0.0f));
	ps1->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps1->setMaterialTexture(0, driver->getTexture("smoke.png"));
	ps1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps1->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps1->setMaterialFlag(video::EMF_FOG_ENABLE, true); // Node is affected by FOG?
	ps1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    em1->setMinParticlesPerSecond(50);
	em1->setMaxParticlesPerSecond(350);
	// TODO stop emiters, if no song playing:
	// ps1->getEmitter()->setMaxParticlesPerSecond(10);
    // ps1->getEmitter()->setMinParticlesPerSecond(10);
    // ps1->setEmitter();
    /// Particles: Smoke for exhaust N2:
	scene::IParticleSystemSceneNode* ps2 = smgr->addParticleSystemSceneNode(false);
	scene::IParticleEmitter* em2 = ps2->createRingEmitter( /// Ring Emitter
    vector3df(0.0f,0.0f,0.0f),        // center
    1,0,                              // radius / ring thickness
    vector3df(0.0f,0.0f,-0.40f),      // direction
    0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
    SColor(0,60,60,60),               // darkest color (a,r,g,b)
    SColor(0,0,0,0),                  // brightest color
    2000,8000,1,                      // lifeTimeMin / lifeTimeMax / maxAngleDegrees
    core::dimension2df(2.0f, 2.0f),   // minStartSize
    core::dimension2df(20.0f, 20.0f));  // maxStartSize
	ps2->setEmitter(em2); // Grabs the emitter
	em2->drop(); // We can drop it here, without deleting it
	scene::IParticleAffector* paf2 = ps2->createFadeOutParticleAffector(); // Fade out particles
	ps2->addAffector(paf2); // Same goes for the affector
	paf2->drop();
    ps2->setParent(nodePlane);
	ps2->setPosition(core::vector3df(-16,2.8,46)); // x, y, z: profundidade
	ps2->setRotation(vector3df(0.0f,0.0f,0.0f));
	ps2->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps2->setMaterialTexture(0, driver->getTexture("smoke.png"));
	ps2->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps2->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps2->setMaterialFlag(video::EMF_FOG_ENABLE, true); // Node is affected by FOG?
	ps2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    em2->setMinParticlesPerSecond(50);
	em2->setMaxParticlesPerSecond(350);
	// TODO stop emiters, if no song playing:
	// ps2->getEmitter()->setMaxParticlesPerSecond(10);
    // ps2->getEmitter()->setMinParticlesPerSecond(10);
    // ps2->setEmitter();
};

/// Nodes Clouds Particles:
void createCloudsParticles(){
    /// Particles:
	scene::IParticleSystemSceneNode* ps1 = smgr->addParticleSystemSceneNode(false);
	scene::IParticleEmitter* em1 = ps1->createRingEmitter( /// Ring Emitter
    vector3df(0.0f,0.0f,0.0f),        // center
    1000,0,                           // radius / ring thickness
    vector3df(0.0f,0.0f,-1.0f),       // direction
    0,0,                              // minParticlesPerSecond / maxParticlesPerSecond (Will change in main loop)
    SColor(0,255,255,255),            // darkest color (a,r,g,b)
    SColor(0,0,0,0),                  // brightest color
    10000,20000,0,                    // lifeTimeMin / lifeTimeMax / maxAngleDegrees
    core::dimension2df(100.0f, 100.0f),   // minStartSize
    core::dimension2df(500.0f, 500.0f));  // maxStartSize
	ps1->setEmitter(em1); // Grabs the emitter
	em1->drop(); // We can drop it here, without deleting it
	scene::IParticleAffector* paf1 = ps1->createFadeOutParticleAffector(); // Fade out particles
	ps1->addAffector(paf1); // Same goes for the affector
	paf1->drop();
    ps1->setParent(camera); /// Parent: Camera!
	ps1->setPosition(core::vector3df(0,100,0)); // x, y, z: profundidade
	ps1->setRotation(vector3df(0.0f,0.0f,0.0f));
	ps1->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps1->setMaterialTexture(0, driver->getTexture("clouds.png"));
	ps1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps1->setMaterialFlag(video::EMF_LIGHTING, true); // Node is affected by LIGHT?
	ps1->setMaterialFlag(video::EMF_FOG_ENABLE, false); // Node is affected by FOG?
	ps1->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    em1->setMinParticlesPerSecond(50);
	em1->setMaxParticlesPerSecond(100);
	// TODO stop emiters, if no song playing:
	// ps1->getEmitter()->setMaxParticlesPerSecond(10);
    // ps1->getEmitter()->setMinParticlesPerSecond(10);
    // ps1->setEmitter();
};


/// Create World:
void createWorld(){

	/// Camera:
    createCamera();

    /// Create Sky:
	createSky();

    /// Create Plane:
    createPlane();

    /// Create Smoke Particles:
    createSmokeParticles();

    /// Create Clouds Particles:
    createCloudsParticles();

    /// Create TV:
    createTV();
};


