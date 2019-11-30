#ifndef _C_SEGMENT
  #define _C_SEGMENT

  #include <irrlicht.h>
  #include <IRoadPart.h>

#define _SEGMENT_NUMBER_OF_BUFFERS 18

using namespace irr;

class INotification;
class CMaterialManager;
class CTextureParameters;

/**
 * @class CSegment
 * This class holds all data of the segments that will then be connected
 * by connections
 * @author Christian Keimel / bulletbyte.de
 * @see CConnection
 */
class CSegment : public IRoadPart {
  protected:
    f32 m_fWidth,       /**<! width of the road segment */
        m_fLength,      /**<! length of the road segment */
        m_fBaseOffset,  /**<! height of the road segment */
        m_fWallHeight,  /**<! height of the walls */
        m_fWallWidth;   /**<! width of the walls */

    bool m_bLevelBase,  /**<! bring all base points to the same Y-value? */
         m_bNormalBase, /**<! use normal of segment for base direction? */
         m_bNormalWall, /**<! use normal of segment for wall direction? */
         m_bWalls[4],   /**<! create walls? */
         m_bCorner[4];  /**<! create corner marks? */

    core::vector3df m_vPosition,      /**<! position of the segment */
                    m_vDirection,     /**<! direction of the segment */
                    m_vNormal,        /**<! normal (upwards) of the segment */
                    m_vPoints[4],     /**<! the 4 points that define the upper polygon of the segment */
                    m_vWallNorm,      /**<! the wall normal */
                    m_vBaseNorm;      /**<! the basement normal */


    IrrlichtDevice *m_pDevice;
    io::IFileSystem *m_pFs;
    video::IVideoDriver *m_pDrv;    /**<! the videodriver */
    core::stringc m_sName;          /**<! name of the segment */

    core::list<INotification *> m_lNotify;  /**<! list of objects that get notifications on changes of the segment */

    scene::IMeshBuffer *m_pBuffer[_SEGMENT_NUMBER_OF_BUFFERS];      /**<! meshbuffers of the segment */
    CTextureParameters *m_pTexParams[_SEGMENT_NUMBER_OF_BUFFERS],   /**<! texture parameters of the segment */
                       *m_pTexInit[_SEGMENT_NUMBER_OF_BUFFERS];

    /**
     * This method recalculates the meshbuffers and notifies all registered listeners of the modification
     * @see CSegment::m_lNotify
     * @see CSegment::setWidth
     * @see CSegment::setLength
     * @see CSegment::setBaseOffset
     * @see CSegment::setLevelBase
     * @see CSegment::setPosition
     * @see CSegment::setDirection
     * @see CSegment::setNormal
     */
    void attributeChanged();

    /**
     * This method recalculates the segment's meshbuffers
     * @see CSegment::CSegment
     * @see CSegment::attributeChanged
     * @see CSegment::load
     * @see CSegment::update
     */
    virtual void recalcMeshBuffer();

    /**
     * This method takes an array of vectors and converts them to vertices
     * @param vec the array of vectors
     * @param pTex the texture parameters to be used
     * @param vert the output vertex array
     */
    void fillVertexArray(core::vector3df vec[], CTextureParameters *pTex, video::S3DVertex *vert, bool bTop, core::vector3df vNorm=core::vector3df(0,0,0));

    core::array<core::vector3df> vTemp[_SEGMENT_NUMBER_OF_BUFFERS];
  public:
    /**
     * The construtor. This one is used by the GUI of the editor
     * @param sName name of the segment
     * @param vPosition position of the segment
     * @param pDrv the Irrlicht video driver
     * @param pInitParam the initial texture parameters
     */
    CSegment(core::stringc sName, core::vector3df vPosition, IrrlichtDevice *pDevice, CTextureParameters *pInitParam);
    /**
     * The construtor. This one is used by the road loading routine
     * @param pDrv the Irrlicht video driver
     */
    CSegment(IrrlichtDevice *pDevice);

    /**
     * The destructor
     */
    virtual ~CSegment();

    void setWidth (f32 f);  /**<! set the width of the segment */
    void setLength(f32 f);  /**<! set the length of the segment */

    f32 getWidth ();  /**<! get the width of the segment */
    f32 getLength();  /**<! get the length of the segment */

    void setBaseOffset(f32 f);  /**<! set the base offset of the segment */
    f32 getBaseOffset();        /**<! get the base offset of the segment */

    void setLevelBase(bool b);  /**<! set the base level flag of the segment */
    bool getLevelBase();        /**<! get the base level flag of the segment */

    void setNormalBase(bool b);
    bool getNormalBase();

    const core::vector3df &getNormalBaseVector() { return m_vBaseNorm; }

    void setPosition (core::vector3df v); /**<! set the position of the segment */
    void setDirection(core::vector3df v); /**<! set direction of the segment */
    void setNormal   (core::vector3df v); /**<! set normal of the segment */

    const core::vector3df &getPosition ();  /**<! get the position of the segment */
    const core::vector3df &getDirection();  /**<! get the direction of the segment */
    const core::vector3df &getNormal   ();  /**<! get the normal of the segment */

    void setName(const core::stringc &sName); /**<! set the name of the segment */

    const core::stringc &getName();   /**<! get the name of the segment */

    virtual void render();  /**<! render the segment (for editor use only) */

    const core::vector3df &getPoint(u32 i);   /**<! get one of the upper points of the segment */

    virtual void save(io::IAttributes *out);  /**<! save the segment */
    virtual void load(io::IAttributes *in);   /**<! load the segment */

    void addNotify(INotification *p);     /**<! add a notifier that will be notified on changes of the segment */
    void delNotify(INotification *p);     /**<! remove a notifier */

    void update();  /**<! update the meshbuffers */

    virtual CTextureParameters *getTextureParameters(u32 i);  /**<! get one of the texture parameter objects */

    virtual u32 getTextureCount() { return 6; }

    virtual scene::IMeshBuffer *getMeshBuffer(u32 i);   /**<! get one of the meshbuffers */

    void setCornerFlag(u32 idx, bool b) { if (idx<4) { m_bCorner[idx]=b; attributeChanged(); } }
    void setWallFlag(u32 idx, bool b) { if (idx<4) { m_bWalls[idx]=b; attributeChanged(); } }
    void setWallHeight(f32 f) { m_fWallHeight=f; attributeChanged(); }
    void setWallWidth(f32 f) { m_fWallWidth=f; attributeChanged(); }

    bool getCornerFlag(u32 idx) { return idx<4?m_bCorner[idx]:false; }
    bool getWallFlag(u32 idx) { return idx<4?m_bWalls[idx]:false; }
    f32 getWallHeight() { return m_fWallHeight; }
    f32 getWallWidth() { return m_fWallWidth; }

    const core::vector3df &getWallNormal() { return m_vWallNorm; }

    virtual s32 getNumberOfMeshBuffers() { return _SEGMENT_NUMBER_OF_BUFFERS; }
    
    CSegment *clone();
    
    void setNormalWall(bool b);
    bool getNormalWall();
};

#endif
