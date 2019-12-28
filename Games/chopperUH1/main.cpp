#include <irrlicht.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include "uh1.h"
#include "chopper_control.h"
#ifndef MAIN_CPP_PROPERTIES
#define MAIN_CPP_PROPERTIES
#define IRRLICHT_DIR "/home/smso/downloads/irrlicht/irrlicht-svn/"
#endif
using namespace irr;
IrrlichtDevice* device = 0;
scene::ICameraSceneNode* camera = 0;
scene::ICameraSceneNode* cockpitCamera = 0;
//core::vector3df cameraOffset(0.0f);
core::vector3df cameraOffset(0.0f, -0.3f, 6.2f);
scene::ITriangleSelector* selector = 0;
Uh1SceneNode* uh1 = 0;
ChopperControl* model = 0;
enum
{
        ID_IsNotPickable = 0,
        IDFlag_IsPickable = 1 << 0,
        IDFlag_IsHighlightable = 1 << 1
};
void rotateCameraTowardsNodeAroundYAxis
(
    scene::ICameraSceneNode* camera,
    scene::ISceneNode* node,
    f32 minDist = 1.0f,
    bool rotateCameraBy90Deg = false
)
{
    node->updateAbsolutePosition();
    core::vector3df pos = node->getAbsolutePosition();
    camera->updateAbsolutePosition();
    core::vector3df cameraPos = camera->getAbsolutePosition();
    core::vector3df delta = pos - cameraPos;
    core::vector3df rot = delta.getHorizontalAngle();
    rot.X = 0.0f;
    rot.Z = 0.0f;
    if (rotateCameraBy90Deg)
        rot.Y -= 90.0f;
    camera->setRotation(rot);
    // bring camera nearer to node:
    if (minDist <= 1.0f)
        return;
    if (delta.getLengthSQ() < (minDist * minDist))
        return;
    delta *= 0.5f;
    camera->setPosition(core::vector3df
    (
        cameraPos.X + delta.X,
        cameraPos.Y + delta.Y,
        cameraPos.Z + delta.Z
    ));
}
void print(const core::vector3df& vec)
{
        printf("(%.2f,%.2f,%.2f)\n", vec.X, vec.Y, vec.Z);
}
////////////////////////////////////////////////////////////////////////////////
class EventReceiver: public IEventReceiver
{
public:
virtual bool OnEvent(const SEvent& event)
{
        if (model)
                model->OnEvent(event);
        if
        (
                (event.EventType == EET_KEY_INPUT_EVENT)
                && (event.KeyInput.PressedDown == true)
        )
        {
                if (event.KeyInput.Key == irr::KEY_ESCAPE)
                {
                        device->closeDevice();
                        device->run();
                        return true;
                }
                else if (event.KeyInput.Key == irr::KEY_F1)
                {
                        if (uh1->getBody())
                                rotateCameraTowardsNodeAroundYAxis(camera, uh1->getBody(), 5.0f);
                }
                else if (event.KeyInput.Key == irr::KEY_F2)
                {
                        //if (uh1->getTailRotor())
                                //rotateCameraTowardsNodeAroundYAxis(camera, uh1->getTailRotor(), 5.0f);
                        print(camera->getTarget());
                }
                else if (event.KeyInput.Key == irr::KEY_F3)
                {
                        bool enabled = camera->isInputReceiverEnabled();
                        camera->setInputReceiverEnabled(!enabled);
                        device->getCursorControl()->setVisible(enabled);
                }
                else if (event.KeyInput.Key == irr::KEY_F4)
                {
                        if (device->getSceneManager()->getActiveCamera() == camera)
                        {
                                device->getSceneManager()->setActiveCamera(cockpitCamera);
                                uh1->getBody()->setVisible(false);
                                uh1->getInterior()->setVisible(true);
                        }
                        else
                        {
                                device->getSceneManager()->setActiveCamera(camera);
                                uh1->getBody()->setVisible(true);
                                uh1->getInterior()->setVisible(false);
                                rotateCameraTowardsNodeAroundYAxis(camera, uh1->getBody(), 5.0f);
                        }
                }
                else if (event.KeyInput.Key == irr::KEY_F5)
                {
                        cameraOffset.Z += 0.1f;
                        printf("cameraOffset="); print(cameraOffset);
                }
                else if (event.KeyInput.Key == irr::KEY_F6)
                {
                        cameraOffset.Z -= 0.1f;
                        printf("cameraOffset="); print(cameraOffset);
                }

                else if (event.KeyInput.Key == irr::KEY_F7)
                {
                        cameraOffset.Y += 0.1f;
                        printf("cameraOffset="); print(cameraOffset);
                }
                else if (event.KeyInput.Key == irr::KEY_F8)
                {
                        cameraOffset.Y -= 0.1f;
                        printf("cameraOffset="); print(cameraOffset);
                }

                else if (event.KeyInput.Key == irr::KEY_F9)
                {
                        cameraOffset.X += 0.1f;
                        printf("cameraOffset="); print(cameraOffset);
                }
                else if (event.KeyInput.Key == irr::KEY_F10)
                {
                        cameraOffset.X -= 0.1f;
                        printf("cameraOffset="); print(cameraOffset);
                }
        }
        else
        {
        }

        return false;
}
};
////////////////////////////////////////////////////////////////////////////////
int main()
{
        video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
        // create device
        EventReceiver receiver; //<---------------------------------------------------
        device
        = createDevice(driverType, core::dimension2d<u32>(1024, 768), 32, false, false, false, &receiver);
        if (device == 0)
                return 1; // could not create selected driver.
        video::IVideoDriver* driver = device->getVideoDriver();
        scene::ISceneManager* smgr = device->getSceneManager();
        io::IFileSystem* fs = device->getFileSystem();
        fs->addFileArchive(IRRLICHT_DIR);
        // create skybox
        driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
        smgr->addSkyBoxSceneNode
        (
                driver->getTexture("media/irrlicht2_up.jpg"),
                driver->getTexture("media/irrlicht2_dn.jpg"),
                driver->getTexture("media/irrlicht2_lf.jpg"),
                driver->getTexture("media/irrlicht2_rt.jpg"),
                driver->getTexture("media/irrlicht2_ft.jpg"),
                driver->getTexture("media/irrlicht2_bk.jpg")
        );
        driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);


        // add uh60:
        uh1 = new Uh1SceneNode(0, smgr, -1);
        uh1->getBody()->setScale(core::vector3df(10.0f));
        ////////////////////////////////////////////////////////////////////////////
        // setup model of uh1
        model = new ChopperControl(device, uh1->getRootEmpty());
        model->getRootEmpty()->setPosition(core::vector3df(100.0f, 1000.0f, 0.0f));

    // load building:
    scene::IMeshSceneNode* q3node = 0;
    c8* filename = "media/map-20kdm2.pk3";
    bool fileOk = fs->addZipFileArchive(filename);
    if (!fileOk)
    {
        printf("Error adding file:\n%s\nto the file system\n\n", filename);
        return -1;
    }
    io::path bspPath = "20kdm2.bsp";
    scene::IAnimatedMesh* q3levelmesh = smgr->getMesh(bspPath);
    if (q3levelmesh)
        q3node = smgr->addOctreeSceneNode(q3levelmesh->getMesh(0), 0, IDFlag_IsPickable);
    if (q3node)
        q3node->setPosition(core::vector3df(-1350.0f, -130.0f, -1400.0f));
        // setup selector for collision detection:
        selector = smgr->createOctreeTriangleSelector(q3node->getMesh(), q3node, 128);
        if (selector)
        {
                q3node->setTriangleSelector(selector);

                const core::aabbox3d<f32>& box = uh1->getBoundingBox();
                core::vector3df radius = box.MaxEdge - box.getCenter();

                scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator
                (
                        selector,
                        //uh1->getBody(),
                        model->getRootEmpty(),
                        radius * 10.0f,
                        core::vector3df(0.0f), // gravity was (0,-10,0)
                        radius * 10.0f
                );

                model->getRootEmpty()->addAnimator(anim);

                ((scene::ISceneNodeAnimatorCollisionResponse*)anim)->setCollisionCallback(model);
                anim->drop();
                //selector->drop(); we need this for drawing landing guide!
        }
        else
                printf("selector == 0\n");
        ////////////////////////////////////////////////////////////////////////////
        // add cam:
        f32 camRotateSpeed = 100.0f;
        f32 camMoveSpeed = 0.2f;
        f32 camJumpSpeed = 3.0f;
        camera = smgr->addCameraSceneNodeFPS(0, camRotateSpeed, camMoveSpeed, -1, 0, 0, false, camJumpSpeed);
        camera->setFarValue(40000.0f);
        camera->setPosition(core::vector3df(-20.0f, 1000.0f, 260.0f));
        model->getRootEmpty()->updateAbsolutePosition();
        camera->setTarget(model->getRootEmpty()->getAbsolutePosition());

        // setup cockpitCamera:
        cockpitCamera = smgr->addCameraSceneNode(0);
        cockpitCamera->bindTargetAndRotation(false);
        cockpitCamera->setFarValue(40000.0f);
        cockpitCamera->setNearValue(0.1f);
        smgr->setActiveCamera(camera);
        device->getCursorControl()->setVisible(false);
        // Add a light
        //scene::ILightSceneNode* light = smgr->addLightSceneNode
        smgr->addLightSceneNode
        (
                0,
                core::vector3df(600,1100,400),
                video::SColorf(1.0f,1.0f,1.0f,1.0f),
                1000.0f // radius
        );
        smgr->addLightSceneNode
        (
                0,
                core::vector3df(-600,1100,-400),
                video::SColorf(1.0f,1.0f,1.0f,1.0f),
                1000.0f // radius
        );

        int lastFPS = -1;
        u32 lastTime = device->getTimer()->getTime();
        u32 deltaTime = 0;

        while(device->run())
        if (device->isWindowActive())
        {
                deltaTime = device->getTimer()->getTime() - lastTime; //<---------
                uh1->update(deltaTime);
                model->update();

                if (smgr->getActiveCamera() == cockpitCamera)
                {
            //            model->updateCockpitCamera(cockpitCamera, cameraOffset);
                }

                	driver->beginScene(true, true, video::SColor(255,200,200,200));
                smgr->drawAll();

                model->drawLandingGuide(selector, 0xFFFF0000);

                driver->endScene();

                int fps = driver->getFPS();
                if (lastFPS != fps)
                {
                        core::stringw str = L"Irrlicht Engine [";
                        str += driver->getName();
                        str += "] FPS:";
                        str += fps;
                        device->setWindowCaption(str.c_str());
                        lastFPS = fps;
                }

                lastTime = device->getTimer()->getTime(); //<---------
        }

        if (uh1) { delete uh1; uh1 = 0; }
        if (model) { delete model; model = 0; }
        //device->drop();
        return 0;
}
