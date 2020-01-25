//#include <windows.h>

#ifdef __cplusplus
    #include <cstdlib>
#else
    #include <stdlib.h>
#endif

#include <SDL/SDL_getenv.h>
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <irrlicht.h>

#include <gdk/gdk.h>
#include <gtk/gtk.h>
//#include <wx/gtk/win_gtk.h>
#include <gdk/gdkx.h>
#include <wx/wx.h>
typedef WXWidget     HWND;



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

  while (g_bThreadRunning) { printf("--> Thread\n"); SDL_Delay(100); }
  printf("Thread end.\n");
  return 0;
}

int main ( int argc, char** argv )
{
SDL_Thread *thread,*kbThread;
int i;
wxWindow *window;
	GtkWidget* widget = window->GetHandle();
	gtk_widget_realize( widget );
XID xHandle = GDK_WINDOW_XWINDOW(widget->window);

HWND sdl_hWnd;

  if (SDL_Init(SDL_INIT_EVERYTHING)<0) { printf( "Unable to init SDL: %s\n", SDL_GetError() ); return 1; }
  SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16,SDL_HWSURFACE|SDL_DOUBLEBUF);

  static SDL_SysWMinfo pInfo;
  SDL_VERSION(&pInfo.version);
  SDL_GetWMInfo(&pInfo);
 // sdl_hWnd = pInfo.info.x11.window;
  //params.WindowId = sdlinfo.info.x11.window;

  irr::SIrrlichtCreationParameters   params;
  params.AntiAlias=false;
  params.Bits=16;
  params.DriverType=irr::video::EDT_OPENGL;
  params.Fullscreen=false;
  params.Stencilbuffer=false;
  params.Vsync=false;
  params.WindowSize=irr::core::dimension2d<irr::s32>(640,480);
  params.WindowId = (void *)sdl_hWnd;

  // params.WindowId = reinterpret_cast<void*>(xHandle);

  irr::IrrlichtDevice *device = createDeviceEx(params);

  irr::video::IVideoDriver* driver = device->getVideoDriver();
  irr::scene::ISceneManager* smgr = device->getSceneManager();

  atexit(SDL_Quit);

  if (!screen) { printf("Unable to set 640x480 video: %s\n", SDL_GetError()); return 1; }

  thread = SDL_CreateThread(threadFunktion, NULL);
  kbThread=SDL_CreateThread(keyboardThread, NULL);

  if (thread==NULL) { fprintf(stderr, "Unable to start thread: %s\n", SDL_GetError()); return 1; }
  if (kbThread==NULL) { fprintf(stderr,"Unable to start keyboard therd: %s\n",SDL_GetError()); return 1; }

  while (g_bThreadRunning) { printf("."); driver->beginScene(true, true, irr::video::SColor(255,0,20,40)); smgr->drawAll(); driver->endScene(); }


  SDL_WaitThread(thread  ,NULL);
  SDL_WaitThread(kbThread,NULL);

  printf("Exited cleanly\n");
  return 0;
}
