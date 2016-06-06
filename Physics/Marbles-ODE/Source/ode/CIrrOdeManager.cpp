  #include "CIrrOdeManager.h"

using namespace irr;
using namespace ode;

dWorldID CIrrOdeManager::m_iWorld=0;
dSpaceID CIrrOdeManager::m_iSpace=0;

dJointGroupID CIrrOdeManager::m_iJointGroup=0;

CIrrOdeManager::CIrrOdeManager(IrrlichtDevice *pDevice) : IIrrOdeBase()
{
  m_pDevice=pDevice;

	m_iWorld=dWorldCreate();
	m_iSpace=dSimpleSpaceCreate(0);
	m_iJointGroup=dJointGroupCreate(0);
	dWorldSetGravity(m_iWorld,0,-5,0);
	m_iObjCnt=0;

	setName("CIrrOdeManager");
}

CIrrOdeManager::~CIrrOdeManager()
{
	for(int i=0; i<m_iObjCnt; i++) if (m_cObjects[i]) delete m_cObjects[i];

	m_cObjects.clear();

	dJointGroupDestroy(m_iJointGroup);
	dSpaceDestroy(m_iSpace);
	dWorldDestroy(m_iWorld);
	dCloseODE();
}

void CIrrOdeManager::setGravity(core::vector3df &cGravity)
{
  dWorldSetGravity(m_iWorld,cGravity.X,cGravity.Y,cGravity.Z);
}

void CIrrOdeManager::updateBeforePhysics()
{
}

void CIrrOdeManager::updateAfterPhysics()
{
  for(int i=0; i<m_iObjCnt; i++) m_cObjects[i]->updateAfterPhysics();
}

void CIrrOdeManager::worldStep(irr::f32 fTimeDiff)
{
int i=0,cnt=(int)(fTimeDiff*100);

  while (cnt>0)
  {
    dSpaceCollide(m_iSpace,0,&CIrrOdeManager::nearCollisionCallback);
    //dWorldStep(m_iWorld,fTimeDiff);
    dWorldStepFast1(m_iWorld,0.015,5);
    dJointGroupEmpty(m_iJointGroup);
    cnt--;
  }
  updateAfterPhysics();
}

void CIrrOdeManager::nearCollisionCallback(void* data, dGeomID o1, dGeomID o2)
{
int i=0;
IIrrOdeEntity *obj1,*obj2;
CObjData      *dat1,*dat2;

const dReal *pVelocity;
dReal rDummy;

  dBodyID b1=dGeomGetBody(o1);
  dBodyID b2=dGeomGetBody(o2);

  if (b1 && b2 && dAreConnectedExcluding(b1,b2,dJointTypeContact)) return;

  obj1=(IIrrOdeEntity *)dGeomGetData(o1);
  obj2=(IIrrOdeEntity *)dGeomGetData(o2);

  dat1=obj1->getData();
  dat2=obj2->getData();

  dContact contact[MAX_CONTACTS];
  for(i=0; i<MAX_CONTACTS; i++)
  {
    contact[i].surface.mode=dContactBounce | dContactSoftERP | dContactSoftCFM | dContactSlip1 | dContactSlip2;
    contact[i].surface.mu=dInfinity;
    contact[i].surface.mu2=0;
    contact[i].surface.bounce=(dReal)0.6;
    contact[i].surface.bounce_vel=1e-9f; //1.5f;//
    contact[i].surface.soft_cfm=0.005f;//1e-6f;
    contact[i].surface.soft_erp=0.00005f;

    contact[i].surface.slip1=0.1f;
    contact[i].surface.slip2=0.1f;

    rDummy=1.0f;

    if (dat1) rDummy+=dat1->m_fSlip1;
    if (dat2) rDummy+=dat2->m_fSlip1;

    contact[i].surface.slip1=rDummy;

    rDummy=1.0f;

    if (dat1) rDummy+=dat1->m_fSlip2;
    if (dat2) rDummy+=dat2->m_fSlip2;

    contact[i].surface.slip2=rDummy;
  }

  int numc=dCollide(o1,o2,MAX_CONTACTS,&contact[0].geom,sizeof(dContact));

  if(numc>0)
  {
    dJointFeedback pFeedback[MAX_CONTACTS];
    dJointID pJoints[MAX_CONTACTS];

    for(i=0; i<numc; i++)
    {
      pJoints[i]=dJointCreateContact(m_iWorld,m_iJointGroup,&contact[i]);
      dJointAttach(pJoints[i],b1,b2);
      dJointSetFeedback(pJoints[i],&pFeedback[i]);
    }

    pVelocity=NULL;

    obj1->collisionCallback(obj2,numc,pFeedback);
    obj2->collisionCallback(obj1,numc,pFeedback);
  }
}

void CIrrOdeManager::addEntity(IIrrOdeEntity *pEntity)
{
  m_cObjects.push_back(pEntity);
  m_iObjCnt++;
}
