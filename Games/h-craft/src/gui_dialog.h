// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_DIALOG_H
#define GUI_DIALOG_H

#include <vector>
#include <string>
#include <map>
#include "irrlicht_manager.h"

#include "IGUITextSlider.h"
#include "EGUIEventTypesHC.h"

class TiXmlElement;

class IEventFunctor;
class Config;

class GuiDialog
{
public:
    GuiDialog(const Config& config);
    virtual ~GuiDialog();

    virtual bool Load(const char* filename_, bool reloadLast_=false);

    virtual void Show();
    virtual void Hide();
	bool IsVisible() const { return mIsVisible; }

    virtual void Enable();
    virtual void Disable()
    ;
    virtual void SetActiveFocus();  // mostly default focus, but can be another in special cases (like default disabled)
    virtual void OnUpdate(irr::u32 timeTick) {}	// only called when SetCallOnUpdate is set to true

    irr::gui::IGUIElement* GetDefaultFocus() const  { return mDefaultFocus; }

    // Check if this element might receive the focus
    static bool AllowedToGetFocus(irr::gui::IGUIElement* element)
    {
    	return element && element->isTabStop() && element->isVisible() && element->isEnabled() && element->getTabOrder() >= 0;
    }

    // Some dialogs don't need 3d rendering in the background, which would only slow them down
    bool GetSuppressSceneRendering() const              { return mSuppressSceneRendering && mIsVisible; }
    void SetSuppressSceneRendering(bool suppress_ )     { mSuppressSceneRendering = suppress_; }

    // Call the OnUpdate function each frame when the dialog is active. Default is false.
    void SetCallOnUpdate(bool doCallUpdate)	 { mDoCallOnUpdate = doCallUpdate; }
    bool GetCallOnUpdate() const			 { return mDoCallOnUpdate; }

    // Find an element in this dialog by it's name
    // Careful: Slow - so don't use in speed-critical parts.
    irr::gui::IGUIElement* GetElementByName(irr::gui::IGUIElement* parent_, const std::string& name_, const std::string& error_=std::string("")) const;

protected:

    void Clear();

    //! Add a functor which is called for the given elementID for irr::EET_GUI_EVENT events
    //! This class will control the memory for the functor_ from now on.
    //! For now only a single functor can be used for each elementId_
    void AddGuiEventFunctor(int elementId_, IEventFunctor * functor_ );
    void RemoveGuiEventFunctor(int elementId_);

    int GetIdForName(const std::string& name_) const;

	irr::video::ITexture * GetGuiTexture(const char * name);

	// Get the skin from xml
	void CreateSkinFromXml(TiXmlElement * xmlParent_);
	// Create the style templates from xml
	void CreateStyleTemplates(TiXmlElement * xmlParent_);
	// Add the gui-elements from xml to the guiParent_
    void CreateElementsFromXml(TiXmlElement * xmlParent_, irr::gui::IGUIElement* guiParent_);

    // Insert style templates into the element if it uses those
    void InsertStyleTemplate(TiXmlElement * xmlElement_);

    // Add gui-elements from xml
    irr::gui::IGUIElement* AddButton(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddImage(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddStatic(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddCheckBox(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddEditBox(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddListBox(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddScrollBar(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddToolBar(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddWindow(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddSpinBox(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUIElement* AddTextSlider(TiXmlElement * xmlElement_, irr::gui::IGUIElement* guiParent_);
    irr::gui::IGUISkin* AddSkin(TiXmlElement * xmlElement_);

    bool ReadScaledRect(irr::core::rect<irr::s32>& rect_, TiXmlElement * xmlElement_);
    irr::gui::IGUIFont* ReadScaledFont(TiXmlElement * xmlElement_);
    int ReadNameForId(TiXmlElement * xmlElement_);
    irr::core::stringw ReadText(TiXmlElement * xmlElement_) const;

    irr::gui::IGUIEnvironment * GetEnvironment();
    irr::gui::IGUIElement* GetDialogParent();
    irr::video::IVideoDriver* GetDriver();
    irr::io::IFileSystem * GetFileSystem();

    bool IsLoaded() const   { return mIsLoaded; }

    irr::gui::IGUIElement* GetAlternateDefaultFocus() const         { return mAlternateDefaultFocus; }
    void SetAlternateDefaultFocus( irr::gui::IGUIElement*element_)  { mAlternateDefaultFocus = element_; }

    void BringElementsToFront();

    const Config& GetConfig() const { return mConfig; }

private:
	const Config& mConfig;

    static int mHighestId;
    int mHighestFocusId;
    typedef std::map<std::string, int> IdNameMap;
    IdNameMap mIdNameMap;

    static int mDefaultScreenWidth;
    static int mDefaultScreenHeight;
    std::vector<irr::gui::IGUIElement*> mGUIElements;
    irr::gui::IGUISkin * mSkin;
    irr::gui::IGUISkin * mOldSkin;
    irr::gui::IGUIElement* mDefaultFocus;
    irr::gui::IGUIElement* mAlternateDefaultFocus;
    std::string mFilename;
    bool    mIsLoaded;
    bool    mSuppressSceneRendering;
    bool    mIsVisible;
    bool	mDoCallOnUpdate;
    typedef std::map<std::string, const TiXmlElement*> StyleXmlMap;
    StyleXmlMap mStyleMap;
};

class TestDialog : public GuiDialog
{
    TestDialog(const Config& config);
    bool EventCallback(const irr::SEvent &event_);
};

#endif // GUI_DIALOG_H
