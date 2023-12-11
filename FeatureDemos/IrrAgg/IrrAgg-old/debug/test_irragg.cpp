// Copyright 2017 Nicolaus Anderson
#include "../include/irragg.h"
#include <irrlicht.h>

bool test_render_triangle( irr::video::IImage*  backbuffer );
//bool test_render_array( irr::video::IImage*  backbuffer );
bool test_render_static_array( irr::video::IImage*  backbuffer );
bool test_render_irr_array( irr::video::IImage*  backbuffer );
bool test_render_ellipse( irr::video::IImage*  backbuffer );
bool test_render_irr_array2( irr::video::IImage*  backbuffer );
bool test_render_rounded_rect1( irr::video::IImage*  backbuffer );
bool test_render_rounded_rect2( irr::video::IImage*  backbuffer );
bool test_render_rounded_rect3( irr::video::IImage*  backbuffer );
bool test_rendererARGB32( irr::video::IImage*  backbuffer );


class Clover : public irr::vecg::PointSource
{
public:
	Clover()
	{
		//
	}

	virtual unsigned
		getPointCount() const
	{
		return 5;
	}

	virtual irr::vecg::Point2D
		getPoint( unsigned  step )
	{
		switch(step) {
		case 0: return irr::vecg::Point2D(50, 30);
		case 1: return irr::vecg::Point2D(200, 50);
		case 2: return irr::vecg::Point2D(150, 300);
		case 3: return irr::vecg::Point2D(80, 200);
		default: return irr::vecg::Point2D(100,100);
		}
	}
};


int main()
{
	irr::video::SColor  white(0xffffffff);
	irr::core::vector2di  zero_vector(0);
	irr::core::dimension2du  screen_size(512,512);
	irr::IrrlichtDevice*  device = irr::createDevice( irr::video::EDT_BURNINGSVIDEO, screen_size );

	if ( !device )
		return 1;

	irr::video::IVideoDriver*  video_driver = device->getVideoDriver();
	irr::video::IImage*  image = video_driver->createImage( irr::video::ECF_A8R8G8B8, screen_size );
	irr::video::ITexture*  texture = 0;

	// Should pick from an options list here

	test_render_triangle( image );
	//test_render_array( image );
	test_render_static_array( image );
	//test_render_irr_array( image );
	test_render_ellipse( image );
	test_render_irr_array2( image );
	test_render_rounded_rect1( image );
	test_render_rounded_rect2( image );
	test_render_rounded_rect3( image );
	test_rendererARGB32( image );

	texture = video_driver->addTexture("texture0", image);

	irr::core::recti texture_source_rect(zero_vector, irr::core::dimension2di(texture->getSize()));

	while ( device->run() ) {
		video_driver->beginScene(true, true);
		video_driver->draw2DImage(texture, zero_vector);
		//video_driver->draw2DImage(texture, zero_vector, texture_source_rect, 0, white, true);
		video_driver->endScene();
	}

	image->drop();
	device->drop();

	return 0;
}


bool
test_render_triangle(
	irr::video::IImage*  backbuffer
) {
	irr::video::SColor  color(255,0,0,255);
	return irr::vecg::renderTriangle(backbuffer, 30,30, 400,30, 220,480, color);
}

bool test_render_static_array(
	irr::video::IImage*  backbuffer
) {
	irr::video::SColor  color(0xffffffff);
	irr::vecg::StaticSizeVertexSource<3>  path;
	path
		.set(0, 30, 200)
		.set(1, 400, 100)
		.set(2, 300, 200);

	return irr::vecg::renderVectorPath(path, backbuffer, color, irr::vecg::EPathStroke::NORMAL);
}


bool test_render_irr_array(
	irr::video::IImage*  backbuffer
) {
	irr::video::SColor  color(0xff00ff00);

	irr::vecg::ArrayVertexSource path;
	path
		.add( 10, 100 )
		.add( 500, 200 )
		.add( 200, 300 )
		.add( 300, 400 )
		.add( 250, 400 );

	return irr::vecg::renderVectorPath(path, backbuffer, color);
}

bool test_render_ellipse(
	irr::video::IImage*  backbuffer
) {
	irr::video::SColor  color(0xffffff00);

	return irr::vecg::renderEllipse(backbuffer, 250, 250, 50, 100, color);
}

bool test_render_irr_array2(
	irr::video::IImage*  target
) {
	irr::video::SColor  color(255,255,0,0); // Red
	irr::f32  width = 2; // width of rendered curve
	irr::u32  resolution = 20; // number of segments between points for a bspline curve

	static const double  x_values[] = { 10, 100, 200, 300, 400, 500 };
	static const double  y_values[] = { 50, 200, 350, 400, 200, 100 };

	// Using an irragg structure
	irr::vecg::ArrayVertexSource  vertex_source(
		5, // number of points
		(const double*)&x_values,
		(const double*)&y_values
	);

	// Using an irragg function
	return irr::vecg::renderVectorPath(vertex_source, target, color, irr::vecg::EPathStroke::BSPLINE, width, resolution);
}

bool test_render_rounded_rect1(
	irr::video::IImage*  backbuffer
) {
	irr::video::SColor  color(255,255,125,0); // Orange

	return irr::vecg::renderRoundedRectangle(backbuffer, 450, 60, 490, 120, 2, color);
}

bool test_render_rounded_rect2(
	irr::video::IImage*  backbuffer
) {
	irr::video::SColor  color(255,255,125,0); // Orange

	return irr::vecg::renderRoundedRectangle(backbuffer, 450, 127, 490, 187, 2, 4, 5, 7, color);
}

bool test_render_rounded_rect3(
	irr::video::IImage*  backbuffer
) {
	irr::video::SColor  color(255,255,125,0); // Orange

	return irr::vecg::renderRoundedRectangle(
				backbuffer,
				450, 200, 490, 380,
				20, 10, 5, 7,
				10, 11, 12, 13,
				color);
}

bool test_rendererARGB32(
	irr::video::IImage*  backbuffer
) {
	irr::vecg::engine::RendererARGB32  renderer;

	typedef irr::core::vector2d<double> vector2dd;

	renderer.setRenderTarget(backbuffer, false);
	renderer.setDrawingColor( irr::video::SColor(0x99ff0000) );

	renderer.drawTriangle(
		vector2dd(150,10),
		vector2dd(300,150),
		vector2dd(30,150)
		);

	renderer.setBSplineResolution(10);
	renderer.setDrawingMatrixEnabled(true);
	renderer.setDrawingMatrix(1.0, 0.3, 0, 1.0);

	renderer.drawCircle(
		150,
		vector2dd(300,300),
		irr::vecg::EPathStroke::BSPLINE
		);

	renderer.setDrawingColor( irr::video::SColor(0x9900ff00) );
	renderer.setStrokeWidth(5);
	//renderer.setDrawingMatrixEnabled(true);
	//renderer.setDrawingMatrix(1.0, 0.3, 0, 1.0);
	//renderer.setDrawingMatrixTranslation(20,20);

	renderer.drawRectangle(
		vector2dd(0,400),
		vector2dd(90,450),
		irr::vecg::EPathStroke::BRUSH
		);

	Clover  clover;
	renderer.bufferPoints(&clover);
	renderer.setDrawingMatrixEnabled(false);
	renderer.setStrokeWidth(10);
	renderer.setDrawingColor( irr::video::SColor(0x99ff00ff) );
	renderer.draw( irr::vecg::EPathStroke::BRUSH );

	return true;
}
