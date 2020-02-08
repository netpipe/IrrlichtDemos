// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_editor.h"

#ifdef HC1_ENABLE_EDITOR

#include "gui_nodemanager.h"
#include "gui_modellist.h"
#include "gui_ai.h"

#include "../gui_ids.h"
#include "../main.h"
#include "../level.h"
#include "../editor.h"
#include "../config.h"
#include "../gui.h"
#include <wchar.h>
#include <assert.h>

using namespace irr;
using namespace gui;

GuiEditor::GuiEditor(irr::gui::IGUIEnvironment* env_)
    : mEnvironment(env_)
    , mWndEditor(NULL)
    , mWndPlayerStart(NULL)
    , mWndTrackMarker(NULL)
    , mWndTeleportSource(NULL)
    , mWndTeleportTarget(NULL)
    , mWndFinishLine(NULL)
    , mWndSounds(NULL)
    , mWndObjects(NULL)
    , mGuiNodeManager(NULL)
    , mGuiModelList(NULL)
{
    mChangeMarkerSize = 10.f;
    mChangeModelPos = 5.f;
    mChangeModelRot = 10.f;
    assert(mEnvironment);

    mGuiNodeManager = new GuiNodeManager(env_);
    mGuiModelList = new GuiModelList(env_, this);
}

GuiEditor::~GuiEditor()
{
    delete mGuiModelList;
    delete mGuiNodeManager;
}

void GuiEditor::CreateEditorWnd()
{
	IGUIElement* e = mEnvironment->getRootGUIElement()->getElementFromId(GUI_WND_EDITOR, true);
	if (e)
	{
        e->remove();
        mWndEditor = NULL;
	}

	// create the editor window
	mWndEditor = mEnvironment->addWindow(core::rect<s32>(520,70,640,400),
		false, L"Editor", 0, GUI_WND_EDITOR);

    int top = 30;
    int left = 5;
    int width = 110;
    int height = 15;

    IGUIButton * btn;

    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_NODES, L"Models/Nodes");
    top += height+20;

    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_PLAYER_START, L"player start");
    top += height+5;

    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_TRACK_MARKER, L"track markers");
    top += height+5;

    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_TELEPORT_SOURCE, L"TP source");
    top += height+5;

    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_TELEPORT_TARGET, L"TP target");
    top += height+5;

    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_FINISH_LINE, L"finish line");
    top += height+5;

    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_SOUNDS, L"sound");
    btn->setEnabled(false);
    top += height+5;

    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_AI, L"AI");
    top += height+5;

    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_OBJECTS, L"Objects");
    top += height+5;

    top += 10;
    btn = mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndEditor, BTN_SAVE_LEVEL, L"Save level");
    top += height+5;
}

void GuiEditor::RemoveEditorWnd()
{
    IGUIElement* root = mEnvironment->getRootGUIElement();
	IGUIElement* e = root->getElementFromId(GUI_WND_EDITOR, true);
	if (e)
	{
        e->remove();
        mWndEditor = NULL;
	}
}

bool GuiEditor::RemoveAllSubWindows(irr::gui::IGUIWindow* checkExistanceWnd_)
{
    bool existed = false;

    APP.GetGui()->GetGuiAi()->Hide();

    IGUIElement* root = mEnvironment->getRootGUIElement();
	IGUIElement* e  = root->getElementFromId(WND_PLAYER_START, true);
	if ( e )
	{
	    if ( e == checkExistanceWnd_ )
            existed = true;
        e->remove();
        mWndPlayerStart = NULL;
	}

    e = root->getElementFromId(WND_TRACK_MARKER, true);
	if ( e )
	{
	    if ( e == checkExistanceWnd_ )
            existed = true;
        e->remove();
        mWndTrackMarker = NULL;
	}

    e = root->getElementFromId(WND_TELEPORT_SOURCE, true);
	if ( e )
	{
	    if ( e == checkExistanceWnd_ )
            existed = true;
        e->remove();
        mWndTeleportSource = NULL;
	}

    e = root->getElementFromId(WND_TELEPORT_TARGET, true);
	if ( e )
	{
	    if ( e == checkExistanceWnd_ )
            existed = true;
        e->remove();
        mWndTeleportTarget = NULL;
	}

    e = root->getElementFromId(WND_FINISH_LINE, true);
	if ( e )
	{
	    if ( e == checkExistanceWnd_ )
            existed = true;
        e->remove();
        mWndFinishLine = NULL;
	}

    e = root->getElementFromId(WND_SOUNDS, true);
	if ( e )
	{
	    if ( e == checkExistanceWnd_ )
            existed = true;
        e->remove();
        mWndSounds = NULL;
	}

	e = root->getElementFromId(WND_OBJECTS, true);
	if ( e )
	{
	    if ( e == checkExistanceWnd_ )
            existed = true;
        e->remove();
        mWndObjects = NULL;
	}

    e = root->getElementFromId(WND_NODE_MANAGER, true);
	if ( e )
	{
	    if ( e == checkExistanceWnd_ )
            existed = true;

        mGuiNodeManager->RemoveNodeWindow();
	}

	return existed;
}

void GuiEditor::OnPlayerStart()
{
    if ( RemoveAllSubWindows(mWndPlayerStart) )
        return;

    mWndPlayerStart = mEnvironment->addWindow(core::rect<s32>(0,70,200,200),
		false, L"player starts", 0, WND_PLAYER_START);
    assert(mWndPlayerStart);

    int top = 25;
    int left = 10;

    core::rect< s32 > listRect(left, top, left + 100, top+100 );
    IGUIListBox * listBox = mEnvironment->addListBox(listRect, mWndPlayerStart, LIST_LEVEL_STARTS );
    for ( int i=0; i < MAX_SPAWNS; ++i )
    {
        listBox->addItem(L"");
    }

    left += 110;
    int width = 70;
    int height = 15;
    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndPlayerStart, BTN_GO_LEVEL_START, L"go");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndPlayerStart, CHECKBOX_SET_LEVEL_START, L"set");
    top += height+5;

    UpdatePlayerStart();
}

void GuiEditor::OnTrackMarker()
{
    if ( RemoveAllSubWindows(mWndTrackMarker) )
        return;

    if ( APP.GetLevelManager()->GetCurrentLevelIndex() < 0 )
        return;

    mWndTrackMarker = mEnvironment->addWindow(core::rect<s32>(0,70,200,450),
		false, L"track markers", 0, WND_TRACK_MARKER);
    assert(mWndTrackMarker);

    int top = 25;
    int left = 10;
    core::rect< s32 > listRect(left, top, left + 100, top+150 );
    mEnvironment->addListBox(listRect, mWndTrackMarker, LIST_TRACKMARKERS);

    left += 110;
    int width = 70;
    int height = 15;
    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, BTN_GO_TRACKMARKER, L"go");
    top += height+5;

    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, BTN_REMOVE_TRACKMARKER, L"remove");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, CHECKBOX_INSERT_TRACKMARKER, L"insert");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, CHECKBOX_SET_TRACKMARKER, L"set");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, CHECKBOX_APPEND_TRACKMARKER, L"append");
    top += height+5;

    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, BTN_AUTO_ALIGN, L"align");
    top += height+5;

    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, BTN_ROTATE_180, L"180 Deg.");
    top += height+5;

    left -= 110;
    top = 180;
    width = 120;
    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, CHECKBOX_WALL_LEFT, L"wall left");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, CHECKBOX_WALL_RIGHT, L"wall right");
    top += height+5;

//    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, CHECKBOX_WALL_TOP, L"wall top");
//    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, CHECKBOX_WALL_BOTTOM, L"wall bottom");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, CHECKBOX_RELOCATE, L"relocate");
    top += height+5;

    int btnWidth = 20;
    mEnvironment->addStaticText (L"left", core::rect<s32>(left, top, left+width, top+height), true, false, mWndTrackMarker, STATIC_LEFT);
    mEnvironment->addButton(core::rect<s32>(left+width+10,top,left+width+10+btnWidth,top+height), mWndTrackMarker, BTN_LEFT_ADD, L"+");
    mEnvironment->addButton(core::rect<s32>(left+width+20+btnWidth,top,left+width+20+2*btnWidth,top+height), mWndTrackMarker, BTN_LEFT_SUB, L"-");
    top += height+5;

    mEnvironment->addStaticText (L"right", core::rect<s32>(left, top, left+width, top+height), true, false, mWndTrackMarker, STATIC_RIGHT);
    mEnvironment->addButton(core::rect<s32>(left+width+10,top,left+width+10+btnWidth,top+height), mWndTrackMarker, BTN_RIGHT_ADD, L"+");
    mEnvironment->addButton(core::rect<s32>(left+width+20+btnWidth,top,left+width+20+2*btnWidth,top+height), mWndTrackMarker, BTN_RIGHT_SUB, L"-");
    top += height+5;

    mEnvironment->addStaticText (L"top", core::rect<s32>(left, top, left+width, top+height), true, false, mWndTrackMarker, STATIC_TOP);
    mEnvironment->addButton(core::rect<s32>(left+width+10,top,left+width+10+btnWidth,top+height), mWndTrackMarker, BTN_TOP_ADD, L"+");
    mEnvironment->addButton(core::rect<s32>(left+width+20+btnWidth,top,left+width+20+2*btnWidth,top+height), mWndTrackMarker, BTN_TOP_SUB, L"-");
    top += height+5;

    mEnvironment->addStaticText (L"bottom", core::rect<s32>(left, top, left+width, top+height), true, false, mWndTrackMarker, STATIC_BOTTOM);
    mEnvironment->addButton(core::rect<s32>(left+width+10,top,left+width+10+btnWidth,top+height), mWndTrackMarker, BTN_BOTTOM_ADD, L"+");
    mEnvironment->addButton(core::rect<s32>(left+width+20+btnWidth,top,left+width+20+2*btnWidth,top+height), mWndTrackMarker, BTN_BOTTOM_SUB, L"-");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTrackMarker, CHECKBOX_DEFAULT_SIZE, L"default size");
    top += height+5;

    UpdateTrackMarker();
}

void GuiEditor::OnTeleportSource()
{
    if ( RemoveAllSubWindows(mWndTeleportSource) )
        return;

    if ( APP.GetLevelManager()->GetCurrentLevelIndex() < 0 )
        return;

    mWndTeleportSource = mEnvironment->addWindow(core::rect<s32>(0,70,200,200),
		false, L"TP source", 0, WND_TELEPORT_SOURCE);
    assert(mWndTeleportSource);

    int top = 25;
    int left = 10;

    mEnvironment->addStaticText (L"", core::rect<s32>(left, top, left+100, top+20), true, false, mWndTeleportSource, STATIC_TP_START);

    left += 110;
    int width = 70;
    int height = 15;
    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndTeleportSource, BTN_GO_TP_START, L"go");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTeleportSource, CHECKBOX_SET_TP_START, L"set");
    top += height+5;

    UpdateTeleportSource();
}

void GuiEditor::OnTeleportTarget()
{
    if ( RemoveAllSubWindows(mWndTeleportTarget) )
        return;

    if ( APP.GetLevelManager()->GetCurrentLevelIndex() < 0 )
        return;

    mWndTeleportTarget = mEnvironment->addWindow(core::rect<s32>(0,70,200,200),
		false, L"tp target", 0, WND_TELEPORT_TARGET);
    assert(mWndTeleportTarget);

    int top = 25;
    int left = 10;

    mEnvironment->addStaticText (L"", core::rect<s32>(left, top, left+100, top+20), true, false, mWndTeleportTarget, STATIC_TP_TARGET);

    left += 110;
    int width = 70;
    int height = 15;
    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndTeleportTarget, BTN_GO_TP_TARGET, L"go");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndTeleportTarget, CHECKBOX_SET_TP_TARGET, L"set");
    top += height+5;

    UpdateTeleportTarget();
}

void GuiEditor::OnFinishLine()
{
    if ( RemoveAllSubWindows(mWndFinishLine) )
        return;

    if ( APP.GetLevelManager()->GetCurrentLevelIndex() < 0 )
        return;

    mWndFinishLine = mEnvironment->addWindow(core::rect<s32>(0,70,200,200),
		false, L"finish line", 0, WND_FINISH_LINE);
    assert(mWndFinishLine);

    int top = 25;
    int left = 10;

    mEnvironment->addStaticText (L"", core::rect<s32>(left, top, left+100, top+20), true, false, mWndFinishLine, STATIC_FINISH_LINE);

    left += 110;
    int width = 70;
    int height = 15;
    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndFinishLine, BTN_GO_FINISH_LINE, L"go");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndFinishLine, CHECKBOX_SET_FINISH_LINE, L"set");
    top += height+5;

    UpdateFinishLine();
}

void GuiEditor::OnSounds()
{
    if ( RemoveAllSubWindows(mWndSounds) )
        return;

    if ( APP.GetLevelManager()->GetCurrentLevelIndex() < 0 )
        return;

    mWndSounds = mEnvironment->addWindow(core::rect<s32>(0,70,200,200),
		false, L"sounds", 0, WND_SOUNDS);
    assert(mWndSounds);

    // TODO

    UpdateSounds();
}

void GuiEditor::OnAi()
{
    if ( RemoveAllSubWindows(APP.GetGui()->GetGuiAi()->GetWindow()) )
        return;

    if ( APP.GetLevelManager()->GetCurrentLevelIndex() < 0 )
        return;

    APP.GetGui()->GetGuiAi()->Show();
}

void GuiEditor::OnObjects()
{
    if ( RemoveAllSubWindows(mWndObjects) )
        return;

    if ( APP.GetLevelManager()->GetCurrentLevelIndex() < 0 )
        return;

    mWndObjects = mEnvironment->addWindow(core::rect<s32>(0,40,200,470),
		false, L"objects", 0, WND_OBJECTS);
    assert(mWndObjects);

    int top = 25;
    int left = 10;
    core::rect< s32 > listRect(left, top, left + 180, top+150 );
    mEnvironment->addListBox(listRect, mWndObjects, LIST_LEVELMODELS);
    top += 160;

    int width = 70;
    int height = 15;
    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndObjects, BTN_GO_LEVELMODEL, L"go");
    top += height+5;

    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndObjects, BTN_ADD_LEVELMODEL, L"add");
    top += height+5;

    mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), mWndObjects, BTN_REMOVE_LEVELMODEL, L"remove");
    top += height+5;

    mEnvironment->addCheckBox(false, core::rect<s32>(left,top,left+width,top+height), mWndObjects, CHECKBOX_SET_LEVELMODEL, L"set");
    top += height+5;

    mEnvironment->addStaticText (L"pos x", core::rect<s32>(left, top, left+50, top+height), false, false, mWndObjects);
    mEnvironment->addButton(core::rect<s32>(left+55,top,left+70,top+height), mWndObjects, BTN_POS_X_SUB, L"-");
    mEnvironment->addEditBox(L"", core::rect<s32>(left+71,top,left+160,top+height), true, mWndObjects, EDIT_POS_X);
    mEnvironment->addButton(core::rect<s32>(left+161,top,left+176,top+height), mWndObjects, BTN_POS_X_ADD, L"+");
    top += height+5;

    mEnvironment->addStaticText (L"pos y", core::rect<s32>(left, top, left+50, top+height), false, false, mWndObjects);
    mEnvironment->addButton(core::rect<s32>(left+55,top,left+70,top+height), mWndObjects, BTN_POS_Y_SUB, L"-");
    mEnvironment->addEditBox(L"", core::rect<s32>(left+71,top,left+160,top+height), true, mWndObjects, EDIT_POS_Y);
    mEnvironment->addButton(core::rect<s32>(left+161,top,left+176,top+height), mWndObjects, BTN_POS_Y_ADD, L"+");
    top += height+5;

    mEnvironment->addStaticText (L"pos z", core::rect<s32>(left, top, left+50, top+height), false, false, mWndObjects);
    mEnvironment->addButton(core::rect<s32>(left+55,top,left+70,top+height), mWndObjects, BTN_POS_Z_SUB, L"-");
    mEnvironment->addEditBox(L"", core::rect<s32>(left+71,top,left+160,top+height), true, mWndObjects, EDIT_POS_Z);
    mEnvironment->addButton(core::rect<s32>(left+161,top,left+176,top+height), mWndObjects, BTN_POS_Z_ADD, L"+");
    top += height+5;

    mEnvironment->addStaticText (L"rot x", core::rect<s32>(left, top, left+50, top+height), false, false, mWndObjects);
    mEnvironment->addButton(core::rect<s32>(left+55,top,left+70,top+height), mWndObjects, BTN_ROT_X_SUB, L"-");
    mEnvironment->addEditBox(L"", core::rect<s32>(left+71,top,left+160,top+height), true, mWndObjects, EDIT_ROT_X);
    mEnvironment->addButton(core::rect<s32>(left+161,top,left+176,top+height), mWndObjects, BTN_ROT_X_ADD, L"+");
    top += height+5;

    mEnvironment->addStaticText (L"rot y", core::rect<s32>(left, top, left+50, top+height), false, false, mWndObjects);
    mEnvironment->addButton(core::rect<s32>(left+55,top,left+70,top+height), mWndObjects, BTN_ROT_Y_SUB, L"-");
    mEnvironment->addEditBox(L"", core::rect<s32>(left+71,top,left+160,top+height), true, mWndObjects, EDIT_ROT_Y);
    mEnvironment->addButton(core::rect<s32>(left+161,top,left+176,top+height), mWndObjects, BTN_ROT_Y_ADD, L"+");
    top += height+5;

    mEnvironment->addStaticText (L"rot z", core::rect<s32>(left, top, left+50, top+height), false, false, mWndObjects);
    mEnvironment->addButton(core::rect<s32>(left+55,top,left+70,top+height), mWndObjects, BTN_ROT_Z_SUB, L"-");
    mEnvironment->addEditBox(L"", core::rect<s32>(left+71,top,left+160,top+height), true, mWndObjects, EDIT_ROT_Z);
    mEnvironment->addButton(core::rect<s32>(left+161,top,left+176,top+height), mWndObjects, BTN_ROT_Z_ADD, L"+");
    top += height+5;

    mEnvironment->addStaticText (L"ambient col (r,g,b)", core::rect<s32>(left, top, left+180, top+height), false, false, mWndObjects);
    top += height+5;
    IGUISpinBox* sb = NULL;
    sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left-5,top,left+59,top+height), true, mWndObjects, SPIN_MODEL_AMB_R);
    sb->setRange(1,255);
    sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+60,top,left+119,top+height), true, mWndObjects, SPIN_MODEL_AMB_G);
    sb->setRange(1,255);
    sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+120,top,left+184,top+height), true, mWndObjects, SPIN_MODEL_AMB_B);
    sb->setRange(1,255);

    UpdateObjects();
}

void GuiEditor::OnNodes()
{
    if ( RemoveAllSubWindows(mGuiNodeManager->GetNodeWindow()) )
        return;

    mGuiNodeManager->CreateNodeWindow();
}

bool GuiEditor::OnEventPlayerStart(const SEvent &event)
{
    if ( event.EventType != EET_GUI_EVENT)
        return false;

    s32 id = event.GUIEvent.Caller->getID();
    switch(event.GUIEvent.EventType)
    {
        case EGET_BUTTON_CLICKED:
        {
            switch(id)
            {
                case BTN_GO_LEVEL_START:
                {
                    IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndPlayerStart->getElementFromId(LIST_LEVEL_STARTS, false));
                    if ( !listBox )
                        break;

                    core::vector3df pos;
                    int selected = listBox->getSelected();
                    if ( selected >= 0 && APP.GetLevelManager()->GetTrackStart(selected).GetCenter(pos) )
                    {
                        APP.GetEditor()->MoveCamToTarget(pos);
                    }
                }
                break;
                default:
                break;
            }
        }
        break;
        case EGET_CHECKBOX_CHANGED:
        {
            switch ( id )
            {
                case CHECKBOX_SET_LEVEL_START:
                {
                    IGUICheckBox *cb = static_cast<IGUICheckBox*>(mWndPlayerStart->getElementFromId(CHECKBOX_SET_LEVEL_START, false));
                    if ( !cb )
                        break;
                    IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndPlayerStart->getElementFromId(LIST_LEVEL_STARTS, false));
                    int selected = -1;
                    if ( listBox )
                        selected = listBox->getSelected();
                    if ( cb->isChecked() && selected >= 0)
                    {
                        APP.GetEditor()->SetMode(EM_SET_PLAYER_START, selected);
                    }
                    else
                    {
                        APP.GetEditor()->SetMode(EM_NONE, selected);
                    }
                }
                break;
                default:
                break;
            }
        }
        break;
        case EGET_LISTBOX_CHANGED:
        {
            switch ( id  )
            {
                case LIST_LEVEL_STARTS:
                {
                    IGUIListBox * listBox = static_cast<IGUIListBox*>(event.GUIEvent.Caller);
                    if ( !listBox )
                        break;
                    IGUICheckBox *cb = static_cast<IGUICheckBox*>(mWndPlayerStart->getElementFromId(CHECKBOX_SET_LEVEL_START, false));
                    if ( !cb )
                        break;
                    if ( cb->isChecked() )
                    {
                        APP.GetEditor()->SetMode(EM_SET_PLAYER_START, listBox->getSelected());
                    }
                }
            }
        }

        default:
        break;
    }

    return false;
}

bool GuiEditor::OnEventTrackMarker(const SEvent &event)
{
    if ( event.EventType != EET_GUI_EVENT)
        return false;

    s32 id = event.GUIEvent.Caller->getID();
    switch(event.GUIEvent.EventType)
    {
        case EGET_BUTTON_CLICKED:
        {
            IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndTrackMarker->getElementFromId(LIST_TRACKMARKERS, false));
            if ( !listBox )
                break;
            int selected = listBox->getSelected();
            switch(id)
            {
                case BTN_LEFT_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeTrackMarkerSizes(false, -mChangeMarkerSize, 0.f, 0.f, 0.f, selected);
                        UpdateTrackMarker();
                    }
                break;
                case BTN_LEFT_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeTrackMarkerSizes(false, mChangeMarkerSize, 0.f, 0.f, 0.f, selected);
                        UpdateTrackMarker();
                    }
                break;
                case BTN_RIGHT_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeTrackMarkerSizes(false, 0.f, mChangeMarkerSize, 0.f, 0.f, selected);
                        UpdateTrackMarker();
                    }
                break;
                case BTN_RIGHT_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeTrackMarkerSizes(false, 0.f, -mChangeMarkerSize, 0.f, 0.f, selected);
                        UpdateTrackMarker();
                    }
                break;
                case BTN_TOP_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeTrackMarkerSizes(false, 0.f, 0.f, mChangeMarkerSize, 0.f, selected);
                        UpdateTrackMarker();
                    }
                break;
                case BTN_TOP_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeTrackMarkerSizes(false, 0.f, 0.f, -mChangeMarkerSize, 0.f, selected);
                        UpdateTrackMarker();
                    }
                break;
                case BTN_BOTTOM_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeTrackMarkerSizes(false, 0.f, 0.f, 0.f, -mChangeMarkerSize, selected);
                        UpdateTrackMarker();
                    }
                break;
                case BTN_BOTTOM_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeTrackMarkerSizes(false, 0.f, 0.f, 0.f, mChangeMarkerSize, selected);
                        UpdateTrackMarker();
                    }
                break;
                case BTN_GO_TRACKMARKER:
                {
                    core::vector3df pos;
                    if ( selected >= 0 && APP.GetLevelManager()->GetTrackMarker(selected).GetCenter(pos) )
                    {
                        APP.GetEditor()->MoveCamToTarget(pos);
                    }
                }
                break;
                case BTN_REMOVE_TRACKMARKER:
                {
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->RemoveTrackMarker(selected);
                        APP.GetEditor()->SetMode(EM_NONE, selected-1);
                        UpdateTrackMarker();
                    }
                }
                case BTN_AUTO_ALIGN:
                {
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->AutoAlignTrackMarker(selected);
                    }
                }
                break;
                case BTN_ROTATE_180:
                {
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->RotateTrackMarker(180.f, selected);
                    }
                }
                break;
                default:
                break;
            }
        }
        break;
        case EGET_CHECKBOX_CHANGED:
        {
            IGUICheckBox *cb = static_cast<IGUICheckBox*>(event.GUIEvent.Caller);
            if ( !cb )
                break;

            IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndTrackMarker->getElementFromId(LIST_TRACKMARKERS, false));
            if ( !listBox )
                break;
            int selected = listBox->getSelected();

            switch(id)
            {
                case CHECKBOX_INSERT_TRACKMARKER:
                {
                    if ( cb->isChecked() )
                    {
                        IGUICheckBox *cbSet = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_SET_TRACKMARKER, false));
                        IGUICheckBox *cbAppend = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_APPEND_TRACKMARKER, false));
                        if ( !listBox || !cbSet|| !cbAppend)
                            break;
                        cbSet->setChecked(false);
                        cbAppend->setChecked(false);
                        int selected = listBox->getSelected();
                        if ( selected < 0 )
                            selected = 0;
                        APP.GetEditor()->SetMode(EM_INSERT_TRACKMARKER, selected);
                    }
                    else
                    {
                        APP.GetEditor()->SetMode(EM_NONE, selected);
                    }
                }
                break;
                case CHECKBOX_APPEND_TRACKMARKER:
                {
                    if ( cb->isChecked() )
                    {
                        IGUICheckBox *cbSet = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_SET_TRACKMARKER, false));
                        IGUICheckBox *cbInsert = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_INSERT_TRACKMARKER, false));
                        if ( !listBox || !cbSet || !cbInsert)
                            break;
                        cbSet->setChecked(false);
                        cbInsert->setChecked(false);

                        if ( selected < 0 )
                            selected = 0;
                        APP.GetEditor()->SetMode(EM_APPEND_TRACKMARKER, selected);
                    }
                    else
                    {
                        APP.GetEditor()->SetMode(EM_NONE, selected);
                    }
                }
                break;
                case CHECKBOX_SET_TRACKMARKER:
                {
                    if ( cb->isChecked() )
                    {
                        IGUICheckBox *cbInsert = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_INSERT_TRACKMARKER, false));
                        IGUICheckBox *cbAppend = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_APPEND_TRACKMARKER, false));
                        if ( !listBox || !cbInsert || !cbAppend)
                            break;
                        cbInsert->setChecked(false);
                        cbAppend->setChecked(false);

                        if ( selected < 0 )
                            break;
                        APP.GetEditor()->SetMode(EM_SET_TRACKMARKER, selected);
                    }
                    else
                    {
                        APP.GetEditor()->SetMode(EM_NONE, selected);
                    }
                }
                break;
                case CHECKBOX_WALL_LEFT:
                {
                    if ( selected < 0 )
                        break;
                    TrackMarkerSettings settings = APP.GetLevelManager()->GetTrackMarker(selected).mSettings;
                    if ( cb->isChecked() )
                    {
                        settings.mHasLeftWall = true;
                    }
                    else
                    {
                        settings.mHasLeftWall = false;
                    }
                    APP.GetLevelManager()->SetTrackMarker(settings, selected);
                }
                break;
                case CHECKBOX_WALL_RIGHT:
                {
                    if ( selected < 0 )
                        break;
                    TrackMarkerSettings settings = APP.GetLevelManager()->GetTrackMarker(selected).mSettings;
                    if ( cb->isChecked() )
                    {
                        settings.mHasRightWall = true;
                    }
                    else
                    {
                        settings.mHasRightWall = false;
                    }
                    APP.GetLevelManager()->SetTrackMarker(settings, selected);
                }
                break;
                case CHECKBOX_WALL_BOTTOM:
                {
                    if ( selected < 0 )
                        break;
                    TrackMarkerSettings settings = APP.GetLevelManager()->GetTrackMarker(selected).mSettings;
                    if ( cb->isChecked() )
                    {
                        settings.mHasBottomWall = true;
                    }
                    else
                    {
                        settings.mHasBottomWall = false;
                    }
                    APP.GetLevelManager()->SetTrackMarker(settings, selected);
                }
                break;
                case CHECKBOX_RELOCATE:
                {
                    if ( selected < 0 )
                        break;
					// TODO: compiler warning makes sense - code doesn't. Will have to check some day what this was about.
                    TrackMarkerSettings settings = APP.GetLevelManager()->GetTrackMarker(selected).mSettings;
                    if ( cb->isChecked() )
                    {
                        settings.mRelocate = true;
                    }
                    else
                    {
                        settings.mRelocate = false;
                    }
                    // no need to call SetTrackMarker as no node's are changed
                }
                case CHECKBOX_DEFAULT_SIZE:
                {
                    if ( selected < 0 )
                        break;
                    APP.GetLevelManager()->ChangeTrackMarkerSizes(true, 0.f, 0.f, 0.f, 0.f, selected);
                    UpdateTrackMarker();
                }

                default:
                break;
            }
        }
        break;
        case EGET_LISTBOX_CHANGED:
        {
            switch(id)
            {
                case LIST_TRACKMARKERS:
                {
                    IGUIListBox * listBox = static_cast<IGUIListBox*>(event.GUIEvent.Caller);
                    if ( !listBox )
                        break;
                    IGUICheckBox *cbSet = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_SET_TRACKMARKER, false));
                    IGUICheckBox *cbInsert = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_INSERT_TRACKMARKER, false));
                    IGUICheckBox *cbAppend = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_APPEND_TRACKMARKER, false));
                    if ( !cbSet || !cbInsert || !cbAppend)
                        break;
                    if ( cbSet->isChecked() )
                    {
                        APP.GetEditor()->SetMode(EM_SET_TRACKMARKER, listBox->getSelected());
                    }
                    else if ( cbInsert->isChecked() )
                    {
                        APP.GetEditor()->SetMode(EM_INSERT_TRACKMARKER, listBox->getSelected());
                    }
                    else if ( cbAppend->isChecked() )
                    {
                        APP.GetEditor()->SetMode(EM_APPEND_TRACKMARKER, listBox->getSelected());
                    }
                    else
                    {
                        APP.GetEditor()->SetMode(EM_NONE, listBox->getSelected());
                    }
                    UpdateTrackMarker();
                }
                break;
                default:
                break;
            }
        }

        default:
        break;
    }

    return false;
}

bool GuiEditor::OnEventTeleportSource(const SEvent &event)
{
    if ( event.EventType != EET_GUI_EVENT)
        return false;

    s32 id = event.GUIEvent.Caller->getID();
    if (    event.GUIEvent.EventType == EGET_BUTTON_CLICKED
        &&  id == BTN_GO_TP_START )
    {
        core::vector3df pos;
        if ( APP.GetLevelManager()->GetTpSource().GetCenter(pos) )
        {
            APP.GetEditor()->MoveCamToTarget(pos);
        }
    }
    else if ( event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED
            &&  id == CHECKBOX_SET_TP_START )
    {
        IGUICheckBox *cb = static_cast<IGUICheckBox*>(event.GUIEvent.Caller);
        if ( cb && cb->isChecked() )
        {
            APP.GetEditor()->SetMode(EM_SET_TP_SOURCE, 0);
        }
        else
        {
            APP.GetEditor()->SetMode(EM_NONE, -1);
        }
    }

    return false;
}

bool GuiEditor::OnEventTeleportTarget(const SEvent &event)
{
    if ( event.EventType != EET_GUI_EVENT)
        return false;

    s32 id = event.GUIEvent.Caller->getID();
    if (    event.GUIEvent.EventType == EGET_BUTTON_CLICKED
        &&  id == BTN_GO_TP_TARGET )
    {
        core::vector3df pos;
        if ( APP.GetLevelManager()->GetTpTarget().GetCenter(pos) )
        {
            APP.GetEditor()->MoveCamToTarget(pos);
        }
    }
    else if ( event.GUIEvent.EventType == EGET_CHECKBOX_CHANGED
            &&  id == CHECKBOX_SET_TP_TARGET )
    {
        IGUICheckBox *cb = static_cast<IGUICheckBox*>(event.GUIEvent.Caller);
        if ( cb && cb->isChecked() )
        {
            APP.GetEditor()->SetMode(EM_SET_TP_TARGET, 0);
        }
        else
        {
            APP.GetEditor()->SetMode(EM_NONE, -1);
        }
    }

    return false;
}

bool GuiEditor::OnEventFinishLine(const SEvent &event)
{
    if ( event.EventType != EET_GUI_EVENT)
        return false;

    s32 id = event.GUIEvent.Caller->getID();
    switch(event.GUIEvent.EventType)
    {
        case EGET_BUTTON_CLICKED:
        {
            switch(id)
            {
                case BTN_GO_FINISH_LINE:
                {
                    core::vector3df pos;
                    if ( APP.GetLevelManager()->GetFinishLine().GetCenter(pos) )
                    {
                        APP.GetEditor()->MoveCamToTarget(pos);
                    }
                }
                break;

                default:
                break;
            }
        }
        break;

        case EGET_CHECKBOX_CHANGED:
        {
            switch ( id )
            {
                case CHECKBOX_SET_FINISH_LINE:
                {
                    IGUICheckBox *cb = static_cast<IGUICheckBox*>(event.GUIEvent.Caller);
                    if ( !cb )
                        break;
                    if ( cb->isChecked() )
                    {
                        APP.GetEditor()->SetMode(EM_SET_FINISH_LINE, 0);
                    }
                    else
                    {
                        APP.GetEditor()->SetMode(EM_NONE, -1);
                    }
                }
                break;
                default:
                break;
            }
        }

        default:
        break;
    }

    return false;
}

bool GuiEditor::OnEventSounds(const SEvent &event)
{
    if ( event.EventType != EET_GUI_EVENT)
        return false;

    s32 id = event.GUIEvent.Caller->getID();
    switch(event.GUIEvent.EventType)
    {
        case EGET_BUTTON_CLICKED:
        {
            switch(id)
            {
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

bool GuiEditor::OnEventObjects(const SEvent &event)
{
    if ( event.EventType != EET_GUI_EVENT)
        return false;

    s32 id = event.GUIEvent.Caller->getID();
    switch(event.GUIEvent.EventType)
    {
        case EGET_BUTTON_CLICKED:
        {
            IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndObjects->getElementFromId(LIST_LEVELMODELS, false));
            if ( !listBox )
                break;
            int selected = listBox->getSelected();

            switch(id)
            {
                case BTN_GO_LEVELMODEL:
                {
                    if ( selected < 0 )
                        break;
                    core::vector3df pos;
                    if ( APP.GetLevelManager()->GetModel(selected).GetCenter(pos) )
                    {
                        APP.GetEditor()->MoveCamToTarget(pos);
                    }
                }
                break;
                case BTN_ADD_LEVELMODEL:
                {
                    mGuiModelList->Show();
                }
                break;
                case BTN_REMOVE_LEVELMODEL:
                    if ( selected < 0 )
                        break;
                    APP.GetLevelManager()->RemoveModel(selected);
                    UpdateObjects();
                break;

                case BTN_POS_X_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelPosition(mChangeModelPos, 0, 0, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_POS_X_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelPosition(-mChangeModelPos, 0, 0, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_POS_Y_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelPosition(0, mChangeModelPos, 0, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_POS_Y_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelPosition(0, -mChangeModelPos, 0, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_POS_Z_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelPosition(0, 0, mChangeModelPos, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_POS_Z_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelPosition(0, 0, -mChangeModelPos, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_ROT_X_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelRotation(mChangeModelRot, 0, 0, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_ROT_X_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelRotation(-mChangeModelRot, 0, 0, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_ROT_Y_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelRotation(0, mChangeModelRot, 0, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_ROT_Y_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelRotation(0, -mChangeModelRot, 0, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_ROT_Z_ADD:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelRotation(0, 0, mChangeModelRot, selected);
                        UpdateObjects();
                    }
                break;
                case BTN_ROT_Z_SUB:
                    if ( selected >= 0 )
                    {
                        APP.GetLevelManager()->ChangeModelRotation(0, 0, -mChangeModelRot, selected);
                        UpdateObjects();
                    }
                break;

                default:
                break;
            }
        }
        break;
        case EGET_SPINBOX_CHANGED:
        {
            IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndObjects->getElementFromId(LIST_LEVELMODELS, false));
            if ( !listBox )
                break;
            int selected = listBox->getSelected();

            switch(id)
            {
                case SPIN_MODEL_AMB_R:
                case SPIN_MODEL_AMB_G:
                case SPIN_MODEL_AMB_B:
                {
                    IGUISpinBox * spinAmbR = static_cast<IGUISpinBox*>(mWndObjects->getElementFromId(SPIN_MODEL_AMB_R, false));
                    IGUISpinBox * spinAmbG = static_cast<IGUISpinBox*>(mWndObjects->getElementFromId(SPIN_MODEL_AMB_G, false));
                    IGUISpinBox * spinAmbB = static_cast<IGUISpinBox*>(mWndObjects->getElementFromId(SPIN_MODEL_AMB_B, false));
                    if ( selected >= 0 && spinAmbR && spinAmbG && spinAmbB)
                    {
                        APP.GetLevelManager()->SetModelAmbient((int)spinAmbR->getValue(), (int)spinAmbG->getValue(), (int)spinAmbB->getValue(), selected);
                        UpdateObjects();
                    }
                }
                break;
                default:
                break;
            }
            //SPIN_MODEL_AMB_R
        }
        break;
        case EGET_EDITBOX_ENTER:
        {
            IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndObjects->getElementFromId(LIST_LEVELMODELS, false));
            if ( !listBox )
                break;
            int selected = listBox->getSelected();
            if ( selected < 0 )
                break;
            IGUIEditBox * edit = static_cast<IGUIEditBox*>(event.GUIEvent.Caller);
            if ( !edit )
                break;
            switch( id )
            {
                case EDIT_POS_X:
                {
                    LevelModel model = APP.GetLevelManager()->GetModel(selected);
                    model.mSettings.mCenter.X = wcstof(edit->getText(), NULL);
                    APP.GetLevelManager()->SetModel(model.mSettings, selected);
                    UpdateObjects();
                }
                break;
                case EDIT_POS_Y:
                {
                    LevelModel model = APP.GetLevelManager()->GetModel(selected);
                    model.mSettings.mCenter.Y = wcstof(edit->getText(), NULL);
                    APP.GetLevelManager()->SetModel(model.mSettings, selected);
                    UpdateObjects();
                }
                break;
                case EDIT_POS_Z:
                {
                    LevelModel model = APP.GetLevelManager()->GetModel(selected);
                    model.mSettings.mCenter.Z = wcstof(edit->getText(), NULL);
                    APP.GetLevelManager()->SetModel(model.mSettings, selected);
                    UpdateObjects();
                }
                break;
                case EDIT_ROT_X:
                {
                    LevelModel model = APP.GetLevelManager()->GetModel(selected);
                    model.mSettings.mRotation.X = wcstof(edit->getText(), NULL);
                    APP.GetLevelManager()->SetModel(model.mSettings, selected);
                    UpdateObjects();
                }
                break;
                case EDIT_ROT_Y:
                {
                    LevelModel model = APP.GetLevelManager()->GetModel(selected);
                    model.mSettings.mRotation.Y = wcstof(edit->getText(), NULL);
                    APP.GetLevelManager()->SetModel(model.mSettings, selected);
                    UpdateObjects();
                }
                break;
                case EDIT_ROT_Z:
                {
                    LevelModel model = APP.GetLevelManager()->GetModel(selected);
                    model.mSettings.mRotation.Z = wcstof(edit->getText(), NULL);
                    APP.GetLevelManager()->SetModel(model.mSettings, selected);
                    UpdateObjects();
                }
                break;
                default:
                break;
            }
        }
        break;
        case EGET_LISTBOX_CHANGED:
        {
            switch(id)
            {
                case LIST_LEVELMODELS:
                {
                    IGUIListBox * listBox = static_cast<IGUIListBox*>(event.GUIEvent.Caller);
                    if ( !listBox )
                        break;
                    int selected = listBox->getSelected();
                    if ( selected >= 0 )
                    {
                        EDIT_MODE mode = APP.GetEditor()->GetMode();
                        APP.GetEditor()->SetMode(mode, selected);

                        UpdateObjects();
                    }
                }
                default:
                break;
            }
        }
        break;

        case EGET_CHECKBOX_CHANGED:
        {
            IGUICheckBox *cb = static_cast<IGUICheckBox*>(event.GUIEvent.Caller);
            if ( !cb )
                break;

            IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndObjects->getElementFromId(LIST_LEVELMODELS, false));
            if ( !listBox )
                break;
            int selected = listBox->getSelected();

            switch(id)
            {
                case CHECKBOX_SET_LEVELMODEL:
                {
                    if ( cb->isChecked() && selected > 0 )
                    {
                        APP.GetEditor()->SetMode(EM_SET_MODEL, selected);
                    }
                    else
                    {
                        APP.GetEditor()->SetMode(EM_NONE, selected);
                    }

                }
                break;
            }
        }
        break;

        default:
        break;
    }

    return false;
}

// this is more of a hack - use an own eventreceiver class if you need it once more
void GuiEditor::OnModelSelected(std::wstring name_)
{
    // load the model file, selected in the file open dialog
    std::string str(name_.begin(), name_.end());

    APP.GetLevelManager()->AddModel(str.c_str());
    UpdateObjects();
}

bool GuiEditor::OnEvent(const SEvent &event)
{
    if ( !mWndEditor )
        return false;

    if ( mWndPlayerStart && OnEventPlayerStart(event) )
        return true;
    if ( mWndTrackMarker && OnEventTrackMarker(event) )
        return true;
    if ( mWndTeleportSource && OnEventTeleportSource(event) )
        return true;
    if ( mWndTeleportTarget && OnEventTeleportTarget(event) )
        return true;
    if ( mWndFinishLine && OnEventFinishLine(event) )
        return true;
    if ( mWndSounds && OnEventSounds(event) )
        return true;
    if ( mWndObjects && OnEventObjects(event) )
        return true;
    if ( mGuiNodeManager->GetNodeWindow() && mGuiNodeManager->OnEvent(event) )
        return true;
    if ( mGuiModelList && mGuiModelList->OnEvent(event) )
        return true;

    switch ( event.EventType )
    {
        case EET_GUI_EVENT:
        {
            assert(event.GUIEvent.Caller);
            s32 id = event.GUIEvent.Caller->getID();
            switch(event.GUIEvent.EventType)
            {
                case EGET_ELEMENT_CLOSED:
                {
                    APP.GetEditor()->SetMode(EM_NONE, -1);
                    switch ( id )
                    {
                        case WND_NODE_MANAGER:
                        {
                            mGuiNodeManager->InfoNodeWindowRemoved();
                        }
                        break;
                        case GUI_WND_EDITOR:
                            mWndEditor = NULL;
                            RemoveAllSubWindows(NULL);
                        break;
                        case WND_PLAYER_START:
                            mWndPlayerStart = NULL;
                        break;
                        case WND_TRACK_MARKER:
                            mWndTrackMarker = NULL;
                        break;
                        case WND_TELEPORT_SOURCE:
                            mWndTeleportSource = NULL;
                        break;
                        case WND_TELEPORT_TARGET:
                            mWndTeleportTarget = NULL;
                        break;
                        case WND_FINISH_LINE:
                            mWndFinishLine = NULL;
                        break;
                        case WND_SOUNDS:
                            mWndSounds = NULL;
                        break;
                        case WND_OBJECTS:
                            mWndObjects = NULL;
                        break;
                    }
                }
                break;
                case EGET_BUTTON_CLICKED:
                {
                    switch(id)
                    {
                        case BTN_NODES:
                            OnNodes();
                            return true;
                        case BTN_PLAYER_START:
                            OnPlayerStart();
                            return true;
                        case BTN_TRACK_MARKER:
                            OnTrackMarker();
                            return true;
                        case BTN_TELEPORT_SOURCE:
                            OnTeleportSource();
                            return true;
                        case BTN_TELEPORT_TARGET:
                            OnTeleportTarget();
                            return true;
                        case BTN_FINISH_LINE:
                            OnFinishLine();
                            return true;
                        case BTN_SOUNDS:
                            OnSounds();
                            return true;
                        case BTN_AI:
                            OnAi();
                            return true;
                        case BTN_OBJECTS:
                            OnObjects();
                            return true;
                        case BTN_SAVE_LEVEL:
                            APP.GetLevelManager()->SaveCurrentTrackData();
                            return true;

                        default:
                        break;
                    }
                }
                default:
                break;
            }
        }
        default:
        break;
    }

    return false;
}

void GuiEditor::UpdatePlayerStart()
{
    if ( !mWndPlayerStart )
        return;

    IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndPlayerStart->getElementFromId(LIST_LEVEL_STARTS, false));
    if ( !listBox )
        return;

    IGUICheckBox *cb = static_cast<IGUICheckBox*>(mWndPlayerStart->getElementFromId(CHECKBOX_SET_LEVEL_START, false));
    if ( !cb )
        return;

    int editSize=1024;
    wchar_t editText[editSize+1];
    for ( int i=0; i < MAX_SPAWNS; ++i )
    {
        TrackStart & trackStart = APP.GetLevelManager()->GetTrackStart(i);
        if ( !trackStart.mSettings.mIsValid )
            swprintf_hc(editText, editSize, L"undefined");
        else
            swprintf_hc(editText, editSize, L"%.0f %.0f %.0f", trackStart.mSettings.mCenter.X, trackStart.mSettings.mCenter.Y, trackStart.mSettings.mCenter.Z);
        std::wstring str( editText );

        listBox->setItem(i, str.c_str(), 0);
    }

    if ( APP.GetEditor()->GetMode() == EM_SET_PLAYER_START )
    {
        cb->setChecked(true);
        listBox->setSelected(APP.GetEditor()->GetEditIndex());
    }
    else
    {
        cb->setChecked(false);
    }
}

void GuiEditor::UpdateTrackMarker()
{
    if ( !mWndTrackMarker )
        return;

    IGUICheckBox *cbSet = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_SET_TRACKMARKER, false));
    IGUICheckBox *cbInsert = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_INSERT_TRACKMARKER, false));
    IGUICheckBox *cbAppend = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_APPEND_TRACKMARKER, false));
    if ( !cbSet || !cbInsert || !cbAppend)
        return;

    switch ( APP.GetEditor()->GetMode() )
    {
        case EM_SET_TRACKMARKER:
            cbSet->setChecked(true);
            cbInsert->setChecked(false);
            cbAppend->setChecked(false);
        break;
        case EM_INSERT_TRACKMARKER:
            cbSet->setChecked(false);
            cbInsert->setChecked(true);
            cbAppend->setChecked(false);
        break;
        case EM_APPEND_TRACKMARKER:
            cbSet->setChecked(false);
            cbInsert->setChecked(false);
            cbAppend->setChecked(true);
        break;
        default:
            cbSet->setChecked(false);
            cbInsert->setChecked(false);
            cbAppend->setChecked(false);
    }

    IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndTrackMarker->getElementFromId(LIST_TRACKMARKERS, false));
    if ( !listBox )
        return;

    listBox->clear();
    int editSize=1024;
    wchar_t editText[editSize+1];
    for ( unsigned int i=0; i < APP.GetLevelManager()->GetNrOfTrackMarkers(); ++i )
    {
        TrackMarker marker = APP.GetLevelManager()->GetTrackMarker(i);
        if ( !marker.mSettings.mIsValid )
            swprintf_hc(editText, editSize, L"undefined");
        else
            swprintf_hc(editText, editSize, L"%.0f %.0f %.0f", marker.mSettings.mCenter.X, marker.mSettings.mCenter.Y, marker.mSettings.mCenter.Z);
        std::wstring str( editText );
        listBox->addItem(str.c_str());
    }

    listBox->setSelected(APP.GetEditor()->GetEditIndex());

    int selected = listBox->getSelected();
    if ( selected >= 0 )
    {
        IGUICheckBox *cbLeft = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_WALL_LEFT, false));
        IGUICheckBox *cbRight = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_WALL_RIGHT, false));
//        IGUICheckBox *cbTop = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_WALL_TOP, false));
        IGUICheckBox *cbBottom = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_WALL_BOTTOM, false));
        if ( !cbLeft || !cbRight || !cbBottom )
            return;

        TrackMarker marker = APP.GetLevelManager()->GetTrackMarker(selected);
        if ( marker.mSettings.mHasLeftWall )
            cbLeft->setChecked(true);
        else
            cbLeft->setChecked(false);
        if ( marker.mSettings.mHasRightWall )
            cbRight->setChecked(true);
        else
            cbRight->setChecked(false);
//        if ( marker.mSettings.mHasTopWall )
//            cbTop->setChecked(true);
//        else
//            cbTop->setChecked(false);
        if ( marker.mSettings.mHasBottomWall )
            cbBottom->setChecked(true);
        else
            cbBottom->setChecked(false);

        IGUICheckBox *cbDefaultSize = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_DEFAULT_SIZE, false));
        if ( cbDefaultSize )
        {
            if ( marker.mSettings.mUseDefaultSizes )
                cbDefaultSize->setChecked(true);
            else
                cbDefaultSize->setChecked(false);
        }

        IGUICheckBox *cbRelocate = static_cast<IGUICheckBox*>(mWndTrackMarker->getElementFromId(CHECKBOX_RELOCATE, false));
        if ( cbRelocate )
        {
            if ( marker.mSettings.mRelocate )
                cbRelocate->setChecked(true);
            else
                cbRelocate->setChecked(false);
        }
    }
}

void GuiEditor::UpdateTeleportSource()
{
    if ( !mWndTeleportSource )
        return;

    IGUIStaticText * staticText = static_cast<IGUIStaticText*>(mWndTeleportSource->getElementFromId(STATIC_TP_START, false));
    if ( !staticText )
        return;

    IGUICheckBox *cb = static_cast<IGUICheckBox*>(mWndTeleportSource->getElementFromId(CHECKBOX_SET_TP_START, false));
    if ( !cb )
        return;

    int editSize=1024;
    wchar_t editText[editSize+1];

    TrackMarker & marker = APP.GetLevelManager()->GetTpSource();
    swprintf_hc(editText, editSize, L"%.0f %.0f %.0f", marker.mSettings.mCenter.X, marker.mSettings.mCenter.Y, marker.mSettings.mCenter.Z);
    std::wstring str( editText );
    staticText->setText(str.c_str());

    if ( APP.GetEditor()->GetMode() == EM_SET_TP_SOURCE )
    {
        cb->setChecked(true);
    }
    else
    {
        cb->setChecked(false);
    }
}

void GuiEditor::UpdateTeleportTarget()
{
    if ( !mWndTeleportTarget )
        return;

    IGUIStaticText * staticText = static_cast<IGUIStaticText*>(mWndTeleportTarget->getElementFromId(STATIC_TP_TARGET, false));
    if ( !staticText )
        return;

    IGUICheckBox *cb = static_cast<IGUICheckBox*>(mWndTeleportTarget->getElementFromId(CHECKBOX_SET_TP_TARGET, false));
    if ( !cb )
        return;

    int editSize=1024;
    wchar_t editText[editSize+1];

    TrackMarker & marker = APP.GetLevelManager()->GetTpTarget();
    swprintf_hc(editText, editSize, L"%.0f %.0f %.0f", marker.mSettings.mCenter.X, marker.mSettings.mCenter.Y, marker.mSettings.mCenter.Z);
    std::wstring str( editText );
    staticText->setText(str.c_str());

    if ( APP.GetEditor()->GetMode() == EM_SET_TP_TARGET )
    {
        cb->setChecked(true);
    }
    else
    {
        cb->setChecked(false);
    }
}

void GuiEditor::UpdateFinishLine()
{
    if ( !mWndFinishLine )
        return;

    IGUIStaticText * staticText = static_cast<IGUIStaticText*>(mWndFinishLine->getElementFromId(STATIC_FINISH_LINE, false));
    if ( !staticText )
        return;

    IGUICheckBox *cb = static_cast<IGUICheckBox*>(mWndFinishLine->getElementFromId(CHECKBOX_SET_FINISH_LINE, false));
    if ( !cb )
        return;

    int editSize=1024;
    wchar_t editText[editSize+1];

    TrackMarker& marker = APP.GetLevelManager()->GetFinishLine();
    swprintf_hc(editText, editSize, L"%.0f %.0f %.0f", marker.mSettings.mCenter.X, marker.mSettings.mCenter.Y, marker.mSettings.mCenter.Z);
    std::wstring str( editText );
    staticText->setText(str.c_str());

    if ( APP.GetEditor()->GetMode() == EM_SET_FINISH_LINE )
    {
        cb->setChecked(true);
    }
    else
    {
        cb->setChecked(false);
    }
}

void GuiEditor::UpdateSounds()
{
    if ( !mWndSounds )
        return;
}

void GuiEditor::UpdateObjects()
{
    if ( !mWndObjects )
        return;

    IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndObjects->getElementFromId(LIST_LEVELMODELS, false));
    if ( !listBox )
        return;

    listBox->clear();
    int editSize=1024;
    wchar_t editText[editSize+1];
    for ( unsigned int i=0; i < APP.GetLevelManager()->GetNrOfModels(); ++i )
    {
        LevelModel model = APP.GetLevelManager()->GetModel(i);
        std::wstring str(model.mSettings.mName.begin(), model.mSettings.mName.end());
        listBox->addItem(str.c_str());
    }

    listBox->setSelected(APP.GetEditor()->GetEditIndex());

    IGUICheckBox *cb = static_cast<IGUICheckBox*>(mWndObjects->getElementFromId(CHECKBOX_SET_LEVELMODEL, false));
    if ( cb )
    {
        if ( APP.GetEditor()->GetMode() == EM_SET_MODEL )
        {
            cb->setChecked(true);
        }
        else
        {
            cb->setChecked(false);
        }
    }

    int selected = listBox->getSelected();
    if ( selected >= 0 )
    {
        LevelModel model = APP.GetLevelManager()->GetModel(selected);

        IGUIEditBox * editPosX = static_cast<IGUIEditBox*>(mWndObjects->getElementFromId(EDIT_POS_X, false));
        IGUIEditBox * editPosY = static_cast<IGUIEditBox*>(mWndObjects->getElementFromId(EDIT_POS_Y, false));
        IGUIEditBox * editPosZ = static_cast<IGUIEditBox*>(mWndObjects->getElementFromId(EDIT_POS_Z, false));
        IGUIEditBox * editRotX = static_cast<IGUIEditBox*>(mWndObjects->getElementFromId(EDIT_ROT_X, false));
        IGUIEditBox * editRotY = static_cast<IGUIEditBox*>(mWndObjects->getElementFromId(EDIT_ROT_Y, false));
        IGUIEditBox * editRotZ = static_cast<IGUIEditBox*>(mWndObjects->getElementFromId(EDIT_ROT_Z, false));
        IGUISpinBox * spinAmbR = static_cast<IGUISpinBox*>(mWndObjects->getElementFromId(SPIN_MODEL_AMB_R, false));
        IGUISpinBox * spinAmbG = static_cast<IGUISpinBox*>(mWndObjects->getElementFromId(SPIN_MODEL_AMB_G, false));
        IGUISpinBox * spinAmbB = static_cast<IGUISpinBox*>(mWndObjects->getElementFromId(SPIN_MODEL_AMB_B, false));
        if ( editPosX && editPosY && editPosZ && editRotX && editRotY && editRotZ
            && spinAmbR && spinAmbG && spinAmbB)
        {
            swprintf_hc(editText, editSize, L"%.1f", model.mSettings.mCenter.X);
            editPosX->setText(editText);
            swprintf_hc(editText, editSize, L"%.1f", model.mSettings.mCenter.Y);
            editPosY->setText(editText);
            swprintf_hc(editText, editSize, L"%.1f", model.mSettings.mCenter.Z);
            editPosZ->setText(editText);

            swprintf_hc(editText, editSize, L"%.1f", model.mSettings.mRotation.X);
            editRotX->setText(editText);
            swprintf_hc(editText, editSize, L"%.1f", model.mSettings.mRotation.Y);
            editRotY->setText(editText);
            swprintf_hc(editText, editSize, L"%.1f", model.mSettings.mRotation.Z);
            editRotZ->setText(editText);

            spinAmbR->setValue(model.mSettings.mAmbientRed);
            spinAmbG->setValue(model.mSettings.mAmbientGreen);
            spinAmbB->setValue(model.mSettings.mAmbientBlue);
        }
    }
}

#endif // HC1_ENABLE_EDITOR
