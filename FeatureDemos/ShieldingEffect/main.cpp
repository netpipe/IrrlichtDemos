
/** \mainpage Irrlicht Shield Effect

    Creating a shield is easy. \n
    First you have to create a shield manager:
    \code
    CShieldManager *shieldmanager = new CShieldManager(smgr,device->getTimer());
    \endcode
    This manager works like the ISceneManger from irrlicht, just for shields.\n


    To add a shield, first create a scene node that you want to use as a shield e.g.
    \code
    scene::IMeshSceneNode shield_node = smgr->addSphereSceneNode(5);
    \endcode
    Next configure the material of the node:
    \code
    shield_node->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
    shield_node->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
    shield_node->setMaterialTexture(0,driver->getTexture("transparent.png"));
    \endcode
    Disabling backface culling makes the shield visible from inside and outside. \n
    Basically you can use every materialtype, but the impacts are drawn over the original node, so it is
    recomended, that you use an transparent base node. Therefore the materialtype is EMT_TRANSPARENT_ALPHA_CHANNEL and
    the first texture is completly transparent.
    \n
    \n
    In case you want to use rays to let the manager calculate impacts, you have to create a triangle selector for the node:
    \code
    scene::ITriangleSelector *selector = smgr->createTriangleSelector(shield_node->getMesh(),shield_node);
    shield_node->setTriangleSelector(selector);
    selector->drop();
    \endcode
    At last you add the shield:
    \code
    shieldmanager->addShield(shield_node);
    \endcode
    To add impacts you have to call the addImpact/addLocalImpact/addGlobalImpact function.
    To render the shields, call drawAll after the drawAll of the scene manager, otherwise the shields will be overdrawn by the rest of the scenery.

    \n
    \n
    So thats that, have fun! \n
    <a href="http://www.freetimestudio.de">www.freetimestudio.de</a>
    //author: freetimecoder
*/

#include <irrlicht.h>
using namespace irr;

#include "CShieldManager.h"

//What shield type should be displayed
int shield_config = 3;
//The two shield nodes
scene::IMeshSceneNode *shield_node1;
scene::IAnimatedMeshSceneNode *shield_node2;
//Other attributes of the impacts
f32 radius = 1;
u32 duration = 500;
video::SColorf impact_color(1,1,1,1);

video::IVideoDriver *driver;

//!Simple Event receiver to process the space key and change the configuration
class myEventReceiver : public IEventReceiver{
public:
    myEventReceiver(){};
    ~myEventReceiver(){};
    virtual bool OnEvent(const SEvent& event)
    {
        if(event.EventType == EET_KEY_INPUT_EVENT){
            if(!event.KeyInput.PressedDown){
                if(event.KeyInput.Key == KEY_SPACE){
                    //Change the shield textures based on the configuration number
                    shield_config++;
                    shield_config = shield_config%5;

                    switch(shield_config){
                    case 0:
                        impact_color = video::SColorf(0,1,2,1);
                        duration = 500;
                        radius=60;

                        shield_node1->setMaterialTexture(1,driver->getTexture("Data/textures/shield_tex.png"));
                        shield_node1->setMaterialTexture(2,driver->getTexture("Data/textures/gradient.png"));

                        shield_node2->setMaterialTexture(1,driver->getTexture("Data/textures/shield_tex.png"));
                        shield_node2->setMaterialTexture(2,driver->getTexture("Data/textures/gradient.png"));
                        break;
                    case 1:
                        radius=10;
                        impact_color = video::SColorf(1,1,0,1);
                        duration = 1000;

                        shield_node1->setMaterialTexture(1,driver->getTexture("Data/textures/detailmap3.jpg"));
                        shield_node1->setMaterialTexture(2,driver->getTexture("Data/textures/gradient_color.png"));

                        shield_node2->setMaterialTexture(1,driver->getTexture("Data/textures/detailmap3.jpg"));
                        shield_node2->setMaterialTexture(2,driver->getTexture("Data/textures/gradient_color.png"));
                        break;
                    case 2:
                        radius=20;
                        impact_color = video::SColorf(0,1,0.5,1);
                        duration = 450;
                        shield_node1->setMaterialTexture(1,driver->getTexture("Data/textures/water.jpg"));
                        shield_node1->setMaterialTexture(2,driver->getTexture("Data/textures/gradient_blinds.png"));

                        shield_node2->setMaterialTexture(1,driver->getTexture("Data/textures/water.jpg"));
                        shield_node2->setMaterialTexture(2,driver->getTexture("Data/textures/gradient_blinds.png"));
                        break;
                    case 3:
                        radius=10;
                        impact_color = video::SColorf(3,1,0,1);
                        duration = 500;
                        shield_node1->setMaterialTexture(1,driver->getTexture("Data/textures/wall.jpg"));
                        shield_node1->setMaterialTexture(2,driver->getTexture("Data/textures/gradient_two.png"));

                        shield_node2->setMaterialTexture(1,driver->getTexture("Data/textures/wall.jpg"));
                        shield_node2->setMaterialTexture(2,driver->getTexture("Data/textures/gradient_two.png"));
                        break;
                    case 4:
                        radius=5;
                        impact_color = video::SColorf(3,1,0,1);
                        duration = 500;
                        shield_node1->setMaterialTexture(1,driver->getTexture("Data/textures/blank.png"));
                        shield_node1->setMaterialTexture(2,driver->getTexture("Data/textures/blank.png"));

                        shield_node2->setMaterialTexture(1,driver->getTexture("Data/textures/blank.png"));
                        shield_node2->setMaterialTexture(2,driver->getTexture("Data/textures/blank.png"));
                        break;
                    };
                    return true;
                }
            }
        }

        return false;
    }
};

int main(int argc, char** argv)
{
    //Set up device and receiver
    myEventReceiver *receiver = new myEventReceiver();
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(640,480), 32,false,false,false,receiver);

    driver = device->getVideoDriver();
    scene::ISceneManager *smgr = device->getSceneManager();

    //Create the camera
    SKeyMap keyMap[8];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0,100,0.03,-1,keyMap,8);
    camera->setPosition(core::vector3df(0,10,-40));
    camera->setTarget(core::vector3df(10,5,0));

    //Add a light
    smgr->addLightSceneNode(0,core::vector3df(0,20,0),
			video::SColorf(1.0f, 1.0f, 1.0f),
			500.0f);

    //Neccessary for textur coordinates changes of the shield nodes
    scene::IMeshManipulator *manipulator = smgr->getMeshManipulator();

    ///Create the shield manager
    CShieldManager *shieldmanager = new CShieldManager(smgr,device->getTimer());

    //Add the first battle turtle
    scene::IAnimatedMeshSceneNode *battle_turtle1 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("Data/turtle.b3d"));
    battle_turtle1->setMaterialTexture(0,driver->getTexture("Data/turtle1d.jpg"));
    battle_turtle1->setScale(core::vector3df(0.25,0.25,0.25));
    battle_turtle1->setPosition(core::vector3df(0,0,0));
    battle_turtle1->setFrameLoop(0,10);
    battle_turtle1->setAnimationSpeed(15);

    //Add the battle turtle's shield
    shield_node1 = smgr->addSphereSceneNode(1,32,battle_turtle1);
    manipulator->scaleTCoords(shield_node1->getMesh(),core::vector2df(6,6),1);
    shield_node1->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
    shield_node1->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
  //      shield_node1->setMaterialFlag(video::EMF_BLEND_OPERATION,true);
    shield_node1->setMaterialTexture(0,driver->getTexture("Data/textures/transparent.png"));
    shield_node1->setScale(core::vector3df(20,20,20));

    scene::ITriangleSelector *selector = smgr->createTriangleSelector(shield_node1->getMesh(),shield_node1);
    shield_node1->setTriangleSelector(selector);
    selector->drop();

    shieldmanager->addShield(shield_node1);


    //Add the second battle turtle
    scene::IAnimatedMeshSceneNode *battle_turtle2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("Data/turtle.b3d"));
    battle_turtle2->setMaterialTexture(0,driver->getTexture("Data/turtle1b.jpg"));
    battle_turtle2->setScale(core::vector3df(0.25,0.25,0.25));
    battle_turtle2->setPosition(core::vector3df(10,0,0));
    battle_turtle2->setRotation(core::vector3df(0,180,0));
    battle_turtle2->setFrameLoop(0,10);
    battle_turtle2->setAnimationSpeed(15);

    //Add the battle turtle's shield
    shield_node2 = smgr->addAnimatedMeshSceneNode(smgr->getMesh("Data/turtle2.b3d"),battle_turtle2);
    shield_node2->setFrameLoop(0,10);
    shield_node2->setAnimationSpeed(15);

    manipulator->scaleTCoords(shield_node2->getMesh(),core::vector2df(6,6),1);
    shield_node2->setMaterialFlag(video::EMF_BACK_FACE_CULLING,false);
    shield_node2->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
  //  shield_node2->setMaterialFlag(video::EMF_BLEND_OPERATION,true);
    shield_node2->setMaterialTexture(0,driver->getTexture("Data/textures/transparent.png"));
    shield_node2->setScale(core::vector3df(1.1,1.1,1.1));

    selector = smgr->createTriangleSelector(shield_node2->getMesh(),shield_node2);
    shield_node2->setTriangleSelector(selector);
    selector->drop();

    shieldmanager->addShield(shield_node2);

    //To draw the shooting rays
    core::array<core::vector3df> battle_rays_1;
    core::array<u32> battle_rays_time_1;

    core::array<core::vector3df> battle_rays_2;
    core::array<u32> battle_rays_time_2;

    //Little workaround to set up the first shield configuration
    SEvent nEvent;
    nEvent.EventType = EET_KEY_INPUT_EVENT;
    nEvent.KeyInput.Key = KEY_SPACE;
    nEvent.KeyInput.PressedDown = false;
    receiver->OnEvent(nEvent);


    //FPS measurement
    int lastFPS = -1;
    u32 lastMS = device->getTimer()->getRealTime();

    while(device->run())
    {

        //Frameindipendence
        u32 difMs = device->getTimer()->getRealTime()-lastMS;
        lastMS = device->getTimer()->getRealTime();
        f32 alignSpeed = difMs/16.0;
        //Move the turtles
        battle_turtle1->setPosition(battle_turtle1->getPosition()+battle_turtle1->getRotation().rotationToDirection()*0.1*alignSpeed);
        battle_turtle1->setRotation(battle_turtle1->getRotation()+core::vector3df(0,0.85,0)*alignSpeed);

        battle_turtle2->setPosition(battle_turtle2->getPosition()+battle_turtle2->getRotation().rotationToDirection()*0.1*alignSpeed);
        battle_turtle2->setRotation(battle_turtle2->getRotation()+core::vector3df(0,-0.85,0)*alignSpeed);


        //Create new rays and impacts randomly
        if(lastMS%(rand()%200+300)==0){
            core::vector3df source = battle_turtle1->getPosition()+core::vector3df(0,5,0);
            core::vector3df target = battle_turtle2->getPosition()+core::vector3df(2-rand()%3,5-rand()%4,2-rand()%3)*0.6;

            const core::line3df ray(source,target);

            core::vector3df result = shieldmanager->addImpact(shield_node2, ray,radius*250,duration,impact_color);
            core::matrix4 trans = shield_node2->getAbsoluteTransformation();
            trans.makeInverse();
            trans.transformVect(result);
            battle_rays_1.push_back(result);
            battle_rays_time_1.push_back(device->getTimer()->getTime()+500);
        }

        if(lastMS%(rand()%200+300)==0){
            core::vector3df source = battle_turtle2->getPosition()+core::vector3df(0,5,0);
            core::vector3df target = battle_turtle1->getPosition()+core::vector3df(4-rand()%9,4-rand()%9,4-rand()%9);

            const core::line3df ray(source,target);
            core::vector3df result = shieldmanager->addImpact(shield_node1, ray,radius,duration,impact_color);
            core::matrix4 trans = shield_node1->getAbsoluteTransformation();
            trans.makeInverse();
            trans.transformVect(result);
            battle_rays_2.push_back(result);
            battle_rays_time_2.push_back(device->getTimer()->getTime()+500);
        }


        //Draw the scene
        driver->beginScene(true, true, video::SColor(50,50,50,50));
        smgr->drawAll();
        ///Draw the shields afterwards, otherwise the regular nodes will overdraw them
        shieldmanager->drawAll();

        //Draw the rays
        video::SMaterial material;
        material.Lighting = false;
        material.Thickness = 1;
        material.MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
        driver->setMaterial(material);
        driver->setTransform(video::ETS_WORLD,core::matrix4());

        for(int i = 0;i<battle_rays_1.size();){
            if(battle_rays_time_1[i]<device->getTimer()->getTime()){
                battle_rays_time_1.erase(i);
                battle_rays_1.erase(i);
            }else{
                u32 time = device->getTimer()->getTime();
                f32 dif = (battle_rays_time_1[i]-time)/500.0f;
                core::vector3df result1 = battle_rays_1[i];
                core::matrix4 trans1 = shield_node2->getAbsoluteTransformation();
                trans1.transformVect(result1);
                driver->draw3DLine(battle_turtle1->getPosition()+core::vector3df(0,5,0),result1,video::SColor(255*dif,0,0,255*dif));
                i++;
            }
        }

        for(int i = 0;i<battle_rays_2.size();){
            if(battle_rays_time_2[i]<device->getTimer()->getTime()){
                battle_rays_time_2.erase(i);
                battle_rays_2.erase(i);
            }else{
                u32 time = device->getTimer()->getTime();
                f32 dif = (battle_rays_time_2[i]-time)/500.0f;
                core::vector3df result2 = battle_rays_2[i];
                core::matrix4 trans2 = shield_node1->getAbsoluteTransformation();
                trans2.transformVect(result2);
                driver->draw3DLine(battle_turtle2->getPosition()+core::vector3df(0,5,0),result2,video::SColor(255*dif,255*dif,0,0));
                i++;
            }
        }

        driver->endScene();


        //Display the FPS
        int fps = driver->getFPS();

        if (lastFPS != fps)
        {
            core::stringw str = L"Shielding - Move with Mouse and WASD/Arrow Keys - Change Shield with Space - FPS:";
            str += fps;

            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }

    }
    //Cleanup
    device->drop();
    delete shieldmanager;

    return 0;
}
