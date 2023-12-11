#include "raycasttankexample.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <irrlicht.h>
#include <iostream>
//#include "../../include/irrBullet.h"

    CRaycastTankExample example;

using namespace std;

void main_loop(){

 example.runExample();
}

int main()
{

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(main_loop,0,1);
#else
    CRaycastTankExample example;
    example.runExample();
#endif

    return 0;
}
