#include "physics.h"
#include "player.h"

CPhysics::CPhysics(ISceneManager* smgr, IVideoDriver* driver, ITimer* Timer, btVector3 Gravity, CMap* Map)
{
    btDefaultCollisionConfiguration* CollisionConfiguration = new btDefaultCollisionConfiguration();
	btBroadphaseInterface* BroadPhase = new btAxisSweep3(btVector3(-100000, -100000, -100000), btVector3(100000, 100000, 100000));
	BroadPhase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	btCollisionDispatcher* Dispatcher = new btCollisionDispatcher(CollisionConfiguration);
	btSequentialImpulseConstraintSolver* Solver = new btSequentialImpulseConstraintSolver();
	CPhysics::World = new btDiscreteDynamicsWorld(Dispatcher, BroadPhase, Solver, CollisionConfiguration);
	CPhysics::World->setGravity(Gravity);
	CPhysics::Gravity = Gravity;
	CPhysics::smgr = smgr;
	CPhysics::driver = driver;
	CPhysics::Timer = Timer;
	CPhysics::TimeStamp = Timer->getTime();
	CPhysics::DeltaTime = 0;

	btTriangleMesh* indexVertexArrays = CPhysics::ConvertIrrMeshToBulletTriangleMesh(Map->getMesh(),Map->getScale());
	btBvhTriangleMeshShape* trimeshShape = new btBvhTriangleMeshShape(indexVertexArrays, true);

	btQuaternion quat;
	quat.setEulerZYX(0,0,0);
	btTransform Transform2;
	Transform2.setIdentity();
	Transform2.setOrigin(btVector3(0,0,0));
	Transform2.setRotation(quat);

	btDefaultMotionState *MotionState2 = new btDefaultMotionState(Transform2);

	// Create the rigid body object
	btRigidBody *RigidBody = new btRigidBody(0, MotionState2, trimeshShape);

	// Store a pointer to the irrlicht node so we can update it later
	RigidBody->setUserPointer((void *)(Map->getNode()));

	// Add it to the world
	CPhysics::World->addRigidBody(RigidBody);
	CPhysics::Objects.push_back(RigidBody);

	RigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);
}

void CPhysics::loadMapObjects()
{
    IrrXMLReader* xml = createIrrXMLReader(CPhysics::fulldir.c_str());

    if(xml)
        if(DEBUG_CONSOLE)
            cout<<"Loading Map Objects for "<<fulldir.c_str()<<endl;

    while(xml && xml->read())
    {
        switch(xml->getNodeType())
        {
            case EXN_ELEMENT:
            {
				if(!strcmp("Stack", xml->getNodeName()))
				{
				    int yshift, xshift;
                    float x,y,z,space,size,Ul,Uy,m;
                    std::string tex,axis;
                    tex = "./Media/Textures/Stacks/";

                    btVector3 Position;

                    x = xml->getAttributeValueAsFloat("PosX");
                    y = xml->getAttributeValueAsFloat("PosY");
                    z = xml->getAttributeValueAsFloat("PosZ");
                    space = xml->getAttributeValueAsFloat("Spacing");
                    size = xml->getAttributeValueAsFloat("Size");
                    Ul= xml->getAttributeValueAsFloat("UnitsL");
                    Uy= xml->getAttributeValueAsFloat("UnitsY");
                    axis= xml->getAttributeValue("Axis");
                    m = xml->getAttributeValueAsFloat("Mass");
                    tex += xml->getAttributeValue("Tex");

                    ITexture* texture = driver->getTexture(tex.c_str());

                    if(DEBUG_CONSOLE)
                        cout<<"Stack at (X:"<<x<<" Y:"<<y<<" Z:"<<z<<")"<<endl;

                    for(yshift=0;yshift<=Uy;yshift++)
                        for(xshift=0;xshift<=Ul;xshift++)
                        {
                            if(axis == "X")
                                Position = btVector3(x+(size*xshift+space),y+(size*yshift+space),z);
                            if(axis == "Z")
                                Position = btVector3(x,y+(size*yshift+space),z+(size*xshift+space));
                            btRigidBody* DummyRigidBody = CPhysics::CreateBox(Position,vector3df(size),m);
                            static_cast<ISceneNode *>(DummyRigidBody->getUserPointer())->setMaterialTexture(0, texture);
                        }
                }
            }
            break;
        }
    };
    delete xml;
}

void CPhysics::setFullDir(core::stringc fulldir)
{
    CPhysics::fulldir = fulldir;
}

void CPhysics::Update()
{
    CPhysics::DeltaTime = CPhysics::Timer->getTime() - CPhysics::TimeStamp;
    CPhysics::TimeStamp = CPhysics::Timer->getTime();
    CPhysics::World->stepSimulation(CPhysics::DeltaTime * 0.001f, 20);
    for(list<btRigidBody *>::Iterator Iterator = CPhysics::Objects.begin(); Iterator != CPhysics::Objects.end();)
        if(!CPhysics::UpdateRender(*Iterator))
        {
            Iterator = CPhysics::Objects.erase(Iterator);
        }
        else
        {
            Iterator++;
        }
}

bool CPhysics::UpdateRender(btRigidBody* TObject)
{
    ISceneNode* Node = static_cast<ISceneNode *>(TObject->getUserPointer());

    if(Node == NULL)
    {
        return false;
    }
    // Set position
    const btVector3& Point = TObject->getCenterOfMassPosition();
    Node->setPosition(vector3df((f32)Point[0], (f32)Point[1], (f32)Point[2]));

    // Set rotation
    btVector3 EulerRotation;
    CPhysics::QuaternionToEuler(TObject->getOrientation(), EulerRotation);
    Node->setRotation(vector3df(EulerRotation[0], EulerRotation[1], EulerRotation[2]));
    return true;
}

btRigidBody* CPhysics::CreateBox(const btVector3 &TPosition, const vector3df &TScale, btScalar TMass)
{
    IMeshSceneNode* Node = CPhysics::smgr->addCubeSceneNode(1.0f);
	Node->setScale(TScale);
	Node->setMaterialFlag(EMF_LIGHTING, LIGHTING);
	Node->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	Node->setMaterialTexture(0, driver->getTexture("rust0.jpg"));
	//Node->setPosition(vector3df((f32)TPosition.getX(), (f32)TPosition.getY(), (f32)TPosition.getZ()));

	btTransform Transform;
	Transform.setIdentity();
	Transform.setOrigin(TPosition);

	btDefaultMotionState *MotionState = new btDefaultMotionState(Transform);

	btVector3 HalfExtents(TScale.X * 0.5f, TScale.Y * 0.5f, TScale.Z * 0.5f);
	btCollisionShape *Shape = new btBoxShape(HalfExtents);

	btVector3 LocalInertia;
	Shape->calculateLocalInertia(TMass, LocalInertia);

	btRigidBody *RigidBody = new btRigidBody(TMass, MotionState, Shape, LocalInertia);

	RigidBody->setUserPointer((void *)(Node));
	RigidBody->setActivationState(ISLAND_SLEEPING);

	CPhysics::World->addRigidBody(RigidBody);
	CPhysics::Objects.push_back(RigidBody);

    //RigidBody->getBroadphaseProxy()->m_collisionFilterMask = btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter;

	return RigidBody;
}

btTriangleMesh* CPhysics::ConvertIrrMeshToBulletTriangleMesh(IMesh* pMesh,const vector3df& scaling)
{
  btVector3 vertices[3];
  u32 i,j,k,index,numVertices,numIndices;
  u16* mb_indices;
  btTriangleMesh *pTriMesh = new btTriangleMesh();
  for (i=0; i<pMesh->getMeshBufferCount(); i++)
  {
    IMeshBuffer* mb=pMesh->getMeshBuffer(i);
    if(mb->getVertexType()==EVT_STANDARD)
    {
      S3DVertex* mb_vertices=(S3DVertex*)mb->getVertices();
      mb_indices = mb->getIndices();
      numVertices = mb->getVertexCount();
      numIndices = mb->getIndexCount();
      for(j=0;j<numIndices;j+=3)
      {
        for (k=0;k<3;k++)
        {
          index = mb_indices[j+k];
          vertices[k] = btVector3(mb_vertices[index].Pos.X*scaling.X, mb_vertices[index].Pos.Y*scaling.Y, mb_vertices[index].Pos.Z*scaling.Z);
        }
        pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
      }
    }
    else if(mb->getVertexType()==EVT_2TCOORDS)
    {
      S3DVertex2TCoords* mb_vertices=(S3DVertex2TCoords*)mb->getVertices();
      mb_indices = mb->getIndices();
      numVertices = mb->getVertexCount();
      numIndices = mb->getIndexCount();
      for(j=0;j<numIndices;j+=3)
      {
        for (k=0;k<3;k++)
        {
          index = mb_indices[j+k];
          vertices[k] = btVector3(mb_vertices[index].Pos.X*scaling.X, mb_vertices[index].Pos.Y*scaling.Y, mb_vertices[index].Pos.Z*scaling.Z);
        }
        pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
      }
    }
  }
  return pTriMesh;
};

void CPhysics::QuaternionToEuler(const btQuaternion &TQuat, btVector3 &TEuler)
{
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
	TEuler *= RADTODEG;
};

btRigidBody* CPhysics::CreateConvex(IAnimatedMeshSceneNode* Node, const btVector3 &TPosition, const vector3df &TScale, btScalar TMass)
{
    btTriangleMesh* trimesh = CPhysics::ConvertIrrMeshToBulletTriangleMesh(Node->getMesh(), Node->getScale());

    btConvexShape* hull = new btConvexTriangleMeshShape(trimesh);
    hull->setUserPointer(hull);

    btVector3 localInertia(0,0,0);
    hull->calculateLocalInertia(TMass, localInertia);

	btQuaternion quat;
	quat.setEulerZYX(Node->getRotation().X,Node->getRotation().Y,Node->getRotation().Z);
	btTransform Transform2;
	Transform2.setIdentity();
	Transform2.setOrigin(TPosition);
	Transform2.setRotation(quat);

	btDefaultMotionState *MotionState2 = new btDefaultMotionState(Transform2);

	btRigidBody* RigidBody = new btRigidBody(TMass, MotionState2, hull, localInertia);

	RigidBody->setUserPointer((void *)(Node));
	RigidBody->setActivationState(DISABLE_DEACTIVATION);

	CPhysics::World->addRigidBody(RigidBody);
	CPhysics::Objects.push_back(RigidBody);

	RigidBody->getBroadphaseProxy()->m_collisionFilterMask = btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter;

	return RigidBody;
};

btRigidBody* CPhysics::loadConvex(std::string filename, const btVector3 &TPosition, const vector3df &TScale, btScalar TMass)
{
    IAnimatedMeshSceneNode* Node = CPhysics::smgr->addAnimatedMeshSceneNode((IAnimatedMesh*)CPhysics::smgr->getMesh(filename.c_str()));
    btTriangleMesh* trimesh = CPhysics::ConvertIrrMeshToBulletTriangleMesh(Node->getMesh(), Node->getScale());

    btConvexShape* hull = new btConvexTriangleMeshShape(trimesh);
    hull->setUserPointer(hull);

    btVector3 localInertia(0,0,0);
    hull->calculateLocalInertia(TMass, localInertia);

	btQuaternion quat;
	quat.setEulerZYX(Node->getRotation().X,Node->getRotation().Y,Node->getRotation().Z);
	btTransform Transform2;
	Transform2.setIdentity();
	Transform2.setOrigin(TPosition);
	Transform2.setRotation(quat);

	btDefaultMotionState *MotionState2 = new btDefaultMotionState(Transform2);

	btRigidBody* RigidBody = new btRigidBody(TMass, MotionState2, hull, localInertia);

	RigidBody->setUserPointer((void *)(Node));
	RigidBody->setActivationState(DISABLE_DEACTIVATION);

	CPhysics::World->addRigidBody(RigidBody);
	CPhysics::Objects.push_back(RigidBody);

	return RigidBody;
};

list<btRigidBody *> CPhysics::getObjectList()
{
    return CPhysics::Objects;
}

btDiscreteDynamicsWorld* CPhysics::getWorld()
{
    return CPhysics::World;
}

void CPhysics::remove()
{
    for(list<btRigidBody *>::Iterator Iterator = CPhysics::Objects.begin(); Iterator != CPhysics::Objects.end();)
    {
        CPhysics::World->removeRigidBody((*Iterator));
        if((*Iterator)->getUserPointer()!=NULL)
            static_cast<ISceneNode *>((*Iterator)->getUserPointer())->remove();
        Iterator = CPhysics::Objects.erase(Iterator);
    }
    CPhysics::World->~btDiscreteDynamicsWorld();

    if(DEBUG_CONSOLE)
        cout<<"Cleaned Physics World"<<endl;
}
