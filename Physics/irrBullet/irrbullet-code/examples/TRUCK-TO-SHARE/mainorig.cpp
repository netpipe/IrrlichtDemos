
/// TO INSTALL NEEDED LIBS, PLEASE TAKE A LOOK AT "header.h" FILE
#include "my_header.h"





/// Irrbullet stuff:
enum ECollisionGroupTypes{
COL_NOTHING = 0, // <-- Collide with nothing
COL_TERRAIN = BIT(1),
COL_WORLD_OBJECT = BIT(2),
COL_VEHICLE = BIT(3)
};

int objectCollidesWith         = COL_VEHICLE | COL_TERRAIN | COL_WORLD_OBJECT;
int vehicleRaycastCollidesWith = COL_TERRAIN | COL_WORLD_OBJECT;
int terrainCollidesWith        = COL_VEHICLE | COL_WORLD_OBJECT;





/// Define Some Irrlicht Global Variables:
IrrlichtDevice *device       = 0; // Create Device
video::IVideoDriver* driver  = 0; // Create Driver
scene::ISceneManager* smgr   = 0; // Create Scene Manager

int SCREEN_WIDTH  = 900;
int SCREEN_HEIGHT = 700;

bool DEBUG        = false;
bool IRRSHADOWS   = false;
bool XEFFECTS     = false;

EffectHandler* effect = 0;











int main(int argc, char** argv){




    /// Loop to chose driver from all available:
    video::E_DRIVER_TYPE driverType;
    for(int i=0; i<irr::video::EDT_COUNT; ++i){
    if(irr::IrrlichtDevice::isDriverSupported(irr::video::E_DRIVER_TYPE(i))){ driverType = irr::video::E_DRIVER_TYPE(i); }
    };


    /// Create device from the above found driver:
	device = createDevice(driverType, core::dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), 32, false, IRRSHADOWS, false);
	if(device == 0){ return 1; }; // If OpenGL doesn´t exist

	video::IVideoDriver* driver  = device->getVideoDriver();     // Create Driver
	io::IFileSystem* filesys     = device->getFileSystem();
    gui::IGUIEnvironment* guienv = device->getGUIEnvironment();  // For GUI texts
	scene::ISceneManager* smgr   = device->getSceneManager();    // Create Scene Manager
	// video::IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices(); // Not used, by now...

	// Add an events receiver: (keyboard)
	MyEventReceiver receiver;
    device->setEventReceiver(&receiver);

    /// Other Stuff: (Window)
	device->setResizable(true); // Resizable window

    /// MOUSE CURSOR Stuff:
	ICursorControl* cursor = device->getCursorControl();
	cursor->setVisible(false); // Mouse cursor visibility

	/// Load all needed files: (package is a ZIP file, in encryption level: AES-128)
	// http://irrlicht.sourceforge.net/docu/classirr_1_1io_1_1_i_file_system.html#a05de4e45b4d36c779b9b8f7cb1dccf7c
    // device->getFileSystem()->addFileArchive("data/world.dll", true, false, EFAT_ZIP, "palavramuitolonga2011"); // String filename, ignoreCase, ignorePaths, fileType, password
    device->getFileSystem()->addFileArchive("data/world.zip", true, false, EFAT_ZIP, "palavramuitolonga2012"); // String filename, ignoreCase, ignorePaths, fileType, password

    /// Load all needed files: (from this folder, too)
    /// device->getFileSystem()->addFolderFileArchive("data");

    /// smgr->getParameters()->setAttribute(scene::ALLOW_ZWRITE_ON_TRANSPARENT, true); /// TRANSPARENCIAS -> problema descrito aqui: http://irrlicht.sourceforge.net/forum/viewtopic.php?p=176955
    /// smgr->getParameters()->setAttribute(scene::B3D_LOADER_IGNORE_MIPMAP_FLAG, true);
	/// driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);









	/// SCENE (lights + nodes + physics + cameras + GUI):
	#include "my_scene.h"

    // save and load a scene:
    // smgr->saveScene("test.irr");
    // smgr->loadScene("test.irr");


    core::stringc myfile = "site2.irrmesh";
    /// SAVE Mesh file:
    IWriteFile* wf = device->getFileSystem()->createAndWriteFile(myfile.c_str());
    smgr->createMeshWriter(EMWT_IRR_MESH)->writeMesh(wf, siteNode->getMesh(), EMWT_IRR_MESH);
    wf->drop();
    /// LOAD Mesh file:
    IMeshSceneNode* loadedNode = smgr->addMeshSceneNode(smgr->getMesh(myfile.c_str()));
    loadedNode->setMaterialFlag(EMF_LIGHTING, true);
    loadedNode->setVisible(false);










    /// For keyboard/mouse/other events (to avoid too fast repetitive press/clicks loops):
    int canClickAgain[100];                                  // Array to hold key/mouse status (UP)
    int arrCASize = sizeof(canClickAgain)/sizeof(int);       // Count array items
	for(u32 i=0; i<arrCASize; ++i){ canClickAgain[i] = 1; }  // Fill array


    // f32 then = device->getTimer()->getTime();
    u32 TimeStamp = device->getTimer()->getTime();
    u32 frameDeltaTime = 0;

    f32 rotat  = 0.0; // wheels default rotation for vehicle
    f32 trotat = 0.0; // wheels default rotation for trailer
    int force  = 0;
    f32 moving = 60.0;
    bool TEXTS = false;

	// Add two shadow lights.
	// The first parameter specifies the shadow map resolution for the shadow light.
	// The shadow map is always square, so you need only pass 1 dimension, preferably
	// a power of two between 512 and 2048, maybe larger depending on your quality
	// requirements and target hardware. We will just pass the value the user picked.
	// The second parameter is the light position, the third parameter is the (look at)
	// target, the next is the light color, and the next two are very important
	// values, the nearValue and farValue, be sure to set them appropriate to the current
	// scene. The last parameter is the FOV (Field of view), since the light is similar to
	// a spot light, the field of view will determine its area of influence. Anything that
	// is outside of a lights frustum (Too close, too far, or outside of it's field of view)
	// will be unlit by this particular light, similar to how a spot light works.
	// We will add one red light and one yellow light.


	/// XEffects:
	if(XEFFECTS){
    // Disable lighting on the dragon.
	vehicleNode->setMaterialFlag(EMF_LIGHTING, false);
	// effect->setClearColour(SColor(0,250,250,250)); // Set the background clear color to black.
	// effect->addShadowLight(SShadowLight(1024, vector3df(0,50,0), vector3df(0,-1,0), SColor(255,255,255,255), 20.0f, 60.0f, 30.0f * DEGTORAD));

	effect->addShadowLight(
    SShadowLight(
      1024,
      vector3df(10, 30,30),    // position
      vector3df(10,-10,30),    // target
      SColor(0,255,255,255),   // o,r,g,b
      1.0f,                    // nearValue
      1000.0f,                 // farValue
      90.0f * DEGTORAD         // field of view
    ));
	};





    /// Run all:
	int lastFPS = -1;
	while(device->run()){



            driver->beginScene(true, true, video::SColor(0,40,40,40));

            frameDeltaTime = device->getTimer()->getTime() - TimeStamp;
            TimeStamp = device->getTimer()->getTime();

			int fps = driver->getFPS();
			if (lastFPS != fps){
			device->setWindowCaption(L"TRAKTOR - By Jorge Rosa");
			};




			/// UPDATING PHYSICS:
			// world->stepSimulation(DeltaTime*0.001f, 10);
			// world->stepSimulation(DeltaTime*0.001f, 10);
			world->stepSimulation(frameDeltaTime*0.0017f, 10);
			//world->stepSimulation((DeltaTime-TimeStamp)/1000.0f, 120);






            /// KEYBOARD + MOUSE STUFF:
            // A Keyboard event: (exit)
            if(receiver.IsKeyDown(irr::KEY_ESCAPE)){
            if(world){ delete world; };
            if(device){ device->closeDevice(); };
            return 0;
            };




            if(receiver.IsKeyDown(irr::KEY_F1)){
            if(TEXTS){ TEXTS = false; } else { TEXTS = true; };
            };


            if(receiver.IsKeyDown(irr::KEY_F2)){
                if(DEBUG){
                    DEBUG = false;
                    /// sky->setVisible(true);
                    core::array<scene::ISceneNode *> allNodes;
                    smgr->getSceneNodesFromType(scene::ESNT_ANY, allNodes); // Find all nodes
                    for(u32 i=0; i<allNodes.size(); ++i){
                    scene::ISceneNode* node = allNodes[i];  // Create node
                    node->setMaterialFlag(video::EMF_WIREFRAME, false);
                    };
                } else {
                    DEBUG = true;
                    /// sky->setVisible(false);
                    core::array<scene::ISceneNode *> allNodes;
                    smgr->getSceneNodesFromType(scene::ESNT_ANY, allNodes); // Find all nodes
                    for(u32 i=0; i<allNodes.size(); ++i){
                    scene::ISceneNode* node = allNodes[i];  // Create node
                    node->setMaterialFlag(video::EMF_WIREFRAME, true);
                    };
                };
            };






            if(receiver.IsKeyDown(irr::KEY_KEY_W)){
            // IAnimatedMeshSceneNode *node = static_cast<IAnimatedMeshSceneNode*>(rigidBody->getCollisionShape()->getSceneNode());
            // irVehicle->getRigidBody()->applyImpulse(vector3df(0,10,100), node->getPosition(), ERBTS_LOCAL);
            vehicle->getRigidBody()->applyCentralImpulse(vector3df(0,500,0), ERBTS_LOCAL);
            // irVehicle->getRigidBody()->applyTorque(vector3df(0.0, 1.0, 0.0), ERBTS_LOCAL); /// A must be! To initialize forces!!!
            // vehicle->getRigidBody()->applyImpulse(vector3df(0,0,-500), node->getJointNode("Muzzle")->getPosition(), ERBTS_LOCAL);
            // createMuzzleFlash(node->getJointNode("Muzzle"));
            };



            if(receiver.IsKeyDown(irr::KEY_KEY_S)){
            // IAnimatedMeshSceneNode *node = static_cast<IAnimatedMeshSceneNode*>(rigidBody->getCollisionShape()->getSceneNode());
            // irVehicle->getRigidBody()->applyImpulse(vector3df(0,10,100), node->getPosition(), ERBTS_LOCAL);
            vehicle->getRigidBody()->applyCentralImpulse(vector3df(0,-500,0), ERBTS_LOCAL);
            // irVehicle->getRigidBody()->applyTorque(vector3df(0.0, 1.0, 0.0), ERBTS_LOCAL); /// A must be! To initialize forces!!!
            // vehicle->getRigidBody()->applyImpulse(vector3df(0,0,-500), node->getJointNode("Muzzle")->getPosition(), ERBTS_LOCAL);
            // createMuzzleFlash(node->getJointNode("Muzzle"));
            };





            /// stop all EMITTERS:
            em3->setMinParticlesPerSecond(0); /// stop smoke
            em3->setMaxParticlesPerSecond(0); /// stop smoke


            /// reset FORCES:
            for(irr::u32 i=0; i<vehicle->getNumWheels(); i++){ vehicle->applyEngineForce(0,i); }; // force, wheelindex





            if(receiver.IsKeyDown(irr::KEY_KEY_A)){
            // IAnimatedMeshSceneNode *node = static_cast<IAnimatedMeshSceneNode*>(rigidBody->getCollisionShape()->getSceneNode());
            // irVehicle->getRigidBody()->applyImpulse(vector3df(0,10,100), node->getPosition(), ERBTS_LOCAL);
            // vehicle->getRigidBody()->applyCentralImpulse(vector3df(0,0,30), ERBTS_LOCAL);
            // ((IRigidBody*)bbody)->applyTorque(vector3df(0.0, 0.0, 0.3), ERBTS_WORLD); /// A must be! To initialize forces!!!
            // vehicle->getRigidBody()->applyImpulse(vector3df(0,0,-500), node->getJointNode("Muzzle")->getPosition(), ERBTS_LOCAL);
            // vehicle->applyEngineForce(50, 0);

            vehicleNode->setFrameLoop(10,10);

            /// WHEELS ROTATING:
            /// This wheel (index) is in contact with the road?... (If so, rotate it!)
            // for(irr::u32 i=0; i<vehicle->getNumWheels(); i++){
                // if(vehicle->getWheelInfo(i).raycastInfo.isInContact){
                // vehicle->getRigidBody()->applyCentralImpulse(vector3df(0,0,-100), ERBTS_LOCAL);
                /// vehicle->applyEngineForce(-1500, i);
                /// irr::f32 getCurrentSpeedKmHour()


                force = 10000;
                vehicle->applyEngineForce( force, 0);
                vehicle->applyEngineForce(-force, 1);
                vehicle->applyEngineForce( force, 2);
                vehicle->applyEngineForce(-force, 3);

                em3->setMinParticlesPerSecond(10); /// play smoke
                em3->setMaxParticlesPerSecond(30); /// play smoke
                // };
            // };
            };



            if(receiver.IsKeyDown(irr::KEY_KEY_D)){
            // IAnimatedMeshSceneNode *node = static_cast<IAnimatedMeshSceneNode*>(rigidBody->getCollisionShape()->getSceneNode());
            // irVehicle->getRigidBody()->applyImpulse(vector3df(0,10,100), node->getPosition(), ERBTS_LOCAL);
            // vehicle->getRigidBody()->applyCentralImpulse(vector3df(0,0,-30), ERBTS_LOCAL);
            // ((IRigidBody*)bbody)->applyCentralImpulse(vector3df(0.0, 0.0, 0.3), ERBTS_WORLD);
            // ((IRigidBody*)bbody)->applyTorque(vector3df(0.0, 0.0, 0.3), ERBTS_WORLD); /// A must be! To initialize forces!!!
            // vehicle->getRigidBody()->applyImpulse(vector3df(0,0,-500), node->getJointNode("Muzzle")->getPosition(), ERBTS_LOCAL);
            // createMuzzleFlash(node->getJointNode("Muzzle"));

            vehicleNode->setFrameLoop(0,0);
            // vehicleNode->setAnimationSpeed(30);
            // vehicleNode->setLoopMode(false);

            /// This wheel (index) is in contact with the road?... (If so, rotate it!)
            // for(irr::u32 i=0; i<vehicle->getNumWheels(); i++){


                force = 10000;
                vehicle->applyEngineForce(-force, 0);
                vehicle->applyEngineForce( force, 1);
                vehicle->applyEngineForce(-force, 2);
                vehicle->applyEngineForce( force, 3);

                // bodyB[0] = wbody1->getPointer(); /// get wheel  --->  obj->getRigidBody()->getPointer();
                em3->setMinParticlesPerSecond(10); /// play smoke
                em3->setMaxParticlesPerSecond(30); /// play smoke

            // };
                // };
            };



            /// rigidBody->setMassProps(10, vector3df(0,-0.001,0));





            if(receiver.IsKeyDown(irr::KEY_SPACE)){
            // vehicleNode->setPosition(vector3df(0,-8,0));
            };





            if(receiver.IsKeyDown(irr::KEY_KEY_1)){ selectedCamera = 1; };
            if(receiver.IsKeyDown(irr::KEY_KEY_2)){ selectedCamera = 2; };
            if(receiver.IsKeyDown(irr::KEY_KEY_3)){ selectedCamera = 3; };
            if(receiver.IsKeyDown(irr::KEY_KEY_4)){ selectedCamera = 4; };
            if(receiver.IsKeyDown(irr::KEY_KEY_5)){ selectedCamera = 5; };




            /*
            /// WHEELS POSITIONS: (Updates bodies)
            irr::core::matrix4 matR1;
            matR1.setTranslation(vector3df(vehicleNode->getJointNode("right_front_wheel")->getAbsolutePosition()));
            w1Body->setWorldTransform(matR1);
            */




            /// WHEELS ROTATING: (Updates nodes according VELOCITY)
            /// Rotate wheels (nodes) according vehicle´s (rigidbody) velocity:
            f32 velocity = vehicle->getRigidBody()->getLinearVelocity().Z;
            /// ROTATING vehicle wheels:
            irr::core::matrix4 mat;
            mat.setRotationDegrees(vector3df(rotat+=(velocity * 0.12 * frameDeltaTime),0,0));
            rodaNode[1]->setRotation(mat.getRotationDegrees());   /// Apply to nodes
            rodaNode[2]->setRotation(mat.getRotationDegrees());   /// Apply to nodes
            rodaNode[3]->setRotation(mat.getRotationDegrees());   /// Apply to nodes
            rodaNode[4]->setRotation(mat.getRotationDegrees());   /// Apply to nodes


            f32 tvelocity = wtbody1->getLinearVelocity().Z;
            /// ROTATING trailer wheels:
            irr::core::matrix4 mattr;
            mattr.setRotationDegrees(vector3df(trotat+=(tvelocity * 0.12 * frameDeltaTime),0,0));
            rodaTNode[1]->setRotation(mattr.getRotationDegrees());  /// Apply to nodes
            rodaTNode[2]->setRotation(mattr.getRotationDegrees());  /// Apply to nodes


            /// SET VELOCITY LIMITS:
            /// if(velocity<-3){ vehicle->getRigidBody()->setLinearVelocity(vector3df(0,0,-3)); };
            /// if(velocity> 3){ vehicle->getRigidBody()->setLinearVelocity(vector3df(0,0, 3)); };
            /// vehicle->getRigidBody()->setAngularVelocity(vector3df(0,0,0));
            /// vehicle->getRigidBody()->setLinearFactor(vector3df(0,0,0));




            /// WHEELS ROTATING: (Updates nodes according ENGINE FORCE)
            /// Rotate wheels (nodes) according vehicle´s (rigidbody) velocity:
            f32 engine_force = wbody1->getTotalForce().Z  + wbody2->getTotalForce().Z  + wbody3->getTotalForce().Z  + wbody4->getTotalForce().Z;
            f32 totalTorque  = wbody1->getTotalTorque().Z + wbody2->getTotalTorque().Z + wbody3->getTotalTorque().Z + wbody4->getTotalTorque().Z;



            /*
            irr::core::matrix4 mate;
            mate.setRotationDegrees(vector3df(rotat+=(engine_force*0.00020),0,0));
            rodaNode[1]->setRotation(mate.getRotationDegrees());  /// Apply to nodes
            rodaNode[2]->setRotation(mate.getRotationDegrees());  /// Apply to nodes
            rodaNode[3]->setRotation(mate.getRotationDegrees());  /// Apply to nodes
            rodaNode[4]->setRotation(mate.getRotationDegrees());  /// Apply to nodes
            rodaNode[5]->setRotation(mate.getRotationDegrees());  /// Apply to nodes
            rodaNode[6]->setRotation(mate.getRotationDegrees());  /// Apply to nodes
            */






            // bodyB[0]->setRotation(mate.getRotationDegrees());    /// Apply to nodes



            /// WHEELS BRAKES:
            /*
            vehicle->setBrake(1,0); // Breaks on wheel N1
            vehicle->setBrake(1,1); // Breaks on wheel N2
            vehicle->setBrake(1,2); // Breaks on wheel N3
            vehicle->setBrake(1,3); // Breaks on wheel N4
            vehicle->setBrake(1,4); // Breaks on wheel N5
            vehicle->setBrake(1,5); // Breaks on wheel N6
            */




            /*
            /// ATTACH and UPDATE TRAILER POSITION + ROTATION:
            f32 cc = vehicleNode->getAbsolutePosition().Z-9;
            irr::core::matrix4 mat2;
            /// mat2.setRotationDegrees(trailerNode->getRotation());
            mat2.setTranslation(vector3df(trailerNode->getAbsolutePosition().X, trailerNode->getAbsolutePosition().Y, cc));
            trailerRigidBody->setWorldTransform(mat2);
            */


            /*
            irr::core::matrix4 myMatxP2;
            myMatxP2.setTranslation(vector3df(vehicleNode->getPosition().X,vehicleNode->getPosition().Y,vehicleNode->getPosition().Z-9));
            myMatxP2.setRotationDegrees(trailerNode->getRotation());
            trailerRigidBody->setWorldTransform(myMatxP2);
            */








            /*
            /// UPDATES: Rigidbody to node position and rotation:
            vector3df pos = rodaNode1->getAbsolutePosition(); // vehicleNode->getJointNode("06")->getAbsolutePosition();
            // vector3df rot = rodaNode6->getRotation();
            irr::core::matrix4 matW1 = wbody1->getWorldTransform(); // = wbody1->getCollisionShape()->getSceneNode()->getAbsoluteTransformation(); // wbody1->getWorldTransform();
            matW1.setTranslation(vector3df(pos));
            // matW1.setRotationDegrees(vector3df(rot));
            // wbody1->getCollisionShape()->getSceneNode()->getAbsoluteTransformation();
            wbody1->setWorldTransform(matW1);
            // wbody1->setAngularVelocity(rigidBody->getAngularVelocity());
            // wbody1->setLinearVelocity(rigidBody->getLinearVelocity());
            // wbody1->updateObject();
            // getWorldTransformationMatrix()
            // wbody1->apllyMass(10);
            */









            /*
            /// UPDATES: Nodes (wheels) positions, according main node (vehicle)
            rodaNode1->setPosition(vehicleNode->getJointNode("right_front_wheel")->getAbsolutePosition());
            rodaNode2->setPosition(vehicleNode->getJointNode("left_front_wheel")->getAbsolutePosition());
            rodaNode3->setPosition(vehicleNode->getJointNode("right_back_wheel01")->getAbsolutePosition());
            rodaNode4->setPosition(vehicleNode->getJointNode("left_back_wheel01")->getAbsolutePosition());
            rodaNode5->setPosition(vehicleNode->getJointNode("right_back_wheel02")->getAbsolutePosition());
            rodaNode6->setPosition(vehicleNode->getJointNode("left_back_wheel02")->getAbsolutePosition());
            */



            /*
            /// UPDATES: Rigidbody to node position and rotation:
            irr::core::matrix4 matR1, matR2, matR3, matR4, matR5, matR6;
            vector3df pos1 = vector3df(vehicleNode->getJointNode("right_front_wheel")->getAbsolutePosition());
            vector3df rot1 = vector3df(vehicleNode->getRotation());
            matR1.setTranslation(pos1);
            matR1.setRotationDegrees(rot1);
            wbody1->setWorldTransform(matR1);

            vector3df pos2 = vector3df(vehicleNode->getJointNode("left_front_wheel")->getAbsolutePosition());
            vector3df rot2 = vector3df(vehicleNode->getRotation());
            matR2.setTranslation(pos2);
            matR2.setRotationDegrees(rot2);
            wbody2->setWorldTransform(matR2);
            */









            /*
            for (int i=0; i<vehicle->getNumWheels(); i++){
            // synchronize the wheels with the (interpolated) chassis worldtransform
            vehicle->updateWheelTransform(i,true);
            }
            */




            /// rodaNode1->setPosition(wheel1.wheelAxleCS);





            /// http://irrlicht.sourceforge.net/forum/viewtopic.php?t=36861&start=270



            // matrix4 dir;

            /*
            vector3df vv1 = vehicle->getRigidBody()->getLinearVelocity();
            vector3df vv2 = vehicle->getRigidBody()->getAngularVelocity();

            wbody1->setLinearVelocity(vector3df(vv1));
            wbody1->setAngularVelocity(vector3df(vv2));
            */


            /*
            wbody1->setCcdValues(5,5);
            // wbody1->autoCalculateCcdValues();


            vector3df pos = vector3df(0,0,0);
            vector3df rot = vector3df(0,0,0);

            // pos.X = wheel1.chassisConnectionPointCS.X; // rodaNode1->getAbsolutePosition().X;
            // pos.Y = wheel1.chassisConnectionPointCS.Y;
            // pos.Z = wheel1.chassisConnectionPointCS.Z;

            pos.X = rodaNode1->getAbsolutePosition().X-4.5;
            pos.Y = rodaNode1->getAbsolutePosition().Y;
            pos.Z = rodaNode1->getAbsolutePosition().Z;


            rot.X = vehicleNode->getRotation().X;
            rot.Y = vehicleNode->getRotation().Y;
            rot.Z = vehicleNode->getRotation().Z;

            dir.setTranslation(irr::core::vector3df(pos.X, pos.Y, pos.Z));
            // dir.setRotationDegrees(irr::core::vector3df(rot.X, rot.Y, rot.Z));

            wbody1->setWorldTransform(dir);
            */








            /*
            vector3df vv = wbody1->getLinearVelocity();
            irr::core::matrix4 matx;
            matx.setRotationDegrees(vector3df(vv));
            rodaNode1->setRotation(matx.getRotationDegrees());    /// Apply to nodes
            */














            /*
            // btVector3 Point = BallBody->getCenterOfMassPosition();
            // Node->setPosition(vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));
            // btVector3 xx = vector3df(vehicleNode->getJointNode("left_back_wheel02")->getAbsolutePosition());

            btTransform transform;
            transform.setOrigin(btVector3(vehicleNode->getJointNode("left_back_wheel02")->getAbsolutePosition().X, vehicleNode->getJointNode("left_back_wheel02")->getAbsolutePosition().Y, vehicleNode->getJointNode("left_back_wheel02")->getAbsolutePosition().Z));
            // transform.setRotation(btVector3(0,0,90));
            // transform.setBasis(someMatrix) // Optionally rotate by a btMatrix3x3
            // wbody1->setWorldTransform(transform);

            irr::core::matrix4 matr;
            btTransformToIrrlichtMatrix(transform, matr);
            wbody1->setWorldTransform(matr);
            */


            /*
            matR2.setTranslation(vector3df(vehicleNode->getJointNode("left_back_wheel01")->getAbsolutePosition()));
            wbody2->setWorldTransform(matR2);
            matR3.setTranslation(vector3df(vehicleNode->getJointNode("left_front_wheel")->getAbsolutePosition()));
            wbody3->setWorldTransform(matR3);
            matR4.setTranslation(vector3df(vehicleNode->getJointNode("right_back_wheel02")->getAbsolutePosition()));
            wbody4->setWorldTransform(matR4);
            matR5.setTranslation(vector3df(vehicleNode->getJointNode("right_back_wheel01")->getAbsolutePosition()));
            wbody5->setWorldTransform(matR5);
            matR6.setTranslation(vector3df(vehicleNode->getJointNode("right_front_wheel")->getAbsolutePosition()));
            wbody6->setWorldTransform(matR6);

            world->updateCollisionObjects();
            world->synchronizeMotionStates();
            */








            /// if(rigidBody->hasContactResponse()){
            // cout << "contact" << endl;
            /// };

            /// if(rigidBody->checkCollideWith(tbody)){
            // cout << "contact" << endl;
            ///};







            /// UPDATES: Particles positions:
            ps3->setPosition(vehicleNode->getJointNode("smoke01")->getAbsolutePosition());
            /// ps3->setPosition(vehicleNode->getAbsolutePosition());



            /// DRAW Line:
            /// driver->draw3DLine(vector3df(vehicleNode->getPosition()), vector3df(trailerNode->getPosition()));
            // driver->setMaterial(someMaterial);
            // driver->setTransform(video::ETS_WORLD, core::IdentityMatrix);


















            /// RENDER WATER:
            /*
            // set render target texture
			driver->setRenderTarget(rt, true, true, video::SColor(0,0,0,255));
			// make cube invisible and set fixed camera as active camera
			waterNode->setVisible(false);
			terrainNode->setVisible(false);

			// waterCamera[1]->setPosition(core::vector3df(camera[selectedCamera]->getAbsolutePosition().X, -camera[selectedCamera]->getAbsolutePosition().Y, camera[selectedCamera]->getAbsolutePosition().Z));  /// Cam Pos
            // waterCamera[1]->setTarget(core::vector3df(camera[selectedCamera]->getAbsolutePosition().X, -camera[selectedCamera]->getAbsolutePosition().Y-1, camera[selectedCamera]->getAbsolutePosition().Z));    /// Cam target
			/// waterCamera[1]->setPosition(vector3df(camera[selectedCamera]->getAbsolutePosition().X, camera[selectedCamera]->getAbsolutePosition().Y-8, camera[selectedCamera]->getAbsolutePosition().Z));
			/// waterCamera[1]->setTarget(vector3df(  camera[selectedCamera]->getAbsolutePosition().X, camera[selectedCamera]->getAbsolutePosition().Y-6, camera[selectedCamera]->getAbsolutePosition().Z));

            smgr->setActiveCamera(waterCamera[1]);

			// draw whole scene into render buffer
			smgr->drawAll();
			// set back old render target
			// The buffer might have been distorted, so clear it
			driver->setRenderTarget(0, true, true, 0);
			waterNode->setVisible(true);
			terrainNode->setVisible(true);
			*/









            /// CAMERA:
            /// smgr->setActiveCamera(camera[0]);
            smgr->setActiveCamera(camera[selectedCamera]);
            /// FOLLOW NODE -> camera[selectedCamera]->setTarget(vehicle->getRigidBody()->getMotionState()->getWorldTransformationMatrix().getTranslation());
            // camera[selectedCamera]->setPosition(vector3df(20,vehicleNode->getPosition().Y,vehicleNode->getPosition().Z)); // Set Camera Position
            // camera[selectedCamera]->setTarget(vehicle->getRigidBody()->getMotionState()->getWorldTransformationMatrix().getTranslation());
            // camera[selectedCamera]->setPosition(vector3df(20,7,vehicleNode->getPosition().Z));
            // camera[selectedCamera]->updateAbsolutePosition();

            // irr::core::matrix4 mat;
            // mat.setTranslation(vector3df(0,90,0));            // sets positions
            // mat.setRotationDegrees(vector3df(rotat+=0.5,0,0));   // sets rotations
            // vehicle->getRigidBody()->setWorldTransform(mat); /// Apply to physics

            // camera[selectedCamera] = SM->addCameraSceneNode();
            // Put my camera in object A position
            if(selectedCamera == 2){
            camera[selectedCamera]->setTarget(vector3df(vehicleNode->getAbsolutePosition().X,vehicleNode->getAbsolutePosition().Y,vehicleNode->getAbsolutePosition().Z));
            camera[selectedCamera]->setPosition(vector3df(7,vehicleNode->getAbsolutePosition().Y+3,vehicleNode->getAbsolutePosition().Z));
            /// camera[selectedCamera]->setPosition(vector3df(35,0,vehicleNode->getAbsolutePosition().Z+1));
            /// camera[selectedCamera]->setPosition(vector3df(vehicleNode->getAbsolutePosition().X+20,vehicleNode->getAbsolutePosition().Y+10,vehicleNode->getAbsolutePosition().Z));
            // camera[selectedCamera]->updateAbsolutePosition();
            };

            if(selectedCamera == 3){
            camera[selectedCamera]->setTarget(vector3df(vehicleNode->getAbsolutePosition().X,vehicleNode->getAbsolutePosition().Y+1.0,vehicleNode->getAbsolutePosition().Z));
            camera[selectedCamera]->setPosition(vector3df(10,vehicleNode->getAbsolutePosition().Y+1.0,vehicleNode->getAbsolutePosition().Z));
            /// camera[selectedCamera]->setPosition(vector3df(35,0,vehicleNode->getAbsolutePosition().Z+1));
            /// camera[selectedCamera]->setPosition(vector3df(vehicleNode->getAbsolutePosition().X+20,vehicleNode->getAbsolutePosition().Y+10,vehicleNode->getAbsolutePosition().Z));
            // camera[selectedCamera]->updateAbsolutePosition();
            };

            if(selectedCamera == 4){
            camera[selectedCamera]->setTarget(vector3df(vehicleNode->getAbsolutePosition().X,vehicleNode->getAbsolutePosition().Y+1.5,vehicleNode->getAbsolutePosition().Z));
            camera[selectedCamera]->setPosition(vector3df(12,vehicleNode->getAbsolutePosition().Y+1.5,vehicleNode->getAbsolutePosition().Z));
            /// camera[selectedCamera]->setPosition(vector3df(35,0,vehicleNode->getAbsolutePosition().Z+1));
            /// camera[selectedCamera]->setPosition(vector3df(vehicleNode->getAbsolutePosition().X+20,vehicleNode->getAbsolutePosition().Y+10,vehicleNode->getAbsolutePosition().Z));
            // camera[selectedCamera]->updateAbsolutePosition();
            };



            /// Cameras speed, using keyboard:
            core::list<ISceneNodeAnimator*>::ConstIterator anims = camera[selectedCamera]->getAnimators().begin();
            ISceneNodeAnimatorCameraFPS* anim = (ISceneNodeAnimatorCameraFPS*)*anims;
            anim->setMoveSpeed(0.09); // My default: 0.03
            // anim->setMoveSpeed(newCameraSpeed/1000);

            /// Cameras auto ZOOM:
            /*
            f32 zoom = (PI/2.5f) + (velocity*0.02); // Default is: PI/2.5f
            camera[3]->setFOV(zoom);
            // cout << zoom << endl;
            */





































            // f32 khm = getCurrentSpeedKmHour();
            int khm = int(vehicle->getCurrentSpeedKmHour());
            if(khm<0){ khm = 0; };






            f32 cx = camera[selectedCamera]->getPosition().X;
            f32 cy = camera[selectedCamera]->getPosition().Y;
            f32 cz = camera[selectedCamera]->getPosition().Z;

            f32 vx = vehicleNode->getPosition().X;
            f32 vy = vehicleNode->getPosition().Y;
            f32 vz = vehicleNode->getPosition().Z;


            vector3df trot = vector3df(trailerNode->getRotation());









            for(u32 i=0; i<world->getNumManifolds(); ++i){
            ICollisionCallbackInformation* info = world->getCollisionCallback(i);

            ICollisionObject* obj1 = info->getBody0();
            ICollisionObject* obj2 = info->getBody1();

            int numContacts = info->getPointer()->getNumContacts();
            SManifoldPoint point = info->getContactPoint(0); /// info->getContactPoint(i);




            /// AUDIOS (IMPACT or SLIDING sound):
            /*
            btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
            for(int t=0; tgetNumContacts(); ++t){
            // auto point=contactManifold->getContactPoint(t);
            if(point.getDistance()<-0.5f && point.getLifeTime()==1 && point.getAppliedImpulse()>100.f){
            /// play impact sound
            }else if(((contactManifold->getBody0()->getLinearVelocity()-contactManifold->getBody1()->getLinearVelocity()).length()>4.f) && point.getDistance()<1.5f ){
            /// play sliding sound
            }
            }
            */
            /// btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
            for(u32 i=0; i<numContacts; ++i){
            // auto point=contactManifold->getContactPoint(t);
            if(point.getDistance()<-0.5f && point.getLifeTime()==1 && point.getAppliedImpulse()>100.f){
            /// play impact sound
            // }else if(((contactManifold->getBody0()->getLinearVelocity()-contactManifold->getBody1()->getLinearVelocity()).length()>4.f) && point.getDistance()<1.5f ){
            /// play sliding sound
            }
            }




            /// if(info->getContactPoint(i).getDistance() < 0.01f && info->getContactPoint(i).getLifeTime() < 2.0f)
            /// if( abs(info->getContactPoint(i).getDistance()) < 0.1f && numContacts > 0){
            if(info->getContactPoint(0).getDistance()<0.1f && numContacts>0){ /// info->getContactPoint(i);


            /// COLLISION:
            if(obj1->getAttributes()->existsAttribute("box") && obj2->getAttributes()->existsAttribute("road")){
            // cout << "hit" << endl;
            // mySound[2]->play();
            // ((IRigidBody*)info->getBody0())->applyCentralImpulse(core::vector3df(point.getNormalWorldOnB().X, 0, point.getNormalWorldOnB().Z).normalize()*10.0, ERBTS_WORLD);
            };

            if(obj1->getAttributes()->existsAttribute("box") && obj2->getAttributes()->existsAttribute("road")){
            f32 ai = info->getContactPoint(i).getAppliedImpulse();
            /// if(ai>1){ if(ai>10){ ai=10; }; mySound[5]->setVolume(ai*0.1); mySound[5]->play(); };
            f32 cf = info->getContactPoint(i).getCombinedFriction();
            f32 cr = info->getContactPoint(i).getCombinedRestitution();
            // cout << "ai force: " << ai << endl;
            // cout << "cf force: " << cf << endl;
            // cout << "cr force: " << cr << endl;
            /// info->getContactPoint(0);
            };


            };
            // obj1->removeAffectors();
            // obj2->removeAffectors();
            info->getPointer()->clearManifold();  //<- is this the right way to clear & delete the info?
            delete info;
            };















            if(TEXTS){
            /// TEXTS (GUI):
            // Technical
            stringw stags;
            stags += "Main Keys:\n";
            stags += "Exit: ESC \n";
            stags += "Camera: CURSOR + MOUSE\n";
            stags += "-----------------------------------\n";
            stags += "Vehicle Keys:\n";
            stags += "A,W,S,D\n";
            stags += "-----------------------------------\n";
            stags += "\n";
            stags += "Technical Info: \n";
            stags += "Irrlicht Version: "; stags += device->getVersion(); stags += "\n";
            stags += "Drive: "; stags += driver->getName(); stags += "\n";
            stags += "FPS: "; stags += fps; stags += "\n";
            stags += "Camera Position: "; stags += cx; stags += " :: "; stags += cy; stags += " :: "; stags += cz; stags += "\n";
            stags += "\n";
            stags += "Vehicle Engine force on Z: "; stags += engine_force; stags += "\n";
            stags += "Vehicle Position: "; stags += vx; stags += " :: "; stags += vy; stags += " :: "; stags += vz; stags += "\n";
            // stags += "Vehicle Size: "; stags += d1; stags += " :: "; stags += d2; stags += " :: "; stags += d3; stags += "\n";
            stags += "Vehicle Torque on Z: "; stags += totalTorque; stags += "\n";
            stags += "Vehicle Khm: "; stags += khm; stags += "\n";
            stags += "Vehicle Force: "; stags += force; stags += "\n";
            stags += "\n";
            stags += "Trailer Rotation on X: "; stags += trot.X; stags += "\n";

            texts[0]->setText(stags.c_str());
            /// texts[0]->setOverrideColor(video::SColor(255,255,255,255)); // opacity, red, green, blue
            } else {
            texts[0]->setText(L"");
            };






























            /*
            // Draw wireframe:
			if(DEBUG){
            core::array<scene::ISceneNode *> allNodes;
            smgr->getSceneNodesFromType(scene::ESNT_ANY, allNodes); // Find all nodes
            for(u32 i=0; i<allNodes.size(); ++i){ scene::ISceneNode* node = allNodes[i]; // Create node
		    node->setMaterialFlag(video::EMF_WIREFRAME, !node->getMaterial(0).Wireframe);  // Apply mode
            node->setMaterialFlag(video::EMF_POINTCLOUD, false);
            };
            };
            */

            // Draw physics debug:
            if(DEBUG){
			world->debugDrawWorld(true);
            world->debugDrawProperties(true);
			};


			/// XEffects:
            /*
            if(XEFFECTS){
            effect->getShadowLight(0).setPosition(DUMMYLIGHT->getPosition());
            effect->getShadowLight(0).setTarget(vector3df(0,-1,20));
            }
            */

            /*
            f32 sp = smgr->getActiveCamera()->getAbsolutePosition().Z;
            sunNode->setPosition(vector3df(0,20,sp));
            sunNode->updateAbsolutePosition();
            */

            /*
            vector3df lp = vehicleNode->getJointNode("smoke01")->getAbsolutePosition();
            sunNode->setPosition(vector3df(lp.X,lp.Y+10,lp.Z));
            /// ps3->setPosition(vehicleNode->getAbsolutePosition());
            */






            /// RENDER ALL:
            /// driver->setMaterial(driver->getMaterial2D()); // Updates all 2D materials (else some will be inverted/distorted when using postprocess effects)
            /// XEffects:
            if(XEFFECTS){
                effect->update();   // EffectHandler->update() replaces smgr->drawAll(). It handles all of the shadow maps, render targets switching, post processing, etc.
                                    // driver->setMaterial(driver->getMaterial2D());
                };                  // effectHandler->update() now replaces smgr->drawAll()!
            /// Normal Render:
            if(!XEFFECTS){
                smgr->drawAll();
                }; /// Draw scene

            guienv->drawAll();  /// Draw GUI
            driver->endScene(); /// ENDS RENDER


	};

	if(world){  delete world;   };
	if(device){ device->drop(); };
	return 0;
};































/*
    // create metatriangleselector
    scene::IMetaTriangleSelector * meta = smgr->createMetaTriangleSelector();
    core::array<scene::ISceneNode *> nodes;

    // gets all nodes
   smgr->getSceneNodesFromType(scene::ESNT_ANY, nodes);

   ICollisionShape *shape;
   IRigidBody* terrain;

   for (u32 i=0; i < nodes.size(); ++i)
   {
      scene::ISceneNode * node = nodes[i];
      scene::ITriangleSelector * selector = 0;

      switch(node->getType())
      {
      case scene::ESNT_CUBE:
      case scene::ESNT_ANIMATED_MESH:
         selector = smgr->createTriangleSelectorFromBoundingBox(node);
      break;

      case scene::ESNT_MESH:
      case scene::ESNT_SPHERE:
            // output for quick debug
         printf("(SPHERE OR MESH)%s\n", node->getName());
         selector = smgr->createTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);

         shape = new IBvhTriangleMeshShape(((scene::IMeshSceneNode*)node),
               ((scene::IMeshSceneNode*)node)->getMesh(),
               0.0);

         terrain = world->addRigidBody(shape);
         terrain->setGravity(vector3df(0,0,0));
         shape->setLocalScaling(node->getScale(), ESP_VISUAL);
         terrain->setCollisionFlags(ECF_STATIC_OBJECT);
      break;

      case scene::ESNT_TERRAIN:
            // output for quick debug
         printf("(TERRAIN)%s\n", node->getName());
         selector = smgr->createTerrainTriangleSelector((scene::ITerrainSceneNode*)node);
        break;

      case scene::ESNT_OCTREE:
            // output for quick debug
         printf("(OCTREE)%s\n", node->getName());
         selector = smgr->createOctreeTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);
        break;

      default:
         // Don't create a selector for this node type
         printf("%s, NO COLLISIONS!\n", node->getName());
         break;
      }

   }
*/



