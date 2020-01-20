  #include <irrlicht.h>

  #include <CIrrOdeSceneNodeFactory.h>

  #include <geom/CIrrOdeGeomHeightfield.h>
  #include <geom/CIrrOdeGeomCylinder.h>
  #include <geom/CIrrOdeGeomCapsule.h>
  #include <geom/CIrrOdeGeomTrimesh.h>
  #include <geom/CIrrOdeGeomSphere.h>
  #include <geom/CIrrOdeGeomPlane.h>
  #include <geom/CIrrOdeGeomRay.h>
  #include <geom/CIrrOdeGeomBox.h>

  #include <joints/CIrrOdeJointBallSocket.h>
  #include <joints/CIrrOdeJointHinge2.h>
  #include <joints/CIrrOdeJointPiston.h>
  #include <joints/CIrrOdeJointSlider.h>
  #include <joints/CIrrOdeJointFixed.h>
  #include <joints/CIrrOdeJointHinge.h>
  #include <joints/CIrrOdeJointPR.h>

  #include <motors/CIrrOdeMotor.h>
  #include <motors/CIrrOdeServo.h>
  #include <motors/CIrrOdeImpulseMotor.h>
  #include <motors/CIrrOdeTorqueMotor.h>
  #include <motors/CIrrOdeAeroDrag.h>

  #include <CIrrOdeSpace.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeBody.h>

namespace irr {
namespace ode {

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

CIrrOdeSceneNodeFactory::CIrrOdeSceneNodeFactory(ISceneManager *pManager) {
  m_pManager=pManager;

  m_cNames.push_back(stringc(IRR_ODE_BODY_NAME             )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_BODY_ID             );
  m_cNames.push_back(stringc(IRR_ODE_GEOM_BOX_NAME         )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_GEOM_BOX_ID         );
  m_cNames.push_back(stringc(IRR_ODE_GEOM_CAPSULE_NAME     )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_GEOM_CAPSULE_ID     );
  m_cNames.push_back(stringc(IRR_ODE_GEOM_CYLINDER_NAME    )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_GEOM_CYLINDER_ID    );
  m_cNames.push_back(stringc(IRR_ODE_GEOM_HEIGHTFIELD_NAME )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_GEOM_HEIGHTFIELD_ID );
  m_cNames.push_back(stringc(IRR_ODE_GEOM_PLANE_NAME       )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_GEOM_PLANE_ID       );
  m_cNames.push_back(stringc(IRR_ODE_GEOM_RAY_NAME         )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_GEOM_RAY_ID         );
  m_cNames.push_back(stringc(IRR_ODE_GEOM_SPHERE_NAME      )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_GEOM_SPHERE_ID      );
  m_cNames.push_back(stringc(IRR_ODE_GEOM_TRIMESH_NAME     )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_GEOM_TRIMESH_ID     );
  m_cNames.push_back(stringc(IRR_ODE_JOINT_BALL_SOCKET_NAME)); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_JOINT_BALL_SOCKET_ID);
  m_cNames.push_back(stringc(IRR_ODE_JOINT_FIXED_NAME      )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_JOINT_FIXED_ID      );
  m_cNames.push_back(stringc(IRR_ODE_JOINT_HINGE_NAME      )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_JOINT_HINGE_ID      );
  m_cNames.push_back(stringc(IRR_ODE_JOINT_HINGE2_NAME     )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_JOINT_HINGE2_ID     );
  m_cNames.push_back(stringc(IRR_ODE_JOINT_PISTON_NAME     )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_JOINT_PISTON_ID     );
  m_cNames.push_back(stringc(IRR_ODE_JOINT_SLIDER_NAME     )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_JOINT_SLIDER_ID     );
  m_cNames.push_back(stringc(IRR_ODE_SPACE_NAME            )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_SPACE_ID            );
  m_cNames.push_back(stringc(IRR_ODE_WORLD_NAME            )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_WORLD_ID            );
  m_cNames.push_back(stringc(IRR_ODE_MOTOR_NAME            )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_MOTOR_ID            );
  m_cNames.push_back(stringc(IRR_ODE_SERVO_NAME            )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_SERVO_ID            );
  m_cNames.push_back(stringc(IRR_ODE_IMPULSE_MOTOR_NAME    )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_IMPULSE_MOTOR_ID    );
  m_cNames.push_back(stringc(IRR_ODE_TORQUE_MOTOR_NAME     )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_TORQUE_MOTOR_ID     );
  m_cNames.push_back(stringc(IRR_ODE_AERO_DRAG_NAME        )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_AERO_DRAG_ID        );
  m_cNames.push_back(stringc(IRR_ODE_JOINT_PR_NAME         )); m_cTypes.push_back((ESCENE_NODE_TYPE)IRR_ODE_JOINT_PR_ID         );

  #ifdef _TRACE_CONSTRUCTOR_DESTRUCTOR
    for (u32 i=0; i<m_cNames.size(); i++)
      printf("CIrrOdeSceneNodeFactory constructor: %i (%i): \"%s\"\n",i,m_cTypes[i],m_cNames[i].c_str());
  #endif
}

ISceneNode *CIrrOdeSceneNodeFactory::addSceneNode (ESCENE_NODE_TYPE type, ISceneNode *parent) {
  CIrrOdeSceneNode *pRet=NULL;
  if (!parent) parent=m_pManager->getRootSceneNode();
  switch (type) {
    case IRR_ODE_BODY_ID             : pRet=new CIrrOdeBody           (parent,m_pManager); break;
    case IRR_ODE_GEOM_BOX_ID         : pRet=new CIrrOdeGeomBox        (parent,m_pManager); break;
    case IRR_ODE_GEOM_CAPSULE_ID     : pRet=new CIrrOdeGeomCapsule    (parent,m_pManager); break;
    case IRR_ODE_GEOM_CYLINDER_ID    : pRet=new CIrrOdeGeomCylinder   (parent,m_pManager); break;
    case IRR_ODE_GEOM_HEIGHTFIELD_ID : pRet=new CIrrOdeGeomHeightfield(parent,m_pManager); break;
    case IRR_ODE_GEOM_PLANE_ID       : pRet=new CIrrOdeGeomPlane      (parent,m_pManager); break;
    case IRR_ODE_GEOM_RAY_ID         : pRet=new CIrrOdeGeomRay        (parent,m_pManager); break;
    case IRR_ODE_GEOM_SPHERE_ID      : pRet=new CIrrOdeGeomSphere     (parent,m_pManager); break;
    case IRR_ODE_GEOM_TRIMESH_ID     : pRet=new CIrrOdeGeomTrimesh    (parent,m_pManager); break;
    case IRR_ODE_JOINT_BALL_SOCKET_ID: pRet=new CIrrOdeJointBallSocket(parent,m_pManager); break;
    case IRR_ODE_JOINT_FIXED_ID      : pRet=new CIrrOdeJointFixed     (parent,m_pManager); break;
    case IRR_ODE_JOINT_HINGE_ID      : pRet=new CIrrOdeJointHinge     (parent,m_pManager); break;
    case IRR_ODE_JOINT_HINGE2_ID     : pRet=new CIrrOdeJointHinge2    (parent,m_pManager); break;
    case IRR_ODE_JOINT_PISTON_ID     : pRet=new CIrrOdeJointPiston    (parent,m_pManager); break;
    case IRR_ODE_JOINT_SLIDER_ID     : pRet=new CIrrOdeJointSlider    (parent,m_pManager); break;
    case IRR_ODE_SPACE_ID            : pRet=new CIrrOdeSpace          (parent,m_pManager); break;
    case IRR_ODE_WORLD_ID            : pRet=new CIrrOdeWorld          (parent,m_pManager); break;
    case IRR_ODE_MOTOR_ID            : pRet=new CIrrOdeMotor          (parent,m_pManager); break;
    case IRR_ODE_SERVO_ID            : pRet=new CIrrOdeServo          (parent,m_pManager); break;
    case IRR_ODE_IMPULSE_MOTOR_ID    : pRet=new CIrrOdeImpulseMotor   (parent,m_pManager); break;
    case IRR_ODE_TORQUE_MOTOR_ID     : pRet=new CIrrOdeTorqueMotor    (parent,m_pManager); break;
    case IRR_ODE_AERO_DRAG_ID        : pRet=new CIrrOdeAeroDrag       (parent,m_pManager); break;
    case IRR_ODE_JOINT_PR_ID         : pRet=new CIrrOdeJointPR        (parent,m_pManager); break;

    default: break;
  }

  #ifdef _TRACE_FACTORY
    stringc sName(pRet->getTypeName());
    printf("CIrrOdeSceneNodeFactory::addSceneNode(%i): \"%s\"\n",(int)type,sName.c_str());
  #endif

  return pRet;
}

ISceneNode *CIrrOdeSceneNodeFactory::addSceneNode (const c8 *typeName, ISceneNode *parent) {
  stringc sName=stringc(typeName);
  for (u32 i=0; i<m_cNames.size(); i++)
    if (m_cNames[i]==sName) {
      #ifdef _TRACE_FACTORY
        printf("CIrrOdeSceneNodeFactory::addSceneNode(\"%s\"): \"%s\"\n",typeName,m_cNames[i].c_str());
      #endif
      return addSceneNode(m_cTypes[i],parent);
    }
  return NULL;
}

u32 CIrrOdeSceneNodeFactory::getCreatableSceneNodeTypeCount() const {
  return m_cNames.size();
}

ESCENE_NODE_TYPE CIrrOdeSceneNodeFactory::getCreateableSceneNodeType (u32 idx) const {
  ESCENE_NODE_TYPE iRet=idx<m_cTypes.size()?m_cTypes[idx]:ESNT_UNKNOWN;
  #ifdef _TRACE_FACTORY
    printf("CIrrOdeSceneNodeFactory::getCreateableSceneNodeType(%i): %i\n",idx,iRet);
  #endif
  return iRet;
}

const c8 *CIrrOdeSceneNodeFactory::getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const {
  for (u32 i=0; i<m_cTypes.size(); i++)
    if (m_cTypes[i]==type) {
      #ifdef _TRACE_FACTORY
        printf("CIrrOdeSceneNodeFactory::getCreateableSceneNodeTypeName(%i): \"%s\"\n",type,m_cNames[i].c_str());
      #endif
      return m_cNames[i].c_str();
    }

  return NULL;
}

const c8 *CIrrOdeSceneNodeFactory::getCreateableSceneNodeTypeName (u32 idx) const {
  const c8 *pRet=idx<m_cNames.size()?m_cNames[idx].c_str():NULL;
  #ifdef _TRACE_FACTORY
    printf("CIrrOdeSceneNodeFactory::getCreateableSceneNodeTypeName(%i): \"%s\"\n",idx,pRet?pRet:"NULL");
  #endif
  return pRet;
}

CIrrOdeSceneNodeFactory::~CIrrOdeSceneNodeFactory () {
}

} //namespace ode
} //namespace irr
