#ifndef _C_RANDOM_FOREST_NODE
  #define _C_RANDOM_FOREST_NODE

  #include <irrlicht.h>

using namespace irr;

const int RANDOM_FOREST_ID=MAKE_IRR_ID('r','a','f','o');

class CRandomForest : public irr::scene::ISceneNode {
  protected:
    class CCheckRect {
      public:
        irr::core::rect<irr::s32> m_cRect;
        irr::core::position2df m_cOffset;
        irr::u32 m_iIndex;
        irr::f32 m_fResize;
        bool m_bTree;

        CCheckRect(irr::core::rect<irr::s32> cRect, irr::f32 fResize) {
          m_cRect=cRect;
          m_bTree=false;
          m_fResize=fResize;
          m_cOffset=irr::core::position2df(0.0f,0.0f);
        }
    };

    irr::video::IVideoDriver *m_pDriver;
    irr::video::SMaterial m_cMat;

    irr::scene::ISceneManager *m_pSmgr;
    irr::scene::ISceneNode *m_pTemplate,
                           *m_pParentNode;
    irr::scene::ITerrainSceneNode *m_pTerrain;

    irr::core::array<irr::core::position2di> m_aPoly;
    irr::core::array<CCheckRect *> m_aRects;
    irr::core::array<irr::scene::ISceneNode *> m_aTrees;
    irr::core::array<irr::video::SColor> m_aColors;
    irr::core::array<irr::u32> m_aSeed;
    irr::core::aabbox3df m_cBox;
    irr::core::stringc m_sTemplate,
                       m_sTerrain,
                       m_sParentName;
    irr::core::array<ISceneNode *> m_aGeneratedTrees;

    irr::u32 m_iStepsizeX,
             m_iStepsizeY,
             m_iThreshold,
             m_iNumTrees;

    irr::f32 m_fResize;

    bool m_bSerializeTrees;

    #ifdef _IRREDIT_PLUGIN
      irr::scene::IAnimatedMesh *m_pMesh;   /**< the mesh to be renderer in IrrEdit */
    #endif

    /**
     * @class CRandom
     * A pseudo random generator. It is important to have that generator here
     * to not rely on various C++ compilers to use the same algorithms in their
     * built in generators
     */
    class CRandom {
      protected:
        irr::u32 *m_aMatrix;
        irr::s32 m_iN,
                 m_iM,
                 m_iMatrixIdx;

        void init(unsigned long s);
      public:
        CRandom(irr::core::array<irr::u32> aInit);

        ~CRandom();

        irr::u32 next();
    };

    /**
     * Calculate the 2d angle between two position points
     * @param cPos1 the first position
     * @param cPos2 the second position
     * @return the angle between the 2d vectors
     */
    irr::f32 Angle2D(irr::core::position2di cPos1, irr::core::position2di cPos2);

    /**
     * Determine whether or not a point is within a polygon
     * @param polygon an array of 2d positions that form a polygon
     * @param p the position
     * @return "true" if the point is inside the polygon, "false" otherwise
     */
    bool InsidePolygon(irr::core::array<irr::core::position2di> polygon, irr::core::position2di p);

    /**
     * Determine whether or not a rectanble is within a polygon, i.e. all corners of the rectangle
     * are inside.
     * @param polygon array of 2d positions that make up the polygon
     * @param rect the rectangle
     * @return "true" if the rectangle is inside the polygon, "false" otherwise
     * @see CRandomForest::InsidePolygon
     */
    bool RectInsidePolygon(irr::core::array<irr::core::position2di> polygon, irr::core::rect<irr::s32> rect);

  public:
    CRandomForest(irr::scene::ISceneNode *pParent,irr::scene::ISceneManager *pSmgr,irr::s32 iId=-1,
                const irr::core::vector3df &cPos=irr::core::vector3df(0.0f,0.0f,0.0f),
                const irr::core::vector3df &cRot=irr::core::vector3df(0.0f,0.0f,0.0f),
                const irr::core::vector3df &cScale=irr::core::vector3df(0.0f,0.0f,0.0f));

    ~CRandomForest();

    void setSeed(irr::core::array<irr::u32> aSeed);

    void setStepSize(irr::u32 x, irr::u32 y);

    void setThreshold(irr::u32 i);

    void addToPolygon(irr::core::position2di newVert);

    void clearForest();
    void createForest();

    virtual void OnRegisterSceneNode();

    virtual void render();

    virtual const irr::core::aabbox3d<irr::f32> &getBoundingBox() const;

    virtual void deserializeAttributes(irr::io::IAttributes *in, irr::io::SAttributeReadWriteOptions *options=0);

    virtual void serializeAttributes(irr::io::IAttributes *out, irr::io::SAttributeReadWriteOptions *options=0) const;

    void setTemplateNodeName(const irr::c8 *sTemplate);
    void setTerrainNodeName(const irr::c8 *sTerrain);

    void plantTrees();

    void setResize(irr::f32 f) { m_fResize=f; }

    virtual irr::scene::ESCENE_NODE_TYPE getType() const {
      return (irr::scene::ESCENE_NODE_TYPE)RANDOM_FOREST_ID;
    }

    virtual irr::video::SMaterial &getMaterial(irr::u32 iIdx) { return m_cMat; }

    virtual irr::u32 getMaterialCount() { return 1; }

    const irr::core::array<irr::scene::ISceneNode *> getGeneratedTrees() { return m_aGeneratedTrees; }
};


class CRandomForestFactory : public irr::scene::ISceneNodeFactory {
  private:
    irr::scene::ISceneManager *m_pSmgr;
    irr::core::stringc m_sTypeName;

  public:
    CRandomForestFactory(irr::scene::ISceneManager *pManager);
    virtual irr::scene::ISceneNode *addSceneNode (irr::scene::ESCENE_NODE_TYPE type, irr::scene::ISceneNode *parent=0);
    virtual irr::scene::ISceneNode *addSceneNode (const irr::c8 *typeName, irr::scene::ISceneNode *parent=0);
    virtual irr::u32 getCreatableSceneNodeTypeCount() const;
    virtual irr::scene::ESCENE_NODE_TYPE getCreateableSceneNodeType (irr::u32 idx) const;
    virtual const irr::c8 *getCreateableSceneNodeTypeName(irr::scene::ESCENE_NODE_TYPE type) const;
    virtual const irr::c8 *getCreateableSceneNodeTypeName(irr::u32 idx) const;
    virtual ~CRandomForestFactory();
};

#endif
