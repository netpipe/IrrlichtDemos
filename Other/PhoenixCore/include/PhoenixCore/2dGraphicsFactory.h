/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PH_2D_GFX_FACTORY_H__
#define __PH_2D_GFX_FACTORY_H__

#include "config.h"
#include "AbstractGeometryFactory.h"
#include "BatchRenderer.h"

namespace phoenix
{

//! Graphics Factory Flags
enum E_GEOMETRY_FLAGS
{
    EGF_NONE = 0x0000,
    EGF_HFLIP = 0x0001, //!< Horizontal Flip
    EGF_VFLIP = 0x0002 //!< Vertical Flip
};

//! Factory for creating common 2d graphics.
/*!
    This class provides a factory for creating common 2d graphics
    such as lines, rectangles, polygons, texture rectangles and polygons,
    etc. It is one of the base classes for RenderSystem. All geometry
    created by this factory is immediate, but can be set persistent.
    \sa RenderSystem
*/
class GraphicsFactory2d
    : public virtual AbstractGeometryFactory
{

public:

    //! Constructor
    /*!
        Makes a new factory and ties it to BatchRenderer.
    */
    GraphicsFactory2d( BatchRenderer& _r )
        : AbstractGeometryFactory(), renderer( _r )
    {
    }

    //! Destructor
    virtual ~GraphicsFactory2d()
    {}

    //! Draws a 2D line segment with a color for each end.
	/*!
        This is a geometry factory.
		\param _v1 The vector to the first point of the line.
		\param _v2 The vector to the second point of the line.
		\param _a Color of the first vertex.
		\param _b Color of the second vertex.
		\sa drawRay()
	*/
	BatchGeometryPtr drawLine(const Vector2d& _v1 = Vector2d(0,0), const Vector2d& _v2 = Vector2d(0,0), const Color& _a = Color(255,255,255), const Color& _b = Color(255,255,255,255));

	//! Draws a 2D ray with a color for each end starting from the origin.
	/*!
        This is a geometry factory.
		\param _o The Point of origin.
		\param _r The Ray.
		\param _a Color of the start of the ray.
		\param _b Color of the end of the ray.
		\sa drawLine()
	*/
	inline BatchGeometryPtr drawRay(const Vector2d& _o = Vector2d(0,0), const Vector2d& _r = Vector2d(0,0), const Color& _a = Color(255,255,255), const Color& _b = Color(255,255,255,255))
    {
        return drawLine( _o, _o+_r, _a, _b );
    }

	//! Draw rectangle.
    /*!
        Draws a rectangle with a color for each corner. This is a geometry factory.
        \param _r The rectangle to be drawn.
        \param _a Color of the top-left corner.
        \param _b Color of the top-right corner.
        \param _c Color of the bottom-right corner.
        \param _d Color of the bottom-left corner.
    */
    BatchGeometryPtr drawRectangle( const Rectangle& _r = Rectangle(0,0,0,0), const Color& _a = Color(255,255,255), const Color& _b = Color(255,255,255), const Color& _c = Color(255,255,255), const Color& _d = Color(255,255,255) );

    //! Draws a polygon.
    /*!
        Simply draws a polygon with the given depth and color. This is a geometry factory.
    */
    BatchGeometryPtr drawPolygon (const Polygon& _p, const Color& _a = Color(255,255,255));

    //! Draws a textured polygon.
    /*!
        Draws a polygon with the given depth and color and applies the given texture to the polygon. This function
        automatically generates texture coordinates for the polygon in either eye space or object space.
        By default, it generates them in object space. This is a geometry factory.
        \param _p The Polygon to draw.
        \param _t The Texture of the polygon.
        \param _c The Color to be applied to the polygon.
        \param _e Set to true to generate coordniates in eye space instead of object space.
    */
    BatchGeometryPtr drawTexturedPolygon (const Polygon& _p, TexturePtr _t, const Color& _c = Color(255,255,255), bool _e = false);

    //! Draw texture.
    /*!
        Draws a textured rectangle with the same dimensions as the given texture at the specified position. This is a geometry factory.
        \param _t The texture to draw.
        \param _p Where to draw it at.
        \param _rot Rotation Matrix.
        \param _scale Scale.
        \param _color Color.
        \param _flags E_GEOMETRY_FLAGS for certain effects.
        \sa drawTexturePart()
    */
   BatchGeometryPtr drawTexture( TexturePtr _t, const Vector2d& _p, const RotationMatrix& _rot = RotationMatrix( 0.0f ), const Vector2d& _scale = Vector2d(1.0f,1.0f), const Color& _color = Color(255,255,255), unsigned int _flags = EGF_NONE );

    //! Draws a texture with a clipping rectangle.
    /*!
        Draws a textured rectangle with the same dimensions as the given rectangle. This is a geometry factory.
        \param _t The texture to draw.
        \param _p Where to draw it at.
        \param _rect The clipping rectangle, this defines the region of the texture to use.
        \param _rot Rotation.
        \param _scale Scale.
        \param _color Color.
        \param _flags E_GEOMETRY_FLAGS for certain effects.
        \sa drawTexture()
    */
    BatchGeometryPtr drawTexturePart( TexturePtr _t, const Vector2d& _p, const Rectangle& _rect, const RotationMatrix& _rot = RotationMatrix(0.0f), const Vector2d& _scale=Vector2d(1.0f,1.0f), const Color& _color=Color(255,255,255), unsigned int _flags = EGF_NONE );


protected:

    BatchRenderer& renderer;
};

}


#endif //__PH_2D_GFX_FACTORY_H__