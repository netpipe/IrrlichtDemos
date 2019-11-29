#include "2dGraphicsFactory.h"

using namespace phoenix;

////////////////////////////////////////////////////////////////////////////////
//Draws a line
////////////////////////////////////////////////////////////////////////////////

BatchGeometryPtr GraphicsFactory2d::drawLine(const Vector2d& _v1, const Vector2d& _v2, const Color& _a, const Color& _b)
{
    // Just make some new geometry, set it to immediate, and add the line's vertices.
    BatchGeometryPtr linegeom = new BatchGeometry( renderer, GL_LINES, getTexture(), getGroup(), getDepth() );
	linegeom->setImmediate( true );
	linegeom->addVertex( Vertex( _v1, _a, TextureCoords(0,0) ) );
	linegeom->addVertex( Vertex( _v2, _b, TextureCoords(1,1) ) );

    return linegeom;

}

////////////////////////////////////////////////////////////////////////////////
//Draws a rectangle
////////////////////////////////////////////////////////////////////////////////

BatchGeometryPtr GraphicsFactory2d::drawRectangle( const Rectangle& _r, const Color& _a, const Color& _b, const Color& _c, const Color& _d )
{
    // Use BatchGeometry's factory for it.
	BatchGeometryPtr rectgeom = new BatchGeometry( renderer, _r, getTexture(), getGroup(), getDepth() );
	rectgeom->setImmediate( true );

    // now just set the colors.
	(*rectgeom)[0].color = _a;
	(*rectgeom)[1].color = _b;
	(*rectgeom)[2].color = _c;
	(*rectgeom)[3].color = _d;

    return rectgeom;
}

////////////////////////////////////////////////////////////////////////////////
//Draws a polygon
////////////////////////////////////////////////////////////////////////////////

BatchGeometryPtr GraphicsFactory2d::drawPolygon (const Polygon& _p, const Color& _c)
{
    // just use the BatchGeometry's factory
	BatchGeometryPtr polygeom = new BatchGeometry( renderer, _p, getTexture(), getGroup(), getDepth());
	polygeom->setImmediate( true );
	polygeom->colorize( _c );
    return polygeom;
}

////////////////////////////////////////////////////////////////////////////////
//Draws a polygon
////////////////////////////////////////////////////////////////////////////////

BatchGeometryPtr GraphicsFactory2d::drawTexturedPolygon (const Polygon& _p, TexturePtr _t, const Color& _c, bool _e)
{

    // Use the regular draw poly function to save us effort.
    setTexture( _t );
    BatchGeometryPtr polygeom = drawPolygon( _p, _c );
    setTexture();

    // planes for coordinate generation
    Vector2d splane( 1.0f/_t->getWidth(), 0);
    Vector2d tplane( 0, 1.0f/_t->getHeight() );

    // Go through each vertex on the geometry and generate the Tcoords.
    for( unsigned int i = 0; i < polygeom->getVertexCount(); ++i )
    {
        // We'll need the current vertex's position to calculate the tcoords.
        Vector2d v = (*polygeom)[i].position;
        // if we're generating object space coordinates, subtract the polygon's position from it.
        if( ! _e )
            v -= _p.getPosition();

        // Now it's as simple as the inner product of the planes and the vector.
        (*polygeom)[i].tcoords = TextureCoords( splane * v, tplane * v );
    }

    return polygeom;

}


////////////////////////////////////////////////////////////////////////////////
//Renders a Texture
////////////////////////////////////////////////////////////////////////////////

BatchGeometryPtr GraphicsFactory2d::drawTexture(  TexturePtr _t, const Vector2d& _p,  const RotationMatrix& _rot, const Vector2d& _scale, const Color& _color, unsigned int _flags )
{
    // Use BatchGeometry's factory for rectangles.
	BatchGeometryPtr geom = new BatchGeometry( renderer, Rectangle( -_t->getSize()/2.0f, _t->getSize()) , _t, getGroup(), getDepth() );
    geom->setImmediate( true );

    // scale, rotate it, and then translate it.
    geom->scale( _scale );
    geom->rotate( _rot );
    geom->translate( _p + _t->getSize()/2.0f );

    // colorize
    geom->colorize( _color );

    // Flip tcoords
    if( _flags & EGF_HFLIP )
    {
        (*geom)[0].tcoords.u = ( - (*geom)[0].tcoords.u ) + 1.0f; 
        (*geom)[1].tcoords.u = ( - (*geom)[1].tcoords.u ) + 1.0f; 
        (*geom)[2].tcoords.u = ( - (*geom)[2].tcoords.u ) + 1.0f; 
        (*geom)[3].tcoords.u = ( - (*geom)[3].tcoords.u ) + 1.0f; 
    }
    if( _flags & EGF_VFLIP )
    {
        (*geom)[0].tcoords.v = ( - (*geom)[0].tcoords.v ) + 1.0f; 
        (*geom)[1].tcoords.v = ( - (*geom)[1].tcoords.v ) + 1.0f; 
        (*geom)[2].tcoords.v = ( - (*geom)[2].tcoords.v ) + 1.0f; 
        (*geom)[3].tcoords.v = ( - (*geom)[3].tcoords.v ) + 1.0f; 
    }

    // return it
    return geom;
}

//this draws a texture with a clipping rectangle
BatchGeometryPtr GraphicsFactory2d::drawTexturePart( TexturePtr _t, const Vector2d& _p, const Rectangle& _rect, const RotationMatrix& _rot, const Vector2d& _scale, const Color& _color, unsigned int  _flags )
{
    // Use BatchGeometry's factory for rectangles.
    BatchGeometryPtr geom = new BatchGeometry( renderer, Rectangle( -_rect.getSize()/2, _rect.getSize() ) , _t, getGroup(), getDepth() );
    geom->setImmediate( true );

    // scale, rotate it, and then translate it.
    geom->scale( _scale );
    geom->rotate( _rot );
    geom->translate( _p + _rect.getSize()/2.0f );

    // colorize
    geom->colorize( _color );

    // Define tcoords.
    Vector2d originuv( _rect.getX()/_t->getWidth(), _rect.getY()/_t->getHeight() );
    Vector2d spanuv( _rect.getWidth()/_t->getWidth(), _rect.getHeight()/_t->getHeight() );

    (*geom)[3].tcoords = TextureCoords( (originuv+spanuv).getX(), originuv.getY() );
    (*geom)[2].tcoords = TextureCoords( (originuv+spanuv).getX(), (originuv+spanuv).getY() );
    (*geom)[1].tcoords = TextureCoords( originuv.getX(), (originuv+spanuv).getY()  );
    (*geom)[0].tcoords = TextureCoords( originuv.getX(), originuv.getY() );

    // Flip tcoords
    if( _flags & EGF_HFLIP )
    {
        (*geom)[0].tcoords.u = ( - (*geom)[0].tcoords.u ) + 1.0f; 
        (*geom)[1].tcoords.u = ( - (*geom)[1].tcoords.u ) + 1.0f; 
        (*geom)[2].tcoords.u = ( - (*geom)[2].tcoords.u ) + 1.0f; 
        (*geom)[3].tcoords.u = ( - (*geom)[3].tcoords.u ) + 1.0f; 
    }
    if( _flags & EGF_VFLIP )
    {
        (*geom)[0].tcoords.v = ( - (*geom)[0].tcoords.v ) + 1.0f; 
        (*geom)[1].tcoords.v = ( - (*geom)[1].tcoords.v ) + 1.0f; 
        (*geom)[2].tcoords.v = ( - (*geom)[2].tcoords.v ) + 1.0f; 
        (*geom)[3].tcoords.v = ( - (*geom)[3].tcoords.v ) + 1.0f; 
    }

    // return it
    return geom;
}
