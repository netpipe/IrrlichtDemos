// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "editor.h"

#ifdef HC1_ENABLE_EDITOR

#include "main.h"
#include "timer.h"
#include "device_keyboard.h"
#include "device_mouse.h"
#include "input_device_manager.h"
#include "game.h"
#include "physics.h"
#include "irrlicht_manager.h"
#include "gui.h"
#include "gui_dialogs/gui_editor.h"
#include "config.h"
#include "level.h"
#include "node_manager.h"
#include "camera.h"
#include <math.h>
#include <float.h>
#include <assert.h>

using namespace irr;

Editor::Editor()
{
    mTimer = NULL;
    mIsCursorPosCurrent = false;
    mActionButtonPressed = false;
    mMode = EM_NONE;
    mEditIndex = -1;
    mDefaultBoxMesh = NULL;
    mDefaultArrowMesh = NULL;
    mSelectedNode = NULL;
    mRotationMode = ERM_NONE;
}

Editor::~Editor()
{
    delete mTimer;
    if ( mDefaultBoxMesh )
		mDefaultBoxMesh->drop();
	if ( mDefaultArrowMesh )
		mDefaultArrowMesh->drop();
}

void Editor::Init()
{
    mTimer = new Timer(APP.GetIrrlichtManager()->GetIrrlichtTimer());
    mTimer->Stop();

    // mDefaultBoxMesh
    float size = 12.5f;
    core::vector3df radius(size, size, size);
    core::aabbox3d<f32> box;
    box.addInternalPoint(-radius);
    box.addInternalPoint(radius);
    scene::SMeshBuffer * boxBuffer = APP.GetIrrlichtManager()->CreateBoxMeshBuffer(box);
    assert(boxBuffer);
    mDefaultBoxMesh = new scene::SMesh();
    assert(mDefaultBoxMesh);
    mDefaultBoxMesh->addMeshBuffer(boxBuffer);
    boxBuffer->drop();
    mDefaultBoxMesh->recalculateBoundingBox();

    // mDefaultArrowMesh;
    float arrowLength = 50.f;
    float arrowWidth = 50.f;
    float arrorHeight = 25.f;
    scene::SMeshBuffer * arrowBuffer = APP.GetIrrlichtManager()->CreateArrowMeshBuffer(arrowLength, arrowWidth, arrorHeight);
    assert(arrowBuffer);
    mDefaultArrowMesh = new scene::SMesh();
    assert(mDefaultArrowMesh);
    mDefaultArrowMesh->addMeshBuffer(arrowBuffer);
    arrowBuffer->drop();
    mDefaultArrowMesh->recalculateBoundingBox();
}

void Editor::Start()
{
    mIsCursorPosCurrent = false;
    mTimer->Start();
    APP.GetGui()->GetGuiEditor()->CreateEditorWnd();
}

void Editor::Stop()
{
    SetSelectedNode(NULL);
    SetMode(EM_NONE, -1);
    APP.GetGui()->GetGuiEditor()->RemoveEditorWnd();
    mTimer->Stop();
}

void Editor::Update()
{
    if ( APP.GetMode() != MODE_EDITOR )
        return;

    mTimer->Tick();

    bool camMouseRotation = false;
    gui::ICursorControl* cursor = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getCursorControl();
    assert(cursor);
    if ( !cursor->isVisible() && mRotationMode == ERM_NONE )
    {
        camMouseRotation = true;
    }
    Camera::UpdateEditCam(mTimer->GetLastTickInSeconds(), APP.GetIrrlichtManager()->GetEditorCamera(), camMouseRotation );

    UpdatePicking();
    UpdateEditAction();
}

void Editor::SetMode(EDIT_MODE mode_, int index_)
{
    mMode = mode_;
    mEditIndex = index_;
}

bool Editor::CalcTrackMarkerSettingsAtCursor(TrackMarkerSettings & settings_, const core::vector3df &rotation_)
{
    if ( !mIsCursorPosCurrent )
        return false;

    core::vector3df target;
    core::vector3df repulsion;
    if ( APP.GetPhysics()->CalcSphereAboveTrack(mCursorPos3D, target, repulsion, APP.GetConfig()->GetHoverRadius()) )
    {
        settings_.mIsValid = true;
        settings_.mCenter = target;
        settings_.mUpVector = repulsion;
        settings_.mRotation = rotation_;

        return true;
    }

    return false;
}

void Editor::UpdateEditAction()
{
    core::vector3df rotation(0,180,0);

    if ( IsSelectionMode() )
    {
        if ( mSelectedNode )
            rotation = mSelectedNode->getRotation();

        gui::ICursorControl* cursor = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getCursorControl();
        assert(cursor);
        const DeviceKeyboard * keyboard =  APP.GetInputDeviceManager()->GetKeyboard();
        assert(keyboard);

        if ( keyboard->IsButtonDown(KEY_KEY_X) && mRotationMode == ERM_NONE )
        {
            mRotationMode = ERM_ONLY_X;
            mOldCursorPos = cursor->getPosition();
            cursor->setVisible(false);
        }
        else if ( !keyboard->IsButtonDown(KEY_KEY_X) && mRotationMode == ERM_ONLY_X )
        {
            mRotationMode = ERM_NONE;
            cursor->setPosition(mOldCursorPos.X, mOldCursorPos.Y);
            cursor->setVisible(true);
        }
        if ( keyboard->IsButtonDown(KEY_KEY_Y) && mRotationMode == ERM_NONE )
        {
            mRotationMode = ERM_ONLY_Y;
            mOldCursorPos = cursor->getPosition();
            cursor->setVisible(false);
        }
        else if ( !keyboard->IsButtonDown(KEY_KEY_Y) && mRotationMode == ERM_ONLY_Y )
        {
            mRotationMode = ERM_NONE;
            cursor->setPosition(mOldCursorPos.X, mOldCursorPos.Y);
            cursor->setVisible(true);
        }
        if ( keyboard->IsButtonDown(KEY_KEY_Z) && mRotationMode == ERM_NONE )
        {
            mRotationMode = ERM_ONLY_Z;
            mOldCursorPos = cursor->getPosition();
            cursor->setVisible(false);
        }
        else if ( !keyboard->IsButtonDown(KEY_KEY_Z) && mRotationMode == ERM_ONLY_Z )
        {
            mRotationMode = ERM_NONE;
            cursor->setPosition(mOldCursorPos.X, mOldCursorPos.Y);
            cursor->setVisible(true);
        }

        if ( mRotationMode != ERM_NONE )
        {
            const DeviceMouse * mouse = APP.GetInputDeviceManager()->GetMouse();
            assert(mouse);

            // rotation
            float rotationSpeed = 150.f;
            rotationSpeed *= mTimer->GetLastTickInSeconds();
            float mouseX = -mouse->GetAxisValue(0) * rotationSpeed;
            //float mouseY = mouse->GetAxisValue(1) * rotationSpeed;

            if ( mRotationMode == ERM_ONLY_X )
            {
                rotation.X += mouseX;
            }
            else if ( mRotationMode == ERM_ONLY_Y )
            {
                rotation.Y += mouseX;
            }
            else if ( mRotationMode == ERM_ONLY_Z )
            {
                rotation.Z += mouseX;
            }
        }
    }

    if ( mActionButtonPressed || IsSelectionMode() )
    {
        mActionButtonPressed = false;

        switch ( mMode )
        {
            case EM_NONE:
                CheckForSelection();
                break;
            case EM_SET_PLAYER_START:
            case EM_SELECTED_PLAYER_START:
            {
                if ( mEditIndex < 0 )
                    break;
                if ( !mIsCursorPosCurrent )
                    break;

                core::vector3df target;
                core::vector3df repulsion;
                if ( APP.GetPhysics()->CalcSphereAboveTrack(mCursorPos3D, target, repulsion, APP.GetConfig()->GetHoverRadius()) )
                {
                    TrackStartSettings trackStart;
                    trackStart.mIsValid = true;
                    trackStart.mCenter = target;
                    trackStart.mRotation = rotation;

                    APP.GetLevelManager()->SetTrackStart(trackStart, mEditIndex);
                }
            }
            break;
            case EM_SET_FINISH_LINE:
            case EM_SELECTED_FINISH_LINE:
            {
                TrackMarkerSettings & marker = APP.GetLevelManager()->GetFinishLine().mSettings;
                if ( CalcTrackMarkerSettingsAtCursor(marker, rotation) )
                {
                    APP.GetLevelManager()->SetFinishLine(marker);
                }
            }
            break;
            case EM_SET_TP_SOURCE:
            case EM_SELECTED_TP_SOURCE:
            {
                TrackMarkerSettings & marker = APP.GetLevelManager()->GetTpSource().mSettings;
                if ( CalcTrackMarkerSettingsAtCursor(marker, rotation) )
                {
                    APP.GetLevelManager()->SetTeleportSource(marker);
                }
            }
            break;
            case EM_SET_TP_TARGET:
            case EM_SELECTED_TP_TARGET:
            {
                TrackMarkerSettings marker = APP.GetLevelManager()->GetTpTarget().mSettings;;
                if ( CalcTrackMarkerSettingsAtCursor(marker, rotation) )
                {
                    APP.GetLevelManager()->SetTeleportTarget(marker);
                }
            }
            break;
            case EM_SET_TRACKMARKER:
            case EM_SELECTED_TRACKMARKER:
            {
                TrackMarkerSettings marker = APP.GetLevelManager()->GetTrackMarker(mEditIndex).mSettings;
                if ( CalcTrackMarkerSettingsAtCursor(marker, rotation) )
                {
                    APP.GetLevelManager()->SetTrackMarker(marker, mEditIndex);
                    APP.GetGui()->GetGuiEditor()->UpdateTrackMarker();
                }
            }
            break;
            case EM_INSERT_TRACKMARKER:
            {
                TrackMarkerSettings marker;
                if ( CalcTrackMarkerSettingsAtCursor(marker, rotation) )
                {
                    marker.EnableWalls();
                    APP.GetLevelManager()->InsertTrackMarker(marker, mEditIndex);
                    APP.GetGui()->GetGuiEditor()->UpdateTrackMarker();
                }
            }
            break;
            case EM_APPEND_TRACKMARKER:
            {
                TrackMarkerSettings marker;
                if ( CalcTrackMarkerSettingsAtCursor(marker, rotation) )
                {
                    marker.EnableWalls();
                    APP.GetLevelManager()->AppendTrackMarker(marker, mEditIndex);
                    ++mEditIndex;
                    APP.GetGui()->GetGuiEditor()->UpdateTrackMarker();
                }
            }
            break;
            case EM_SELECTED_MODEL:
                break;
            case EM_SET_MODEL:
            {
                if ( !mIsCursorPosCurrent )
                    break;

                LevelModelSettings modelSettings = APP.GetLevelManager()->GetModel(mEditIndex).mSettings;
                core::vector3df target;
                core::vector3df repulsion;
                float radius = 0.f;
                if ( APP.GetPhysics()->CalcSphereAboveTrack(mCursorPos3D, target, repulsion, radius) )
                {
                    modelSettings.mCenter = target;
//                    modelSettings.mRotation = rotation;   // should stay the same here
                }
                APP.GetLevelManager()->SetModel(modelSettings, mEditIndex);
                APP.GetGui()->GetGuiEditor()->UpdateObjects();
            }
            break;
        }
    }
}

void Editor::SetSelectedNode(scene::ISceneNode* node_)
{
    if ( mSelectedNode )
    {
        mSelectedNode->setScale( core::vector3df(1,1,1) );
    }
    mSelectedNode = node_;
    if ( mSelectedNode )
    {
        mSelectedNode->setScale( core::vector3df(2,2,2) );
    }
}

void Editor::CheckForSelection()
{
    scene::ICameraSceneNode* camera = APP.GetIrrlichtManager()->GetEditorCamera();
    assert(camera);
    scene::ISceneCollisionManager * collManager = APP.GetIrrlichtManager()->GetSceneManager()->getSceneCollisionManager();
    assert(collManager);

    core::position2d<s32> cursorpos = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getCursorControl()->getPosition();
    core::line3d<f32> line = collManager->getRayFromScreenCoordinates( cursorpos, camera );

    bool wasSelected = IsSelectionMode();

    int index = 0;
    float dist = FLT_MAX;
    if ( APP.GetLevelManager()->CheckTrackStartEditCollision(line, index, dist) )
    {
        SetSelectedNode( APP.GetLevelManager()->GetTrackStart(index).mEditNode );
        SetMode(EM_SELECTED_PLAYER_START, index);
    }
    if ( APP.GetLevelManager()->CheckFinishLineEditCollision(line, index, dist) )
    {
        SetSelectedNode( APP.GetLevelManager()->GetFinishLine().mEditNodeCenter );
        SetMode(EM_SELECTED_FINISH_LINE, index);
    }
    if ( APP.GetLevelManager()->CheckTpSourceEditCollision(line, dist) )
    {
        SetSelectedNode( APP.GetLevelManager()->GetTpSource().mEditNodeCenter );
        SetMode(EM_SELECTED_TP_SOURCE, index);
    }
    if ( APP.GetLevelManager()->CheckTpTargetEditCollision(line, dist) )
    {
        SetSelectedNode( APP.GetLevelManager()->GetTpTarget().mEditNodeCenter );
        SetMode(EM_SELECTED_TP_TARGET, index);
    }
    if ( APP.GetLevelManager()->CheckTrackMarkerEditCollision(line, index, dist) )
    {
        SetSelectedNode( APP.GetLevelManager()->GetTrackMarker(index).mEditNodeCenter );
        SetMode(EM_SELECTED_TRACKMARKER, index);
    }
    if ( APP.GetLevelManager()->CheckModelEditCollision(line, index, dist) )
    {
        SetSelectedNode( APP.GetLevelManager()->GetModel(index).mNodeModel );
        SetMode(EM_SELECTED_MODEL, index);
    }

    if ( IsSelectionMode() && !wasSelected )
    {
        scene::ISceneCollisionManager * collManager = APP.GetIrrlichtManager()->GetSceneManager()->getSceneCollisionManager();
        assert(collManager);
        gui::ICursorControl* cursor = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getCursorControl();
        assert(cursor);

        core::vector3df intersection;
        core::line3d<f32> line;
        line.start = mSelectedNode->getPosition();
        line.end = line.start;
        line.start.Y += 100.f;
        line.end.Y -= 500.f;
        if ( CheckTrackCollision(line, intersection ) )
        {
            core::position2d<s32> screenPos = collManager->getScreenCoordinatesFrom3DPosition( intersection );
            cursor->setPosition(screenPos);
        }
    }
}

bool Editor::IsSelectionMode()
{
    if (    mMode == EM_SELECTED_PLAYER_START
        ||  mMode == EM_SELECTED_FINISH_LINE
        ||  mMode == EM_SELECTED_TP_SOURCE
        ||  mMode == EM_SELECTED_TP_TARGET
        ||  mMode == EM_SELECTED_TRACKMARKER
        ||  mMode == EM_SELECTED_MODEL
        )
        return true;

    return false;
}

bool Editor::CheckTrackCollision(const core::line3d<f32> &line_, core::vector3df &intersection_)
{
    bool coll = false;
    float nearestDist = FLT_MAX;
    core::triangle3df tri;
    core::triangle3df nearestTriangle;
#if IRR_SVN <= 3231
	const scene::ISceneNode* outNode=0;
#else
	scene::ISceneNode* outNode=0;
#endif

    // find nearest colliding geometry to the line start
    for ( size_t i=0; i < APP.GetPhysics()->GetNumTriangleSelectors(); ++i )
    {
        if (APP.GetIrrlichtManager()->GetSceneManager()->getSceneCollisionManager()->getCollisionPoint(
            line_, APP.GetPhysics()->GetTriangleSelector(i), intersection_, tri, outNode))
        {
            float dist = (intersection_-line_.start).getLength();
            if ( dist < nearestDist )
            {
                nearestDist = dist;
                nearestTriangle = tri;
                coll = true;
            }
        }
    }
    return coll;
}

void Editor::UpdatePicking()
{
    scene::ICameraSceneNode* camera = APP.GetIrrlichtManager()->GetEditorCamera();
    assert(camera);
    video::IVideoDriver * driver = APP.GetIrrlichtManager()->GetVideoDriver();
    assert(driver);
    scene::ISceneCollisionManager * collManager = APP.GetIrrlichtManager()->GetSceneManager()->getSceneCollisionManager();
    assert(collManager);

    mIsCursorPosCurrent = false;

    gui::ICursorControl* cursor = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getCursorControl();
    assert(cursor);
    core::position2d<s32> cursorpos;
    if ( cursor->isVisible() )
        cursorpos = cursor->getPosition();
    else
        cursorpos = mOldCursorPos;
    core::line3d<f32> line = collManager->getRayFromScreenCoordinates( cursorpos, camera );
    core::vector3df intersection;

    if ( CheckTrackCollision(line, intersection ) )
    {
        mCursorPos3D = intersection;
        mIsCursorPosCurrent = true;
    }

    // display the 3D-cursor
    if ( mIsCursorPosCurrent )
    {
        APP.GetIrrlichtManager()->SetDriverDrawMode();

        // intersection (as cross)
        float size = 150.f;
        float height = 10.f;
        driver->draw3DLine (core::vector3df(mCursorPos3D.X-size, mCursorPos3D.Y + height, mCursorPos3D.Z),
                            core::vector3df(mCursorPos3D.X+size, mCursorPos3D.Y + height, mCursorPos3D.Z), video::SColor(255, 255, 255, 255));
        driver->draw3DLine (core::vector3df(mCursorPos3D.X, mCursorPos3D.Y + height, mCursorPos3D.Z-size),
                            core::vector3df(mCursorPos3D.X, mCursorPos3D.Y + height, mCursorPos3D.Z+size), video::SColor(255, 255, 255, 255));
    }
}

void Editor::MoveCamToTarget(const core::vector3df &target_)
{
    scene::ICameraSceneNode* camera = APP.GetIrrlichtManager()->GetEditorCamera();
    assert(camera);

    core::vector3df lookAt(camera->getTarget() - camera->getPosition() );
    lookAt.setLength(500.f);
    camera->setTarget(target_);
    camera->setPosition(target_ - lookAt);
}

bool Editor::OnEvent(const SEvent &event)
{
    if ( APP.GetMode() != MODE_EDITOR )
        return false;

    gui::ICursorControl* cursor = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getCursorControl();
    assert(cursor);

    switch ( event.EventType )
    {
        case EET_MOUSE_INPUT_EVENT:
        {
            switch ( event.MouseInput.Event )
            {
                // selection
                case EMIE_LMOUSE_PRESSED_DOWN:
                    if ( !APP.GetGui()->IsHovered() )
                    {
                        mActionButtonPressed = true;
                    }
                break;
                case EMIE_LMOUSE_LEFT_UP:
                    if ( mRotationMode != ERM_NONE )
                    {
                        mRotationMode = ERM_NONE;
                        if ( !cursor->isVisible() )
                        {
                            cursor->setPosition(mOldCursorPos.X, mOldCursorPos.Y);
                            cursor->setVisible(true);
                        }
                    }

                    if ( IsSelectionMode() )
                    {
                        SetSelectedNode(NULL);
                        SetMode(EM_NONE, mEditIndex);
                    }
                break;

                // rotation mode
                case EMIE_RMOUSE_PRESSED_DOWN:
                {
                    if ( cursor->isVisible() && mRotationMode == ERM_NONE )
                    {
                        mOldCursorPos = cursor->getPosition();
                        cursor->setVisible(false);
                    }
                }
                break;
                case EMIE_RMOUSE_LEFT_UP:
                {
                    if ( !cursor->isVisible() && mRotationMode == ERM_NONE )
                    {
                        cursor->setPosition(mOldCursorPos.X, mOldCursorPos.Y);
                        cursor->setVisible(true);
                    }
                }
                break;
                case EMIE_MOUSE_MOVED:
                {
                    //event.MouseInput.X
                    //event.MouseInput.Y
                    //event.Wheel
                }
                break;
                default:
                break;
            }

        }
        break;

        default:
        break;
    }

    return false;
}

#endif // HC1_ENABLE_EDITOR
