#ifndef _IRR_ODE_DEVICE
  #define _IRR_ODE_DEVICE

  #ifndef _USE_ODE_NULL_DEVICE

  #include <IIrrOdeDevice.h>
  #include <ode/ode.h>

namespace irr {
namespace ode {

using namespace core;

/**
 * @class COdeDataWrapper
 * This class is just a data structure that holds all ODE specific data. It is used to hide the ODE data from the
 * rest of IrrODE, so that it is possible to remove all ODE stuff by changing a single class
 */
class COdeDataWrapper {
  public:
    COdeDataWrapper();
    ~COdeDataWrapper();

    dWorldID       m_pWorld;    /*!< a world id */
    dJointID       m_pJoint;    /*!< a joint id */
    dBodyID        m_pBody;     /*!< a body id */
    dGeomID        m_pGeom;     /*!< a geom id */
    dMass          m_pMass;     /*!< a mass class */
    dSpaceID       m_pSpace;    /*!< a space id */
    dTriMeshDataID m_pData;     /*!< a trimesh data id */
    dJointGroupID  m_pJGroup;   /*!< a jointground id */

    dHeightfieldDataID m_pHFData; /*!< a heightfield data id */

    dVector3 *m_pVertices;              /*!< a vertex array. Used for trimeshes */
    u32 *m_pIndices;                    /*!< an index array. Used for trimeshes */
    u32 m_iIndexCount,m_iVertexCount;   /*!< sizes of the vertex and index array. Used for trimeshes */
};

/**
 * This class is used internally to store information of fast objects in a queue for
 * processing after the step
 */
class CFastCollision {
  public:
    CIrrOdeBody *m_pBody;   /*!< the colliding body */
    CIrrOdeGeom *m_pGeom;   /*!< the geom the body collidied with */
    vector3df m_cPos;       /*!< the collision point of the bodies */
};

class CIrrOdeWorld;
class IIrrOdeEventQueue;
class IIrrOdeContactParameters;

/**
 * @class CIrrOdeDevice
 * This class is the wrapper for all ODE function calls. It can be replaced by CIrrOdeNullDevice to disable all
 * ODE functionality.
 */
class CIrrOdeDevice : public IIrrOdeDevice {
  protected:
    array<COdeDataWrapper *> m_pOdeData;
    IIrrOdeContactParameters *m_pContactCalculator;
    bool m_bOdeInitialized;

    f32 m_fStepSize,m_fStepTime;
    u32 m_iStep;

    dGeomID getGeomId(u32 iGeom);
    dBodyID getBodyId(u32 iBody);
    dJointGroupID getJGroupId(u32 iJGroup);

    static void convertToSurfaceParameters(dSurfaceParameters *pOde, CIrrOdeSurfaceParameters *pIrr);

    int convertToOdeJointParam(u16 iGroup, s32 &iParam);

    static IIrrOdeEventQueue *m_pQueue;

    void handleFastCollisions();

  public:
    CIrrOdeDevice();

    dWorldID getWorldId(int iWorld);
    virtual ~CIrrOdeDevice();

    virtual void initODE();
    virtual void closeODE();
    virtual void clearODE();

    static void nearCollisionCallback(void *pData, dGeomID iGeom1, dGeomID iGeom2);
    static void handleRayEvent(dGeomID theRay, dGeomID theGeom);

    static void quaternionToEuler(const dQuaternion q, vector3df &v);  /**< convert quaternion to euler angles */
    static void eulerToQuaternion(const vector3df v, dQuaternion q);   /**< convert euler angles to quaternion */

    static void bodyMovedCallback(dBodyID iMovedBody);                 /**< method to update Irrlicht position and rotation */

    virtual void setStepSize(f32 f);
    virtual f32 getStepSize();

    virtual void step(f32 f, CIrrOdeWorld *pWorld);
    virtual void quickStep(f32 fTime, CIrrOdeWorld *pWorld);

    virtual void setContactCalculator(IIrrOdeContactParameters *pCalculator);
    virtual IIrrOdeContactParameters *getContactCalculator();

    virtual void worldDestroy(u32 iIdx);
    virtual u32 worldCreate();

    virtual void worldSetLinearDamping(u32 iWorld, f32 fScale);
    virtual f32 worldGetLinearDamping(u32 iWorld);
    virtual void worldSetAngularDamping(u32 iWorld, f32 fScale);
    virtual f32 worldGetAngularDamping(u32 iWorld);
    virtual void worldSetLinearDampingThreshold(u32 iWorld, f32 fScale);
    virtual f32 worldGetLinearDampingThreshold(u32 iWorld);
    virtual void worldSetAngularDampingThreshold(u32 iWorld, f32 fScale);
    virtual f32 worldGetAngularDampingThreshold(u32 iWorld);
    virtual void worldSetMaxAngularSpeed(u32 iWorld, f32 fMax);
    virtual f32 worldGetMaxAngularSpeed(u32 iWorld);
    virtual void worldSetAutoDisableFlag(u32 iWorld, s32 iFlag);
    virtual s32 worldGetAutoDisableFlag(u32 iWorld);
    virtual void worldSetAutoDisableLinearThreshold(u32 iWorld, f32 f);
    virtual f32 worldGetAutoDisableLinearThreshold(u32 iWorld);
    virtual void worldSetAutoDisableAngularThreshold(u32 iWorld, f32 f);
    virtual f32 worldGetAutoDisableAngularThreshold(u32 iWorld);
    virtual void worldSetAutoDisableSteps(u32 iWorld, s32 i);
    virtual s32 worldGetAutoDisableSteps(u32 iWorld);
    virtual void worldSetAutoDisableTime(u32 iWorld, f32 f);
    virtual f32 worldGetAutoDisableTime(u32 iWorld);
    virtual void worldSetQuickStepNumIterations(u32 iWorld, s32 i);
    virtual s32 worldGetQuickStepNumIterations(u32 iWorld);
    virtual void worldSetQuickStepW(u32 iWorld, f32 f);
    virtual f32 worldGetQuickStepW(u32 iWorld);
    virtual void worldSetGravity(u32 iWorld, vector3df cGravity);
    virtual void worldSetERP(u32 iWorld, f32 fErp);
    virtual f32 worldGetERP(u32 iWorld);
    virtual void worldSetCFM(u32 iWorld, f32 fCfm);
    virtual f32 worldGetCFM(u32 iWorld);

    virtual u32 jointCreateBallSocket(u32 iWorld);
    virtual u32 jointCreateFixed(u32 iWorld);
    virtual u32 jointCreateHinge(u32 iWorld);
    virtual u32 jointCreateHinge2(u32 iWorld);
    virtual u32 jointCreatePiston(u32 iWorld);
    virtual u32 jointCreateSlider(u32 iWorld);
    virtual u32 jointCreateUniversal(u32 iWorld);
    virtual u32 jointCreatePR(u32 iWorld);

    virtual void jointAttach(u32 iJoint, CIrrOdeBody *b1, CIrrOdeBody *b2);
    virtual void jointSetData(u32 iJoint, void *pData);
    virtual void jointDestroy(u32 iJoint);

    virtual void jointSetBallAnchor(u32 iJoint, vector3df pos);
    virtual vector3df jointGetBallAnchor(u32 iJoint);
    virtual vector3df jointGetBallAnchor2(u32 iJoint);

    virtual void jointSetFixed(u32 iJoint);

    virtual void jointSetHingeAnchor(u32 iJoint, vector3df p);
    virtual void jointSetHingeAxis(u32 iJoint, vector3df a);
    virtual vector3df jointGetHingeAxis(u32 iJoint);
    virtual vector3df jointGetHingeAnchor1(u32 iJoint);
    virtual vector3df jointGetHingeAnchor2(u32 iJoint);
    virtual f32 jointGetHingeAngle(u32 iJoint);
    virtual f32 jointGetHingeAngleRate(u32 iJoint);
    virtual void jointSetHingeParam(u32 iJoint, s32 iParam, f32 iValue);
    virtual f32 jointGetHingeParam(u32 iJoint, s32 iParam);

    virtual void jointSetHinge2Anchor(u32 iJoint, vector3df a);
    virtual void jointSetHinge2Axis1(u32 iJoint, vector3df a);
    virtual void jointSetHinge2Axis2(u32 iJoint, vector3df a);
    virtual vector3df jointGetHinge2Axis1(u32 iJoint);
    virtual vector3df jointGetHinge2Axis2(u32 iJoint);
    virtual vector3df jointGetHinge2Anchor(u32 iJoint);
    virtual vector3df jointGetHinge2Anchor2(u32 iJoint);
    virtual f32 jointGetHinge2Angle1(u32 iJoint);
    virtual f32 jointGetHinge2Angle1Rate(u32 iJoint);
    virtual f32 jointGetHinge2Angle2Rate(u32 iJoint);
    virtual void jointSetHinge2Param(u32 iJoint, u32 iGroup, s32 iParam, f32 iValue);
    virtual f32 jointGetHinge2Param(u32 iJoint, u32 iGroup, s32 iParam);

    virtual void jointSetPistonAnchor(u32 iJoint, vector3df a);
    virtual void jointSetPistonAxis(u32 iJoint, vector3df a);
    virtual f32 jointGetPistonPosition(u32 iJoint);
    virtual f32 jointGetPistonPositionRate(u32 iJoint);
    virtual void jointSetPistonParam(u32 iJoint, u32 iGroup, s32 iParam, f32 fValue);
    virtual f32 jointGetPistonParam(u32 iJoint, u32 iGroup, s32 iParam);
    virtual vector3df jointGetPistonAnchor(u32 iJoint);
    virtual vector3df jointGetPistonAxis(u32 iJoint);

    virtual void jointSetSliderAxis(u32 iJoint, vector3df a);
    virtual vector3df jointGetSliderAxis(u32 iJoint);
    virtual f32 jointGetSliderPosition(u32 iJoint);
    virtual f32 jointGetSliderPositionRate(u32 iJoint);
    virtual void jointSetSliderParam(u32 iJoint, s32 iParam, f32 fValue);
    virtual f32 jointGetSliderParam(u32 iJoint, s32 iParam);

    virtual void jointSetUniversalAnchor(u32 iJoint, vector3df a);
    virtual void jointSetUniversalAxis1(u32 iJoint, vector3df a);
    virtual void jointSetUniversalAxis2(u32 iJoint, vector3df a);
    virtual vector3df jointGetUniversalAnchor(u32 iJoint);
    virtual vector3df jointGetUniversalAnchor2(u32 iJoint);
    virtual vector3df jointGetUniversalAxis1(u32 iJoint);
    virtual vector3df jointGetUniversalAxis2(u32 iJoint);
    virtual f32 jointGetUniversalAngle1(u32 iJoint);
    virtual f32 jointGetUniversalAngle2(u32 iJoint);
    virtual f32 jointGetUniversalAngle1Rate(u32 iJoint);
    virtual f32 jointGetUniversalAngle2Rate(u32 iJoint);
    virtual void jointSetUniversalParam(u32 iJoint, u32 iGroup, s32 iParam, f32 fValue);
    virtual f32 jointGetUniversalParam(u32 iJoint, u32 iGroup, s32 iParam);

    virtual void jointSetPrAxis1(u32 iJoint, vector3df a);
    virtual void jointSetPrAxis2(u32 iJoint, vector3df a);
    virtual void jointSetPrAnchor(u32 iJoint, vector3df a);
    virtual vector3df jointGetPrAxis1(u32 iJoint);
    virtual vector3df jointGetPrAxis2(u32 iJoint);
    virtual vector3df jointGetPrAnchor(u32 iJoint);
    virtual f32 jointGetPrPosition(u32 iJoint);
    virtual f32 jointGetPrAngle(u32 iJoint);
    virtual f32 jointGetPrAngleRate(u32 iJoint);
    virtual void  jointSetPrParam(u32 iJoint, s32 iParam, f32 iValue);
    virtual f32 jointGetPrParam(u32 iJoint, s32 iParam);

    virtual u32 bodyCreate(u32 iWorld);
    virtual void bodyDestroy(u32 iBody);
    virtual void bodySetPosition(u32 iBody, vector3df pos);
    virtual vector3df bodyGetPosition(u32 iBody);
    virtual vector3df bodyGetRotation(u32 iBody);
    virtual void bodySetQuaternion(u32 iBody, core::quaternion rot);
    virtual void bodySetRotation(u32 iBody, vector3df rot);
    virtual void bodySetData(u32 iBody, void *pData);
    virtual void bodySetLinearDamping(u32 iBody, f32 f);
    virtual f32 bodyGetLinearDamping(u32 iBody);
    virtual void bodySetAngularDamping(u32 iBody, f32 f);
    virtual f32 bodyGetAngularDamping(u32 iBody);
    virtual void bodySetLinearDampingThreshold(u32 iBody, f32 f);
    virtual f32 bodyGetLinearDampingThreshold(u32 iBody);
    virtual void bodySetAngularDampingThreshold(u32 iBody, f32 f);
    virtual f32 bodyGetAngularDampingThreshold(u32 iBody);
    virtual void bodySetMaxAngularSpeed(u32 iBody, f32 f);
    virtual f32 bodyGetMaxAngularSpeed(u32 iBody);
    virtual void bodySetAutoDisableFlag(u32 iBody, s32 i);
    virtual s32 bodyGetAutoDisableFlag(u32 iBody);
    virtual void bodySetLinearAutoDisableThreshold(u32 iBody, f32 f);
    virtual f32 bodyGetLinearAutoDisableThreshold(u32 iBody);
    virtual void bodySetAngularAutoDisableThreshold(u32 iBody, f32 f);
    virtual f32 bodyGetAngularAutoDisableThreshold(u32 iBody);
    virtual void bodySetAutoDisableSteps(u32 iBody, s32 i);
    virtual s32 bodyGetAutoDisableSteps(u32 iBody);
    virtual void bodySetAutoDisableTime(u32 iBody, f32 f);
    virtual f32 bodyGetAutoDisableTime(u32 iBody);
    virtual void bodySetFiniteRotationMode(u32 iBody, bool b);
    virtual bool bodyGetFiniteRotationMode(u32 iBody);
    virtual void bodySetFiniteRotationAxis(u32 iBody, vector3df v);
    virtual const vector3df bodyGetFiniteRotationAxis(u32 iBody);
    virtual void bodySetGravityMode(u32 iBody, s32 i);
    virtual s32 bodyGetGravityMode(u32 iBody);
    virtual CIrrOdeGeom *bodyGetFirstGeom(u32 iBody);
    virtual CIrrOdeGeom *bodyGetNextGeom(CIrrOdeGeom *pPrev);
    virtual bool bodyIsEnabled(u32 iBody);
    virtual void bodyAddForce(u32 iBody, vector3df f);
    virtual void bodyAddForceAtPosition(u32 iBody, vector3df f, vector3df p);
    virtual void bodyAddTorque(u32 iBody, vector3df t);
    virtual void bodySetTorque(u32 iBody, vector3df t);
    virtual void bodyEnable(u32 iBody);
    virtual void bodyDisable(u32 iBody);
    virtual void bodySetLinearVelocity(u32 iBody, vector3df v);
    virtual vector3df bodyGetLinearVelocity(u32 iBody);
    virtual void bodySetAngularVelocity(u32 iBody, vector3df v);
    virtual vector3df bodyGetAngularVelocity(u32 iBody);
    virtual vector3df bodyGetPointVel(u32 iBody, vector3df vPos);
    virtual vector3df bodyGetRelPointVel(u32 iBody, vector3df vPos);
    virtual vector3df bodyGetRelPointPos(u32 iBody, vector3df vPos);
    virtual vector3df bodyGetPosRelPoint(u32 iBody, vector3df vPos);
    virtual vector3df bodyVectorFromWorld(u32 iBody, vector3df vPos);

    virtual void bodySetMass(u32 iBody, u32 iMass);

    virtual void geomSetBody(u32 iGeom, CIrrOdeBody *pBody);
    virtual void geomDestroy(u32 iGeom);

    virtual void geomSetPosition(u32 iGeom, vector3df pos);
    virtual void geomSetRotation(u32 iGeom, vector3df rot);
    virtual void geomSetData(u32 iGeom, void *pData);
    virtual void geomRaySetData(u32 iGeom, vector3df pos, vector3df dir, f32 fLength);
    virtual void geomSetBackfaceCull(u32 iGeom, bool bBackfaceCull);
    virtual void geomSetOffsetPosition(u32 iGeom, vector3df offset);
    virtual void geomSetOffsetQuaternion(u32 iGeom, vector3df rot);

    virtual u32 geomCreateRay(u32 iSpace, f32 fLength);
    virtual u32 geomCreateSphere(u32 iSpace, f32 fRadius);
    virtual u32 geomCreateTrimesh(u32 iSpace, u32 iData);
    virtual u32 geomCreateCapsule(u32 iSpace, f32 fRadius, f32 fLength);
    virtual u32 geomCreateCylinder(u32 iSpace, f32 fRadius, f32 fLength);
    virtual u32 geomCreateHeightfield(u32 iSpace, u32 iData);
    virtual u32 geomCreatePlane(u32 iSpace, f32 A, f32 B, f32 C, f32 D);
    virtual u32 geomCreateBox(u32 iSpace, f32 fWidth, f32 fHeight, f32 fDepth);

    virtual u32 geomHeightfieldDataCreate();
    virtual void geomHeightfieldSetBounds(u32 iHFData, f32 fMinHeight, f32 fMaxHeight);
    virtual void geomHeightfieldDataCallback(u32 iHFData, void *pData, void *pCallback, f32 fWidth, f32 fDepth, s32 iWidthSamples, s32 iDepthSamples, f32 fScale, f32 fOffset, f32 fThickness, s32 bWrap);

    virtual u32 geomTrimeshDataCreate();
    virtual void geomTrimeshDataBuildSimple(u32 iData, ISceneNode *pNode);
    virtual void geomTrimeshDataBuildFromMeshBuffer(u32 iData, ISceneNode *pNode, u32 iBuffer);

    virtual u32 massCreate();
    virtual void massAdd(u32 iDst, u32 iSrc);
    virtual void massSetZero(u32 iMass);
    virtual void massSetParameters(u32 iMass, f32 fMass, vector3df c, vector3df i1, vector3df i2);
    virtual void massTranslate(u32 iMass, vector3df pos);
    virtual void massSetBoxTotal(u32 iMass, f32 fMass, f32 lx, f32 ly, f32 lz);
    virtual void massSetCapsuleTotal(u32 iMass, f32 fMass, s32 iDirection, f32 fRadius, f32 fLength);
    virtual void massSetCylinderTotal(u32 iMass, f32 fMass, s32 iDirection, f32 fRadius, f32 fLength);
    virtual void massSetSphereTotal(u32 iMass, f32 fMass, f32 fRadius);

    virtual u32 spaceCreateSimple(u32 iParent);
    virtual u32 spaceCreateHash(u32 iParent);
    virtual u32 spaceCreateQuadTree(u32 iParent, vector3df center, vector3df extends, s32 iDepth);
    virtual void spaceDestroy(u32 iSpace);
    virtual u32 jointGroupCreate(s32 iMaxSize);
    virtual void jointGroupDestroy(u32 iJointGroup);
    virtual void jointGroupEmtpy(u32 iJointGroup);

    virtual void trimeshDrawDebug(u32 iData, IVideoDriver *pDrv);

    COdeDataWrapper *getData(u32 iIdx);

    static void rayCollide(CIrrOdeWorld *pWorld);
};

} //namespace ode
} //namespace irr

  #endif
#endif
