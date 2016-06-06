
#include "CurseOfThePharaoh.h"
#include "PortalSceneNode.h"

IrrlichtDevice* device = 0;
scene::IMetaTriangleSelector* metaSelector = 0;
scene::ICameraSceneNode* camera = 0;
PortalSceneNode portal;
//AudioDevicePtr soundDevice;
vector<core::vector3df> camPositions;
vector<core::vector3df> camTargets;
int camLoc = 0;

class EventReceiver : public IEventReceiver {


    public:
        virtual bool OnEvent(SEvent event) {

            // Keyboard Events
            if (event.EventType == irr::EET_KEY_INPUT_EVENT && !event.KeyInput.PressedDown) {
               switch (event.KeyInput.Key) {
                   case KEY_KEY_C: { // Move camera to a new preset position
                       camLoc = (camLoc+1)%camPositions.size();
                       camera->setPosition(camPositions.at(camLoc));
                       camera->setTarget(camTargets.at(camLoc));
                       return true;
                   }
                   case KEY_KEY_P: { // Activate/Deactivate portal
                        if (camera->getPosition().getDistanceFrom(portal.getPosition()) < 250) {
                           portal.togglePortalActive(metaSelector);
                        }
                        return true;
                   }
                   case KEY_ESCAPE: { // Exit
                       exit(0);
                       return true;
                   }
               }
            }

            return false;

        }


};

int main() {

    CurseOfThePharoah cop;
    cop.run();

}

CurseOfThePharoah::CurseOfThePharoah() {}

int CurseOfThePharoah::run() {

    screenDim = dimension2d<int>(800, 600);
    EventReceiver *er;
    device = createDevice(video::EDT_OPENGL, screenDim, 32, false, false, false, 0); //!should be er on the end but i cant get the receiver to work

    if (device == 0)
       return 1;

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    env = device->getGUIEnvironment();
    metaSelector = smgr->createMetaTriangleSelector();
    env->getSkin()->setFont(env->getFont("Images/GillSans12.png"));

    createProgressBar();
    updateProgressBar(0, "Loading...");

    fps = env->addStaticText(L"FPS:", rect<int>(30,30,100,60));
    fps->setVisible(false);
    fps->setOverrideColor(SColor(0,255,255,255));
    clock = env->addStaticText(L"00:00:00", rect<int>(30,60,100,90));
    clock->setVisible(false);
    clock->setOverrideColor(SColor(0,255,255,255));

    updateProgressBar(1*25, "Initialisation Done...");

    device->getFileSystem()->addZipFileArchive("Meshes/kmldm2a5.pk3");
    device->getFileSystem()->addZipFileArchive("Meshes/pak0.pk3");

    updateProgressBar(2*25, "Loading Map...");

    scene::IMesh* mesh = smgr->getMesh("kmldm2a5.bsp")->getMesh(0);
    scene::ISceneNode* node = 0;

    updateProgressBar(3*25, "Loading Map...");

    node = smgr->addOctTreeSceneNode(mesh);

    updateProgressBar(4*25, "Loading Map...");

    node->setPosition(core::vector3df(-1300,-144,-1249));
    metaSelector->addTriangleSelector(smgr->createOctTreeTriangleSelector(mesh, node, 16));

    updateProgressBar(5*25, "Map Loaded...");

    // create water
    IAnimatedMesh* aMesh = smgr->addHillPlaneMesh("myHill",
		core::dimension2d<f32>(120,120),
		core::dimension2d<u32>(5,5), 0, 0,
		core::dimension2d<f32>(0,0),
		core::dimension2d<f32>(5,5));

	ISceneNode* waterNode = smgr->addWaterSurfaceSceneNode(aMesh->getMesh(0), 3.0f, 500.0f, 30.0f);
	waterNode->setPosition(vector3df(40,-50,0));
	//waterNode->setMaterialTexture(0,	driver->getTexture("Images/sandstone.jpg"));
	waterNode->setMaterialTexture(0, driver->getTexture("Images/water.jpg"));
	waterNode->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	metaSelector->addTriangleSelector(smgr->createTriangleSelector(aMesh->getMesh(0), waterNode));

	// create light
	node = smgr->addLightSceneNode(0, vector3df(100,50,50),
		video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 75);
	ISceneNodeAnimator* anim = 0;
	anim = smgr->createFlyCircleAnimator(vector3df(100,50,50),50);
	node->addAnimator(anim);
	anim->drop();

	// Create texture animated fire
	core::array<video::ITexture*> fireTextures;
	for (int i = 1; i <= 8; i++)	{
		char tmp[20];
		sprintf(tmp, "Images/flame%d.jpg", i);
		fireTextures.push_back(driver->getTexture(tmp));
	}
	anim = smgr->createTextureAnimator(fireTextures, 100);

    array<vector3df> fireLocations;
    // create fire 1
	scene::IBillboardSceneNode* bill = 0;
    bill = smgr->addBillboardSceneNode(0, core::dimension2d<f32>(180,180), vector3df(575,250,30));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialTexture(0, driver->getTexture("Images/flame1.jpg"));
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->addAnimator(anim);
	fireLocations.push_back(vector3df(575,250,30));

	// create fire 2
	bill = 0;
    bill = smgr->addBillboardSceneNode(0, dimension2d<f32>(180,180), vector3df(-1050,80,-360));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialTexture(0, driver->getTexture("Images/flame1.jpg"));
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->addAnimator(anim);
	fireLocations.push_back(vector3df(-1050,80,-360));

	// create fire 3
	bill = 0;
    bill = smgr->addBillboardSceneNode(0, dimension2d<f32>(180,180), vector3df(-1550,80,-360));
	bill->setMaterialFlag(EMF_LIGHTING, false);
	bill->setMaterialTexture(0, driver->getTexture("Images/flame1.jpg"));
	bill->setMaterialType(EMT_TRANSPARENT_ADD_COLOR);
	bill->addAnimator(anim);
	fireLocations.push_back(vector3df(-1550,80,-360));
	anim->drop();

	// Column
	mesh = smgr->getMesh("Meshes/column.ms3d")->getMesh(0);
	node = smgr->addMeshSceneNode(mesh);
	node->setMaterialFlag(video::EMF_LIGHTING, false);
	node->setPosition(core::vector3df(-1300,-10,-300));
	node->setScale(core::vector3df(5,5,5));
	node->setRotation(core::vector3df(-90,0,0));

	updateProgressBar(6*25, "Scene Created...");

	// Store camera positions & targets
    camPositions.push_back(core::vector3df(475,150,30));
    camTargets.push_back(core::vector3df(0,100,30));
    camPositions.push_back(core::vector3df(-380,200,30));
    camTargets.push_back(core::vector3df(0,50,30));
    camPositions.push_back(core::vector3df(-1300,100,425));
    camTargets.push_back(core::vector3df(-1300,150,30));
    camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .10f, -1, 0, 0, true);
    camera->setPosition(camPositions.at(camLoc));
    camera->setTarget(camTargets.at(camLoc));
    ISceneNodeAnimatorCollisionResponse* camAnim = smgr->createCollisionResponseAnimator(metaSelector, camera, core::vector3df(30,60,30), core::vector3df(0,-30,0));
    camera->addAnimator(camAnim);

//    soundDevice = AudioDevicePtr(OpenDevice());
//    OutputStreamPtr theme = OutputStreamPtr(OpenSound(soundDevice,"Audio/wind.wav", true));
//    theme->setRepeat(true);
//    theme->setVolume(0.5f);
//    fireSound = OutputStreamPtr(OpenSound(soundDevice,"Audio/fire.wav", true));
//    fireSound->setRepeat(true);
//    fireSound->setVolume(calculateVolume(fireLocations, camera->getPosition())/2);

    portal = PortalSceneNode(device, vector3df(100,150,550), camPositions.at(2), camTargets.at(2));
    metaSelector->addTriangleSelector(portal.getTriangleSelector());

	updateProgressBar(7*25, "Portal Created...");

    device->getCursorControl()->setVisible(false);

    updateProgressBar(8*25, "Loading Complete...");

    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    smgr->addSkyBoxSceneNode(
		driver->getTexture("Images/spaceskybox/space1_up.jpg"),
		driver->getTexture("Images/spaceskybox/space1_dn.jpg"),
		driver->getTexture("Images/spaceskybox/space1_lf.jpg"),
		driver->getTexture("Images/spaceskybox/space1_rt.jpg"),
		driver->getTexture("Images/spaceskybox/space1_ft.jpg"),
		driver->getTexture("Images/spaceskybox/space1_bk.jpg"));
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);

    displayProgressBar(false);
    fps->setVisible(true);
    clock->setVisible(true);
//    theme->play();
//    fireSound->play();

    bool wasFalling = false;

    while(device->run())
        if (device->isWindowActive()) {

           driver->beginScene(true, true, video::SColor(0,200,200,200));
           portal.render(camera);
           smgr->drawAll();
           updateFPS();
           updateClock();
           env->drawAll();
           driver->endScene();

           // Check for portal entry;
           camAnim = portal.checkCameraInside(camera, metaSelector, camAnim);

           // Update fire sound volume
//           fireSound->setVolume(calculateVolume(fireLocations, camera->getPosition())/2);

           //! Play landing sound if needed
           if (camAnim && camAnim->isFalling())
              wasFalling = true;
           else
               if (wasFalling) {
                  if (waterNode->getTransformedBoundingBox().intersectsWithLine(line3d<float>(camera->getPosition(),camera->getPosition()+vector3df(0,-100,0)))) {
             //         OutputStreamPtr sound = OutputStreamPtr(OpenSound(soundDevice,"Audio/landwater.wav", false));
             //         sound->setVolume(0.25f);
             //         sound->play();
                      wasFalling = false;
                  } else {
            //          OutputStreamPtr sound = OutputStreamPtr(OpenSound(soundDevice,"Audio/land.wav", false));
            //          sound->setVolume(0.25f);
             //         sound->play();
                      wasFalling = false;
                  }
               }
device->sleep(1);

               //simpleportal hack for now Unit41
                        if (camera->getPosition().getDistanceFrom(portal.getPosition()) < 200) {
                           portal.togglePortalActive(metaSelector);
                        }
        }

    device->drop();
    return 0;

}

// 3D Sound with Audiere by Omar Emad
vector3df CurseOfThePharoah::getTargetAngle(vector3df v, vector3df r) {
    //v -position, r -target

    vector3df angle;
    float x,y,z;
    x = r.X - v.X;
    y = r.Y - v.Y;
    z = r.Z - v.Z;

    //angle in X-Z plane
    angle.Y = atan2 (x, z);
    angle.Y *= (180 / PI); //converting from rad to degrees

    //just making sure angle is somewhere between 0-360 degrees
    if(angle.Y < 0) angle.Y += 360;
    if(angle.Y >= 360) angle.Y -= 360;

    //angle in Y-Z plane while Z and X axes are already rotated around Y
    float z1 = sqrt(x*x + z*z);

    angle.X = atan2 (z1, y);
    angle.X *= (180 / PI); //converting from rad to degrees

    //just making sure angle is somewhere between 0-360 degrees
    if(angle.X < 0) angle.X += 360;
    if(angle.X >= 360) angle.X -= 360;

    return angle;

}

float CurseOfThePharoah::calculateVolume(array<vector3df> eposs, vector3df lpos) {

      vector3df epos = eposs[0];
      float dist = eposs[0].getDistanceFromSQ(lpos);
      for (int i = 1 ; i < eposs.size() ; i++)
          if (eposs[i].getDistanceFromSQ(lpos) < dist) {
             epos = eposs[i];
             dist = eposs[i].getDistanceFromSQ(lpos);
          }

      return (1/(epos.getDistanceFrom(lpos)/500));

}

int CurseOfThePharoah::calculateBalance(vector3df epos, vector3df lpos) {

    int bal;
    float rotaddy = (int)(getTargetAngle(lpos,epos).Y-camera->getRotation().Y);

    if (rotaddy > 0 & rotaddy < 180)
       bal = 1;
    else
        bal = -1;

    return bal;

}

void CurseOfThePharoah::updateClock() {

    // http://delphi.about.com/cs/adptips2003/a/bltip0403_5.htm

     long timeInMilliSeconds = device->getTimer()->getTime();

     int hrs = timeInMilliSeconds/1000/3600;
     int mins = timeInMilliSeconds/1000/60 - hrs * 60;
     int secs = timeInMilliSeconds/1000 - (hrs * 3600 + mins * 60) ;

     stringw time;
     if (hrs < 10)
        time += "0";
     time += hrs;
     time += ":";
     if (mins < 10)
        time += "0";
     time += mins;
     time += ":";
     if (secs < 10)
        time += "0";
     time += secs;
     clock->setText(time.c_str());

}

void CurseOfThePharoah::updateFPS() {

     stringw fpstxt = "FPS: ";
     fpstxt += driver->getFPS();
     fps->setText(fpstxt.c_str());

}

void CurseOfThePharoah::createProgressBar() {

        progBar = env->addImage(rect<int>((screenDim.Width/2)-100+1, (screenDim.Height/2)-15+1, (screenDim.Width/2)-100+1+1, (screenDim.Height/2)+15-1));
        progBar->setImage(driver->getTexture("Images/progBar.jpg"));
        progBox = env->addImage(rect<int>((screenDim.Width/2)-(256/2), (screenDim.Height/2)-(256/2), (screenDim.Width/2)+(256/2), (screenDim.Height/2)+(256/2)));
        progBox->setImage(driver->getTexture("Images/progBox.png"));
        progBox->setUseAlphaChannel(true);
        progText = env->addStaticText(L"", rect<int>((screenDim.Width/2)-100, (screenDim.Height/2)+15, (screenDim.Width/2)+100, (screenDim.Height/2)+40), false, false);
        progText->setOverrideColor(SColor(0,255,255,255));
        background = env->addImage(driver->getTexture("Images/background.png"), position2d<int>((screenDim.Width/2)-(256/2),0), true);

     }

    void CurseOfThePharoah::displayProgressBar(bool val) {

        progBar->setVisible(val);
        progBox->setVisible(val);
        progText->setVisible(val);
        background->setVisible(val);

    }

    void CurseOfThePharoah::updateProgressBar(int newValue, stringw msg) {

        progBar->setVisible(false);
        progBar = env->addImage(rect<int>((screenDim.Width/2)-100+1,(screenDim.Height/2)-15+1,(screenDim.Width/2)-100+1+newValue,(screenDim.Height/2)+15-1));
        progBar->setImage(driver->getTexture("Images/progBar.jpg"));

        progText->setText(msg.c_str());

        driver->beginScene(true, true, SColor(0, 0, 0, 0));
        env->drawAll();
        driver->endScene();

    }
