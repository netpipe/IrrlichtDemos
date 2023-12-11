#include <irrlicht.h>
#include <irrBullet.h>
#include "helloworldexample.h"


using namespace irr;
using namespace core;
using namespace video;
using namespace scene;
using namespace gui;
using namespace io;

IRigidBody *ship;

CHelloWorldExample::CHelloWorldExample()
{

}


bool CHelloWorldExample::OnEvent(const SEvent& event)
{
	if (!device)
		return false;

    switch(event.EventType)
    {

//    case irr::EET_JOYSTICK_INPUT_EVENT:
//    {
//        if(event.JoystickEvent.Joystick == 0)
//        {
//            JoystickState = event.JoystickEvent;
//        }
//        break;
//    }
        case EET_KEY_INPUT_EVENT:
        {
            if(event.KeyInput.PressedDown)
            {
                switch(event.KeyInput.Key)
                {
                    case KEY_KEY_A:
                    {
                        if(translation.Z > -0.1f)
                        {
                            translation.Z -= 0.1f;
                        }
                        break;
                    }
                    case KEY_KEY_Z:
                    {
                        if(translation.Z < 0.1f)
                        {
                            translation.Z += 0.1f;
                        }
                        break;
                    }

                    case KEY_KEY_S:
                    {
                        if(translation.X < 0.1f)
                        {
                            translation.X += 0.1f;
                        }
                        break;
                    }
                    case KEY_KEY_D:
                    {
                        if(translation.X > -0.1f)
                        {
                            translation.X -= 0.1f;
                        }
                        break;
                    }
                    case KEY_KEY_R:
                    {
                        if(translation.Y < 0.1f)
                        {
                            translation.Y += 0.1f;
                        }
                        break;
                    }
                    case KEY_KEY_F:
                    {
                        if(translation.Y > -0.1f)
                        {
                            translation.Y -= 0.1f;
                        }
                        break;
                    }

                    // Rotational controls
                    case KEY_LEFT:
                    {
                        if(rotation.Y > -0.5f)
                        {
                            rotation.Y -= 0.5f;
                        }
                        break;
                    }
                    case KEY_RIGHT:
                    {
                        if(rotation.Y < 0.5f)
                        {
                            rotation.Y += 0.5f;
                        }
                        break;
                    }
                    case KEY_UP:
                    {
                        if(rotation.X > -0.5f)
                        {
                            rotation.X -= 0.5f;
                        }
                        break;
                    }
                    case KEY_DOWN:
                    {
                        if(rotation.X < 0.5f)
                        {
                            rotation.X += 0.5f;
                        }
                        break;
                    }
                    case KEY_KEY_Q:
                    {
                        if(rotation.Z > -0.5f)
                        {
                            rotation.Z -= 0.5f;
                        }
                        break;
                    }
                    case KEY_KEY_E:
                    {
                        if(rotation.Z < 0.5f)
                        {
                            rotation.Z += 0.5f;
                        }
                        break;
                    }
                        default: break;
                    }
            }
            else if(!event.KeyInput.PressedDown)
            {
                switch(event.KeyInput.Key)
                {
                    case KEY_KEY_A:
                    {
                        if(translation.Z < 0.1f)
                        {
                            translation.Z += 0.1f;
                        }
                        break;
                    }
                    case KEY_KEY_Z:
                    {
                        if(translation.Z > -0.1f)
                        {
                            translation.Z -= 0.1f;
                        }
                        break;
                    }
                    case KEY_KEY_S:
                    {
                        if(translation.X > -0.1f)
                        {
                            translation.X -= 0.1f;
                        }
                        break;
                    }
                    case KEY_KEY_D:
                    {
                        if(translation.X < 0.1f)
                        {
                            translation.X += 0.1f;
                        }
                        break;
                    }
                    case KEY_KEY_R:
                    {
                        if(translation.Y > -0.1f)
                        {
                            translation.Y -= 0.1f;
                        }
                        break;
                    }
                    case KEY_KEY_F:
                    {
                        if(translation.Y < 0.1f)
                        {
                            translation.Y += 0.1f;
                        }
                        break;
                    }
                    case KEY_LEFT:
                    {
                        if(rotation.Y < 0.5f)
                        {
                            rotation.Y += 0.5f;
                        }
                        break;
                    }
                    case KEY_RIGHT:
                    {
                        if(rotation.Y > -0.5f)
                        {
                            rotation.Y -= 0.5f;
                        }
                        break;
                    }
                    case KEY_UP:
                    {
                        if(rotation.X < 0.5f)
                        {
                            rotation.X += 0.5f;
                        }
                        break;
                    }
                    case KEY_DOWN:
                    {
                        if(rotation.X > -0.5f)
                        {
                            rotation.X -= 0.5f;
                        }
                        break;
                    }
                    case KEY_KEY_Q:
                    {
                        if(rotation.Z < 0.5f)
                        {
                            rotation.Z += 0.5f;
                        }
                        break;
                    }
                    case KEY_KEY_E:
                    {
                        if(rotation.Z > -0.5f)
                        {
                            rotation.Z -= 0.5f;
                        }
                        break;
                    }
                    default: break;
                }
            }


            break;
        }
        default: break;
    }
    return false;

}


void CHelloWorldExample::runExample()
{

    debugDraw = true;
    drawProperties = true;
    drawWireFrame = false;

    rows = 10;
	columns = 10;

    device =
        createDevice(video::EDT_OPENGL, dimension2d<u32>(1024, 768), 16,
            false, false, false, this);

    // Joysticks initialization
    core::array<SJoystickInfo> joystickInfo;
	if(device->activateJoysticks(joystickInfo))
	{
		std::cout << "Joystick support is enabled and " << joystickInfo.size() << " joystick(s) are present." << std::endl;

		for(u32 joystick = 0; joystick < joystickInfo.size(); ++joystick)
		{
			std::cout << "Joystick " << joystick << ":" << std::endl;
			std::cout << "\tName: '" << joystickInfo[joystick].Name.c_str() << "'" << std::endl;
			std::cout << "\tAxes: " << joystickInfo[joystick].Axes << std::endl;
			std::cout << "\tButtons: " << joystickInfo[joystick].Buttons << std::endl;

			std::cout << "\tHat is: ";

			switch(joystickInfo[joystick].PovHat)
			{
			case SJoystickInfo::POV_HAT_PRESENT:
				std::cout << "present" << std::endl;
				break;

			case SJoystickInfo::POV_HAT_ABSENT:
				std::cout << "absent" << std::endl;
				break;

			case SJoystickInfo::POV_HAT_UNKNOWN:
			default:
				std::cout << "unknown" << std::endl;
				break;
			}
		}
	}
	else
	{
		std::cout << "Joystick support is not enabled." << std::endl;
	}


    device->setWindowCaption(L"irrBullet: Physics Toy1");

    device->getFileSystem()->addFolderFileArchive("media/");


    device->getSceneManager()->addLightSceneNode(0, vector3df(20, 40, -50), SColorf(1.0f, 1.0f, 1.0f), 4000.0f);


   ISceneNode *sphere = device->getSceneManager()->addSphereSceneNode(10);
    sphere->setPosition(core::vector3df(10.0f,10.0f,0.0f));

    camera = device->getSceneManager()->addCameraSceneNode();


    ////////////////////////////
    // Create irrBullet World //
    ////////////////////////////
    world = createIrrBulletWorld(device, true, debugDraw);

    world->setDebugMode(EPDM_DrawAabb |
            EPDM_DrawContactPoints);

    world->setGravity(vector3df(0,0,0));

ICollisionShape *shape = new ISphereShape(sphere, 4, true);
//	ICollisionShape *shape = new ISphereShape(sphere,  .05, true);

	ship = world->addRigidBody(shape);
	ship->setActivationState(EAS_DISABLE_DEACTIVATION);
	ship->setDamping(0.65,0.55);
	//ship->setFriction(25.0f);
	ship->setFriction(0.0329895f);

   // camera = device->getSceneManager()->addCameraSceneNodeFPS();
	camera->setPosition(vector3df(50,15,200));
	createGround();
	createBoxes();

    device->getSceneManager()->setAmbientLight(SColorf(1000.f,1000.f,1000.f));

    // Set our delta time and time stamp
    u32 TimeStamp = device->getTimer()->getTime();
    u32 DeltaTime = 0;
    while(device->run())
    {
        device->getVideoDriver()->beginScene(true, true, SColor(255,100,101,140));

        DeltaTime = device->getTimer()->getTime() - TimeStamp;
		TimeStamp = device->getTimer()->getTime();

        //handle_input(joystickInfo);

//        if(joystickInfo.size() > 0)
//		{
//			f32 yaw = 0.f; // Range is -1.f for full left to +1.f for full right
//			f32 pitch = 0.f; // -1.f for full down to +1.f for full up.
//			f32 roll = 0.f;
//
//			const SEvent::SJoystickEvent & joystickData = JoystickState;
//
//			// We receive the full analog range of the axes, and so have to implement our
//			// own dead zone.  This is an empirical value, since some joysticks have more
//			// jitter or creep around the center point than others.  We'll use 5% of the
//			// range as the dead zone, but generally you would want to give the user the
//			// option to change this.
//			const f32 DEAD_ZONE = 0.10f;
//
//			yaw =
//				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
//			if(fabs(yaw) < DEAD_ZONE)
//				yaw = 0.f;
//
//			pitch =
//				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
//			if(fabs(pitch) < DEAD_ZONE)
//				pitch = 0.f;
//
//            roll =
//                (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_R] / 32767.f;
//            if(fabs(roll) < DEAD_ZONE)
//                roll = 0.f;
///
//            ship->applyTorqueImpulse(core::vector3df(DeltaTime*(-0.01f*pitch),
//                                                DeltaTime*(0.05f*yaw),
//                                                DeltaTime*(0.01f*roll)),
//                                                ERBTS_LOCAL);
//*/
//            ship->applyTorqueImpulse(core::vector3df(DeltaTime*(-0.1399994f*pitch),
//                                                DeltaTime*(0.1399994f*yaw),
//                                                DeltaTime*(0.1399994f*roll)),
//                                                ERBTS_LOCAL);
//
//		}
/*
        ship->applyCentralImpulse(core::vector3df(DeltaTime*(0.05f*translation.X),
                                                DeltaTime*(0.05f*translation.Y),
                                                DeltaTime*(0.05f*translation.Z)),
                                                ERBTS_LOCAL);
*/
        ship->applyCentralImpulse(core::vector3df(DeltaTime*(7.799988f*translation.X),
                                                DeltaTime*(7.799988f*translation.Y),
                                                DeltaTime*(7.799988f*translation.Z)),
                                                ERBTS_LOCAL);

            ship->applyTorqueImpulse(core::vector3df(DeltaTime*(0.14f*rotation.X),
                                                DeltaTime*(0.14f*rotation.Y),
                                                DeltaTime*(0.14f*rotation.Z)),
                                                ERBTS_LOCAL);

		// Step the simulation with our delta time
        world->stepSimulation(DeltaTime*0.001f, 120);

        logic();


        world->debugDrawWorld(debugDraw);
        // This call will draw the technical properties of the physics simulation
        // to the GUI environment.
        world->debugDrawProperties(drawProperties);


        device->getSceneManager()->drawAll();
        device->getGUIEnvironment()->drawAll();

        device->getVideoDriver()->endScene();
    }

    // We're done with the IrrBullet world, so we free the memory that it takes up.
    if(world)
        delete world;

    if(device)
        device->drop();
}


void CHelloWorldExample::createBoxes()
{
    for(u32 j=0; j < columns; j++)
    {
        for(u32 i=0; i < rows; i++)
        {
            addCube(vector3df(3 * i, 0 + 3 * j + 3, 0), vector3df(3,3,3), 3);
        }
    }
}


void CHelloWorldExample::createGround()
{

    ISceneNode *Node = device->getSceneManager()->addCubeSceneNode(1.0);
	Node->setScale(vector3df(300,3,300)); // 400, 3, 400
	Node->setPosition(vector3df(20,0,10));
	Node->setMaterialFlag(video::EMF_LIGHTING, true);
	Node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("stones.jpg"));

	//Node->setMaterialTexture(0, device->getVideoDriver()->getTexture("rockwall.jpg"));
/*
    if(drawWireFrame)
        Node->setMaterialFlag(EMF_WIREFRAME, true);
*/
    //IAnimatedMesh* mesh = device->getSceneManager()->getMesh("media/tridoor1/mdl/ascent.orf_Mat0.irrmesh");
    IAnimatedMesh* mesh = device->getSceneManager()->getMesh("media/speedtroid.obj");

    if (!mesh)
	{
		exit(42);
;
	}

	IAnimatedMeshSceneNode* node = device->getSceneManager()->addAnimatedMeshSceneNode( mesh );
    //node->setMaterialFlag(EMF_WIREFRAME, true);
    if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, true);
		node->setMaterialFlag(EMF_GOURAUD_SHADING, true);
		node->setRotation(irr::core::vector3df(90.f,0,180.f));
		node->setScale(vector3df(40.f,40.f,40.f));
	}


	ICollisionShape *shape = new IBvhTriangleMeshShape(node, mesh, false);

	//shape->setMargin(0.01);

	IRigidBody *body;
	body = world->addRigidBody(shape);
}

//void CHelloWorldExample::convertIrrMeshBufferBtTriangleMesh(IMeshBuffer *meshBuffer, btTriangleMesh *collisionMesh, vector3df scale)
//{
//  btVector3 triVerts[3];
//  u16 *indices = meshBuffer->getIndices();
//
//  if(meshBuffer->getVertexType() == EVT_STANDARD)
//  {
//    S3DVertex *verts = (S3DVertex *)meshBuffer->getVertices();
//    for(u32 i = 0; i < meshBuffer->getIndexCount(); i+= 3)
//    {
//      triVerts[0] = btVector3(
//        verts[indices[i]].Pos.X * scale.X,
//        verts[indices[i]].Pos.Y * scale.Y,
//        verts[indices[i]].Pos.Z * scale.Z);
//
//      triVerts[1] = btVector3(
//        verts[indices[i+1]].Pos.X * scale.X,
//        verts[indices[i+1]].Pos.Y * scale.Y,
//        verts[indices[i+1]].Pos.Z * scale.Z);
//
//      triVerts[2] = btVector3(
//        verts[indices[i+2]].Pos.X * scale.X,
//        verts[indices[i+2]].Pos.Y * scale.Y,
//        verts[indices[i+2]].Pos.Z * scale.Z);
//
//      collisionMesh->addTriangle(triVerts[0], triVerts[1], triVerts[2]);
//    }
//  }
//  else
//  {
//    S3DVertex2TCoords *verts = (S3DVertex2TCoords *)meshBuffer->getVertices();
//    for(u32 i = 0; i < meshBuffer->getIndexCount(); i+= 3)
//    {
//      triVerts[0] = btVector3(
//        verts[indices[i]].Pos.X * scale.X,
//        verts[indices[i]].Pos.Y * scale.Y,
//        verts[indices[i]].Pos.Z * scale.Z);
//
//      triVerts[1] = btVector3(
//        verts[indices[i+1]].Pos.X * scale.X,
//        verts[indices[i+1]].Pos.Y * scale.Y,
//        verts[indices[i+1]].Pos.Z * scale.Z);
//
//      triVerts[2] = btVector3(
//        verts[indices[i+2]].Pos.X * scale.X,
//        verts[indices[i+2]].Pos.Y * scale.Y,
//        verts[indices[i+2]].Pos.Z * scale.Z);
//
//      collisionMesh->addTriangle(triVerts[0], triVerts[1], triVerts[2]);
//    }
//  }
//  //printf("Converted Irrlicht mesh buffer to Bullet triangle mesh, numTriangles = %i\n", collisionMesh->getNumTriangles());
//}

CHelloWorldExample::~CHelloWorldExample()
{
}

void CHelloWorldExample::handle_input(core::array<SJoystickInfo> joystickInfo)
{
//    if(joystickInfo.size() > 0)
//		{
//			f32 yaw = 0.f; // Range is -1.f for full left to +1.f for full right
//			f32 pitch = 0.f; // -1.f for full down to +1.f for full up.
//			f32 roll = 0.f;
//
//			const SEvent::SJoystickEvent & joystickData = JoystickState;
//
//			// We receive the full analog range of the axes, and so have to implement our
//			// own dead zone.  This is an empirical value, since some joysticks have more
//			// jitter or creep around the center point than others.  We'll use 5% of the
//			// range as the dead zone, but generally you would want to give the user the
//			// option to change this.
//			const f32 DEAD_ZONE = 0.05f;
//
//			yaw =
//				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_X] / 32767.f;
//			if(fabs(yaw) < DEAD_ZONE)
//				yaw = 0.f;
//
//			pitch =
//				(f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_Y] / -32767.f;
//			if(fabs(pitch) < DEAD_ZONE)
//				pitch = 0.f;
//
//            roll =
//                (f32)joystickData.Axis[SEvent::SJoystickEvent::AXIS_R] / 32767.f;
//            if(fabs(roll) < DEAD_ZONE)
//                roll = 0.f;
//
//            //ship->applyTorqueImpulse(core::vector3df(0, 0.05f*yaw, 0), ERBTS_LOCAL);
//            //ship->applyTorqueImpulse(core::vector3df(-0.05f*pitch, 0, 0), ERBTS_LOCAL);
//            //ship->applyTorqueImpulse(core::vector3df(0, 0, 0.05f*roll), ERBTS_LOCAL);
//
//		}
}

void CHelloWorldExample::logic()
{
        //Move Camera to position of Camera in relation to the RigidBody
        matrix4 m;
        m.setRotationDegrees(ship->getWorldTransform().getRotationDegrees());

        // Transform forward vector of camera
        irr::core::vector3df frv = irr::core::vector3df (0.0f, 0.0f, 1.0f);
        m.transformVect(frv);

        // Transform up vector of camera
        irr::core::vector3df upv = irr::core::vector3df (0.0f, 1.0f, 0.0f);
        m.transformVect(upv);

        // Transform camera offset
        //m.transformVect(irr::core::vector3df(0.0f, 0.0f, 0.0f));

        // set camera
 //       camera->setPosition(ship->getPosition());
        camera->setPosition(ship->getWorldTransform().getTranslation());
        camera->setUpVector(upv); //set up vector of camera
        camera->setTarget(ship->getWorldTransform().getTranslation() - frv); //set target of camera (look at point)

}
