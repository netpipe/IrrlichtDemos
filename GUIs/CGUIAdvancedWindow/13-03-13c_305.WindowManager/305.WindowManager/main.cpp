/** Example X05 Window Manager & Advanced Window

*/
#include <irrlicht.h>
//#include "driverChoice.h"
#include "CGUIWindowManager.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//#ifdef _IRR_WINDOWS_
//#pragma comment(lib, "Irrlicht.lib")
//#endif

struct SAppContext
{
	IrrlichtDevice *device;
	IVideoDriver* driver;
	s32				counter;
	IGUIListBox*	listbox;
	CGUIWindowManager* wm;             ///< ADDED
};

enum
{
	GUI_ID_QUIT_BUTTON = 101,
	GUI_ID_NEW_SMALL_WINDOW_BUTTON,     ///< ADDED
	GUI_ID_NEW_LARGE_WINDOW_BUTTON,     ///< ADDED
	GUI_ID_SHOW_JOINTS_BUTTON,          ///< ADDED
	GUI_ID_HIDE_JOINTS_BUTTON,          ///< ADDED
	GUI_ID_GRID_RESET,                  ///< ADDED
	GUI_ID_GRID_ATTACH_RUN_ONCE,        ///< ADDED
	GUI_ID_GRID_ATTACH_FOCUSED,         ///< ADDED
	GUI_ID_GRID_RUN,                    ///< ADDED
	GUI_ID_GRID_DETACH_FOCUSED,         ///< ADDED
	GUI_ID_GRID_DETACH_ALL,             ///< ADDED
	GUI_ID_WINDOW_SPECIAL_BUTTON,       ///< ADDED
	GUI_ID_WINDOW_MINIMIZE_ALL,         ///< ADDED
	GUI_ID_WINDOW_RESTORE_ALL           ///< ADDED
};

class MyEventReceiver : public IEventReceiver
{
public:
	MyEventReceiver(SAppContext & context) : Context(context) { }

	virtual bool OnEvent(const SEvent& event)
	{
		if (event.EventType == EET_GUI_EVENT)
		{
			s32 id = event.GUIEvent.Caller->getID();
			IGUIEnvironment* env = Context.device->getGUIEnvironment();

			switch(event.GUIEvent.EventType)
			{
			case EGET_BUTTON_CLICKED:
				switch(id)
				{
				case GUI_ID_QUIT_BUTTON:
					Context.device->closeDevice();
					return true;
				case GUI_ID_NEW_SMALL_WINDOW_BUTTON:
					{
                        Context.listbox->addItem(L"Small Window created");
                        Context.counter += 60;
                        if (Context.counter > 500)
                            Context.counter = 0;

                        /// First, we create a list of buttons for the new window
                        core::array<CGUIAdvancedWindow::buttoninfo> ButtonInfo;

                        /// Configure the CLOSE button
                        CGUIAdvancedWindow::buttoninfo CloseButton;
                        CloseButton.Type = EWBT_CLOSE;
                        CloseButton.VisibleWhenNormal = true;
                        CloseButton.VisibleWhenBar = true;
                        CloseButton.VisibleWhenMinimized = false;
                        ButtonInfo.push_back(CloseButton);

                        /// Configure the MINIMIZE button
                        CGUIAdvancedWindow::buttoninfo MinimizeButton;
                        MinimizeButton.Type = EWBT_MINIMIZE;
                        ButtonInfo.push_back(MinimizeButton);

                        /// Configure the PIN button
                        CGUIAdvancedWindow::buttoninfo PinButton;
                        PinButton.Type = EWBT_PIN;
                        PinButton.VisibleWhenNormal = true;
                        PinButton.VisibleWhenBar = true;
                        PinButton.VisibleWhenMinimized = false;
                        ButtonInfo.push_back(PinButton);

                        /// Configure a Uuser-defined button
                        CGUIAdvancedWindow::buttoninfo MySpecialButton;
                        MySpecialButton.Type = EWBT_USER_DEFINED; //!< it's a user-defined button
                        IGUISpriteBank* bank = env->getSpriteBank("SpecialButton"); //!< here is its image
                        if(!bank)
                        {
                            bank = env->addEmptySpriteBank("SpecialButton");
                            bank->addTextureAsSprite(Context.driver->getTexture("../../media/reload16.png"));
                            Context.driver->makeColorKeyTexture(Context.driver->getTexture("../../media/reload16.png"), position2di(0,0));
                        }
                        MySpecialButton.Sprite = bank;
                        MySpecialButton.SpriteIndex = 0;
                        MySpecialButton.VisibleWhenNormal = true;
                        MySpecialButton.VisibleWhenBar = false;
                        MySpecialButton.VisibleWhenMinimized = false;
                        MySpecialButton.Text = L"";
                        MySpecialButton.ToolTipText = L"My Special Button !";
                        MySpecialButton.UserEventId = GUI_ID_WINDOW_SPECIAL_BUTTON;
                        ButtonInfo.push_back(MySpecialButton);

                        /// Configure background texture of the window
                        video::ITexture* texture = Context.driver->getTexture("../../media/brushed-steel-texture.jpg");

                        /// Create the advanced window, using Window Manager API
                        CGUIAdvancedWindow* window = Context.wm->addWindow(
                            rect<s32>(150 + Context.counter, 100 + Context.counter, 350 + Context.counter, 200 + Context.counter),
                            L"New Small Advanced Window",
                            ButtonInfo,
                            irr::gui::EWC_HIDE,
                            texture );

                        /// Add control into the newly created window
                        env->addStaticText(L"Please close me",
                            rect<s32>(35,35,140,50),
                            true, // border?
                            false, // wordwrap?
                            window);
					}
					return true;
				case GUI_ID_NEW_LARGE_WINDOW_BUTTON:
					{
                        Context.listbox->addItem(L"Large Window created");
                        Context.counter += 60;
                        if (Context.counter > 500)
                            Context.counter = 0;

                        /// First, we create a list of buttons for the new window
                        core::array<CGUIAdvancedWindow::buttoninfo> ButtonInfo;

                        /// Configure the CLOSE button
                        CGUIAdvancedWindow::buttoninfo CloseButton;
                        CloseButton.Type = EWBT_CLOSE;
                        CloseButton.VisibleWhenNormal = true;
                        CloseButton.VisibleWhenBar = true;
                        CloseButton.VisibleWhenMinimized = false;
                        ButtonInfo.push_back(CloseButton);

                        /// Configure the MINIMIZE button
                        CGUIAdvancedWindow::buttoninfo MinimizeButton;
                        MinimizeButton.Type = EWBT_MINIMIZE;
                        ButtonInfo.push_back(MinimizeButton);

                        /// Configure the PIN button
                        CGUIAdvancedWindow::buttoninfo PinButton;
                        PinButton.Type = EWBT_PIN;
                        PinButton.VisibleWhenNormal = true;
                        PinButton.VisibleWhenBar = true;
                        PinButton.VisibleWhenMinimized = false;
                        ButtonInfo.push_back(PinButton);

                        /// Configure a Uuser-defined button
                        CGUIAdvancedWindow::buttoninfo MySpecialButton;
                        MySpecialButton.Type = EWBT_USER_DEFINED;
                        IGUISpriteBank* bank = env->getSpriteBank("SpecialButton");
                        if(!bank)
                        {
                            bank = env->addEmptySpriteBank("SpecialButton");
                            bank->addTextureAsSprite(Context.driver->getTexture("../../media/reload16.png"));
                            Context.driver->makeColorKeyTexture(Context.driver->getTexture("../../media/reload16.png"), position2di(0,0));
                        }
                        MySpecialButton.Sprite = bank;
                        MySpecialButton.SpriteIndex = 0;
                        MySpecialButton.VisibleWhenNormal = true;
                        MySpecialButton.VisibleWhenBar = false;
                        MySpecialButton.VisibleWhenMinimized = false;
                        MySpecialButton.Text = L"";
                        MySpecialButton.ToolTipText = L"My Special Button !";
                        MySpecialButton.UserEventId = GUI_ID_WINDOW_SPECIAL_BUTTON;
                        ButtonInfo.push_back(MySpecialButton);

                        /// Configure background texture of the window
                        video::ITexture* texture = Context.driver->getTexture("../../media/brushed-steel-texture.jpg");

                        /// Create the advanced window, using Window Manager API
                        CGUIAdvancedWindow* window = Context.wm->addWindow(
                            rect<s32>(150 + Context.counter, 100 + Context.counter, 550 + Context.counter, 300 + Context.counter),
                            L"New Large Advanced Window",
                            ButtonInfo,
                            irr::gui::EWC_HIDE,
                            texture );

                        /// Add control into the newly created window
                        env->addStaticText(L"Please close me",
                            rect<s32>(35,35,140,50),
                            true, // border?
                            false, // wordwrap?
                            window);
					}
					return true;
				case GUI_ID_SHOW_JOINTS_BUTTON:
					Context.wm->setJointVisible(true);
					return true;
				case GUI_ID_HIDE_JOINTS_BUTTON:
					Context.wm->setJointVisible(false);
					return true;
				case GUI_ID_WINDOW_MINIMIZE_ALL:
					Context.wm->minimizeAll();
					return true;
				case GUI_ID_WINDOW_RESTORE_ALL:
					Context.wm->restoreAll();
					return true;
				case GUI_ID_GRID_RESET:
				case GUI_ID_GRID_ATTACH_RUN_ONCE:
				case GUI_ID_GRID_ATTACH_FOCUSED:
				case GUI_ID_GRID_RUN:
				case GUI_ID_GRID_DETACH_FOCUSED:
				case GUI_ID_GRID_DETACH_ALL:
					Context.wm->setBehavior((EGUI_WINDOW_MANAGER_BEHAVIOR)(id-GUI_ID_GRID_RESET));
					return true;
                case GUI_ID_WINDOW_SPECIAL_BUTTON:
					Context.listbox->addItem(L"Click on Window SPECIAL BUTTON!");
					return true;
				default:
					return false;
				}
				break;
			default:
				break;
			}
		}
		return false;
	}
private:
	SAppContext & Context;
};

int main()
{
//	// ask user for driver
//	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
//	if (driverType==video::EDT_COUNT)
//		return 1;

	// create device and exit if creation failed

	IrrlichtDevice * device = createDevice(video::EDT_OPENGL , core::dimension2d<u32>(1280, 800));

	if (device == 0)
		return 1; // could not create selected driver.
	device->setWindowCaption(L"Irrlicht Engine - Window Manager Demo");
	device->setResizable(true);
	video::IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* env = device->getGUIEnvironment();

    /// Init Irrlicht SKIN
    {
        IGUISkin * skin = env->createSkin(EGST_WINDOWS_METALLIC);
        if(skin)
        {
            env->setSkin(skin);
            skin->drop();
        }
    }

	/// Adjust Font
	{
	    IGUIFont* default_font = env->getFont("../../media/Lucida_Console_Regular_8_AA_Alpha.xml");
        if(default_font)
        {
            env->getSkin()->setFont(default_font);
        }
	}

	/// Adjust Color
	{
        env->getSkin()->setColor(EGDC_3D_HIGH_LIGHT ,video::SColor(225,255,255,255));
        env->getSkin()->setColor(EGDC_3D_DARK_SHADOW ,video::SColor(220,100,100,100));
        env->getSkin()->setColor(EGDC_3D_LIGHT  ,video::SColor(220,255,255,255));
        env->getSkin()->setColor(EGDC_3D_SHADOW  ,video::SColor(220,255,255,255));
        env->getSkin()->setColor(EGDC_3D_FACE ,video::SColor(255,0,0,0));
        env->getSkin()->setColor(EGDC_ACTIVE_BORDER ,video::SColor(255,255,255,255));
        env->getSkin()->setColor(EGDC_INACTIVE_BORDER ,video::SColor(255,0,0,0));
        env->getSkin()->setColor(EGDC_ACTIVE_CAPTION,video::SColor(255,0,0,0));
        env->getSkin()->setColor(EGDC_INACTIVE_CAPTION,video::SColor(255,255,255,255));
        env->getSkin()->setColor(EGDC_APP_WORKSPACE,video::SColor(255,255,255,255));
        env->getSkin()->setColor(EGDC_WINDOW,video::SColor(255,255,255,255));
        env->getSkin()->setColor(EGDC_WINDOW_SYMBOL ,video::SColor(255,255,255,255));
        env->getSkin()->setColor(EGDC_SCROLLBAR ,video::SColor(20,255,255,255));
        env->getSkin()->setColor(EGDC_HIGH_LIGHT,video::SColor(20,255,255,255));
        env->getSkin()->setColor(EGDC_ICON_HIGH_LIGHT,video::SColor(255,255,255,255));
        env->getSkin()->setColor(EGDC_HIGH_LIGHT_TEXT,video::SColor(255,255,255,255));
        env->getSkin()->setColor(EGDC_BUTTON_TEXT  ,video::SColor(200,255,255,255));
        env->getSkin()->setColor(EGDC_ICON  ,video::SColor(255,180,180,180));
        env->getSkin()->setColor(EGDC_EDITABLE  ,video::SColor(20,255,255,255));
        env->getSkin()->setColor(EGDC_FOCUSED_EDITABLE  ,video::SColor(20,50,50,50));
	}

    /// Add a wm (first GUI root child  = background GUI = last called when event)
    CGUIWindowManager* wm =  new CGUIWindowManager( env, env->getRootGUIElement(), device->getCursorControl() );

	/// Adjust Advanced Window Color
	{
	    IGUISkin * skin = env->createSkin(EGST_WINDOWS_METALLIC); //EGST_WINDOWS_METALLIC EGST_WINDOWS_CLASSIC
	    IGUIFont* font = env->getFont("../../media/Lucida_Console_Regular_8_AA_Alpha.xml");
        if(font)
        {
            skin->setFont(font);
        }

        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_DARK_SHADOW ,video::SColor(80,0,0,0));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_SHADOW ,video::SColor(80,255,255,255));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_HIGH_LIGHT ,video::SColor(255,255,255,255));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_LIGHT ,video::SColor(255,0,0,0));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_ACTIVE_TITTLE_BAR ,video::SColor(255,255,255,255));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_INACTIVE_TITTLE_BAR ,video::SColor(20,40,40,40));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_ACTIVE_TITTLE_TEXT ,video::SColor(255,0,0,0));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_INACTIVE_TITTLE_TEXT ,video::SColor(200,255,255,255));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_WINDOW_LIGHT ,video::SColor(160,160,160,160));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_WINDOW_SHADOW ,video::SColor(80,0,0,0));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_ACTIVE_WINDOW_SYMBOL ,video::SColor(255,255,255,255));
        skin->setColor((EGUI_DEFAULT_COLOR)EGAWC_INACTIVE_WINDOW_SYMBOL ,video::SColor(200,40,40,40));

        wm->setSkin(skin);
        wm->setMinimizedPosition(EGWMMP_RIGHT);
        wm->setMinimizedDirection(EGWMMD_FROM_TOP_LEFT);
	}

    /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	env->addButton(rect<s32>(10,240,240,240 + 32), 0, GUI_ID_QUIT_BUTTON,
			L"Quit", L"Exits Program");
	env->addButton(rect<s32>(10,280,240,280 + 32), 0, GUI_ID_NEW_SMALL_WINDOW_BUTTON,
			L"New Small Window", L"Launches a new Window");
	env->addButton(rect<s32>(10,320,240,320 + 32), 0, GUI_ID_NEW_LARGE_WINDOW_BUTTON,
			L"New Large Window", L"Launches a new Window");
	env->addButton(rect<s32>(10,440,120,460), 0, GUI_ID_SHOW_JOINTS_BUTTON,
			L"Show Joints", L"DEBUG : Shows the joints");
	env->addButton(rect<s32>(130,440,240,460), 0, GUI_ID_HIDE_JOINTS_BUTTON,
			L"Hide Joints", L"DEBUG : Hides the joints");
	IGUIStaticText * behavior = env->addStaticText(0, rect<s32>(10,470,240,490), true );
	env->addButton(rect<s32>(10,500,120,520), 0, GUI_ID_GRID_RESET,
			stringw(GUIDynamicGridBehaviorNames[0]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(130,500,240,520), 0, GUI_ID_GRID_ATTACH_RUN_ONCE,
			stringw(GUIDynamicGridBehaviorNames[1]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(10,530,120,550), 0, GUI_ID_GRID_ATTACH_FOCUSED,
			stringw(GUIDynamicGridBehaviorNames[2]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(130,530,240,550), 0, GUI_ID_GRID_RUN,
			stringw(GUIDynamicGridBehaviorNames[3]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(10,560,120,580), 0, GUI_ID_GRID_DETACH_FOCUSED,
			stringw(GUIDynamicGridBehaviorNames[4]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(130,560,240,580), 0, GUI_ID_GRID_DETACH_ALL,
			stringw(GUIDynamicGridBehaviorNames[5]).c_str(), L"DYNAMIC GRID BEHAVIOR : Reset");
	env->addButton(rect<s32>(10,590,120,610), 0, GUI_ID_WINDOW_MINIMIZE_ALL,
			L"Minimize All", L"");
	env->addButton(rect<s32>(130,590,240,610), 0, GUI_ID_WINDOW_RESTORE_ALL,
			L"Restore All", L"");
    /// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	env->addStaticText(L"Logging ListBox:", rect<s32>(50,110,250,130), true);
	IGUIListBox * listbox = env->addListBox(rect<s32>(50, 140, 250, 210));

	SAppContext context;
	context.device = device;
	context.driver = driver;
	context.counter = 0;
	context.listbox = listbox;
	context.wm = wm;

	MyEventReceiver receiver(context);
	device->setEventReceiver(&receiver);

	env->addImage(driver->getTexture("../../media/irrlichtlogo2.png"),
			position2d<int>(10,10));

	ITexture * background_tex = driver->getTexture("../../media/brushed-steel-texture4.jpg"); //brushed-steel-texture.jpg   cosmos0001.jpg

	while(device->run() && driver)
	{
	   device->sleep(10);
        if (device->isWindowActive())
        {
            behavior->setText( stringw(GUIDynamicGridBehaviorNames[wm->getBehavior()]).c_str() );
            driver->beginScene(true, true, SColor(0,0,0,0));
            driver->draw2DImage(
                background_tex,
                position2d<s32>(0,0),
                rect<s32>(0,0,1920,1200),
                0,
                SColor(255,255,255,255),
                true
            );
            env->drawAll();

            driver->endScene();
        }
                device->sleep(5);
	}

	device->drop();

	return 0;
}

/*
**/
