#ifndef _I_IRR_ODE_DEVICE
  #define _I_IRR_ODE_DEVICE

  #include <irrlicht.h>
  #include <CIrrOdeWorld.h>
  #include <IIrrOdeContactParameters.h>
  #include <geom/CIrrOdeGeomRay.h>

namespace irr {
namespace ode {

class CIrrOdeWorld;
class CIrrOdeBody;
class CIrrOdeGeom;

/**
 * @class IIrrOdeDevice
 * This interface is used to encapsulate all calls to ODE in one single class. This has mainly been done
 * so that it is not necessary to compile ODE in VisualC++ but to use the CIrrOdeNullDevice for the IrrEdit
 * plugin
 * @see CIrrOdeNullDevice
 * @see CIrrOdeDevice
 */
class IIrrOdeDevice {
  public:
    virtual ~IIrrOdeDevice() { }

    virtual void initODE()=0;
    virtual void closeODE()=0;
    virtual void clearODE()=0;

    /**
     * Some general functions to set and get the contact calculator
     */
    virtual void setContactCalculator(IIrrOdeContactParameters *pCalculator)=0;
    virtual IIrrOdeContactParameters *getContactCalculator()=0;

    /**
     * Some general functions to set the stepsize (default is 0.016)
     */
    virtual void setStepSize(f32 f)=0;
    virtual f32 getStepSize()=0;

    /**
     * The step functions
     */
    virtual void step(f32 fTime, CIrrOdeWorld *pWorld)=0;
    virtual void quickStep(f32 fTime, CIrrOdeWorld *pWorld)=0;

    /**
     * The first block of functions are for the ODE world
     */
    virtual void worldDestroy(u32 iIdx)=0;
    virtual u32 worldCreate()=0;
    virtual void worldSetLinearDamping(u32 iWorld, f32 fScale)=0;
    virtual f32 worldGetLinearDamping(u32 iWorld)=0;
    virtual void worldSetAngularDamping(u32 iWorld, f32 fScale)=0;
    virtual f32 worldGetAngularDamping(u32 iWorld)=0;
    virtual void worldSetLinearDampingThreshold(u32 iWorld, f32 fScale)=0;
    virtual f32 worldGetLinearDampingThreshold(u32 iWorld)=0;
    virtual void worldSetAngularDampingThreshold(u32 iWorld, f32 fScale)=0;
    virtual f32 worldGetAngularDampingThreshold(u32 iWorld)=0;
    virtual void worldSetMaxAngularSpeed(u32 iWorld, f32 fMax)=0;
    virtual f32 worldGetMaxAngularSpeed(u32 iWorld)=0;
    virtual void worldSetAutoDisableFlag(u32 iWorld, s32 iFlag)=0;
    virtual s32 worldGetAutoDisableFlag(u32 iWorld)=0;
    virtual void worldSetAutoDisableLinearThreshold(u32 iWorld, f32 f)=0;
    virtual f32 worldGetAutoDisableLinearThreshold(u32 iWorld)=0;
    virtual void worldSetAutoDisableAngularThreshold(u32 iWorld, f32 f)=0;
    virtual f32 worldGetAutoDisableAngularThreshold(u32 iWorld)=0;
    virtual void worldSetAutoDisableSteps(u32 iWorld, s32 i)=0;
    virtual s32 worldGetAutoDisableSteps(u32 iWorld)=0;
    virtual void worldSetAutoDisableTime(u32 iWorld, f32 f)=0;
    virtual f32 worldGetAutoDisableTime(u32 iWorld)=0;
    virtual void worldSetQuickStepNumIterations(u32 iWorld, s32 i)=0;
    virtual s32 worldGetQuickStepNumIterations(u32 iWorld)=0;
    virtual void worldSetQuickStepW(u32 iWorld, f32 f)=0;
    virtual f32 worldGetQuickStepW(u32 iWorld)=0;
    virtual void worldSetGravity(u32 iWorld, vector3df cGravity)=0;
    virtual void worldSetERP(u32 iWorld, f32 fErp)=0;
    virtual f32 worldGetERP(u32 iWorld)=0;
    virtual void worldSetCFM(u32 iWorld, f32 fCfm)=0;
    virtual f32 worldGetCFM(u32 iWorld)=0;

    /**
     * General joint related functions
     */
    virtual void jointAttach(u32 iJoint, CIrrOdeBody *b1, CIrrOdeBody *b2)=0;
    virtual void jointSetData(u32 iJoint, void *pData)=0;
    virtual void jointDestroy(u32 iJoint)=0;

    /**
     * Now for a lot of joint related functions: first of all: create joints
     */
    virtual u32 jointCreateBallSocket(u32 iWorld)=0;
    virtual u32 jointCreateFixed(u32 iWorld)=0;
    virtual u32 jointCreateHinge(u32 iWorld)=0;
    virtual u32 jointCreateHinge2(u32 iWorld)=0;
    virtual u32 jointCreatePiston(u32 iWorld)=0;
    virtual u32 jointCreateSlider(u32 iWorld)=0;
    virtual u32 jointCreateUniversal(u32 iWorld)=0;
    virtual u32 jointCreatePR(u32 iWorld)=0;

    /**
     * Functions for the Ball and Socket joint
     */
    virtual void jointSetBallAnchor(u32 iJoint, vector3df pos)=0;
    virtual vector3df jointGetBallAnchor(u32 iJoint)=0;
    virtual vector3df jointGetBallAnchor2(u32 iJoint)=0;

    /**
     * Function for the Fixed joint
     */
    virtual void jointSetFixed(u32 iJoint)=0;

    /**
     * Functions for the Hinge joint
     */
    virtual void jointSetHingeAnchor(u32 iJoint, vector3df p)=0;
    virtual void jointSetHingeAxis(u32 iJoint, vector3df a)=0;
    virtual vector3df jointGetHingeAxis(u32 iJoint)=0;
    virtual vector3df jointGetHingeAnchor1(u32 iJoint)=0;
    virtual vector3df jointGetHingeAnchor2(u32 iJoint)=0;
    virtual f32 jointGetHingeAngle(u32 iJoint)=0;
    virtual f32 jointGetHingeAngleRate(u32 iJoint)=0;
    virtual void jointSetHingeParam(u32 iJoint, s32 iParam, f32 iValue)=0;
    virtual f32 jointGetHingeParam(u32 iJoint, s32 iParam)=0;

    /**
     * Functions for the Hinge2 joint
     */
    virtual void jointSetHinge2Anchor(u32 iJoint, vector3df a)=0;
    virtual void jointSetHinge2Axis1(u32 iJoint, vector3df a)=0;
    virtual void jointSetHinge2Axis2(u32 iJoint, vector3df a)=0;
    virtual vector3df jointGetHinge2Axis1(u32 iJoint)=0;
    virtual vector3df jointGetHinge2Axis2(u32 iJoint)=0;
    virtual vector3df jointGetHinge2Anchor(u32 iJoint)=0;
    virtual vector3df jointGetHinge2Anchor2(u32 iJoint)=0;
    virtual f32 jointGetHinge2Angle1(u32 iJoint)=0;
    virtual f32 jointGetHinge2Angle1Rate(u32 iJoint)=0;
    virtual f32 jointGetHinge2Angle2Rate(u32 iJoint)=0;
    virtual void jointSetHinge2Param(u32 iJoint, u32 iGroup, s32 iParam, f32 iValue)=0;
    virtual f32 jointGetHinge2Param(u32 iJoint, u32 iGroup, s32 iParam)=0;

    /**
     * Functions for the Piston joint
     */
    virtual void jointSetPistonAnchor(u32 iJoint, vector3df a)=0;
    virtual void jointSetPistonAxis(u32 iJoint, vector3df a)=0;
    virtual f32 jointGetPistonPosition(u32 iJoint)=0;
    virtual f32 jointGetPistonPositionRate(u32 iJoint)=0;
    virtual vector3df jointGetPistonAnchor(u32 iJoint)=0;
    virtual vector3df jointGetPistonAxis(u32 iJoint)=0;
    virtual void jointSetPistonParam(u32 iJoint, u32 iGroup, s32 iParam, f32 fValue)=0;
    virtual f32 jointGetPistonParam(u32 iJoint, u32 iGroup, s32 iParam)=0;

    /**
     * Functions for the Slider joint
     */
    virtual void jointSetSliderAxis(u32 iJoint, vector3df a)=0;
    virtual vector3df jointGetSliderAxis(u32 iJoint)=0;
    virtual f32 jointGetSliderPosition(u32 iJoint)=0;
    virtual f32 jointGetSliderPositionRate(u32 iJoint)=0;
    virtual void jointSetSliderParam(u32 iJoint, s32 iParam, f32 fValue)=0;
    virtual f32 jointGetSliderParam(u32 iJoint, s32 iParam)=0;

    /**
     * Functions for the Universal joint
     */
    virtual void jointSetUniversalAnchor(u32 iJoint, vector3df a)=0;
    virtual void jointSetUniversalAxis1(u32 iJoint, vector3df a)=0;
    virtual void jointSetUniversalAxis2(u32 iJoint, vector3df a)=0;
    virtual vector3df jointGetUniversalAnchor(u32 iJoint)=0;
    virtual vector3df jointGetUniversalAnchor2(u32 iJoint)=0;
    virtual vector3df jointGetUniversalAxis1(u32 iJoint)=0;
    virtual vector3df jointGetUniversalAxis2(u32 iJoint)=0;
    virtual f32 jointGetUniversalAngle1(u32 iJoint)=0;
    virtual f32 jointGetUniversalAngle2(u32 iJoint)=0;
    virtual f32 jointGetUniversalAngle1Rate(u32 iJoint)=0;
    virtual f32 jointGetUniversalAngle2Rate(u32 iJoint)=0;
    virtual void jointSetUniversalParam(u32 iJoint, u32 iGroup, s32 iParam, f32 fValue)=0;
    virtual f32 jointGetUniversalParam(u32 iJoint, u32 iGroup, s32 iParam)=0;

    /**
     * Functions for the Prismatic-Rotoide joint
     */
    virtual void jointSetPrAxis1(u32 iJoint, vector3df a)=0;
    virtual void jointSetPrAxis2(u32 iJoint, vector3df a)=0;
    virtual void jointSetPrAnchor(u32 iJoint, vector3df a)=0;
    virtual vector3df jointGetPrAxis1(u32 iJoint)=0;
    virtual vector3df jointGetPrAxis2(u32 iJoint)=0;
    virtual vector3df jointGetPrAnchor(u32 iJoint)=0;
    virtual f32 jointGetPrPosition(u32 iJoint)=0;
    virtual f32 jointGetPrAngle(u32 iJoint)=0;
    virtual f32 jointGetPrAngleRate(u32 iJoint)=0;
    virtual void  jointSetPrParam(u32 iJoint, s32 iParam, f32 iValue)=0;
    virtual f32 jointGetPrParam(u32 iJoint, s32 iParam)=0;

    /**
     * Body functions
     */
    virtual u32 bodyCreate(u32 iWorld)=0;
    virtual void bodyDestroy(u32 iBody)=0;
    virtual void bodySetPosition(u32 iBody, vector3df pos)=0;
    virtual vector3df bodyGetPosition(u32 iBody)=0;
    virtual vector3df bodyGetRotation(u32 iBody)=0;
    virtual void bodySetQuaternion(u32 iBody, core::quaternion rot)=0;
    virtual void bodySetRotation(u32 iBody, vector3df rot)=0;
    virtual void bodySetData(u32 iBody, void *pData)=0;
    virtual void bodySetLinearDamping(u32 iBody, f32 f)=0;
    virtual f32 bodyGetLinearDamping(u32 iBody)=0;
    virtual void bodySetAngularDamping(u32 iBody, f32 f)=0;
    virtual f32 bodyGetAngularDamping(u32 iBody)=0;
    virtual void bodySetLinearDampingThreshold(u32 iBody, f32 f)=0;
    virtual f32 bodyGetLinearDampingThreshold(u32 iBody)=0;
    virtual void bodySetAngularDampingThreshold(u32 iBody, f32 f)=0;
    virtual f32 bodyGetAngularDampingThreshold(u32 iBody)=0;
    virtual void bodySetMaxAngularSpeed(u32 iBody, f32 f)=0;
    virtual f32 bodyGetMaxAngularSpeed(u32 iBody)=0;
    virtual void bodySetAutoDisableFlag(u32 iBody, s32 i)=0;
    virtual s32 bodyGetAutoDisableFlag(u32 iBody)=0;
    virtual void bodySetLinearAutoDisableThreshold(u32 iBody, f32 f)=0;
    virtual f32 bodyGetLinearAutoDisableThreshold(u32 iBody)=0;
    virtual void bodySetAngularAutoDisableThreshold(u32 iBody, f32 f)=0;
    virtual f32 bodyGetAngularAutoDisableThreshold(u32 iBody)=0;
    virtual void bodySetAutoDisableSteps(u32 iBody, s32 i)=0;
    virtual s32 bodyGetAutoDisableSteps(u32 iBody)=0;
    virtual void bodySetAutoDisableTime(u32 iBody, f32 f)=0;
    virtual f32 bodyGetAutoDisableTime(u32 iBody)=0;
    virtual void bodySetFiniteRotationMode(u32 iBody, bool b)=0;
    virtual bool bodyGetFiniteRotationMode(u32 iBody)=0;
    virtual void bodySetFiniteRotationAxis(u32 iBody, vector3df v)=0;
    virtual const vector3df bodyGetFiniteRotationAxis(u32 iBody)=0;
    virtual void bodySetGravityMode(u32 iBody, s32 i)=0;
    virtual s32 bodyGetGravityMode(u32 iBody)=0;
    virtual CIrrOdeGeom *bodyGetFirstGeom(u32 iBody)=0;
    virtual CIrrOdeGeom *bodyGetNextGeom(CIrrOdeGeom *pPrev)=0;
    virtual bool bodyIsEnabled(u32 iBody)=0;
    virtual void bodyAddForce(u32 iBody, vector3df f)=0;
    virtual void bodyAddForceAtPosition(u32 iBody, vector3df f, vector3df p)=0;
    virtual void bodyAddTorque(u32 iBody, vector3df t)=0;
    virtual void bodySetTorque(u32 iBody, vector3df t)=0;
    virtual void bodyEnable(u32 iBody)=0;
    virtual void bodyDisable(u32 iBody)=0;
    virtual void bodySetLinearVelocity(u32 iBody, vector3df v)=0;
    virtual vector3df bodyGetLinearVelocity(u32 iBody)=0;
    virtual void bodySetAngularVelocity(u32 iBody, vector3df v)=0;
    virtual vector3df bodyGetAngularVelocity(u32 iBody)=0;
    virtual vector3df bodyGetPointVel(u32 iBody, vector3df vPos)=0;
    virtual vector3df bodyGetRelPointVel(u32 iBody, vector3df vPos)=0;
    virtual vector3df bodyGetRelPointPos(u32 iBody, vector3df vPos)=0;
    virtual vector3df bodyGetPosRelPoint(u32 iBody, vector3df vPos)=0;
    virtual vector3df bodyVectorFromWorld(u32 iBody, vector3df vPos)=0;

    virtual void bodySetMass(u32 iBody, u32 iMass)=0;

    virtual void geomSetBody(u32 iGeom, CIrrOdeBody *pBody)=0;
    virtual void geomDestroy(u32 iGeom)=0;
    virtual void geomSetOffsetPosition(u32 iGeom, vector3df offset)=0;
    virtual void geomSetOffsetQuaternion(u32 iGeom, vector3df rot)=0;

    virtual void geomSetPosition(u32 iGeom, vector3df pos)=0;
    virtual void geomSetRotation(u32 iGeom, vector3df rot)=0;
    virtual void geomSetData(u32 iGeom, void *pData)=0;
    virtual void geomRaySetData(u32 iGeom, vector3df pos, vector3df dir, f32 fLength)=0;
    virtual void geomSetBackfaceCull(u32 iGeom, bool bBackfaceCull)=0;

    virtual u32 geomCreateBox(u32 iSpace, f32 fWidth, f32 fHeight, f32 fDepth)=0;
    virtual u32 geomCreateCapsule(u32 iSpace, f32 fRadius, f32 fLength)=0;
    virtual u32 geomCreateCylinder(u32 iSpace, f32 fRadius, f32 fLength)=0;
    virtual u32 geomCreateHeightfield(u32 iSpace, u32 iData)=0;
    virtual u32 geomCreatePlane(u32 iSpace, f32 A, f32 B, f32 C, f32 D)=0;
    virtual u32 geomCreateRay(u32 iSpace, f32 fLength)=0;
    virtual u32 geomCreateSphere(u32 iSpace, f32 fRadius)=0;
    virtual u32 geomCreateTrimesh(u32 iSpace, u32 iData)=0;

    virtual u32 geomHeightfieldDataCreate()=0;
    virtual void geomHeightfieldSetBounds(u32 iHFData, f32 fMinHeight, f32 fMaxHeight)=0;
    virtual void geomHeightfieldDataCallback(u32 iHFData, void *pData, void *pCallback, f32 fWidth, f32 fDepth, s32 iWidthSamples, s32 iDepthSamples, f32 fScale, f32 fOffset, f32 fThickness, s32 bWrap)=0;

    virtual u32 geomTrimeshDataCreate()=0;
    virtual void geomTrimeshDataBuildSimple(u32 iData, ISceneNode *pNode)=0;
    virtual void geomTrimeshDataBuildFromMeshBuffer(u32 iData, ISceneNode *pNode, u32 iBuffer)=0;

    virtual u32 massCreate()=0;
    virtual void massAdd(u32 iDst, u32 iSrc)=0;
    virtual void massSetZero(u32 iMass)=0;
    virtual void massSetParameters(u32 iMass, f32 fMass, vector3df c, vector3df i1, vector3df i2)=0;
    virtual void massTranslate(u32 iMass, vector3df pos)=0;
    virtual void massSetBoxTotal(u32 iMass, f32 fMass, f32 lx, f32 ly, f32 lz)=0;
    virtual void massSetCapsuleTotal(u32 iMass, f32 fMass, s32 iDirection, f32 fRadius, f32 fLength)=0;
    virtual void massSetCylinderTotal(u32 iMass, f32 fMass, s32 iDirection, f32 fRadius, f32 fLength)=0;
    virtual void massSetSphereTotal(u32 iMass, f32 fMass, f32 fRadius)=0;

    virtual u32 spaceCreateSimple(u32 iParent)=0;
    virtual u32 spaceCreateHash(u32 iParent)=0;
    virtual u32 spaceCreateQuadTree(u32 iParent, vector3df center, vector3df extends, s32 iDepth)=0;
    virtual void spaceDestroy(u32 iSpace)=0;

    virtual u32 jointGroupCreate(s32 iMaxSize)=0;
    virtual void jointGroupDestroy(u32 iJointGroup)=0;
    virtual void jointGroupEmtpy(u32 iJointGroup)=0;

    virtual void trimeshDrawDebug(u32 iData, IVideoDriver *pDrv)=0;
};

} //namespace ode
} //namespace irr

#endif
