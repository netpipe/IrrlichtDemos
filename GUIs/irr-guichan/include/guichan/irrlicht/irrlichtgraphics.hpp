
#ifndef GCN_IRRLICHTGRAPHICS_HPP
#define GCN_IRRLICHTGRAPHICS_HPP

#include "irrlicht.h"

#include "guichan/color.hpp"
#include "guichan/graphics.hpp"
#include "guichan/platform.hpp"

namespace gcn
{
    class Image;
    class Rectangle;
    
    /**
    * Irrlicht implementation of the Graphics.
    */
    class GCN_EXTENSION_DECLSPEC IrrlichtGraphics : public Graphics
    {
    public:

        // Needed so that drawImage(gcn::Image *, int, int) is visible.
        using Graphics::drawImage;

        /**
        * Constructor.
        *
        * @param driver the Irrlicht video driver to draw to.
        */
        IrrlichtGraphics(irr::video::IVideoDriver* driver);

        /**
        * Destructor.
        */
        ~IrrlichtGraphics();

        // Inherited from Graphics

        virtual void _beginDraw();

        virtual void _endDraw();

        virtual bool pushClipArea(Rectangle area);

        virtual void popClipArea();

        virtual void drawImage(const Image* image,
                                int srcX,
                                int srcY,
                                int dstX,
                                int dstY,
                                int width,
                                int height);

        virtual void drawPoint(int x, int y);

        virtual void drawLine(int x1, int y1, int x2, int y2);

        virtual void drawRectangle(const Rectangle& rectangle);

        virtual void fillRectangle(const Rectangle& rectangle);

        virtual void setColor(const Color& color);

        virtual const Color& getColor() const;

    protected:

        Color mColor;
        irr::video::IVideoDriver* mDriver;
    };
}

#endif // end GCN_IRRLICHTGRAPHICS_HPP
