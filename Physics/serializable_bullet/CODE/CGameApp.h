// A game application wrapper class

#ifndef __C_GAME_APP_H_INCLUDED__
#define __C_GAME_APP_H_INCLUDED__

#include "irrlicht.h"

using namespace irr;


//!	CGameApp
/** 
*/
class CGameApp : public IEventReceiver
{
public:

  //! Ctor
  CGameApp();
  //! Dtor
  virtual ~CGameApp();

  //! Create app
  bool Create();

  //! Run main loop
  bool Run();


  //! Get and set screen size
  core::dimension2di GetVideoDim() { return iVideoDim; }
  void SetVideoDim(s32 pW, s32 pH);

  //! Get and set driver type
  video::E_DRIVER_TYPE GetDriverType() { return iDriverType; }
  void  SetDriverType(video::E_DRIVER_TYPE pDriverType);

  //! Some usefull getters
  IrrlichtDevice*       GetDevice() { return iIrrDevice; }
  video::IVideoDriver*  GetVideoDriver() { return iIrrVideoDriver; }
  scene::ISceneManager* GetSceneManager() { return iIrrSceneManager; }
  gui::IGUIEnvironment* GetGUIEnvironment() { return iIrrGUIEnvironment; }

  //! Add and remove event receiver
  void AddEventReciever(IEventReceiver* pReciever);
  void RemoveEventReciever(IEventReceiver* pReciever);

protected:

  //! called if an event happened. returns true if event was processed
  virtual bool OnEvent(SEvent event);


  //! Base initialization
  virtual bool OnCreate();
  //! Update
  virtual void OnUpdate(u32 timeMs);
  //! Render
  virtual void OnRender();


private:

  //! Screen size
  core::dimension2di      iVideoDim;

  //! Irrlicht driver type
  video::E_DRIVER_TYPE    iDriverType;

  //! Irrlicht related objects
  IrrlichtDevice*         iIrrDevice;
  video::IVideoDriver*    iIrrVideoDriver;
  scene::ISceneManager*   iIrrSceneManager;
  gui::IGUIEnvironment*   iIrrGUIEnvironment;

  core::list<IEventReceiver*> iEventReceivers;
};


#endif
