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

	IrrlichtDevice *device =
		createDevice(EDT_OPENGL, core::dimension2d<u32>(1024, 768),32,false,false,false, &receiver);

	if (device == 0)
		return 1;


	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();


	device->getFileSystem()->addZipFileArchive("./map-20kdm2.pk3");

	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;

	if (mesh)
		node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);

	if (node)
		node->setPosition(core::vector3df(-1300,-144,-1249));

	scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS();
	device->getCursorControl()->setVisible(false);

	// Add ball node
	ISceneNode* ball = smgr->addSphereSceneNode(25);
	ball->setMaterialFlag(video::EMF_LIGHTING, false);


	// put points here!
core::array<core::vector3df> points;
points.push_back(core::vector3df(9.61209,775.952,149.994));
points.push_back(core::vector3df(157.636,703.333,430.049));
points.push_back(core::vector3df(309.295,615.547,968.525));
points.push_back(core::vector3df(676.304,608.047,813.19));
points.push_back(core::vector3df(735.284,608.03,443.086));
points.push_back(core::vector3df(590.427,612.69,-52.2957));
points.push_back(core::vector3df(473.169,621.991,-406.778));
points.push_back(core::vector3df(83.8762,614.54,-564.241));
points.push_back(core::vector3df(-267.004,600.532,-630.234));
points.push_back(core::vector3df(-485.994,589.381,-330.58));
points.push_back(core::vector3df(-108.808,681.572,-105.359));
points.push_back(core::vector3df(-69.0693,725.259,238.067));
points.push_back(core::vector3df(29.5398,716.71,663.684));
points.push_back(core::vector3df(-762.159,873.058,647.072));
points.push_back(core::vector3df(-854.891,766.362,394.708));
points.push_back(core::vector3df(-681.573,659.584,603.079));
points.push_back(core::vector3df(-844.364,532.033,762.483));
points.push_back(core::vector3df(-931.76,415.876,533.981));
points.push_back(core::vector3df(-783.21,365.712,325.554));
points.push_back(core::vector3df(-626.326,365.978,343.783));
points.push_back(core::vector3df(-217.236,447.875,399.743));
points.push_back(core::vector3df(174.385,518.156,425.872));
points.push_back(core::vector3df(410.734,497.84,260.361));
points.push_back(core::vector3df(430.494,625.644,-6.11581));
points.push_back(core::vector3df(403.159,855.381,-165.174));
points.push_back(core::vector3df(375.216,1155.84,-265.046));
points.push_back(core::vector3df(438.572,1072.44,181.74));


bezier4f *haaaa= new bezier4f (points[0],points[1],points[2],points[3] );


float x=0;
int g=points.size();
bool setArray;

	// Add animator
        ISceneNodeAnimator* anim;
    //    anim = smgr->createFollowSplineAnimator(0, points, 5, 0.5, true, false);
    //    ball->addAnimator(anim);
	// ----------------------------------------------------
	PathCreator creator(driver, cam, "test.txt", "points"); // Create an instance of PathCreator

	while(device->run())
	{
		if(receiver.IsKeyDown(irr::KEY_RETURN))
			creator.addPath(); // add path to list
		else if (receiver.IsKeyDown(irr::KEY_BACK))
			creator.delPath(); // delete path from list
		else if (receiver.IsKeyDown(irr::KEY_KEY_C)){
			creator.conPath(); // connect path
            anim = smgr->createFollowSplineAnimator(0, creator.points, 5, 0.5, true, false);
            ball->addAnimator(anim);
	}
		else if (receiver.IsKeyDown(irr::KEY_ESCAPE))
			creator.save(); // save list to file



///////////////////////////////// MY Testing
        // iterate through all the vectors move the ball and make the path

        if (receiver.IsKeyDown(irr::KEY_KEY_X)){setArray=true;}

         if (x <= 0){
            x=1; // use 1 for the full travel
            if ( g<(points.size() -3)){
                g+=2;
                //make sure to set the balls path initially to the first vector.
                haaaa= new bezier4f (ball->getPosition(),points[g+1],points[g+2],points[g+3] );
                //haaaa= new bezier4f (points[g],points[g+1],points[g+2],points[g+3] );
            }else g=0;
        };
        // iterate through all the vectors move the ball and make the path
        if (x >= 0){
            core::vector3df BEZ =  haaaa->getPositionAt(x);
                // printf( "position is X= %f Y= %f Z= %f \n",   BEZ.X,BEZ.Y,BEZ.Z );
            ball->setPosition(core::vector3df( BEZ.X,BEZ.Y,BEZ.Z));
        }


        x-=0.005;
		driver->beginScene(true, true, video::SColor(255,100,100,100));



		creator.drawPath(); // turns lighting off


        for(float i=0; i<=x; i+=.05) {  //draw line of current position between g iteration bezier
            driver->draw3DLine(haaaa->getPositionAt(i), haaaa->getPositionAt(i+.05), video::SColor(255, 0, 255, 0));
            ///driver->draw3DLine(points[g],points[g]
        }

///////////////////////////////// MY Testing




		smgr->drawAll();
		driver->endScene();
        device->sleep(1);
	}


	device->drop();
	return 0;
}
