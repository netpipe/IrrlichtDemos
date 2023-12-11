#ifndef _C_ALIGN_DIALOG
  #define _C_ALIGN_DIALOG

  #include <irrlicht.h>

  #include <CDialogWindow.h>

class CSegment;

using namespace irr;

/**
 * @class CAlignDialog
 * @author Christian Keimel / bulletbyte.de
 * This is the dialog to align segments to scene nodes
 */
class CAlignDialog : public CDialogWindow {
  protected:
    IrrlichtDevice *m_pDevice;
    video::IVideoDriver *m_pDrv;
    scene::ISceneManager *m_pSmgr;
    
    gui::IGUIStaticText *m_lblSegment,
                        *m_lblSceneNode,
                        *m_lblMatCnt,
                        *m_lblVtxCnt;
    gui::IGUISpinBox *m_pMaterial,
                     *m_pVertex,
                     *m_pSegVtx;
    gui::IGUIButton *m_pAlign,
                    *m_pPickNode;
    
    scene::ISceneNode *m_pSceneNode,
                      *m_pHoverNode;
    CSegment *m_pSegment;
    
    bool m_bSelect;
    
    u32 m_iNodeMaterial,
        m_iNodeVertex,
        m_iSegNode,
        m_iState;;
    
    core::vector3df m_vNode,
                    m_vSeg;
    
    void materialSelect();
    void vertexSelect();
    void segmentSelect();
    
    core::aabbox3df m_cBoxNode,
                    m_cBoxSeg;
    
    void toggleState() {
      m_iState=m_iState==0?1:0;
      if (m_iState==0 && m_pHoverNode) m_pHoverNode->setMaterialFlag(video::EMF_LIGHTING,false);
      m_bSelect=false;
    }
    
  public:
    CAlignDialog(IrrlichtDevice *pDevice);
    
    /**
     * Callback method for the Irrlicht event receiver
     * @param event the event to handle
     */
    virtual bool OnEvent(const SEvent &event);

    /**
     * Get the name of the dialog to load/save the size
     * @return the name of the dialog
     */
    virtual const core::stringc &getSaveName() {
      static core::stringc s="AlignDialog";
      return s;
    }
    
    void setSelectedNode(scene::ISceneNode *p);
    
    void setSelectedSegment(CSegment *p);
    
    scene::ISceneNode *getSelectedNode() { return m_pSceneNode; }
    
    void draw(bool bLeftBtn, s32 iMouseX, s32 iMouseY);
    
    virtual void setVisible(bool b);
};

#endif
