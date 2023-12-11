//http://www.irrlicht3d.de/forum/viewtopic.php?f=2&t=584&start=0
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL_getenv.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <irrlicht.h>
#include <SDL/SDL_opengl.h>

#include <X11/Xlib.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//#include <gdk/gdk.h>
//#include <gtk/gtk.h>
////#include <wx/gtk/win_gtk.h>
//#include <gdk/gdkx.h>
//typedef WXWidget     HWND;


bool g_bThreadRunning=true;

int keyboardThread(void *nothing)
{
SDL_Event event;

  SDL_EnableKeyRepeat(0,0);
  SDL_EnableUNICODE(1);

  printf("kbThread...\n");
  while (g_bThreadRunning)
  {
    if (SDL_WaitEvent(&event))
    {
      printf("\tEvent ... %i\n",event.type);
      switch (event.type)
      {
        case SDL_QUIT: g_bThreadRunning=false; break;
        case SDL_KEYDOWN: { printf("\t\tkeyDown\n"); if (event.key.keysym.sym == SDLK_ESCAPE) g_bThreadRunning=false; break; }
      }
    }
  }
  printf("kbThread end\n");

  return 0;
}

int threadFunktion(void *nichtVerwendet)
{
int found,i;

  printf("Thread\n");

  while (g_bThreadRunning) { printf("--> Thread\n"); SDL_Delay(1200); }
  printf("Thread end.\n");
  return 0;
}



int main ( int argc, char** argv )
{

SDL_Thread *thread,*kbThread;
int i;
//HWND sdl_hWnd;

  if (SDL_Init(SDL_INIT_EVERYTHING)<0) { printf( "Unable to init SDL: %s\n", SDL_GetError() ); return 1; }
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16,SDL_SWSURFACE|SDL_DOUBLEBUF);//SDL_HWSURFACE
  //  SDL_Surface * sdldisplay = SDL_SetVideoMode(800,600,16,SDL_OPENGL);

//  static SDL_SysWMinfo pInfo;
//  SDL_VERSION(&pInfo.version);
//  SDL_GetWMInfo(&pInfo);
  //sdl_hWnd = pInfo.window;
  //SDL_Window* sdl_hWnd = pInfo.info.x11.window; //wmInfo.info.x11.display
//  Window sdl_hWnd = pInfo.info.x11.window; //wmInfo.info.x11.display
//hWnd = CreateWindow( Win32ClassName, "Irrlicht Win32 window example", style, 100,

//gContext = SDL_GL_CreateContext( gWindow );

 //Use Vsync
//if( SDL_GL_SetSwapInterval( 1 ) < 0 )
//{ printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
//}

//  https://forums.libsdl.org/viewtopic.php?p=37660
// winHandle = Ogre::StringConverter::toString((unsigned
//long)wmInfo.info.win.window);
//break;
//#elif __MACOSX__
//case SDL_SYSWM_COCOA:
////required to make OGRE play nice with our window
//params.insert(std::make_pair("macAPI", "cocoa"));
//params.insert(std::make_pair("macAPICocoaUseNSView", "true"));
//  winHandle = Ogre::StringConverter::toString((unsigned
//long)wmInfo.info.x11.display);
//winHandle += ":0:";
//winHandle += Ogre::StringConverter::toString((unsigned
//long)wmInfo.info.x11.window);


  irr::SIrrlichtCreationParameters   params;
  params.AntiAlias=false;
  params.Bits=16;
  params.DriverType=irr::video::EDT_OPENGL;
//  params.DriverType=irr::video::EDT_SDL2; //EDT_SDL
  params.Fullscreen=false;
  params.Stencilbuffer=false;
  params.Vsync=false;
  params.WindowSize=irr::core::dimension2d<irr::s32>(640,480);
 // params.WindowId = (void *)sdl_hWnd;

  //  params.WindowId = reinterpret_cast<void*>(pInfo.info.x11.window);
   //     params.WindowId = pInfo.info.x11.window;
//  	GtkWidget* widget = window->GetHandle();
//	gtk_widget_realize( widget );
//XID xHandle = GDK_WINDOW_XWINDOW(widget->window);
//
//   param.WindowId = reinterpret_cast<void*>(xHandle);
// params.WindowId = (void*)GDK_WINDOW_XWINDOW (drawing_area->window);
  irr::IrrlichtDevice *device = createDeviceEx(params);

  irr::video::IVideoDriver* driver = device->getVideoDriver();
  irr::scene::ISceneManager* smgr = device->getSceneManager();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

  atexit(SDL_Quit);

  if (!screen) { printf("Unable to set 640x480 video: %s\n", SDL_GetError()); return 1; }

  thread = SDL_CreateThread(threadFunktion, NULL);
  kbThread=SDL_CreateThread(keyboardThread, NULL);

  if (thread==NULL) { fprintf(stderr, "Unable to start thread: %s\n", SDL_GetError()); return 1; }
  if (kbThread==NULL) { fprintf(stderr,"Unable to start keyboard therd: %s\n",SDL_GetError()); return 1; }

  while (g_bThreadRunning) {
      printf(".");

                                          	guienv->addStaticText(L"Hello World! This is Irrlicht with the burnings software renderer!",
		rect<s32>(10,10,260,22), true);
//          SDL_RenderDrawLine(screen,10,10,50,50);
SDL_Delay(100);
      driver->beginScene(true, true, irr::video::SColor(255,40,200,40));

      		        SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

      SDL_Flip(screen);
            SDL_Delay(2500);

      smgr->drawAll();
      guienv->drawAll();

      driver->endScene();

                                          //Fill the surface white

                        //Update the surface
//                        SDL_UpdateWindowSurface( sdl_hWnd );



   }


  SDL_WaitThread(thread  ,NULL);
  SDL_WaitThread(kbThread,NULL);

  printf("Exited cleanly\n");
  return 0;
}
