//bitplane's how to use

// make a 512x512 texture skybox
scene::CRTTSkyBoxSceneNode *sky = new
    CRTTSkyBoxSceneNode(core::dimension2d<s32>(512,512), smgr->getRootSceneNode(),smgr,0);

// make your main scene invisible
gameSceneRoot->setVisible(false);

// render sky, sun, clouds, whatever with default blue bg
backgroundsky->setVisible(true);
sky->renderToSkyBox(core::vector3df(0,0,0));
backgroundsky->setVisible(false);

// render distant landscape with sky
distantTerrainRoot->setVisible(true);
sky->renderToSkyBox(zoneposition, video::SColor(0,0,0,0), true);
distantTerrainRoot->setVisible(false);

// carry on with game
gameSceneRoot->setVisible(true); 
