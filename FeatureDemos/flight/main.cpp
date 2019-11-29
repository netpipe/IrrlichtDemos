
    #include <irrlicht.h>
    #include <iostream>

    #include "simple_flight_control.h"
    #include "trajectory.h"

    using namespace irr;

    #ifndef MAIN_CPP_PROPERTIES
    #define MAIN_CPP_PROPERTIES
    #define IRRLICHT_DIR "./"
    #define AIRPLANE_FILE "../../media/dwarf.x"
    #define AIRPLANE_TEX_FILE "../../media/detailmap3.jpg"
    #endif

    IrrlichtDevice* device = 0;
    scene::ISceneManager* smgr = 0;

    scene::ICameraSceneNode* camera = 0;
    scene::ICameraSceneNode* followCamera = 0;

    scene::ISceneNode* airplane = 0;
    SimpleFlightControl* flightControl = 0;
    Trajectory* trajectory = 0;

    ////////////////////////////////////////////////////////////////////////////////
    // some utility functions:
    void updateFollowCamera

    (

        scene::ICameraSceneNode* camera,

        scene::ISceneNode* node,
        f32 minDist = 250.0f

    )

    {

        camera->updateAbsolutePosition();

        node->updateAbsolutePosition();



        core::vector3df pos = camera->getAbsolutePosition();
        core::vector3df target = node->getAbsolutePosition();



        camera->setTarget(target);


        if (minDist < 10.0f)

            minDist = 10.0f;


        core::vector3df vectorTC = pos - target;

        if (vectorTC.getLengthSQ() < (minDist * minDist))

            return;


        vectorTC.normalize();

        camera->setPosition(target + vectorTC * minDist);
        camera->updateAbsolutePosition();

    }

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

    ////////////////////////////////////////////////////////////////////////////////
    class EventReceiver: public IEventReceiver
    {

    public:
    bool OnEvent(const SEvent& event)
    {
        if (flightControl)
            flightControl->OnEvent(event);

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
                if (airplane)
                {
                    scene::ICameraSceneNode* cam = smgr->getActiveCamera();
                    rotateCameraTowardsNodeAroundYAxis(cam, airplane, 5.0f);
                }
            }
            else if (event.KeyInput.Key == irr::KEY_F2)
            {
            }
            else if (event.KeyInput.Key == irr::KEY_F3)
            {
                bool enabled = camera->isInputReceiverEnabled();
                camera->setInputReceiverEnabled(!enabled);
                device->getCursorControl()->setVisible(enabled);
            }
            else if (event.KeyInput.Key == irr::KEY_F4)
            {

            }
            else if (event.KeyInput.Key == irr::KEY_F5)
            {
                if (smgr->getActiveCamera() == followCamera)

                {
                    smgr->setActiveCamera(camera);

                    if (airplane)
                        rotateCameraTowardsNodeAroundYAxis(camera, airplane, 5.0f);

                }
                else

                {
                    smgr->setActiveCamera(followCamera);
                }
            }
            else if (event.KeyInput.Key == irr::KEY_F6)
            {
            }
            else if (event.KeyInput.Key == irr::KEY_F7)
            {
            }
            else if (event.KeyInput.Key == irr::KEY_F8)
            {
            }
            else if (event.KeyInput.Key == irr::KEY_F9)
            {
            }
            else if (event.KeyInput.Key == irr::KEY_F10)
            {
            }
            else if (event.KeyInput.Key == irr::KEY_F11)
            {
            }
            else if (event.KeyInput.Key == irr::KEY_F12)
            {
            }



        }
        else
        {

        }
        return false;
    }

    };


    int main()
    {
        device = createDevice
        (
            video::EDT_OPENGL, core::dimension2d<u32>(1024, 768),
            32,
            false, false, false
        );

        if (device == 0)
            return 1;

        EventReceiver receiver;
        device->setEventReceiver(&receiver);

        video::IVideoDriver* driver = device->getVideoDriver();
        smgr = device->getSceneManager();

        io::IFileSystem* fs = device->getFileSystem();
        fs->addFileArchive(IRRLICHT_DIR);

        // load building:
        scene::IMeshSceneNode* q3node = 0;
        const char* name = "../../media/map-20kdm2.pk3";
        fs->addZipFileArchive(name);


        ////////////////////////////////////////////////////////////////////////////
        // setup airplane:

        // correct airplane mesh or rotate airplane node so that forward is in z-dir
        scene::IAnimatedMesh* mesh = smgr->getMesh(AIRPLANE_FILE);

        // FIXME
        // work with mesh but does NOT work with animated mesh
        //core::matrix4 mat;
        //mat.setRotationDegrees(core::vector3df(0.0f, 180.0f, 0.0f));
        //smgr->getMeshManipulator()->transform(mesh, mat);

        airplane = smgr->addAnimatedMeshSceneNode(mesh);
        for (u32 n=0; n<airplane->getMaterialCount(); ++n)
            airplane->getMaterial(n).setFlag(video::EMF_LIGHTING, false);

        airplane->setRotation(core::vector3df(0.0f, 180.0f, 0.0f));
        f32 scale = 2.5f;
        airplane->setScale(core::vector3df(scale));

        // workaround for pivot not at the center of bbox:
        core::vector3df center = airplane->getTransformedBoundingBox().getCenter();
        airplane->setPosition(center * (-1.0f) * scale);


        io::path texfile = AIRPLANE_TEX_FILE;
        if (texfile.size() != 0)
            airplane->setMaterialTexture(0, driver->getTexture(texfile));

        // setup flight control:
        flightControl = new SimpleFlightControl(smgr, airplane);
        flightControl->getRootEmpty()->setPosition(core::vector3df(0.0f, 1000.0f, 0.0f));

        trajectory =  new Trajectory(device, flightControl->getRootEmpty());

        trajectory->setColor(0x80FF0000);


        ////////////////////////////////////////////////////////////////////////////



        ////////////////////////////////////////////////////////////////////////////
        // setup camera:
        f32 camRotateSpeed = 100.0f;
        f32 camMoveSpeed = 0.5f;
        f32 camJumpSpeed = 3.0f;

        camera = smgr->addCameraSceneNodeFPS(0, camRotateSpeed, camMoveSpeed, -1, 0, 0, false, camJumpSpeed);
        camera->setPosition(core::vector3df(50.0f, 50.0f, -60.0f));
        camera->setTarget(core::vector3df(-70.0f, 30.0f, -60.0f));
        camera->setFarValue(5000.0f);
        camera->setPosition(airplane->getPosition() + core::vector3df(100.0f, 50.0f, 100.0f));
        camera->setTarget(airplane->getPosition());

        followCamera = smgr->addCameraSceneNode();
        followCamera->setFarValue(5000.0f);
        followCamera->bindTargetAndRotation(false);

        smgr->setActiveCamera(followCamera);



        ////////////////////////////////////////////////////////////////////////////
        device->getCursorControl()->setVisible(false);

        // ----------------------------------------------------
        u32 currentTime;

        while (device->run())
        {
            driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH, video::SColor(255,113,113,133));
              //   driver->beginScene(true, true, 0);
            currentTime = device->getTimer()->getTime();

            updateFollowCamera(followCamera, airplane);
            flightControl->update(currentTime);
            trajectory->update();



            smgr->drawAll();
            trajectory->draw();
            driver->endScene();
        }
    //   device->yield();

        if (flightControl) { delete flightControl; flightControl = 0; }
        if (trajectory) { delete trajectory; trajectory = 0; }
        if (device) { device->drop(); }

        return 0;
    }
