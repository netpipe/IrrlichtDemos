// Created 2017 Nicolaus Anderson
// This code is hereby released as public domain

#include <irrlicht.h>
#include "../../include/irragg.h"


int main()
{
	TrianglePath my_vs;
	my_vs.point_a.set(10, 0);
	my_vs.point_b.set(20, 10);
	my_vs.point_c.set(30, -10);

	// create Irrlicht device
	irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_BURNINGS, irr::core::dimension2du(0));

	// create image
	irr::core::dimension2du  image_size(500,500);
	irr::video::IVideoDriver*  video_driver = device->getVideoDriver();
	irr::video::IImage*  image = video_driver->createImage(irr::video::ECF_A8R8G8B8, image_size);

	// render our shape
	bool success = irr::agg::renderVectorPath( my_vs, image );

	// variables for rendering
	irr::video::ITexture*  texture;
	irr::video::SColor  white(0xffffffff);
	irr::core::vector2di  zero_vector(0);
	irr::core::recti  source_rect(0,0, (irr::s32) image_size.Width, (irr::s32) image_size.Height);

	// render
	if ( success )
	{
		texture = video_driver->addTexture( irr::io::path("texture_name"), image );

		while ( device->run ) {
			video_driver->beginScene();
			video_driver->draw2DImage( texture, zero_vector, source_rect, 0, white, false );
			video_driver->endScene();
		}
	}

	// clean up
	device->drop();

	return 0;
}
