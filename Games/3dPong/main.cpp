#include "CPong.h"
#include "assert.h"
#include "CBallAnimator.h"
#include "CGUIMenu.h"
void test_reflect(){
    irr::core::vector3df axis(0,1,0);
    irr::core::vector3df in(-1,1,0);
    irr::core::vector3df out = irr::game::CBallAnimator::reflectVector(-in,axis);
    out.normalize();
    printf("out is %f %f %f\n",out.X,out.Y, out.Z);
}

void test_gui(){
    irr::IrrlichtDevice* Device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(800,600), 32, false, false, false, 0);
    irr::gui::IGUIEnvironment* Env = Device->getGUIEnvironment();
    irr::video::IVideoDriver* Driver = Device->getVideoDriver();
    new irr::game::CGUIMenu(Env,irr::core::recti(0,0,800,600));
    while(Device->run()){
        Driver->beginScene(true, true);
        Env->drawAll();
        Driver->endScene();
    }
}
int main(){
    irr::game::CPong pong;
    pong.run();
    //test_gui();
    return 0;
}
