#ifndef _C_IRR_ODE_WORLD
  #define _C_IRR_ODE_WORLD

  #include <CIrrOdeDampable.h>
  #include <IIrrOdeContactParameters.h>

  #define MAX_CONTACTS 16

namespace irr {
namespace ode {

const int IRR_ODE_WORLD_ID=MAKE_IRR_ID('i','o','w','d');
const wchar_t IRR_ODE_WORLD_NAME[0xFF]=L"CIrrOdeWorld";

class CIrrOdeSpace;
class CIrrOdeBody;
class CIrrOdeGeom;
class CIrrOdeEventBodyMoved;

class CIrrOdeWorld : public CIrrOdeDampable {
  protected:
    u32 m_iJointGroupId;
    u32 m_iWorldId;
    f32 m_fStepSize;      /**< the stepsize */

    bool m_bDrawEditorMesh;

    CIrrOdeSpace *m_pWorldSpace;

    vector3df m_cGravity;
    list<CIrrOdeBody *> m_pBodies;
    list<CIrrOdeGeom *> m_pGeoms;
    list<CIrrOdeSpace *> m_pSpaces;
    #ifdef _IRREDIT_PLUGIN
      list<CIrrOdeSceneNode *> m_pChildNodes;
    #endif

	stringw m_sSurfaceFile;

	void loadParameter(IXMLReader *pReader);
	u32 loadFromFile(const wchar_t *sName);
  public:
    CIrrOdeWorld(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                 const vector3df &position=core::vector3df(0,0,0),
		             const vector3df &rotation = core::vector3df(0,0,0),
		             const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeWorld();

    void frameUpdate();
    u32 getWorldId();

    void addBody(CIrrOdeBody *pBody);
    void addGeom(CIrrOdeGeom *pGeom);
    void addSpace(CIrrOdeSpace *pSpace);
    void removeBody(CIrrOdeBody *pBody);
    void removeGeom(CIrrOdeGeom *pGeom);
    void removeSpace(CIrrOdeSpace *pSpace);

    u32 getBodyCount();
    u32 getGeomCount();

    virtual void setLinearDamping(f32 fScale);
    virtual f32 getLinearDamping();
    virtual void setAngularDamping(f32 fScale);
    virtual f32 getAngularDamping();
    virtual void setLinearDampingThreshold(f32 fThreshold);
    virtual f32 getLinearDampingThreshold();
    virtual void setAngularDampingThreshold(f32 fThreshold);
    virtual f32 getAngularDampingThreshold();
    virtual void setMaxAngularSpeed(f32 fMaxSpeed);
    virtual f32 getMaxAngularSpeed();

    virtual void setAutoDisableFlag(int iFlag);
    virtual int getAutoDisableFlag();
    virtual void setAutoDisableLinearThreshold(f32 fThreshold);
    virtual f32 getAutoDisableLinearThreshold();
    virtual void setAutoDisableAngularThreshold(f32 fThreshold);
    virtual f32 getAutoDisableAngularThreshold();
    virtual void setAutoDisableSteps(int iSteps);
    virtual int getAutoDisableSteps();
    virtual void setAutoDisableTime(f32 fTime);
    virtual f32 getAutoDisableTime();

    virtual void initPhysics();

    void setStepSize(f32 fStepSize);
    f32 getStepSize();
    void setQuickStepNumIterations(s32 iIterations);
    s32 getQuickStepNumIterations();
    void setQuickStepSOR(f32 fSOR);
    f32 getQuickStepSOR();

    void step(f32 fTime);
    void quickStep(f32 fTime);

    void setGravity(vector3df cGravity);
    vector3df &getGravity();
    void setERP(f32 fERP);
    f32 getERP();
    void setCFM(f32 CFM);
    f32 getCFM();

    s32 getID();

    virtual void render();
    virtual void OnRegisterSceneNode();

    ESCENE_NODE_TYPE getType() const;
    virtual const wchar_t *getTypeName();

    u32 getJointGroupId();

    list<CIrrOdeBody *> &getBodyList();

    CIrrOdeSpace *getSpace();
    void setSpace(CIrrOdeSpace *pWorldSpace);
    void setContactCalculator(IIrrOdeContactParameters *pCalculator);
    IIrrOdeContactParameters *getContactCalculator();

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

  	virtual ISceneNode *clone(ISceneNode* newParent=0, ISceneManager* newManager=0);
    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true);
    virtual void removeFromPhysics();

    #ifdef _IRREDIT_PLUGIN
      void addOdeChildNode(CIrrOdeSceneNode *pNode);
      void removeOdeChildNode(CIrrOdeSceneNode *pNode);
    #endif

    /**
     * load a surface parameter XML file created with IrrOdeSurfaceEdit
     * @param sName the file to load
     * @return number of surface parameters loaded
     */
    u32 setSurfaceXml(const wchar_t *sName);

    /**
     * This method resets the body collision attributes. It's called by the device before each step
     */
    void resetCollisionAttributes();
};

} //namespace ode
} //namespace irr

#endif
