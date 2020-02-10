
#ifndef __Editor_h
#define __Editor_h

#include "Player.h"
#include "Lift.h"
#include "Platform.h"
#include "Scenery.h"
#include "FuelCan.h"
#include "Generator.h"
#include "Block.h"
#include "Creature.h"

#include "ThirdPersonCamera.h"

const core::stringw worldFilePath = L"media/worlds/";
const core::stringw worldFilePathTemp = L"media/worlds/temp/";

class Editor : public WorldBase
{
public:
    Editor(IrrlichtDevice *);
    
protected:
    bool editing;
    
    ThirdPersonCamera *thirdPersonCamera;
    Player *player;
    
    void initEditor();
    void startEditor();
    void stopEditor();
    bool editorOnEvent(SEvent);
    void renderRelations();
    
    void saveWorld(core::stringw, core::stringw);
    void saveWorld(XMLNode &);
    void loadWorld(core::stringw);
    void loadWorld(XMLNode &);
    void clearObjects();
    void createPlayer();
    
private:
    enum
    {
        BUTTON_ADD_OBJECT_OK = 100,
        LISTBOX_DEFINED_OBJECTS = 101,
        WINDOW_WORLD_EDITOR = 102,
        BUTTON_ADD_OBJECT = 103,
        BUTTON_RETURN_TO_GAME = 104,
        WINDOW_ADD_OBJECT = 105,
        BUTTON_ADD_OBJECT_CLOSE = 106,
        
        BUTTON_NEW = 120,
        BUTTON_LOAD = 121,
        BUTTON_SAVE = 122,
        
        // World name?
        WINDOW_WORLD_NAME = 125,
        EDITBOX_WORLD_NAME = 126,
        BUTTON_WORLD_NAME_CANCEL = 127,
        BUTTON_WORLD_NAME_OK = 128,
        
        // Object settings window
        WINDOW_OBJECT_SETTINGS = 107,
        SCROLLBAR_ROTATION_YAW = 110,
        CHECKBOX_ACTIVATED = 111,
        BUTTON_SET_ACTIVATED_BY = 112,
        BUTTON_DELETE_OBJECT = 113,
        BUTTON_CLONE_OBJECT = 114,
        BUTTON_CLEAR_ACTIVATED_BY = 108,
        COMBOBOX_ACTIVE_POINT = 109,
        BUTTON_NEW_POINT = 119,
        
        // Platform creator...
        WINDOW_CREATE_PLATFORM = 115,
        EDITBOX_PLATFORM_MESHNAME = 116,
        BUTTON_CREATE_PLATFORM_CANCEL = 117,
        BUTTON_CREATE_PLATFORM_OK = 118,
        
        // Scenery creator...
        WINDOW_CREATE_SCENERY = 129,
        EDITBOX_SCENERY_MESHNAME = 130,
        BUTTON_CREATE_SCENERY_CANCEL = 131,
        BUTTON_CREATE_SCENERY_OK = 132
    };
    
    enum WORLD_NAME_OPERATION
    {
        WORLD_OPERATION_NEW,
        WORLD_OPERATION_LOAD,
        WORLD_OPERATION_SAVE
    };
    
    core::stringw worldName;
    WORLD_NAME_OPERATION worldOperation;
    
    scene::ICameraSceneNode *oldCamera;
    scene::ICameraSceneNode *editCamera;
    gui::IGUIListBox *objDefListBox;
    
    // make smaller steps when moving an object
    bool accurateObjectPositioning;
    // Can select multiple objects...
    bool multipleObjectSelect;
    
    core::array <core::stringw> objectNames;
    void createObject(s32, void *);
    void createObject(s32, XMLNode *);
    core::vector3df chooseNewObjectPosition();
    
    // Displays an editor error message from language file.
    void errorMsg(wchar_t *);
    
    core::stringw getTime();
    
    gui::IGUIWindow *getWindowFromID(int);
    Object *getObjectByNode(scene::ISceneNode *);
    void objectSelectByMouse();
    Object *selectObject(Object *);
    Object *deselectObjects();
    
    // Fill settings window from first selected object
    void fillObjectSettings();
    // Any object settings that can be repeated (i.e. not clearing a list box as this would
    // reset the list box currently selected option)
    void updateObjectSettings();
    
    core::array <Object *> selectedObjects;
};

#endif
