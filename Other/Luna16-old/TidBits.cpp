//! stick whatever in here for use later or to clean up luna.cpp

/*
    gui::CGUIChatBox* chat = new gui::CGUIChatBox(  guienv, guienv->getRootGUIElement(),
                                                    230, core::rect<int>(20,40,300,500));

    for (int i=0;i < 100;i++){
        chat->addItem(L"(john) Kill that thing...");
        chat->addItem(L"(firestaff) lol", video::SColor(250, 220, 255, 255));
        chat->addItem(L"(juli) that's too big", video::SColor(0, 220, 0, 255));
        chat->addItem(L"(john) Kill that thing...");
        chat->addItem(L"(firestaff) lol", video::SColor(220, 220, 255, 255));
        chat->addItem(L"(juli) that's too big", video::SColor(0, 220, 0, 255));
    };
*/


/*
    gui::IGUIComboBox* box = guienv->addComboBox(
    core::rect<s32>(250,4,350,23),
    0,
    108
  );
  box->addItem(L"No filtering");
  box->addItem(L"Bilinear");
  box->addItem(L"Trilinear");
  box->addItem(L"Anisotropic");
  box->addItem(L"Isotropic");
*/




/*  this used to work really good.

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

   u16* mb_indices = buffer->getIndices();

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

   btDefaultMotionState* state =   new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),
      btVector3(node->getPosition().X,node->getPosition().Y,node->getPosition().Z)));

   btRigidBody* mRigidBody = new btRigidBody(0, state, mShape, btVector3(0, 0, 0));
   mRigidBody->setCollisionFlags(mRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
   mDynamicsWorld->addRigidBody(mRigidBody);
}

*/



//#include "Scene/RealisticWater.h"
//#include "TerrainFactory/ProceduralTrees/CTreeGenerator.h"
//#include "TerrainFactory/ProceduralTrees/CBillboardGroupSceneNode.h"
//#include "TerrainFactory/ProceduralTrees/CTreeSceneNode.h"
//#include "TerrainFactory/WaterNode/CReflectedWater.h"
//CReflectedWater *water;
//        water->updateRendertarget(smgr);
/*
    //! Create Water
    RealisticWaterSceneNode* Water=new RealisticWaterSceneNode(smgr, 360.0f, 234.0f,
    driver->getTexture("./data/waterbump.png"),device->getTimer());

    Water->setPosition(vector3df(0,-50,0));
    // set the water color
    Water->setWaterColor(video::SColor(0.1f, 0.2f, 0.5f, 1.0f));
    // set the wind direction
    Water->setWindDirection(core::vector2df(1,0.5f));
    // Set the wind force
    Water->setWindForce(1222.0f);
    //set the wave height (max 1.0);
    Water->setWaveHeight(1.0f);



void Vehicle::createTerrain(vector3df pos)
{
    if (!m_irrDevice) return;
    int smoothFactor = 0;
    vector3df t_position = pos;
    vector3df t_rotation = vector3df(0,0,0);
    vector3df t_scale = vector3df(5,3,5);

    ITerrainSceneNode *terrainNode = m_irrDevice->getSceneManager()->addTerrainSceneNode(
                                         "data/textures/heightmaps/ctown.png", 0, -1,
                                         t_position, t_rotation, t_scale,
                                         SColor(255,255,255,255), 5, ETPS_17, smoothFactor);
    tr.setIdentity();
    tr.setOrigin(btVector3(pos.X, pos.Y, pos.Z));
    //terrainNode->setRotation (vector3df(0, 1, 0));
    terrainNode->scaleTexture(1,40);
    terrainNode->setMaterialTexture(0, m_irrDevice->getVideoDriver()->getTexture("data/textures/cells/ctown.png"));
    terrainNode->setMaterialTexture(1, m_irrDevice->getVideoDriver()->getTexture("data/textures/detailmaps/rock.png"));
    terrainNode->setMaterialType(EMT_DETAIL_MAP);
    terrainNode->setMaterialFlag(EMF_FOG_ENABLE, false);
    //terrainNode->setMaterialFlag(EMF_LIGHTING, true);

    m_cScene->setGenericMaterial(terrainNode, 0);
    scene::SMeshBufferLightMap meshBuffer;
  //  terrainNode->getMeshBufferForLOD( meshBuffer, 3 );

    btTriangleMesh *collisionMesh = new btTriangleMesh();
    m_cPhysics->convertIrrMeshBufferBtTriangleMesh((IMeshBuffer*)(&meshBuffer), collisionMesh, t_scale);
    btBvhTriangleMeshShape *groundShape = new btBvhTriangleMeshShape(collisionMesh, true);

    localCreateRigidBody(0,tr,groundShape,terrainNode);
}


*/

//terrain->getRenderBuffer()->setHardwareMappingHint(EHM_DYNAMIC);  vbo terrain

/*
   if (sm->getSceneCollisionManager()->getCollisionPoint(
      line, mapSelector, end, triangle))
   {
      // collides with wall
      core::vector3df out = triangle.getNormal();
      out.setLength(0.03f);

      imp.when = 1;
      imp.outVector = out;
      imp.pos = end;
   }
*/




/*
class IShakeItAnimator : public ISceneNodeAnimator
{
    public:
        IShakeItAnimator(u32 ShakeTime, u16 Amp);
        virtual ~IShakeItAnimator() {};
        virtual void animateNode(ISceneNode *node, u32 timeMs);
    private:
        vector3df startPosition;
        u32 mStartTime;
        u32 mTimeToShake;
        u16 Amplitude;
        s8 Direction;
};

IShakeItAnimator::IShakeItAnimator(u32 ShakeTime, u16 Amp)
{
    mStartTime = 0;
    mTimeToShake = ShakeTime;
    Amplitude = Amp;
    Direction = 1;
}

void IShakeItAnimator::animateNode(ISceneNode *node, u32 timeMs)
{
    if (!mStartTime)
    {
        mStartTime = timeMs;
        startPosition = node->getPosition();
    }
    u32 passedTime = timeMs - mStartTime;
    if (passedTime > mTimeToShake)
    {
        node->setPosition(startPosition);
        return;
    }

    float progress = float(passedTime) / float(mTimeToShake);
    node->setPosition(vector3df(node->getPosition().X + ((1-progress)*Amplitude)*Direction, node->getPosition().Y, node->getPosition().X));
    Direction *= -1;
}

*/



/*

IRRFRAMEWORK_API bool Mesh_RayHitNodeTriangles (IAnimatedMeshSceneNode* Node,IAnimatedMesh* mesh,core::line3df Ray,core::vector3df &intersection)
{
int nMeshBuffer = 0;
int v_index[3] = {0,0,0};
core::triangle3df triangle;
IMeshBuffer *mesh_buffer = NULL;
if (Node->getTransformedBoundingBox().intersectsWithLine(Ray))
{
int frame=(int)Node->getFrameNr();
IMesh* irr_mesh=mesh->getMesh(frame);
for( nMeshBuffer=0 ; nMeshBuffer < irr_mesh->getMeshBufferCount() ; nMeshBuffer++ )
{
mesh_buffer = irr_mesh->getMeshBuffer(nMeshBuffer);
S3DVertex *vertices = (S3DVertex*)mesh_buffer->getVertices();
u16 *indices = mesh_buffer->getIndices();
for(int i=0; i<mesh_buffer->getIndexCount()/3; i++)
{
v_index[0] = indices[i*3 ];
v_index[1] = indices[i*3+1];
v_index[2] = indices[i*3+2];
triangle.pointA=vertices[ v_index[0] ].Pos;
triangle.pointB=vertices[ v_index[1] ].Pos;
triangle.pointC=vertices[ v_index[2] ].Pos;
Node->getAbsoluteTransformation().transformVect(triangle.pointA);
Node->getAbsoluteTransformation().transformVect(triangle.pointB);
Node->getAbsoluteTransformation().transformVect(triangle.pointC);
if (triangle.getIntersectionWithLine(Ray.start,Ray.end,intersection))
 {
            driver->setTransform(video::ETS_WORLD, core::matrix4());
         material.Lighting=false;
         material.setTexture(0,0);
         driver->setMaterial(material);
          driver->draw3DTriangle(triangle, video::SColor(255,255,0,0));
         return true;
  }
  }
 }
}
return false;
}
*/



/*
//	plane = smgr->getMeshManipulator()->createMeshWithTangents(plane);
//	ISceneNode *parallaxplane = smgr->addMeshSceneNode(plane); //, null, -1);
//	parallaxplane->setMaterialFlag(EMF_LIGHTING, false);

	water = new CReflectedWater("ReflectedWater", device, smgr, -1, 180, 100, dimension2di(512,512));
	ISceneNode *waternode = water->m_waternode;
	waternode->setPosition(vector3df(0, -30, 0));
	water->m_WaveDisplacement /= 1.5f;
	water->m_WaveHeight *= 1.0f;
	water->m_WaveSpeed *= 1.0f;
	water->m_RefractionFactor = 0.51f;
*/



        // Park camera where the vehicle is.
/*
        btPoint3 point = m_cVehicle->getVehiclePosition();
        camera->setPosition(vector3df(
          (f32)point[0],
          (f32)point[1]+10,
          (f32)point[2]));
*/



/*

   //Setup a oak tree
   video::E_MATERIAL_TYPE leafMaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
    // Load leaf shader
    leafMaterialType = (video::E_MATERIAL_TYPE) driver->getGPUProgrammingServices()->addHighLevelShaderMaterialFromFiles(
        "shaders/trees/leaves.vert",
        "main",
        EVST_VS_2_0,
        "shaders/trees/leaves.frag",
        "main", EPST_PS_2_0 , 0 , EMT_TRANSPARENT_ALPHA_CHANNEL , 0 );

   CTreeGenerator* oakGenerator = new CTreeGenerator(smgr);
   io::IXMLReader* xml = device->getFileSystem()->createXMLReader( "data/trees/Oak.xml" );
   oakGenerator->loadFromXML(xml);
   xml->drop();
   scene::CTreeSceneNode* tree = new scene::CTreeSceneNode(smgr->getRootSceneNode(), smgr);
   tree->drop();
        //Make it smaller
   tree->setScale(core::vector3df(0.2f,0.5f,0.2f));
   tree->setup( oakGenerator );
   tree->getLeafNode()->getMaterial(0).TextureLayer[0].AnisotropicFilter = true;
    tree->getLeafNode()->getMaterial(0).TextureLayer[0].BilinearFilter = false;
   tree->setMaterialTexture(0, driver->getTexture("data/textures/trees/OakBark.png"));
   tree->getLeafNode()->setMaterialTexture(0, driver->getTexture("data/textures/trees/OakLeaf.png"));
   tree->getLeafNode()->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF );
   tree->getLeafNode()->setMaterialType( leafMaterialType );
*/




//!from vehicle.cpp

    // Create chassi. Keep old code
    //  ISceneNode *l_node_chassi = m_irrDevice->getSceneManager()->addCubeSceneNode(1.0f);
    //  l_node_chassi->setScale(vector3df(scale[0], scale[1], scale[2]));
    //  l_node_chassi->setMaterialFlag(EMF_LIGHTING, true);
    //  l_node_chassi->setPosition(vector3df((f32)m_vehiclePosition[0], (f32)m_vehiclePosition[1], (f32)m_vehiclePosition[2]));
    //  l_node_chassi->setMaterialTexture(0, m_irrDevice->getVideoDriver()->getTexture("data/textures/material/stone1_512.png"));

    //IMeshBuffer *meshBuffer = l_node->getMeshBuffer(0);
    //btTriangleMesh *collisionMesh = new btTriangleMesh();
    //m_cPhysics->convertIrrMeshBufferBtTriangleMesh(meshBuffer, collisionMesh, vector3df(5,5,5));
    //btBvhTriangleMeshShape *chassisShape = new btBvhTriangleMeshShape(collisionMesh, true);


/*
       void Vehicle::createTerrain(vector3df pos)//buggy as heck
{
    if (!m_irrDevice) return;

    // Create Irrlicht Terrain.
    int smoothFactor = 0;
    vector3df t_position = pos;
    vector3df t_rotation = vector3df(0,0,0);
    vector3df t_scale = vector3df(4,1,4);

    tr.setIdentity();
    tr.setOrigin(btVector3(pos.X, pos.Y, pos.Z));

    ITerrainSceneNode *terrainNode = m_irrDevice->getSceneManager()->addTerrainSceneNode(
                                         "data/textures/heightmaps/ctown.png", 0, -1,
                                         t_position, t_rotation, t_scale,
                                         SColor(255,255,255,255), 1, ETPS_9, smoothFactor);


    scene::ITriangleSelector*
    selector = m_irrDevice->getSceneManager()->createTerrainTriangleSelector(terrainNode, 0);
	terrainNode->setTriangleSelector(selector);
    metaSelector = m_irrDevice->getSceneManager()->createMetaTriangleSelector();

    //terrainNode->setRotation (vector3df(0, 1, 0));
    terrainNode->scaleTexture(1,40);
    terrainNode->setMaterialTexture(0, m_irrDevice->getVideoDriver()->getTexture("data/textures/cells/ctown.png"));
    terrainNode->setMaterialTexture(1, m_irrDevice->getVideoDriver()->getTexture("data/textures/detailmaps/rock.png"));
    terrainNode->setMaterialType(EMT_DETAIL_MAP);
    // terrainNode->setMaterialFlag(EMF_FOG_ENABLE, false);
    // terrainNode->setMaterialFlag(EMF_LIGHTING, true);
    //  m_cScene->setGenericMaterial(terrainNode, 0);


    ///-------------TESTING AREA
    irr::scene::CDynamicMeshBuffer* meshBuffer = 0;
    meshBuffer = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_2TCOORDS, irr::video::EIT_16BIT);
    terrainNode->getMeshBufferForLOD(*meshBuffer, 4);
    video::S3DVertex2TCoords* mb_vertices = (video::S3DVertex2TCoords*)meshBuffer->getVertexBuffer().getData();
    const irr::u32 vertexCount = meshBuffer->getVertexCount();
    const irr::u32 indexCount = meshBuffer->getIndexCount();
    u16* mb_indices = meshBuffer->getIndices();
    btTriangleMesh *collisionMesh = new btTriangleMesh();

    btVector3 vertices[3];
    s32 j,k;

    for(j=0;j<indexCount;j+=3)
    {
      for (k=0;k<3;k++)
      {
            s32 index = mb_indices[j+k];
            vertices[k] = btVector3(
                mb_vertices[index].Pos.X*terrainNode->getScale().X,
                mb_vertices[index].Pos.Y*terrainNode->getScale().Y,
                mb_vertices[index].Pos.Z*terrainNode->getScale().Z);
        }
        collisionMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
    }

    m_cPhysics->convertIrrMeshBufferBtTriangleMesh(meshBuffer, collisionMesh, t_scale);//(&data, collisionMesh, t_scale);
    btBvhTriangleMeshShape *groundShape = new btBvhTriangleMeshShape(collisionMesh, true);
    localCreateRigidBody(0,tr,groundShape,terrainNode);
	meshBuffer->drop();

//       //this replaces the above code in fewer lines
//    irr::scene::CDynamicMeshBuffer* meshBuffer = 0;
//    mewshBuffer = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_2TCOORDS, irr::video::EIT_16BIT);
//    terrainNode->getMeshBufferForLOD(*meshBuffer, 1);
//    btTriangleMesh *collisionMesh = new btTriangleMesh();
//    m_cPhysics->convertIrrMeshBufferBtTriangleMesh(meshBuffer, collisionMesh, t_scale);
//    btBvhTriangleMeshShape *groundShape = new btBvhTriangleMeshShape(collisionMesh, true);
//    localCreateRigidBody(0,tr,groundShape,terrainNode);
//

    //for irrlicht CD
    #ifdef irrCD
    recursiveFillMetaSelector( terrainNode, metaSelector );
  //  recursiveFillMetaSelector( m_irrDevice->getSceneManager()->getRootSceneNode() , metaSelector );

	scene::ISceneNodeAnimator* anim;
    //FOR the whole scene
//	anim = m_irrDevice->getSceneManager()->createCollisionResponseAnimator(
//		metaSelector, camera, core::vector3df(60,50,60),
//		core::vector3df(0,0,0),
//		core::vector3df(0,50,0));
//	metaSelector->drop();
//	camera->addAnimator(anim);
	//FOR the just this mesh
//	#ifdef MESHONLY
	anim = m_irrDevice->getSceneManager()->createCollisionResponseAnimator(
		selector, camera, core::vector3df(60,50,60),
		core::vector3df(0,0,0),
		core::vector3df(0,50,0));
    camera->addAnimator(anim);
 //   #endif
    selector->drop();
	anim->drop();
	#endif
}
*/

