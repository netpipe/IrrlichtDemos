#include <irrlicht.h>
#include "CGUIProgressBar.h"
#include "CImageGUISkin.h"
#include "SkinLoader.h"

using namespace irr;

int main()
{
    IrrlichtDevice* device = createDevice(video::EDT_OPENGL);

    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* manager = device->getSceneManager();
    io::IFileSystem* filesys = device->getFileSystem();
    gui::IGUIEnvironment* env = device->getGUIEnvironment();

    scene::ISceneNode* node = manager->addCubeSceneNode();
    scene::ICameraSceneNode* camera = manager->addCameraSceneNode();
    camera->setPosition(core::vector3df(45,45,15));
    camera->setTarget(core::vector3df(0,0,0));

    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setMaterialFlag(video::EMF_WIREFRAME, true);

    // Load GUI
    gui::SImageGUISkinConfig guicfg = LoadGUISkinFromFile(filesys, driver, "../ui/guiskin.cfg");
    gui::CImageGUISkin* skin = new gui::CImageGUISkin(driver, env->getSkin());
    skin->loadConfig(guicfg);

    gui::IGUIFont* font = env->getFont("ui/fontlucida.png");
    if (font != 0)
		skin->setFont(font, gui::EGDF_DEFAULT);

    env->setSkin(skin);

    skin->drop();

    gui::IGUIWindow* win = env->addWindow(core::rect<s32>(60,60,260,260), false, L"This is a window");
    env->addButton( core::rect<s32>(10,10,100,100), 0, -1, L"Button!" );
    env->addCheckBox(true, core::rect<s32>(10,30,30,50), win );
    env->addStaticText(L"Checkbox", core::rect<s32>(30, 30, 120, 50), false, false, win);
    env->addEditBox(L"Edit box", core::rect<s32>(120,120,400,150) );
    gui::CGUIProgressBar* bar = new gui::CGUIProgressBar(env->getRootGUIElement(), env, core::rect<s32>(10,150,210,180));
    bar->setProgress(0.35f);
    bar->drop();

    float progress = 0.0f;
    bool increaseProgress = true;

    s32 lastTime = device->getTimer()->getTime();

    while ( device->run() )
    {
        driver->beginScene(true, true, video::SColor(0,25,25,25));
        manager->drawAll();
        env->drawAll();
        driver->endScene();

        s32 newTime = device->getTimer()->getTime();
        s32 deltaTime = newTime - lastTime;
        lastTime = newTime;
        if (increaseProgress)
            progress += deltaTime * 0.0002f;
        else
            progress -= deltaTime * 0.0002f;
        if (progress > 1.0f)
        {
            progress = 1.0f;
            increaseProgress = false;
        }
        else
        if (progress < 0.0f)
        {
            progress = 0.0f;
            increaseProgress = true;
        }
        bar->setProgress(progress);
    }

    device->drop();

    return 0;
}

