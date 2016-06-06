#include <iostream>

#include "GameEntity/IEntityManager.h"
#include "GameShader/ToonShading.h"
//! the licence is on EntityManager.h

using namespace std;
using namespace GameShader;
using namespace GameEntity;
using namespace GamePhysique;

using namespace irr;
using namespace gui;
using namespace core;
using namespace video;
using namespace scene;
#define SHADER

class EventManager : public IEventReceiver
{
    public :
        EventManager(IrrlichtDevice*dev,IEntityManager*ph) : device(dev), iemgr(ph)
        {}
        ~EventManager(){}
        bool OnEvent(const SEvent &event)
        {
            ICameraSceneNode *cam = device->getSceneManager()->getActiveCamera();
            if(event.EventType == EET_MOUSE_INPUT_EVENT && event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
            {
                IEntity *tmp = iemgr->addEntity(GamePhysique::OBJECT(OFM_TRIMESH),device->getSceneManager()->addCubeSceneNode(),iemgr->getNomberEntity()+1,10.f);
                         tmp->setPosition(device->getSceneManager()->getActiveCamera()->getPosition());
                         tmp->getPhysiqueObjet()->applyCentralImpulse(Outils::vectorToDegre(device->getSceneManager()->getActiveCamera()->getRotation())*2000);
                         tmp->setTexture("add.jpg");
                return true;
            }
            else
                cam->OnEvent(event);
        }
    private :
        IEntityManager*iemgr;
        IrrlichtDevice*device;
};
int main()
{
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n%s"
           "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n",EntityLicence);
    IrrlichtDevice *device = createDevice(video::EDT_OPENGL,core::dimension2di(640,480));
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    IVideoDriver* driver = device->getVideoDriver();
    ISceneManager* smgr = device->getSceneManager();
    IGUIEnvironment *guienv = device->getGUIEnvironment();
    device->setWindowCaption(L"Physique test");

    ICameraSceneNode *cam = smgr->addCameraSceneNodeFPS();
                      cam->setPosition(vector3df(40,85,32));
                      cam->setRotation(vector3df(39,233,0));
    smgr->addLightSceneNode(0,vector3df(0,5000,0),SColorf(1.0f,1.0f,1.0f),10000.0f);
    smgr->addSkyDomeSceneNode(driver->getTexture("skydome.jpg"),8,15,1,2);

    IEntityManager *iemgr = new IEntityManager(device);
    EventManager *event = new EventManager(device,iemgr);
                  device->setEventReceiver(event);


    IEntity *terrain = iemgr->addEntity(GamePhysique::OBJECT(OTP_STATIC),1,0.0f);
             terrain->setTexture("static.jpg");
             terrain->setScale(vector3df(150.f,.5f,200.f));
             terrain->getPhysiqueObjet()->setFriction(8.f);

    int nbr = 10;// 8 = 511, 10 = 1000
	for(int x=0-(nbr/2);x<(nbr/2);x++)
        for(int y=1;y<nbr+1;y++)
            for(int z=0-(nbr/2);z<(nbr/2);z++)
            {
                IEntity *tmp = iemgr->addEntity(GamePhysique::OBJECT(),iemgr->getNomberEntity()+1,10.f);
                         tmp->setTexture("static.jpg");
                         tmp->setPosition(vector3df(x*30,y*25,z*30));
                         tmp->getPhysiqueObjet()->setFriction(8.f);
                         tmp->getPhysiqueObjet()->setRestitution(0.00001f);
            }

    while(device->run())
    {
        driver->beginScene(true, true, video::SColor(255,161,187,210));
            guienv->drawAll();
            smgr->drawAll();
            iemgr->drawAll();
            stringw str  = L"Entity Manager : "; str += driver->getFPS();
                    str += ".fps  |  Entity : "; str += iemgr->getNomberEntity();
                    str += "  |  Poly : ";       str += driver->getPrimitiveCountDrawn();
            device->setWindowCaption(str.c_str());
        driver->endScene();
    }
    delete iemgr;// all physiqueObject is deleted if delete physique
    device->drop();
    return 0;
}
