// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_NODEMANAGER_H
#define GUI_NODEMANAGER_H

#include "../irrlicht_manager.h"

struct NodeSettingsUnknown;
struct NodeSettingsMesh;
struct NodeSettingsAnimatedMesh;
struct NodeSettingsLight;

class GuiNodeManager
{
public:
    GuiNodeManager( irr::gui::IGUIEnvironment * env_);
    virtual ~GuiNodeManager();

    void CreateNodeWindow();
    void RemoveNodeWindow();
    void InfoNodeWindowRemoved();
    irr::gui::IGUIWindow* GetNodeWindow() { return mWndNodeManager; }

    bool OnEvent(const irr::SEvent &event);

protected:
    void Update();
    void UpdateNodeType();
    void UpdateUnknown(NodeSettingsUnknown * node_);
    void UpdateMesh(NodeSettingsMesh * node_);
    void UpdateAnimatedMesh(NodeSettingsAnimatedMesh * node_);
    void UpdateLight(NodeSettingsLight * node_);

    NodeSettingsUnknown *GetActiveNode();

    enum Mode
    {
        GNM_NONE,
        GNM_MODELS,
        GNM_NODES,
    };

    void SetMode(Mode mode_);

private:
    irr::gui::IGUIEnvironment * mEnvironment;
    irr::gui::IGUIWindow*       mWndNodeManager;
    irr::core::rect<irr::s32>   mRectWindow;

    Mode        mMode;
    int         mNodeType;      // NODE_TYPE or -1
    int         mActiveModel;
    irr::scene::ISceneNode*  mNodeActiveModel;
    int         mActiveNode;

    int         mEditTop;
    int         mEditNextTop;
};


#endif // GUI_NODEMANAGER_H
