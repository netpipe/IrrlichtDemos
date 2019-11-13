#ifndef _IRR_ODE_NULL_DEVICE
  #define _IRR_ODE_NULL_DEVICE

  #include <IIrrOdeDevice.h>
  #include <IIrrOdeContactParameters.h>
  #include <CIrrOdeWorld.h>
  #include <CIrrOdeBody.h>
  #include <geom/CIrrOdeGeom.h>

  #define _VEC static vector3df v=vector3df(0.0f,0.0f,0.0f); return v

namespace irr {
namespace ode {

/**
 * @class CIrrOdeNullDevice
 * This is just a dummy that does nothing at all. It is used for the IrrOdEdit plugin, so that it is not necessary
 * to have any ODE functionality in the plugin that wouldn't use it anyways.
 */
class CIrrOdeNullDevice : public IIrrOdeDevice {
  protected:
    f32 m_fStepSize;

  public:
    CIrrOdeNullDevice() : IIrrOdeDevice() { m_fStepSize=0.016f; }
    virtual ~CIrrOdeNullDevice() { }

    virtual void initODE() { }
    virtual void closeODE() { }
    virtual void clearODE() { }

    virtual void setContactCalculator(IIrrOdeContactParameters *pCalculator) { }
    virtual IIrrOdeContactParameters *getContactCalculator() { return NULL; }

    virtual void setStepSize(f32 f) { m_fStepSize=f; }
    virtual f32 getStepSize() { return m_fStepSize; }

    virtual void step(f32 fTime, CIrrOdeWorld *pWorld, u32 iJointGroupId) { }
    virtual void quickStep(f32 fTime, CIrrOdeWorld *pWorld, u32 iJointGroupId) { }

    virtual void worldDestroy(u32 iIdx) { }
    virtual u32 worldCreate() { return 0; }
    virtual void worldSetLinearDamping(u32 iWorld, f32 fScale) { }
    virtual f32 worldGetLinearDamping(u32 iWorld) { return 0.0f; }
    virtual void worldSetAngularDamping(u32 iWorld, f32 fScale) { }
    virtual f32 worldGetAngularDamping(u32 iWorld) { return 0.0f; }
    virtual void worldSetLinearDampingThreshold(u32 iWorld, f32 fScale) { }
    virtual f32 worldGetLinearDampingThreshold(u32 iWorld) { return 0.0f; }
    virtual void worldSetAngularDampingThreshold(u32 iWorld, f32 fScale) { }
    virtual f32 worldGetAngularDampingThreshold(u32 iWorld) { return 0.0f; }
    virtual void worldSetMaxAngularSpeed(u32 iWorld, f32 fMax) { }
    virtual f32 worldGetMaxAngularSpeed(u32 iWorld) { return 0.0f; }
    virtual void worldSetAutoDisableFlag(u32 iWorld, s32 iFlag) { }
    virtual s32 worldGetAutoDisableFlag(u32 iWorld) { return 0; }
    virtual void worldSetAutoDisableLinearThreshold(u32 iWorld, f32 f) { }
    virtual f32 worldGetAutoDisableLinearThreshold(u32 iWorld) { return 0.0f; }
    virtual void worldSetAutoDisableAngularThreshold(u32 iWorld, f32 f) { }
    virtual f32 worldGetAutoDisableAngularThreshold(u32 iWorld) { return 0.0f; }
    virtual void worldSetAutoDisableSteps(u32 iWorld, s32 i) { }
    virtual s32 worldGetAutoDisableSteps(u32 iWorld) { return 0; }
    virtual void worldSetAutoDisableTime(u32 iWorld, f32 f) { }
    virtual f32 worldGetAutoDisableTime(u32 iWorld) { return 0.0f; }
    virtual void worldSetQuickStepNumIterations(u32 iWorld, s32 i) { }
    virtual s32 worldGetQuickStepNumIterations(u32 iWorld) { return 0; }
    virtual void worldSetQuickStepW(u32 iWorld, f32 f) { }
    virtual f32 worldGetQuickStepW(u32 iWorld) { return 0; }
    virtual void worldSetGravity(u32 iWorld, vector3df cGravity) { }

    virtual u32 jointCreateBallSocket(u32 iWorld) { return 0; }
    virtual u32 jointCreateFixed(u32 iWorld) { return 0; }
    virtual u32 jointCreateHinge(u32 iWorld) { return 0; }
    virtual u32 jointCreateHinge2(u32 iWorld) { return 0; }
    virtual u32 jointCreatePiston(u32 iWorld) { return 0; }
    virtual u32 jointCreateSlider(u32 iWorld) { return 0; }
    virtual u32 jointCreateUniversal(u32 iWorld) { return 0; }

    virtual void jointAttach(u32 iJoint, CIrrOdeBody *b1, CIrrOdeBody *b2) { }
    virtual void jointSetData(u32 iJoint, void *pData) { }
    virtual void jointDestroy(u32 iJoint) { }

    virtual void jointSetBallAnchor(u32 iJoint, vector3df pos) { }
    virtual vector3df jointGetBallAnchor(u32 iJoint) { static vector3df v=vector3df(0,0,0); return v; }
    virtual vector3df jointGetBallAnchor2(u32 iJoint) { static vector3df v=vector3df(0,0,0); return v; }

    virtual void jointSetFixed(u32 iJoint) { }

    virtual void jointSetHingeAnchor(u32 iJoint, vector3df p) { }
    virtual void jointSetHingeAxis(u32 iJoint, vector3df a) { }
    virtual vector3df jointGetHingeAxis(u32 iJoint) { _VEC; }
    virtual vector3df jointGetHingeAnchor1(u32 iJoint) { _VEC; }
    virtual vector3df jointGetHingeAnchor2(u32 iJoint) { _VEC; }
    virtual f32 jointGetHingeAngle(u32 iJoint) { return 0.0f; }
    virtual f32 jointGetHingeAngleRate(u32 iJoint) {  return 0.0f; }
    virtual void jointSetHingeParam(u32 iJoint, s32 iParam, f32 iValue) { }
    virtual f32 jointGetHingeParam(u32 iJoint, s32 iParam) { return 0.0f; }

    virtual void jointSetHinge2Anchor(u32 iJoint, vector3df a) { }
    virtual void jointSetHinge2Axis1(u32 iJoint, vector3df a) { }
    virtual void jointSetHinge2Axis2(u32 iJoint, vector3df a) { }
    virtual vector3df jointGetHinge2Axis1(u32 iJoint) { _VEC; }
    virtual vector3df jointGetHinge2Axis2(u32 iJoint) { _VEC; }
    virtual vector3df jointGetHinge2Anchor(u32 iJoint) { _VEC; }
    virtual vector3df jointGetHinge2Anchor2(u32 iJoint) { _VEC; }
    virtual f32 jointGetHinge2Angle1(u32 iJoint) { return 0.0f; }
    virtual f32 jointGetHinge2Angle1Rate(u32 iJoint) { return 0.0f; }
    virtual f32 jointGetHinge2Angle2Rate(u32 iJoint) { return 0.0f; }
    virtual void jointSetHinge2Param(u32 iJoint, u32 iGroup, s32 iParam, f32 iValue) { }
    virtual f32 jointGetHinge2Param(u32 iJoint, u32 iGroup, s32 iParam) { return 0.0f; }

    virtual void jointSetPistonAnchor(u32 iJoint, vector3df a) { }
    virtual void jointSetPistonAxis(u32 iJoint, vector3df a) { }
    virtual f32 jointGetPistonPosition(u32 iJoint) { return 0.0f; }
    virtual f32 jointGetPistonPositionRate(u32 iJoint) { return 0.0f; }
    virtual void jointSetPistonParam(u32 iJoint, u32 iGroup, s32 iParam, f32 fValue) { }
    virtual f32 jointGetPistonParam(u32 iJoint, u32 iGroup, s32 iParam) { return 0.0f; }
    virtual vector3df jointGetPistonAnchor(u32 iJoint) { _VEC; }
    virtual vector3df jointGetPistonAxis(u32 iJoint) { _VEC; }

    virtual void jointSetSliderAxis(u32 iJoint, vector3df a) { }
    virtual vector3df jointGetSliderAxis(u32 iJoint) { _VEC; }
    virtual f32 jointGetSliderPosition(u32 iJoint) { return 0.0f; }
    virtual f32 jointGetSliderPositionRate(u32 iJoint) { return 0.0f; }
    virtual void jointSetSliderParam(u32 iJoint, s32 iParam, f32 fValue) { }
    virtual f32 jointGetSliderParam(u32 iJoint, s32 iParam) { return 0.0f; }

    virtual void jointSetUniversalAnchor(u32 iJoint, vector3df a) { }
    virtual void jointSetUniversalAxis1(u32 iJoint, vector3df a) { }
    virtual void jointSetUniversalAxis2(u32 iJoint, vector3df a) { }
    virtual vector3df jointGetUniversalAnchor(u32 iJoint) { _VEC; }
    virtual vector3df jointGetUniversalAnchor2(u32 iJoint) { _VEC; }
    virtual vector3df jointGetUniversalAxis1(u32 iJoint) { _VEC; }
    virtual vector3df jointGetUniversalAxis2(u32 iJoint) { _VEC; }
    virtual void jointSetUniversalParam(u32 iJoint, u32 iGroup, s32 iParam, f32 fValue) { }
    virtual f32 jointGetUniversalParam(u32 iJoint, u32 iGroup, s32 iParam) { return 0.0f; }
    virtual f32 jointGetUniversalAngle1(u32 iJoint) { return 0.0f; }
    virtual f32 jointGetUniversalAngle2(u32 iJoint) { return 0.0f; }
    virtual f32 jointGetUniversalAngle1Rate(u32 iJoint) { return 0.0f; }
    virtual f32 jointGetUniversalAngle2Rate(u32 iJoint) { return 0.0f; }

    virtual u32 bodyCreate(u32 iWorld) { return 0; }
    virtual void bodyDestroy(u32 iBody) { }
    virtual void bodySetPosition(u32 iBody, vector3df pos) { }
    virtual void bodySetQuaternion(u32 iBody, vector3df rot) { }
    virtual void bodySetData(u32 iBody, void *pData) { }
    virtual void bodySetLinearDamping(u32 iBody, f32 f) { }
    virtual f32 bodyGetLinearDamping(u32 iBody) { return 0.0f; }
    virtual void bodySetAngularDamping(u32 iBody, f32 f) { }
    virtual f32 bodyGetAngularDamping(u32 iBody) { return 0.0f; }
    virtual void bodySetLinearDampingThreshold(u32 iBody, f32 f) { }
    virtual f32 bodyGetLinearDampingThreshold(u32 iBody) { return 0.0f; }
    virtual void bodySetAngularDampingThreshold(u32 iBody, f32 f) { }
    virtual f32 bodyGetAngularDampingThreshold(u32 iBody) { return 0.0f; }
    virtual void bodySetMaxAngularSpeed(u32 iBody, f32 f) { }
    virtual f32 bodyGetMaxAngularSpeed(u32 iBody) { return 0.0f; }
    virtual void bodySetAutoDisableFlag(u32 iBody, s32 i) { }
    virtual s32 bodyGetAutoDisableFlag(u32 iBody) { return 0; }
    virtual void bodySetLinearAutoDisableThreshold(u32 iBody, f32 f) { }
    virtual f32 bodyGetLinearAutoDisableThreshold(u32 iBody) { return 0.0f; }
    virtual void bodySetAngularAutoDisableThreshold(u32 iBody, f32 f) { }
    virtual f32 bodyGetAngularAutoDisableThreshold(u32 iBody) { return 0.0f; }
    virtual void bodySetAutoDisableSteps(u32 iBody, s32 i) { }
    virtual s32 bodyGetAutoDisableSteps(u32 iBody) { return 0; }
    virtual void bodySetAutoDisableTime(u32 iBody, f32 f) { }
    virtual f32 bodyGetAutoDisableTime(u32 iBody) { return 0.0f; }
    virtual void bodySetFiniteRotationMode(u32 iBody, s32 i) { }
    virtual s32 bodyGetFiniteRotationMode(u32 iBody) { return 0; }
    virtual void bodySetFiniteRotationAxis(u32 iBody, vector3df v) { }
    virtual const vector3df bodyGetFiniteRotationAxis(u32 iBody) { static vector3df v=vector3df(0,0,0); return v; }
    virtual void bodySetGravityMode(u32 iBody, s32 i) { }
    virtual s32 bodyGetGravityMode(u32 iBody) { return 0; }
    virtual CIrrOdeGeom *bodyGetFirstGeom(u32 iBody) { return NULL; }
    virtual CIrrOdeGeom *bodyGetNextGeom(CIrrOdeGeom *pPrev) { return NULL; }
    virtual bool bodyIsEnabled(u32 iBody) { return false; }
    virtual void bodyAddForce(u32 iBody, vector3df f) { }
    virtual void bodyAddForceAtPosition(u32 iBody, vector3df f, vector3df p) { }
    virtual void bodyAddTorque(u32 iBody, vector3df t) { }
    virtual void bodySetTorque(u32 iBody, vector3df t) { }
    virtual void bodyEnable(u32 iBody) { }
    virtual void bodyDisable(u32 iBody) { }
    virtual void bodySetLinearVelocity(u32 iBody, vector3df v) { }
    virtual vector3df bodyGetLinearVelocity(u32 iBody) { _VEC; }
    virtual void bodySetAngularVelocity(u32 iBody, vector3df v) { }
    virtual vector3df bodyGetAngularVelocity(u32 iBody) { _VEC; }
    virtual vector3df bodyGetPointVel(u32 iBody, vector3df vPos) { _VEC; }
    virtual vector3df bodyGetRelPointVel(u32 iBody, vector3df vPos) { _VEC; }
    virtual vector3df bodyGetRelPointPos(u32 iBody, vector3df vPos) { _VEC; }

    virtual void bodySetMass(u32 iBody, u32 iMass) { }

    virtual void geomSetBody(u32 iGeom, CIrrOdeBody *pBody) { }
    virtual void geomDestroy(u32 iGeom) { }
    virtual void geomSetRotation(u32 iGeom, vector3df rot) { }
    virtual void geomSetData(u32 iGeom, void *pData) { }
    virtual void geomSetOffsetPosition(u32 iGeom, vector3df offset) { }
    virtual void geomSetOffsetQuaternion(u32 iGeom, vector3df rot) { }
    virtual u32 geomCreateBox(u32 iSpace, f32 fWidth, f32 fHeight, f32 fDepth) { return 0; }
    virtual u32 geomCreateCapsule(u32 iSpace, f32 fRadius, f32 fLength) { return 0; }
    virtual u32 geomCreateCylinder(u32 iSpace, f32 fRadius, f32 fLength) { return 0; }
    virtual u32 geomCreateHeightfield(u32 iSpace, u32 iData) { return 0; }
    virtual u32 geomCreatePlane(u32 iSpace, f32 A, f32 B, f32 C, f32 D) { return 0; }
    virtual u32 geomCreateRay(u32 iSpace, f32 fLength) { return 0; }
    virtual void geomSetBackfaceCull(u32 iGeom, bool bBackfaceCull) { }
    virtual u32 geomCreateSphere(u32 iSpace, f32 fRadius) { return 0; }
    virtual u32 geomCreateTrimesh(u32 iSpace, u32 iData) { return 0; }
    virtual u32 geomHeightfieldDataCreate() { return 0; }
    virtual void geomHeightfieldSetBounds(u32 iHFData, f32 fMinHeight, f32 fMaxHeight) { }
    virtual void geomHeightfieldDataCallback(u32 iHFData, void *pData, void *pCallback, f32 fWidth, f32 fDepth, s32 iWidthSamples, s32 iDepthSamples, f32 fScale, f32 fOffset, f32 fThickness, s32 bWrap) { }

    virtual void geomSetPosition(u32 iGeom, vector3df pos) { }
    virtual void geomRaySetData(u32 iGeom, vector3df pos, vector3df dir, f32 fLength) { }

    virtual u32 geomTrimeshDataCreate() { return 0; }
    virtual void geomTrimeshDataBuildSimple(u32 iData, ISceneNode *pNode) { }
    virtual void geomTrimeshDataBuildFromMeshBuffer(u32 iData, ISceneNode *pNode, u32 iBuffer) { }

    virtual u32 massCreate() { return 0; }
    virtual void massAdd(u32 iDst, u32 iSrc) { }
    virtual void massSetZero(u32 iMass) { }
    virtual void massSetParameters(u32 iMass, f32 fMass, vector3df c, vector3df i1, vector3df i2) { }
    virtual void massTranslate(u32 iMass, vector3df pos) { }
    virtual void massSetBoxTotal(u32 iMass, f32 fMass, f32 lx, f32 ly, f32 lz) { }
    virtual void massSetCapsuleTotal(u32 iMass, f32 fMass, s32 iDirection, f32 fRadius, f32 fLength) { }
    virtual void massSetCylinderTotal(u32 iMass, f32 fMass, s32 iDirection, f32 fRadius, f32 fLength) { }
    virtual void massSetSphereTotal(u32 iMass, f32 fMass, f32 fRadius) { }

    virtual u32 spaceCreateSimple(u32 iParent) { return 0; }
    virtual u32 spaceCreateHash(u32 iParent) { return 0; }
    virtual u32 spaceCreateQuadTree(u32 iParent, vector3df center, vector3df extends, s32 iDepth) { return 0; }
    virtual void spaceDestroy(u32 iSpace) { }

    virtual u32 jointGroupCreate(s32 iMaxSize) { return 0; }
    virtual void jointGroupDestroy(u32 iJointGroup) { }
    virtual void jointGroupEmtpy(u32 iJointGroup) { }

    virtual void trimeshDrawDebug(u32 iData, IVideoDriver *pDrv) { }
};

} //namespace ode
} //namespace irr

#endif
