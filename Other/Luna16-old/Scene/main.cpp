#ifdef _IRR_WINDOWS_
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#pragma comment(lib, "Irrlicht.lib")
#include <windows.h>
#endif

#include <irrlicht.h>

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

// A custom shader used below
#define GL_OLDMONITOR "uniform float randNum,vecValue0,vecValue1;varying vec2 mTC;uniform sampler2D texture1;float rand(in float s){return sin(mTC.x*(1009.0+s*10.0)+tan(mTC.y*(1090.0+s*100.0)+tan(mTC.x*111.0+s*10000.0))+s*4.0+mTC.y*10000.0)*0.5+0.5;}float tear(in float y){return (y-0.8)*0.01/(y*y*y*y+2.0);}void main(void){float c=dot(texture2D(texture1,vec2(floor(mTC.x/0.002)*0.002+tear((mTC.y-1.2+mod(vecValue1*0.1,1.4))*50.0)*rand(randNum)*5.0*vecValue0,floor(mTC.y/0.005)*0.005+floor(mod(vecValue1*0.1+randNum*0.1,1.0)*1.1)*0.03*vecValue0)),vec4(0.2,0.5,0.3,0.0));gl_FragColor=(texture2D(texture1,vec2(mTC.x+tear(tan(mTC.y*10.0+vecValue1*2.0))*vecValue0,mTC.y))*(1.0-0.7*vecValue0)+c*0.7*vecValue0+vec4(rand(randNum))*0.1*vecValue0)*(1.0-(sin(mTC.y*400.0)*0.05+(pow(mTC.x*2.0-1.0,4.0)+pow(mTC.y*2.0-1.0,4.0))*0.5)*vecValue0);}"
#define DX_OLDMONITOR ""


int main( ) {
	// Boring generic stuff from the original example
	IrrlichtDevice *device = createDevice( video::EDT_OPENGL, dimension2d<s32>(640, 480), 16, false, false, false, 0);
	if (!device) return 1;
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	guienv->addStaticText(L"Hello World!", rect<s32>(10,10,260,22), true);
	IAnimatedMesh* mesh = smgr->getMesh("../../media/sydney.md2");
	if (!mesh)
		return 1;
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("../../media/sydney.bmp") );
	}
	smgr->addCameraSceneNode(0, vector3df(0,30,-40), vector3df(0,5,0));


	// Make a second scene manager. This is not needed unless you want to use the transition effect.
	ISceneManager* smgr2 = smgr->createNewSceneManager( false );
	ISceneNode* rootnode2 = smgr2->getRootSceneNode( );

	// Here we use exactly the same mesh but a different texture. We'll also set a different background.
	// Don't forget the rootnode2 bits! You can ommit them in the main scene but not in a sub-scene
	IAnimatedMeshSceneNode* node2 = smgr2->addAnimatedMeshSceneNode( mesh, rootnode2 );
	if (node2)
	{
		node2->setMaterialFlag(EMF_LIGHTING, false);
		node2->setMD2Animation(scene::EMAT_STAND);
		node2->setMaterialTexture( 0, driver->getTexture("../../media/fireball.bmp") );
	}
	smgr2->addCameraSceneNode(rootnode2, vector3df(0,30,-40), vector3df(0,5,0));

	// Interesting stuff:

	// Set up the postprocessing:
	// Create a renderer node to render the scene:
	// (scenemanager to render, size of output, clearBackBuffer, clearZBuffer, distanceColour)
	IPostProc* ppRenderer = new CRendererPostProc( smgr, dimension2di( 1024, 512 ), true, true, SColor(255,100,101,140) );

	// Create a blur effect
	// (input, size of output, effect type, optional parameters - in this case size of blur)
	CEffectPostProc* ppBlur = new CEffectPostProc( ppRenderer, dimension2di( 1024, 512 ), PP_BLUR, 0.01f );
	// See CEffectPostProc.h for a full list of built-in effects and their parameters

	// Set up the postprocessing on the second scene:
	IPostProc* ppRenderer2 = new CRendererPostProc( smgr2, dimension2di( 1024, 512 ), true, true, SColor(255,200,220,240) );

	// And let's add a filter to this scene too
	IPostProc* ppBloom2 = new CEffectPostProc( ppRenderer2, dimension2di( 1024, 512 ), PP_BLOOM ); // No parameters given, so some nice defaults will be used

	// Create a transition between the two scenes
	CTransitionPostProc* ppTrans = new CTransitionPostProc( ppBlur, ppBloom2, dimension2di( 1024, 512 ), driver->getTexture( "../../media/terrain-heightmap.bmp" ) );

	// Split-screen is also possible (to see this, remove the disable line)
	CSplitPostProc* ppSplit = new CSplitPostProc( ppTrans, ppBloom2, dimension2di( 1024, 512 ), rect<float>( -0.5f, -1.0f, 0.5f, 0.0f ), rect<float>( -0.7f, 0.0f, 0.3f, 1.0f ) );
	ppSplit->setVoidColor( SColor( 255, 128, 128, 128 ) ); // Change the colour shown where there are no views. Call setVoidColor() to use no color (slightly faster, but should only be used if the views entirely cover the screen)
	ppSplit->disable( );
	// The format is: CSplitPostProc( [inputs], dimension2di( render size ), [rects] ) where rects are in the range [-1,1] with -1 = top left, 1 = bottom right
	// Leaving rects blank will use some simple defaults based on the number of inputs

	// You can delete any node at any time without breaking the chain. For example, I could add
//	delete ppBloom2;
	// to remove the bloom filter. The second scene will still render.

	// As well as built-in shaders, you can use your own - this one is defined at the top of this file; (remove the disable line to see an old monitor effect!)
	CEffectPostProc* ppMon = new CEffectPostProc( ppSplit, dimension2di( 1024, 512 ), GL_OLDMONITOR, DX_OLDMONITOR, EPST_PS_2_0, EMT_SOLID, 1.0f );
	ppMon->disable( );

	// Set a variable to the last node, so that it's easy to keep track of
	IPostProc* ppFin = ppMon;

	wchar_t tmp[255];
	u8 t = 0u;
	while(device->run())
	{
		// Change the blur amount. setParameters lets you change the values exactly the same as the initial constructor
		// It even keeps track of composite effects like bi-directional blur and bloom, changing the appropriate parts.
		ppBlur->setParameters( sinf( device->getTimer( )->getTime( ) * 0.005f ) * 0.008f + 0.002f );

		// Change the transition amount. You don't need to worry about capping the value, or optimizing the rendering.
		// If this is <= 0.0, the first scene is shown and the second not rendered. If >= 1.0, the second is shown and the first not rendered.
		ppTrans->setBlend( sinf( device->getTimer( )->getTime( ) * 0.0005f - 2.0f ) * 2.0f + 0.5f );

		// Update the OLDMONITOR effect. Even though it's disabled, it will remember these values and use them if enabled later.
		ppMon->setParameters( fminf( 1.0, device->getTimer( )->getTime( ) * 0.0002f ), device->getTimer( )->getTime( ) * 0.001f );

		driver->beginScene( false, false );
		// Render the postprocessing. Note that NULL can be replaced with any texture, or ommited.
		// (If ommited, will render into an internal texture with size given on creation. You can get this with ppFin->getOutputTexture( ) )
		ppFin->render( NULL );
		guienv->drawAll( );
		driver->endScene( );

		// Show the FPS, for testing mainly
		if( t ++ == 30u ) {
			t = 0u;
			swprintf(tmp, 255, L"%ls fps:%3d", driver->getName(), driver->getFPS() );
			device->setWindowCaption( tmp );
		}
	}

	device->drop();

	return 0;
}
