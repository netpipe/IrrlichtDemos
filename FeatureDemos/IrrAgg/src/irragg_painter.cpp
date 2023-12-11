// (c) 2019 Nicolaus Anderson
// See license.txt for details.

#include "../include/irragg_painter.h"
#include "../include/irragg_color.h"

namespace irr {
namespace vecg {

Painter::Painter( video::IVideoDriver*  driver, const core::dimension2du&  size )
	: Ready(false)
	, Size(size)
	, Canvas(0)
	, Color(0,0,0,1)
	, StrokeWidth(1)
	, BSplineStep(1.0/20)
	, Step(0)
	, Shape(0)
	, Buffer()
	, Format()
	, Base()
{
	Canvas = driver->createImage(video::ECF_A8R8G8B8, size);
	prepareCanvas();
}

Painter::Painter( video::IImage*  image )
	: Ready(false)
	, Size(0,0)
	, Canvas(image)
	, Color(0,0,0,1)
	, StrokeWidth(1)
	, BSplineStep(1.0/20)
	, Step(0)
	, Shape(0)
	, Buffer()
	, Format()
	, Base()
{
	if ( Canvas ) {
		Canvas->grab();
		prepareCanvas();
	}
}

Painter::~Painter()
{
	if ( Canvas )
		Canvas->drop();
}

void
Painter::setCanvas( video::IImage*  image ) {
	Ready = false;
	if ( image )
		image->grab();
	if ( Canvas )
		Canvas->drop();
	Canvas = image;
	if ( Canvas ) {
		prepareCanvas();
	}
}

video::IImage*
Painter::getCanvas()
{
	return Canvas;
}

void
Painter::setColor( const video::SColor&  color )
{
	Color = core::convertSColorToAGGrgba(color);
}

video::SColor
Painter::getColor()
{
	return core::convertAGGrgbaToSColor(Color);
}

void
Painter::setStrokeWidth( double  width )
{
	StrokeWidth = width;
}

void
Painter::setBSplineResolution( u32  resolution )
{
	BSplineStep = 1.0 / resolution;
}

bool
Painter::drawFilled( const IShape&  shape )
{
	if ( ! Ready )
		return false;

	Shape = &shape;
	rewind(0);

	::agg::scanline_p8  scanline;
	::agg::rasterizer_scanline_aa<>  rasterizer;

	rasterizer.add_path(*this);
	::agg::render_scanlines_aa_solid(
		rasterizer, scanline, Base, Color
		);

	Shape = 0; // Cleanup pointer
	return true;
}

bool
Painter::drawStroked( const IShape&  shape )
{
	if ( ! Ready )
		return false;

	Shape = &shape;
	rewind(0);

	::agg::scanline_p8  scanline;
	::agg::rasterizer_scanline_aa<>  rasterizer;

	::agg::conv_stroke< Painter >  conv_stroke_vs(*this);
	conv_stroke_vs.width( StrokeWidth );

	rasterizer.add_path( conv_stroke_vs );
	::agg::render_scanlines_aa_solid(
		rasterizer, scanline, Base, Color
		);

	Shape = 0; // Cleanup pointer
	return true;
}

bool
Painter::drawBSpline( const IShape&  shape )
{
	if ( ! Ready )
		return false;

	Shape = &shape;
	rewind(0);

	::agg::scanline_p8  scanline;
	::agg::rasterizer_scanline_aa<>  rasterizer;

	bspline_type  vs_bspline(*this);
	vs_bspline.interpolation_step(BSplineStep);

	::agg::conv_stroke< bspline_type >  vs_stroke(vs_bspline);
	vs_stroke.width( StrokeWidth );

	rasterizer.add_path(vs_stroke);
	::agg::render_scanlines_aa_solid(
		rasterizer, scanline, Base, Color
		);

	Shape = 0; // Cleanup pointer
	return true;
}

bool
Painter::drawBSplineFilled( const IShape&  shape )
{
	if ( ! Ready )
		return false;

	Shape = &shape;
	rewind(0);

	::agg::scanline_p8  scanline;
	::agg::rasterizer_scanline_aa<>  rasterizer;

	bspline_type  vs_bspline(*this);
	vs_bspline.interpolation_step(BSplineStep);

	rasterizer.add_path(vs_bspline);
	::agg::render_scanlines_aa_solid(
		rasterizer, scanline, Base, Color
		);

	Shape = 0; // Cleanup pointer
	return true;
}

void
Painter::prepareCanvas()
{
	if ( ! Canvas )
		return;
	if ( Canvas->getColorFormat() != video::ECF_A8R8G8B8 )
		return;

	irr::u8*  data = (irr::u8*)Canvas->getData();
	Size = Canvas->getDimension();
	Buffer.attach( data, Size.Width, Size.Height, Canvas->getPitch() );
	Format.attach(Buffer);
	Base.attach(Format);

	Ready = true;
}

// AGG -----------------------------
void
Painter::rewind(unsigned)
{
	Step = 0;
}

unsigned
Painter::vertex( double* x, double* y )
{
	if ( Step >= Shape->getPathStepCount() )
		return ::agg::path_cmd_stop;
	// NOTE: Cannot return ::agg::path_cmd_end_poly because it resets to zero

	PathMotion  p = Shape->getPathMotion( Step );
	*x = p.X;
	*y = p.Y;
	++Step;

	if ( Step == 1 )
		return ::agg::path_cmd_move_to;

	//return ::agg::path_cmd_line_to;
	return p.Motion;
}

} // end namespace vecg
} // end namespace irr
