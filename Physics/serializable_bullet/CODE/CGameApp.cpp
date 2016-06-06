#include "CGameApp.h"

//------------------------------------------------------------------------------
//! Ctor
//! Base initialization
CGameApp::CGameApp() :
  iVideoDim(0, 0), iDriverType(video::EDT_NULL),
  iIrrDevice(NULL),
  iIrrVideoDriver(NULL),
  iIrrSceneManager(NULL)
{
}

//------------------------------------------------------------------------------
//! Dtor
//!
CGameApp::~CGameApp()
{

}

//------------------------------------------------------------------------------
//! Create app
//!
bool CGameApp::Create()
{
  SIrrlichtCreationParameters aIrrDeviceParams;
  aIrrDeviceParams.EventReceiver = this;
  aIrrDeviceParams.DriverType = iDriverType;
  aIrrDeviceParams.WindowSize = iVideoDim;
  aIrrDeviceParams.Bits = 32;
  aIrrDeviceParams.Fullscreen = false;
  aIrrDeviceParams.Stencilbuffer = false;
  aIrrDeviceParams.Vsync = false;
  aIrrDeviceParams.HighPrecisionFPU = false;
  aIrrDeviceParams.AntiAlias = false;

  // create device
  iIrrDevice = createDeviceEx(aIrrDeviceParams);
  if (iIrrDevice == NULL)
  {
    // could not create selected driver.
    return false;
  }

  /*
  Get a pointer to the video driver, the SceneManager and GUIEnvironment.
  */
  iIrrVideoDriver    = iIrrDevice->getVideoDriver();
  iIrrSceneManager   = iIrrDevice->getSceneManager();
  iIrrGUIEnvironment = iIrrDevice->getGUIEnvironment();


  // Do app specific work
  OnCreate();

  return true;
}

//------------------------------------------------------------------------------
//! Run main loop
//!
bool CGameApp::Run()
{
	/*
	We have done everything, so lets draw it. We also write the current
	frames per second and the drawn primitives to the caption of the
	window. The 'if (device->isWindowActive())' line is optional, but
	prevents the engine render to set the position of the mouse cursor
	after task switching when other program are active.
	*/
  bool aIsRunning = true;

  while (aIsRunning)
  {
    // process messages
    aIsRunning = iIrrDevice->run();

    if (iIrrDevice->isWindowActive())
    {
      // update world
      OnUpdate(iIrrDevice->getTimer()->getTime());


      // render world
      iIrrVideoDriver->beginScene(true, true, video::SColor(255,20,20,40));
      // custom render
      OnRender();
      iIrrSceneManager->drawAll();
      iIrrGUIEnvironment->drawAll();
      iIrrVideoDriver->endScene();

      int fps = iIrrVideoDriver->getFPS();

      // show fps
      {
        io::IAttributes * attr = iIrrSceneManager->getParameters();


        s32 calls = attr->getAttributeAsInt ( "calls" );
        s32 culled = attr->getAttributeAsInt ( "culled" );
        u32 primitives = iIrrVideoDriver->getPrimitiveCountDrawn();

        core::stringw str = L"MiniGolf [";
        str += iIrrVideoDriver->getName();
        str += "] FPS:";
        str += fps;
        str += " Cull:";
        str += calls;
        str += "/";
        str += culled;
        str += " Triangles:";
        str += primitives;

        iIrrDevice->setWindowCaption(str.c_str());
      }
    }
  }

  /*
  In the end, delete the Irrlicht device.
  */
  iIrrDevice->drop();

  return true;
}

//------------------------------------------------------------------------------
//! Event receiver
//!
void CGameApp::SetVideoDim(s32 pW, s32 pH)
{
  if (iIrrDevice != NULL)
  {
    // TODO: resize
  }

  iVideoDim.set(pW, pH);
}

//------------------------------------------------------------------------------
//! Event receiver
//!
void CGameApp::SetDriverType(video::E_DRIVER_TYPE pDriverType)
{
  if (iIrrDevice != NULL)
  {
    // TODO: reset device
  }
  iDriverType = pDriverType;
}

//------------------------------------------------------------------------------
//! AddEventReciever
//! Add event receiver
void CGameApp::AddEventReciever(IEventReceiver* pReciever)
{
  iEventReceivers.push_back(pReciever);
}

//------------------------------------------------------------------------------
//! RemoveEventReciever
//! Remove event receiver
void CGameApp::RemoveEventReciever(IEventReceiver* pReciever)
{
  for (core::list<IEventReceiver*>::Iterator it = iEventReceivers.begin();
       it != iEventReceivers.end(); it++)
  {
    if (*it == pReciever)
    {
      iEventReceivers.erase(it);
      break;
    }
  }
}

//------------------------------------------------------------------------------
//! Event receiver
//!
bool CGameApp::OnEvent(const SEvent& event)
{
  // propagate event
  for (core::list<IEventReceiver*>::Iterator it = iEventReceivers.begin();
       it != iEventReceivers.end(); it++)
  {
    if ((*it)->OnEvent(event))
    {
      // event absorbed
      return true;
    }
  }

  return false;
}

//------------------------------------------------------------------------------
//! Do additional init work after irrlicht device was created
//!
bool CGameApp::OnCreate()
{
  return true;
}

//------------------------------------------------------------------------------
//! Custom per tick update func
//!
void CGameApp::OnUpdate(u32 timeMs)
{
}

//------------------------------------------------------------------------------
//! Custom render func
//!
void CGameApp::OnRender()
{
}
