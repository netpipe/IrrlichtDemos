


	/// Create SKY:
	ISceneNode* sky = smgr->addSkyDomeSceneNode(driver->getTexture("sky1.jpg"),4,4,1.0f,2.0f);
	sky->setMaterialFlag(video::EMF_LIGHTING, false);
    // Animate SKY:
	ISceneNodeAnimator* sky_animation = smgr->createRotationAnimator(vector3df(0, -0.02f, 0));
    sky->addAnimator(sky_animation);
    sky_animation->drop();

    /// Create dynamic CLOUDS:
//    scene::CCloudSceneNode* cloudLayer1;
//    scene::CCloudSceneNode* cloudLayer2;
//	// Add 1st cloud layer
//	cloudLayer1 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), smgr);
//	core::vector2d<f32> sp1 = vector2d<f32>(-0.010f, 0.0f);  // Defining the vector2d variable, before function (else it crashes)
//	cloudLayer1->setTranslation(sp1);                        // set translation (speed of the clouds)
//	cloudLayer1->getMaterial(0).setTexture(0, driver->getTexture("clouds_day_01.png"));
//	cloudLayer1->setCloudHeight(0.5f, 0.1f, -0.05f);
//	cloudLayer1->setTextureScale(1.0f);
//	cloudLayer1->setRotation(vector3df(0,90,0));
//	cloudLayer1->setMaterialFlag(video::EMF_LIGHTING, true);
//	// cloudLayer1->setID(ID_ISNOTPICKABLE);
//	// Add 2nd cloud layer
//	cloudLayer2 = new scene::CCloudSceneNode(smgr->getRootSceneNode(), smgr);
//	core::vector2d<f32> sp2 = vector2d<f32>(-0.005f, 0.0f);  // Defining the vector2d variable, before function (else it crashes)
//	cloudLayer2->setTranslation(sp2);                        // set translation (speed of the clouds)
//	cloudLayer2->getMaterial(0).setTexture(0, driver->getTexture("clouds_day_02.png"));
//	cloudLayer2->setCloudHeight(0.4f, 0.02f, -0.1f);
//	cloudLayer2->setTextureScale(1.0f);
//	cloudLayer2->setRotation(vector3df(0,90,0));
//	cloudLayer2->setMaterialFlag(video::EMF_LIGHTING, true);
	// cloudLayer2->setID(ID_ISNOTPICKABLE);

	/// Main light:
	// smgr->setAmbientLight(video::SColorf(1.0f,1.0f,1.0f,1.0f));
	smgr->setAmbientLight(video::SColor(0,255,255,255)); // o,r,g,b
	/// Shadows color:
	// smgr->setShadowColor(video::SColor(150,0,0,0)); // o,r,g,b
	if(IRRSHADOWS){ smgr->setShadowColor(video::SColor(150,0,0,0)); }; // o,r,g,b
	/// Fog settings:
	driver->setFog(video::SColor(0,255,255,255), video::EFT_FOG_LINEAR, 50, 180, 1.0f, true, false); // SetFog(color,linearFog,start,end,density,pixelFog As Boolean,rangeFog As Boolean)












	/// XEffects:
//	if(XEFFECTS){ effect = new EffectHandler(device, driver->getScreenSize()); }


//	if(XEFFECTS){
    /*
    // Set the background clear color to sky blue.
	effect->setClearColour(SColor(255, 50, 100, 200));

	// Here we enable the depth pass and add the terrain scene node to the depth pass.
	effect->enableDepthPass(true);


	// Set the water normal map as our post processing user texture.
	effect->setPostProcessingUserTexture(driver->getTexture("road_textures.png"));

	// Add our post-processing effect.
	/// const stringc shaderExt = (driver->getDriverType() == EDT_DIRECT3D9) ? ".hlsl" : ".glsl";
	effect->addPostProcessingEffectFromFile(core::stringc("Water_ps") + shaderExt);
	effect->addPostProcessingEffectFromFile("Water_vs.glsl");


	// Add a terrain scene node.
	ISceneNode* terrain = smgr->addTerrainSceneNode("terrain_heightmap.png", 0, -1, vector3df(-250.0f, -20.0f, 0.0f), vector3df(0.0f, 0.0f, 0.0f), vector3df(1.0f, 0.2f, 1.0f), SColor(0,0,0,0), 5, ETPS_17, 1);
	terrain->getMaterial(0).setTexture(0, driver->getTexture("terrain_texture.jpg"));
	terrain->getMaterial(0).Lighting = true;

	// Add terrain to depth pass.
	effect->addNodeToDepthPass(terrain);
	*/
//	};





















	/// LIGHTS
	/// Sun light:
	if(IRRSHADOWS){
	scene::ISceneNode* sunNode = smgr->addLightSceneNode(0, core::vector3df(0,4,10), video::SColor(255,255,0,0), 20.0f);
	// Animator:
    scene::ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(vector3df(0,4,10), 3, 0.003); // position, radius, speed
    sunNode->addAnimator(anim);
    anim->drop();
    // Attach billboard to LIGHT:
	scene::ISceneNode* bill1 = smgr->addBillboardSceneNode(sunNode, core::dimension2d<f32>(1,1));
	bill1->setMaterialFlag(video::EMF_LIGHTING, false);
	bill1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	bill1->setMaterialTexture(0, driver->getTexture("light.png"));
	// bill1->setVisible(false);
	// sunNode->enableCastShadow(true);
	// sunNode->setShadowmesh();
	};





    /// Light: (for XEffects TESTING)
    if(XEFFECTS){
    scene::ILightSceneNode* DUMMYLIGHT = smgr->addLightSceneNode(0, core::vector3df(0,4,10), video::SColor(255,255,0,0), 20.0f); // addLightSceneNode( ISceneNode, position As Vector3D, Colorf, radius As Single, id )
    // Animator:
    scene::ISceneNodeAnimator* anim = smgr->createFlyCircleAnimator(vector3df(0,4,10), 3, 0.003); // position, radius, speed
    DUMMYLIGHT->addAnimator(anim);
    anim->drop();
	// Billboard:
	scene::ISceneNode* DUMMYBILL = smgr->addBillboardSceneNode(DUMMYLIGHT, core::dimension2d<f32>(1.0,1.0));  // Attach billboard to light.
	DUMMYBILL->setMaterialFlag(video::EMF_LIGHTING, false);                           // Bill Node is affected by LIGHT?
	DUMMYBILL->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);                 // Transparent texture.
	DUMMYBILL->setMaterialTexture(0, driver->getTexture("light.png"));
	// DUMMYBILL->setVisible(false);
    };





/*
	scene::IVolumeLightSceneNode * n = smgr->addVolumeLightSceneNode(0, -1,
				32,                              // Subdivisions on U axis
				32,                              // Subdivisions on V axis
				video::SColor(0, 255, 255, 255), // foot color
				video::SColor(0, 0, 0, 0));      // tail color

	if (n)
	{
		n->setScale(core::vector3df(2.0f, 2.0f, 2.0f));
		n->setPosition(core::vector3df(-5,0,0));

		// load textures for animation
		core::array<video::ITexture*> textures;
		for (s32 g=7; g > 0; --g)
		{
			core::stringc tmp;
			tmp = "../../../media/portal";
			tmp += g;
			tmp += ".bmp";
			video::ITexture* t = driver->getTexture( tmp.c_str() );
			textures.push_back(t);
		}

		// create texture animator
		scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(textures, 100);

		// add the animator
		n->addAnimator(glow);

		// drop the animator because it was created with a create() function
		glow->drop();
	}

*/














    /// ##########################################################################################
    /// ############# LOAD NODES AND GENERATES PHYSICS ###########################################
    /// ##########################################################################################


    /// FROM WIKIPEDIA -> http://www.irrlicht3d.org/wiki/index.php?n=Main.B3d
    /// FROM WIKIPEDIA -> Maya / 3DS Max to .b3d Export your model from Maya or 3DSMax to FBX format (2010 or lower) and import this in FragMotion and export it to Blitz Basic 3D (.b3d)



    /// INITIALIZE PHYSICS WORLD:
    irrBulletWorld* world = createIrrBulletWorld(device, true, true); ///  irr::IrrlichtDevice *const Device, bool useGImpact, bool useDebugDrawer
    world->setDebugMode(EPDM_DrawAabb | EPDM_DrawContactPoints);
    // world->setGravity(vector3df(0, -9.8, 0));
    world->setGravity(vector3df(0, -10, 0));
    world->registerGImpactAlgorithm();
    // world->setDamping(0.01,0.01);


    f32 sm  = 0.02;    /// setMargin road only (base)
    f32 smo = 0.02;    /// setMargin to all other objects
    f32 sr  = 0.01;    /// setRestitution
    vector3df globalScaling = vector3df(1.0,1.0,1.0);







    /// NODE -> TERRAIN:
	// add terrain scene node
	scene::ITerrainSceneNode* terrainNode = smgr->addTerrainSceneNode(
        "terrain_heightmap.png",                  // data: heightmap (256x256)
		0,					                      // parent node
		-1,					                      // node id
		core::vector3df(-233.f, -17.0f, -8.0f),   // position
		core::vector3df(0.0f, 0.0f, 0.0f),		  // rotation
		core::vector3df(1.0f, 0.1f, 2.0f),	      // scale
		video::SColor(255, 255, 255, 255 ),       // vertexColor
		10,					                      // maxLOD (simplify distant meshes)
		scene::ETPS_17,				              // patchSize (9,17,33,65)
		0					                      // smoothFactor
		);

	terrainNode->setMaterialFlag(video::EMF_LIGHTING, true);
	terrainNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
	terrainNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	terrainNode->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	terrainNode->setMaterialTexture(0, driver->getTexture("terrain_textures.png"));
	// terrainNode->setMaterialTexture(1, driver->getTexture("terrain_details.jpg"));
	terrainNode->setMaterialType(video::EMT_DETAIL_MAP);
	terrainNode->scaleTexture(1.0f, 1.0f);
	// terrainNode->setDebugDataVisible(true);


	/// Add to physics world:
    scene::CDynamicMeshBuffer meshBuffer(video::EVT_2TCOORDS, video::EIT_16BIT);
    terrainNode->getMeshBufferForLOD(meshBuffer, 0);
    scene::SMesh terraMesh;
    terraMesh.addMeshBuffer(&meshBuffer);
    ICollisionShape* terraShape = new IBvhTriangleMeshShape(terrainNode, &terraMesh, 0.0); /// Create terrain shape
    world->addRigidBody(terraShape);

	/// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(terrainNode, EFT_NONE, ESM_BOTH); };
	/// if(IRRSHADOWS){ terrainNode->addShadowVolumeSceneNode(); terrainNode->enableCastShadow(); };

    /// Get mesh size:
    vector3df dimension = vector3df(terrainNode->getTransformedBoundingBox().getExtent());
    f32 d1 = dimension.X/2;
    f32 d2 = dimension.Y/2;
    f32 d3 = dimension.Z/2; /// 121.31
    cout << "Road width/2: " << d3 << endl;

    /// VERTICES SELECT
    /// BASED ON -> http://irrlichtirc.g0dsoft.com/kat104/3D/terrain-deformation/index.html
    /*
    int indexcount  = 0;  // For dev info only
    int vertexcount = 0;  // For dev info only
    IMesh* pMesh = terrainNode->getMesh();

    for(s32 i=0; i<pMesh->getMeshBufferCount(); ++i){
        IMeshBuffer* pMeshBuffer = pMesh->getMeshBuffer(i);
        // skip mesh buffers that are not the right type
        // if(pMeshBuffer->getVertexType() != video::EVT_2TCOORDS) continue;
        video::S3DVertex2TCoords* sVertices = (video::S3DVertex2TCoords*)pMeshBuffer->getVertices();
        indexcount  += pMeshBuffer->getIndexCount();  // For dev info only
        vertexcount += pMeshBuffer->getVertexCount(); // For dev info only
        for(s32 i=0; i<50000; ++i){
        sVertices[i].Pos.Y += 100; // Applying higher ground to these vertices
        // sVertices[i].video::SColor(0,200,200,200));
        };
    };
    cout << "Terrain -> index count:  " << indexcount  << endl;  // For dev info only
    cout << "Terrain -> vertex count: " << vertexcount << endl;  // For dev info only

    /// IMPORTANT: forces terrain render buffer to reload:
    terrainNode->setPosition(terrainNode->getPosition());
    */







    /// NODE -> SITE:
    irr::scene::IAnimatedMeshSceneNode* siteNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("site.b3d"));
    siteNode->setName("siteNode");
    siteNode->setPosition(vector3df(0, -2.7, 465));
	siteNode->setMaterialFlag(video::EMF_LIGHTING, true);
	siteNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
	siteNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	siteNode->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	/// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(siteNode, EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ siteNode->addShadowVolumeSceneNode(); };

    /// NODE -> NAU:
    /*
    irr::scene::IAnimatedMeshSceneNode* nauNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("nau.b3d"));
    nauNode->setPosition(vector3df(-30, -12, 410));
    nauNode->setScale(vector3df(5, 5, 5));
    // nauNode->setRotation(vector3df(8, 0, -23));
	nauNode->setMaterialFlag(video::EMF_LIGHTING, true);
	nauNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
	nauNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	nauNode->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	/// Effects:
	if(XEFFECTS){ effect->addShadowToNode(nauNode, EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ nauNode->addShadowVolumeSceneNode(); };
	*/
	/*
	for(u32 i=110; i<114; ++i){ // Apply new transparent textures
	nauNode->getMaterial(i).setTexture(0, driver->getTexture("cordas.bmp"));               // Apply texture to my specified material
	nauNode->getMaterial(i).MaterialType = irr::video::EMT_TRANSPARENT_REFLECTION_2_LAYER; // Apply material type to my specified material
	// nauNode->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	// nauNode->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;
	};
	*/


	/// NODES LOOP -> MILLS:
	IAnimatedMeshSceneNode* millNode[10];
	IAnimatedMeshSceneNode* millSailNode [10];
	f32 millposX[] = {   -60,  -66,  -35,   -20  };
	f32 millposY[] = {  -5.6, -4.2, -7.8,  -6.5  };
	f32 millposZ[] = {    80,  105,  190,   295  };
	f32 rotSpeed[] = {  -0.3, -0.8, -0.4,  -0.6  };
	ISceneNodeAnimator* anims[10];

    /// NODE -> MILL:
    for(int i=0; i<4; ++i){
    millNode[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("mill.b3d"));
    millNode[i]->setPosition(vector3df(millposX[i],millposY[i],millposZ[i]));
    millNode[i]->setScale(vector3df(1, 1, 1));
    // millNode[i]->setRotation(vector3df(8, 0, -23));
	millNode[i]->setMaterialFlag(video::EMF_LIGHTING, true);
	millNode[i]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
	millNode[i]->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	millNode[i]->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	/// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(millNode[i], EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ millNode[i]->addShadowVolumeSceneNode(); };

    /// NODE -> MILL SAILS:
    millSailNode[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("mill_sails.b3d"));
    millSailNode[i]->setParent(millNode[i]);
    millSailNode[i]->setPosition(millNode[i]->getJointNode("mill_joint")->getPosition());
    millSailNode[i]->setScale(vector3df(1, 1, 1));
    // millSailNode[i]->setRotation(vector3df(8, 0, -23));
	millSailNode[i]->setMaterialFlag(video::EMF_LIGHTING, true);
	millSailNode[i]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
	millSailNode[i]->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	millSailNode[i]->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	millSailNode[i]->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL); // Transparent sail
	/// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(millSailNode[i], EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ millSailNode[i]->addShadowVolumeSceneNode(); };
	/// Animator:
	anims[i] = smgr->createRotationAnimator(vector3df(rotSpeed[i], 0.0, 0.0));
    millSailNode[i]->addAnimator(anims[i]);
    anims[i]->drop();
    };


















	/// NODES LOOP -> BRIDGES:
	IMeshSceneNode* bridgeNode[10];
	ICollisionShape* brshape[10];
	IRigidBody* brbody[10];
	f32 bposY[] = { -13.0, -16.0, -18.8 };
	f32 bposZ[] = {  32.7, 141.5, 424.3 };

	for(int i=0; i<3; ++i){
	bridgeNode[i] = device->getSceneManager()->addMeshSceneNode(smgr->getMesh("bridge.b3d")->getMesh(0));
	bridgeNode[i]->setPosition(vector3df(0, bposY[i], bposZ[i]));
	bridgeNode[i]->setMaterialFlag(video::EMF_LIGHTING, true);
	bridgeNode[i]->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);  // (make the model a little bit bigger) and normalize its normals, because of the scaling, for correct lighting
    bridgeNode[i]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    bridgeNode[i]->setMaterialFlag(video::EMF_BACK_FACE_CULLING, true);
    bridgeNode[i]->setMaterialFlag(video::EMF_FOG_ENABLE, true);
    /// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(bridgeNode[i], EFT_NONE, ESM_BOTH); };
	/// if(IRRSHADOWS){ bridgeNode[i]->addShadowVolumeSceneNode(); bridgeNode[i]->enableCastShadow(); };
    // For the terrain, instead of adding a cube or sphere shape, we are going to add a BvhTriangleMeshShape. This is the standard trimesh shape for static objects.
    // The first parameter is of course the node to control, the second parameter is the collision mesh, incase you want a low-poly collision mesh, and the third parameter is the mass.
	brshape[i] = new IBvhTriangleMeshShape(bridgeNode[i], device->getSceneManager()->getMesh("bridge_raycast.b3d"), 0.0);
	brshape[i]->setMargin(smo);
    // The rigid body will be placed at the origin of the node that the collision shape is controlling, so we do not need to set the position after positioning the node.
	brbody[i] = world->addRigidBody(brshape[i], COL_WORLD_OBJECT, objectCollidesWith); // shape, group, mask
	// brbody[i] = world->addRigidBody(brshape[i]);
	// brshape[i]->setLocalScaling(vector3df(1,bscale[i],bscale[i]), ESP_BOTH);
	brbody[i]->setCollisionFlags(ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    brbody[i]->getAttributes()->addBool("bridge", true);
	};








    /// NODE -> WATER: (river)
    /// addHillPlaneMesh(name, tileSize, tileCount, material, hillHeight, countHills, textureRepeatCount):
    scene::IAnimatedMesh* waterMesh = smgr->addHillPlaneMesh("water",
    core::dimension2d<f32>(25,50),     // tile size
    core::dimension2d<u32>(10,10),     // tile count
    0,0,                               // hill height
    core::dimension2d<f32>(0,0),       // Count hills
    core::dimension2d<f32>(25,50));    // textureRepeatCount

    /// addWaterSurfaceSceneNode(IMesh, waveHeight, waveSpeed, waveLenght, parent node, id):
	scene::ISceneNode* waterNode = smgr->addWaterSurfaceSceneNode(waterMesh->getMesh(0), 0.03f, 3000.0f, 5.0f);
	waterNode->setPosition(core::vector3df(-110,-12,230));

	/// waterNode->setMaterialTexture(0, driver->getTexture("clouds_day_01.png"));
	/// waterNode->setMaterialTexture(1, driver->getTexture("clouds_day_02.png"));
	waterNode->setMaterialTexture(0, driver->getTexture("water.png"));
	waterNode->setMaterialTexture(1, driver->getTexture("water_reflexions.png")); // Brilho solar
	/// waterNode->setMaterialType(video::EMT_DETAIL_MAP);
	waterNode->setMaterialFlag(video::EMF_LIGHTING,  true);
	waterNode->setMaterialType(video::EMT_TRANSPARENT_REFLECTION_2_LAYER);   // Transparent texture.
	/// waterNode->getMaterial(0).MaterialType = video::EMT_REFLECTION_2_LAYER;
	/// waterNode->getMaterial(1).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	waterNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
	waterNode->setMaterialFlag(video::EMF_FOG_ENABLE, true);









    /// NODES -> LEAVES: (Transparencies)
    /*
	IAnimatedMeshSceneNode* leavesNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("leaves.b3d"));
	// IMeshSceneNode* leavesNode = device->getSceneManager()->addMeshSceneNode(smgr->getMesh("leaves.b3d")->getMesh(0));
	leavesNode->setPosition(vector3df(0,0,0));

	// leavesNode->setRotation(vector3df(0,190,0));
	leavesNode->setMaterialFlag(video::EMF_LIGHTING,  true);
	leavesNode->setMaterialFlag(video::EMF_WIREFRAME, false);
	/// leavesNode->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
	leavesNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    /// add shadows:
	/// leavesNode->addShadowVolumeSceneNode();
	leavesNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true); // (make the model a little bit bigger) and normalize its normals, because of the scaling, for correct lighting
	leavesNode->setMaterialType(video::EMT_ONETEXTURE_BLEND); /// PACK


	for(u32 i=0; i<leavesNode->getMaterialCount(); ++i){
   	leavesNode->getMaterial(i).setTexture(0, driver->getTexture("road_textures.png")); // Apply texture to my specified material
	leavesNode->getMaterial(i).Lighting = true;
	leavesNode->getMaterial(i).ZBuffer = true; // video::ECFN_NEVER;
	leavesNode->getMaterial(i).ZWriteEnable = true; // false;

	// leavesNode->getMaterial(i).MaterialTypeParam = pack_texureBlendFunc( /// PACK
    //   video::EBF_SRC_ALPHA,
    //   video::EBF_ONE_MINUS_SRC_ALPHA,
    //   video::EMFN_MODULATE_1X,
    //   video::EAS_TEXTURE | video::EAS_VERTEX_COLOR);
    leavesNode->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	};
	*/



















        /*
        // Apply texture: (Floor)
        /// waterNode->getMaterial(0).setTexture(0, driver->getTexture("waterbump.png"));  // Apply texture to my specified material
        /// waterNode->getMaterial(0).setTexture(1, driver->getTexture("waterbump.png")); // Apply texture to my specified material

        // Texture type:
        /// waterNode->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        /// waterNode->setMaterialType(video::EMT_DETAIL_MAP); // OK!!

        ICameraSceneNode* waterCamera[10]={0,0,0,0,0,0,0,0,0,0};

        /// Camera:
        // waterCamera[1] = smgr->addCameraSceneNode(0, core::vector3df(waterNode->getPosition()), core::vector3df(waterNode->getPosition().X,waterNode->getPosition().Y-1,waterNode->getPosition().Z));
        waterCamera[1] = smgr->addCameraSceneNode(0, core::vector3df(waterNode->getPosition().X, waterNode->getPosition().Y+2, waterNode->getPosition().Z),  /// Cam Pos
                                                     core::vector3df(waterNode->getPosition().X, waterNode->getPosition().Y+3, waterNode->getPosition().Z)); /// Cam target

        // waterCamera[1] = smgr->addCameraSceneNodeFPS();
        // waterCamera[1]->setPosition(core::vector3df(0,0,0));
        // waterCamera[1]->setTarget(core::vector3df(0,0,0));
        // waterCamera[1]->setFOV(PI/2.5f); // Default is: PI/2.5f

		video::ITexture* rt = driver->addRenderTargetTexture(core::dimension2d<u32>(256,256), "RTT1");
		waterNode->setMaterialTexture(0, rt); // set material of cube to render target
		// waterNode->getMaterial(0).getTextureMatrix(0).setTextureScale(2,2); // Scale texture
		// waterNode->getMaterial(0).getTextureMatrix(0).setRotationDegrees(core::vector3df(0,180,0)); // Flip texture
		// waterNode->getMaterial(0).getTextureMatrix(0).setTextureTranslate(0.5,0.7); // Range is clipped between 0 and 1 with the default initial value of 0.5.
		*/
















	/// NODES LOOP -> TRUNKS:
	IAnimatedMeshSceneNode* trunkNode[10];
	IGImpactMeshShape* tkshape[10];
	IRigidBody* tkbody[10];
	f32 tposZ[]  = { 5.0, 5.5, 6.2, 9.5, 10. };
	f32 tscale[] = { 1.0, 1.5, 1.3, 1.5, 1.1 };

	for(int i=0; i<5; ++i){
	trunkNode[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("trunk.b3d"));
	trunkNode[i]->setPosition(vector3df(0,-1,tposZ[i]));
	trunkNode[i]->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true); // (make the model a little bit bigger) and normalize its normals, because of the scaling, for correct lighting
    trunkNode[i]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    // For the terrain, instead of adding a cube or sphere shape, we are going to add a BvhTriangleMeshShape. This is the standard trimesh shape for static objects.
    // The first parameter is of course the node to control, the second parameter is the collision mesh, incase you want a low-poly collision mesh, and the third parameter is the mass.
    /// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(trunkNode[i], EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ trunkNode[i]->addShadowVolumeSceneNode(); };
	tkshape[i] = new IGImpactMeshShape(trunkNode[i], device->getSceneManager()->getMesh("trunk.b3d"), 6000*tscale[i]);
	tkshape[i]->setMargin(smo);
    // The rigid body will be placed at the origin of the node that the collision shape is controlling, so we do not need to set the position after positioning the node.
	tkbody[i] = world->addRigidBody(tkshape[i], COL_WORLD_OBJECT, objectCollidesWith); // shape, group, mask
	tkshape[i]->setLocalScaling(vector3df(1,tscale[i],tscale[i]), ESP_BOTH);
	tkbody[i]->setDamping(0.2, 0.2);
    tkbody[i]->setFriction(20.0); // rigidBody->setFriction(7.0f);
    tkbody[i]->setRestitution(sr); // void bouncy
    tkbody[i]->getAttributes()->addBool("trunk", true);
    /// tshape->setLocalScaling(vector3df(1,1,1), ESP_BOTH);
	// When setting a rigid body to a static object, please be sure that you have that object's mass set to 0.0. Otherwise, undesired results will occur.
	// tkbody->setCollisionFlags(ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    tkbody[i]->setLinearFactor(vector3df(0,1,1));  // Allowed positions axis (y,z)
    tkbody[i]->setAngularFactor(vector3df(1,0,0)); // Allowed rotations axis (x)
    /// tkbody->setGravity(vector3df(0, -30, 0));
	};







    /*
    /// NODES LOOP -> BARROTES:
	IAnimatedMeshSceneNode* barroteNode[10];
	IGImpactMeshShape* bkshape[10];
	IRigidBody* barrbody[10];
	f32 bposY[] = {0,0,0,0,0};
	f32 bposZ[] = {200,203,294,297,300};
	f32 bscale[] = {1.0, 1.1, 1.2, 1.0, 1.1};

	for(int i=0; i<5; ++i){
    barroteNode[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("barrote.b3d"));
	barroteNode[i]->setPosition(vector3df(0,bposY[i],bposZ[i]));
    // For the terrain, instead of adding a cube or sphere shape, we are going to add a BvhTriangleMeshShape. This is the standard trimesh shape for static objects.
    // The first parameter is of course the node to control, the second parameter is the collision mesh, incase you want a low-poly collision mesh, and the third parameter is the mass.
	bkshape[i] = new IGImpactMeshShape(barroteNode[i], device->getSceneManager()->getMesh("barrote.b3d"), 500*bscale[i]);
	bkshape[i]->setMargin(smo);
    // The rigid body will be placed at the origin of the node that the collision shape is controlling, so we do not need to set the position after positioning the node.
	barrbody[i] = world->addRigidBody(bkshape[i], COL_WORLD_OBJECT, objectCollidesWith); // shape, group, mask
	bkshape[i]->setLocalScaling(vector3df(1,bscale[i],bscale[i]), ESP_BOTH);
	barrbody[i]->setDamping(0.1, 0.1);
    barrbody[i]->setFriction(10); // rigidBody->setFriction(7.0f);
    barrbody[i]->setRestitution(sr); // void bouncy
    /// tshape->setLocalScaling(vector3df(1,1,1), ESP_BOTH);
	// When setting a rigid body to a static object, please be sure that you have that object's mass set to 0.0. Otherwise, undesired results will occur.
	// barrbody1->setCollisionFlags(ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    barrbody[i]->setLinearFactor(vector3df(0,1,1));  // Allowed positions axis (y,z)
    barrbody[i]->setAngularFactor(vector3df(1,0,0)); // Allowed rotations axis (x)
    /// barrbody1->setGravity(vector3df(0, -30, 0));
	};
	*/



    /// NODES LOOP -> BOXES:
	IAnimatedMeshSceneNode* boxNode[10];
	ICollisionShape* boxshape[10];
	IRigidBody* boxbody[10];
	f32 boxposZ[] = {-4.0,-3.0,0.0,2.0};

	for(int i=0; i<4; ++i){
	boxNode[i] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("box.b3d"));
	boxNode[i]->setPosition(vector3df(0,3,boxposZ[i]));
	boxNode[i]->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true); // (make the model a little bit bigger) and normalize its normals, because of the scaling, for correct lighting
	boxNode[i]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
	/// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(boxNode[i], EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ boxNode[i]->addShadowVolumeSceneNode(); };
	/// IGImpactMeshShape *boxshape = new IGImpactMeshShape(boxNode, device->getSceneManager()->getMesh("box.b3d"), 10); // OK
	boxshape[i] = new IBoxShape(boxNode[i], 90, 0.0); // node, mass, margin
	boxshape[i]->setMargin(smo);
    // The rigid body will be placed at the origin of the node that the collision shape is controlling, so we do not need to set the position after positioning the node.
	boxbody[i] = world->addRigidBody(boxshape[i], COL_WORLD_OBJECT, objectCollidesWith); // shape, group, mask
	boxshape[i]->setLocalScaling(globalScaling, ESP_BOTH);
	boxbody[i]->setDamping(0.2,0.2);
    boxbody[i]->setFriction(10); // rigidBody->setFriction(7.0f);
    boxbody[i]->setRestitution(0); // void bouncy
    boxbody[i]->getAttributes()->addBool("box", true);
    /// tshape->setLocalScaling(vector3df(1,1,1), ESP_BOTH);
	// When setting a rigid body to a static object, please be sure that you have that object's mass set to 0.0. Otherwise, undesired results will occur.
	// boxbody->setCollisionFlags(ECF_STATIC_OBJECT); // EAS_ACTIVE; EAS_SLEEPING; EAS_WANTS_DEACTIVATION; EAS_DISABLE_DEACTIVATION; EAS_DISABLE_SIMULATION;
    boxbody[i]->setLinearFactor(vector3df(1,1,1));  // Allowed positions axis (y,z)
    boxbody[i]->setAngularFactor(vector3df(1,1,1)); // Allowed rotations axis (x)
    /// boxbody->setGravity(vector3df(0, -30, 0));
	};




    /// ##########################################################################################
    /// ############# HERE WE GO: PHYSICS FIGTH .... #############################################
    /// ##########################################################################################


    /// NODES -> VEHICLE:
    irr::scene::IAnimatedMeshSceneNode* vehicleNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("vehicle_truck.b3d")); /// vehicle_traktor.b3d
    vehicleNode->setPosition(vector3df(0,0,0));
    vehicleNode->setCurrentFrame(0);   // ANIMATIONS -> http://irrlicht.sourceforge.net/docu/classirr_1_1scene_1_1_i_animated_mesh_scene_node.html
    vehicleNode->setLoopMode(false);

	vehicleNode->setMaterialTexture(0, driver->getTexture("vehicle_truck.png"));
	/// vehicleNode->setMaterialTexture(1, driver->getTexture("metal_reflex.png")); // Brilho solar
	/// vehicleNode->setMaterialType(video::EMT_REFLECTION_2_LAYER);   // Transparent texture.

    /// materials:
    for(int i=0; i<vehicleNode->getMaterialCount(); ++i){
    vehicleNode->getMaterial(i).Lighting = true;                    // Light?
    vehicleNode->getMaterial(i).Shininess = 20.0f;                  // 20.0 = looks like metal
    vehicleNode->getMaterial(i).SpecularColor.set(255,255,255,255); // affect the colour of the reflected light
    vehicleNode->getMaterial(i).AmbientColor.set(255,255,255,255);  // light related aspects of the material
    vehicleNode->getMaterial(i).DiffuseColor.set(255,255,255,255);  // light related aspects of the material
    vehicleNode->getMaterial(i).EmissiveColor.set(0,0,0,0);         // light related aspects of the material
    // vehicleNode->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
    };
	vehicleNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);  // (make the model a little bit bigger) and normalize its normals, because of the scaling, for correct lighting
    vehicleNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
	/// add shadows:
//	if(XEFFECTS){ effect->addShadowToNode(vehicleNode, EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ vehicleNode->addShadowVolumeSceneNode(); };



    /// Create impacts mesh shapes:
    // IGImpactMeshShape* vshape = new IGImpactMeshShape(vehicleNode, device->getSceneManager()->getMesh("vehicle_raycast.b3d"), 1000.0);
    // irr::scene::IAnimatedMeshSceneNode* vehicleNodeRaycast = smgr->addAnimatedMeshSceneNode(smgr->getMesh("vehicle_raycast.b3d"));
    // vehicleNodeRaycast->setParent(emptyNode); // Set as parent of empty node
    /// IGImpactMeshShape* vshape = new IGImpactMeshShape(emptyNode, vehicleNode->getMesh(), 1000.0); // Mass
    IGImpactMeshShape* vshape = new IGImpactMeshShape(vehicleNode, device->getSceneManager()->getMesh("vehicle_truck_raycast.b3d"), 1000.0); /// vehicle_traktor_raycast.b3d
    // IBvhTriangleMeshShape* vshape = new IBvhTriangleMeshShape(vehicleNode, device->getSceneManager()->getMesh("vehicle_truck_raycast.b3d"), 800.0); /// vehicle_traktor_raycast.b3d
    /// ICollisionShape* vshape = new IBvhTriangleMeshShape(vehicleNode, smgr->getMesh("vehicle_raycast.b3d"), 6000.0);
    // vehicleNodeRaycast->setVisible(false);
    vshape->setMargin(smo);
    // vshape->calculateLocalInertia(800, 0);
    IRigidBody* rigidBody = world->addRigidBody(vshape, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    vshape->setLocalScaling(globalScaling, ESP_BOTH);
    rigidBody->setLinearFactor(vector3df(0,1,1));  // Allowed positions axis (y,z)
    rigidBody->setAngularFactor(vector3df(1,0,0)); // Allowed rotations axis (x)
    // rigidBody->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0, 0, 1)));
    // rigidBody->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0, 0,-1)));
    // rigidBody->setMassProps(500, vector3df(0,-1,0));
    // rigidBody->setAngularFactor(vector3df(0.0, 0.0, 0.0));

    // rigidBody->setName("vehicle");
    // rigidBody->getAttributes()->addBool("collide", true);
    rigidBody->getAttributes()->addBool("vehicle", true);
    // rigidBody->getPointer()->setCenterOfMassTransform(); // center the mass
    // rigidBody->setCcdValues(1000,1000);

    rigidBody->includeNodeOnRemoval(false);
    // When using a raycast vehicle, we don't want this rigid body to deactivate.
    rigidBody->setActivationState(EAS_DISABLE_DEACTIVATION);
    // rigidBody->setDamping(0.000001,0.000001);
    rigidBody->setFriction(20);  // rigidBody->setFriction(7.0f);
    rigidBody->setRestitution(0); // void bouncy
    /// rigidBody->setMassProps(200, vector3df(1,0,0));
    rigidBody->setDamping(0.1,0.1);
    // rigidBody->setGravity(vector3df(0, -30, 0));



    /// NODES -> WHEELS:
    irr::scene::IAnimatedMeshSceneNode* rodaNode[10];
    rodaNode[1] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // right_front_wheel
    rodaNode[2] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // left_front_wheel
    rodaNode[3] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // right_back_wheel01
    rodaNode[4] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // left_back_wheel01


    for(int r=1; r<5; ++r){
    for(int i=0; i<rodaNode[r]->getMaterialCount(); ++i){
    rodaNode[r]->getMaterial(i).Shininess = 4.0f;                   // 20.0 = looks like metal
    rodaNode[r]->getMaterial(i).SpecularColor.set(255,255,255,255); // affect the colour of the reflected light
    rodaNode[r]->getMaterial(i).AmbientColor.set(255,255,255,255);  // light related aspects of the material
    rodaNode[r]->getMaterial(i).DiffuseColor.set(255,255,255,255);  // light related aspects of the material
    rodaNode[r]->getMaterial(i).EmissiveColor.set(0,0,0,0);         // light related aspects of the material
    // rodaNode[r]->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
	rodaNode[r]->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);  // (make the model a little bit bigger) and normalize its normals, because of the scaling, for correct lighting
    rodaNode[r]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    /// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(rodaNode[r], EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ rodaNode[r]->addShadowVolumeSceneNode(); };
    };
    };



/*
btBoxShape * box = new btBoxShape(0.5f,0.5f,0.5f);
btVector3 inertia;
float mass = 10.0f;
box->calculateLocalInertia(mass,inertia);
btRigidBodyConstructionInfo info(10.0f,null,mass,inertia); //motion state would actually be non-null in most real usages
info.m_restitution = 1.3f;
info.m_friction = 1.5f;
btRigidBody * rb = new btRigidBody(info);
*/




    f32 wsm   = 0.07;   /// setMargin
    f32 wsf   = 10.0;   /// setFriction
    f32 dap   = 0.3;    /// setDamping -> Set some damping on the rigid body because the raycast vehicles tend to bounce a lot without a lot of tweaking.
    f32 wmass = 800.0;  /// mass


    // IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	ISphereShape* wShape1 = new ISphereShape(rodaNode[1], wmass, true);
	/// wShape1->calculateLocalInertia(wmass, vector3df(1000,1000,1000));
	IRigidBody *wbody1 = world->addRigidBody(wShape1, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
	wShape1->setMargin(wsm);
	wbody1->setFriction(wsf);
	wbody1->setDamping(dap,dap);
	wbody1->getAttributes()->addBool("wheel", true);
	// wbody1->setActivationState(EAS_DISABLE_DEACTIVATION);

    // IGImpactMeshShape* wShape2 = new IGImpactMeshShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape2 = new IConvexHullShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0);
	ISphereShape* wShape2 = new ISphereShape(rodaNode[2], wmass, true);
	IRigidBody *wbody2 = world->addRigidBody(wShape2, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
	wShape2->setMargin(wsm);
	wbody2->setFriction(wsf);
	wbody2->setDamping(dap,dap);
	wbody2->getAttributes()->addBool("wheel", true);
	// wbody2->setActivationState(EAS_DISABLE_DEACTIVATION);

    // IGImpactMeshShape* wShape3 = new IGImpactMeshShape(rodaNode3, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape2 = new IConvexHullShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0);
	ISphereShape* wShape3 = new ISphereShape(rodaNode[3], wmass, true);
	IRigidBody *wbody3 = world->addRigidBody(wShape3, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
	wShape3->setMargin(wsm);
	wbody3->setFriction(wsf);
	wbody3->setDamping(dap,dap);
	wbody3->getAttributes()->addBool("wheel", true);
	// wbody3->setActivationState(EAS_DISABLE_DEACTIVATION);

    // IGImpactMeshShape* wShape4 = new IGImpactMeshShape(rodaNode4, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape2 = new IConvexHullShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0);
	ISphereShape* wShape4 = new ISphereShape(rodaNode[4], wmass, true);
	IRigidBody *wbody4 = world->addRigidBody(wShape4, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
	wShape4->setMargin(wsm);
	wbody4->setFriction(wsf);
	wbody4->setDamping(dap,dap);
	wbody4->getAttributes()->addBool("wheel", true);
	// wbody4->setActivationState(EAS_DISABLE_DEACTIVATION);






    /// FINISHING: Create vehicle
    IRaycastVehicle* vehicle = world->addRaycastVehicle(rigidBody); // Assuming world is pointer to your irrBullet World
    // world->addRaycastVehicle(wbody1); // Assuming world is pointer to your irrBullet World

    vehicle->getVehicleRaycaster()->setUseFilter(true);
    vehicle->getVehicleRaycaster()->setCollisionFilterGroup(COL_VEHICLE);
    vehicle->getVehicleRaycaster()->setCollisionFilterMask(vehicleRaycastCollidesWith);







    /*
    vehicleNode->getVehicleRaycaster()->setUseFilter(true);
    vehicleNode->setCollisionFilterMask(vehicleCollidesWith);
    vehicleNode->setCollisionFilterGroup(COL_VEHICLE);
    */



    /*
    ICollisionShape* wShape1 = new IBvhTriangleMeshShape(vehicleNode, device->getSceneManager()->getMesh("wheel_big.b3d"), 100.0);
    wShape1->setMargin(smo);
    // IGImpactMeshShape* wShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 100.0); /// vehicle_traktor_raycast.b3d
    // ICollisionShape* wShape1 = new IBoxShape(rodaNode6, 0, false);
    // ICollisionShape* wShape1 = new btCylinderShape(rodaNode6, 0, false);
    // ICollisionShape* wShape1 = new IBoxShape(rodaNode6, 0, 0); /// vehicle_traktor_raycast.b3d
    // ICollisionShape *wShape1 = new ISphereShape(rodaNode1, 100.0, 0.0); // node, mass, margin
	/// IRigidBody *wbody1 = world->addRigidBody(wShape1, 1, 1); /// ICollisionShape *shape, s32 group, s32 mask
	IRigidBody *wbody1 = world->addRigidBody(wShape1);
	wbody1->setGravity(vector3df(0, 0, 0));
    wShape1->setLocalScaling(globalScaling, ESP_BOTH);
    wbody1->setLinearFactor(vector3df(0,1,1));  // Allowed positions axis (y,z)
    wbody1->setAngularFactor(vector3df(1,0,0)); // Allowed rotations axis (x)
    */



    /// WHEELS "attached" to CHASSI, the secret is to use "constrains":


    btTransform frameInA[10]; // Chassi
    btTransform frameInB[10]; // Tyre

    frameInA[0] = btTransform::getIdentity();
    frameInA[0].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("01")->getPosition())); // position
    frameInB[0] = btTransform::getIdentity();
    frameInB[0].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    frameInA[1] = btTransform::getIdentity();
    frameInA[1].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("02")->getPosition())); // position
    frameInB[1] = btTransform::getIdentity();
    frameInB[1].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    frameInA[2] = btTransform::getIdentity();
    frameInA[2].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("03")->getPosition())); // position
    frameInB[2] = btTransform::getIdentity();
    frameInB[2].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    frameInA[3] = btTransform::getIdentity();
    frameInA[3].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("04")->getPosition())); // position
    frameInB[3] = btTransform::getIdentity();
    frameInB[3].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    btRigidBody* bodyA = rigidBody->getPointer(); /// get chassi --->  parentObject->getRigidBody()->getPointer();

    btRigidBody* bodyB[10];
    bodyB[0] = wbody1->getPointer(); /// get wheel  --->  obj->getRigidBody()->getPointer();
    bodyB[1] = wbody2->getPointer(); /// get wheel  --->  obj->getRigidBody()->getPointer();
    bodyB[2] = wbody3->getPointer(); /// get wheel  --->  obj->getRigidBody()->getPointer();
    bodyB[3] = wbody4->getPointer(); /// get wheel  --->  obj->getRigidBody()->getPointer();

    btGeneric6DofSpringConstraint* C1 = new btGeneric6DofSpringConstraint(*bodyA, *bodyB[0], frameInA[0], frameInB[0], true);
    btGeneric6DofSpringConstraint* C2 = new btGeneric6DofSpringConstraint(*bodyA, *bodyB[1], frameInA[1], frameInB[1], true);
    btGeneric6DofSpringConstraint* C3 = new btGeneric6DofSpringConstraint(*bodyA, *bodyB[2], frameInA[2], frameInB[2], true);
    btGeneric6DofSpringConstraint* C4 = new btGeneric6DofSpringConstraint(*bodyA, *bodyB[3], frameInA[3], frameInB[3], true);

    C1->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C1->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit
    // C1->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularUpperLimit
    // C1->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularLowerLimit

    C2->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C2->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit
    // C2->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularUpperLimit
    // C2->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularLowerLimit

    C3->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C3->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit
    // C3->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularUpperLimit
    // C3->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularLowerLimit

    C4->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C4->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit
    // C4->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularUpperLimit
    // C4->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularLowerLimit

    world->getPointer()->addConstraint(C1, true);
    world->getPointer()->addConstraint(C2, true);
    world->getPointer()->addConstraint(C3, true);
    world->getPointer()->addConstraint(C4, true);







    /// Create wheel 1 - info:
    SWheelInfoConstructionInfo wheel1;
    wheel1.chassisConnectionPointCS = vehicleNode->getJointNode("01")->getPosition();
    wheel1.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0);
    wheel1.wheelAxleCS = irr::core::vector3df(0.2, 0.0, 0.0);
    wheel1.suspensionRestLength = 0.3;  // 0.30;
    wheel1.wheelRadius = 8;            // wheel (node) radius (10)
    wheel1.isFrontWheel = true;
    vehicle->addWheel(wheel1);          // Add left front wheel

    /// Create wheel 2 - info:
    SWheelInfoConstructionInfo wheel2;
    wheel2.chassisConnectionPointCS = vehicleNode->getJointNode("02")->getPosition();
    wheel2.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0);
    wheel2.wheelAxleCS = irr::core::vector3df(-0.2, 0.0, 0.0);
    wheel2.suspensionRestLength = 0.3;  // 0.30;
    wheel2.wheelRadius = 8;            // wheel (node) radius (10)
    wheel2.isFrontWheel = true;
    vehicle->addWheel(wheel2);          // Add left front wheel

    /// Create wheel 3 - info:
    SWheelInfoConstructionInfo wheel3;
    wheel3.chassisConnectionPointCS = vehicleNode->getJointNode("03")->getPosition();
    wheel3.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0); // irr::core::vector3df(0.0, -1.0, 0.0);
    wheel3.wheelAxleCS = irr::core::vector3df(0.2, 0.0, 0.0);
    wheel3.suspensionRestLength = 0.3;  // 0.30;
    wheel3.wheelRadius = 8;            // wheel (node) radius (15)
    wheel3.isFrontWheel = false;
    vehicle->addWheel(wheel3);          // Add right back wheel

    /// Create wheel 4 - info:
    SWheelInfoConstructionInfo wheel4;
    wheel4.chassisConnectionPointCS = vehicleNode->getJointNode("04")->getPosition();
    wheel4.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0); // irr::core::vector3df(0.0, -1.0, 0.0);
    wheel4.wheelAxleCS = irr::core::vector3df(-0.2, 0.0, 0.0);
    wheel4.suspensionRestLength = 0.3;  // 0.30;
    wheel4.wheelRadius = 8;            // wheel (node) radius (15)
    wheel4.isFrontWheel = false;
    vehicle->addWheel(wheel4);          // Add left back wheel






    /// Tweak wheel info a bit, tweak here more:
    // SRaycastInfo& info = vehicle->getWheelInfo(i);
    // info.contactNormalWS = irr::core::vector3df(-0.1, 0.0, 0.0);


    for(irr::u32 i=0; i<vehicle->getNumWheels(); i++){
    SWheelInfo& info = vehicle->getWheelInfo(i);
    info.suspensionStiffness     = 0.06f; // Vibração: tipo mola frente/trás (maior número => mais vibração)
    info.wheelDampingRelaxation  = 0.20f;
    info.wheelDampingCompression = 0.20f;
    info.frictionSlip  = 1000;
    info.rollInfluence = 0.1f; // Fluidez no rolar (colisao entre rigid bodies)
    // info.wheelSuspensionForce = 0.1f;
    // info.engineForce = 100;
    vehicle->updateWheelInfo(i);
    // vehicle->updateWheelTransform(i,true);
    };


    rigidBody->setVehicleReference(vehicle);







    /// Set all wheels (nodes) in the right positions:

    rodaNode[1]->setPosition(wheel1.chassisConnectionPointCS);
    rodaNode[2]->setPosition(wheel2.chassisConnectionPointCS);
    rodaNode[3]->setPosition(wheel3.chassisConnectionPointCS);
    rodaNode[4]->setPosition(wheel4.chassisConnectionPointCS);

    /*
    rodaNode1->setPosition(vehicleNode->getJointNode("right_front_wheel")->getAbsolutePosition());
    rodaNode2->setPosition(vehicleNode->getJointNode("left_front_wheel")->getAbsolutePosition());
    rodaNode3->setPosition(vehicleNode->getJointNode("right_back_wheel01")->getAbsolutePosition());
    rodaNode4->setPosition(vehicleNode->getJointNode("left_back_wheel01")->getAbsolutePosition());
    rodaNode5->setPosition(vehicleNode->getJointNode("right_back_wheel02")->getAbsolutePosition());
    rodaNode6->setPosition(vehicleNode->getJointNode("left_back_wheel02")->getAbsolutePosition());
    */



    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ############# HERE WE GO .... ############################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################







    /// NODES -> TRAILER:
    irr::scene::IAnimatedMeshSceneNode* trailerNode = smgr->addAnimatedMeshSceneNode(smgr->getMesh("vehicle_truck_trailer.b3d")); /// vehicle_traktor.b3d
    trailerNode->setPosition(vector3df(0,0,0));
    trailerNode->setCurrentFrame(0);   // ANIMATIONS -> http://irrlicht.sourceforge.net/docu/classirr_1_1scene_1_1_i_animated_mesh_scene_node.html
    trailerNode->setLoopMode(false);

    trailerNode->setMaterialTexture(0, driver->getTexture("vehicle_truck.png"));
	/// trailerNode->setMaterialTexture(1, driver->getTexture("metal_reflex.png")); // Brilho solar
	/// trailerNode->setMaterialType(video::EMT_REFLECTION_2_LAYER);   // Transparent texture.

    /// materials:
    for(int i=0; i<trailerNode->getMaterialCount(); ++i){
    trailerNode->getMaterial(i).Lighting = true;                    // Light?
    trailerNode->getMaterial(i).Shininess = 20.0f;                  // 20.0 = looks like metal
    trailerNode->getMaterial(i).SpecularColor.set(255,255,255,255); // affect the colour of the reflected light
    trailerNode->getMaterial(i).AmbientColor.set(255,255,255,255);  // light related aspects of the material
    trailerNode->getMaterial(i).DiffuseColor.set(255,255,255,255);  // light related aspects of the material
    trailerNode->getMaterial(i).EmissiveColor.set(0,0,0,0);         // light related aspects of the material
    // trailerNode->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
    };
	trailerNode->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);  // (make the model a little bit bigger) and normalize its normals, because of the scaling, for correct lighting
    trailerNode->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    /// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(trailerNode, EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ trailerNode->addShadowVolumeSceneNode(); };

    /// Create impacts mesh shapes:
    /// IGImpactMeshShape* vshape = new IGImpactMeshShape(emptyNode, trailerNode->getMesh(), 1000.0); // Mass
    IGImpactMeshShape* trshape = new IGImpactMeshShape(trailerNode, device->getSceneManager()->getMesh("vehicle_truck_trailer_raycast.b3d"), 500.0); /// vehicle_traktor_raycast.b3d
    trshape->setMargin(smo);
    // trshape->calculateLocalInertia(800, 0);
    IRigidBody* rigidTBody = world->addRigidBody(trshape, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
    trshape->setLocalScaling(globalScaling, ESP_BOTH);
    rigidTBody->setLinearFactor(vector3df(0,1,1));  // Allowed positions axis (y,z)
    rigidTBody->setAngularFactor(vector3df(1,0,0)); // Allowed rotations axis (x)

    rigidTBody->getAttributes()->addBool("trailer", true);
    rigidTBody->includeNodeOnRemoval(false);
    rigidTBody->setFriction(20);  // rigidBody->setFriction(7.0f);
    rigidTBody->setRestitution(0); // void bouncy
    rigidTBody->setDamping(0.1,0.1);




    /// NODES -> WHEELS:
    irr::scene::IAnimatedMeshSceneNode* rodaTNode[10];
    rodaTNode[1] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // right_front_wheel
    rodaTNode[2] = smgr->addAnimatedMeshSceneNode(smgr->getMesh("wheel_big.b3d"));  // left_front_wheel

    for(int r=1; r<3; ++r){
    for(int i=0; i<rodaTNode[r]->getMaterialCount(); ++i){
    rodaTNode[r]->getMaterial(i).Shininess = 4.0f;                   // 20.0 = looks like metal
    rodaTNode[r]->getMaterial(i).SpecularColor.set(255,255,255,255); // affect the colour of the reflected light
    rodaTNode[r]->getMaterial(i).AmbientColor.set(255,255,255,255);  // light related aspects of the material
    rodaTNode[r]->getMaterial(i).DiffuseColor.set(255,255,255,255);  // light related aspects of the material
    rodaTNode[r]->getMaterial(i).EmissiveColor.set(0,0,0,0);         // light related aspects of the material
    // rodaNode[r]->getMaterial(i).MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
	rodaTNode[r]->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);  // (make the model a little bit bigger) and normalize its normals, because of the scaling, for correct lighting
    rodaTNode[r]->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, true); // Increase view distance quality (similar to sharpness)
    /// Effects:
//	if(XEFFECTS){ effect->addShadowToNode(rodaTNode[r], EFT_NONE, ESM_BOTH); };
	if(IRRSHADOWS){ rodaTNode[r]->addShadowVolumeSceneNode(); };
    };
    };


    /// f32 wsm   = 0.07;   /// setMargin
    /// f32 wsf   = 10.0;   /// setFriction
    /// f32 dap   = 0.0;    /// setDamping
    /// f32 wmass = 800.0;  /// mass


    // IGImpactMeshShape* wTShape1 = new IGImpactMeshShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape1 = new IConvexHullShape(rodaNode1, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0); /// IBoxShape* wShape1 = new IBoxShape(rodaNode1, 10.0, true);
	ISphereShape* wTShape1 = new ISphereShape(rodaTNode[1], wmass, true);
	IRigidBody *wtbody1 = world->addRigidBody(wTShape1, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
	wTShape1->setMargin(wsm);
	wtbody1->setFriction(wsf);
	wtbody1->setDamping(dap,dap);
	wtbody1->getAttributes()->addBool("wheel", true);
	// wbody1->setActivationState(EAS_DISABLE_DEACTIVATION);

    // IGImpactMeshShape* wTShape2 = new IGImpactMeshShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 50.0); /// IConvexHullShape* wShape2 = new IConvexHullShape(rodaNode2, device->getSceneManager()->getMesh("wheel_big.b3d"), 10.0);
	ISphereShape* wTShape2 = new ISphereShape(rodaTNode[2], wmass, true);
	IRigidBody *wtbody2 = world->addRigidBody(wTShape2, COL_VEHICLE, vehicleRaycastCollidesWith); // shape, group, mask
	wTShape2->setMargin(wsm);
	wtbody2->setFriction(wsf);
	wtbody2->setDamping(dap,dap);
	wtbody2->getAttributes()->addBool("wheel", true);
	// wbody2->setActivationState(EAS_DISABLE_DEACTIVATION);





    /// FINISHING: Create vehicle2
    IRaycastVehicle* vehicle2 = world->addRaycastVehicle(rigidTBody); // Assuming world is pointer to your irrBullet World
    // world->addRaycastVehicle(wbody1); // Assuming world is pointer to your irrBullet World

    vehicle2->getVehicleRaycaster()->setUseFilter(true);
    vehicle2->getVehicleRaycaster()->setCollisionFilterGroup(COL_VEHICLE);
    vehicle2->getVehicleRaycaster()->setCollisionFilterMask(vehicleRaycastCollidesWith);






    /// WHEELS "attached" to TRAILER, the secret is to use "constrains":

    btTransform frameInAA[10];
    btTransform frameInBB[10];

    frameInAA[0] = btTransform::getIdentity();
    frameInAA[0].setOrigin(irrlichtToBulletVector(trailerNode->getJointNode("05")->getPosition())); // position
    frameInBB[0] = btTransform::getIdentity();
    frameInBB[0].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    frameInAA[1] = btTransform::getIdentity();
    frameInAA[1].setOrigin(irrlichtToBulletVector(trailerNode->getJointNode("06")->getPosition())); // position
    frameInBB[1] = btTransform::getIdentity();
    frameInBB[1].setOrigin(irrlichtToBulletVector(vector3df(0,0,0))); // rotation

    btRigidBody* bodyAA = rigidTBody->getPointer(); /// get chassi --->  parentObject->getRigidBody()->getPointer();

    btRigidBody* bodyBB[10];
    bodyBB[0] = wtbody1->getPointer(); /// get wheel  --->  obj->getRigidBody()->getPointer();
    bodyBB[1] = wtbody2->getPointer(); /// get wheel  --->  obj->getRigidBody()->getPointer();

    btGeneric6DofSpringConstraint* C11 = new btGeneric6DofSpringConstraint(*bodyAA, *bodyBB[0], frameInAA[0], frameInBB[0], true);
    btGeneric6DofSpringConstraint* C22 = new btGeneric6DofSpringConstraint(*bodyAA, *bodyBB[1], frameInAA[1], frameInBB[1], true);

    C11->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C11->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit
    // C1->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularUpperLimit
    // C1->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularLowerLimit

    C22->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0, 0.1,0)));    // linearUpperLimit
    C22->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,-0.1,0)));    // linearLowerLimit
    // C2->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularUpperLimit
    // C2->setAngularLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));      // angularLowerLimit

    world->getPointer()->addConstraint(C11, true);
    world->getPointer()->addConstraint(C22, true);







    /// Create wheel 1 - info:
    SWheelInfoConstructionInfo wheel11;
    wheel11.chassisConnectionPointCS = trailerNode->getJointNode("05")->getPosition();
    wheel11.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0);
    wheel11.wheelAxleCS = irr::core::vector3df(-0.2, 0.0, 0.0);
    wheel11.suspensionRestLength = 0.3;  // 0.30;
    wheel11.wheelRadius = 8;            // wheel (node) radius (10)
    wheel11.isFrontWheel = false;
    vehicle2->addWheel(wheel11);          // Add left front wheel

    /// Create wheel 2 - info:
    SWheelInfoConstructionInfo wheel22;
    wheel22.chassisConnectionPointCS = trailerNode->getJointNode("06")->getPosition();
    wheel22.wheelDirectionCS = irr::core::vector3df(0.0, -0.1, 0.0);
    wheel22.wheelAxleCS = irr::core::vector3df(0.2, 0.0, 0.0);
    wheel22.suspensionRestLength = 0.3;  // 0.30;
    wheel22.wheelRadius = 8;            // wheel (node) radius (10)
    wheel22.isFrontWheel = false;
    vehicle2->addWheel(wheel22);          // Add left front wheel






    /// Tweak wheel info a bit, tweak here more:
    // SRaycastInfo& info = vehicle->getWheelInfo(i);
    // info.contactNormalWS = irr::core::vector3df(-0.1, 0.0, 0.0);


    for(irr::u32 i=0; i<vehicle2->getNumWheels(); i++){
    SWheelInfo& info = vehicle2->getWheelInfo(i);
    info.suspensionStiffness     = 0.06f; // Vibração: tipo mola frente/trás (maior número => mais vibração)
    info.wheelDampingRelaxation  = 0.20f;
    info.wheelDampingCompression = 0.20f;
    info.frictionSlip  = 1000;
    info.rollInfluence = 0.1f; // Fluidez no rolar (colisao entre rigid bodies)
    // info.wheelSuspensionForce = 0.1f;
    // info.engineForce = 100;
    vehicle2->updateWheelInfo(i);
    // vehicle2->updateWheelTransform(i,true);
    };


    rigidTBody->setVehicleReference(vehicle2);




    /// Set all wheels (nodes) in the right positions:

    rodaTNode[1]->setPosition(wheel11.chassisConnectionPointCS);
    rodaTNode[2]->setPosition(wheel22.chassisConnectionPointCS);






    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ############# HERE WE GO .... ############################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################
    /// ##########################################################################################






    /// TRAILER "attached" to VEHICLE, the secret is to use "constrains":

    btTransform frameInAAA[10];
    btTransform frameInBBB[10];

    frameInAAA[0] = btTransform::getIdentity();
    frameInAAA[0].setOrigin(irrlichtToBulletVector(vehicleNode->getJointNode("conn1_trailer")->getPosition()));  /// VEHICLE
    frameInBBB[0] = btTransform::getIdentity();
    frameInBBB[0].setOrigin(irrlichtToBulletVector(trailerNode->getJointNode("conn2_trailer")->getPosition()));  /// TRAILER
    // frameInBBB[0].setOrigin(irrlichtToBulletVector(trailerNode->getJointNode("conn2_trailer")->getPosition()));  /// TRAILER

    btRigidBody* bodyAAA = rigidBody->getPointer();  /// get chassi --->  parentObject->getRigidBody()->getPointer();
    btRigidBody* bodyBBB = rigidTBody->getPointer(); /// get wheel  --->  obj->getRigidBody()->getPointer();

    btGeneric6DofSpringConstraint* C111 = new btGeneric6DofSpringConstraint(*bodyAAA, *bodyBBB, frameInAAA[0], frameInBBB[0], true);
    C111->setLinearUpperLimit(irrlichtToBulletVector(vector3df(0,0,0)));       // linearUpperLimit
    C111->setLinearLowerLimit(irrlichtToBulletVector(vector3df(0,0,0)));       // linearLowerLimit
    C111->setAngularUpperLimit(irrlichtToBulletVector(vector3df(0.5,0,0)));    // AngularUpperLimit ok: 0.5
    C111->setAngularLowerLimit(irrlichtToBulletVector(vector3df(-340,0,0)));   // AngularlowerLimit ok: -340

    world->getPointer()->addConstraint(C111, true);











    /// ##########################################################################################
    /// ############# PARTICLES ##################################################################
    /// ##########################################################################################

    /*
    /// Create particle system: (RAIN)
    /// http://irrlicht.sourceforge.net/docu/classirr_1_1scene_1_1_i_particle_system_scene_node.html
	scene::IParticleSystemSceneNode* ps2 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em2 = ps2->createBoxEmitter(
	core::aabbox3d<f32>(0,0,0,30,20,9),  // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                         // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(0.0f,-0.4f,0.0f),    // initial direction. Y = speed
	200,400,                             // emit rate
	// video::SColor(0,0,0,255),         // darkest color
	video::SColor(0,0,0,0),              // darkest color
	video::SColor(255,255,255,255),      // brightest color
	1000,1000,5,                         // min age, max age, angle
	core::dimension2df(5.0f,5.0f),       // min size
	core::dimension2df(10.0f,10.0f));    // max size

	ps2->setEmitter(em2); // Grabs the emitter
	em2->drop();          // We can drop it here, without deleting it

	// scene::IParticleAffector* paf2 = ps2->createFadeOutParticleAffector(); // Fade out particles
	// ps2->addAffector(paf2); // Same goes for the affector
	// paf2->drop();

	// ps2->setParticlesAreGlobal(true);

	ps2->setPosition(core::vector3df(-600,150,-650)); // x, y, z: profundidade
	// ps2->setScale(core::vector3df(100,100,100));
	ps2->setScale(core::vector3df(2,2,2));
	ps2->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	ps2->setMaterialTexture(0, driver->getTexture("bolinhas.png"));
	// ps2->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps2->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps2->setMaterialFlag(video::EMF_LIGHTING, true);              // Node is affected by LIGHT?
	ps2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	*/



    /// Create particle system: (SMOKE 1)
    /// http://irrlicht.sourceforge.net/docu/classirr_1_1scene_1_1_i_particle_system_scene_node.html
	scene::IParticleSystemSceneNode* ps3 = smgr->addParticleSystemSceneNode(false);

	scene::IParticleEmitter* em3 = ps3->createBoxEmitter(
	core::aabbox3d<f32>(0,0,0,1,1,1),   // emitter size --> aabbox3d  (T minx, T miny, T minz, T maxx, T maxy, T maxz)
                                        // emitter size --> aabbox3d  (T minx, T miny, T minz, largura, altura, espessura)
	core::vector3df(-0.002f,0.002f,-0.002f),  // initial direction. Y = speed
	10,30,                              // emit rate
	// video::SColor(0,0,0,255),        // darkest color
	video::SColor(0,0,0,0),             // darkest color
	video::SColor(255,255,255,255),     // brightest color
	2000,5000,10,                       // min age, max age, angle
	core::dimension2df(0.1f,0.1f),      // min size
	core::dimension2df(0.4f,0.4f));     // max size

	ps3->setEmitter(em3); // Grabs the emitter
	em3->drop();          // We can drop it here, without deleting it

	// ps3->setPosition(core::vector3df(-550,100,-500)); // x, y, z: profundidade
	ps3->setPosition(vehicleNode->getJointNode("smoke01")->getAbsolutePosition());
	// ps3->setScale(core::vector3df(100,100,100));
	ps3->setScale(core::vector3df(1,1,1));
	ps3->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);        // disable z-buffer writes
	ps3->setMaterialTexture(0, driver->getTexture("smoke.png"));
	// ps3->setMaterialType(video::EMT_TRANSPARENT_VERTEX_ALPHA); // Transparent texture.
	ps3->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);   // Transparent texture.
	ps3->setMaterialFlag(video::EMF_LIGHTING, false);             // Node is affected by LIGHT?
	ps3->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

	// scene::IParticleAffector* paf3 = ps3->createFadeOutParticleAffector(); // Fade out particles
	scene::IParticleAffector* paf3 = ps3->createFadeOutParticleAffector(video::SColor(0,0,0,0), 100);
	ps3->addAffector(paf3); // Same goes for the affector
	paf3->drop();

    /// ##########################################################################################
    /// ############# PARTICLES - END ############################################################
    /// ##########################################################################################








    /// ##########################################################################################
    /// ############# VEHICLE LIGHT - START ######################################################
    /// ##########################################################################################

    /// Create vehicle FRONT LIGHT:
    /*
    // Create LIGHT data:
    SLight lightdat2; // SLight lightdat3 = light4->getLightData();
    lightdat2.OuterCone = 200;
    lightdat2.Falloff   = 300;
    lightdat2.CastShadows = true;
    lightdat2.DiffuseColor = SColorf(SColor(0,244,205,122)); // o,r,g,b
    // lightdat2.Direction = vector3df(0,-1,5);
    lightdat2.Type = ELT_SPOT; // ELT_POINT, ELT_SPOT, ELT_DIRECTIONAL

    // Create LIGHT:
    scene::ILightSceneNode* light3 = smgr->addLightSceneNode(vehicleNode, core::vector3df(0,0,0)); // Color values: red, green, blue, and alpha.
    // The parameters to addLightSceneNode are: parent node (as a ISceneNode*, 0 for NULL/root), position (Vector3D), color (Colorf), radius (single), and id (integer).

    light3->setParent(vehicleNode);
    light3->setPosition(core::vector3df(0,8,14));
    light3->setRotation(core::vector3df(160,0,0));
    light3->setLightData(lightdat2); // Add data to light
    light3->setRadius(2);
    light3->enableCastShadow(true);

    // Attach billboard to LIGHT:
	scene::ISceneNode* bill3;
	bill3 = smgr->addBillboardSceneNode(light3, core::dimension2d<f32>(1,1));
	bill3->setMaterialFlag(video::EMF_LIGHTING, false);
	bill3->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	bill3->setMaterialTexture(0, driver->getTexture("star.png"));
	*/


    /// ##########################################################################################
    /// ############# VEHICLE LIGHT - END ########################################################
    /// ##########################################################################################





    /// ##########################################################################################
    /// ############# CAMERAS ####################################################################
    /// ##########################################################################################

    /// Cameras Settings:
    ICameraSceneNode* camera[10]={0,0,0,0,0,0,0,0,0,0};
    int selectedCamera = 3; // Default camera
    f32 CAM_SPEED = 0.001f; // Default: 0.001f;

    /// Camera:
    camera[1] = smgr->addCameraSceneNodeFPS();                 // Auto animated Camera
    camera[1]->setMaterialFlag(video::EMF_LIGHTING, false);    // Node is affected by LIGHT?
    camera[1]->setPosition(core::vector3df(0,7,-10));         // Set Camera Position
    camera[1]->setTarget(vehicleNode->getPosition());          // Set Camera Target Node

    /// Isometric Camera:
    camera[2] = smgr->addCameraSceneNodeFPS();
    camera[2]->setPosition(core::vector3df(0,0,0));
    camera[2]->setTarget(vehicleNode->getPosition());
    camera[2]->setFOV(PI/2.5f); // Default is: PI/2.5f
    // camera[2]->setAspectRatio(1.0f); // Default is: 4.0f / 3.0f
    // Get the scene's width and height
    dimension2d<u32> size = driver->getScreenSize();
    matrix4 clipMatrix2;
    // buildProjectionMatrixOrthoLH(widthOfViewVolume, heightOfViewVolume, zNear, zFar);
    clipMatrix2.buildProjectionMatrixOrthoLH(size.Width*0.016f, size.Height*0.016f, 0.0f, 100.0f);
    camera[2]->setProjectionMatrix(clipMatrix2, true);
    // camera[2]->setViewMatrixAffector(clipMatrix2, true);

    /// Camera:
    camera[3] = smgr->addCameraSceneNodeFPS();                 // Auto animated Camera
    camera[3]->setMaterialFlag(video::EMF_LIGHTING, false);    // Node is affected by LIGHT?
    camera[3]->setPosition(core::vector3df(0,0,0));            // Set Camera Position
    camera[3]->setTarget(vehicleNode->getPosition());          // Set Camera Target Node
    camera[3]->setFOV(PI/2.5f); // Default is: PI/2.5f

    /// Camera:
    camera[4] = smgr->addCameraSceneNodeFPS();                 // Auto animated Camera
    camera[4]->setMaterialFlag(video::EMF_LIGHTING, false);    // Node is affected by LIGHT?
    camera[4]->setPosition(core::vector3df(0,0,0));            // Set Camera Position
    camera[4]->setTarget(vehicleNode->getPosition());          // Set Camera Target Node

    /// Camera:
    // Create a basic fps camera.
	camera[5] = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.05f);
	camera[5]->setPosition(vector3df(300.0f, 100.0f, 00.0f));
	camera[5]->setTarget(vector3df(200.0f, 0.0f, 50.0f));
	camera[5]->setFarValue(500.0f);
	camera[5]->setNearValue(1.0f);

    /// ##########################################################################################
    /// ############# CAMERAS - END ##############################################################
    /// ##########################################################################################





    /// ##########################################################################################
    /// ############# USER INTERFACE (GUI) #######################################################
    /// ##########################################################################################

    /// Initialize GUI stuff:
    IGUISkin* skin  = guienv->getSkin();
    IGUIFont* myfont  = guienv->getFont("myfont.bmp"); // Get Texts Font
    if(myfont){ skin->setFont(myfont); };
    skin->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255, 255, 255, 255)); // Set Texts Font Color

    /// Add GUI texts:
    IGUIStaticText* texts[10]={};
    texts[0] = guienv->addStaticText(L"Texts will display here...", core::rect<s32>(10,150,800,600), false, false); // border? wordwrap?

    /// Add GUI image:
	guienv->addImage(driver->getTexture("logo.png"),position2d<int>(SCREEN_WIDTH-250,5));
	// guienv->addGUIElement("myButton", IGUIButton(core::rect< s32> (0,0,200,50), 0, 101, wchar_t "mybutton", wchar_t "clickhere")  )

	/// ##########################################################################################
    /// ############# USER INTERFACE (GUI) - END #################################################
    /// ##########################################################################################




