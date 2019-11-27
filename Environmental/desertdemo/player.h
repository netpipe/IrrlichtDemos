#ifndef _PLAYER_H_
#define _PLAYER_H_

    struct TimeFire
    {
        u32 flags;
        u32 next;
        u32 delta;
    };

    enum eTimeFireFlag
    {
        FIRED = 1,
    };

    void setTimeFire ( TimeFire *t, u32 delta, u32 flags = 0 );
    void checkTimeFire ( TimeFire *t, u32 listSize, u32 now );
    scene::ISceneNodeAnimatorCollisionResponse* camCollisionResponse( IrrlichtDevice * device );

    #define dropElement(x)	if (x) { x->remove(); x = 0; }

    struct Player : public scene::IAnimationEndCallBack
    {
        Player ()
        : Device(0), WeaponNode(0), StartPositionCurrent(0)
        {
            animation[0] = 0;
            ammo = 18;
            health = 100;
            shield = 100;
            memset(Anim, 0, sizeof(TimeFire)*4);
        }

        virtual void OnAnimationEnd(scene::IAnimatedMeshSceneNode* node){ setAnim ( 0 );};

        void create (	IrrlichtDevice *device,
                        scene::ITriangleSelector *meta
                    )
        {
            setTimeFire ( Anim + 0, 200, FIRED );
            setTimeFire ( Anim + 1, 5000 );

            // load FPS weapon to Camera
            Device = device;

            scene::ISceneManager *smgr = device->getSceneManager ();
            video::IVideoDriver * driver = device->getVideoDriver();

            scene::ICameraSceneNode* camera = 0;

            SKeyMap keyMap[10];
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

            keyMap[8].Action = EKA_JUMP_UP;
            keyMap[8].KeyCode = KEY_KEY_J;

       //     keyMap[9].Action = EKA_CROUCH;
            keyMap[9].KeyCode = KEY_KEY_C;

            camera = smgr->addCameraSceneNodeFPS(0, 100.0f, 0.6f, -1, keyMap, 10, false, 0.6f);
            camera->setFarValue( 25000.f );

            scene::IAnimatedMeshMD2* weaponMesh = (scene::IAnimatedMeshMD2*) smgr->getMesh("data/weapons/gun.md2");
            if ( 0 == weaponMesh )
                return;

            WeaponNode = smgr->addAnimatedMeshSceneNode(
                                weaponMesh,
                                smgr->getActiveCamera(),
                                10,
                                core::vector3df( 0, 0, 0),
                                core::vector3df(-90,-90,90)
                                );
            WeaponNode->setMaterialFlag(video::EMF_LIGHTING, true);
            WeaponNode->setMaterialTexture(0, driver->getTexture( "data/weapons/gun.jpg"));
            WeaponNode->setLoopMode ( false );

            //create a collision auto response animator
            scene::ISceneNodeAnimator* anim =
                smgr->createCollisionResponseAnimator( meta, camera,
                    core::vector3df(30,45,30),
                    core::vector3df ( 0.f, -90.f, 0.f ),
                    core::vector3df(0,40.0f,0),
                    0.0005f
                );

            camera->addAnimator( anim );
            anim->drop();

            if ( meta )
            {
                meta->drop ();
            }

            respawn ();
            setAnim ( "idle" );
        };

        void shutdown ()
        {
            setAnim ( 0 );

            dropElement (WeaponNode);

            if ( Device )
            {
                scene::ICameraSceneNode* camera = Device->getSceneManager()->getActiveCamera();
                dropElement ( camera );
                Device = 0;
            }
        };

        void setAnim ( const c8 *name )
        {
            if ( name )
            {
                snprintf ( animation, 64, "%s", name );
                if ( WeaponNode )
                {
                    WeaponNode->setAnimationEndCallback ( this );
                    WeaponNode->setMD2Animation ( animation );
                }
            }
            else
            {
                animation[0] = 0;
                if ( WeaponNode )
                {
                    WeaponNode->setAnimationEndCallback ( 0 );
                }
            }
        };

        void respawn ()
        {
            scene::ICameraSceneNode* camera = Device->getSceneManager()->getActiveCamera();
            camera->setPosition(core::vector3df(4000.0f, 2000.0f, 6000.0f));
        };

        void setpos ( const core::vector3df &pos, const core::vector3df& rotation )
        {
            scene::ICameraSceneNode* camera = Device->getSceneManager()->getActiveCamera();
            if ( camera )
            {
                camera->setPosition ( pos );
                camera->setRotation ( rotation );
                camera->OnAnimate ( 0 );
            }
        };

        scene::ISceneNodeAnimatorCollisionResponse * cam() { return camCollisionResponse ( Device ); }

        IrrlichtDevice *Device;
        scene::IAnimatedMeshSceneNode* WeaponNode;
        s32 StartPositionCurrent;
        TimeFire Anim[4];
        c8 animation[64];
        c8 buf[64];
        int ammo, health, shield;
    };

#endif
