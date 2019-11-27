void recursiveFillMetaSelector(scene::ISceneNode* node, scene::IMetaTriangleSelector* meta )
         {//
          // NODE TREE BASIC META TRIANGLE COLLECTOR / First position of the camera
          // This will start from the root node to the last node in the tree, to check for some specified node and setup
          // the metatriangleselector for defining the collision and setup the FPS position based on the last std camera
          // defined in the scene.
          // Code based on Vitek (Travis) informations
          // To do: Adding also land to the meta collision
          printf ("Init");
          // Basic Metaselector application for Basic Collision
          // Put all the occtree in the  Meta Selector
          // Set the FPS camera if a camera is defined there. (Still need cleanup of the old Camera)
          //printf ("Node name is: %s \n",node->getName());
          //printf ("Node id is: %d \n",node->getID());
          //printf ("Node type:");
          //node=smgr->getRootSceneNode();
          Rig rig; // need the rig class to get the cam positions (camera,CamRot,CamPos)

          if (node->getType() ==   ESNT_CAMERA )
                { printf("STD Camera! \n");
                  printf ("The current position of this camera is: %f,%f,%f\n\n",node->getPosition().X,node->getPosition().Y,node->getPosition().Z);
                  //feetnode->setPosition(node->getPosition());
                  rig.camera->setPosition(node->getPosition());
                  rig.camera->setRotation(node->getRotation());
                  //camera->setTarget(node->getTarget());
                  //Gameinfo game;
                  rig.CamRot=node->getRotation();
                  rig.CamPos=node->getPosition();
                  printf ("Included cam rotation: %f,%f,%f \n",node->getRotation().X,node->getRotation().Y,rig.CamRot.Z);
                  //TO do, remove the node after applying it to the FPS Camera.
                  // This will have to be reworked.
                  //node->removeAll();
                }
          if (node->getType() ==   ESNT_OCTREE)
                {

                  node->setMaterialFlag(EMF_FOG_ENABLE,true);
                  // Occ Trees are for objects that will use collision, put the triangle selector in a metaSelector
                  printf("Occtree! \n");
                  io::IAttributes* attribs = device->getFileSystem()->createEmptyAttributes();
                  if (attribs)
                     {// get the mesh name out
                         node->serializeAttributes(attribs);
                         printf("Occtree step 1! \n");
                         core::stringc name = attribs->getAttributeAsString("Mesh");
                         attribs->drop();
                         printf("Occtree step 2! \n");
                         // get the animated mesh for the object
                         scene::IAnimatedMesh* mesh = device->getSceneManager()->getMesh(name.c_str());
                         if (mesh)
                              {
                               scene::ITriangleSelector* selector =
                               device->getSceneManager()->createOctTreeTriangleSelector(mesh->getMesh(0), node, 128);
                               node->setTriangleSelector(selector);
                               meta->addTriangleSelector(selector);
                               selector->drop();
                              }

                     }

                  }
           // now recurse on children...
           core::list<scene::ISceneNode*>::ConstIterator begin = node->getChildren().begin();
           core::list<scene::ISceneNode*>::ConstIterator end   = node->getChildren().end();

           for (; begin != end; ++begin)
           recursiveFillMetaSelector(*begin, meta);
}

