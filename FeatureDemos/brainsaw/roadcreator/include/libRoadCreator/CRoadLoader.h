#ifndef _C_ROAD_LOADER
  #define _C_ROAD_LOADER

  #include <irrlicht.h>
  #include <CSurface.h>
  #include <CTextureParameters.h>

using namespace irr;

class CSegment;
class CSurface;
class CConnection;

class CRoadLoader {
  protected:
    IrrlichtDevice *m_pDevice;

    core::list<CSegment *> m_lSegments;         /**<! list of road segments */
    core::list<CConnection *> m_lConnections;   /**<! list of connections */

    CSurface *m_pSurface;
    
    core::vector3df m_vOfffset;     /**<! the Offset of the node (only relevant when shrinking the node) */
    bool m_bShrinkNode;
    
    CTextureParameters m_aSurfaceDefaults[_SURFACE_NUMBER_OF_BUFFERS];

    core::stringc m_sCurrentRoad;               /**<! the currently opened road */

    void addBufferToArray(scene::IMeshBuffer *p, core::array<scene::IMeshBuffer *> &aBuffers);

    bool sameMaterial(const video::SMaterial &m1, const video::SMaterial &m2);

  public:
    CRoadLoader(IrrlichtDevice *pDevice);
    
    ~CRoadLoader() {
      if (m_pSurface!=NULL) delete m_pSurface;
    }
    
    void setSurfaceDefaultParameters(CTextureParameters *p) {
      if (p==NULL) return;
      for (u32 i=0; i<_SURFACE_NUMBER_OF_BUFFERS; i++) p[i].copyTo(&m_aSurfaceDefaults[i]);
    }

    bool loadRoad(const core::stringc sName);

    /**
     * This method saves a road to a file
     * @param sName the file to save the road to
     * @see CSegment::save
     * @see CConnection::save
     * @see CTextureParameters::save
     */
    void saveRoad();

    /**
     * Remove a road from memory
     */
    void destroyRoad();

    core::list<CSegment    *> &getSegments   ();
    core::list<CConnection *> &getConnections();

    const core::stringc getCurrentRoadName();
    void setCurrentRoadName(const core::stringc s);

    scene::IAnimatedMesh *createMesh();

    bool deleteConnection(CConnection *pToDelete);

    bool deleteSegment(CSegment *pToDelete);

    CSurface *getSurface() { 
      if (m_pSurface==NULL) m_pSurface=new CSurface(m_pDevice,m_aSurfaceDefaults);
      return m_pSurface; 
    }
    
    void shrinkNode(bool b) { m_bShrinkNode=b; }
    bool didShrinkNode() { return m_bShrinkNode; }
    const core::vector3df &getOffset() { return m_vOfffset; }
};

#endif
