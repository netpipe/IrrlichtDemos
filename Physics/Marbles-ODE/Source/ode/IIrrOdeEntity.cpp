  #include "IIrrOdeEntity.h"

using namespace irr;
using namespace ode;

IIrrOdeEntity::IIrrOdeEntity(dWorldID iWorld, dSpaceID iSpace, scene::IMesh *pMesh, scene::IMeshSceneNode *pNode)
{
  m_pData=NULL;

  m_iWorld=iWorld;
  m_iSpace=iSpace;
  m_iBody =0;
  m_iGeom =0;

  m_pMesh=pMesh;
  m_pNode=pNode;
}

IIrrOdeEntity::~IIrrOdeEntity()
{
  if (m_iBody) dBodyDestroy(m_iBody);
  if (m_iGeom) dGeomDestroy(m_iGeom);

  if (m_pIndices ) delete [] m_pIndices ;
  if (m_pVertices) delete [] m_pVertices;
}

void IIrrOdeEntity::QuaternionToEuler(const dQuaternion quaternion, irr::core::vector3df &euler)
{
dReal w,x,y,z;

double sqw;
double sqx;
double sqy;
double sqz;

  w=quaternion[0];
  x=quaternion[1];
  y=quaternion[2];
  z=quaternion[3];

  sqw = w*w;
  sqx = x*x;
  sqy = y*y;
  sqz = z*z;

  euler.Z = (irr::f32) (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))*GRAD_PI);
  euler.X = (irr::f32) (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))*GRAD_PI);
  euler.Y = (irr::f32) (asin(-2.0 * (x*z - y*w))*GRAD_PI);
}

void IIrrOdeEntity::updateBeforePhysics()
{
  if (m_iGeom!=0)
  {
    dReal ode_pos[3];

    ode_pos[0]=m_pNode->getPosition().X;
    ode_pos[1]=m_pNode->getPosition().Y;
    ode_pos[2]=m_pNode->getPosition().Z;

    dGeomSetPosition(m_iGeom,ode_pos[0],ode_pos[1],ode_pos[2]);
  }
}

void IIrrOdeEntity::updateAfterPhysics()
{
core::vector3df pos;
core::vector3df rot;

  if (m_iGeom!=0)
  {
    // get the new position of the ODE geometry
    dReal* ode_pos=(dReal*)dGeomGetPosition(m_iGeom);

    // set the position at the scenenode
    pos.set((irr::f32)ode_pos[0],(irr::f32)ode_pos[1],(irr::f32)ode_pos[2]);
    m_pNode->setPosition(pos);

    // get the rotation quaternion
    dQuaternion result;
    dGeomGetQuaternion(m_iGeom, result);
    // convert it to eulerangles
    QuaternionToEuler(result,rot);
    // set the rotation
    m_pNode->setRotation(rot);
  }
}

void IIrrOdeEntity::setSphereGeomData()
{
f32 fRad;

   irr::core::aabbox3d<irr::f32> box=m_pNode->getBoundingBox();
   irr::core::vector3df extend=box.getExtent();
   irr::core::vector3df pos=m_pNode->getPosition();

   fRad=(dReal)extend.X/2;

   m_iGeom=dCreateSphere(m_iSpace,fRad);
   dGeomSetData(m_iGeom,(void*)this);
   dGeomSetPosition(m_iGeom,pos.X,pos.Y,pos.Z);
   // create a body for this object
   m_iBody=dBodyCreate(m_iWorld);
   // set mass shape
   dMassSetBox(&m_dMass,1.0f,(dReal)extend.X,(dReal)extend.Y,(dReal)extend.Z);
   // adjust mass weight
   dMassAdjust(&m_dMass,0.25);
   // make the body have a mass
   dBodySetMass(m_iBody,&m_dMass);
   // add the body to the geom
   dGeomSetBody(m_iGeom,m_iBody);
   dBodySetPosition(m_iBody,pos.X,pos.Y,pos.Z);
   dBodySetData(m_iBody,(void*)this);
}

void IIrrOdeEntity::setPosition(irr::core::vector3df cNewPosition)
{
  m_pNode->setPosition(cNewPosition);
  if (m_iBody) dBodySetPosition(m_iBody,cNewPosition.X,cNewPosition.Y,cNewPosition.Z);
}

void IIrrOdeEntity::setForce(core::vector3df cNewForce)
{
  if (m_iBody) dBodySetForce(m_iBody,cNewForce.X,cNewForce.Y,cNewForce.Z);
}

void IIrrOdeEntity::setTorque(core::vector3df cNewTorque)
{
const dReal *pTorque;

  if (m_iBody)
  {
    pTorque=dBodyGetTorque(m_iBody);
    dBodyAddTorque(m_iBody,cNewTorque.X-pTorque[0],cNewTorque.Y-pTorque[1],cNewTorque.Z-pTorque[2]);
  }
}

void IIrrOdeEntity::setVelocity(core::vector3df cNewVelocity)
{
  if (m_iBody) dBodySetLinearVel(m_iBody,cNewVelocity.X,cNewVelocity.Y,cNewVelocity.Z);
}

void IIrrOdeEntity::setGeomData(void)
{
  if(m_pMesh==NULL || m_pNode==NULL) return;

  unsigned int i,j,ci,cif,cv;
  m_iIndexCount=0;
  m_iVertexCount=0;

  // count vertices and indices
  for(i=0; i<m_pMesh->getMeshBufferCount(); i++)
  {
    irr::scene::IMeshBuffer* mb=m_pMesh->getMeshBuffer(i);
    m_iIndexCount+=mb->getIndexCount();
    m_iVertexCount+=mb->getVertexCount();
  }

  // build structure for ode trimesh geom
  m_pVertices=new dVector3[m_iVertexCount];
  m_pIndices=new int[m_iIndexCount];

  // fill trimesh geom
  ci=0; // current index in the indices array
  cif=0; // offset of the irrlicht-vertex-index in the vetices array
  cv=0; // current index in the vertices array

  for(i=0; i<m_pMesh->getMeshBufferCount(); i++)
  {
    irr::scene::IMeshBuffer* mb=m_pMesh->getMeshBuffer(i);

    irr::u16* mb_indices=mb->getIndices();

    for(j=0; j<mb->getIndexCount(); j++)
    {
      m_pIndices[ci]=cif+mb_indices[j];
      ci++;
    }

    cif=cif+mb->getVertexCount();

    if(mb->getVertexType()==irr::video::EVT_STANDARD)
    {
     irr::video::S3DVertex* mb_vertices=(irr::video::S3DVertex*)mb->getVertices();

     for(j=0;j<mb->getVertexCount();j++)
     {
       m_pVertices[cv][0]=mb_vertices[j].Pos.X*m_pNode->getScale().X;
       m_pVertices[cv][1]=mb_vertices[j].Pos.Y*m_pNode->getScale().Y;
       m_pVertices[cv][2]=mb_vertices[j].Pos.Z*m_pNode->getScale().Z;
       cv++;
     }
   }
   else
     if(mb->getVertexType()==irr::video::EVT_2TCOORDS)
     {
       irr::video::S3DVertex2TCoords* mb_vertices=(irr::video::S3DVertex2TCoords*)mb->getVertices();

       for(j=0;j<mb->getVertexCount();j++)
       {
         m_pVertices[cv][0]=mb_vertices[j].Pos.X*m_pNode->getScale().X;
         m_pVertices[cv][1]=mb_vertices[j].Pos.Y*m_pNode->getScale().Y;
         m_pVertices[cv][2]=mb_vertices[j].Pos.Z*m_pNode->getScale().Z;
         cv++;
       }
     }
  }
  irr::core::vector3df pos=m_pNode->getPosition();

  // build the trimesh data
  dTriMeshDataID data=dGeomTriMeshDataCreate();
  dGeomTriMeshDataBuildSimple(data,(dReal*)m_pVertices,m_iVertexCount, m_pIndices, m_iIndexCount);

  // build the trimesh geom
  m_iGeom=dCreateTriMesh(m_iSpace,data,0,0,0);

  // set the geom position
  dGeomSetPosition(m_iGeom,pos.X,pos.Y,pos.Z);
  // lets have a pointer to our bounceable
  // we could need this in the collision callback
  dGeomSetData(m_iGeom,(void*)this);
  dGeomSetBody(m_iGeom,m_iBody);
}

void IIrrOdeEntity::setAngularVelocity(core::vector3df cNewVelocity)
{
  dBodySetAngularVel(m_iBody,cNewVelocity.X,cNewVelocity.Y,cNewVelocity.Z);
}

void IIrrOdeEntity::setMass(irr::f32 fMass)
{
  m_fMass=fMass;
  if (m_iBody) updateMass();
}

void IIrrOdeEntity::updateMass()
{
const dReal *pos;

  if (m_fMass<=0.0f) return;

  if (!m_iBody)
  {
    m_iBody=dBodyCreate(m_iWorld);
    dMassSetBox(&m_dMass,m_fMass,1.0f,1.0f,1.0f);
    dBodySetMass(m_iBody,&m_dMass);
    dBodySetData(m_iBody,this);
    pos=dGeomGetPosition(m_iGeom);
    dBodySetPosition(m_iBody,pos[0],pos[1],pos[2]);
    dGeomSetBody(m_iGeom,m_iBody);
  }

  printf("Mass Updated: %.2f\n",m_fMass);
}

irr::f32 IIrrOdeEntity::getMass()
{
  return m_fMass;
}
