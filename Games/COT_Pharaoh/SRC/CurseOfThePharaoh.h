
#include <irrlicht.h>
//#include <audiere.h>
#include <iostream>
#include <vector>

using namespace irr;
using namespace core;
using namespace scene;
using namespace io;
using namespace video;
using namespace gui;
//using namespace audiere;
using namespace std;

class CurseOfThePharoah {

      public:
             CurseOfThePharoah();
             int run();
             vector3df getTargetAngle(vector3df v, vector3df r);
             float calculateVolume(array<vector3df> eposs, vector3df lpos);
             int calculateBalance(vector3df epos, vector3df lpos);

      private:
              IGUIStaticText* fps,* clock,* progText;
              IGUIImage* background,* progBar,* progBox;
              IGUIEnvironment* env;
              IVideoDriver* driver;
              ISceneManager* smgr;
              dimension2d<int> screenDim;
//              OutputStreamPtr fireSound;

              void updateClock();
              void updateFPS();
              void createProgressBar();
              void displayProgressBar(bool val);
              void updateProgressBar(int newValue, stringw msg);
};
