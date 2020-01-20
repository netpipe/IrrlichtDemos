#ifndef _C_IRR_ODE_SCENENODE
  #define _C_IRR_ODE_SCENENODE

  #include <irrlicht.h>

/**
 * some defines to display debug data:
 *
 * _TRACE_CONSTRUCTOR_DESTRUCTOR: plot constructor/destructor information to stdout
 * _TRACE_INIT_PHYSICS: plot init physics information to stdout
 * _DRAW_BOUNDING_BOXES: draw bounding boxes of the affected scene nodes
 * _DRAW_JOINT_INFO: draw joint info lines
 * _TRACE_CHANGED_PROPERTIES: trace changed properties
 * _TRACE_FACTORY: trace scenenode factory
 * _IRREDIT_PLUGIN: display additional objects for the IrrEdit plugin
 */

namespace irr {
namespace ode {

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CIrrOdeManager;
class IIrrOdeDevice;

/**
 * @class CIrrOdeSceneNode
 * This class is the base class for all scene nodes provided by IrrODE
 */
class CIrrOdeSceneNode : public scene::ISceneNode {
  protected:
    aabbox3df m_cBoundingBox;               /**< the bounding box of the scene node */
    ISceneManager *m_pSceneManager;         /**< the scene manager */
    IVideoDriver *m_pVideoDriver;           /**< the video driver */
    bool m_bPhysicsInitialized;             /**< is the physics of this node alread initialized? */
    stringc m_sOdeClassName;                /**< this node's irrODE classname */
    s32 m_iIdent;                           /**< this node's irrODE ID */
    CIrrOdeManager *m_pOdeManager;          /**< the ODE manager */
    IIrrOdeDevice *m_pOdeDevice;            /**< the ODE device */

    SMaterial m_cMat;                /**< the material of the node */
    ITriangleSelector *m_pSelector;

    void *m_pUserData;

    ISceneNode *getAncestorOfType(ESCENE_NODE_TYPE iType);  /**< retrieve an ancestor of a given type */

    #ifdef _IRREDIT_PLUGIN
      IAnimatedMesh *m_pMesh;   /**< the mesh to be renderer in IrrEdit when compiled as plugin */
  	  c8 m_sResources[1024];    /**> the path name of the resources when copiled as plugin */
  	  bool m_bDrawEditorMesh;
    #endif

    void cloneChildren(ISceneNode *pNewParent, ISceneManager *pNewManager);
  public:
    CIrrOdeSceneNode(ISceneNode *parent,ISceneManager *mgr,s32 id = -1,
                     const vector3df &position=core::vector3df(0,0,0),
		                 const vector3df &rotation = core::vector3df(0,0,0),
		                 const vector3df &scale = core::vector3df(1.0f, 1.0f, 1.0f));

    virtual ~CIrrOdeSceneNode();

    virtual void initPhysics(); /*!< init the dynamic object. Implemented by subclasses */
    virtual const aabbox3df &getBoundingBox() const;

    /**
     * Get the type name of the node
     * @return the type name of the node
     */
    virtual const wchar_t *getTypeName()=0;

    bool physicsInitialized();

    static const c8 *nodeNameToC8(const wchar_t *sName); /*!< a little helper that converts wchar_t* to c8* */

    virtual void serializeAttributes(IAttributes* out, SAttributeReadWriteOptions* options) const;
    virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options);

    stringc getOdeClassname();                /*!< set the IrrODE classname */
    void setOdeClassname(stringc sClassName); /*!< get the IrrODE classname */

    int getIdent();             /*!< get the IrrODE identifier */
    void setIdent(int iIdent);  /*!< set the IrrODE identifier */

    virtual u32 getMaterialCount();
    virtual SMaterial &getMaterial(u32 iIdx);
    virtual void 	addChild (ISceneNode *child);

    virtual void render();
    virtual void removeFromPhysics();  /**< remove the node from physics */
    virtual void copyParams(CIrrOdeSceneNode *pDest, bool bRecurse=true); /**< copy the parameter to another ODE scene node */

    virtual void remove();

    ISceneManager *getSceneManager();

    ITriangleSelector *getTriangleSelector();

    void setUserData(void *p);
    void *getUserData();

    void setPhysicsInitialized(bool b) { m_bPhysicsInitialized=b; }
    #ifdef _IRREDIT_PLUGIN
      void setDrawEditorMesh(bool b);
    #endif

    virtual void setParent(ISceneNode *newParent);
};

} //namespace ode
} //namespace irr

#endif
