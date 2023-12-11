//http://www.irrlicht3d.de/forum/viewtopic.php?f=2&t=584&start=0
#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

//#include <SDL/SDL_getenv.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>
#include <irrlicht.h>
#include <SDL2/SDL_opengl.h>
//#include <GL/GLU.h>

#include <X11/Xlib.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

//#define GTK
#ifdef GTK
//#include <gdk/gdk.h>
#include <gtk/gtk.h>
//#include <wx/gtk/win_gtk.h>
#include <gdk/gdkx.h>
#include <wx/setup.h>
#include <wx/wxprec.h>

//typedef WXWidget     HWND;
// GtkWidget *fix, *drawing_area, *button;
// wxWindow* window;
//HWND* window;
//GtkWidget *windows;
#endif
            //Display *display;           /**< The X11 display */


            	char *display_name = getenv("DISPLAY");  /* address of the X display.      */
//	char variable[64];
	Display *display = XOpenDisplay(display_name);
           Window window;              /**< The X11 window */

bool g_bThreadRunning=true;

int keyboardThread(void *nothing)
{
SDL_Event event;

//  SDL_EnableKeyRepeat(0,0);
//  SDL_EnableUNICODE(1);

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

  while (g_bThreadRunning) { printf("--> Thread\n"); SDL_Delay(1020); }
  printf("Thread end.\n");
  return 0;
}


typedef int32_t s32;

void DrawCircle(SDL_Renderer *Renderer, s32 _x, s32 _y, s32 radius)
{
   s32 x = radius - 1;
   s32 y = 0;
   s32 tx = 1;
   s32 ty = 1;
   s32 err = tx - (radius << 1); // shifting bits left by 1 effectively
                                 // doubles the value. == tx - diameter
   while (x >= y)
   {
      //  Each of the following renders an octant of the circle
      SDL_RenderDrawPoint(Renderer, _x + x, _y - y);
      SDL_RenderDrawPoint(Renderer, _x + x, _y + y);
      SDL_RenderDrawPoint(Renderer, _x - x, _y - y);
      SDL_RenderDrawPoint(Renderer, _x - x, _y + y);
      SDL_RenderDrawPoint(Renderer, _x + y, _y - x);
      SDL_RenderDrawPoint(Renderer, _x + y, _y + x);
      SDL_RenderDrawPoint(Renderer, _x - y, _y - x);
      SDL_RenderDrawPoint(Renderer, _x - y, _y + x);

      if (err <= 0)
      {
         y++;
         err += ty;
         ty += 2;
      }
      if (err > 0)
      {
         x--;
         tx += 2;
         err += tx - (radius << 1);
      }
   }
}

SDL_GLContext gContext;
int main ( int argc, char** argv )
{

SDL_Thread *thread,*kbThread;
int i;
//HWND sdl_hWnd;
//SDL_Init(SDL_INIT_VIDEO);
  if (SDL_Init(SDL_INIT_EVERYTHING)<0) { printf( "Unable to init SDL: %s\n", SDL_GetError() ); return 1; }
  //SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16,SDL_OPENGL);


  SDL_Window *screen = //SDL_CreateWindowFrom((void *)window);
                          SDL_CreateWindow("My Game Window",
                          SDL_WINDOWPOS_UNDEFINED,
                          SDL_WINDOWPOS_UNDEFINED,
                          640, 480,
                          SDL_WINDOW_OPENGL);




    SDL_SysWMinfo sdlinfo;
    SDL_VERSION(&sdlinfo.version);
//    SDL_GetWMInfo(&sdlinfo);
  //SDL_GetWMInfo(&pInfo);
  //if(-1 == SDL_GetWindowWMInfo(screen, &pInfo))
  SDL_GetWindowWMInfo(screen, &sdlinfo);
//  throw std::runtime_error("Couldn't get WM Info!");


 // sdl_hWnd = pInfo.window;
  //SDL_Window* sdl_hWnd = pInfo.info.x11.window; //wmInfo.info.x11.display
//  SDL_Window* sdl_hWnd = pInfo.window;


//SDL_Surface * window_surface = nullptr;

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

#ifdef GTK2
int *test;
char* test2;
gtk_init(test,test2);
windows = gtk_window_new(GTK_WINDOW_TOPLEVEL);
GtkWidget *widget = GetHandle();
gtk_widget_realize(widget);
	GdkWindow * window = GetHandle()->window;
#endif
gContext = SDL_GL_CreateContext( screen );

 //Use Vsync
if( SDL_GL_SetSwapInterval( 1 ) < 0 )
{ printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
}
//Initialize OpenGL
//if( !initGL() )
//{ printf( "Unable to initialize OpenGL!\n" ); success = false; }
//

//code posted for sdl1 makes 2 windows
//        SDL_Init(SDL_INIT_VIDEO );
//        SDL_Surface * sdldisplay = SDL_SetVideoMode(800,600,16,SDL_OPENGL);
//
//        SDL_SysWMinfo sdlinfo;
//        SDL_VERSION(&sdlinfo.version);
//        SDL_GetWMInfo(&sdlinfo);
//        SIrrlichtCreationParameters irrparas = SIrrlichtCreationParameters ();
//
//        irrparas.Bits=16;
//        irrparas.Fullscreen=false;
//        irrparas.WindowSize=dimension2d<s32>(800,600);
//        irrparas.DriverType=EDT_OPENGL;
//        irrparas.WindowId = sdlinfo.info.x11.window;
//
//        IrrlichtDevice *device = createDeviceEx(irrparas);
// eoc

  irr::SIrrlichtCreationParameters   params;
  params.AntiAlias=false;
  params.Bits=16;
  params.DriverType=irr::video::EDT_OPENGL;
  params.Fullscreen=false;
  params.Stencilbuffer=false;
  params.Vsync=false;
  params.WindowSize=irr::core::dimension2d<irr::s32>(640,480);

 #ifdef GTK
// GtkWidget* drawing_area = gtk_drawing_area_new ();
// 	GtkWidget *widget ;
//Window w = gdk_x11_drawable_get_xid(gtk_widget_get_window(drawing_area));
//
//XID xid = GDK_WINDOW_XWINDOW(drawing_area->window);
//params.WindowId = (void*)GDK_WINDOW_XWINDOW (drawing_area->window);



//window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//  	GtkWidget* widget = window->GetHandle();
//	gtk_widget_realize( widget );
//XID xHandle = GDK_WINDOW_XWINDOW(widget->window);
//
  // params.WindowId = reinterpret_cast<void*>(window);
 // params.WindowId = (void*)GDK_WINDOW_XWINDOW (drawing_area->window);
 // params.WindowId = reinterpret_cast<void*>(pInfo.info.x11.window);
 // params.WindowId = xid;

 	GtkWidget* widget = sdlinfo.info.x11.window;//window->GetHandle();
	gtk_widget_realize( widget );
XID xHandle = GDK_WINDOW_XWINDOW(widget->window);

   params.WindowId = reinterpret_cast<void*>(xHandle);


 #else
   params.WindowId = sdlinfo.info.x11.window;//screen;//(void*)sdl_hWnd;
 // params.WindowId = (void*)Window(screen);
 #endif


  irr::IrrlichtDevice *device = createDeviceEx(params);

  irr::video::IVideoDriver* driver = device->getVideoDriver();
  irr::scene::ISceneManager* smgr = device->getSceneManager();
	irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

  atexit(SDL_Quit);

  if (!screen) { printf("Unable to set 640x480 video: %s\n", SDL_GetError()); return 1; }

  thread = SDL_CreateThread(threadFunktion, "test", NULL);
  kbThread=SDL_CreateThread(keyboardThread, "test",NULL);

  if (thread==NULL) { fprintf(stderr, "Unable to start thread: %s\n", SDL_GetError()); return 1; }
  if (kbThread==NULL) { fprintf(stderr,"Unable to start keyboard therd: %s\n",SDL_GetError()); return 1; }


  SDL_Renderer* renderer = SDL_CreateRenderer(screen,-1, SDL_RENDERER_SOFTWARE);//SDL_RENDERER_ACCELERATED); //SDL_RENDERER_SOFTWARE

  while (g_bThreadRunning) {
  //  while (device->run()) {
device->run();
      printf(".");
      driver->beginScene(true, true, irr::video::SColor(211,40,20,40));
	//guienv->addStaticText(L"Hello World! This is Irrlicht with the burnings software renderer!",
		//rect<s32>(10,10,260,22), true);


      smgr->drawAll();
            guienv->drawAll();
    driver->endScene();

//DrawCircle(renderer,20,20,50);
//          window_surface = SDL_GetWindowSurface(sdl_hWnd);
//    image_surface = SDL_LoadBMP("image.bmp");

//    SDL_BlitSurface(image_surface, NULL, window_surface, NULL);
                                    //Fill the surface white
		//        SDL_FillRect( screen, NULL, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );
		        SDL_RenderDrawLine(renderer,10,10,50,50);
//                       int           x1,
//                       int           y1,
//                       int           x2,
//                       int           y2)

//SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
//                         "Missing file",
//                         "File is missing. Please reinstall the program.",
//                         NULL);

                        //Update the surface
                        SDL_UpdateWindowSurface( screen );
//SDL_Flip(screen);

SDL_Delay(100);

   }


  SDL_WaitThread(thread  ,NULL);
  SDL_WaitThread(kbThread,NULL);

  printf("Exited cleanly\n");
  return 0;
}
