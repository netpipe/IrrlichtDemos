#include "main.h"
class Cparadigm;
class Cevents;
class Cmap;
class Cchar;
class Cgui;

static Cparadigm *instance = &Cparadigm::Instance();
static Cevents *myevents = &Cevents::Instance();
static Cmap *mymap = &Cmap::Instance();
static Cchar *mychar = &Cchar::Instance();
static Cgui *mygui = &Cgui::Instance();


int Cmap::init()
{
        mymap->setRunning(true);
        mymap->loadLevel = false;

        mymap->world = createIrrBulletWorld(instance->device, true, true);

        mymap->world->setDebugMode(EPDM_DrawAabb |
                EPDM_DrawContactPoints);

        mymap->world->setGravity(vector3df(0,-10,0));

        return 0;
};

void Cmap::loadlevel()
{
    if (!mymap->loadLevel)
    {

        instance->smgr->loadScene("./data/level1.irr");

        // create metatriangleselector
        scene::IMetaTriangleSelector * meta = instance->smgr->createMetaTriangleSelector();
        core::array<scene::ISceneNode *> nodes;

        // gets all nodes
       instance->smgr->getSceneNodesFromType(scene::ESNT_ANY, nodes);

	   ICollisionShape *shape;
	   IRigidBody* physicsNode;

       for (u32 i=0; i < nodes.size(); ++i)
       {
          scene::ISceneNode * node = nodes[i];
          scene::ITriangleSelector * selector = 0;

          switch(node->getType())
          {
          case scene::ESNT_ANIMATED_MESH:
             selector = instance->smgr->createTriangleSelectorFromBoundingBox(node);
          break;

          case scene::ESNT_CUBE:
			 printf("(MESH/CUBE)%s\n", node->getName());
			 selector = instance->smgr->createTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);

			 shape = new IConvexHullShape(((scene::IMeshSceneNode*)node),
					   ((scene::IMeshSceneNode*)node)->getMesh(),
					   25.0);
			 // Add object to physics world
             physicsNode = world->addRigidBody(shape);
			 physicsNode->setGravity(vector3df(0,-10,0));
			 shape->setLocalScaling(node->getScale(), ESP_VISUAL);

		  break;

		  // If game object is a regular mesh
          case scene::ESNT_MESH:
          case scene::ESNT_SPHERE:
             // output for quick debug
             printf("(MESH)%s\n", node->getName());
             selector = instance->smgr->createTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);

			 // If object is level geometry, make static.
			 if ((int)node->getID() < _LEVELGEO)
				 shape = new IBvhTriangleMeshShape(((scene::IMeshSceneNode*)node),
					   ((scene::IMeshSceneNode*)node)->getMesh(),
					   0.0);

			 // If object is NOT level geometry add MASS
			 else
			 {
				 shape = new IGImpactMeshShape(((scene::IMeshSceneNode*)node),
					   ((scene::IMeshSceneNode*)node)->getMesh(),
					   25.0);
			 }

			 // Add object to physics world
             physicsNode = world->addRigidBody(shape);
		
		 	 // If object is  not level geometry add gravity
			 if ((int)node->getID() < _LEVELGEO)
				physicsNode->setGravity(vector3df(0,0,0));
			 else
			 {
				physicsNode->setGravity(vector3df(0,-10,0));
			 }
			  
			// Scale object for physics
             shape->setLocalScaling(node->getScale(), ESP_VISUAL);

			// If object is level geometry, define as static
			if ((int)node->getID() < _LEVELGEO)
				physicsNode->setCollisionFlags(ECF_STATIC_OBJECT);

          break;

          case scene::ESNT_OCTREE:
                // output for quick debug
             printf("(OCTREE)%s\n", node->getName());
             selector = instance->smgr->createOctreeTriangleSelector(((scene::IMeshSceneNode*)node)->getMesh(), node);
            break;

          default:
             // Don't create a selector for this node type
             printf("%s, NO COLLISIONS!\n", node->getName());
             break;
          }


    }
    }

    else
        cout << "Level already loaded.." << endl;
};


