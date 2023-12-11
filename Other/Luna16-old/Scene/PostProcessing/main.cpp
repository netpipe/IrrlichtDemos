/*
#include <irrlicht.h>
#ifdef _IRR_WINDOWS_
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#pragma comment(lib, "Irrlicht.lib")
#endif
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

// Include the headers for post processing
#include "CRendererPostProc.h"
#include "CEffectPostProc.h"
#include "CTransitionPostProc.h"
#include "CSplitPostProc.h"
#include "CLensFlarePostProc.h"
#include "CWaterPostProc.h"

// Got an error? chances are this is the line to fix
// Specify the media directory - this is different for me on mac/windows, and depends on how your project is set up
#ifdef _IRR_WINDOWS_
#define MEDIA_DIRECTORY "media/"
#else
#define MEDIA_DIRECTORY "../../media/"
#endif

// Change this to EDT_OPENGL, EDT_DIRECT3D8 or EDT_DIRECT3D9
#define DRIVER video::EDT_OPENGL

// Change which of these lines is uncommented to specify the example to show
//#define EXAMPLE_ONE_EFFECT
//#define EXAMPLE_LENS_FLARE
//#define EXAMPLE_ONE_EFFECT_ANIMATED
//#define EXAMPLE_MANY_EFFECTS
//#define EXAMPLE_MIXING_SCENES
//#define EXAMPLE_MIXING_DIFFERENT_RENDERS_OF_THE_SAME_SCENE
//#define EXAMPLE_TRANSITIONS
//#define EXAMPLE_SPLITSCREEN
//#define EXAMPLE_DEPTH_OF_FIELD
//#define EXAMPLE_HAZE
#define EXAMPLE_CUSTOM_EFFECT

// This last one is my playground for testing new effects
//#define EXAMPLE_SANDBOX


// An odd bug: any effects which only use 1 scene manager crash when closed. Adding the line
// ISceneManager* smgr2 = smgr->createNewSceneManager( false );
// to them stops this crash. Bug in Irrlicht or my code?


// Each example is entirely self-contained - find the one you're looking for below:

#if defined EXAMPLE_ONE_EFFECT
// EXAMPLE 1: A single effect (blur)

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Single Effect", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node->setMaterialFlag( EMF_LIGHTING, false );
	node->setMD2Animation( scene::EMAT_STAND );
	node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	smgr->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 0.0f, 5.0f, 0.0f ) );

	// Interesting stuff

	// We make 2 objects - a scene renderer, and a blur. both render to textures with size 1024x512 (always use powers of 2)
	// When setting up the renderer, the parameters are:
	// SceneManager to render, size of output, clearBackBuffer?, clearZBuffer?, background colour
	IPostProc* ppRenderer = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	// When setting up the effect, the parameters are:
	// Input, size of output, effect ID (see CEffectPostProc.h for full list), effect parameters (in this case, blur size)
	CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer, dimension2di( 1024, 512 ), PP_BLUR, 0.01f );
	// Change to a better quality - not all shaders will respect these, but they can be used to hint the rendering standard required.
	ppBlur->setQuality( PPQ_GOOD );
	// Options (worst to best): PPQ_CRUDE, PPQ_FAST, PPQ_DEFAULT, PPQ_GOOD, PPQ_BEST
	// You can also call setOverallQuality( PPQ_WHATEVER ) to change the quality of all effects which are in the chain.

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		// What's happened to beginScene? well we want to use beginScene( false, false ) for speed, but this makes Irrlicht complain (wrongly) when using Direct3D9, so use ( false, true ) in that case.
		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		// The rendering is as normal, except smgr->drawAll(); is replaced with this line:
		ppBlur->render( NULL ); // NULL = render to screen. Can also take a texture to render to, or no parameter (renders to an internal texture)
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppBlur;
	delete ppRenderer;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_LENS_FLARE
// EXAMPLE 2: A lens flare

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Lens Flare", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node->setMaterialFlag( EMF_LIGHTING, false );
	node->setMD2Animation( scene::EMAT_STAND );
	node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	IAnimatedMeshSceneNode* node2 = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node2->setMaterialFlag( EMF_LIGHTING, false );
	node2->setMD2Animation( scene::EMAT_STAND );
	node2->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	node2->setPosition( vector3df( 20.0f, 0.0f, 0.0f ) );
	ICameraSceneNode* cam = smgr->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 0.0f, 5.0f, 0.0f ) );

	ISceneNode* sun = smgr->addSphereSceneNode( 50.0f, 16 );
	sun->setPosition( vector3df( 0.0f, 50.0f, 1000.0f ) );
	sun->setMaterialFlag( EMF_LIGHTING, false );
	sun->setMaterialTexture( 0, driver->getTexture("sun.png") );
	// Interesting stuff

	// As before, we make a renderer
	IPostProc* ppRenderer = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	// Now make a flare effect:
	// (render from, output size, sun scene node)
	// can also use a vector instead of a scene node - when using a scene node the position will follow node automatically
	CLensFlarePostProc* ppFlare1 = new CLensFlarePostProc( ppRenderer, dimension2di( 1024, 512 ), driver->getTexture("flare.png"), sun, 50.0f );
	CLensFlarePostProc* ppFlare2 = new CLensFlarePostProc( ppFlare1, dimension2di( 1024, 512 ), driver->getTexture("flare.png"), vector3df( -2000.0f, 50.0f, 1000.0f ) );
	ppFlare2->setQuality( PPQ_CRUDE ); // Setting the quality to crude avoids pixel checking, which is slow (expecially when more than one lens flare), so if you use >1 flare, set most of them to crude.

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		cam->setPosition( vector3df( -(device->getCursorControl( )->getPosition( ).X - 320.0f) * 0.1f, (device->getCursorControl( )->getPosition( ).Y - 240.0f) * 0.2f, -70.0f ) );
		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		ppFlare2->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppFlare1;
	delete ppFlare2;
	delete ppRenderer;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_ONE_EFFECT_ANIMATED
// EXAMPLE 2: A single effect (blur) animated

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Animated Effect", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node->setMaterialFlag( EMF_LIGHTING, false );
	node->setMD2Animation( scene::EMAT_STAND );
	node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	smgr->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 0.0f, 5.0f, 0.0f ) );

	// Interesting stuff (see previous example for full details)
	IPostProc* ppRenderer = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer, dimension2di( 1024, 512 ), PP_BLUR );

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		// Animate the blur amount:
		ppBlur->setParameters( sinf( device->getTimer( )->getTime( ) * 0.002f ) * 0.01f - 0.005f );

		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		ppBlur->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppBlur;
	delete ppRenderer;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_MANY_EFFECTS
// EXAMPLE 3: A series of effects (invert colours then blur)

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Combined Effects", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node->setMaterialFlag( EMF_LIGHTING, false );
	node->setMD2Animation( scene::EMAT_STAND );
	node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	smgr->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 0.0f, 5.0f, 0.0f ) );

	// Interesting stuff (see previous example for full details)
	IPostProc* ppRenderer = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	CEffectPostProc* ppInvert = new CEffectPostProc( ppRenderer, dimension2di( 1024, 512 ), PP_INVERT );
	// The second effect is made exactly the same way, but takes ppInvert as the input instead of ppRenderer
	CEffectPostProc* ppBlur = new CEffectPostProc( ppInvert, dimension2di( 1024, 512 ), PP_BLUR, 0.01f );

//	delete ppInvert;

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		ppBlur->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	// The effects can be deleted in any order
	delete ppBlur;
	delete ppInvert;
	delete ppRenderer;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_MIXING_SCENES
// EXAMPLE 4: Mixing scenes - creates 2 scene managers, then mixes them on the screen with an overlay

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Mixing Scenes", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node->setMaterialFlag( EMF_LIGHTING, false );
	node->setMD2Animation( scene::EMAT_STAND );
	node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	smgr->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 0.0f, 5.0f, 0.0f ) );
	// Set up the second scene, object & camera - this time, use a different texture and camera angle to show some change
	ISceneManager* smgr2 = smgr->createNewSceneManager( false );
	ISceneNode* rootnode2 = smgr2->getRootSceneNode( );
	IAnimatedMeshSceneNode* node2 = smgr2->addAnimatedMeshSceneNode( smgr2->getMesh( "sydney.md2" ), rootnode2 );
	node2->setMaterialFlag( EMF_LIGHTING, false );
	node2->setMD2Animation( scene::EMAT_STAND );
	node2->setMaterialTexture( 0, driver->getTexture("fireball.bmp") );
	smgr2->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 10.0f, 5.0f, 0.0f ) );

	// Interesting stuff (see previous example for full details)
	IPostProc* ppRenderer1 = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	IPostProc* ppRenderer2 = new CRendererPostProc( smgr2, dimension2di( 1024, 512 ), true, true, SColor( 255u, 0u, 0u, 0u ) );
	// PP_OVERLAY takes a multiplication parameter - setting this to 1.0 does a simple add, 2.0 doubles the second texture's luminosity, etc.
	CEffectPostProc* ppOverlay = new CEffectPostProc( ppRenderer1, dimension2di( 1024, 512 ), PP_OVERLAY, 2.0f );
	// The constructor can only take 1 input, but PP_OVERLAY needs 2, so set the other one now;
	ppOverlay->setInput( 1, ppRenderer2 );

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		// We only need to tell the final node to render - it will automatically render both scenes
		ppOverlay->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppRenderer1;
	delete ppRenderer2;
	delete ppOverlay;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_MIXING_DIFFERENT_RENDERS_OF_THE_SAME_SCENE
// EXAMPLE 5: Like above, but render the same scene twice, making changes each time

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Multiple Renders", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node->setMaterialFlag( EMF_LIGHTING, false );
	node->setMD2Animation( scene::EMAT_STAND );
	node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	smgr->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 0.0f, 5.0f, 0.0f ) );

	// Interesting stuff
	// This time, we make 2 renderers for the same scene
	IPostProc* ppRenderer1 = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	IPostProc* ppRenderer2 = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer2, dimension2di( 1024, 512 ), PP_BLUR, 0.005f );
	CEffectPostProc* ppOverlay = new CEffectPostProc( ppRenderer1, dimension2di( 1024, 512 ), PP_OVERLAY, 1.0f );
	ppOverlay->setInput( 1, ppBlur );

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );

		// Because we want to have each scene render differently, we need to render them manually between changes:
		node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
		ppRenderer1->preRender( ); // Note: preRender not render, and no NULL

		node->setMaterialTexture( 0, driver->getTexture("fireball.bmp") );
		// We could call ppRender2->preRender() here, but it isn't necessary since the overlay will do that for us anyway
		ppOverlay->render( NULL );

		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppOverlay;
	delete ppBlur;
	delete ppRenderer1;
	delete ppRenderer2;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_TRANSITIONS
// EXAMPLE 6: Transitions from one scene to another (you could also do transitions within the same scene using preRender as above)

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Transitions", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node->setMaterialFlag( EMF_LIGHTING, false );
	node->setMD2Animation( scene::EMAT_STAND );
	node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	smgr->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 0.0f, 5.0f, 0.0f ) );
	// Set up the second scene, object & camera - this time, use a different texture and camera angle to show some change
	ISceneManager* smgr2 = smgr->createNewSceneManager( false );
	ISceneNode* rootnode2 = smgr2->getRootSceneNode( );
	IAnimatedMeshSceneNode* node2 = smgr2->addAnimatedMeshSceneNode( smgr2->getMesh( "sydney.md2" ), rootnode2 );
	node2->setMaterialFlag( EMF_LIGHTING, false );
	node2->setMD2Animation( scene::EMAT_STAND );
	node2->setMaterialTexture( 0, driver->getTexture("fireball.bmp") );
	smgr2->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 10.0f, 5.0f, 0.0f ) );

	// Interesting stuff
	IPostProc* ppRenderer1 = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	IPostProc* ppRenderer2 = new CRendererPostProc( smgr2, dimension2di( 1024, 512 ), true, true, SColor( 255u, 0u, 0u, 0u ) );
	CTransitionPostProc* ppTransition = new CTransitionPostProc( ppRenderer1, ppRenderer2, dimension2di( 1024, 512 ), driver->getTexture("terrain-heightmap.bmp") );
//	ppTransition->setQuality( PPQ_CRUDE );

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		ppTransition->setBlend( sinf( device->getTimer( )->getTime( ) * 0.0005f - 2.0f ) * 2.0f + 0.5f );

		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		ppTransition->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppTransition;
	delete ppRenderer1;
	delete ppRenderer2;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_SPLITSCREEN
// EXAMPLE 7: Split screens with multiple scene managers

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Splitscreen", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node->setMaterialFlag( EMF_LIGHTING, false );
	node->setMD2Animation( scene::EMAT_STAND );
	node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	smgr->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 0.0f, 5.0f, 0.0f ) );
	// Set up the second scene, object & camera - this time, use a different texture and camera angle to show some change
	ISceneManager* smgr2 = smgr->createNewSceneManager( false );
	ISceneNode* rootnode2 = smgr2->getRootSceneNode( );
	IAnimatedMeshSceneNode* node2 = smgr2->addAnimatedMeshSceneNode( smgr2->getMesh( "sydney.md2" ), rootnode2 );
	node2->setMaterialFlag( EMF_LIGHTING, false );
	node2->setMD2Animation( scene::EMAT_STAND );
	node2->setMaterialTexture( 0, driver->getTexture("fireball.bmp") );
	smgr2->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 10.0f, 5.0f, 0.0f ) );

	// Interesting stuff
	IPostProc* ppRenderer1 = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	IPostProc* ppRenderer2 = new CRendererPostProc( smgr2, dimension2di( 1024, 512 ), true, true, SColor( 255u, 0u, 0u, 0u ) );
	// Splitscreen objects can be given up to 4 views. The rects at the end are optional - if not given, an arrangement will be created to fit the given number of views
	CSplitPostProc* ppSplit = new CSplitPostProc( ppRenderer1, ppRenderer2, dimension2di( 1024, 512 ), rect<f32>( -0.5f, -1.0f, 0.5f, 0.0f ), rect<f32>( -0.7f, 0.0f, 0.3f, 1.0f ) );
	ppSplit->setVoidColor( SColor( 255u, 128u, 128u, 128u ) ); // Change the colour shown where there are no views. Call setVoidColor() to use no color (slightly faster, but should only be used if the views entirely cover the screen)

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		ppSplit->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppRenderer1;
	delete ppSplit;
	delete ppRenderer2;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_DEPTH_OF_FIELD
// EXAMPLE 8: Depth of Field blurring
// This is the first example to require z-position in the alpha channel of the input
// This is accomplished by using special shaders when rendering the scene
// Obviously this limits the effects which are possible (no transparency, only basic lighting is supported, etc.)
// If you want to add these effects, you'll need to write your own shaders for them.

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	// To make things more interesting, we add many Sydneys and a textured floor this time
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Depth of Field", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMesh* mesh = smgr->getMesh("sydney.md2");

	// Create the required material. This is a simple shader which draws the texture with no lights.
	// Use GL_PLAIN1 / DX_PLANE1 or GL_PLAIN2 / DX_PLANE2 to support 1 or 2 basic point-lights
	u32 matid;
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
	switch( driver->getDriverType( ) ) {
		case EDT_OPENGL:
			matid = gpu->addHighLevelShaderMaterial( GL_V_MAT, "main", EVST_VS_1_1, GL_PLAIN, "main", EPST_PS_1_1, new CTexturesShaderCallback( ), EMT_SOLID, 1 );
			break;
		case EDT_DIRECT3D8:
		case EDT_DIRECT3D9:
		default:
			matid = gpu->addHighLevelShaderMaterial( DX_V_MAT, "main", EVST_VS_1_1, DX_PLAIN, "main", EPST_PS_2_0, new CTexturesShaderCallback( ), EMT_SOLID, 0 );
	}

	for( u8 x = 0u; x != 2u; ++ x )
		for( u8 i = 0u; i != 5u; ++ i ) {
			IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
			node->setMaterialFlag(EMF_LIGHTING, false);
			node->setMD2Animation(scene::EMAT_STAND);
			node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
			node->setMaterialType( (E_MATERIAL_TYPE) matid );
			node->setPosition( vector3df( -x * 20.0f, 0.0f, i * 40.0f ) );
		}
	ISceneNode* node2 = smgr->addMeshSceneNode( smgr->addHillPlaneMesh( "", dimension2df( 200.0f, 200.0f ), dimension2d<u32>( 10, 10 ), NULL, 0.0f, dimension2df( 0.0f, 0.0f ), dimension2df( 100.0f, 100.0f ) ) );
	node2->setMaterialFlag(EMF_LIGHTING, false);
	node2->setMaterialTexture( 0, driver->getTexture("terrain-heightmap.bmp") );
	node2->setMaterialType( (E_MATERIAL_TYPE) matid );
	node2->setPosition( vector3df( 0.0f, -22.0f, 0.0f ) );
	ICameraSceneNode* cam = smgr->addCameraSceneNode( 0, vector3df( 40.0f, 60.0f, -40.0f ), vector3df( 0.0f, 5.0f, 50.0f ) );

	IPostProc* ppRenderer = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	CEffectPostProc* ppBlurDOF = new CEffectPostProc( ppRenderer, dimension2di( 1024, 512 ), PP_BLURDOF );
	// We could set parameters on creation, but no need since we will animate it anyway.
	// Parameters are: near blur, near focus, far focus, far blur, blur level
	// You can also use PP_BLURDOFNEAR or PP_BLURDOFFAR to have only near or far blurring

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		// Change the camera angle
		cam->setTarget( vector3df( -(device->getCursorControl( )->getPosition( ).X - 320.0f) * 0.1f, (device->getCursorControl( )->getPosition( ).Y - 240.0f) * 0.2f, 0.0f ) );

		// Animate the depth of field:
		f32 p = sinf( device->getTimer( )->getTime( ) * 0.0005f ) * 0.5f - 0.2f;
		ppBlurDOF->setParameters( p * 100.0f + 80.0f, p * 100.0f + 110.0f, p * 100.0f + 160.0f, p * 100.0f + 240.0f, 0.01f );

		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		ppBlurDOF->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppBlurDOF;
	delete ppRenderer;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_HAZE
// EXAMPLE 9: Haze with depth dependence (more realistic than PP_HAZE)

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	// To make things more interesting, we add many Sydneys and a textured floor this time
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Heat Haze (with Depth dependence)", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMesh* mesh = smgr->getMesh("sydney.md2");
	ISceneManager* smgr2 = smgr->createNewSceneManager( false );
	ISceneNode* rootnode2 = smgr2->getRootSceneNode( );
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
	u32 matid, matid2;
	switch( driver->getDriverType( ) ) {
		case EDT_OPENGL:
			matid = gpu->addHighLevelShaderMaterial( GL_V_MAT, "main", EVST_VS_1_1, GL_PLAIN, "main", EPST_PS_1_1, new CTexturesShaderCallback( ), EMT_SOLID, 1 );
			matid2 = gpu->addHighLevelShaderMaterial( GL_V_MAT, "main", EVST_VS_1_1, GL_HEATSOURCE, "main", EPST_PS_1_1, new CTexturesShaderCallback( ), EMT_TRANSPARENT_ALPHA_CHANNEL, 1 );
			break;
		case EDT_DIRECT3D8:
		case EDT_DIRECT3D9:
		default:
			matid = gpu->addHighLevelShaderMaterial( DX_V_MAT, "main", EVST_VS_1_1, DX_PLAIN, "main", EPST_PS_2_0, new CTexturesShaderCallback( ), EMT_SOLID, 0 );
			matid2 = gpu->addHighLevelShaderMaterial( DX_V_MAT, "main", EVST_VS_1_1, DX_HEATSOURCE, "main", EPST_PS_2_0, new CTexturesShaderCallback( ), EMT_TRANSPARENT_ALPHA_CHANNEL, 0 );
	}
	for( u8 x = 0u; x != 2u; ++ x )
		for( u8 i = 0u; i != 5u; ++ i ) {
			IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
			node->setMaterialFlag(EMF_LIGHTING, false);
			node->setMD2Animation(scene::EMAT_STAND);
			node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
			node->setMaterialType( (E_MATERIAL_TYPE) matid );
			node->setPosition( vector3df( -x * 20.0f, 0.0f, i * 40.0f ) );
			ISceneNode* node2 = smgr2->addBillboardSceneNode( rootnode2, dimension2df( 30.0f, 80.0f ), vector3df( -x * 20.0f, 30.0f, i * 40.0f ) );
			node2->setMaterialFlag(EMF_LIGHTING, false);
			node2->setMaterialType( EMT_TRANSPARENT_ADD_COLOR );
			node2->setMaterialTexture( 0, driver->getTexture("fireball.bmp") );
			node2->setMaterialType( (E_MATERIAL_TYPE) matid2 );
		}
	ISceneNode* nodeF = smgr->addMeshSceneNode( smgr->addHillPlaneMesh( "", dimension2df( 200.0f, 200.0f ), dimension2d<u32>( 10, 10 ), NULL, 0.0f, dimension2df( 0.0f, 0.0f ), dimension2df( 100.0f, 100.0f ) ) );
	nodeF->setMaterialFlag(EMF_LIGHTING, false);
	nodeF->setMaterialTexture( 0, driver->getTexture("terrain-heightmap.bmp") );
	nodeF->setPosition( vector3df( 0.0f, -22.0f, 0.0f ) );
	nodeF->setMaterialType( (E_MATERIAL_TYPE) matid );
	ICameraSceneNode* cam1 = smgr->addCameraSceneNode( 0, vector3df( 40.0f, 60.0f, -40.0f ), vector3df( 0.0f, 5.0f, 50.0f ) );
	ICameraSceneNode* cam2 = smgr2->addCameraSceneNode( rootnode2, vector3df( 40.0f, 60.0f, -40.0f ), vector3df( 0.0f, 5.0f, 50.0f ) );

	IPostProc* ppRenderer1 = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	IPostProc* ppRenderer2 = new CRendererPostProc( smgr2, dimension2di( 1024, 512 ), true, true, SColor( 255u, 0u, 0u, 0u ) );
	CEffectPostProc* ppHaze = new CEffectPostProc( ppRenderer1, dimension2di( 1024, 512 ), PP_HAZEDEPTH );
	ppHaze->setInput( 1, ppRenderer2 );
	ppHaze->setTimer( device->getTimer( ) );

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		cam1->setPosition( vector3df( 40.0f, 60.0f - (device->getCursorControl( )->getPosition( ).Y - 240.0f) * 0.1f, -40.0f ) );
		cam1->setTarget( vector3df( -(device->getCursorControl( )->getPosition( ).X - 320.0f) * 0.1f, (device->getCursorControl( )->getPosition( ).Y - 240.0f) * 0.2f, 50.0f ) );
		cam2->setPosition( cam1->getPosition( ) );
		cam2->setTarget( cam1->getTarget( ) );
		cam2->setUpVector( cam1->getUpVector( ) );

		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		ppHaze->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppHaze;
	delete ppRenderer1;
	delete ppRenderer2;

	// Back to boring stuff
	device->drop();
	return 0;
}

#elif defined EXAMPLE_CUSTOM_EFFECT
// EXAMPLE 9: Custom post-processing effect (old monitor)

// Define the shader code
// Parameters:	randNum			= a random number from 0.0 to 1.0
//				time			= the current time in seconds (since app started)
//				v0-7			= the input parameters
//				gl_TexCoord[0]	= the texture coordinates to use at this point (x/y screen position in the range 0.0 to 1.0)
//				texture1-2		= the input textures (currently only 2 are supported, though this could be raised to 4)
//			Note: in directX, texture coords are in TEXCOORD0 and textures are register(s0) and register(s1), so you can use your own variables with these
#define GL_OLDMONITOR "uniform float randNum,time,v0;uniform sampler2D texture1;float rand(in float s){return sin(gl_TexCoord[0].x*(1009.0+s*10.0)+tan(gl_TexCoord[0].y*(1090.0+s*100.0)+tan(gl_TexCoord[0].x*111.0+s*10000.0))+s*4.0+gl_TexCoord[0].y*10000.0)*0.5+0.5;}float tear(in float y){return (y-0.8)*0.01/(y*y*y*y+2.0);}void main(void){float c=dot(texture2D(texture1,vec2(floor(gl_TexCoord[0].x*500.0)*0.002+tear((gl_TexCoord[0].y-1.2+mod(time*0.1,1.4))*50.0)*rand(randNum)*5.0*v0,floor(gl_TexCoord[0].y*200.0)*0.005+floor(fract(time*0.1+randNum*0.1)*1.1)*0.03*v0)),vec4(0.2,0.5,0.3,0.0));gl_FragColor=(texture2D(texture1,vec2(gl_TexCoord[0].x+tear(tan(gl_TexCoord[0].y*10.0+time*2.0))*v0,gl_TexCoord[0].y))*(1.0-0.7*v0)+c*0.7*v0+vec4(rand(randNum))*0.1*v0)*(1.0-(sin(gl_TexCoord[0].y*400.0)*0.05+(pow(abs(gl_TexCoord[0].x*2.0-1.0),4.0)+pow(abs(gl_TexCoord[0].y*2.0-1.0),4.0))*0.5)*v0);}"
#define DX_OLDMONITOR "sampler2D s;float randNum,time,v0;float rand(in float2 t,in float s){return frac(t.x*(1009.0+s*10.0)+tan(t.y*(1090.0+s*100.0)+t.x*111.0)+t.y*10000.0)-0.5;}float tear(in float y){return (y-0.8)*0.01/(y*y*y*y+2.0);}float4 main(float2 t:TEXCOORD0):COLOR{float c=dot(tex2D(s,float2(t.x+tear(frac(t.y-time*0.2)*70.0-35.0)*(rand(t,randNum)+0.5)*4.0,floor(t.y*200.0)*0.005)),float4(0.2,0.5,0.3,0.0));return tex2D(s,t)*(1.0-0.7*v0)+(c*0.7+float4(0.1,0.1,0.1,0.0)*rand(t,randNum))*(1.0-sin(t.y*400.0)*0.05-(pow(abs(t.x-0.5),4.0)+pow(abs(t.y-0.5),4.0))*8.0)*v0;}"

// This monitor effect is NOT supported, and has been known to have problems. I encourage you to use the officially supported PP_MONITOR effect instead in your programs.

// A much simpler shader; this will add a constant number (set by parameters) to the rgba channels
#define GL_A_SIMPLER_SHADER "uniform float v0;uniform sampler2D texture1;void main(void){gl_FragColor=texture2D(texture1,gl_TexCoord[0].xy)+v0;}"
#define DX_A_SIMPLER_SHADER "sampler2D s;float v0;float4 main(float2 t:TEXCOORD0):COLOR{return tex2D(s,t)+v0;}"

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Custom Effect (Old Monitor)", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( smgr->getMesh( "sydney.md2" ) );
	node->setMaterialFlag( EMF_LIGHTING, false );
	node->setMD2Animation( scene::EMAT_STAND );
	node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
	smgr->addCameraSceneNode( 0, vector3df( 40.0f, 30.0f, -40.0f ), vector3df( 0.0f, 5.0f, 0.0f ) );

	// Interesting stuff

	IPostProc* ppRenderer = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255u, 100u, 101u, 140u ) );
	CEffectPostProc* ppMine = new CEffectPostProc( ppRenderer, dimension2di( 1024, 512 ), GL_OLDMONITOR, DX_OLDMONITOR, EPST_PS_1_2, EPST_PS_2_0, EMT_SOLID, PPF_FROMCODE, 1.0f );
	// Notes:
	//	EPST_PS_1_2	shows the minimum version required to run your shader - usually EPST_PS_1_1 for simple things
	//	EMT_SOLID	can be changed to EMT_TRANSPARENT_ALPHA_CHANNEL, EMT_TRANSPARENT_ALPHA_CHANNEL_REF, etc. If this is done, the first texture is copied to the screen, then the second drawn on top using your shader. This is much faster than using 2 dynamic textures in one shader (not sure why) so use it when you can if you have 2 inputs
	//	1.0f		is where your parameters go
	// Remember to use setTimer if your effect uses the time variable:
	ppMine->setTimer( device->getTimer( ) );
	// I hate this next line, but it needs to be set correctly to prevent MAJOR slowdown in Irrlicht when running in Direct3D mode.
	// (if not called, your shader will have all parameters set to 0. If called with values too high, your app will slow significantly)
	// Note: this is ONLY needed with custom shaders
	ppMine->setRequiredVariables( 1, true, true ); // (number of input parameters, uses time?, uses randNum?)

	// These variables aren't important - they are just for showing the FPS
	wchar_t tmp[255]; u8 t = 0u;

	while( device->run( ) ) {
		// The monitor effect needs to know how strong it should be
		ppMine->setParameters( min_( 1.0f, device->getTimer( )->getTime( ) * 0.0002f ) );

		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		ppMine->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the current FPS
		if( ++ t == 30u ) { t = 0u; swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() ); device->setWindowCaption( tmp ); }
	}

	delete ppMine;
	delete ppRenderer;

	// Back to boring stuff
	device->drop();
	return 0;
}


#elif defined EXAMPLE_SANDBOX
// EXAMPLE 10: Could be anything
// This will show whatever I was last working on, I guess

int main( ) {
	// Boring stuff: set up the scene, object & camera as usual
	// To make things more interesting, we add many Sydneys and a textured floor this time
	IrrlichtDevice* device = createDevice( DRIVER, dimension2d<s32>( 640, 480 ), 16, false, false, false, 0 );
	IVideoDriver* driver = device->getVideoDriver( );
	ISceneManager* smgr = device->getSceneManager( );
	IGUIEnvironment* guienv = device->getGUIEnvironment( );
	device->getFileSystem( )->changeWorkingDirectoryTo( MEDIA_DIRECTORY );
	guienv->addStaticText( L"Sandbox", rect<s32>( 10, 10, 260, 22 ), true );
	IAnimatedMesh* mesh = smgr->getMesh("sydney.md2");
	IGPUProgrammingServices* gpu = driver->getGPUProgrammingServices( );
	u32 matid;
	switch( driver->getDriverType( ) ) {
		case EDT_OPENGL:
			matid = gpu->addHighLevelShaderMaterial( GL_V_MAT, "main", EVST_VS_1_1, GL_PLAIN, "main", EPST_PS_1_1, new CTexturesShaderCallback( ), EMT_SOLID, 1 );
			break;
		case EDT_DIRECT3D8:
		case EDT_DIRECT3D9:
		default:
			matid = gpu->addHighLevelShaderMaterial( DX_V_MAT, "main", EVST_VS_1_1, DX_PLAIN, "main", EPST_PS_2_0, new CTexturesShaderCallback( ), EMT_SOLID, 0 );
	}
	for( u8 x = 0u; x != 2u; ++ x )
		for( u8 i = 0u; i != 5u; ++ i ) {
			IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
			node->setMaterialFlag(EMF_LIGHTING, false);
			node->setMD2Animation(scene::EMAT_STAND);
			node->setMaterialTexture( 0, driver->getTexture("sydney.bmp") );
			node->setMaterialType( (E_MATERIAL_TYPE) matid );
			node->setPosition( vector3df( -x * 20.0f, 0.0f, i * 40.0f ) );
		}
	ISceneNode* nodeF = smgr->addTerrainSceneNode( "terrain-heightmap.bmp" );
	nodeF->setMaterialFlag(EMF_LIGHTING, false);
	nodeF->setMaterialTexture( 0, driver->getTexture("terrain-heightmap.bmp") );
	nodeF->setPosition( vector3df( -512.0f, -50.0f, -512.0f ) );
	nodeF->setScale( vector3df( 4.0f, 0.2f, 4.0f ) );
	nodeF->setMaterialType( (E_MATERIAL_TYPE) matid );
	ICameraSceneNode* cam1 = smgr->addCameraSceneNode( 0, vector3df( 0.0f, 0.0f, -40.0f ), vector3df( 0.0f, 5.0f, 50.0f ) );

	IPostProc* ppRenderer1 = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor( 255, 130u, 180u, 230u ) );
	CWaterPostProc* ppWater = new CWaterPostProc( ppRenderer1, dimension2di( 1024, 512 ), -22.0f );
	ppWater->setTimer( device->getTimer( ) );
	wchar_t tmp[255];
	u8 t = 0u;
	while( device->run( ) ) {
//		cam1->setPosition( vector3df( 40.0f, 60.0f - (device->getCursorControl( )->getPosition( ).Y - 240.0f) * 0.1f, -40.0f ) );
		cam1->setPosition( vector3df( (device->getCursorControl( )->getPosition( ).X - 320.0f) * 0.1f, (device->getCursorControl( )->getPosition( ).Y - 240.0f) * 0.1f, -40.0f ) );
//		cam1->setTarget( vector3df( (device->getCursorControl( )->getPosition( ).X - 320.0f) * 0.1f, (device->getCursorControl( )->getPosition( ).Y - 240.0f) * 0.1f, 0.0f ) );
//		cam1->setTarget( cam1->getPosition( ) + vector3df( 0.0f, 0.0f, 10.0f ) );
//		cam1->setTarget( vector3df( -(device->getCursorControl( )->getPosition( ).X - 320.0f) * 0.1f, (device->getCursorControl( )->getPosition( ).Y - 240.0f) * 0.2f, 50.0f ) );

		driver->beginScene( false, driver->getDriverType( ) == video::EDT_DIRECT3D9 );
		ppWater->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		if( ++ t == 30u ) {
			t = 0u;
			swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() );
			device->setWindowCaption( tmp );
		}
	}

	delete ppWater;
	delete ppRenderer1;

	// Back to boring stuff
	device->drop();
	return 0;
}

#endif
*/
