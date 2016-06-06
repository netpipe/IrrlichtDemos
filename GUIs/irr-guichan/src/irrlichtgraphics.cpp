/*
 * For comments regarding functions please see the header file.
 */

#include "guichan/irrlicht/irrlichtgraphics.hpp"

#include "guichan/exception.hpp"
#include "guichan/font.hpp"
#include "guichan/image.hpp"
#include "guichan/irrlicht/irrlichtimage.hpp"

namespace gcn
{

    IrrlichtGraphics::IrrlichtGraphics(irr::video::IVideoDriver* driver) : mDriver(driver)
    {
        if (mDriver)
            mDriver->grab();
    }

    IrrlichtGraphics::~IrrlichtGraphics()
    {
        if (mDriver)
        {
            mDriver->drop();
            mDriver = 0;
        }
    }

    void IrrlichtGraphics::_beginDraw()
    {
        Rectangle area;
        area.x = 0;
        area.y = 0;
        area.width = mDriver->getScreenSize().Width;
        area.height = mDriver->getScreenSize().Height;
        pushClipArea(area);
    }

    void IrrlichtGraphics::_endDraw()
    {
        popClipArea();
    }

    bool IrrlichtGraphics::pushClipArea(Rectangle area)
    {
        return Graphics::pushClipArea(area);
    }

    void IrrlichtGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty())
        {
            return;
        }
    }

    void IrrlichtGraphics::drawImage(const Image* image,
                                int srcX,
                                int srcY,
                                int dstX,
                                int dstY,
                                int width,
                                int height)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        const IrrlichtImage* srcImage = dynamic_cast<const IrrlichtImage*>(image);

        if (srcImage == NULL)
        {
            throw GCN_EXCEPTION("Trying to draw an image of unknown format, must be an IrrlichtImage.");
        }

        irr::core::position2d<irr::s32> destPos(dstX + top.xOffset, dstY + top.yOffset);
        irr::core::rect<irr::s32> sourceRect(srcX, srcY, srcX + width, srcY + height);
        irr::core::rect<irr::s32> clipRect(top.x, top.y, top.x + top.width, top.y + top.height);
        irr::video::SColor color(255, 255, 255, 255);

        mDriver->draw2DImage(srcImage->getTexture(), destPos, sourceRect, &clipRect, color, true);
    }

    void IrrlichtGraphics::fillRectangle(const Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        Rectangle area = rectangle;
        area.x += top.xOffset;
        area.y += top.yOffset;

        if(!area.isIntersecting(top))
        {
            return;
        }

        irr::video::SColor color(mColor.a, mColor.r, mColor.g, mColor.b);
        irr::core::rect<irr::s32> pos(area.x, area.y, area.x + area.width, area.y + area.height);
        irr::core::rect<irr::s32> clip(top.x, top.y, top.x + top.width, top.y + top.height);
        
        mDriver->draw2DRectangle(color, pos, &clip);
    }

    void IrrlichtGraphics::drawPoint(int x, int y)
    {
        /*
         * Currently Irrlicht does not have a function to draw a single
         * pixel. I've proposed adding this to the engine and provided them
         * with the patches to add it (for all the drivers except the directx ones).
         * For now just use this little hack of drawing a really short line.
         */
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        /*
         * If you are running the latest svn you should add this in.
         * I don't think any of the Guichan demos actually use drawPoint,
         * so I'm leaving this function broken (until Irrlicht patches get accepted
         * and I update my Guichan to the latest svn).
         */
        //if(!top.isContaining(x,y))
        //    return;

        irr::core::position2d<irr::s32> start(x, y);
        irr::core::position2d<irr::s32> end(x + 1, y + 1);
        irr::video::SColor color(mColor.a, mColor.r, mColor.g, mColor.b);

        mDriver->draw2DLine(start, end, color);
    }

    void IrrlichtGraphics::drawRectangle(const Rectangle& rectangle)
    {
        int x1 = rectangle.x;
        int x2 = rectangle.x + rectangle.width - 1;
        int y1 = rectangle.y;
        int y2 = rectangle.y + rectangle.height - 1;

        drawLine(x1, y1, x2, y1);
        drawLine(x2, y1, x2, y2);
        drawLine(x1, y1, x1, y2);
        drawLine(x1, y2, x2, y2);
    }

    void IrrlichtGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const ClipRectangle& top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        if(x2 > top.x + top.width)
            x2 = top.x + top.width;

        if(y2 > top.y + top.height)
            y2 = top.y + top.height;

        irr::core::position2d<irr::s32> start(x1, y1);
        irr::core::position2d<irr::s32> end(x2, y2);
        irr::video::SColor color(mColor.a, mColor.r, mColor.g, mColor.b);

        mDriver->draw2DLine(start, end, color);
    }

    void IrrlichtGraphics::setColor(const Color& color)
    {
        mColor = color;
    }

    const Color& IrrlichtGraphics::getColor() const
    {
        return mColor;
    }
}
