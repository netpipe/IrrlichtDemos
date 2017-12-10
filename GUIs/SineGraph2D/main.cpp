
#include <irrlicht.h>
#include "SGraph2D.h"


using namespace irr;
using namespace gui;

void plotsine( SGraph2D* );

void plotsine( SGraph2D* win )
{
Inc<f32> dis;
core::vector2df pt;

//dis.copy( win->getIterableXRange() );
dis.setStep( 1.0f/1000.0f );

do {
pt.X = dis.Val();
pt.Y = 10 * sin( dis.Val() );

win->drawOnGraph( pt, video::SColor(255,255,0,0) );
} while ( !++dis );
}

int main()
{
IrrlichtDevice* device = createDevice();
video::IVideoDriver* vid = device->getVideoDriver();
IGUIEnvironment* envir = device->getGUIEnvironment();

SGraph2D* graph = new SGraph2D(
envir, // GUI environment
envir->getRootGUIElement(), // parent
0, // id
irr::core::recti(0,0,600,400), // GUI element boundaries
irr::core::rectf( -10.0f, -10.0f, 20.0f, 10.0f ), // graph window
true, // use marks or lines?
true // use tick marks?
);



graph->setBackgroundColor( video::SColor(255,150,150,150) );

plotsine( graph );

graph->drop();
graph = 0;

while( device->run() )
{
vid->beginScene();
envir->drawAll();
vid->endScene();
}

device->drop();
}
