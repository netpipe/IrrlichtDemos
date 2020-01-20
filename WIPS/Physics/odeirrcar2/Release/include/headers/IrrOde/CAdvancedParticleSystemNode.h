  #include <irrlicht.h>

  #include <CParticleExplosionEmitter.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

const int ADVANCED_PARTICLE_NODE_ID=MAKE_IRR_ID('a','p','s','n');           /**< create an Irrlicht ID for this class */
const c8 ADVANCED_PARTICLE_NODE_NAME[0xFF]="AdvancedParticleSystemNode";    /**< store a name for this class */
const int ADVANCED_PARTICLE_ANIMATOR_ID=MAKE_IRR_ID('a','p','a','n');       /**< id of the animator */
const c8 ADVANCED_PARTICLE_ANIMATOR_NAME[0xFF]="AdvancedParticleAnimator";  /**< name of the animator */

/**
 * @class CAdvancedParticleSystemNode
 * @author Christian Keimel / bulletbyte.de
 * This is a little Irrlicht addition I have written. It's based on the CParticleSystemSceneNode
 * class and adds proper (de)serialization and a clone method. The rest is taken directly from
 * the Irrlicht class. There are some differences however:
 * (1) if you create an emitter or affector it will automatically be used/added
 * (2) no default emitter will be created
 */
class CAdvancedParticleSystemNode : public irr::scene::IParticleSystemSceneNode {
  public:
    /**
     * @enum eParticleEmitter
     * This is a enum for the indices of the comboboxes in IrrEdit
     */
    enum eParticleEmitter {
      eEmitterNone=0,         /**<! no emitter */
      eEmitterPoint,          /**<! point emitter */
      eEmitterSphere,         /**<! sphere emitter */
      eEmitterRing,           /**<! ring emitter */
      eEmitterBox,            /**<! box emitter */
      eEmitterCylinder,       /**<! cylinder emitter */
      eEmitterMesh,           /**<! mesh emitter */
      eEmitterAnimatedMesh,   /**<! animated mesh emitter */
      eEmitterExplosion,

      eEmitterNull,           /**<! dummy */
      eEmitterCount,          /**<! counter */
    };

    /**
     * @enum eParticleAffectors
     * An enum for the indices of the particle affectors for IrrEdit
     */
    enum eParticleAffectors {
      eAffectorNone=0,      /**<! no affector */
      eAffectorAttraction,  /**<! attraction affector */
      eAffectorFadeOut,     /**<! fadeout affector */
      eAffectorGravity,     /**<! gravity affector */
      eAffectorRotation,    /**<! rotation affector */
      eAffectorScale,       /**<! scale affector */

      eAffectorNull,        /**<! dummy */
      eAffectorCount        /**<! counter */
    };

    static const c8 *m_sEmitterNames[eEmitterCount];    /**<! names of the emitters for IrrEdit */
    static const c8 *m_sAffectorNames[eAffectorCount];  /**<! names of the affectors for IrrEdit */

  protected:
    eParticleEmitter m_eEmitter;    /**<! index of the current emitter */
    IParticleEmitter *m_pEmitter;   /**<! the current emitter */
    ISceneManager *m_pSmgr;         /**<! the scene manager */

    list<IParticleAffector *> m_lAffectors;   /**<! list of active affectors */
    array<SParticle> m_aParticles;            /**<! array of particles */

    dimension2df m_cParticleSize,     /**<! size of the particles */
                 m_cScaleTarget;      /**<! the scale target (can't be read from the scale affector */

    s32 m_iMaxAngleDegrees;   /**<! the maximum angle of the emitted particles */
    u32 m_iAffectorCount,     /**<! affector count */
        m_iMinTtl,            /**<! minimum time to live of a particle */
        m_iMaxTtl,            /**<! maximum time to live of a particle */
        m_iLastEmitTime,      /**<! timestamp of the last particle emission */
        m_iCurrentTime,       /**<! timestamp of the current render pass */
        m_iAtlasSize,         /**<! atlas size. Defines how many atlas parts the texture has in X and Y direction */
        m_iAtlasCount;
    f32 m_fAtlasSize;

    array<vector2df> m_aAtlasPos; /**<! positions of the atlas parts */

    SMeshBuffer *m_pBuffer;   /**<! buffer for rendering */

    bool m_bParticlesAreGlobal, /**<! are the particles global? */
         m_bIsActive,           /**<! is this system active? */
         m_bAtlasTexture;       /**<! use an Atlas texture? */

    void reallocateBuffers();   /**<! buffer reallocation */

  public:
    /**
     * Standard ISceneNode constructor
     */
    CAdvancedParticleSystemNode(ISceneNode *parent,ISceneManager *mgr,s32 id=-1,
                             const core::vector3df &position=core::vector3df(0,0,0),
                             const core::vector3df &rotation=core::vector3df(0,0,0),
                             const core::vector3df &scale=core::vector3df(1.0f, 1.0f, 1.0f));

    ~CAdvancedParticleSystemNode();

    /**
     * get the emitter type (index of the combobox)
     * @return index of the current emitter
     */
    eParticleEmitter getEmitterType() { return m_eEmitter; }

    /**
     * Enhanced deserialization method
     */
    virtual void deserializeAttributes(io::IAttributes *in, io::SAttributeReadWriteOptions *options=0);

    /**
     * Enhanced serialization method
     */
    virtual void serializeAttributes(io::IAttributes *out, io::SAttributeReadWriteOptions *options=0) const;

    /**
     * Clone method
     * @param newParent the parent of the clone
     * @param newManager the scene manager for the clone
     * @return a clone
     */
    virtual ISceneNode *clone(ISceneNode *newParent=0, ISceneManager *newManager=0);

    /**
     * Create an animatedMeshSceneNodeEmitter. See Irrlicht documentation for parameters.
     */
    virtual IParticleAnimatedMeshSceneNodeEmitter *createAnimatedMeshSceneNodeEmitter(
            scene::IAnimatedMeshSceneNode *node, bool useNormalDirection=true,
            const core::vector3df &direction=core::vector3df(0.0f, 0.03f, 0.0f),
            f32 normalDirectionModifier=100.0f, s32 mbNumber=-1,bool everyMeshVertex=false,
            u32 minParticlesPerSecond=5, u32 maxParticlesPerSecond=10,
            const video::SColor &minStartColor=video::SColor(255, 0, 0, 0),
            const video::SColor &maxStartColor=video::SColor(255, 255, 255, 255),
            u32 lifeTimeMin=2000, u32 lifeTimeMax=4000, s32 maxAngleDegrees=0,
            const core::dimension2df &minStartSize=core::dimension2df(5.0f, 5.0f),
            const core::dimension2df &maxStartSize=core::dimension2df(5.0f, 5.0f));

    /**
     * Create a cylinder emitter. See Irrlicht documentation for parameters.
     * Note: the emitter will automatically be used, no need to manually add it
     */
    virtual IParticleCylinderEmitter *createCylinderEmitter(
            const core::vector3df &center, f32 radius, const core::vector3df &normal, f32 length,
            bool outlineOnly=false, const core::vector3df &direction=core::vector3df(0.0f, 0.03f, 0.0f),
            u32 minParticlesPerSecond=5, u32 maxParticlesPerSecond=10,
            const video::SColor &minStartColor=video::SColor(255, 0, 0, 0),
            const video::SColor &maxStartColor=video::SColor(255, 255, 255, 255),
            u32 lifeTimeMin=2000, u32 lifeTimeMax=4000, s32 maxAngleDegrees=0,
            const core::dimension2df &minStartSize=core::dimension2df(5.0f, 5.0f),
            const core::dimension2df &maxStartSize=core::dimension2df(5.0f, 5.0f));

    /**
     * Create a mesh emitter. See Irrlicht documentation for parameters.
     * Note: the emitter will automatically be used, no need to manually add it
     */
    virtual IParticleMeshEmitter *createMeshEmitter(scene::IMesh *mesh, bool useNormalDirection=true,
            const core::vector3df &direction=core::vector3df(0.0f, 0.03f, 0.0f),
            f32 normalDirectionModifier=100.0f, s32 mbNumber=-1, bool everyMeshVertex=false,
            u32 minParticlesPerSecond=5, u32 maxParticlesPerSecond=10,
            const video::SColor &minStartColor=video::SColor(255, 0, 0, 0),
            const video::SColor &maxStartColor=video::SColor(255, 255, 255, 255),
            u32 lifeTimeMin=2000, u32 lifeTimeMax=4000, s32 maxAngleDegrees=0,
            const core::dimension2df &minStartSize=core::dimension2df(5.0f, 5.0f),
            const core::dimension2df &maxStartSize=core::dimension2df(5.0f, 5.0f));

    /**
     * Create a point emitter. See Irrlicht documentation for parameters.
     * Note: the emitter will automatically be used, no need to manually add it
     */
    virtual IParticlePointEmitter *createPointEmitter(const core::vector3df &direction=core::vector3df(0.0f, 0.03f, 0.0f),
            u32 minParticlesPerSecond=5, u32 maxParticlesPerSecond=10,
            const video::SColor &minStartColor=video::SColor(255, 0, 0, 0),
            const video::SColor &maxStartColor=video::SColor(255, 255, 255, 255),
            u32 lifeTimeMin=2000, u32 lifeTimeMax=4000, s32 maxAngleDegrees=0,
            const core::dimension2df &minStartSize=core::dimension2df(5.0f, 5.0f),
            const core::dimension2df &maxStartSize=core::dimension2df(5.0f, 5.0f));

    /**
     * Create a box emitter. See Irrlicht documentation for parameters.
     * Note: the emitter will automatically be used, no need to manually add it
     */
    virtual IParticleBoxEmitter *createBoxEmitter (const core::aabbox3df &box=core::aabbox3df(-10, 28,-10, 10, 30, 10),
            const core::vector3df &direction=core::vector3df(0.0f, 0.03f, 0.0f),
            u32 minParticlesPerSecond=5, u32 maxParticlesPerSecond=10,
            const video::SColor &minStartColor=video::SColor(255, 0, 0, 0),
            const video::SColor &maxStartColor=video::SColor(255, 255, 255, 255),
            u32 lifeTimeMin=2000, u32 lifeTimeMax=4000, s32 maxAngleDegrees=0,
            const core::dimension2df &minStartSize=core::dimension2df(5.0f, 5.0f),
            const core::dimension2df &maxStartSize=core::dimension2df(5.0f, 5.0f));

    /**
     * Create a ring emitter. See Irrlicht documentation for parameters.
     * Note: the emitter will automatically be used, no need to manually add it
     */
    virtual IParticleRingEmitter *createRingEmitter (const core::vector3df &center, f32 radius, f32 ringThickness,
            const core::vector3df &direction=core::vector3df(0.0f, 0.03f, 0.0f), u32 minParticlesPerSecond=5,
            u32 maxParticlesPerSecond=10, const video::SColor &minStartColor=video::SColor(255, 0, 0, 0),
            const video::SColor &maxStartColor=video::SColor(255, 255, 255, 255), u32 lifeTimeMin=2000,
            u32 lifeTimeMax=4000, s32 maxAngleDegrees=0,
            const core::dimension2df &minStartSize=core::dimension2df(5.0f, 5.0f),
            const core::dimension2df &maxStartSize=core::dimension2df(5.0f, 5.0f));

    /**
     * Create a sphere emitter. See Irrlicht documentation for parameters.
     * Note: the emitter will automatically be used, no need to manually add it
     */
    virtual IParticleSphereEmitter *createSphereEmitter (const core::vector3df &center, f32 radius,
            const core::vector3df &direction=core::vector3df(0.0f, 0.03f, 0.0f),
            u32 minParticlesPerSecond=5, u32 maxParticlesPerSecond=10,
            const video::SColor &minStartColor=video::SColor(255, 0, 0, 0),
            const video::SColor &maxStartColor=video::SColor(255, 255, 255, 255),
            u32 lifeTimeMin=2000, u32 lifeTimeMax=4000, s32 maxAngleDegrees=0,
            const core::dimension2df &minStartSize=core::dimension2df(5.0f, 5.0f),
            const core::dimension2df &maxStartSize=core::dimension2df(5.0f, 5.0f));

    /**
     * Create an explosion emitter.
     * Note: the emitter will automatically be used, no need to manually add it
     * Note2: the explosion emitter will only emitt a given number of particles and stop after that
     */
    virtual CParticleExplosionEmitter *createExplosionEmitter (const core::vector3df &center, f32 radius,
            vector3df vDirection=vector3df(0.0f,0.03f,0.0f), u32 minParticles=5, u32 maxParticles=10,
            const video::SColor &minStartColor=video::SColor(255, 0, 0, 0),
            const video::SColor &maxStartColor=video::SColor(255, 255, 255, 255),
            u32 lifeTimeMin=2000, u32 lifeTimeMax=4000,
            const core::dimension2df &minStartSize=core::dimension2df(5.0f, 5.0f),
            const core::dimension2df &maxStartSize=core::dimension2df(5.0f, 5.0f));

    /**
     * Create an attraction affector. See Irrlicht documentation for parameters.
     * Note: the affector will automatically be added, no need to manually add it
     */
    virtual IParticleAttractionAffector *createAttractionAffector(
            const core::vector3df &point, f32 speed=1.0f, bool attract=true,
            bool affectX=true, bool affectY=true, bool affectZ=true);

    /**
     * Create a fadeout affector. See Irrlicht documentation for parameters.
     * Note: the affector will automatically be added, no need to manually add it
     */
    virtual IParticleFadeOutAffector *createFadeOutParticleAffector(
            const video::SColor &targetColor=video::SColor(0, 0, 0, 0),
            u32 timeNeededToFadeOut=1000);

    /**
     * Create a gravity affector. See Irrlicht documentation for parameters.
     * Note: the affector will automatically be added, no need to manually add it
     */
    virtual IParticleGravityAffector *createGravityAffector(const core::vector3df &gravity=core::vector3df(0.0f,-0.03f, 0.0f), u32 timeForceLost=1000);

    /**
     * Create a rotation affector. See Irrlicht documentation for parameters.
     * Note: the affector will automatically be added, no need to manually add it
     */
    virtual IParticleRotationAffector *createRotationAffector(const core::vector3df &speed=core::vector3df(5.0f, 5.0f, 5.0f), const core::vector3df &pivotPoint=core::vector3df(0.0f, 0.0f, 0.0f));

    /**
     * Create a scale affector. See Irrlicht documentation for parameters.
     * Note: the affector will automatically be added, no need to manually add it
     */
    virtual IParticleAffector *createScaleParticleAffector(const core::dimension2df &scaleTo=core::dimension2df(1.0f, 1.0f));

    /** Remove all affectors */
    virtual void removeAllAffectors();

    /** Register method for rendering */
    virtual void OnRegisterSceneNode();

    /**
     * Get the current emitter
     * @return the current emitter
     */
    virtual IParticleEmitter *getEmitter() { return m_pEmitter; }

    /**
     * Set a new emitter. Note: you do normally not have to use this, will automatically be called
     * upon creation of a new emitter
     * @param pEmitter the new emitter
     */
    virtual void setEmitter(IParticleEmitter* pEmitter);

    /**
     * Add an affector. Note: you do normally not have to use this, will automatically be called
     * upon creation of a new affector
     * @param pAffector the new affector
     */
    virtual void addAffector(IParticleAffector *pAffector);

    /**
     * Get the number of used materials
     * @return 1
     */
    virtual video::SMaterial& getMaterial(u32 i) { return m_pBuffer->getMaterial(); }

    virtual u32 getMaterialCount() const { return 1; }
    virtual void render();

    virtual const core::aabbox3d<f32>& getBoundingBox() const;
    virtual void setParticlesAreGlobal(bool bGlobal=true);
   	virtual void setParticleSize( const dimension2d<f32> &size=core::dimension2d<f32>(5.0f, 5.0f));

   	void doParticleSystem(u32 iTime);

   	const char *getTypeName() const;
   	ESCENE_NODE_TYPE getType() const;

   	virtual void OnAnimate(u32 timeMs);

    void setIsActive(bool b) {
      if (!m_bIsActive && m_eEmitter==eEmitterExplosion)
        ((CParticleExplosionEmitter *)m_pEmitter)->restart();

      m_bIsActive=b;
    }

    bool isActive() { return m_bIsActive || m_aParticles.size()>0; }

    u32 getParticleCount() { return m_aParticles.size(); }

    void setAtlasSize(u32 iSize);
};

/**
 * @class CAdvancedParticleSystemNodeFactory
 * The scene node factory that creates the advanced particle nodes.
 * Register an instance of this class to the scene manager if you use the IrrEdit plugin
 */
class CAdvancedParticleSystemNodeFactory : public ISceneNodeFactory {
  protected:
    ISceneManager *m_pSmgr;

  public:
    CAdvancedParticleSystemNodeFactory(ISceneManager *pSmgr);

    virtual ~CAdvancedParticleSystemNodeFactory() { }

    //ISceneNodeFactory methods
    virtual ISceneNode *addSceneNode(const c8 *typeName, ISceneNode *parent=0);
    virtual ISceneNode *addSceneNode(ESCENE_NODE_TYPE type, ISceneNode *parent=0);
    virtual u32 getCreatableSceneNodeTypeCount() const;
    virtual ESCENE_NODE_TYPE getCreateableSceneNodeType(u32 idx) const;
    virtual const c8 *getCreateableSceneNodeTypeName(ESCENE_NODE_TYPE type) const;
    virtual const c8 *getCreateableSceneNodeTypeName(u32 idx) const;
};
