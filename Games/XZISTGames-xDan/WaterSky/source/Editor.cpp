
#include "Editor.h"
#include <time.h>

Editor::Editor(IrrlichtDevice *device)
    : WorldBase(device)
{
    editing = false;
    accurateObjectPositioning = false;
    multipleObjectSelect = false;

    player = NULL;
    thirdPersonCamera = new ThirdPersonCamera(this);
}

void Editor::initEditor()
{
    // Window caption
//    device->setWindowCaption( xTranslation->getChildNode(L"window").getAttribute(L"caption") );

    // Game Editor language
    XMLNode xEditor = xTranslation->getChildNode(L"editor");
    XMLNode xObjectNames = xEditor.getChildNode(L"objectNames");

    // NOTE: These must be in the same order as OBJECT_TYPE_STRINGS in Object.cpp
    objectNames.push_back( xObjectNames.getChildNode(OBJECT_TYPE_STRINGS[OBJECT_TYPE_PLATFORM]).getAttribute(L"name") );
    objectNames.push_back( xObjectNames.getChildNode(OBJECT_TYPE_STRINGS[OBJECT_TYPE_SCENERY]).getAttribute(L"name") );
    objectNames.push_back( xObjectNames.getChildNode(OBJECT_TYPE_STRINGS[OBJECT_TYPE_GENERATOR]).getAttribute(L"name") );
    objectNames.push_back( xObjectNames.getChildNode(OBJECT_TYPE_STRINGS[OBJECT_TYPE_FUELCAN]).getAttribute(L"name") );
    objectNames.push_back( xObjectNames.getChildNode(OBJECT_TYPE_STRINGS[OBJECT_TYPE_LIFT]).getAttribute(L"name") );
    objectNames.push_back( xObjectNames.getChildNode(OBJECT_TYPE_STRINGS[OBJECT_TYPE_BLOCK]).getAttribute(L"name") );
    objectNames.push_back( xObjectNames.getChildNode(OBJECT_TYPE_STRINGS[OBJECT_TYPE_CREATURE]).getAttribute(L"name") );

    // Load world...
    loadWorld(L"game");
}

void Editor::startEditor()
{
    editing = true;

    // Add new camera
    oldCamera = smgr->getActiveCamera();
    SKeyMap keyMap[8];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;
    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;
    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;
    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;
    editCamera = smgr->addCameraSceneNodeFPS(0, 100.0,20.0,-1,keyMap,8);
    editCamera->setPosition(oldCamera->getPosition());
    editCamera->setTarget(oldCamera->getTarget());
    editCamera->setInputReceiverEnabled(false);

    // Create editor GUI
    guienv->loadGUI("editor.xml");

    // Hide window close buttons (using own one)
    getWindowFromID(WINDOW_WORLD_EDITOR)->getCloseButton()->setVisible(false);
    getWindowFromID(WINDOW_ADD_OBJECT)->getCloseButton()->setVisible(false);
    getWindowFromID(WINDOW_OBJECT_SETTINGS)->getCloseButton()->setVisible(false);
    getWindowFromID(WINDOW_CREATE_PLATFORM)->getCloseButton()->setVisible(false);
    getWindowFromID(WINDOW_CREATE_SCENERY)->getCloseButton()->setVisible(false);
    getWindowFromID(WINDOW_WORLD_NAME)->getCloseButton()->setVisible(false);

    // Hide sub windows to start with
    getWindowFromID(WINDOW_OBJECT_SETTINGS)->setVisible(false);
    getWindowFromID(WINDOW_ADD_OBJECT)->setVisible(false);
    getWindowFromID(WINDOW_CREATE_PLATFORM)->setVisible(false);
    getWindowFromID(WINDOW_CREATE_SCENERY)->setVisible(false);
    getWindowFromID(WINDOW_WORLD_NAME)->setVisible(false);

    // Populate Add Object list
    objDefListBox = (gui::IGUIListBox *)guienv->getRootGUIElement()->
        getElementFromId(LISTBOX_DEFINED_OBJECTS, true);
    for (u32 i = 0; i < objectNames.size(); i ++)
    {
        objDefListBox->addItem(objectNames[i].c_str());
    }

    curcon->setVisible(true);
}

void Editor::stopEditor()
{
    deselectObjects();

    editing = false;
    guienv->clear();
    curcon->setVisible(false);
    curcon->setPosition(0.5f,0.5f);

    // Restore old camera
    smgr->setActiveCamera(oldCamera);
}

bool Editor::editorOnEvent(SEvent e)
{

    if (e.EventType == EET_KEY_INPUT_EVENT)
    {
        if (e.KeyInput.Key == KEY_SPACE)
        {
            accurateObjectPositioning = e.KeyInput.PressedDown;
        }
        if (e.KeyInput.Key == KEY_SHIFT)
        {
            multipleObjectSelect = e.KeyInput.PressedDown;
        }
    }

    if (e.EventType == EET_KEY_INPUT_EVENT && e.KeyInput.PressedDown)
    {
        switch (e.KeyInput.Key)
        {
        case KEY_ESCAPE:
            //stopEditor();
            return true;
        default:
            break;
        }

        // Can move object with: WSAD,RF
        // As long as no GUI element has focus and camera is not using input.
        if (selectedObjects.size()
            //&& !guienv->getFocus() Remove this unless needed??
            && !editCamera->isInputReceiverEnabled())
        {
            f32 angle = (editCamera->getTarget() - editCamera->getPosition()).getHorizontalAngle().Y*0.017453;

            core::vector3df moveVec;

            switch (e.KeyInput.Key)
            {
            case KEY_KEY_W:
                moveVec.X += sin(angle);
                moveVec.Z += cos(angle);
                break;
            case KEY_KEY_S:
                moveVec.X -= sin(angle);
                moveVec.Z -= cos(angle);
                break;
            case KEY_KEY_A:
                moveVec.X -= cos(angle);
                moveVec.Z += sin(angle);
                break;
            case KEY_KEY_D:
                moveVec.X += cos(angle);
                moveVec.Z -= sin(angle);
                break;
            case KEY_KEY_R:
                moveVec.Y += 1.0;
                break;
            case KEY_KEY_F:
                moveVec.Y -= 1.0;
                break;
            default:
                break;
            }

            if (moveVec.getLength() > 0.0)
            {
                moveVec.normalize();
                moveVec *= 0.5;

                // smaller movements when a certain key is being held down
                if (accurateObjectPositioning) moveVec *= 0.05;

                // But which object position is being modified?
                // (actual position, or one of its control points?)
                s32 point = ((gui::IGUIComboBox *)guienv->getRootGUIElement()
                    ->getElementFromId(COMBOBOX_ACTIVE_POINT, true))
                    ->getSelected();

                if (point <= 0) // Main object position
                {
                    // Move ALL selected objects
                    for (u32 i = 0; i < selectedObjects.size(); i ++)
                    {
                        selectedObjects[i]->setPosition(
                            selectedObjects[i]->getPosition() + moveVec
                            );
                    }
                }
                else // Control point
                {
                    // point-1 as zero was used for "main" position.
                    selectedObjects[0]->getPoint(point-1).pos += moveVec;
                }
            }
        }

    }

    if (e.EventType == EET_GUI_EVENT)
    {

        s32 id = e.GUIEvent.Caller->getID();

        switch (e.GUIEvent.EventType)
        {
        case gui::EGET_BUTTON_CLICKED:
            switch (id)
            {
            // World Editor buttons
            case BUTTON_ADD_OBJECT:
                deselectObjects();
                getWindowFromID(WINDOW_ADD_OBJECT)->setVisible(true);
                return true;
            case BUTTON_RETURN_TO_GAME:
                stopEditor();
                return true;
            case BUTTON_NEW:
                // create input box to modify worldName (worlds are like levels...)
                worldOperation = WORLD_OPERATION_NEW;
                guienv->getRootGUIElement()->getElementFromId(EDITBOX_WORLD_NAME, true)->setText(L"");
                getWindowFromID(WINDOW_WORLD_NAME)->setVisible(true);
                return true;
            case BUTTON_LOAD:
                deselectObjects();
                worldOperation = WORLD_OPERATION_LOAD;
                guienv->getRootGUIElement()->getElementFromId(EDITBOX_WORLD_NAME, true)->setText(worldName.c_str());
                getWindowFromID(WINDOW_WORLD_NAME)->setVisible(true);
                return true;
            case BUTTON_SAVE:
                worldOperation = WORLD_OPERATION_SAVE;
                guienv->getRootGUIElement()->getElementFromId(EDITBOX_WORLD_NAME, true)->setText(worldName.c_str());
                getWindowFromID(WINDOW_WORLD_NAME)->setVisible(true);
                return true;

            // World Name buttons

            case BUTTON_WORLD_NAME_CANCEL:
                getWindowFromID(WINDOW_WORLD_NAME)->setVisible(false);
                return true;

            case BUTTON_WORLD_NAME_OK:
            {
                getWindowFromID(WINDOW_WORLD_NAME)->setVisible(false);

                core::stringw chosenName = guienv->getRootGUIElement()
                    ->getElementFromId(EDITBOX_WORLD_NAME, true)->getText();

                if (!chosenName.size())
                {
                    getWindowFromID(WINDOW_WORLD_NAME)->setVisible(true);
                    errorMsg(L"noFilenameSpecified");
                    return false;
                }

                switch (worldOperation)
                {
                case WORLD_OPERATION_NEW:
                    if (!filesys->existFile( core::stringc( (worldFilePath+chosenName+L".world").c_str()).c_str() ))
                    {
                        saveWorld(worldFilePathTemp, worldName+L" "+getTime()); // Backup
                        worldName = chosenName;
                        clearObjects();
                        createPlayer();
                    }
                    else
                    {
                        getWindowFromID(WINDOW_WORLD_NAME)->setVisible(true);
                        errorMsg(L"worldAlreadyExists");
                    }
                    return true;
                case WORLD_OPERATION_LOAD:
                    if (filesys->existFile( core::stringc( (worldFilePath+chosenName+L".world").c_str()).c_str() ))
                    {
                        saveWorld(worldFilePathTemp, worldName+L" "+getTime()); // Backup
                        loadWorld( chosenName);
                    }
                    else
                    {
                        getWindowFromID(WINDOW_WORLD_NAME)->setVisible(true);
                        errorMsg(L"worldNotExist");
                    }
                    return true;
                case WORLD_OPERATION_SAVE:
                    saveWorld(worldFilePath, chosenName);
                    return true;
                }

                return true;
            }

            // Add Object buttons
            case BUTTON_ADD_OBJECT_CLOSE:
                getWindowFromID(WINDOW_ADD_OBJECT)->setVisible(false);
                return true;
            case BUTTON_ADD_OBJECT_OK:
                // Add a new Object?
                if (objDefListBox->getSelected() != -1)
                {
                    createObject(objDefListBox->getSelected(), (void *)NULL);
                }
                else
                {
                    errorMsg(L"noObjectSelected");
                }
                getWindowFromID(WINDOW_ADD_OBJECT)->setVisible(false);
                return true;

            // Platform create options
            case BUTTON_CREATE_PLATFORM_CANCEL:
                getWindowFromID(WINDOW_CREATE_PLATFORM)->setVisible(false);
                return true;

            case BUTTON_CREATE_PLATFORM_OK:
                createObject(OBJECT_TYPE_PLATFORM,
                    (void *)guienv->getRootGUIElement()->getElementFromId(EDITBOX_PLATFORM_MESHNAME, true)->getText()
                    );
                getWindowFromID(WINDOW_CREATE_PLATFORM)->setVisible(false);
                return true;

            // Scenery create options
            case BUTTON_CREATE_SCENERY_CANCEL:
                getWindowFromID(WINDOW_CREATE_SCENERY)->setVisible(false);
                return true;

            case BUTTON_CREATE_SCENERY_OK:
                createObject(OBJECT_TYPE_SCENERY,
                    (void *)guienv->getRootGUIElement()->getElementFromId(EDITBOX_SCENERY_MESHNAME, true)->getText()
                    );
                getWindowFromID(WINDOW_CREATE_SCENERY)->setVisible(false);
                return true;

            // Object Settings buttons
            case BUTTON_DELETE_OBJECT:
                // Remove first selected and deselect all others.
                delete deselectObjects();
                return true;
            case BUTTON_CLONE_OBJECT:
            {
                Object *newObj = selectedObjects[0]->clone();
                newObj->setRotation( newObj->getRotation() + core::vector3df(0,90,0) );
                selectObject(newObj);
                return true;
            }
            case BUTTON_SET_ACTIVATED_BY:
                // Set first selected object to be activated by all other selected objects.
                for (u32 i = 1; i < selectedObjects.size(); i ++)
                {
                    selectedObjects[0]->activatedBy(selectedObjects[i]);
                }
                return true;
            case BUTTON_CLEAR_ACTIVATED_BY:
                selectedObjects[0]->clearActivatedBy();
                return true;
            case BUTTON_NEW_POINT:
                {
                    LocRot lr;
                    lr.pos = chooseNewObjectPosition();
                    selectedObjects[0]->addPoint(lr);
                    fillObjectSettings();
                    return true;
                }
            }
            break;

        case gui::EGET_SCROLL_BAR_CHANGED:
            switch (id)
            {
            case SCROLLBAR_ROTATION_YAW:
                s32 point = ((gui::IGUIComboBox *)guienv->getRootGUIElement()
                    ->getElementFromId(COMBOBOX_ACTIVE_POINT, true))
                    ->getSelected();

                core::vector3df rot = core::vector3df(
                    0,
                    ((gui::IGUIScrollBar *)e.GUIEvent.Caller)->getPos(),
                    0);

                if (point <= 0)
                {
                    selectedObjects[0]->setRotation(rot);
                }
                else
                {
                    selectedObjects[0]->getPoint(point-1).rot = rot;
                }

                return true;
            }
            break;

        case gui::EGET_COMBO_BOX_CHANGED:
            switch (id)
            {
            case COMBOBOX_ACTIVE_POINT:
                // (to set yaw angle of control point)
                updateObjectSettings();
                break;
            }
            break;

        case gui::EGET_CHECKBOX_CHANGED:
            switch (id)
            {
            case CHECKBOX_ACTIVATED:
                selectedObjects[0]->setActive(
                    ((gui::IGUICheckBox *)e.GUIEvent.Caller)->isChecked()
                    );
                return true;
            }
            break;

        default:
            break;
        }
    }

    if (e.EventType == EET_MOUSE_INPUT_EVENT)
    {
        switch (e.MouseInput.Event)
        {
        case EMIE_LMOUSE_PRESSED_DOWN:
            if ( // If a GUI element was not clicked on (excluding the root element)
                guienv->getRootGUIElement()->getElementFromPoint(curcon->getPosition())
                == guienv->getRootGUIElement()
                )
            {
                // Check for object selection
                objectSelectByMouse();
                // Hide any windows
                getWindowFromID(WINDOW_ADD_OBJECT)->setVisible(false);
                getWindowFromID(WINDOW_CREATE_PLATFORM)->setVisible(false);
                getWindowFromID(WINDOW_CREATE_SCENERY)->setVisible(false);
                getWindowFromID(WINDOW_WORLD_NAME)->setVisible(false);

                // returning true prevents GUI from acting on this click
                // (i.e. so a button in object settings is not pressed accidentally)
                return true;
            }
            return false;
        case EMIE_RMOUSE_PRESSED_DOWN:
            editCamera->setInputReceiverEnabled(true);
            return true;
        case EMIE_RMOUSE_LEFT_UP:
            editCamera->setInputReceiverEnabled(false);
            return true;
        default:
            break;
        }
    }

    return false;
}

void Editor::renderRelations()
{
    Object *selectedObject = NULL;
    s32 point = -1;

    if (selectedObjects.size())
    {
        selectedObject = selectedObjects[0];

        point = ((gui::IGUIComboBox *)guienv->getRootGUIElement()
            ->getElementFromId(COMBOBOX_ACTIVE_POINT, true))
            ->getSelected();
    }

    for (u32 i = 0; i < objects->size(); i ++)
    {
        (*objects)[i]->renderRelations(
            selectedObject && (*objects)[i] == selectedObject, // is it selected?
            point // number of point which is selected... (-1 if none)
            );
    }
}

void Editor::saveWorld(core::stringw path, core::stringw filename)
{
    worldName = filename;
    filename = path + filename + L".world";

    XMLNode xWorld;
    saveWorld(xWorld);

    XMLError err = xWorld.writeToFile(filename.c_str(), NULL, 1);

    if (err != eXMLErrorNone)
    {
        // Error!
    }
}

void Editor::saveWorld(XMLNode &xWorld)
{
    xWorld = XMLNode::createXMLTopNode(L"world");

    XMLNode xObjectList = xWorld.addChild(L"objectList");

    for (u32 i = 0; i < objects->size(); i ++)
    {
        XMLNode xObject = xObjectList.addChild(L"object");
        (*objects)[i]->serializeToXML(xObject);
    }
}

void Editor::loadWorld(core::stringw filename)
{
    // open XML
    // for objects, get type ID from type name (index into TYPE_NAMES is the ID)
    // then createObject(type, xObject)

    worldName = filename;
    filename = worldFilePath + filename + L".world";

    XMLNode xWorld = XMLNode::openFileHelper(filename.c_str(), L"world");

    loadWorld(xWorld);
}

void Editor::loadWorld(XMLNode &xWorld)
{
    clearObjects();

    XMLNode xObjectList = xWorld.getChildNode(L"objectList");

    int objectListSize = xObjectList.nChildNode(L"object");

    for (int i = 0; i < objectListSize; i ++)
    {
        XMLNode xObject = xObjectList.getChildNode(L"object", i);

        createObject( OBJECT_TYPE_FROM_STRING(xObject.getAttribute(L"type", 0)) , &xObject);
    }

    if (!player)
    {
        printf("No player, so default one created.\n");
        createPlayer();
    }
}

void Editor::clearObjects()
{
    // objects remove themselves from the list on destruction... :/
    while (objects->size() > 0)
    {
        delete (*objects)[0];
    }
    player = NULL;
}

void Editor::createPlayer()
{
    if (player) delete player;

    player = new Player(this);
    thirdPersonCamera->setup(player->getNode());
}

void Editor::createObject(s32 type, XMLNode *xObject)
{
    Object *newObj = NULL;
    switch (type)
    {
    case OBJECT_TYPE_PLATFORM:
        newObj = new Platform(this);
        break;
    case OBJECT_TYPE_SCENERY:
        newObj = new Scenery(this);
        break;
    case OBJECT_TYPE_GENERATOR:
        newObj = new Generator(this);
        break;
    case OBJECT_TYPE_FUELCAN:
        newObj = new FuelCan(this);
        break;
    case OBJECT_TYPE_LIFT:
        newObj = new Lift(this);
        break;
    case OBJECT_TYPE_BLOCK:
        newObj = new Block(this);
        break;
    case OBJECT_TYPE_CREATURE:
        newObj = new Creature(this);
        break;
    case OBJECT_TYPE_PLAYER:
        createPlayer();
        newObj = player;
        break;
    default:
        printf("Error: createObject (XML): Invalid Object %i!\n", type);
        return;
    }

    if (newObj) newObj->deserializeFromXML(*xObject);
}

// extraData is set when an object needs more than default data... e.g. Platform needs a mesh filename.
// If createObject is called without extraData set and object needs more data
// then window to collect that data is displayed. That window may then call createObject later supplying
// the correct extraData
void Editor::createObject(s32 type, void *extraData)
{
    core::vector3df pos = chooseNewObjectPosition();

    // If created in editor...
    switch (type)
    {

    case OBJECT_TYPE_PLATFORM:
        if (extraData)
        {
            if (filesys->existFile( core::stringc((wchar_t *)extraData).c_str() ))
            {
                new Platform(this, (wchar_t *)extraData, pos, core::vector3df(0,0,0));
            }
            else
            {
                errorMsg(L"invalidMesh");
                return;
            }
        }
        else
        {
            getWindowFromID(WINDOW_CREATE_PLATFORM)->setVisible(true);
            return;
        }
        break;

    case OBJECT_TYPE_SCENERY:
        if (extraData)
        {
            if (filesys->existFile( core::stringc((wchar_t *)extraData).c_str() ))
            {
                new Scenery(this, (wchar_t *)extraData, pos, core::vector3df(0,0,0));
            }
            else
            {
                errorMsg(L"invalidMesh");
                return;
            }
        }
        else
        {
            getWindowFromID(WINDOW_CREATE_SCENERY)->setVisible(true);
            return;
        }
        break;

    case OBJECT_TYPE_GENERATOR:
        new Generator(this,
            pos,
            core::vector3df(0,0,0));
        break;

    case OBJECT_TYPE_FUELCAN:
        new FuelCan(this, pos);
        break;

    case OBJECT_TYPE_LIFT:
    {
        if (extraData) // is two LocRots, start and end.
        {
            LocRot *points = (LocRot *)extraData;
            new Lift(this, points[0], points[1]);
        }
        else
        {
            LocRot start,end;
            start.pos = pos;
            end.pos = start.pos;
            end.pos.Y += 12.0;
            new Lift(this, start, end);
        }
        break;
    }

    case OBJECT_TYPE_BLOCK:
        new Block(this, pos);
        break;

    case OBJECT_TYPE_CREATURE:
        new Creature(this, pos);
        break;

    default:
        // No object could be created, so return
        printf("Error: createObject: Invalid Object! %i\n", type);
        return;
    }

    // Select newly added object
    selectObject( (*objects)[objects->size() - 1] );
}

core::vector3df Editor::chooseNewObjectPosition()
{
    core::vector3df pos = editCamera->getPosition() - editCamera->getTarget().normalize()*4.0;

    pos.X += rand()%100 / 100.0;
    pos.Y += rand()%100 / 100.0;
    pos.Z += rand()%100 / 100.0;

    return pos;
}

core::stringw Editor::getTime()
{
    time_t rawtime;
    time(&rawtime);
    tm *theTime = localtime(&rawtime);

    char tstr[256];
    strftime(tstr,256, "%d-%m-%Y %H.%M.%S", theTime);

    return core::stringw(tstr);
}

void Editor::errorMsg(wchar_t *msg)
{
    guienv->addMessageBox(
        xTranslation->getChildNode(L"editor").getChildNode(L"errorMessage").getAttribute(L"caption"),
        xTranslation->getChildNode(L"editor").getChildNode(L"errorMessage").getChildNode(msg)
            .getAttribute(L"text")
        );
}

gui::IGUIWindow *Editor::getWindowFromID(int id)
{
    gui::IGUIWindow *e = (gui::IGUIWindow *)guienv->getRootGUIElement()->
        getElementFromId(id, true);
    assert(e);
    return e;
}

Object *Editor::getObjectByNode(scene::ISceneNode *sn)
{
    for (u32 i = 0; i < objects->size(); i ++)
    {
        if ( (*objects)[i]->getNode() == sn )
        {
            return (*objects)[i];
        }
    }
    return NULL;
}

void Editor::objectSelectByMouse()
{
    core::line3df ray = smgr->getSceneCollisionManager()->
        getRayFromScreenCoordinates(curcon->getPosition(), editCamera);

    // Problems may be caused here by non-object scene nodes.
    // e.g. Light has a large radius, and will prevent objects behind it
    // from being selected.
    // But ignore for now...
    selectObject(
        getObjectByNode(smgr->getSceneCollisionManager()->getSceneNodeFromRayBB(ray))
        );
}

Object *Editor::selectObject(Object *obj)
{
    // Check not already selected
    for (u32 i = 0; i < selectedObjects.size(); i ++)
    {
        if (selectedObjects[i] == obj) return NULL;
    }

    if (obj)
    {
        // If is not selecting multiple objects, then deselect all previously selected.
        if (!multipleObjectSelect) deselectObjects();
        // Add object to selected list.
        selectedObjects.push_back(obj);
        obj->getNode()->setDebugDataVisible(scene::EDS_BBOX);
        // Set object settings window visible
        getWindowFromID(WINDOW_OBJECT_SETTINGS)->setVisible(selectedObjects.size());
        // and if this is the first object, then update the settings window...
        if (selectedObjects.size() == 1) fillObjectSettings();
    }
    else // if none selected (e.g. empty space clicked on), deselect all
    {
        deselectObjects();
    }

    return obj; // Return newly selected object (NOT first selected)
}

Object *Editor::deselectObjects()
{
    Object *temp = NULL;
    if (selectedObjects.size())
    {
        temp = selectedObjects[0];
        // Deselect all
        for (u32 i = 0; i < selectedObjects.size(); i ++)
        {
            selectedObjects[i]->getNode()->setDebugDataVisible(scene::EDS_OFF);
        }
        selectedObjects.clear();
        // Hide object settings window
        getWindowFromID(WINDOW_OBJECT_SETTINGS)->setVisible(false);
    }
    return temp; // Returns (previously) first selected object, e.g. for deletion
}

void Editor::fillObjectSettings()
{

    // Fill active position list
    gui::IGUIComboBox *box = (gui::IGUIComboBox *)guienv->getRootGUIElement()
        ->getElementFromId(COMBOBOX_ACTIVE_POINT, true);
    box->clear();
    box->addItem( xTranslation->getChildNode(L"editor").getChildNode(L"words").getChildNode(L"object")
        .getAttribute(L"word") );
    for (u32 i = 0; i < selectedObjects[0]->getPointCount(); i ++)
    {
        core::stringw str = xTranslation->getChildNode(L"editor").getChildNode(L"words").getChildNode(L"extra")
            .getAttribute(L"word");
        str += L" ";
        str += i;
        box->addItem(str.c_str());
    }

    updateObjectSettings();
}

void Editor::updateObjectSettings()
{
    // Add points button visible?
    guienv->getRootGUIElement()->getElementFromId(BUTTON_NEW_POINT, true)
        ->setVisible( !selectedObjects[0]->isLimitedPoints() );

    // Yaw rotation scrollbar
    s32 point = ((gui::IGUIComboBox *)guienv->getRootGUIElement()
        ->getElementFromId(COMBOBOX_ACTIVE_POINT, true))
        ->getSelected();
    int rot;
    if (point <= 0)
    {
        rot = (int)selectedObjects[0]->getRotation().Y;
    }
    else
    {
        rot = (int)selectedObjects[0]->getPoint(point-1).rot.Y;
    }
    ((gui::IGUIScrollBar *)guienv->getRootGUIElement()->getElementFromId(SCROLLBAR_ROTATION_YAW, true))
        ->setPos(rot);

    // Activated check box
    ((gui::IGUICheckBox *)guienv->getRootGUIElement()->getElementFromId(CHECKBOX_ACTIVATED, true))
        ->setChecked(selectedObjects[0]->isActive());
}

