//#define INT_MAX 5 //notsure quickfix
#include <string.h>
#include "nvsg/nvsg.h" // NVSG entry points, storage-class defines, etc. ...
#include "GLXRenderArea.h"
#include "SimpleScene.h"

#include "nvsg/PointLight.h"
#include "nvsg/PerspectiveCamera.h"
#include "nvsg/ViewState.h"
#include "nvui/TrackballCameraManipulator.h"

#include "nvsg/PlugInterface.h"
#include "nvsg/PlugInterfaceID.h"
#include "nvutil/PlugIn.h"
#include "nvutil/Tools.h"

#include "CVPlugInCallback.h"

#include <X11/keysym.h>
#include <assert.h>

#include "SaveScene.h"
#include "ExtractGeometryTraverser.h"

#include "nvutil/DbgNew.h" // this must be the last include

using namespace nvsg;
using namespace nvui;
using namespace nvmath;
using namespace nvutil;
using std::string;
using std::vector;

class FindAPolygon : public ExtractGeometryTraverser
{
public:
  FindAPolygon() {}
  ~FindAPolygon() {}



  bool findGoodPosition( Vec3f & out, Vec3f & normal )
  {
    // return the centroid of the one triangle
    out = theTri[0] + theTri[1] + theTri[2];
    out /= 3.f;

    // create a normal, and move us away a few meters
    // create a face normal
    Vec3f v0 = theTri[1] - theTri[0];
    Vec3f v1 = theTri[2] - theTri[0];
    Vec3f n = v0 ^ v1;

    normal = normalize( -n );

    n *= 3.f;

    out += n;

    return true;
  }


  void submitIndexedTriangleSet( 
            const std::vector<unsigned int> & indices, 
            const std::vector<nvmath::Vec3f> & verts )
  {
    // copy the first one out
    theTri[0] = verts[ indices[0] ];
    theTri[1] = verts[ indices[1] ];
    theTri[2] = verts[ indices[2] ];
  }

  Vec3f theTri[3];
};






// define a unique plug-interface ID for SceneLoader 
const nvutil::UPITID PITID_SCENE_LOADER(UPITID_SCENE_LOADER, UPITID_VERSION);

GLXRenderArea * g_renderArea=NULL;
SceneHandle * g_scene=NULL;
ViewStateHandle * g_viewState=NULL;
TrackballCameraManipulator * camManip=NULL;

int attrList[] = { 
  GLX_RGBA, 
  GLX_DOUBLEBUFFER, 
  GLX_RED_SIZE, 4, 
  GLX_GREEN_SIZE, 4, 
  GLX_BLUE_SIZE, 4, 
  GLX_DEPTH_SIZE, 24,
  None 
};






// taken from MCADViewerView::OnInitalUpdate()
void setupCamera()
{
  NVSG_TRACE();
  
  //
  // init camera and camera manipulator
  //
  
  SceneHandle * pScene = g_scene;
  ViewStateHandle * pViewState = NULL;




  // Get the initial ViewState of the scene and clone it for this render area.
  // Ask the ViewState for the camera and use it. If the ViewState does not 
  // contain a valid camera ask the scene object for the first camera in the 
  // camera list. Since we do not want to change the preset camera from the 
  // camera list, simply clone the camera for interactive usage and tell it to the 
  // ViewState Clone. If the scene does not contain cameras in its camera list 
  // simply create a new camera from scratch and tell it to the ViewState Clone.
  if ( g_viewState )
  {
    // copy the documents default view state (ViewState from file)
    pViewState = g_viewState->clone(); 
    pViewState->addRef();
    g_viewState->addRef();
  }

  CameraHandle * pCam = NULL;
  if ( pScene )
  {
    ReadableObject<Scene> scene(pScene);
    // if we have an initial ViewState we use the camera of the initial vs
    if(pViewState && (pCam=ReadableObject<ViewState>(pViewState)->getCamera()))
    {
      // causes assert fail on the way out??
      //pCam->addRef();
    }
    // else try to get the first camera of the scene from the camera list
    else if ( scene->getNumberOfCameras() != 0 )
    {
      pCam = scene->getCamera()->clone();
      NVSG_ASSERT(pCam);
      pCam->addRef();

      if (pViewState == NULL)
      {
        pViewState = CreateHandle(ViewStateHandle);
        pViewState->addRef();
      }
      WritableObject<ViewState> pNCViewState(pViewState);
      pNCViewState->setCamera(pCam);
    }
    // else create our own camera
    else
    {
      pCam = CreateHandle(PerspectiveCameraHandle);
      NVSG_ASSERT(pCam);
      // shouldn't be necessary because we setCamera below
      //pCam->addRef();
      {
        WritableObject<Camera> pNCCamera( pCam );
        pNCCamera->setName( "ViewCamera" );
        Sphere3f bs = getBoundingSphere(scene->getRootNode());
        if ( !isEmpty( bs ) )
        {
          pNCCamera->zoom( bs, float(nvmath::PI_QUARTER) );
        }
      }

      if (pViewState == NULL)
      {
        pViewState = CreateHandle(ViewStateHandle);
        pViewState->addRef();
      }
      WritableObject<ViewState> pNCViewState(pViewState);
      pNCViewState->setCamera(pCam);
    }

    //  make the (world space) window aspect ratio correspond to the 
    // (pixel space) viewport aspect ratio
    {
      WritableObject<Camera> pNCCamera( pCam );
      pNCCamera->setAspectRatio( g_renderArea->getAspectRatio() );

      // If there is no light in the scene and no head light attached 
      // to the camera then add a head light.
      if ( ! scene->containsLight() && pNCCamera->getNumberOfHeadLights() == 0)
      {
        PointLightHandle * pLight = CreateHandle(PointLightHandle);
        WritableObject<PointLight> pPointLight(pLight);
        Vec3f ambient(0.2f, 0.2f, 0.2f);
        Vec3f diffuse(1.0f, 1.0f, 1.0f);
        Vec3f specular(1.0f, 1.0f, 1.0f);
        pPointLight->setAmbientColor(ambient);
        pPointLight->setDiffuseColor(diffuse);
        pPointLight->setSpecularColor(specular);
        pPointLight->setAttenuation(1.0f, 0.0f, 0.0f);

        pNCCamera->addHeadLight( pLight );
      }
    }

    // set scene data
    g_renderArea->setSceneData( pScene, pViewState );

    // we do not need it anymore
    if ( pViewState )
    {
      pViewState->removeRef();
    }
  }
      
  // Create camera manipulator
  camManip = new TrackballCameraManipulator;
  camManip->setRenderArea(g_renderArea);
}














SceneHandle * loadScene(char* filename, vector<string>& searchPaths)
{
  printf("Loading %s...\n", filename); 

  SceneHandle * pScene = NULL;

  // appropriate search paths for the 
  // loader dll and the sample file.
  string modulePath;
  string curDir;
  string dir;
  
  nvutil::GetCurrentDir(curDir);  
  printf("Adding search path %s\n", curDir.c_str());
  searchPaths.push_back(curDir);

  nvutil::GetModulePath(modulePath);
  GetDirFromPath(modulePath, dir);
  printf("Adding search path %s\n", dir.c_str());
  searchPaths.push_back(dir);

  // also add the scene path    
  GetDirFromPath(filename, dir);
  printf("Adding search path %s\n", dir.c_str());
  searchPaths.push_back(dir);

  // for supplied models add additional resource pathes to make sure
  // that required resources will be found by subsequent loaders
  size_t pos = dir.rfind("media");   

  if ( pos != string::npos )
  {
    string nvsgsdk(dir.substr(0, pos));
    searchPaths.push_back(nvsgsdk + "media/effects");
    searchPaths.push_back(nvsgsdk + "media/effects/textures");
    searchPaths.push_back(nvsgsdk + "media/volumes");
  }

    
  // Search for the right loader dll. 
  bool foundAppropriate = false;
  vector<nvutil::UPIID> piids;
  if ( nvutil::queryInterfaceType(searchPaths, PITID_SCENE_LOADER, piids) )
  {
    // extract file extension from filename
    string ext;
    nvutil::GetFileExtFromPath(filename, ext);

    nvutil::UPIID plugID;
    vector<nvutil::UPIID>::iterator it = piids.begin();
    for ( ; it != piids.end(); ++it )
    {
      if ( !_stricmp((*it).getPlugSpecificIDString(), ext.c_str()) )
      {
        // found, copy the ID
        plugID = *it;
        foundAppropriate = true;
        printf("Found %s loader\n", ext.c_str());
        break; // look no further
      }
    }

    // If there is a loader available, 
    // get the interface, load the file and 
    // release the interface when done.
    if ( foundAppropriate )
    {
      nvutil::PlugIn * plug; 
      if ( nvutil::getInterface(searchPaths, plugID, plug) )
      {
        nvsg::SceneLoader * loader = reinterpret_cast<SceneLoader*>(plug);

        loader->setCallback( new CVPlugInCallback( "/tmp/ConsoleViewer.log" ) );

        assert(g_viewState==NULL);
        string fname=filename;        
        pScene = loader->load( (const string)fname, searchPaths, g_viewState );        
        if( pScene ) 
        { 
          pScene->addRef();
        }
        
        nvutil::releaseInterface(plugID);
      }
    }
  }

  if (!foundAppropriate)
  {
    printf("Error: No scene loader found!\n");
  }
  else
  {
    if(pScene)
    {
      return pScene;
    }
  }

  return NULL;
}

int main(int argc, char** argv)
{
  Display* dpy;
  int screen;
  Window wnd;
  XSetWindowAttributes attr;
  XEvent event;
  XVisualInfo* vi;
  int width, height;
  Atom wm_delete_window;
  vector<string> userDefinedPath;

  // one-time initialization of global data required by the NVSG API
  nvsg::nvsgInitialize();
  
  for(int i=2; i< argc; i++)
  {
      userDefinedPath.push_back(string(argv[i])); 	
      printf("Adding search Path %s\n",argv[i]);	
  }

  if( argc > 1 )
  {
    // load a scene as specified on command line
   
    g_scene = loadScene(argv[1], userDefinedPath);
    if( g_scene == NULL )
    {
      printf("Aborting...\n");
      return 1;
    }
  }
  else
  {  
    // create a simple default scene
    g_scene = SimpleSceneFactory::create();
    g_scene->addRef();
  }

  // create the render area
  g_renderArea = new GLXRenderArea;  

  // setup camera and manipulator
  setupCamera();

  dpy = XOpenDisplay(0);
  screen = DefaultScreen(dpy);
  vi = glXChooseVisual(dpy, screen, attrList);

  if( vi == NULL )
  {
    printf("ChooseVisual failed\n");
    exit(0);
  }

  attr.colormap = XCreateColormap( dpy, RootWindow( dpy, vi->screen ),
                  vi->visual, AllocNone );
  attr.border_pixel = 0;  
  attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | StructureNotifyMask;

  wnd = XCreateWindow(dpy, RootWindow(dpy, vi->screen),
            0, 0, 400, 400, 0, vi->depth, InputOutput, vi->visual,
            CWBorderPixel | CWColormap | CWEventMask, &attr);

  XSizeHints sizeHints;
  XWMHints wmHints;

  sizeHints.flags = USPosition | USSize;
  sizeHints.x = 100;
  sizeHints.y = 100;
  sizeHints.width = 400;
  sizeHints.height = 400;

  char * caption = "ConsoleViewer";
  XTextProperty tprop;

  XStringListToTextProperty( &caption, 1, &tprop );

  wmHints.flags = StateHint;
  wmHints.initial_state = NormalState;

  XSetWMProperties( dpy, wnd, &tprop, &tprop, 0, 0, &sizeHints, &wmHints, 0 );

  g_renderArea->attachWindow(dpy, wnd);
  g_renderArea->init(vi);
  g_renderArea->setViewportSize(400, 400);

  XMapRaised(dpy, wnd);  

  // register to receive a message, when the window is about to be destroyed
  wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
  XSetWMProtocols( dpy, wnd, &wm_delete_window, 1);

  // main event loop
  bool done = false;  
  while (!done)
  {    
    while (XPending(dpy) > 0)
    {
      XNextEvent(dpy, &event);             
      switch (event.type)
      {
        case Expose:
            if (event.xexpose.count == 0)
            {
              g_renderArea->renderScene();
            }                 

            break;

        case ConfigureNotify:
            // detect window-size change          
            if ((event.xconfigure.width != width) || 
                (event.xconfigure.height != height))
            {
              width=event.xconfigure.width;
              height=event.xconfigure.height;
              g_renderArea->setViewportSize(width,height);
              g_renderArea->triggerRedraw();
            }
            break;        

        case ButtonPress:                   
            switch(event.xbutton.button)
            {
            case Button1:
              camManip->mouseButtonDown(TrackballCameraManipulator::_LEFT_MOUSE_BTN, event.xbutton.x, event.xbutton.y);
              break;
            case Button2:
              camManip->mouseButtonDown(TrackballCameraManipulator::_MIDDLE_MOUSE_BTN, event.xbutton.x, event.xbutton.y);
              break;
            case Button3:
              break;
            default:
              break;
            }
            break;

        case ButtonRelease:                   
            switch(event.xbutton.button)
            {
            case Button1:
              camManip->mouseButtonUp(TrackballCameraManipulator::_LEFT_MOUSE_BTN, event.xbutton.x, event.xbutton.y);
              break;
            case Button2:
              camManip->mouseButtonUp(TrackballCameraManipulator::_MIDDLE_MOUSE_BTN, event.xbutton.x, event.xbutton.y);
              break;
            case Button3:
              break;
            default:
              break;
            }
            break;

        case MotionNotify:
            camManip->mouseMotion(event.xmotion.x, event.xmotion.y);
            if( (XPending(dpy) == 0) && camManip->updateFrame())
            {
              g_renderArea->triggerRedraw();
            }
            break;

        case KeyPress:
        {
            KeySym ks = XLookupKeysym( &event.xkey, 0 );
            if (ks == XK_Escape)
            {
              done = true;
            }
            else if( ks == XK_Shift_L || ks == XK_Shift_R )
            {
              camManip->keyDown(TrackballCameraManipulator::_SHIFT_KEY);
            }
            else if( ks == XK_Control_L || ks == XK_Control_R )
            {
              camManip->keyDown(TrackballCameraManipulator::_CONTROL_KEY);
            }
            else if( ks == 's' )
            {
              DumpScreen( "screenshot.bmp", width, height );
            }
            else if( ks == 'a' )
            {
              DumpScene( "scene.nvsg", g_scene, g_viewState );
            }
            else if( ks == 'b' )
            {
              DumpScene( "scene.nbf", g_scene, g_viewState );
            }
            else if( ks == 'f' )
            {
              // find a polygon in the scene and move there
              FindAPolygon fap;

              fap.apply( g_scene );

              ReadableObject<ViewState> pNCViewState(g_viewState);
              CameraHandle * hcam = pNCViewState->getCamera();    

              // reset manipulator
              camManip->reset();

              // set new position
              {
                WritableObject< Camera > cam( hcam );

                Vec3f pos, norm;
                fap.findGoodPosition( pos, norm );

                cam->setPosition( pos );

                std::cout << "Norm: " << norm[0] << " " << norm[1] 
                                      << " " << norm[2] << std::endl;

                cam->setDirection( norm );
              }

              g_renderArea->triggerRedraw();
            }

            break;       
        }

        case KeyRelease:
        {
            KeySym ks = XLookupKeysym( &event.xkey, 0 );
            if (ks == XK_Escape)
            {
              done = true;
            }
            else if( ks == XK_Shift_L || ks == XK_Shift_R )
            {
              camManip->keyUp(TrackballCameraManipulator::_SHIFT_KEY);
            }
            else if( ks == XK_Control_L || ks == XK_Control_R )
            {
              camManip->keyUp(TrackballCameraManipulator::_CONTROL_KEY);
            }

            break;       
        }

        case ClientMessage:
            if( event.xclient.data.l[0] == wm_delete_window )
            {
              done = true;
            }
            
            break;

        default:
            break;
      }
    }
  }

  g_renderArea->destroy();
  delete g_renderArea;

  XCloseDisplay(dpy);

  g_scene->removeRef();

  if( g_viewState )
  {
    g_viewState->removeRef();
  }

  delete camManip;

  // free global data required by the nvsg API
  nvsg::nvsgTerminate();

  return 0;
}

