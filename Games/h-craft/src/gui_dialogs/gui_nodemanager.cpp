// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_nodemanager.h"
#include "../gui_ids.h"
#include "../node_manager.h"
#include "../main.h"
#include "../helper_file.h"
#include "../config.h"
#include "../globals.h"
#include <assert.h>

using namespace irr;
using namespace gui;

GuiNodeManager::GuiNodeManager( IGUIEnvironment * env_)
    : mEnvironment(env_)
    , mWndNodeManager(NULL)
    , mMode(GNM_NONE)
    , mNodeType(-1)
    , mActiveModel(-1)
    , mNodeActiveModel(NULL)
    , mActiveNode(-1)
    , mEditTop(0)
    , mEditNextTop(0)
{
}

GuiNodeManager::~GuiNodeManager()
{
}

bool GuiNodeManager::OnEvent(const SEvent &event)
{
    switch ( event.EventType )
    {
        case EET_GUI_EVENT:
        {
            s32 id = event.GUIEvent.Caller->getID();

            if (    id == STATIC_MODELS
                ||  id == STATIC_NODES
                ||  id == STATIC_NODE_EDIT )
                return false;

            switch(event.GUIEvent.EventType)
            {
                case EGET_FILE_SELECTED:
                {
                    switch ( id )
                    {
                        case FILEDLG_OPEN_MESH:
                        {
                            if ( mMode == GNM_MODELS || (mMode == GNM_NODES && APP.GetNodeManager()->GetNode(mActiveModel)) )
                            {
                                IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
                                std::wstring wstr(dialog->getFileName());
                                std::string strName(wstr.begin(), wstr.end());
                                std::string strFile(strName);
                                strFile = APP.GetConfig()->MakeRelativeMediaName(strFile);
                                ExtFile::ErasePathFromFilename(strName);
                                ExtFile::EraseFiletypeFromFilename(strName);

                                NodeSettingsUnknown* node = NULL;
                                if ( mMode == GNM_MODELS )
                                {
                                    node = APP.GetNodeManager()->AddNode(strName.c_str(), NT_MESH);
                                    mActiveModel = (int)APP.GetNodeManager()->GetNumNodes() - 1;
                                }
                                else if ( mMode == GNM_NODES )
                                {
                                    NodeSettingsUnknown* model = APP.GetNodeManager()->GetNode(mActiveModel);
                                    if ( model )
                                    {
                                        node = model->AddChild(strName.c_str(), NT_MESH);
                                        mActiveNode = (int)model->GetNumChilds() - 1;
                                    }
                                }
                                if ( node )
                                {
                                    static_cast<NodeSettingsMesh*>(node)->mFile = strFile;
                                    if (strstr(strFile.c_str(), ".X"))
                                    {
                                        static_cast<NodeSettingsMesh*>(node)->mScale.set(100.f, 100.f, 100.f);
                                    }
                                }
                            }
                            Update();
                        }
                        break;
                        case FILEDLG_OPEN_ANI_MESH:
                        {
                            if ( mMode == GNM_MODELS || (mMode == GNM_NODES && APP.GetNodeManager()->GetNode(mActiveModel)) )
                            {
                                IGUIFileOpenDialog* dialog = (IGUIFileOpenDialog*)event.GUIEvent.Caller;
                                std::wstring wstr(dialog->getFileName());
                                std::string strName(wstr.begin(), wstr.end());
                                std::string strFile(strName);
                                strFile = APP.GetConfig()->MakeRelativeMediaName(strFile);
                                ExtFile::ErasePathFromFilename(strName);
                                ExtFile::EraseFiletypeFromFilename(strName);

                                NodeSettingsUnknown* node = NULL;
                                if ( mMode == GNM_MODELS )
                                {
                                    node = APP.GetNodeManager()->AddNode(strName.c_str(), NT_ANIMATED_MESH);
                                    mActiveModel = (int)APP.GetNodeManager()->GetNumNodes() - 1;
                                }
                                else if ( mMode == GNM_NODES )
                                {
                                    NodeSettingsUnknown* model = APP.GetNodeManager()->GetNode(mActiveModel);
                                    if ( model )
                                    {
                                        node = model->AddChild(strName.c_str(), NT_ANIMATED_MESH);
                                        mActiveNode = (int)model->GetNumChilds() - 1;
                                    }
                                }
                                if ( node )
                                {
                                    static_cast<NodeSettingsMesh*>(node)->mFile = strFile;
                                    if (strstr(strFile.c_str(), ".X"))
                                    {
                                        static_cast<NodeSettingsMesh*>(node)->mScale.set(100.f, 100.f, 100.f);
                                    }
                                }
                            }
                            Update();
                        }
                        break;
                        default:
                        break;
                    }
                }
                break;

                case EGET_BUTTON_CLICKED:
                {
                    switch(id)
                    {
                        case BTN_SAVE_NODEMANAGER:
                        {
                            APP.GetNodeManager()->SaveSettings();
                            break;
                        }
                        case BTN_LOAD_NODEMANAGER:
                        {
                            APP.GetNodeManager()->LoadSettings();
                            Update();
                            break;
                        }
                        case BTN_OPEN_NODES:
                        {
                            SetMode(GNM_NODES);
                            Update();
                            break;
                        }
                        case BTN_OPEN_MODEL:
                        {
                            SetMode(GNM_MODELS);
                            Update();
                            break;
                        }
                        case BTN_NODE_ADD_EMPTY:
                        {
                            if ( mMode == GNM_MODELS )
                            {
                                APP.GetNodeManager()->AddNode("unnamed", NT_UNKNOWN);
                                mActiveModel = (int)APP.GetNodeManager()->GetNumNodes() - 1;
                            }
                            else if ( mMode == GNM_NODES && mActiveModel >= 0 )
                            {
                                NodeSettingsUnknown* model = APP.GetNodeManager()->GetNode(mActiveModel);
                                if ( model )
                                {
                                    model->AddChild("unnamed", NT_UNKNOWN);
                                    mActiveNode = (int)model->GetNumChilds() - 1;
                                }
                            }
                            Update();
                            break;
                        }
                        case BTN_NODE_ADD_LIGHT:
                        {
                            if ( mMode == GNM_MODELS )
                            {
                                APP.GetNodeManager()->AddNode("light", NT_LIGHT);
                                mActiveModel = (int)APP.GetNodeManager()->GetNumNodes() - 1;
                            }
                            else if ( mMode == GNM_NODES && mActiveModel >= 0 )
                            {
                                NodeSettingsUnknown* model = APP.GetNodeManager()->GetNode(mActiveModel);
                                if ( model )
                                {
                                    model->AddChild("light", NT_LIGHT);
                                    mActiveNode = (int)model->GetNumChilds() - 1;
                                }
                            }
                            Update();
                            break;
                        }
                        case BTN_NODE_REMOVE:
                        {
                            if ( mMode == GNM_MODELS && mActiveModel >= 0)
                            {
                                if ( mNodeActiveModel )
                                {
                                    mNodeActiveModel->remove();
                                    mNodeActiveModel = NULL;
                                }

                                APP.GetNodeManager()->RemoveNode(mActiveModel);
                                --mActiveModel;
                                if ( mActiveModel < 0 && APP.GetNodeManager()->GetNumNodes() > 0 )
                                    mActiveModel = 0;
                            }
                            else if ( mMode == GNM_NODES && mActiveNode >= 0)
                            {
                                NodeSettingsUnknown* model = APP.GetNodeManager()->GetNode(mActiveModel);
                                if ( model )
                                {
                                    model->RemoveChild(mActiveNode);
                                    --mActiveNode;
                                    if ( mActiveNode < 0 && model->GetNumChilds() > 0 )
                                        mActiveNode = 0;
                                }
                            }
                            Update();
                            break;
                        }
                        case BTN_NODE_ADD_MESH:
                        {
                            if ( mMode == GNM_MODELS || (mMode == GNM_NODES && APP.GetNodeManager()->GetNode(mActiveModel)) )
                            {
                                io::IFileSystem *fs = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem();
                                assert(fs);
                                fs->changeWorkingDirectoryTo(APP.GetConfig()->GetPathLevels().c_str());
                                mEnvironment->addFileOpenDialog(L"static mesh (.obj)", /*modal*/false, /*parent*/0, FILEDLG_OPEN_MESH);
                            }
                            break;
                        }
                        case BTN_NODE_ADD_ANIM_MESH:
                        {
                            if ( mMode == GNM_MODELS || (mMode == GNM_NODES && APP.GetNodeManager()->GetNode(mActiveModel)) )
                            {
                                io::IFileSystem *fs = APP.GetIrrlichtManager()->GetIrrlichtDevice()->getFileSystem();
                                assert(fs);
                                fs->changeWorkingDirectoryTo(APP.GetConfig()->GetPathLevels().c_str());
                                mEnvironment->addFileOpenDialog(L"animated mesh (.X)", /*modal*/false, /*parent*/0, FILEDLG_OPEN_ANI_MESH);
                            }
                            break;
                        }
                        default:
                        break;
                    }
                    break;
                }
                case EGET_CHECKBOX_CHANGED:
                {
                    switch(id)
                    {
                        case CHECKBOX_NODE_CAST_SHADOWS:
                        {
                            IGUICheckBox* cb = static_cast<IGUICheckBox*>(event.GUIEvent.Caller);
                            if ( !cb )
                                break;
                            NodeSettingsUnknown* node = GetActiveNode();
                            if ( !node )
                                break;
                            NodeSettingsLight* light = node->IsOfClassType(NT_LIGHT) ? static_cast<NodeSettingsLight*>(node) : NULL;
                            if ( !light )
                                break;
                            light->mCastShadows = cb->isChecked();
                            Update();
                        }
                        break;
                        case CHECKBOX_NODE_BF_CULLING:
                        {
                            IGUICheckBox* cb = static_cast<IGUICheckBox*>(event.GUIEvent.Caller);
                            if ( !cb )
                                break;
                            NodeSettingsUnknown* node = GetActiveNode();
                            if ( !node )
                                break;
                            NodeSettingsMesh* nodeMesh = node->IsOfClassType(NT_MESH) ? static_cast<NodeSettingsMesh*>(node) : NULL;
                            if ( !nodeMesh )
                                break;
                            nodeMesh->mBackfaceCulling = cb->isChecked();
                            Update();
                        }
                        break;
                        default:
                        break;
                    }
                    break;
                }
                case EGET_LISTBOX_CHANGED:
                {
                    switch(id)
                    {
                        case LIST_MODELS:
                        {
                            IGUIListBox * listBox = static_cast<IGUIListBox*>(event.GUIEvent.Caller);
                            if ( !listBox )
                                break;
                            mActiveModel = listBox->getSelected();
                            UpdateNodeType();
                            break;
                        }
                        case LIST_NODES:
                        {
                            IGUIListBox * listBox = static_cast<IGUIListBox*>(event.GUIEvent.Caller);
                            if ( !listBox )
                                break;
                            mActiveNode = listBox->getSelected();
                            UpdateNodeType();
                            break;
                        }
                        default:
                        break;
                    }
                    break;
                }
                case EGET_EDITBOX_ENTER:
                {
                    switch(id)
                    {
                        case EDIT_NODE_NAME:
                        {
                            IGUIEditBox * editName = static_cast<IGUIEditBox*>(event.GUIEvent.Caller);
                            if ( !editName  )
                                break;
                            NodeSettingsUnknown* node = GetActiveNode();
                            if ( !node )
                                break;
                            std::wstring wstr( editName->getText() );
                            node->mName = std::string( wstr.begin(), wstr.end() );
                            Update();

                            return true;
                        }
                        default:
                        break;
                    }
                    break;
                }
                case EGET_SPINBOX_CHANGED:
                {
                    NodeSettingsUnknown* node = GetActiveNode();
                    if ( !node )
                        break;
                    NodeSettingsMesh* nodeMesh = node->IsOfClassType(NT_MESH) ? static_cast<NodeSettingsMesh*>(node) : NULL;
                    NodeSettingsAnimatedMesh* nodeAniMesh = node->IsOfClassType(NT_ANIMATED_MESH) ? static_cast<NodeSettingsAnimatedMesh*>(node) : NULL;
                    NodeSettingsLight* nodeLight = node->IsOfClassType(NT_LIGHT) ? static_cast<NodeSettingsLight*>(node) : NULL;

                    IGUISpinBox * spin = static_cast<IGUISpinBox*>(event.GUIEvent.Caller);
                    if ( !spin )
                        break;

                    switch(id)
                    {
                        case SPIN_NODE_POS_X:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mCenter.X = spin->getValue();
                                Update();
                                return true;
                            }
                            if ( nodeMesh )
                            {
                                nodeMesh->mCenter.X = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_POS_Y:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mCenter.Y = spin->getValue();
                                Update();
                                return true;
                            }
                            if ( nodeMesh )
                            {
                                nodeMesh->mCenter.Y = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_POS_Z:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mCenter.Z = spin->getValue();
                                Update();
                                return true;
                            }
                            if ( nodeMesh )
                            {
                                nodeMesh->mCenter.Z = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_ROT_X:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mRotation.X = spin->getValue();
                                Update();
                                return true;
                            }
                            if ( nodeMesh )
                            {
                                nodeMesh->mRotation.X = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_ROT_Y:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mRotation.Y = spin->getValue();
                                Update();
                                return true;
                            }
                            if ( nodeMesh )
                            {
                                nodeMesh->mRotation.Y = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_ROT_Z:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mRotation.Z = spin->getValue();
                                Update();
                                return true;
                            }
                            if ( nodeMesh )
                            {
                                nodeMesh->mRotation.Z = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_SCALE_X:
                        {
                            if ( !nodeMesh )
                                break;
                            nodeMesh->mScale.X = spin->getValue();
                            Update();
                            return true;
                        }
                        case SPIN_NODE_SCALE_Y:
                        {
                            if ( !nodeMesh )
                                break;
                            nodeMesh->mScale.Y = spin->getValue();
                            Update();
                            return true;
                        }
                        case SPIN_NODE_SCALE_Z:
                        {
                            if ( !nodeMesh )
                                break;
                            nodeMesh->mScale.Z = spin->getValue();
                            Update();
                            return true;
                        }
                        case SPIN_ANIM_SPEED:
                        {
                            if ( !nodeAniMesh )
                                break;
                            nodeAniMesh->mAnimationSpeed = (int)spin->getValue();
                            Update();
                            return true;
                        }
                        case SPIN_NODE_RADIUS:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mRadius = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_DIF_R:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mDiffuseColor.r = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_DIF_G:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mDiffuseColor.g = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_DIF_B:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mDiffuseColor.b = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_DIF_A:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mDiffuseColor.a = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_SPC_R:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mSpecularColor.r = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_SPC_G:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mSpecularColor.g = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_SPC_B:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mSpecularColor.b = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        case SPIN_NODE_SPC_A:
                        {
                            if ( nodeLight )
                            {
                                nodeLight->mSpecularColor.a = spin->getValue();
                                Update();
                                return true;
                            }
                        }
                        break;
                        default:
                        break;
                    }
                    break;
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

NodeSettingsUnknown *GuiNodeManager::GetActiveNode()
{
    if ( mMode == GNM_NONE )
        return NULL;
    NodeSettingsUnknown* model = mActiveModel >= 0 ? APP.GetNodeManager()->GetNode(mActiveModel) : NULL;
    if ( mMode == GNM_MODELS )
        return model;
    if ( mMode == GNM_NODES )
    {
        if ( !model )
            return NULL;
        if ( mActiveNode < 0 )
            return NULL;

        return model->GetChild(mActiveNode);
    }
    return NULL;
}

void GuiNodeManager::CreateNodeWindow()
{
    mRectWindow = core::rect<s32>(0,50,250,479);
    mWndNodeManager = mEnvironment->addWindow(mRectWindow,
		false, L"models/nodes", 0, WND_NODE_MANAGER);
    assert(mWndNodeManager);

    mEditTop = 215;

    SetMode(GNM_MODELS);
    Update();
}

void GuiNodeManager::SetMode(Mode mode_)
{
    if ( mode_ != mMode )
    {
        // remove gui elements of old mode
        if ( mMode == GNM_MODELS )
        {
            IGUIElement* e = NULL;
            if ( (e = mWndNodeManager->getElementFromId(STATIC_MODELS, true) ) )
            {
                e->remove();
                e = NULL;
            }
        }
        else if ( mMode == GNM_NODES )
        {
            IGUIElement* e = NULL;
            if ( (e = mWndNodeManager->getElementFromId(STATIC_NODES, true) ) )
            {
                e->remove();
                e = NULL;
            }
        }

        // set new mode with it's gui elements
        if ( mode_ == GNM_MODELS)
        {
//            mActiveModel = -1;

            IGUIStaticText * parent = mEnvironment->addStaticText (L"", core::rect<s32>(0,25,mRectWindow.getWidth(), mEditTop), true, false, mWndNodeManager, STATIC_MODELS);

            int top = 0;
            int left = 10;
            int width = 150;
            int height = 15;

            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_OPEN_NODES, L"to Childs");
            mEnvironment->addButton(core::rect<s32>(left+width+10,top,left+width+80,top+height), parent, BTN_LOAD_NODEMANAGER, L"LOAD");
            top += height+5;
            mEnvironment->addButton(core::rect<s32>(left+width+10,top,left+width+80,top+height), parent, BTN_SAVE_NODEMANAGER, L"SAVE");
            top += height+5;

            core::rect< s32 > listRect(left, top, left + width, top+160 );
            mEnvironment->addListBox(listRect, parent, LIST_MODELS );

            left += width +10;
            width = 70;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_ADD_EMPTY, L"+ Empty");
            top += height+5;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_ADD_MESH, L"+ Mesh");
            top += height+5;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_ADD_ANIM_MESH, L"+ Animesh");
            top += height+5;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_ADD_LIGHT, L"+ Light");
            top += height+5;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_REMOVE, L"Remove");
            top += height+5;
        }
        else if ( mode_ == GNM_NODES )
        {
//            mActiveNode = -1;

            IGUIStaticText * parent = mEnvironment->addStaticText (L"", core::rect<s32>(0,25,mRectWindow.getWidth(), mEditTop), true, false, mWndNodeManager, STATIC_NODES);

            int top = 0;
            int left = 10;
            int width = 150;
            int height = 15;

            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_OPEN_MODEL, L"to Parent");
            mEnvironment->addButton(core::rect<s32>(left+width+10,top,left+width+80,top+height), parent, BTN_SAVE_NODEMANAGER, L"SAVE");
            top += height+5;

            core::rect< s32 > listRect = core::rect< s32 >(left, top, left + width, top+160 );
            mEnvironment->addListBox(listRect, parent, LIST_NODES );

            left += width +10;
            width = 70;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_ADD_EMPTY, L"+ Empty");
            top += height+5;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_ADD_MESH, L"+ Mesh");
            top += height+5;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_ADD_ANIM_MESH, L"+ Animesh");
            top += height+5;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_ADD_LIGHT, L"+ Light");
            top += height+5;
            mEnvironment->addButton(core::rect<s32>(left,top,left+width,top+height), parent, BTN_NODE_REMOVE, L"Remove");
            top += height+5;
        }
        else if ( mode_ == GNM_NONE )
        {
//            mActiveModel = -1;
//            mActiveNode = -1;
        }
        mMode = mode_;
        UpdateNodeType();
    }
}

void GuiNodeManager::InfoNodeWindowRemoved()
{
    mWndNodeManager = NULL;
    mNodeType = -1;
    mMode = GNM_NONE;
    if ( mNodeActiveModel )
    {
        mNodeActiveModel->remove();
        mNodeActiveModel = NULL;
    }
}

void GuiNodeManager::RemoveNodeWindow()
{
    SetMode(GNM_NONE);
    IGUIElement* root = mEnvironment->getRootGUIElement();
    IGUIElement* e  = root->getElementFromId(WND_NODE_MANAGER, true);
	if ( e )
	{
        e->remove();
	}

	InfoNodeWindowRemoved();
}

void GuiNodeManager::Update()
{
    NodeSettingsUnknown * nodeSelected = NULL;
    if ( mMode == GNM_MODELS)
    {
        IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndNodeManager->getElementFromId(LIST_MODELS, true));
        if ( !listBox )
            return;
        listBox->clear();

        for ( unsigned int i=0 ; i < APP.GetNodeManager()->GetNumNodes(); ++i)
        {
            NodeSettingsUnknown* node = APP.GetNodeManager()->GetNode(i);
            assert(node);
            std::wstring wstr( node->mName.begin(), node->mName.end() );
            listBox->addItem(wstr.c_str());
        }
        listBox->setSelected(mActiveModel);

        if ( mActiveModel >= 0 )
            nodeSelected = APP.GetNodeManager()->GetNode(mActiveModel);

    }
    else if ( mMode == GNM_NODES )
    {
        IGUIListBox * listBox = static_cast<IGUIListBox*>(mWndNodeManager->getElementFromId(LIST_NODES, true));
        if ( !listBox )
            return;
        listBox->clear();

        if ( mActiveModel >= 0 )
            nodeSelected = APP.GetNodeManager()->GetNode(mActiveModel);

        if ( nodeSelected )
        {
            for ( unsigned int i=0; i < nodeSelected->GetNumChilds(); ++i)
            {
                NodeSettingsUnknown* node = nodeSelected->GetChild(i);
                assert(node);
                std::wstring wstr( node->mName.begin(), node->mName.end() );
                listBox->addItem(wstr.c_str());
            }
            if ( mActiveNode >= 0 )
            {
                nodeSelected = nodeSelected->GetChild(mActiveNode);
            }
            else
            {
                nodeSelected = NULL;
            }
            listBox->setSelected(mActiveNode);
        }
    }

    UpdateNodeType();
}

void GuiNodeManager::UpdateNodeType()
{
    int newType = -1;
    NodeSettingsUnknown * modelSelected = NULL;
    NodeSettingsUnknown * nodeSelected = NULL;
    if ( mMode == GNM_MODELS)
    {
        if ( mActiveModel >= 0 )
            modelSelected = APP.GetNodeManager()->GetNode(mActiveModel);
    }
    else if ( mMode == GNM_NODES )
    {
        if ( mActiveModel >= 0 )
            modelSelected = APP.GetNodeManager()->GetNode(mActiveModel);
        if ( modelSelected && mActiveNode >= 0 )
            nodeSelected = modelSelected->GetChild(mActiveNode);
        else
            nodeSelected = NULL;
    }
    if ( nodeSelected )
        newType = nodeSelected->mType;
    else if ( modelSelected )
    {
        newType = modelSelected->mType;
        nodeSelected = modelSelected;
    }

    if ( newType != mNodeType )
    {
        // remove old edit fields
        IGUIElement* e = NULL;
        if ( (e = mWndNodeManager->getElementFromId(STATIC_NODE_EDIT, true) ) )
        {
            e->remove();
            e = NULL;
        }

        // create new edit fields
        if ( nodeSelected )
        {
            core::rect<s32> rectParent(0,mEditTop,mRectWindow.getWidth(), mRectWindow.getHeight());
            IGUIStaticText * parent = mEnvironment->addStaticText (L"", rectParent, true, false, mWndNodeManager, STATIC_NODE_EDIT);
            mEditNextTop = 5;
            int left = 10;
            int width = 70;
            int rowWidth = width+10;
            int withCol = 57;
            int rowWithCol = withCol+3;
            int height = 15;

            if ( nodeSelected->IsOfClassType(NT_UNKNOWN) )
            {
                mEnvironment->addEditBox(L"", core::rect<s32>(left,mEditNextTop,left+width+2*rowWidth,mEditNextTop+height), true, parent, EDIT_NODE_NAME);
                mEditNextTop += height +5;
            }
            if ( nodeSelected->IsOfClassType(NT_MESH) )
            {
                mEnvironment->addStaticText (L"", core::rect<s32>(left,mEditNextTop,left+width+2*rowWidth,mEditNextTop+height), true, false, parent, STATIC_NODE_FILE);
                mEditNextTop += height +5;
                mEnvironment->addStaticText (L"position", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), false, false, parent, -1);
                mEditNextTop += height +5;
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), true, parent, SPIN_NODE_POS_X);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+rowWidth,mEditNextTop,left+width+rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_POS_Y);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+2*rowWidth,mEditNextTop,left+width+2*rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_POS_Z);
                mEditNextTop += height +5;
                mEnvironment->addStaticText (L"rotation", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), false, false, parent, -1);
                mEditNextTop += height +5;
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), true, parent, SPIN_NODE_ROT_X);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+rowWidth,mEditNextTop,left+width+rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_ROT_Y);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+2*rowWidth,mEditNextTop,left+width+2*rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_ROT_Z);
                mEditNextTop += height +5;
                mEnvironment->addStaticText (L"scale", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), false, false, parent, -1);
                mEditNextTop += height +5;
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), true, parent, SPIN_NODE_SCALE_X);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+rowWidth,mEditNextTop,left+width+rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_SCALE_Y);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+2*rowWidth,mEditNextTop,left+width+2*rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_SCALE_Z);
                mEditNextTop += height +5;
                mEnvironment->addCheckBox(false, core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), parent, CHECKBOX_NODE_BF_CULLING, L"bf culling");
                mEditNextTop += height +5;
            }
            if ( nodeSelected->IsOfClassType(NT_ANIMATED_MESH) )
            {
                mEnvironment->addStaticText (L"animspeed", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), false, false, parent, -1);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+rowWidth,mEditNextTop,left+width+rowWidth,mEditNextTop+height), true, parent, SPIN_ANIM_SPEED);
                mEditNextTop += height + 5;
            }
            if ( nodeSelected->IsOfClassType(NT_LIGHT) )
            {
                mEnvironment->addStaticText (L"position", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), false, false, parent, -1);
                mEditNextTop += height +3;
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), true, parent, SPIN_NODE_POS_X);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+rowWidth,mEditNextTop,left+width+rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_POS_Y);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+2*rowWidth,mEditNextTop,left+width+2*rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_POS_Z);
                mEditNextTop += height +5;
                mEnvironment->addStaticText (L"rotation", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), false, false, parent, -1);
                mEditNextTop += height +3;
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), true, parent, SPIN_NODE_ROT_X);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+rowWidth,mEditNextTop,left+width+rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_ROT_Y);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+2*rowWidth,mEditNextTop,left+width+2*rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_ROT_Z);
                mEditNextTop += height +5;
                mEnvironment->addCheckBox(false, core::rect<s32>(left,mEditNextTop,left+2*rowWidth,mEditNextTop+height), parent, CHECKBOX_NODE_CAST_SHADOWS, L"cast shadows");
                mEditNextTop += height +5;
                mEnvironment->addStaticText (L"radius", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), false, false, parent, -1);
                mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+rowWidth,mEditNextTop,left+width+rowWidth,mEditNextTop+height), true, parent, SPIN_NODE_RADIUS);
                mEditNextTop += height +5;
                mEnvironment->addStaticText (L"diffuse", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), false, false, parent, -1);
                mEditNextTop += height +3;
                IGUISpinBox * sb = 0;
                const float colStepsSize = 0.1f;
                sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left,mEditNextTop,left+withCol,mEditNextTop+height), true, parent, SPIN_NODE_DIF_R);
                sb->setStepSize(colStepsSize);
                sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+rowWithCol,mEditNextTop,left+withCol+rowWithCol,mEditNextTop+height), true, parent, SPIN_NODE_DIF_G);
                sb->setStepSize(colStepsSize);
                sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+2*rowWithCol,mEditNextTop,left+withCol+2*rowWithCol,mEditNextTop+height), true, parent, SPIN_NODE_DIF_B);
                sb->setStepSize(colStepsSize);
                sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+3*rowWithCol,mEditNextTop,left+withCol+3*rowWithCol,mEditNextTop+height), true, parent, SPIN_NODE_DIF_A);
                sb->setStepSize(colStepsSize);sb->setStepSize(colStepsSize);
                mEditNextTop += height +5;
                mEnvironment->addStaticText (L"specular", core::rect<s32>(left,mEditNextTop,left+width,mEditNextTop+height), false, false, parent, -1);
                mEditNextTop += height +3;
                sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left,mEditNextTop,left+withCol,mEditNextTop+height), true, parent, SPIN_NODE_SPC_R);
                sb->setStepSize(colStepsSize);
                sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+rowWithCol,mEditNextTop,left+withCol+rowWithCol,mEditNextTop+height), true, parent, SPIN_NODE_SPC_G);
                sb->setStepSize(colStepsSize);
                sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+2*rowWithCol,mEditNextTop,left+withCol+2*rowWithCol,mEditNextTop+height), true, parent, SPIN_NODE_SPC_B);
                sb->setStepSize(colStepsSize);
                sb = mEnvironment->addSpinBox(L"0.0", core::rect<s32>(left+3*rowWithCol,mEditNextTop,left+withCol+3*rowWithCol,mEditNextTop+height), true, parent, SPIN_NODE_SPC_A);
                sb->setStepSize(colStepsSize);
                mEditNextTop += height +5;
            }
        }
    }
    newType = mNodeType;

    if ( mNodeActiveModel )
    {
        mNodeActiveModel->remove();
        mNodeActiveModel = NULL;
    }

    if ( modelSelected )
    {
        mNodeActiveModel = APP.GetNodeManager()->LoadNode(modelSelected->mName);
    }

    if ( nodeSelected )
    {
        if ( nodeSelected->IsOfClassType(NT_UNKNOWN) )
        {
            UpdateUnknown(nodeSelected);
        }
        if ( nodeSelected->IsOfClassType(NT_MESH) )
        {
            UpdateMesh(static_cast<NodeSettingsMesh*>(nodeSelected));
        }
        if ( nodeSelected->IsOfClassType(NT_ANIMATED_MESH) )
        {
            UpdateAnimatedMesh(static_cast<NodeSettingsAnimatedMesh*>(nodeSelected));
        }
        if ( nodeSelected->IsOfClassType(NT_LIGHT) )
        {
            UpdateLight(static_cast<NodeSettingsLight*>(nodeSelected));
        }
    }
}

void GuiNodeManager::UpdateUnknown(NodeSettingsUnknown * node_)
{
    if ( !node_ )
        return;

    IGUIElement* parent = mWndNodeManager->getElementFromId(STATIC_NODE_EDIT, false);
    if ( !parent  )
        return;

    IGUIEditBox * editName = static_cast<IGUIEditBox*>(parent->getElementFromId(EDIT_NODE_NAME, false));
    if ( !editName )
        return;

    std::wstring wstr( node_->mName.begin(), node_->mName.end() );
    editName->setText(wstr.c_str());
}

void GuiNodeManager::UpdateMesh(NodeSettingsMesh * node_)
{
    if ( !node_ )
        return;

    IGUIElement* parent = mWndNodeManager->getElementFromId(STATIC_NODE_EDIT, false);
    if ( !parent  )
        return;

    IGUIStaticText * staticFile = static_cast<IGUIStaticText*>(parent->getElementFromId(STATIC_NODE_FILE, false));
    if ( staticFile )
    {
        std::wstring wstr(node_->mFile.begin(), node_->mFile.end());
        staticFile->setText(wstr.c_str());
    }

    IGUISpinBox * spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_POS_X, false));
    if (!spin )
        return;
    spin->setValue(node_->mCenter.X);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_POS_Y, false));
    if (!spin )
        return;
    spin->setValue(node_->mCenter.Y);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_POS_Z, false));
    if (!spin )
        return;
    spin->setValue(node_->mCenter.Z);

    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_ROT_X, false));
    if (!spin )
        return;
    spin->setValue(node_->mRotation.X);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_ROT_Y, false));
    if (!spin )
        return;
    spin->setValue(node_->mRotation.Y);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_ROT_Z, false));
    if (!spin )
        return;
    spin->setValue(node_->mRotation.Z);

    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_SCALE_X, false));
    if (!spin )
        return;
    spin->setValue(node_->mScale.X);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_SCALE_Y, false));
    if (!spin )
        return;
    spin->setValue(node_->mScale.Y);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_SCALE_Z, false));
    if (!spin )
        return;
    spin->setValue(node_->mScale.Z);

    IGUICheckBox* cb = static_cast<IGUICheckBox*>(parent->getElementFromId(CHECKBOX_NODE_BF_CULLING, false));
    if ( !cb )
        return;
   cb->setChecked(node_->mBackfaceCulling);
}

void GuiNodeManager::UpdateAnimatedMesh(NodeSettingsAnimatedMesh * node_)
{
    if ( !node_ )
        return;

    IGUIElement* parent = mWndNodeManager->getElementFromId(STATIC_NODE_EDIT, false);
    if ( !parent  )
        return;

    IGUISpinBox * spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_ANIM_SPEED, false));
    if (!spin )
        return;
    spin->setValue(node_->mAnimationSpeed);
}

void GuiNodeManager::UpdateLight(NodeSettingsLight * node_)
{
    if ( !node_ )
        return;

    IGUIElement* parent = mWndNodeManager->getElementFromId(STATIC_NODE_EDIT, false);
    if ( !parent  )
        return;

    IGUISpinBox * spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_POS_X, false));
    if (!spin )
        return;
    spin->setValue(node_->mCenter.X);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_POS_Y, false));
    if (!spin )
        return;
    spin->setValue(node_->mCenter.Y);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_POS_Z, false));
    if (!spin )
        return;
    spin->setValue(node_->mCenter.Z);

    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_ROT_X, false));
    if (!spin )
        return;
    spin->setValue(node_->mRotation.X);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_ROT_Y, false));
    if (!spin )
        return;
    spin->setValue(node_->mRotation.Y);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_ROT_Z, false));
    if (!spin )
        return;
    spin->setValue(node_->mRotation.Z);


    IGUICheckBox* cb = static_cast<IGUICheckBox*>(parent->getElementFromId(CHECKBOX_NODE_CAST_SHADOWS, false));
    if ( !cb )
        return;
    cb->setChecked(node_->mCastShadows);

    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_RADIUS, false));
    if (!spin )
        return;
    spin->setValue(node_->mRadius);

    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_DIF_R, false));
    if (!spin )
        return;
    spin->setValue(node_->mDiffuseColor.r);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_DIF_G, false));
    if (!spin )
        return;
    spin->setValue(node_->mDiffuseColor.g);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_DIF_B, false));
    if (!spin )
        return;
    spin->setValue(node_->mDiffuseColor.b);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_DIF_A, false));
    if (!spin )
        return;
    spin->setValue(node_->mDiffuseColor.a);

    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_SPC_R, false));
    if (!spin )
        return;
    spin->setValue(node_->mSpecularColor.r);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_SPC_G, false));
    if (!spin )
        return;
    spin->setValue(node_->mSpecularColor.g);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_SPC_B, false));
    if (!spin )
        return;
    spin->setValue(node_->mSpecularColor.b);
    spin = static_cast<IGUISpinBox*>(parent->getElementFromId(SPIN_NODE_SPC_A, false));
    if (!spin )
        return;
    spin->setValue(node_->mSpecularColor.a);
}
