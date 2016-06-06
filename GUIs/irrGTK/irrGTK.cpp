#include <unistd.h>
#include <iostream>
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace gui;

static IAnimatedMeshSceneNode* node;
static bool running;

static void destroy_handler(GtkWidget *widget, gpointer data)
{
   running = false;
}

static void buttons_handler(GtkWidget *widget, gpointer data)
{
   int bdata;

   bdata = *(int *)data;
   switch (bdata) {
   case(scene::EMAT_STAND):
      node->setFrameLoop(1,320);
      break;

   case(scene::EMAT_RUN):
      node->setMD2Animation(scene::EMAT_RUN);
      break;

   case(scene::EMAT_JUMP):
      node->setMD2Animation(scene::EMAT_JUMP);
      break;
   }
}

int main (int argc, char **argv)
{
   // gtk stuff
   GtkWidget *window, *fix, *drawing_area, *button;
   char mode, *blabels[] = { "Stand", "Run", "Jump" };
   int i, bdata[] = { scene::EMAT_STAND, scene::EMAT_RUN, scene::EMAT_JUMP };
   // irrlicht stuff
   SIrrlichtCreationParameters params;
   IrrlichtDevice *device;
   IVideoDriver *driver;
   ISceneManager *smgr;
   IGUIEnvironment *guienv;
   IAnimatedMesh *mesh;
   ICameraSceneNode *cam;

   printf("Please select the mode you want for this example:\n"\
      " (g)tk only\n"\
      " (i)rrlicht only\n"\
      " (m)ixed gtk & irrlicht\n\n");

   std::cin >> mode;

   /* ==================
    *     init gtk
    * ================== */

   if (mode == 'g' || mode == 'm') {
      gtk_init(&argc, &argv);
      window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title(GTK_WINDOW(window), "gtk & irrlicht testbed");
      gtk_window_set_default_size(GTK_WINDOW(window), 640, 480);
      g_signal_connect(G_OBJECT(window), "destroy",
             G_CALLBACK(destroy_handler), NULL);
      // container
      fix = gtk_fixed_new();
      gtk_container_add(GTK_CONTAINER(window), fix);
      // drawing_area
      drawing_area = gtk_drawing_area_new();
      gtk_widget_set_size_request(drawing_area, 320, 240);
      gtk_fixed_put(GTK_FIXED(fix), drawing_area, 0, 0);
      // buttons
      for (i = 0; i < 3; i ++) {
         button = gtk_button_new_with_label(blabels[i]);
         gtk_widget_set_size_request(button, 100, 30);
         if (mode == 'm')
            g_signal_connect(G_OBJECT(button), "released",
                    G_CALLBACK(buttons_handler),
                   &bdata[i]);
         gtk_fixed_put(GTK_FIXED(fix), button, 120 * i, 320);

      }
      // show
      gtk_widget_show_all(window);
   }

        /* ==================
         *   init irrlicht
         * ================== */

   if (mode == 'm') {
//      params.display = GDK_WINDOW_XDISPLAY(drawing_area->window);
  //    params.xid = GDK_WINDOW_XWINDOW (drawing_area->window);


      params.WindowId = (void*)GDK_WINDOW_XWINDOW (drawing_area->window);
   }
   if (mode == 'm' || mode == 'i') {
      params.DriverType = EDT_OPENGL;
      params.WindowSize = dimension2d<s32>(320, 240);
      device = createDeviceEx(params);
      driver = device->getVideoDriver();
      smgr = device->getSceneManager();
      guienv = device->getGUIEnvironment();
      guienv->addStaticText(L"Hello World!", rect<int>(0,0,50,20), true);
      mesh = smgr->getMesh("./sydney.md2");
      node = smgr->addAnimatedMeshSceneNode(mesh);
      node->setMaterialFlag(EMF_LIGHTING, false);
      node->setFrameLoop(1,320);
      node->setPosition(vector3df(0, 0, 0));
      node->setMaterialTexture(0, driver->getTexture("./sydney.bmp"));
      cam = smgr->addCameraSceneNodeFPS(0, 100.0f, 100.0f);
      cam->setPosition(vector3df(0,0,50));
      cam->setTarget(vector3df(0,0,0));
      cam->setInputReceiverEnabled(false);
   }

        /* ==================
         *     run
         * ================== */

   running = true;
   while (running) {
      if (mode == 'i' || mode == 'm' ) {
         device->run();
         driver->beginScene(true, true, SColor(255,100,101,140));
         smgr->drawAll();
         guienv->drawAll();
         driver->endScene();
      }
      if (mode == 'g' || mode == 'm')
         gtk_main_iteration_do(FALSE);
      // don't use all cpu
      usleep(10000);
   }

   return 0;
}
