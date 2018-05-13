#include "app.h"

using namespace std;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

/**
* App constructor, set initial values and load objects.
*/
App::App()
{
    //Load configs
    int w=800;
    int h=600;
    int b=16;
    int f=0;
    std::string line;
    ifstream file ("config.txt");
    if(file.is_open())
    {
        getline (file,line);
        w=atoi(line.c_str());
        getline (file,line);
        h=atoi(line.c_str());
        getline (file,line);
        b=atoi(line.c_str());
        getline (file,line);
        f=atoi(line.c_str());
        file.close();
    }

    //Basic configs
    eventReceiver = new EventReceiver();
    device = createDevice(EDT_OPENGL, dimension2d<s32>(w, h), b, f, false, true, eventReceiver);
    driver = device->getVideoDriver();
    smgr = device->getSceneManager();
    guienv = device->getGUIEnvironment();

    device->setWindowCaption(L"Genetic");

    //Fog Seetings
    driver->setFog(SColor(255,150,170,200),true,0,800);
    //Load 'n Config scenario.
    scenarioMesh=smgr->getMesh("../media/scenario.b3d");
    scenario = smgr->addMeshSceneNode(scenarioMesh);
    scenario->setScale(vector3df(10,10,10));
    scenario->setMaterialFlag(EMF_FOG_ENABLE,true);
    scenario->setPosition(vector3df(0,-5,0));

    //Sky
    skyMesh=smgr->getMesh("../media/sky.b3d");
    sky = smgr->addMeshSceneNode(skyMesh);
    sky->setScale(vector3df(1000,750,1000));
    sky->setMaterialTexture(0,driver->getTexture("../media/sky.jpg"));
    sky->setMaterialFlag(EMF_LIGHTING,false);
    //lighting
    light=smgr->addLightSceneNode(0,vector3df(100,500,50),SColorf(1.0f, 1.0f, 1.0f),5000);
    smgr->setAmbientLight(SColorf(0.5,0.5,0.5));

    //Archer
    archer=new Archer(device);

    //Camera
    cam=smgr->addCameraSceneNode(archer->getMesh(),vector3df(3,5,-5));
    cam->setTarget(vector3df(0,0,0));

    arrow=new Arrow(device);

    setupGUI();

    system("cls");
}

/**
* Start application.
*/
void App::run()
{
    float aimAngleV=0;
    float power=0;
    double tempFitness=0;

    float shootMaxTime=0.9;

    arrow->setPosition(0,10,0);

    float shootTime=0;

    Genetic* genetic=new Genetic(0,0);

    while(device->run())
    {
        sky->setRotation(vector3df(0,sky->getRotation().Y+0.01,0));


        archer->animate();
        if (shootTime<0.1)
            archer->setAnimationAim(aimAngleV);

        if(shootTime>0.5) arrow->hideArrow();


        if(eventReceiver->keyDown(irr::KEY_KEY_A))
            cam->setFOV(0.2);
        else
            cam->setFOV(1.25664);

        if (shootTime>0)
        {
            if (shootTime<shootMaxTime-0.3) arrow->showArrow();
            shoot(aimAngleV,power,shootMaxTime-shootTime);
            shootTime-=0.01;
            archer->rotateMainNode(aimAngleV);
        }

        if (eventReceiver->buttonPressed(1))
        {
            double cro=toDouble((wchar_t*)guiCro->getText());
            double mut=toDouble((wchar_t*)guiMut->getText());
            genetic->redefineRates(cro,mut);

            double pop=toDouble((wchar_t*)guiPop->getText());
            double gen=toDouble((wchar_t*)guiGen->getText());
            Chromosome* tempGenetic=genetic->execute(archer->getDistanceFromTarget(),pop,gen,showResults->isChecked());

            shootMaxTime=tempGenetic->getFitnessTime();
            aimAngleV=tempGenetic->getAngle();
            power=tempGenetic->getPower();

            stringw str="POWER:";
            str+=power;
            guiPower->setText(str.subString(0,11).c_str());
            str="ANGLE:";
            str+=-aimAngleV;
            guiAngleV->setText(str.subString(0,11).c_str());
            str="FITNESS:";
            str+=tempGenetic->getFitness();
            guiFitness->setText(str.subString(0,15).c_str());
            tempFitness=tempGenetic->getFitness();
        }

        if (eventReceiver->buttonPressed(2) && shootTime<0.1)
        {
            arrow->setPosition(archer->getPosition().X,0,0);
            archer->setAnimationShoot();
            shootTime=shootMaxTime;
        }

        if (eventReceiver->buttonPressed(3) && shootTime<0.1)
        {
            double distance=toDouble((wchar_t*)guiDistance->getText());
            if(distance<140)
            {
                distance=140;
                guiDistance->setText(L"140");
            }
            if(distance>340)
            {
                distance=340;
                guiDistance->setText(L"340");
            }
            archer->setDistanceFromTarget(distance);
        }

        if (eventReceiver->buttonPressed(4)) exit(0);

        if (eventReceiver->buttonPressed(5))
            saveLog(toDouble((wchar_t*)guiPop->getText()),
                    toDouble((wchar_t*)guiGen->getText()),
                    toDouble((wchar_t*)guiCro->getText()),
                    toDouble((wchar_t*)guiMut->getText()),
                    aimAngleV,
                    power,
                    tempFitness);



        sky->setPosition(cam->getPosition()+vector3df(0,300,0));



        driver->beginScene(true, true, SColor(0,200,200,200));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }
}

double App::toDouble(wchar_t* str)
{
    stringc nstr=str;
    return atof(nstr.c_str());
}

/**
* @return Returns the X coordinated of the projectile at the time T.
*/
double App::xaxis(double x0, double v0, float angle, double t)
{
    return x0+v0*cos(angle)*t;
}

/**
* @return Returns the Y coordinated of the projectile at the time T.
*/
double App::yaxis(double y0, double v0, float angle, double t)
{
    float gravity=10;
    return y0+v0*sin(angle)*t-(gravity*t*t)/2;
}

/**
* Degrees to Radians..
*/
double App::toRadians(double angle)
{
    return (angle*PI)/180;
}

/**
* Set projectile position according the time
* @param angle: aim angle.
* @param power: shoot power.
* @param shoottime: time.
*/
void App::shoot(float angle, double power, float shoottime)
{
    double X=xaxis(0,power,toRadians(-angle),shoottime);
    double Y=yaxis(0,power,toRadians(-angle),shoottime);
    double Z=0;
    X+=arrow->getPosition().X;
    Y+=arrow->getPosition().Y;
    arrow->setPosition(X,Y,Z);
    arrow->setRotation(vector3df((-angle)-shoottime*(-angle)*1.5,arrow->getRotation().Y,arrow->getRotation().Z));
}

rect<s32> App::rect4(int x,int y,int w,int h)
{
    return rect<s32>(x,y,x+w,y+h);
}

void App::saveLog(int pop, int gen,double cross, double mut, float angle, double power, double fitness)
{
    fstream file;
    file.open("log.csv");

        if(!file.is_open())
        {
            ofstream of;
            of.open("log.csv");
            of << "POPULATION,GENERATIONS,CROSSOVER,MUTATION,ANGLE,POWER,FITNESS" << endl;
            of.close();
            file.open("log.txt");
        }

    file.seekp(0,ios::end);
    file << pop << "," << gen << "," << cross << "," << mut << "," << angle << "," << power << "," << fitness << endl;
    file.close();
}

void App::setupGUI()
{
    guienv->getSkin()->setFont(guienv->getFont("../media/myfont0.png"));
    guiAngleV=guienv->addStaticText(L"ANGLE: 0",rect<s32>(20,20,400,50),false);
    guiPower=guienv->addStaticText(L"POWER: 0",rect<s32>(20,60,400,90),false);
    guiFitness=guienv->addStaticText(L"FITNESS: 0",rect<s32>(20,100,400,130),false);

    guienv->addButton(rect4(driver->getScreenSize().Width-160,20,140,40),0,1,L"RUN GENETIC");
    guienv->addButton(rect4(driver->getScreenSize().Width-160,70,140,40),0,2,L"SHOOT!");

    guienv->addStaticText(L"POPULATION:",rect4(driver->getScreenSize().Width/2-110,18,150,100),false);
    guiPop=guienv->addEditBox(L"100",rect4(driver->getScreenSize().Width/2,20,80,20),true,0);
    guienv->addStaticText(L"GENERATIONS:",rect4(driver->getScreenSize().Width/2-120,48,150,100),false);
    guiGen=guienv->addEditBox(L"10",rect4(driver->getScreenSize().Width/2,50,80,20),true,0);
    guienv->addStaticText(L"CROSSOVER:",rect4(driver->getScreenSize().Width/2-100,78,150,100),false);
    guiCro=guienv->addEditBox(L"0.2",rect4(driver->getScreenSize().Width/2,80,80,20),true,0);
    guienv->addStaticText(L"MUTATION:",rect4(driver->getScreenSize().Width/2-90,108,150,100),false);
    guiMut=guienv->addEditBox(L"0.2",rect4(driver->getScreenSize().Width/2,110,80,20),true,0);

    showResults=guienv->addCheckBox(true,rect4(20,150,200,20),0,2,L"SHOW RESULTS");

    guienv->addStaticText(L"DISTANCE:",rect4(20,190,150,100),false);
    guiDistance=guienv->addEditBox(L"200",rect4(110,190,60,20),true,0);
    guienv->addButton(rect4(20,210,151,20),0,3,L"UPDATE");

    guienv->addButton(rect4(driver->getScreenSize().Width-160,driver->getScreenSize().Height-50,140,40),0,4,L"EXIT");
    guienv->addButton(rect4(driver->getScreenSize().Width-160,driver->getScreenSize().Height-100,140,40),0,5,L"SAVE LOG");
}

