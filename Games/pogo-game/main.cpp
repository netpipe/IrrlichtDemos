
#include <irrlicht.h>
#include <iostream>
#include "include/CPlayer.h"
#include "include/CItem.h"
#include "include/Utils.h"

#include "PostProcessRadialBlur.h"
#include  "shader.h"
#include "include/CCloudSceneNode.h"
//#include <irrKlang.h>
#include <iostream>

//using namespace irrklang;
using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

//Combo variables
bool mouseClicked;
double mouseTimer;
int airCoins;
int jumpCounter;
int comboHolder=0;
int score=10;
float YSpinHolder;
float lastYSpin;



//Devices
IrrlichtDevice *device;
ITimer*timer;

//ISoundEngine* engine;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
gui::IGUIEnvironment* env;


//ISound* music;
//ISoundEffectControl* fx;
btDiscreteDynamicsWorld* dynamicsWorld;
scene::ITriangleSelector* selector;

//Various ndoes
scene::ISceneNode* q3node = 0;
scene::ICameraSceneNode* camera;
scene::ISceneNode* pogoStick;
scene::ISceneNode* springPoint;
scene::ISceneNode* aim;
scene::IAnimatedMeshSceneNode*arms;
scene::CCloudSceneNode* clouds;

gui::IGUIStaticText* comboText;

//shader things
IPostProcessRadialBlur *Blur;
s32 mat;
s32 armmat;
s32 coinShader;

//Game entities
CItem* coins[100];
CPlayer* player;

int timeLeft;
float ltime;
float dt;
core::vector3df dir;
int timeToGo=300000;
int windPower;

class Shader_Bump_callback: public video::IShaderConstantSetCallBack
{
public:

    vector3df fvLightPosition[1]; //the light positions
    SColorf fvLightColor[1];
    SColorf fvAmbient;
    float fSpecularPower;
    float fSpecularStrength;
    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
    {



        core::matrix4 invWorld = services->getVideoDriver()->getTransform(video::ETS_WORLD);
        invWorld.makeInverse();
        services->setPixelShaderConstant("matWorldInverse", (float*)(&invWorld), 16);



        services->setPixelShaderConstant("fvLightPosition1", (float*)(&fvLightPosition[0]), 3);

        services->setPixelShaderConstant("fvAmbient", (float*)(&fvAmbient), 4);
        services->setPixelShaderConstant("fvLight1Color", (float*)(&fvLightColor[0]), 4);

        services->setPixelShaderConstant("fSpecularPower", (float*)(&fSpecularPower), 1);
        services->setPixelShaderConstant("fSpecularStrength", (float*)(&fSpecularStrength), 1);

        int texVar = 0;
        services->setPixelShaderConstant("tex0", (float*)(&texVar), 1);

        texVar = 1;
        services->setPixelShaderConstant("tex1", (float*)(&texVar), 1);
        texVar = 2;
        services->setPixelShaderConstant("tex2", (float*)(&texVar), 1);
        texVar = 3;
        services->setPixelShaderConstant("tex3", (float*)(&texVar), 1);

    }
};

class shaderCallback: public video::IShaderConstantSetCallBack
{
public:
    float sampleDist;
    float sampleStrength;
    virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
    {
        video::IVideoDriver* driver = services->getVideoDriver();
        int texVar = 0;
        services->setPixelShaderConstant("tex0", (float*)(&texVar), 1);

        texVar = 1;
        services->setPixelShaderConstant("tex1", (float*)(&texVar), 1);
        texVar = 2;
        services->setPixelShaderConstant("tex2", (float*)(&texVar), 1);
        texVar = 3;
        services->setPixelShaderConstant("tex3", (float*)(&texVar), 1);
        vector3df lightPosOS;
        SColorf lightColorC;
        vector3df lightColor;
        if (driver->getDynamicLightCount() > 0)
        {
            lightPosOS = driver->getDynamicLight(0).Position;

            lightColorC = driver->getDynamicLight(0).DiffuseColor;
            lightColor.X = lightColorC.r;
            lightColor.Y = lightColorC.g;
            lightColor.Z = lightColorC.b;
        }
        services->setVertexShaderConstant("fvLightPosition", reinterpret_cast<f32*>(&lightPosOS.X), 4);
        services->setPixelShaderConstant("fvDiffuse", reinterpret_cast<f32*>(&lightColor.X), 4);
        vector3df eyePos;
        if (device->getSceneManager()->getActiveCamera())
        {
            eyePos = device->getSceneManager()->getActiveCamera()->getAbsolutePosition();
        }
        services->setVertexShaderConstant("fvEyePosition", reinterpret_cast<f32*>(&eyePos.X), 4);

        core::matrix4 worldmat= driver->getTransform(video::ETS_WORLD);
        services->setVertexShaderConstant("worldMat", worldmat.pointer(), 16);

        worldmat.makeInverse();


        services->setVertexShaderConstant("mInvTWorld", worldmat.pointer(), 16);

    }
};

class MyEventReceiver : public IEventReceiver
{
public:
    // This is the one method that we have to implement
    virtual bool OnEvent(const SEvent& event)
    {
        // Remember whether each key is down or up
        if (event.EventType == irr::EET_KEY_INPUT_EVENT)
            KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
        if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
            if (event.MouseInput.Event==EMIE_LMOUSE_PRESSED_DOWN & !mouseClicked)
            {
                //Set the combo timer, if the click and bounce time are too far apart
                //no boost jump is excuted, time is checked on next bounce
                mouseClicked=true;
                mouseTimer=timer->getTime();
                score-=10;
            }

        return false;
    }

    // This is used to check whether a key is being held down
    virtual bool IsKeyDown(EKEY_CODE keyCode) const
    {
        return KeyIsDown[keyCode];
    }

    MyEventReceiver()
    {
        for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
            KeyIsDown[i] = false;
    }

private:
    // We use this array to store the current state of each key
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

MyEventReceiver Input;

void breakCombo()
{
    score+=comboHolder;
    comboHolder=0;
//    engine->play2D("crash.wav");

}

void shaderSetup()
{
    //Init callbacks and shaders
    video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices();

    shaderCallback* callback2= new shaderCallback;

    Shader_Bump_callback *callback= new Shader_Bump_callback;


    callback->fSpecularStrength=2;
    callback->fSpecularPower=20;

    callback->fvAmbient=SColorf(0.0350,0.022,0.011);

    callback->fvLightColor[0]=SColorf(0.750,0.52,0.31);
    callback->fvLightPosition[0]=vector3df(164.875305, 2468.435059, -1795.774414);



    mat=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
        (
            vertBumpShader.c_str(), "main", video::EVST_VS_1_1,
            fragBumpShader.c_str(), "main", video::EPST_PS_1_1,
            callback, (video::EMT_LIGHTMAP)
        );
    armmat=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
           (
               armvShader.c_str(), "main", video::EVST_VS_1_1,
               armfShader.c_str(), "main", video::EPST_PS_1_1,
               callback2, (video::EMT_SOLID)
           );
    coinShader=(E_MATERIAL_TYPE)gpu->addHighLevelShaderMaterial
               (
                   coinvShader.c_str(), "main", video::EVST_VS_1_1,
                   coinfShader.c_str(), "main", video::EPST_PS_1_1,
                   callback2, (video::EMT_SOLID)
               );
}
void setupPhysics(core::stringc map)
{
    ///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

    ///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
    btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

    ///the maximum size of the collision world. Make sure objects stay within these boundaries
    ///Don't make the world AABB size too large, it will harm simulation quality and performance
    btVector3 worldAabbMin(-10000,-10000,-10000);
    btVector3 worldAabbMax(10000,10000,10000);
    int	maxProxies = 1024;
    btAxisSweep3* overlappingPairCache = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

    dynamicsWorld->setGravity(btVector3(0,-300,0));

    {
        //Convert Irrlicht mesh to bullet mesh
        btCollisionShape* groundShape = new btBvhTriangleMeshShape((btStridingMeshInterface*)ConvertIrrMeshToBulletTriangleMesh(smgr->getMesh(map.c_str()),core::vector3df(1000,1000,1000)),true,true);



        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0,0,0));


        btScalar mass(0.);

        ///rigidbody is dynamic if and only if mass is non zero, otherwise static
        bool isDynamic = (mass != 0.f);

        btVector3 localInertia(0,0,0);
        if (isDynamic)
            groundShape->calculateLocalInertia(mass,localInertia);

        ///using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);
        btRigidBody* body = new btRigidBody(rbInfo);

        ///add the body to the dynamics world
        dynamicsWorld->addRigidBody(body);
    }

}

void sceneSetup()
{
    camera =
        smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, -1, 0, 0, true, 3.f);
    camera->setPosition(core::vector3df(0,700,0));



    //Radial blur effect setup
    Blur = new IPostProcessRadialBlur(smgr->getRootSceneNode(), smgr, 666);
    Blur->initiate(640,480,0.4,0,smgr);
    smgr->loadScene("scene.irr");
    // scene::IMeshSceneNode*node=(scene::IMeshSceneNode*)


    scene::IMesh* q3levelmesh = smgr->getMesh("Buildings.irrmesh");
    q3levelmesh->setHardwareMappingHint(scene::EHM_STATIC);
    if (q3levelmesh)
        q3node = smgr->getSceneNodeFromName("Buildings");
    q3node->setMaterialType((E_MATERIAL_TYPE)mat);
    q3node->setMaterialFlag(video::EMF_BACK_FACE_CULLING,true);


    //Triangle selector for checking if the spring has hit the level
    selector = smgr->createOctTreeTriangleSelector(
                   q3levelmesh, q3node, 128);
    q3node->setTriangleSelector(selector);

    //Load skybox
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
    scene::ISceneNode* skybox=smgr->addSkyBoxSceneNode(
                                  driver->getTexture("irrlicht2_up.jpg"),
                                  driver->getTexture("irrlicht2_dn.jpg"),
                                  driver->getTexture("irrlicht2_lf.jpg"),
                                  driver->getTexture("irrlicht2_rt.jpg"),
                                  driver->getTexture("irrlicht2_ft.jpg"),
                                  driver->getTexture("irrlicht2_bk.jpg"));
    driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);


    //setup HUD
    env->getSkin()->setFont(env->getFont("font.xml"));
    comboText= env->addStaticText(L"",core::rect<s32>(0,4,600,30), true, false);


//    music = engine->play2D("money.mod",true, false, true, ESM_AUTO_DETECT, true);
//    music->setVolume(1);
//    fx = music->getSoundEffectControl();



    smgr->addLightSceneNode(0, core::vector3df(0,0,0),
                            video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 8000.0f);

    // create a cloud node
    clouds= new scene::CCloudSceneNode(
            smgr->getRootSceneNode(), smgr,
                device->getTimer(), 666, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(1,1,1));

    // set the level of details to draw at what distance
    clouds->setLOD(5);

    // set the maximum detail level when recursing
    clouds->setMaxDepth(3);

    // we dont use lighting yet. all the normals face the camera and it looks wierd
    // we could edit the colour of the particles in the list in realtime though!
    clouds->setMaterialFlag(video::EMF_LIGHTING, false);


    clouds->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

    // call a "here's one I made earlier" function
    clouds->makeRandomCloud(150);

    clouds->setLOD(1.0f);
    clouds->setMaxDepth(5);
}

void rigSetup()
{

    //This arrangement of scenodes is the structure of the pogostick
    pogoStick=smgr->addCubeSceneNode(0.00011);

    springPoint=smgr->addCubeSceneNode(2,pogoStick);
    springPoint->setPosition(core::vector3df(0,-150,0));


    camera->setFarValue(10000);
    smgr->getMesh("arms.irrmesh")->setHardwareMappingHint(scene::EHM_STATIC);

    arms=smgr->addAnimatedMeshSceneNode(smgr->getMesh("arms.irrmesh"),camera);
    arms->setPosition(core::vector3df(0,-5,15));
    arms->setRotation(core::vector3df(0,180,0));
    arms->setVisible(false);
    arms->setMaterialTexture(2, driver->getTexture("irrlicht2_lf.jpg"));
    arms->setMaterialType((E_MATERIAL_TYPE)armmat);

    arms->setMaterialTexture(3, Blur->rt0);

    //Create player
    player = new CPlayer(5,smgr,camera,dynamicsWorld,coinShader,windPower);

    //Generate initial coin positions


    for (int i=0;i<100;i++)
    {
        coins[i]=new CItem(i,smgr,coinShader,timer,q3node,selector);
    }

}

void bounceLogic()
{
    //Camera look direction useful for jumping and effects
    dir=core::vector3df(camera->getPosition()-camera->getTarget()).normalize();

    bool reverse=false;
    //If looking down jump backwards
    if (camera->getRotation().X>45&&camera->getRotation().X<90)
    {
        dir*=-1;
        reverse=true;
    }

    pogoStick->setPosition(camera->getPosition());

    //Check if player has collected a coin, if yes respwan it
    for (int i=0;i<100;i++)
    {
        coins[i]->checkHit(score,comboHolder,airCoins,player,reverse);
        if (i==49&jumpCounter>10)
        {
            coins[i]->spwan();
            jumpCounter=0;
        }
    }

    //Bounce Logic
    core::line3d<f32> line;
    line.start = camera->getPosition();
    line.end = springPoint->getAbsolutePosition();

    core::vector3df intersection;
    core::triangle3df tri;

    //Check if mouse has been clicked recently for a boost jump
    if (mouseClicked)
    {
        if (timer->getTime()-mouseTimer>450)
        {
            mouseClicked=false;
            breakCombo();
        }


    }
     scene::ISceneNode* outNode;
    //make sure we jump after a fixed timestep so multiple jumps dont occur over the next few frames
    if (smgr->getSceneCollisionManager()->getCollisionPoint(
                line, selector, intersection, tri,outNode)&&((timer->getTime()-ltime)>500))
    {
        ltime=timer->getTime();

        //boost jump
        if (mouseClicked)
        {
            //apply jump force will overriding previous horizontal velocities
            player->applyForce(core::vector3df(800*-dir.X,660,800*-dir.Z),true);

            comboHolder+=10+(comboHolder*1.5)+(100*airCoins);
        }

        //normaljump
        if (!mouseClicked)
        {
            //apply jump force will overriding previous horizontal velocities
            player->applyForce(core::vector3df(400*-dir.X,350,400*-dir.Z),true);
            if (comboHolder)
            {

                if (comboHolder>10)
                    player->showJumpCombo();
                breakCombo();
            }
        }

        mouseClicked=false;
//        engine->play2D("boing.wav");

        //reset air coin counter
        airCoins=0;
        YSpinHolder=0;
        //progress jump counter, if >10 the special coin respwans
        jumpCounter+=1;
    }

}

void comboLogic()
{
    //update game timer
    timeToGo=300000-(timer->getTime()-timeLeft);

    //compare velcoity with look direction for blur effect
    float dot=dir.dotProduct(player->getVel().normalize());
    if (dot>0)
        dot=0;
    //if comboing change music
    if (comboHolder)
    {
//        fx->enableWavesReverbSoundEffect();
        Blur->callback->sampleStrength=-0.025*player->getVel().Y*dot;
    }
    else
    {
//        fx->disableWavesReverbSoundEffect();
        Blur->callback->sampleStrength=0;
    }
    //lower volume at height point in jump
//    music->setVolume(0.5+(sqrt(player->getVel().Y*player->getVel().Y)*0.001f));
    //break huge combos to prevent score overflows
    if (comboHolder>10000)
    {
        breakCombo();
        player->showJumpCombo();
    }

    //generate combo HUD text
    core::stringw combo=L"Score:";
    combo+=score;
    combo+=L" Combo: ";
    combo+= comboHolder;
    combo+=L" Time: ";
    if (timeToGo>0)
        combo+=timeToGo;
    else
        combo+=L"Time Up!";
    comboText->setText((const wchar_t*)combo.c_str());
}

void drawWorld()
{
    driver->beginScene(true, true, 0);

    driver->setRenderTarget(Blur->rt0, true, true, video::SColor(0,0,0,0));  //First Render The entire scene into the renderTarget
    smgr->drawAll();
    driver->setRenderTarget(0);                                               //set The Rendertarget to the mainview
    Blur->render();

    //exclude arms from the blur
    arms->setVisible(true);
    arms->setPosition(core::vector3df(0,-5,15)+core::vector3df(0,0.01*player->getVel().Y,0));
    arms->OnAnimate(0);
    arms->render();
    arms->setVisible(false);
    env->drawAll();


    driver->endScene();
}

void gameLoop()
{
    timeLeft=timer->getTime();
    ltime=0;
    dt=timer->getTime();

    //while the timer is not 0, run rendering and game logic
    while (device->run()&&timeToGo>0)
        if (device->isWindowActive())
        {

            bounceLogic();

            float dtt=timer->getTime()-dt;
            dynamicsWorld->stepSimulation(dtt/1000.0,10);
            dt=timer->getTime();

            //Camera spin score system
            YSpinHolder=(camera->getRotation().Y-lastYSpin)*dtt/100.0;
            lastYSpin=camera->getRotation().Y;
            score+=sqrt(YSpinHolder*YSpinHolder);

            clouds->setRotation(core::vector3df(0,clouds->getRotation().Y+(0.001*dtt),0));


            player->update(10,timer->getTime(),dtt);
            comboLogic();


            drawWorld();

        }
    //time up
//    engine->play2D("horn.wav");
    //Render only

    while (device->run())
        {
            drawWorld();
            device->yield();
        }

        {
            delete dynamicsWorld;
//            engine->drop();
            selector->drop();
            device->drop();
            exit(0);
        }


}

//This gets called by irrlicht when the intro animation ends
//does the loading and setup
class animCallback: public scene::IAnimationEndCallBack
{
public:

    virtual void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node)
    {
        //remove intro node
        node->remove();

        setupPhysics("Buildings.irrmesh");
        //smgr->loadScene("./data/scene.irr");
        shaderSetup();
        sceneSetup();
        rigSetup();
        gameLoop();


    }
};
void doIntro()
{
    //intro scene setup
    scene::ICameraSceneNode* cam =
        smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, -1, 0, 0, true, 3.f);
    scene::IAnimatedMeshSceneNode* titleNode =smgr->addAnimatedMeshSceneNode(smgr->getMesh("title.b3d"),cam);
    matrix4 projMat;
    projMat.buildProjectionMatrixOrthoLH(650,500,10,1000);
    cam->setProjectionMatrix(projMat,true);
    titleNode->setPosition(core::vector3df(-140.75,40,10));
    titleNode->setScale(core::vector3df(50,60,40));
    titleNode->setAnimationSpeed(22);
    titleNode->setFrameLoop (0, 288);
    titleNode->setLoopMode(false);
    titleNode->setMaterialFlag(video::EMF_LIGHTING, false);
    animCallback* cb=new animCallback();

    device->getCursorControl()->setVisible(false);

    titleNode->setAnimationEndCallback(cb);
//    music = engine->play2D("intro.ogg");
    while (device->run())
    {
        driver->beginScene(true, true, SColor(0,0,0,0));

        smgr->drawAll();

        driver->endScene();
        device->yield();
    }



}
int main()
{
    // let user select driver type



    printf("Welcome to COIN JUMP! Please enter the wind power you want for this session:\n");

    std::cin >> windPower;
    // create device

    device =
        createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640, 480), 32, false,false,false,&Input);
    timer=device->getTimer();
    srand(timer->getRealTime());

    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    env = device->getGUIEnvironment();
    device->getFileSystem()->addZipFileArchive("cdata.zip");
    device->getFileSystem()->addZipFileArchive("map.zip");

//    engine = createIrrKlangDevice();

    //start epic intro sequence
    doIntro();


}

/*
**/
