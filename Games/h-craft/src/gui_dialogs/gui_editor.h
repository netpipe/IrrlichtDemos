// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GUI_EDITOR_H
#define GUI_EDITOR_H

#include "../compile_config.h"
#include <string>

#ifdef HC1_ENABLE_EDITOR

#include "../irrlicht_manager.h"

class GuiNodeManager;
class GuiModelList;

class GuiEditor
{
public:
    friend class Editor;
    friend class GuiModelList;

    GuiEditor(irr::gui::IGUIEnvironment* env_);
    ~GuiEditor();

    void CreateEditorWnd();
    void RemoveEditorWnd();
    bool OnEvent(const irr::SEvent &event);

protected:
    void OnPlayerStart();
    void OnTrackMarker();
    void OnTeleportSource();
    void OnTeleportTarget();
    void OnFinishLine();
    void OnSounds();
    void OnAi();
    void OnObjects();
    void OnSaveLevel();
    void OnNodes();

    bool OnEventPlayerStart(const irr::SEvent &event);
    bool OnEventTrackMarker(const irr::SEvent &event);
    bool OnEventTeleportSource(const irr::SEvent &event);
    bool OnEventTeleportTarget(const irr::SEvent &event);
    bool OnEventFinishLine(const irr::SEvent &event);
    bool OnEventSounds(const irr::SEvent &event);
    bool OnEventObjects(const irr::SEvent &event);
    void OnModelSelected(std::wstring name_);

    void UpdatePlayerStart();
    void UpdateTrackMarker();
    void UpdateTeleportSource();
    void UpdateTeleportTarget();
    void UpdateFinishLine();
    void UpdateSounds();
    void UpdateObjects();

    bool RemoveAllSubWindows(irr::gui::IGUIWindow* checkExistanceWnd_);

private:
    irr::gui::IGUIEnvironment*   mEnvironment;
    irr::gui::IGUIWindow* mWndEditor;

    irr::gui::IGUIWindow* mWndPlayerStart;
    irr::gui::IGUIWindow* mWndTrackMarker;
    irr::gui::IGUIWindow* mWndTeleportSource;
    irr::gui::IGUIWindow* mWndTeleportTarget;
    irr::gui::IGUIWindow* mWndFinishLine;
    irr::gui::IGUIWindow* mWndSounds;
    irr::gui::IGUIWindow* mWndObjects;

    GuiNodeManager* mGuiNodeManager;
    GuiModelList *mGuiModelList;

    float mChangeMarkerSize;
    float mChangeModelPos;
    float mChangeModelRot;
};

#endif // HC1_ENABLE_EDITOR

#endif // GUI_EDITOR_H
