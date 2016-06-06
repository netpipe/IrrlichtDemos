#include <irrlicht.h>
#include "CGUIProgressBar.h"
#include "CImageGUISkin.h"
#include "SkinLoader.h"

using namespace irr;


bool shouldQuit = false;

class demoEventsReceiever : public IEventReceiver
{
public:
	demoEventsReceiever() {};
	~demoEventsReceiever() {};
	
	virtual bool OnEvent(const SEvent& event)
	{
		if(event.EventType == irr::EET_GUI_EVENT)
		{
			//Gui is doing something here
			if(event.GUIEvent.EventType == gui::EGET_BUTTON_CLICKED)
			{
				//If its the button : 
				if(event.GUIEvent.Caller->getID() == 1000)
				{
					shouldQuit = true;
				}
			}
		}

		//So that other events can happen
		return false;
	}

};

f32 updateProgressBar(f32 curProgress, bool &increaseProgress, f32 deltaTime)
{
	f32 progress = curProgress;

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
        
		return progress;
}

int main()
{
    IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9);
    
    video::IVideoDriver* driver = device->getVideoDriver();
    scene::ISceneManager* manager = device->getSceneManager();
    io::IFileSystem* filesys = device->getFileSystem();
    gui::IGUIEnvironment* env = device->getGUIEnvironment();

	demoEventsReceiever evnts;

	device->setEventReceiver(&evnts);

    // Load GUI
    gui::SImageGUISkinConfig guicfg = LoadGUISkinFromFile(filesys, driver, "../ui/guiskin.cfg");
    gui::CImageGUISkin* skin = new gui::CImageGUISkin(driver, env->getSkin());
    skin->loadConfig(guicfg);

    gui::IGUIFont* font = env->getFont("../ui/fontlucida.png");

	if (font != 0)
	{
		skin->setFont(font, gui::EGDF_DEFAULT);
		skin->setFont(font, gui::EGDF_WINDOW);
	}

	s32 j = 0;
	video::SColor newCol = video::SColor();
	video::SColor oldCol = newCol;

	for(j = 0; j < gui::EGDC_COUNT; ++j)
	{
		oldCol = skin->getColor((gui::EGUI_DEFAULT_COLOR)j);
		
		newCol = oldCol;
		newCol.setAlpha(255);

		skin->setColor((gui::EGUI_DEFAULT_COLOR)j, newCol);
	}

	skin->setColor(gui::EGDC_ACTIVE_CAPTION, video::SColor(255,255,255,255));
	skin->setColor(gui::EGDC_BUTTON_TEXT, video::SColor(255,0,0,0));
	skin->setColor(gui::EGDC_HIGH_LIGHT_TEXT, video::SColor(255,255,255,255));
	skin->setColor(gui::EGDC_GRAY_TEXT, video::SColor(150,255,255,255));
	skin->setColor(gui::EGDC_INACTIVE_CAPTION, video::SColor(150,255,255,255));

	skin->setSize(gui::EGDS_WINDOW_BUTTON_WIDTH, 20);
	skin->setSize(gui::EGDS_TITLEBARTEXT_DISTANCE_X, 2);
	skin->setSize(gui::EGDS_TITLEBARTEXT_DISTANCE_Y, 9);

    env->setSkin(skin);

    skin->drop();

	env->addImage(driver->getTexture("../ui/bg.jpg"), core::vector2di(0,0));

	gui::IGUIWindow* win = env->addWindow(core::rect<s32>(10,10,520,400), false, L"Klaskers GUI (Irrlicht 1.6 (SVN)");
	
    env->addButton( core::rect<s32>(380,320,480,350), win, 1000, L"Exit Demo!" );
    env->addCheckBox(false, core::rect<s32>(40,50,250,70), win , -1, L"Item 1");
    env->addCheckBox(false, core::rect<s32>(40,80,250,100), win , -1, L"Item 2");
    env->addCheckBox(false, core::rect<s32>(40,110,250,130), win , -1, L"Item 3");
    env->addEditBox(L"This is an editbox OH YEA!", core::rect<s32>(40,160,250,190), true, win );
    gui::CGUIProgressBar* bar = new gui::CGUIProgressBar(env->getRootGUIElement(), env, core::rect<s32>(40,280,470,300));
	bar->setFillColor(video::SColor(255,255,255,255));
	bar->setEmptyColor(video::SColor(150,255,255,255));
	win->addChild(bar);
    bar->setProgress(0.35f);
    bar->drop();


    float progress = 0.0f;
    bool increaseProgress = true;

    s32 lastTime = device->getTimer()->getTime();
    
    while ( device->run() )
    {
        driver->beginScene(true, true, video::SColor(0,0,0,0));
        env->drawAll();
        driver->endScene();

        s32 newTime = device->getTimer()->getTime();
        s32 deltaTime = newTime - lastTime;
        lastTime = newTime;

		progress = updateProgressBar(progress, increaseProgress,deltaTime);
		bar->setProgress(progress);

		if(shouldQuit)
		{
			device->closeDevice();
		}

    }
    
    device->drop();
    
    return 0;
}

