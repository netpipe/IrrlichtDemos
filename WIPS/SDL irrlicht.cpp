#include <irrlicht/irrlicht.h>
#include <iostream>
#include "event.h"
using namespace std;
using namespace irr;

int main()
{
@@	SDL_InputEvent event;
	
	IrrlichtDevice* device = createDevice( video::EDT_OPENGL );
	scene::ISceneManager* scn = device->getSceneManager();
	video::IVideoDriver* driver = device->getVideoDriver();
	
	scene::ISceneNode* node = scn->addCubeSceneNode();
	scene::ICameraSceneNode* cam = scn->addCameraSceneNode();
	
	scene::ISceneNodeAnimator* anim = scn->createRotationAnimator( core::vector3df( 0, 0.1f, 0) );
	node->addAnimator( anim );
	anim->drop();
	
	cam->setPosition( core::vector3df( 15, 15, 15 ) );
	cam->setTarget( core::vector3df( 0, 0, 0) );
	
	node->setMaterialFlag( video::EMF_LIGHTING, false );
	
	core::vector3df center;
	core::vector3df scale (1,1,1);
	while( device->run() && !event.quitMessageReceived() )
	{
@@		event.beginEventHandle();
		
@@		center = core::vector3df( event.getJoyAxis(0), event.getJoyAxis(1), event.getJoyAxis(2) );
		center *= 4.0f;
@@		scale *= event.getJoyAxis(3)+1.01f;
		
		cout<<"Center = "<<center.X<<", "<<center.Y<<", "<<center.Z<<'\t'
			<<"Scale = "<<scale.X<<". "<<scale.Y<<". "<<scale.Z<<'\r';
		
		node->setPosition( center );
		node->setScale( scale );
		
		driver->beginScene( true, true, video::SColor( 255, 0, 0, 255 ) );
		scn->drawAll();
		driver->endScene();
		
		scale = core::vector3df( 1,1,1 );
		
		device->yield();
	}
	
	device->closeDevice();
	return 0;
}