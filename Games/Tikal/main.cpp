#include <irrlicht.h>
#include "src/Game/TKCore.h"


using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;



int main(int argc, char** argv)
{
    TKCore* tkCore=new TKCore();

    //tkCore->loadLevel("MAP0");

    tkCore->runGame();



    return 0;
}

