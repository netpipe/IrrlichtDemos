#ifndef _C_PROJECTS_WINDOW
  #define _C_PROJECTS_WINDOW

  #include <irrlicht.h>
  #include <CDialogWindow.h>
  #include <CTextureParameters.h>

using namespace irr;

/**
 * @class CProjects
 * @author Christian Keimel / bulletbyte.de
 * This class is the implementation of the "select project" dialog
 */
class CProjects : public CDialogWindow {
  protected:
    /**
     * @class CProject
     * @author Christian Keimel / bulletbyte.de
     * A little class that holds all information about the projects
     * you can select in the "select project" dialog.
     */
    class CProject {
      public:
        core::stringc m_sName,        /**<! name of the project */
                      m_sProjectRoot, /**<! root path (aka "path of the executable") of the project */
                      m_sTexturePath, /**<! texture path of the project */
                      m_sSavePath,    /**<! save path of the project */
                      m_sScene;       /**<! the loaded scene */

        CProject() {
          m_sProjectRoot="";
          m_sTexturePath="";
          m_sSavePath="";
        }
        
      /**
       * Save the project to the attributes
       * @param out the attributes
       */
      void save(io::IAttributes *out) {
        out->addString("Name"    ,m_sName       .c_str());
        out->addString("Root"    ,m_sProjectRoot.c_str());
        out->addString("Textures",m_sTexturePath.c_str());
        out->addString("SaveTo"  ,m_sSavePath   .c_str());
        out->addString("Scene"   ,m_sScene      .c_str());
      }
      
      /**
       * Load the project from the attributes
       * @param in the attributes
       */
      void load(io::IAttributes *in) {
        m_sName       =in->getAttributeAsString("Name"    );
        m_sProjectRoot=in->getAttributeAsString("Root"    );
        m_sTexturePath=in->getAttributeAsString("Textures");
        m_sSavePath   =in->getAttributeAsString("SaveTo"  );
        m_sScene      =in->getAttributeAsString("Scene"   );
      }
    };

    core::stringc m_sEmpty,     /**<! empty string */
                  m_sSelected,  /**<! selected project name */
                  m_sScene;     /**<! the loaded scene */

    core::array<CProject *> m_aProjects;  /**<! array of loaded projects */
    CProject *m_pSelected;                /**<! the selected project */

    gui::IGUIListBox *m_pProjectList; /**<! the "available projects" list */
    gui::IGUIEditBox *m_pProjectName, /**<! the "project name" edit field */
                     *m_pProjectRoot, /**<! the "project root path" edit field */
                     *m_pTexturePath, /**<! the "texture path" edit field */
                     *m_pSavePath;    /**<! the "save path" edit field */
    gui::IGUIButton  *m_pNew,         /**<! the "new project" button */
                     *m_pDel,         /**<! the "delete project" button */
                     *m_pSelect;      /**<! the "select project" button */

    IrrlichtDevice *m_pDevice;    /**<! the Irrlicht device */
    
    void updateElements();  /**<! update enabled flag of GUI elements depending on the current state */

  public:
    /**
     * The constructor
     * @param pDevice the Irrlicht device
     */
    CProjects(IrrlichtDevice *pDevice);
    
    /**
     * Irrlicht event receiver callback
     * @param event the event to handle
     */
    virtual bool OnEvent(const SEvent &event);
    
    /**
     * Get the project root path of the selected project
     * @return the project root path of the selected project or an empty string if no project was selected
     */
    const core::stringc &getSelectedProjectRoot() {
      return m_pSelected!=NULL?m_pSelected->m_sProjectRoot:m_sEmpty;
    }

    /**
     * Get the texture path of the selected project
     * @return the texture path of the selected project or an empty string if no project was selected
     */
    const core::stringc &getSelectedTexturePath() {
      return m_pSelected!=NULL?m_pSelected->m_sTexturePath:m_sEmpty;
    }

    /**
     * Get the save path of the selected project
     * @return the save path of the selected project or an empty string if no project was selected
     */
    const core::stringc &getSelectedSavePath() {
      return m_pSelected!=NULL?m_pSelected->m_sSavePath:m_sEmpty;
    }
    
    /**
     * Save a project to an attributes object
     * @param pOut the attributes object
     * @param iProject index of the project to save
     */
    void saveProject(io::IAttributes *pOut, u32 iProject);
    
    /**
     * Load a project from an attributes object
     * @param pIn the attributes object
     */
    void loadProject(io::IAttributes *pIn);
    
    /**
     * Set the selected project
     * @param sSelected name of the selected project
     */
    void setSelected(const core::stringc sSelected) { m_sSelected=sSelected; }
    
    /**
     * Get the selected project
     * @return sSelected name of the selected project
     */
    const core::stringc getSelected() { return m_sSelected; }
    
    /**
     * Update the list of projects list
     */
    void updateProjectList();
    
    /**
     * Update the complete UI
     */
    void updateUI();
    
    /**
     * Query the number of projects
     * @return the number of projects
     */
    u32 getProjectCount() { return m_aProjects.size(); }

    /**
     * This method returns the string that's used by the main
     * routine to save the position of this window
     * @return the name of this window for the savefile
     */
    virtual const core::stringc &getSaveName() {
      static core::stringc sSaveName="SelectProject";
      return sSaveName;
    }
    
    void setScene(const c8 *sScene) { if (m_pSelected!=NULL) m_pSelected->m_sScene=sScene; }
    
    const c8 *getScene() { return m_pSelected!=NULL?m_pSelected->m_sScene.c_str():NULL;  }
};

#endif
