/*

Copyright (c) 2010, Jonathan Wayne Parrott

Please see the license.txt file included with this source
distribution for more information.

*/

#ifndef __PHFONT_H__
#define __PHFONT_H__

#include "config.h"
#include "Resource.h"
#include "AbstractGeometryFactory.h"
#include "BatchRenderer.h"
#include <string>
#include "Color.h"

namespace phoenix
{

class RenderSystem;


//! Text Alignment.
enum E_TEXT_ALIGNMENT{
	T_ALIGN_LEFT,
	T_ALIGN_CENTER,
	T_ALIGN_RIGHT
};

//! Font class.
/*!
    Abstract base class for font handling. It is possible (and suggested)
	to overload this class and provide your own custom interfaces for phoenix
	to draw text.
    The default font interface in phoenix is provided by BitmapFont.
*/
class Font :
    public Resource, public AbstractGeometryFactory
{

public:
    
    //! Constructor
    /*!
        \param _r ResourceManager pointer.
        \param _t Type, defaults to 2 ( ERT_FONT ) for abstract font.
    */
    Font(ResourceManager& _r, BatchRenderer& _b, int _t = 2)
        : Resource( _r, _t ), renderer(&_b), color(), scale( Vector2d( 1, 1 ) )
    {}


	//! RenderSystem based construction.
	Font(RenderSystem& _r, int _t = 2);

    //! Destructor
    virtual ~Font()
    {}

	//! Calculates the rendered dimensions of a given text
	virtual const Vector2d getTextDimensions( const std::string& _s, const Vector2d& _scale = Vector2d(0,0) ){
		return Vector2d(0,0);
	}

    //! Draw text.
    /*!
        The basic interface for drawing text. Must be overloaded by derived classes to provide their
        facility of drawing text. It is a geometry factory.
        \param _s The string to print to the screen.
        \param _p The position to draw it at.
		\param _c Color (white by default).
		\param _scale The amount to scale. If (0,0), it uses the scale from the Font, otherwise it'll override the font's scale.
		\param _align The horizontal alignment of the text.
    */
    virtual BatchGeometryPtr drawText( const std::string& _s, const Vector2d& _p = Vector2d(0,0), const Color& _c = Color(255,255,255), const Vector2d& _scale = Vector2d(0,0), E_TEXT_ALIGNMENT _align = T_ALIGN_LEFT ) = 0;

	//! Gets the current color of the font.
	inline virtual const Color& getColor() { return color; }

    //! Set color.
	inline virtual void setColor( const Color& _c ) { color = _c; }

	//! Gets the scaling factor of the font.
    inline const Vector2d& getScale() const { return scale; }

    //! Set scale.
    inline void setScale( const Vector2d& _s = Vector2d(1,1)) { scale = _s; }

	//! Gets the current target batch renderer
	inline virtual BatchRenderer* getBatchRenderer() { return renderer; }

    //! Set the target batch renderer.
	inline virtual void setBatchRenderer( BatchRenderer* _r ) { renderer = _r; }

protected:

	//! Batcher.
    BatchRenderer* renderer;

	//! Color
	Color color;

	//! Scale.
    Vector2d scale;

};

//! Friendly Font pointer
typedef boost::intrusive_ptr<Font> FontPtr;

}//namespace phoenix

#include "RenderSystem.h"

#endif // __PHFONT_H__
