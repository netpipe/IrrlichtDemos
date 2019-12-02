void addShadow() // add a shadow volume node to the scene
{
   video::IVideoDriver* driver = Device->getVideoDriver();
   scene::ISceneManager* smgr = Device->getSceneManager();
   if (ThePoly<10000)
        {    if (ThePoly>2000)
             {driver->beginScene(true, true, video::SColor(150,50,50,50));
              Device->getGUIEnvironment()->addMessageBox(L"Warning!",
              L"This could take a little time to calculate.\n The display will redraw once the calculation is complete.");
              Device->getGUIEnvironment()->drawAll();
              driver->endScene();
             }
             Model->addShadowVolumeSceneNode(0, (10000+selNode),true);
             smgr->setShadowColor (video::SColor(75, 0, 0, 0));
        } else Device->getGUIEnvironment()->addMessageBox(L"Warning!",
          L"Too much polygons to use that function (May crash the application)\n Please, use a less complex model.");
}

void removeShadow() // Revove the shadow volume node from the scene
{
     ISceneNode * node = Device->getSceneManager()->getSceneNodeFromId(10000+selNode);
     if (node) node->remove();
     node = Device->getSceneManager()->getSceneNodeFromId(10000+selNode);
     if (node) node->remove();
     // For some reason, I have to search 2 times and delete 2 times to correctly remove that node
}

void createLights() // create a basic 3 light setup and remove the shadow casting from 2 of them
{
   scene::ISceneManager* smgr = Device->getSceneManager();
   // Create light in the viewer scene.. 3 Lights setup
   light1 = smgr->addLightSceneNode(0, core::vector3df(50,150,-100), video::SColorf(1.0f,1.0f,1.0f),2000); //main
   light2 = smgr->addLightSceneNode(0, core::vector3df(50,190,100), video::SColorf(1.0f,0.75f,0.85f),1000); // fill
   light3 = smgr->addLightSceneNode(0, core::vector3df(-150,150,100), video::SColorf(0.75f,1.0f,0.75f),1000); // other fill
   light1->setRotation(vector3df(50,0,0));
   light2->setRotation(vector3df(110,0,0));
   light3->setRotation(vector3df(135,-45,0));
   // Shut the shadow casting on the 2 other lights
   video::SLight Light1Data = light1->getLightData(); Light1Data.CastShadows=true;  Light1Data.Type= ELT_SPOT; light1->setLightData(Light1Data);
   video::SLight Light2Data = light2->getLightData(); Light2Data.CastShadows=false; Light2Data.Type= ELT_SPOT; light2->setLightData(Light2Data);
   video::SLight Light3Data = light3->getLightData(); Light3Data.CastShadows=false; Light3Data.Type= ELT_SPOT; light3->setLightData(Light3Data);

}
