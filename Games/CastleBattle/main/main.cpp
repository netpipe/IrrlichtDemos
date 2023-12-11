#ifndef MAIN
#define MAIN
#include <irrlicht.h>
#include <cstdlib>
#include <time.h>
#include <list>
#include "Application.h"
using namespace irr;
/** Initialize seed and start ::Application */
int main() {
  Application app = Application();
  srand (time(NULL));
  return (app.init()) ? app.loop() : 1;
}
#endif
