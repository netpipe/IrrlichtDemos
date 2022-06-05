#include "CEditor.h"
#include "CMap.h"
#include "CTile.h"
#include "CStateManager.h"
#include "GameVariables.h"

CEditor::CEditor(CStateManager* manager) : CState(manager)
{
    Set = false;
    Info = NULL;

    //setup Camera
    irr::scene::ICameraSceneNode* camera = Manager->getDevice()->getSceneManager()->getActiveCamera();
    if (camera)
        camera->setPosition(irr::core::vector3df(game_width*tile_size/2.f, game_height*tile_size/2.f, -10));
    else
        camera = Manager->getDevice()->getSceneManager()->addCameraSceneNode(NULL, irr::core::vector3df(game_width*tile_size/2.f, game_height*tile_size/2.f,-10), irr::core::vector3df(game_width*tile_size/2.f, game_height*tile_size/2.f,0), -1);
    irr::core::matrix4 mat;
    mat.buildProjectionMatrixOrthoLH(game_width*tile_size, game_height*tile_size, 0, 20);
    camera->setProjectionMatrix(mat);

    Map.init(30, 20, manager->getDevice()->getSceneManager());


}

CEditor::~CEditor()
{
    printf("delete Editor\n");
}

void CEditor::update(irr::f32 timediff)
{

}

void CEditor::key_event(const irr::SEvent::SKeyInput& key)
{
    if (key.Key == irr::KEY_KEY_S)
        Type = ETT_STREET;
    else if (key.Key == irr::KEY_KEY_F)
        Type = ETT_FORBIDDEN;
    else if (key.Key == irr::KEY_KEY_B)
        Type = ETT_BUILD;
    else if (key.Key == irr::KEY_KEY_G)
        Type = ETT_GOAL;
    else if (key.Key == irr::KEY_KEY_M)
        Type = ETT_START;
    else if (key.Key == irr::KEY_KEY_O)
        Map.saveMap("maps/autosave.map", Manager);
    else if (key.Key == irr::KEY_KEY_P)
        Map.loadMap("maps/autosave.map", Manager);
    else if (key.Key == irr::KEY_ESCAPE)
    {
        Manager->createState("MainMenu");
        remove();
    }
}

void CEditor::mouse_event(const irr::SEvent::SMouseInput& mouse)
{
    if (mouse.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
    {
        Set = true;
    }
    else if (mouse.Event == irr::EMIE_LMOUSE_LEFT_UP)
    {
        Set = false;
    }
    if (mouse.Event == irr::EMIE_MOUSE_MOVED && Set)
    {
        int X = mouse.X/tile_size;
        int Y = (game_height*tile_size-mouse.Y)/tile_size;
        printf("Select %i - %i\n", X, Y);
        Map.setSelected(X, Y, Type);
    }
}

void CEditor::becomeMainState(void)
{
    printf("Make Editor MainState...");
    Manager->getInput()->registerInput((IMouseReceiver*)this);
    Manager->getInput()->registerInput((IKeyboardReceiver*)this);
    Info = Manager->getDevice()->getGUIEnvironment()->addStaticText(L"", irr::core::rect<irr::s32>(50,50,200,300), false, true);
    Info->setText(  L"Tiles:\n"
                    L"S - Street\n"
                    L"F - Grass(forbidden)\n"
                    L"B - Buildground\n"
                    L"G - Goal\n"
                    L"M - Start\n"
                    L"Save/Load Map:\n"
                    L"O - Save\n"
                    L"P - Load\n"
                    L"Misc:\n"
                    L"Escape - Leave Editor\n"
                    );
    printf("done\n");
}

void CEditor::becomeSlaveState(void)
{
    printf("Make Editor SlaveState...");
    Manager->getInput()->unregisterInput((IMouseReceiver*)this);
    Manager->getInput()->unregisterInput((IKeyboardReceiver*)this);
    if (Info)
        Info->remove();
    Info = NULL;
    printf("done\n");
}

