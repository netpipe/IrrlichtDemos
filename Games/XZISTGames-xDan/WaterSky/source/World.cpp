
#include "World.h"
#include "Lift.h"
#include "Platform.h"
#include "FuelCan.h"
#include "Generator.h"

World::World(IrrlichtDevice *device, audio::ISoundEngine *sengine)
    : Editor(device)
{
    this->device = device;
    this->sengine = sengine;
    this->dPhysics = this;
    objects = new core::array <Object *>;
    
    guienv->getSkin()->setFont(guienv->getFont("lucida.xml"));
    
    // Load language
    
    xTranslation = new XMLNode;
    
    wchar_t *currentLanguage = L"english"; //Need to read current language from a settings file?
    XMLNode xMainNode = XMLNode::openFileHelper(L"media/xml/lang.xml", L"language");
    
    int translationCount = xMainNode.nChildNode();
    
    for (int i = 0; i < translationCount; i ++)
    {
        *xTranslation = xMainNode.getChildNode(i);
        
        if ( wcscmp(xTranslation->getName(), currentLanguage) == 0)
        {
            break;
        }
    }
    
    // Initialise shaders
    
    shader = new Shader(device);
    
    // Setup sound listener
    cameraSoundAnim = new SoundAnimator(sengine);
    smgr->getActiveCamera()->addAnimator(cameraSoundAnim);
    cameraSoundAnim->setAsListener(true);
    cameraSoundAnim->drop();
    
    device->setEventReceiver(this);
    
    // Setup editor...
    initEditor();
    
    // Init bloom
    bloomEnabled = false;
    
    int rttSize = 512;
    bloomRTT = driver->createRenderTargetTexture(core::dimension2d<s32>(rttSize,rttSize));
    bloomRTT2 = driver->createRenderTargetTexture(core::dimension2d<s32>(rttSize,rttSize));
    
    if (!bloomRTT || !bloomRTT2) printf("Warning: could not create RTTs\n");
    
    bloomMat.Lighting = false;
    bloomMat.TextureLayer[0].Texture = bloomRTT;
    bloomMat.ZBuffer = false;
    bloomMat.ZWriteEnable = false;
    bloomMat.MaterialType = (video::E_MATERIAL_TYPE)SHADER_MATERIAL_BLOOM;
    
	bloomIndices[0] = 0;
    bloomIndices[1] = 1;
	bloomIndices[2] = 2;
	bloomIndices[3] = 2;
	bloomIndices[4] = 3;
	bloomIndices[5] = 0;
	bloomVertices[0].TCoords.set(0.0f, 0.0f); // To invert texture would toggle Vs.
	bloomVertices[1].TCoords.set(0.0f, 1.0f);
	bloomVertices[2].TCoords.set(1.0f, 1.0f);
	bloomVertices[3].TCoords.set(1.0f, 0.0f);
	bloomVertices[0].Pos = core::vector3df(-1.0,-1.0,0.0);
	bloomVertices[1].Pos = core::vector3df(-1.0,1.0,0.0);
	bloomVertices[2].Pos = core::vector3df(1.0,1.0,0.0);
	bloomVertices[3].Pos = core::vector3df(1.0,-1.0,0.0);
	
	// Add sky dome
	//smgr->addSkyDomeSceneNode(driver->getTexture("Sky_Murky_2a.jpg"), 12,12, 1.0, 1.6);
	//smgr->addSkyDomeSceneNode(driver->getTexture("test.jpg"), 12,12, 1.0, 2.0);
	
	core::stringc skyDir = "sky/Set 40/";
	scene::ISceneNode *skyBox = smgr->addSkyBoxSceneNode(
	   driver->getTexture( (skyDir+"6.png").c_str() ),
	   driver->getTexture( (skyDir+"5.png").c_str() ),
	   driver->getTexture( (skyDir+"1.png").c_str() ),
	   driver->getTexture( (skyDir+"3.png").c_str() ),
	   driver->getTexture( (skyDir+"2.png").c_str() ),
	   driver->getTexture( (skyDir+"4.png").c_str() )
	   );
	for (u32 i = 0; i < skyBox->getMaterialCount(); i ++)
	{
        skyBox->getMaterial(i).TextureLayer[0].TextureWrap = video::ETC_CLAMP_TO_EDGE;
    }
	
	// Weather!
	
	weather = new Weather(this);
	
	
	weather->rainSetup(
        40.0, // height above camera
        6.0,
        100.01, // area
        core::vector3df(0.005,-0.01,0), // vel
        200, // per second
        8000 // life time
        );
	weather->setRaining(true);
	
}

World::~World()
{
    if (bloomRTT) bloomRTT->drop();
    if (bloomRTT2) bloomRTT2->drop();
    
    delete weather;
    delete xTranslation;
    shader->drop();
    delete thirdPersonCamera;
    
    // Remove sound listener
    smgr->getActiveCamera()->removeAnimator(cameraSoundAnim);
    
    clearObjects();
    
    delete objects;
}

void World::update(float dt)
{
    if (editing) dt = 0;
    
    // physics stepping
    updatePhysics(dt);
    
    f32 cameraAngle = (smgr->getActiveCamera()->getTarget() - smgr->getActiveCamera()->getPosition())
        .getHorizontalAngle().Y*0.017453;
    
    // Player movement
    
    static const f32 playerMoveForce = 1.0;
    
    core::vector3df force;
    
    // calculate force based on key input
    // also rotate player to face movement direction
    if (playerMovement.forward)
    {
        force += -core::vector3df(-cos(cameraAngle), 0, sin(cameraAngle));
        player->rotateTo(cameraAngle+M_PI);
    }
    
    if (playerMovement.back)
    {
        force += -core::vector3df(cos(cameraAngle), 0, -sin(cameraAngle));
        player->rotateTo(cameraAngle);
    }
    
    if (playerMovement.left)
    {
        force += core::vector3df(sin(cameraAngle), 0, cos(cameraAngle));
        player->rotateTo(cameraAngle+M_PI/2.0);
    }
    
    if (playerMovement.right)
    {
        force += -core::vector3df(sin(cameraAngle), 0, cos(cameraAngle));
        player->rotateTo(cameraAngle-M_PI/2.0);
    }
    
    force.normalize();
    force *= playerMoveForce;
    force.Y = 0;
    player->getPhysicsAnimator()->addTorque(force);
    
    if (playerMovement.moved())
    {
        player->setNextAnimation(player->ANIM_WALK);
    }
    else
    {
        player->setNextAnimation(player->ANIM_IDLE);
    }
    
    // Camera (third person) positioning
    
    if (!editing) thirdPersonCamera->update(dt);
    
    // Object updates
    for (unsigned int i = 0; i < objects->size(); i ++)
    {
        (*objects)[i]->preUpdate(dt);
    }
    for (unsigned int i = 0; i < objects->size(); i ++)
    {
        (*objects)[i]->update(dt);
    }
    
    // Rendering
    
    //video::SColor backCol = video::SColor(0,180,197,255);
    video::SColor backCol = video::SColor(0,0,0,0);
    
    driver->beginScene(true, true, backCol);
    
    if (bloomEnabled && bloomRTT && bloomRTT2 && shader->areAvailable())
    {
        // Render scene to texture
        driver->setRenderTarget(bloomRTT, true,true, backCol);
        smgr->drawAll();
    }
    
    if (bloomEnabled && bloomRTT && bloomRTT2 && shader->areAvailable())
    {
        // Render first texture to another, blurring horizontally
        driver->setRenderTarget(bloomRTT2, true,true, backCol);
        shader->setPass(0);
        bloomMat.TextureLayer[0].Texture = bloomRTT;
        driver->setMaterial(bloomMat);
        driver->drawIndexedTriangleList(bloomVertices,4,bloomIndices,2);
        // Render to screen...
        driver->setRenderTarget(0, true, true, backCol);
    }
    
    smgr->drawAll();
    
    if (bloomEnabled && bloomRTT && bloomRTT2 && shader->areAvailable())
    {
        // Draw bloom
        shader->setPass(1);
        bloomMat.TextureLayer[0].Texture = bloomRTT2;
        driver->setMaterial(bloomMat);
        driver->drawIndexedTriangleList(bloomVertices,4,bloomIndices,2);
    }
    
    if (editing) renderRelations(); // Display object relationships when in editor
    guienv->drawAll();
    driver->endScene();
}

bool World::OnEvent(const SEvent &e)
{
    if (editing)
    {
        return editorOnEvent(e);
    }
    else
    {
        //return ingameOnEvent(e);
    }
    
    if (e.EventType == EET_KEY_INPUT_EVENT)
    {
        switch (e.KeyInput.Key)
        {
        case KEY_KEY_E:
            playerMovement.reset();
            startEditor();
            return true;
        case KEY_KEY_W:
            playerMovement.forward = e.KeyInput.PressedDown;
            return true;
        case KEY_KEY_S:
            playerMovement.back = e.KeyInput.PressedDown;
            return true;
        case KEY_KEY_A:
            playerMovement.left = e.KeyInput.PressedDown;
            return true;
        case KEY_KEY_D:
            playerMovement.right = e.KeyInput.PressedDown;
            return true;
        default:
            break;
        }
        if (e.KeyInput.PressedDown)
        {
            switch (e.KeyInput.Key)
            {
            case KEY_ESCAPE:
                device->closeDevice();
                return true;
            
            default:
                return false;
            }
        }
    }
    else if (e.EventType == EET_MOUSE_INPUT_EVENT)
    {
        if (e.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
        {
            return false;
        }
    }
    return false;
}

