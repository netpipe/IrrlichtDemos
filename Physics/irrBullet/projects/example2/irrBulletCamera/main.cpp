
    #include <irrlicht.h>
    #include <btBulletDynamicsCommon.h>

    using namespace irr;
    using namespace core;
    using namespace scene;
    using namespace video;
    using namespace io;
    using namespace gui;

    #ifdef _IRR_WINDOWS_
    #pragma comment(lib, "Irrlicht.lib")
    #pragma comment(lib, "BulletDynamics.lib")
    #pragma comment(lib, "BulletSoftBody.lib")
    #pragma comment(lib, "GIMPACTUtils.lib")
    #pragma comment(lib, "LinearMath.lib")
    #pragma comment(lib, "BulletCollision.lib")
    #pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
    #endif

    static int GetRandInt(int TMax) { return rand() % TMax; }

    btDiscreteDynamicsWorld *World;
    static list<btRigidBody *> Objects;

    static IrrlichtDevice *device;
    static video::IVideoDriver *driver;
    static scene::ISceneManager *smgr;
    static gui::IGUIEnvironment* guienv;

    static io::IFileSystem *irrFile;
    static ITimer *irrTimer;
    static ILogger *irrLog;
    static scene::ICameraSceneNode* camera;

    static bool Done = false;

    static void CreateStartScene();
    static void CreateBox(const btVector3 &TPosition, const core::vector3df &TScale, btScalar TMass);
    static void ShootBox(const btVector3 &TPosition, const core::vector3df &TScale, btScalar TMass);
    static void CreateSphere(const btVector3 &TPosition, btScalar TRadius, btScalar TMass);
    static void ClearObjects();
    void UpdatePhysics(u32 TDeltaTime);
    void QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler);

    void createHeightField(scene::ITerrainSceneNode* node);
    void CreateSpherePlayer(const btVector3 &TPosition, btScalar TRadius, btScalar TMass);

    // Event receiver
    class MyEventReceiver : public IEventReceiver  {

    public:

            virtual bool OnEvent(const SEvent &TEvent) {

                    if(TEvent.EventType == EET_KEY_INPUT_EVENT && !TEvent.KeyInput.PressedDown) {
                            switch(TEvent.KeyInput.Key) {
                                    case KEY_ESCAPE:
                                            Done = true;
                                    break;
                                    case KEY_KEY_1:
                                            // Turned off while I experiment with other ways of doing cameras
                                            // Uncomment to turn back on
                                             CreateBox(btVector3(camera->getAbsolutePosition().X,camera->getAbsolutePosition().Y,camera->getAbsolutePosition().Z), core::vector3df(GetRandInt(3) + 0.5f, GetRandInt(3) + 0.5f, GetRandInt(3) + 0.5f), 1.0f);
                                    break;
                                    case KEY_KEY_2:
                                            // Turned off while I experiment with other ways of doing cameras
                                            // Uncomment to turn back on
                                             CreateSphere(btVector3(camera->getAbsolutePosition().X,camera->getAbsolutePosition().Y,camera->getAbsolutePosition().Z), GetRandInt(5) / 5.0f + 0.2f, 1.0f);
                                    break;
                                    case KEY_KEY_X:
                                            CreateStartScene();
                                    break;
                                    default:
                                            return false;
                                    break;
                            }

                            return true;
                    }

                    return false;
            }
    };

    int main()
    {
            MyEventReceiver receiver;

            device = createDevice( video::EDT_OPENGL, dimension2d<u32>(1280, 720), 16, false, false, false, &receiver);

        if (!device)
            return 1;

            irrTimer = device->getTimer();

            device->setWindowCaption(L"Messing with terrain");

            driver = device->getVideoDriver();
        smgr = device->getSceneManager();
        guienv = device->getGUIEnvironment();


            btBroadphaseInterface *BroadPhase = new btAxisSweep3(btVector3(-10000,-10000,-10000), btVector3(10000,10000,10000));
            btDefaultCollisionConfiguration *CollisionConfiguration = new btDefaultCollisionConfiguration();
            btCollisionDispatcher *Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
            btSequentialImpulseConstraintSolver *Solver = new btSequentialImpulseConstraintSolver();
            World = new btDiscreteDynamicsWorld(Dispatcher, BroadPhase, Solver, CollisionConfiguration);

            IGUISkin* skin = guienv->getSkin();
            IGUIFont* font = guienv->getBuiltInFont();

            // turned off while experimenting
        //camera = smgr->addCameraSceneNodeFPS(0,300.0f,1.2f);
        //camera->setPosition(core::vector3df(4138,1088,2354));
        //camera->setTarget(core::vector3df(0,0,0));
        //camera->setFarValue(42000.0f);

            // disable mouse cursor
        device->getCursorControl()->setVisible(false);

            scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("skydome.jpg"),16,8,0.95f,2.0f);

            // add terrain scene node
        scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
                    "terrain-heightmap.bmp",
            0,                                      // parent node
            -1,                                     // node id
            core::vector3df(0.f, -100.f, 0.f),         // position
            core::vector3df(0.f, 0.f, 0.f),         // rotation
            core::vector3df(10, 2, 10),      // scale
            video::SColor ( 255, 255, 255, 255 ),   // vertexColor
            5,                                      // maxLOD
            scene::ETPS_17,                         // patchSize
            5                                      // smoothFactor
        );

        terrain->setMaterialFlag(video::EMF_LIGHTING, false);

            createHeightField(terrain);

        terrain->setMaterialTexture(0, driver->getTexture("terrain-texture.jpg"));
            /* ITexture* terrainNormalMap = driver->getTexture("hmap_normal.png");
            if(terrainNormalMap) {
                    driver->makeNormalMapTexture(terrainNormalMap, 9.0f);
                    terrain->setMaterialType(E_MATERIAL_TYPE::EMT_NORMAL_MAP_SOLID);
                    terrain->setMaterialTexture(1,terrainNormalMap);
            } */
        //terrain->setMaterialTexture(1, driver->getTexture("detailmap3.jpg"));
        //terrain->setMaterialType(video::EMT_DETAIL_MAP);

        terrain->scaleTexture(1.04, 1.04);

            //smgr->addLightSceneNode(0, vector3df(821,6416,1327),SColorf(1.0f,1.0f,1.0f),100000,-1);

            CreateSpherePlayer(btVector3(500,100,354), 10.0f, 90);

            u32 TimeStamp = irrTimer->getTime(), DeltaTime = 0;
            while(device->run())
            {
                    if(Done)
                            break;

                    if(device->isWindowActive())
                    {
                            DeltaTime = irrTimer->getTime() - TimeStamp;
                            TimeStamp = irrTimer->getTime();

                            UpdatePhysics(DeltaTime);

                            driver->beginScene(true, true, SColor(255,0,0,0));

                            /* core::stringw tmp(L"Cam: ");
                            tmp += camera->getPosition().X;
                            tmp += ", ";
                            tmp += camera->getPosition().Y;
                            tmp += ", ";
                            tmp += camera->getPosition().Z;

                            font->draw(tmp.c_str(),rect<s32>(130,10,300,50),SColor(255,255,255,255)); */

                            smgr->drawAll();
                            guienv->drawAll();

                            driver->endScene();
                    } else
                    device->sleep(1,0);
                            device->yield();
            }

            device->drop();

            return 0;
    }

    // Create a Box rigid body
    void CreateBox(const btVector3 &TPosition, const core::vector3df &TScale, btScalar TMass) {
            // First create an irrLicht cube
            scene::ISceneNode *Node = smgr->addCubeSceneNode(1.0f);
            Node->setScale(TScale);
            Node->setMaterialFlag(video::EMF_LIGHTING, 1);
            Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            Node->setMaterialTexture(0, driver->getTexture("rust0.jpg"));

            // Set the initial position of the object
            btTransform Transform;
            Transform.setIdentity();
            Transform.setOrigin(TPosition);

            // Give it to the motionState
            btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

            // Create the shape
            btVector3 HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
            btCollisionShape *Shape = new btBoxShape(HalfExtents);

            // Add mass
            btVector3 LocalInertia;
            Shape->calculateLocalInertia(TMass, LocalInertia);

            // Create the rigid body object
            btRigidBody *RigidBody = new btRigidBody(TMass, MotionState, Shape, LocalInertia);

            // Store a pointer to the irrlicht node so we can update it later
            RigidBody->setUserPointer((void *) (Node));

            // Add it to the world
            World->addRigidBody(RigidBody);
            Objects.push_back(RigidBody);
    }

    void createHeightField(scene::ITerrainSceneNode* node)
    {
       btVector3 vertices[3];
       s32 j,k;
       btTriangleMesh *  mTriMesh = new btTriangleMesh();

       irr::scene::CDynamicMeshBuffer* buffer = 0;

       buffer = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_2TCOORDS, irr::video::EIT_16BIT);
       node->getMeshBufferForLOD(*buffer, 0);

       const irr::u32 vertexCount = buffer->getVertexCount();
       const irr::u32 indexCount = buffer->getIndexCount();

       irr::video::S3DVertex2TCoords* mb_vertices = (irr::video::S3DVertex2TCoords*)buffer->getVertexBuffer().getData();

       irr::u16* mb_indices = buffer->getIndices();

       for(j=0;j<indexCount;j+=3)
       {
          for (k=0;k<3;k++)
          {
             s32 index = mb_indices[j+k];
             vertices[k] = btVector3(
                mb_vertices[index].Pos.X*node->getScale().X,
                mb_vertices[index].Pos.Y*node->getScale().Y,
                mb_vertices[index].Pos.Z*node->getScale().Z);
          }
          mTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
       }

       buffer->drop();

       btCollisionShape* mShape = new btBvhTriangleMeshShape(mTriMesh, true);

       btDefaultMotionState* state = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(node->getPosition().X,node->getPosition().Y,node->getPosition().Z)));

       btRigidBody* mRigidBody = new btRigidBody(0, state, mShape, btVector3(0, 0, 0));
       mRigidBody->setCollisionFlags(mRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
       mRigidBody->setUserPointer((void *) (node));
       World->addRigidBody(mRigidBody);
       //Objects.push_back(mRigidBody);
    }

    // Create a Sphere rigid body
    void CreateSphere(const btVector3 &TPosition, btScalar TRadius, btScalar TMass) {
            // CReate an IrrLicht Sphere
            scene::ISceneNode *Node = smgr->addSphereSceneNode(TRadius, 32);
            Node->setMaterialFlag(video::EMF_LIGHTING, 1);
            Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            Node->setMaterialTexture(0, driver->getTexture("ice0.jpg"));

            // Set the initial position of the object
            btTransform Transform;
            Transform.setIdentity();
            Transform.setOrigin(TPosition);

            // give it a default MotionState
            btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

            // create the shape
            btCollisionShape *Shape = new btSphereShape(TRadius);

            // Add mass
            btVector3 LocalInertia;
            Shape->calculateLocalInertia(TMass, LocalInertia);

            // Create the rigid body object
            btRigidBody *RigidBody = new btRigidBody(TMass, MotionState, Shape, LocalInertia);

            // Store a pointer to the irrlicht node so we can update it later
            RigidBody->setUserPointer((void *) (Node));

            // Add it to the world
            World->addRigidBody(RigidBody);
            Objects.push_back(RigidBody);
    }

    // Create a Sphere rigid body as a player?
    void CreateSpherePlayer(const btVector3 &TPosition, btScalar TRadius, btScalar TMass) {
            // CReate an IrrLicht Sphere
            scene::ISceneNode *Node = smgr->addSphereSceneNode(TRadius, 32);
            Node->setMaterialFlag(video::EMF_LIGHTING, 1);
            Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            Node->setMaterialTexture(0, driver->getTexture("ice0.jpg"));
            ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(Node);

            // Set the initial position of the object
            btTransform Transform;
            Transform.setIdentity();
            Transform.setOrigin(TPosition);

            // give it a default MotionState
            btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

            // create the shape
            btCollisionShape *Shape = new btSphereShape(TRadius);

            // Add mass
            btVector3 LocalInertia;
            Shape->calculateLocalInertia(TMass, LocalInertia);

            // Create the rigid body object
            btRigidBody *RigidBody = new btRigidBody(TMass, MotionState, Shape, LocalInertia);

            // Store a pointer to the irrlicht node so we can update it later
            RigidBody->setUserPointer((void *) (Node));

            // Add it to the world
            World->addRigidBody(RigidBody);
            Objects.push_back(RigidBody);
    }

    // Creates a ground box
    void CreateStartScene() {
            ClearObjects();
            CreateBox(btVector3(0.0f, 0.0f, 0.0f), core::vector3df(10.0f, 0.5f, 10.0f), 0.0f);
    }

    // Runs the physics simulation
    // - TDeltaTime tells the simulation how much time has passed since the last frame so the simulation can run independently of the frame rate
    void UpdatePhysics(u32 TDeltaTime) {
            World->stepSimulation(TDeltaTime * 0.001f, 60);

            btRigidBody *TObject;
            // Relay the objects orientation to irrlicht
            for(core::list<btRigidBody *>::Iterator it = Objects.begin(); it != Objects.end(); ++it) {
                    // Update Renderer(*Iterator)
                    scene::ISceneNode *Node = static_cast<scene::ISceneNode *>((*it)->getUserPointer());
                    TObject = *it;

                    // Set position
                    btVector3 Point = TObject->getCenterOfMassPosition();
                    Node->setPosition(core::vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));

                    // Set rotation
                    btVector3 EulerRotation;
                    QuaternionToEuler(TObject->getOrientation(), EulerRotation);
                    Node->setRotation(core::vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));
            }
    }

    // Converts a quaternion to an euler angle
    void QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler) {
            btScalar W = TQuat.getW();
            btScalar X = TQuat.getX();
            btScalar Y = TQuat.getY();
            btScalar Z = TQuat.getZ();
            float WSquared = W * W;
            float XSquared = X * X;
            float YSquared = Y * Y;
            float ZSquared = Z * Z;

            TEuler.setX(atan2f(2.0f * (Y * Z + X * W), -XSquared - YSquared + ZSquared + WSquared));
            TEuler.setY(asinf(-2.0f * (X * Z - Y * W)));
            TEuler.setZ(atan2f(2.0f * (X * Y + Z * W), XSquared - YSquared - ZSquared + WSquared));
            TEuler *= core::RADTODEG;
    }

    void ClearObjects() {
            Objects.clear();
    }


