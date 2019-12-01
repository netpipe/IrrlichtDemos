// Copyright 2019 Nicolaus Anderson
#include "../include/irragg.h"
#include <irrlicht.h>

using irr::vecg::point_t;
using irr::vecg::Painter;
using irr::vecg::IShape;
using irr::vecg::Triangle;
using irr::vecg::Rectangle;
using irr::vecg::GhostArray;


static const point_t  premadePoints[] = {
	{ 50, 40 },
	{ 100, 40 },
	{ 250, 230 },
	{ 120, 170 },
	{ 75, 200 }
};


int main()
{
	irr::core::vector2di  zero_vector(0);
	irr::core::dimension2du  screen_size(512,512);
	//irr::IrrlichtDevice*  device = irr::createDevice( irr::video::EDT_BURNINGSVIDEO, screen_size );
	irr::IrrlichtDevice*  device = irr::createDevice( irr::video::EDT_OPENGL, screen_size );
	if ( !device ) return 1;

	irr::video::IVideoDriver*  video_driver = device->getVideoDriver();
	irr::video::IImage*  image = video_driver->createImage( irr::video::ECF_A8R8G8B8, screen_size );
	irr::video::ITexture*  texture = 0;

	Triangle  triangle( point_t(10,500), point_t(250, 30), point_t(500, 200) );
	Rectangle  rectangle( point_t(20, 40), point_t(400, 300) );
	Painter  painter(image);
	painter.setColor( irr::video::SColor(0xffff0000) );

	if ( ! painter.drawFilled(triangle) ) {
		image->drop();
		device->drop();
		return 1;
	}

	painter.setColor( irr::video::SColor(0xff00ff00) );

	if ( ! painter.drawStroked(rectangle) ) {
		image->drop();
		device->drop();
		return 1;
	}

	painter.setStrokeWidth(2);
	painter.setColor( irr::video::SColor(0xff0000ff) );

	//const size_t  size = 20;
	//point_t  points[size];
	//for ( size_t i = 0; i < size; ++i ) {
	//	points[i] = point_t( 20.0 * ((i*3)%13) + 50, 40.0 * ((i+1)%4) + 50 );
	//}

	//GhostArray  ghostArray(premadePoints, 5);
	//GhostArray  ghostArray(points, size);
	//if ( ! painter.drawBSpline( ghostArray ) ) {
	if ( ! painter.drawBSpline( GhostArray{premadePoints, 5} ) ) {
		image->drop();
		device->drop();
		return 1;
	}

	const size_t  size = 7;
	point_t  points[size];
	unsigned i = 0;
	points[i++] = point_t(40,300);
	points[i++] = point_t(80,400);
	points[i++] = point_t(240,420);
	points[i++] = point_t(70,480);
	points[i++] = point_t(25,350);
	points[i++] = point_t(30,330);
	points[i++] = point_t(30,305);

	if ( ! painter.drawBSplineFilled( GhostArray{points, size} ) ) {
		image->drop();
		device->drop();
		return 1;
	}

	texture = video_driver->addTexture("texture0", image);

	while ( device->run() ) {
		video_driver->beginScene(true, true);
		video_driver->draw2DImage(texture, zero_vector);
		video_driver->endScene();
	}

	image->drop();
	device->drop();
	return 0;
}
