#include <irrlicht.h>
#include <iostream>
#include <math.h>
#include <fstream>
#include "archer.h"
#include "EventReceiver.h"
#include "Arrow.h"
#include "Genetic.h"


using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class App
{
public:
    App();
    void run();
    double xaxis(double x0, double v0, float angle, double t);
    double yaxis(double y0, double v0, float angle, double t);
    double toRadians(double angle);
    void shoot(float angle, double power, float time);
    void setupGUI();
    rect<s32> rect4(int x, int y, int w, int h);
    void saveLog(int pop, int gen,double cross, double mut, float angle, double power, double fitness);

private:
    double toDouble(wchar_t* str);
    IrrlichtDevice* device;
    IVideoDriver* driver;
    ISceneManager* smgr;
    IGUIEnvironment* guienv;
    EventReceiver *eventReceiver;

    IGUIStaticText* guiAngleV;
    IGUIStaticText* guiPower;
    IGUIStaticText* guiFitness;
    IGUICheckBox* showResults;
    IGUIEditBox* guiPop;
    IGUIEditBox* guiGen;
    IGUIEditBox* guiMut;
    IGUIEditBox* guiCro;
    IGUIEditBox* guiDistance;

    IMesh* scenarioMesh;
    IMeshSceneNode* scenario;
    IMesh *skyMesh;
    IMeshSceneNode* sky;
    ILightSceneNode* light;

    ICameraSceneNode* cam;

    Archer* archer;
    Arrow *arrow;
};
