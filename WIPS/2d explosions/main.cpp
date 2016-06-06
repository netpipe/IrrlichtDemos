 //animations are royalty free.


   scene::ISceneManager* smgr = device->getSceneManager();

    scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
    bill->setPosition(core::vector3df(0, 0, 3));
    bill->setMaterialTexture(0, driver->getTexture("images/explosion_1.png"));
    bill->setMaterialFlag(video::EMF_LIGHTING, false);
    bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
    bill->setSize(core::dimension2d<f32>(250.0f, 250.0f));
    bill->setScale(core::vector3df(1, 1, 1));
