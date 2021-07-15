#include "mainwindow.h"
#include <QApplication>
#include <SDL2/SDL.h>
#include <irrlicht.h>
//https://stackoverflow.com/questions/25232756/sdl2-rendering-into-qwidget

SDL_Joystick *joystick1 = NULL;

IGUIEnvironment* env;
    SDL_Event event;


int renderThread()
{




    while(device->run() && driver){

        //{check joysticks
        if ( SDL_NumJoysticks() < 1 ) joy_error->setVisible(true); else joy_error->setVisible(false);
        //}

                if (device->isWindowActive())
            {
                driver->beginScene(video::ECBF_COLOR | video::ECBF_DEPTH, SColor(0,200,200,200));

                env->drawAll();

                driver->endScene();
            }
        //{SDL event receiver
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                    case SDL_KEYDOWN:
                    /* handle keyboard stuff here */
                    break;

                    case SDL_QUIT:
                    /* Set whatever flags are necessary to */
                    /* end the main game loop here */
                    break;

                    case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */

                    if ( ( event.jaxis.value < -3200 ) || (event.jaxis.value > 3200 ) )
                    {
//						 if( event.jaxis.axis == 0)
//						 {
//						 menu_item++;
//						 }
//
//						 if( event.jaxis.axis == 1)
//						 {
//						 menu_item--;
//						 }
                   }
                   break;


            }
            }



    }

SDL_JoystickClose(joystick1);
    device->drop();

    return 0;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindowFrom((void*)w.ptr_gfx->winId());
    SDL_Renderer* render = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    SDL_RenderFillRect(render, NULL);
    SDL_RenderPresent(render);


        // ask user for driver
    //	video::E_DRIVER_TYPE driverType=driverChoiceConsole();
    //	if (driverType==video::EDT_COUNT)
    //		return 1;

        // create device and exit if creation failed
        IrrlichtDevice * device = createDevice(EDT_OPENGL, core::dimension2d<u32>(640, 480));

        if (device == 0)
            return 1; // could not create selected driver.

        /* The creation was successful, now we set the event receiver and
            store pointers to the driver and to the gui environment. */

        device->setWindowCaption(L"Irrlicht Engine - User Interface Demo");
        device->setResizable(true);

        if(SDL_Init(SDL_INIT_JOYSTICK|SDL_INIT_TIMER) < 0)  return 1;

        video::IVideoDriver* driver = device->getVideoDriver();
        env = device->getGUIEnvironment();
         //The images
        SDL_Surface* hello = NULL;
        SDL_Surface* screen = NULL;

        //Start SDL
      //  SDL_Init( SDL_INIT_EVERYTHING );

        //Set up screen
      //  screen = SDL_SetVideoMode( 640, 480, 32, SDL_SWSURFACE );

        //Load image
        hello = SDL_LoadBMP( "sdltest.bmp" );

        //Apply image to screen
       // SDL_BlitSurface( hello, NULL, screen, NULL );

        //Update Screen
       // SDL_Flip( screen );

        SDL_SaveBMP( hello, "your_snapshot_name.bmp" );

           SDL_FreeSurface( hello );


        env->addImage(driver->getTexture("your_snapshot_name.bmp"),
                position2d<int>(10,10));


    //{error text
    IGUIStaticText* joy_error = env->addStaticText(L"Please insert a joystick or gamepad...",rect<s32>(15,1,260,22), true);
    joy_error->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER);
    joy_error->move(position2d<int>(350,350) );
    //}

    //}

    //{open joysticks

    //Open joystick1
    SDL_JoystickEventState(SDL_ENABLE);
    joystick1 = SDL_JoystickOpen(0);




        const io::path mediaPath = "./media/";//getExampleMediaPath();


        IGUISkin* skin = env->getSkin();
        IGUIFont* font = env->getFont(mediaPath + "fonthaettenschweiler.bmp");
        if (font)
            skin->setFont(font);

        skin->setFont(env->getBuiltInFont(), EGDF_TOOLTIP);


        env->addButton(rect<s32>(10,240,110,240 + 32), 0, GUI_ID_QUIT_BUTTON,
                L"Quit", L"Exits Program");
        env->addButton(rect<s32>(10,280,110,280 + 32), 0, GUI_ID_NEW_WINDOW_BUTTON,
                L"New Window", L"Launches a new Window");
        env->addButton(rect<s32>(10,320,110,320 + 32), 0, GUI_ID_FILE_OPEN_BUTTON,
                L"File Open", L"Opens a file");


        env->addStaticText(L"Transparent Control:", rect<s32>(150,20,350,40), true);
        IGUIScrollBar* scrollbar = env->addScrollBar(true,
                rect<s32>(150, 45, 350, 60), 0, GUI_ID_TRANSPARENCY_SCROLL_BAR);
        scrollbar->setMax(255);
        scrollbar->setPos(255);
        setSkinTransparency( scrollbar->getPos(), env->getSkin());

        // set scrollbar position to alpha value of an arbitrary element
        scrollbar->setPos(env->getSkin()->getColor(EGDC_WINDOW).getAlpha());

        env->addStaticText(L"Logging ListBox:", rect<s32>(10,110,350,130), true);
        IGUIListBox * listbox = env->addListBox(rect<s32>(10, 140, 350, 210));
        env->addEditBox(L"Editable Text", rect<s32>(350, 80, 550, 100));

        // Store the appropriate data in a context structure.
        SAppContext context;
        context.device = device;
        context.counter = 0;
        context.listbox = listbox;

        // Then create the event receiver, giving it that context structure.
        MyEventReceiver receiver(context);

        // And tell the device to use our custom event receiver.
        device->setEventReceiver(&receiver);

        env->addImage(driver->getTexture(mediaPath + "irrlichtlogo2.png"),
                position2d<int>(10,10));

        renderThread();
    return a.exec();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(render);
    SDL_Quit();
}
