//include haders and link libraries
#include <IrrNewt.hpp>
#include <irrlicht.h>

using namespace irr;

#include "CSpring.h"


//irrlicht variables
IrrlichtDevice* device;
video::IVideoDriver* driver;
scene::ISceneManager* smgr;
scene::ICameraSceneNode* camera;

//level body
irr::newton::IBody* level_body;
//physics world
irr::newton::IWorld* p_world;

f32 idlelength;
f32 Strength;

//-----------------------------------------------------------------
//the event receiver
class MyEventReceiver:public IEventReceiver {
public:
	virtual bool OnEvent(const SEvent& event) {
	if(event.EventType == EET_KEY_INPUT_EVENT) {
        if(event.KeyInput.PressedDown == false) { //pressed down false

			if(event.KeyInput.Key==KEY_KEY_W){
				idlelength++;
				printf("Idle Length: %i\n",(int)idlelength);
			}

			if(event.KeyInput.Key==KEY_KEY_S){
				idlelength--;
				printf("Idle Length: %i\n",(int)idlelength);
			}


			if(event.KeyInput.Key==KEY_KEY_Q){
				Strength+=0.1;
				printf("Strength: %f\n",Strength);
			}

			if(event.KeyInput.Key==KEY_KEY_A){
				Strength-=0.1;
				printf("Strength: %f\n",Strength);
			}


			//exit
			if(event.KeyInput.Key==KEY_ESCAPE)
				device->closeDevice();


		}//else if(event.KeyInput.PressedDown == false) {
	}
	return false;
	}
}my_event_receiver;


int main(int argc, char** argv) {

    //Get all the framework running
	device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(800,600), 32,false,false,true);
	smgr=device->getSceneManager();
	driver=device->getVideoDriver();
	p_world=irr::newton::createPhysicsWorld(device);
	device->setEventReceiver(&my_event_receiver);

    //Scene stuff
	camera=smgr->addCameraSceneNodeFPS();
	camera->setPosition(core::vector3df(0,55,-50));
    smgr->addLightSceneNode(0,core::vector3df(0,100,-100));

	//Some ground
	scene::ISceneNode* world_node = smgr->addCubeSceneNode(2);
	world_node->setScale(core::vector3df(102,1,102));
	world_node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS,true);
	world_node->setAutomaticCulling(scene::EAC_OFF);
	irr::newton::SBodyFromNode levelData;
	levelData.Node = world_node;
	levelData.Mass = 0;
	levelData.Type = newton::EBT_PRIMITIVE_BOX;
	level_body = p_world->createBody(levelData);
	irr::newton::IMaterial* ground_material= p_world->createMaterial();
	level_body->setMaterial(ground_material);



    //Hooks for the net
	irr::newton::IMaterial* hook_material= p_world->createMaterial();

    //First hook
	scene::ISceneNode* hook_node = smgr->addCubeSceneNode(2);
	hook_node->setPosition(core::vector3df(-20,60,0));
	irr::newton::SBodyFromNode hook_data1;
	hook_data1.Node = hook_node;
	hook_data1.Mass = 0;
	hook_data1.Type = newton::EBT_PRIMITIVE_BOX;
	irr::newton::IBody* hook_body1 = p_world->createBody(hook_data1);
	hook_body1->setMaterial(hook_material);

    //Second hook
	scene::ISceneNode* hook_node2 = smgr->addCubeSceneNode(2);
	hook_node2->setPosition(core::vector3df(20,60,0));
	irr::newton::SBodyFromNode hook_data2;
	hook_data2.Node = hook_node2;
	hook_data2.Mass = 0;
	hook_data2.Type = newton::EBT_PRIMITIVE_BOX;
	irr::newton::IBody* hook_body2 = p_world->createBody(hook_data2);
	hook_body2->setMaterial(hook_material);


    //Spheres in the net
    irr::newton::IMaterial* sphere_material;
	sphere_material= p_world->createMaterial();
	sphere_material->setElasticity(ground_material, 1);

    //Sphere 1
    scene::ISceneNode* sphere_node1;
	irr::newton::SBodyFromNode sphereData;
    irr::newton::IBody *sphere_body;

	sphere_node1 = smgr->addSphereSceneNode(2);
    sphere_node1->setPosition(core::vector3df(-20,20,0));
	sphereData.Node = sphere_node1;
	sphereData.Type = newton::EBT_PRIMITIVE_ELLIPSOID;
    sphereData.Mass=10;
    sphere_body = p_world->createBody(sphereData);
	sphere_body->addForceContinuous(core::vector3df(0,-9.81,0));//gravity
	sphere_body->addForce(core::vector3df(5,0,5));  //give it a little push
	sphere_body->setMaterial(sphere_material);

    //Sphere 2
    scene::ISceneNode* sphere_node2;
	irr::newton::SBodyFromNode sphereData2;
    irr::newton::IBody *sphere_body2;
	sphere_node2 = smgr->addSphereSceneNode(2);
    sphere_node2->setPosition(core::vector3df(20,20,0));
	sphereData2.Node = sphere_node2;
    sphereData2.Mass=10;
	sphereData2.Type = newton::EBT_PRIMITIVE_ELLIPSOID;
    sphere_body2 = p_world->createBody(sphereData2);
	sphere_body2->addForceContinuous(core::vector3df(0,-9.81,0));   //gravity
	sphere_material= p_world->createMaterial();
	sphere_body2->setMaterial(sphere_material);


    //Set up the spring system
    core::list<CSpring*> list_springs = core::list<CSpring*>();

    //Connect everything with everything
    idlelength = 40.0f;
    Strength = 1;
	CSpring *spring = new CSpring(sphere_body,sphere_body2);
	list_springs.push_back(spring);

	CSpring *spring2 = new CSpring(sphere_body,hook_body2);
	spring2->setLength(20);
	list_springs.push_back(spring2);

	CSpring *spring3 = new CSpring(sphere_body2,hook_body1);
	spring3->setLength(20);
	list_springs.push_back(spring3);

	CSpring *spring4 = new CSpring(sphere_body,hook_body1);
	spring4->setLength(20);
	list_springs.push_back(spring4);

	CSpring *spring5 = new CSpring(sphere_body2,hook_body2);
	spring5->setLength(20);
	list_springs.push_back(spring5);

    printf("\n\n\nChange the middle spring's idle length with W and S. Current Length is: 20\n");
    printf("Change the middle spring's strength with Q and A. Current Strength is: 1\n");

    //Here we go
	while(device->run()) {


        driver->beginScene(true,true,video::SColor(0,0,0,0));

        video::SMaterial material;
        material.Lighting = false;
        material.Thickness = 3;
        driver->setMaterial(material);
        driver->setTransform(video::ETS_WORLD,core::matrix4());
        video::SColor drawcolor=video::SColor(255,200,100,0);
        driver->draw3DLine(hook_node->getPosition(),sphere_node1->getPosition(),drawcolor);
        driver->draw3DLine(hook_node2->getPosition(),sphere_node2->getPosition(),drawcolor);


        spring->setStrength(Strength);
        spring->setLength(idlelength);

        core::list<CSpring*>::Iterator it_spring = list_springs.begin();
        for(;it_spring!=list_springs.end();it_spring++){
            (*it_spring)->update();
            (*it_spring)->drawDebugData(driver);
        }
        p_world->update();


        smgr->drawAll();

        driver->endScene();

}

	device->drop();
	return 0;
}




