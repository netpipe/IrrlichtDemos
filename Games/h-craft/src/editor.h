// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef EDITOR_H
#define EDITOR_H

#include "compile_config.h"

#ifdef HC1_ENABLE_EDITOR

#include "irrlicht_manager.h"

class Timer;
struct TrackMarkerSettings;

enum EDIT_MODE
{
    EM_NONE,
    EM_SET_PLAYER_START,
    EM_SELECTED_PLAYER_START,
    EM_SET_FINISH_LINE,
    EM_SELECTED_FINISH_LINE,
    EM_SET_TP_SOURCE,
    EM_SELECTED_TP_SOURCE,
    EM_SET_TP_TARGET,
    EM_SELECTED_TP_TARGET,
    EM_SET_TRACKMARKER,
    EM_INSERT_TRACKMARKER,
    EM_APPEND_TRACKMARKER,
    EM_SELECTED_TRACKMARKER,
    EM_SET_MODEL,
    EM_SELECTED_MODEL,
};

enum EDIT_ROTATION_MODE
{
    ERM_NONE,
    ERM_ONLY_X,
    ERM_ONLY_Y,
    ERM_ONLY_Z,
};

class Editor
{
public:
    Editor();
    ~Editor();

    void Init();
    void Start();
    void Stop();
    void Update();
    bool OnEvent(const irr::SEvent &event);

    void SetMode(EDIT_MODE mode_, int index_);
    EDIT_MODE GetMode() const   { return mMode; }
    int GetEditIndex() const    { return mEditIndex; }

    void MoveCamToTarget(const irr::core::vector3df &target_);

    irr::scene::SMesh * GetDefaultBoxMesh()  { return mDefaultBoxMesh; }
    irr::scene::SMesh * GetDefaultArrowMesh() { return mDefaultArrowMesh; }

    bool CheckTrackCollision(const irr::core::line3d<irr::f32> &line_, irr::core::vector3df &intersection_);   // TODO: belongs somewhere else (game maybe)

protected:
    void UpdateCamera();
    void UpdatePicking();
    void UpdateEditAction();
    void CheckForSelection();

    bool IsSelectionMode();
    void SetSelectedNode(irr::scene::ISceneNode* node_);

    bool CalcTrackMarkerSettingsAtCursor(TrackMarkerSettings & settings_, const irr::core::vector3df &rotation_);

private:
    EDIT_MODE   mMode;
    int         mEditIndex; // depends on mode
    EDIT_ROTATION_MODE  mRotationMode;

    Timer*      mTimer;
    irr::core::position2d<irr::s32> mOldCursorPos;
    irr::core::vector3df         	mCursorPos3D;   // that one is always above tracks - TODO: rename it
    bool		mIsCursorPosCurrent;   // is it below the mouse-cursor (which could also point to the void)
    bool        mActionButtonPressed;

    irr::scene::SMesh * mDefaultBoxMesh;
    irr::scene::SMesh * mDefaultArrowMesh;
    irr::scene::ISceneNode* mSelectedNode;
};

#endif // HC1_ENABLE_EDITOR

#endif // EDITOR_H

