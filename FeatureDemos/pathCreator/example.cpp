/*
* A little example of use PathCreator
*/

#include <irrlicht.h>
#include <iostream>
#include "PathCreator.h"

#include "Bezier.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


class MyEventReceiver : public IEventReceiver
{
public:
        virtual bool OnEvent(const SEvent& event)
        {
                if (event.EventType == irr::EET_KEY_INPUT_EVENT)
                        KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
                return false;
        }

        virtual bool IsKeyDown(EKEY_CODE keyCode) const
        {                return KeyIsDown[keyCode];        }

        MyEventReceiver()
        {
                for (u32 i=0; i<KEY_KEY_CODES_COUNT; ++i)
                        KeyIsDown[i] = false;
        }

private:
        bool KeyIsDown[KEY_KEY_CODES_COUNT];
};


int main()
{

	video::E_DRIVER_TYPE driverType;

	MyEventReceiver receiver;

	IrrlichtDevice *device =    createDevice(EDT_OPENGL, core::dimension2d<u32>(1024, 768),32,false,false,false, &receiver);

	if (device == 0)		return 1;

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	device->getFileSystem()->addZipFileArchive("../../media/map-20kdm2.pk3");

	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;

	if (mesh)	node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);
	if (node)	node->setPosition(core::vector3df(-1300,-144,-1249));

	scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);

    //splineAnimator
    #include "parabolaAnimator.h"
    /*
        for (int i = 0; i < gibMeshCount; i ++)
        {
            scene::IAnimatedMeshSceneNode *n = smgr->addAnimatedMeshSceneNode(gibMeshes[i]);

            core::vector3df rvel;
            rvel.X = rand()%100 * ((rand()%2) ? -1 : 1);
            rvel.Y = rand()%100 * ((rand()%2) ? -1 : 1);
            rvel.Z = rand()%100 * ((rand()%2) ? -1 : 1);

            scene::ISceneNodeAnimator *an = new ParabolaAnimator(
                smgr,
                device->getTimer()->getTime(),
                gibPos, // initial position
                core::vector3df(rand()%200-100,100.0,rand()%200-100) / 50.0, // initial velocity
                core::vector3df(0,-5.0,0), // acceleration
                rvel, // angular velocity
                core::plane3df(0,-1.0,0, 0,1,0) // ground plane
                );
            n->addAnimator(an);
            an->drop();
        }
    */

	// Add ball node
	ISceneNode* ball = smgr->addSphereSceneNode(25);
	ball->setMaterialFlag(video::EMF_LIGHTING, false);


	// Add animator
    ISceneNodeAnimator* anim;
    //    anim = smgr->createFollowSplineAnimator(0, points, 5, 0.5, true, false);
    //    ball->addAnimator(anim);
	PathCreator creator(driver, cam, "test.txt", "points"); // Create an instance of PathCreator


    core::vector3df ok = core::vector3df(292.802,215.752,975.173);
    bezier4f *haaaa= new bezier4f (ok,ok,ok,ok );

    float x = 0;
    int g   = 0;
    bool setArray =0;


    ///MAIN LOOP
	while(device->run())
	{
		if(receiver.IsKeyDown(irr::KEY_RETURN)){
			creator.addPath(); // add path to list
		//	g=creator.points.size();
        }

		else if (receiver.IsKeyDown(irr::KEY_BACK))
			creator.delPath(); // delete path from list

		else if (receiver.IsKeyDown(irr::KEY_KEY_C)){
			creator.conPath(); // connect path
            anim = smgr->createFollowSplineAnimator(0, creator.points, 5, 0.5, true, false);
            ball->addAnimator(anim);
        }

		else if (receiver.IsKeyDown(irr::KEY_ESCAPE))
			creator.save(); // save list to file

		driver->beginScene(true, true, video::SColor(255,100,100,100));

//cam->getUpVector();
///////////////////////////////// MY Testing
        // iterate through all the vectors move the ball and make the path

        if (receiver.IsKeyDown(irr::KEY_KEY_X)){setArray=true;}

if (setArray ){
        if (x >= 0 ){ //1 for full length
            core::vector3df BEZ =  haaaa->getPositionAt(x);
                // printf( "position is X= %f Y= %f Z= %f \n",   BEZ.X,BEZ.Y,BEZ.Z );
            ball->setPosition(core::vector3df( BEZ.X,BEZ.Y,BEZ.Z));
        } else {
            x=0.5;
            if (g>=(creator.points.size()-3)){
                g+=1;
                haaaa =new bezier4f(creator.points[g],creator.points[g+1],creator.points[g+2],creator.points[g+3] );
            }else g=0;
        };

        x-=0.01;

		creator.drawPath(); // draw path on scene
        for(float i=0; i<=x; i+=.05) {  //draw line of current position between g iteration bezier
            driver->draw3DLine(haaaa->getPositionAt(i), haaaa->getPositionAt(i+.05), video::SColor(255, 0, 255, 0));
            ///driver->draw3DLine(points[g],points[g]
        }
}
///////////////////////////////// MY Testing


		smgr->drawAll();
		driver->endScene();
        device->sleep(10);
	}


	device->drop();
	return 0;
}
