#include "Collision.hpp"

Collision *Collision::m_collision = NULL;
Collision Collision::m_default_collision_buffer;

Collision *Collision::Instance()
{
  if(!m_collision)
    m_collision = &m_default_collision_buffer;

  return m_collision;
}

void Collision::createRootCollision(void)
{
  metaSelector = irrDevice->getSceneManager()->createMetaTriangleSelector();
  recursiveFillMetaSelector(irrDevice->getSceneManager()->getRootSceneNode(), metaSelector);
}

void Collision::createPlayerCollision(void)
{

  aabbox3d<f32> box = Player::Instance()->getPlayerNode()->getBoundingBox();
  ellipsoidRadius = box.MaxEdge - box.getCenter();

  anim = irrDevice->getSceneManager()->createCollisionResponseAnimator(
    metaSelector,
    Player::Instance()->getPlayerNode(),
    vector3df(ellipsoidRadius),
    vector3df(0,-30,0),//-11 gravity on the y
    vector3df(0,-23,0),
    0.005f);

  Player::Instance()->getPlayerNode()->addAnimator(anim);
  anim->drop();
}

vector3df Collision::getEllipsoidRadius(void)
{
  return ellipsoidRadius;
}


void Collision::recursiveFillMetaSelector(ISceneNode* node, IMetaTriangleSelector* meta )
{
  //
  // the following if is basically the same as ISceneNode_assignTriangleSelector
  //
  Output::Instance()->w("Node name is: %s \n",node->getName());
  Output::Instance()->w("Node id is: %d \n",node->getID());
  Output::Instance()->w("Node type: ");

  if(strcmp(node->getName(), "elevator") == 0)
  {
  	node->setMaterialType(EMT_LIGHTMAP_M4);
  	node->setMaterialFlag(EMF_LIGHTING, false);
    Elevator::Instance()->Add(node);
    Elevator::Instance()->elevatorStart = node->getAbsolutePosition();
  }
  if(strcmp(node->getName(), "obstacle") == 0)
  {
		Obstacle::Instance()->Add(node);
  }
  if(strcmp(node->getName(), "level") == 0)
  {
    //! Enable fog for all materials on the level node.

    u32 materialCount = node->getMaterialCount();
    for(s32 i = 0; i < materialCount; i++)
    {
      node->getMaterial(i).FogEnable = false;
    }

   // node->setMaterialFlag(EMF_FOG_ENABLE, true);
    node->setMaterialType(EMT_LIGHTMAP_M4);
    node->setMaterialFlag(EMF_LIGHTING, false);
  }
  if(strcmp(node->getName(), "glass") == 0)
  {
    node->setMaterialTexture(1,	irrDevice->getVideoDriver()->getTexture("StageData/glass.tga"));
    node->setMaterialFlag(EMF_LIGHTING, false);
    node->setMaterialFlag(EMF_FOG_ENABLE, false);
    node->setMaterialType(EMT_TRANSPARENT_REFLECTION_2_LAYER);

    SColor col = SColor(155,37,207,243);
    for(s32 i = 0; i < node->getMaterialCount(); i++)
    {
			node->getMaterial(i).Shininess = 0.0f;
			node->getMaterial(i).DiffuseColor = col;
			node->getMaterial(i).EmissiveColor = col;
			node->getMaterial(i).AmbientColor = col;
			node->getMaterial(i).SpecularColor = col;
			node->getMaterial(i).MaterialTypeParam = 0.01;
    }
  }

  if (node->getType() ==   ESNT_UNKNOWN)
  {
  }
  if (node->getType() ==   ESNT_MESH)
  {
      io::IAttributes* attribs = irrDevice->getFileSystem()->createEmptyAttributes();
      if (attribs)
        {// get the mesh name out
         node->serializeAttributes(attribs);
         core::stringc name = attribs->getAttributeAsString("Mesh");
         attribs->drop();
         // get the animated mesh for the object
         scene::IAnimatedMesh* mesh = irrDevice->getSceneManager()->getMesh(name.c_str());
         if (mesh)
         {
            Output::Instance()->w("\tAdding triangle selector for mesh\n");
            scene::ITriangleSelector* selector =
            irrDevice->getSceneManager()->createTriangleSelector(mesh->getMesh(0), node);
            node->setTriangleSelector(selector);
            metaSelector->addTriangleSelector(selector);
            selector->drop();

            node->setMaterialFlag(video::EMF_LIGHTING, true);
            node->getMaterial(0).Shininess = 100.0f;
            node->getMaterial(0).DiffuseColor = SColor(255,255,255,255);
            node->getMaterial(0).EmissiveColor = SColor(255,255,255,255);
            node->getMaterial(0).AmbientColor = SColor(255,255,255,255);
            node->getMaterial(0).SpecularColor = SColor(255,255,255,255);
            node->getMaterial(0).MaterialTypeParam = 0.01;

            //node->getMaterial(0).MaterialType = EMT_ONETEXTURE_BLEND;
            //node->setFlag(EMF_TRILINEAR_FILTER, true);
         }
     }
  }
  if (node->getType() ==   ESNT_ANIMATED_MESH)
    Output::Instance()->w("Animated Mesh! \n\n");
  if (node->getType() ==   ESNT_SKY_BOX)
    Output::Instance()->w("SkyBox! \n\n");
  if (node->getType() ==   ESNT_CAMERA_FPS)
    Output::Instance()->w("Fps Camera! \n\n");
  if (node->getType() ==   ESNT_CAMERA_MAYA )
    Output::Instance()->w("Maya Camera! \n\n");
  if (node->getType() ==   ESNT_CAMERA )
    Output::Instance()->w("STD Camera! \n");
  if (node->getType() ==   ESNT_PARTICLE_SYSTEM )
    Output::Instance()->w("Particles! \n\n");
  if (node->getType() ==   ESNT_LIGHT  )
  {
    Output::Instance()->w("Light! \n\n");
  }
  if (node->getType() ==   ESNT_OCTREE)
  {
      // Occ Trees are for land
      Output::Instance()->w("Occtree! \n");
      io::IAttributes* attribs = irrDevice->getFileSystem()->createEmptyAttributes();
      if (attribs)
        {// get the mesh name out
         node->serializeAttributes(attribs);
         core::stringc name = attribs->getAttributeAsString("Mesh");
         attribs->drop();
         // get the animated mesh for the object
         scene::IAnimatedMesh* mesh = irrDevice->getSceneManager()->getMesh(name.c_str());
         if (mesh)
         {
            scene::ITriangleSelector* selector =
            irrDevice->getSceneManager()->createOctTreeTriangleSelector(mesh->getMesh(0), node, 128);
            node->setTriangleSelector(selector);
            metaSelector->addTriangleSelector(selector);
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
