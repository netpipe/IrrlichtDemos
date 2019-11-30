  #include <irrlicht.h>
  #include <CStateMachine.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int main(int argc, char** argv) {
  IrrlichtDevice *device=createDevice(EDT_OPENGL,dimension2d<u32>(800,600),16,false,false,false,0);
  CStateMachine *pMachine=new CStateMachine();

  device->setWindowCaption(L"irrConfigController");
  pMachine->run(device);
  delete pMachine;
  device->drop();

  return 0;
}

