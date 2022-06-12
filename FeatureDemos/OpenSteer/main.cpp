/*! Irrlicht implementation by A.Buschhüter (http://abusoft.g0dsoft.com) */

// not sure what i did wrong but its not running at the moment , could be just from newer irrlicht

#include "cOpenSteerDemo.h"
#undef useIrrExtensions13

int main(int argc, char **argv){

  // initialize graphics first !!!
  OpenSteer::initializeGraphics();
  // initialize OpenSteerDemo application
  OpenSteer::OpenSteerDemo::initialize();
   OpenSteer::OpenSteerDemo::selectNextPlugIn();
 //  run main loop
  OpenSteer::runGraphics();

  return 0;
}
