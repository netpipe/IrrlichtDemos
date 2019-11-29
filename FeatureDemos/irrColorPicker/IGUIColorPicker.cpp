
#include "IGUIColorPicker.h"

#include <IGUIEnvironment.h>
#include <IGUIButton.h>
#include <IGUIScrollBar.h>
#include <IGUIStaticText.h>
#include <IVideoDriver.h>
#include <irrMath.h>
#include <SColor.h>

// Copyright (C) <2014> <Jehan-antoine vayssade>
// Ovan/Magun contact on irrlicht-fr.org or ovan@sleek-think.ovh
// Code is under the zlib license (same as Irrlicht)
// For conditions of distribution and use, see copyright notice in irrlicht.h

namespace irr
{
    inline core::vector3df RGBftoHSV(const video::SColorf &rgb)
    {
        core::vector3df hsv;

        f32 M = core::max_(rgb.getRed(), rgb.getGreen(), rgb.getBlue());
        f32 m = core::min_(rgb.getRed(), rgb.getGreen(), rgb.getBlue());
        f32 C = M - m;

        if(C == 0)
            hsv.X = 0;
        else if(M <= rgb.getRed())
            hsv.X =((rgb.getGreen() - rgb.getBlue()) / C);
        else if(M <= rgb.getGreen())
            hsv.X =((rgb.getBlue() - rgb.getRed()) / C) + 2;
        else if(M <= rgb.getBlue())
            hsv.X =((rgb.getRed() - rgb.getGreen()) / C) + 4;

        hsv.X *= 60;
        if(hsv.X < 0)
            hsv.X += 360;

        hsv.Y = M;

        if(hsv.Y == 0) hsv.Z = 0;
        else hsv.Z = C / hsv.Y;

        return hsv;
    }
    namespace gui
    {
        IGUIColorPicker::IGUIColorPicker(IGUIEnvironment *environment, IGUIElement *parent, s32 id, bool uad) noexcept
            : IGUIElement(EGUIET_COLOR_SELECT_DIALOG , environment, parent, id, {0, 0, 110 + uad*15, 160}),
              background{255, 255, 255, 255}, white{255, 255, 255, 255}, black{255, 0, 0, 0}, alpha(0, 0, 0, 0),
              colorpos(0), isGradient(false), isColor(false), isExtended(false), useAdvenced(uad)
        {
            setNotClipped(false);
            close = Environment->addButton({5, 140, 85, 156}, this, 0, L"take this color");

            IGUISkin *skin = Environment->getSkin();

            extend = Environment->addButton({110, 5, 120, 156}, this);
            extend->setVisible(useAdvenced);
            extend->setSpriteBank(skin->getSpriteBank());
            extend->setSprite(EGBS_BUTTON_UP,   skin->getIcon(EGDI_CURSOR_RIGHT), video::SColor(255,0,0,0));
            extend->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_CURSOR_RIGHT), video::SColor(255,255,255,255));

            scroll = Environment->addScrollBar(true, {5, 125, 85, 135}, this);
            scroll->setMin(0);
            scroll->setMax(255);
            scroll->setPos(255);

            #define implement_scrollbar(i, text, x, max) \
                tx[i][0] = Environment->addStaticText(text, {125, x, 150, x+10}, false, true, this, -1, true); \
                tx[i][1] = Environment->addStaticText(L"",  {150, x, 270, x+10}, false, true, this, -1, true); \
                tx[i][0]->setVisible(false); \
                tx[i][1]->setTextAlignment(EGUIA_CENTER, EGUIA_CENTER); \
                sb[i] = Environment->addScrollBar(true, {125, x+10, 270, x+20}, this); \
                sb[i]->setMin(0); \
                sb[i]->setMax(max); \
                sb[i]->setPos(max); \
                sb[i]->setVisible(false);

            implement_scrollbar(0, L"H",  5, 359);
            implement_scrollbar(1, L"S", 27, 100);
            implement_scrollbar(2, L"V", 49, 100);

            implement_scrollbar(3, L"R",  89, 255);
            implement_scrollbar(4, L"G", 112, 255);
            implement_scrollbar(5, L"B", 135, 255);

            #undef implement_scrollbar

            createAlphaTexture();
            createGradientTexture();

            setPickedColor({255, 64, 64, 128});
            setSelectedColor({255, 64, 64, 128});
            updateAbsolutePosition();
        }
        IGUIColorPicker::~IGUIColorPicker() noexcept
        {
            close->drop();
            extend->drop();
            scroll->drop();
            img[0]->drop();
            img[1]->drop();

            for(int i = 0; i< 6; ++i)
            {
                tx[i][0]->drop();
                tx[i][1]->drop();
                sb[i]->drop();
            }
        }
        void IGUIColorPicker::setEnabled(bool c)
        {
            close->setEnabled(c);
            scroll->setEnabled(c);
            for(int i = 0; i< 6; ++i)
            {
                tx[i][0]->setEnabled(c);
                tx[i][1]->setEnabled(c);
                sb[i]->setEnabled(c);
            }
            IGUIElement::setEnabled(c);
        }
        IGUIButton *IGUIColorPicker::getCloseButton() const noexcept
        {
            return close;
        }

        IGUIButton *IGUIColorPicker::getExtendButton() const noexcept
        {
            return extend;
        }
        void IGUIColorPicker::createAlphaTexture() noexcept
        {
            img[0] = Environment->getVideoDriver()->addTexture({16, 16}, "alpha", video::ECF_A8R8G8B8);
            u32 *tmp = (u32*) img[0]->lock();

            video::SColor color;

            #define square(colorstart, sx, sy, sz, sw)                          \
                color = colorstart;                                             \
                for(int y=sy; y<sw; ++y)                                        \
                    for(int x=sx; x<sz; ++x)                                    \
                        color.getData(&tmp[x + y*16], video::ECF_A8R8G8B8);\

            square(video::SColor(255, 153, 153, 153), 0, 0,  8,  8);
            square(video::SColor(255, 153, 153, 153), 8, 8, 16, 16);
            square(video::SColor(255, 102, 102, 102), 8, 0, 16,  8);
            square(video::SColor(255, 102, 102, 102), 0, 8,  8, 16);

            #undef square

            img[0]->unlock();
        }
        void IGUIColorPicker::createGradientTexture() noexcept
        {
            img[1] = Environment->getVideoDriver()->addTexture({15, 151}, "gradient", video::ECF_A8R8G8B8);
            u32 *tmp = (u32*) img[1]->lock();

            video::SColor from;
            video::SColor to;

            #define interpolate(colorstart, colorend, start, end)              \
                from = colorstart;                                             \
                to = colorend;                                                 \
                                                                               \
                for(int y=start; y<end; ++y)                                   \
                {                                                              \
                    video::SColor c = to.getInterpolated(                 \
                        from, (y-start)/25.f                                   \
                    );                                                         \
                    for(int x=0; x<15; ++x)                                    \
                        c.getData(&tmp[x + y*15], video::ECF_A8R8G8B8);   \
                }

            interpolate(video::SColor(255, 255, 0, 0),   video::SColor(255, 255, 0, 255),   0,  25);
            interpolate(video::SColor(255, 255, 0, 255), video::SColor(255, 0, 0, 255),    25,  50);

            interpolate(video::SColor(255, 0, 0, 255), video::SColor(255, 0, 255, 255),  50,  75);
            interpolate(video::SColor(255, 0, 255, 255), video::SColor(255, 0, 255, 0),  75, 100);

            interpolate(video::SColor(255, 0, 255, 0), video::SColor(255, 255, 255, 0), 100, 125);
            interpolate(video::SColor(255, 255, 255, 0), video::SColor(255, 255, 0, 0), 125, 151);

            #undef interpolate

            img[1]->unlock();
        }
        void IGUIColorPicker::setRelativePosition(const core::recti &r)
        {
            RelativeRect.UpperLeftCorner = r.UpperLeftCorner;
            RelativeRect.LowerRightCorner.X = r.UpperLeftCorner.X + 110 + useAdvenced*15;
            RelativeRect.LowerRightCorner.Y = r.UpperLeftCorner.Y + 160;

            if(isExtended)
                RelativeRect.LowerRightCorner.X += 150;

            IGUIElement::setRelativePosition(RelativeRect);
        }
        void IGUIColorPicker::setAdvenced(bool i) noexcept
        {
            IGUISkin *skin = Environment->getSkin();
            isExtended = i;

            for(int j = 0; j<6; ++j)
            {
                tx[j][0]->setVisible(isExtended);
                tx[j][1]->setVisible(isExtended);
                sb[j]->setVisible(isExtended);
            }

            if(isExtended)
            {
                extend->setSprite(EGBS_BUTTON_UP,   skin->getIcon(EGDI_CURSOR_LEFT), video::SColor(255,0,0,0));
                extend->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_CURSOR_LEFT), video::SColor(255,255,255,255));
                recalculatePickedColor();
                recalculateAdvenced();
            }
            else
            {
                extend->setSprite(EGBS_BUTTON_UP,   skin->getIcon(EGDI_CURSOR_RIGHT), video::SColor(255,0,0,0));
                extend->setSprite(EGBS_BUTTON_DOWN, skin->getIcon(EGDI_CURSOR_RIGHT), video::SColor(255,255,255,255));
            }

            setRelativePosition(RelativeRect);
        }
        bool IGUIColorPicker::getAdvenced() const noexcept
        {
            return isExtended;
        }
        bool IGUIColorPicker::OnEvent(const SEvent &event) noexcept
        {
            if(!isEnabled())
                return IGUIElement::OnEvent(event);

            if(event.EventType == EET_MOUSE_INPUT_EVENT)
            {
                core::vector2di pos(event.MouseInput.X, event.MouseInput.Y);

                if(event.MouseInput.Event == EMIE_LMOUSE_PRESSED_DOWN)
                {
                    isGradient = gradient.isPointInside(pos);
                    isColor = box.isPointInside(pos);
                    isInside = AbsoluteRect.isPointInside(pos);
                }

                if(event.MouseInput.Event == EMIE_LMOUSE_LEFT_UP)
                {
                    // does not work since 1.7 (i think)
                    if(!AbsoluteRect.isPointInside(pos) && !isInside)
                    {
                        SEvent event;
                        event.EventType = EET_GUI_EVENT;
                        event.GUIEvent.Caller = this;
                        event.GUIEvent.Element = 0;
                        event.GUIEvent.EventType = EGET_ELEMENT_CLOSED;
                        Parent->OnEvent(event);
                        remove();
                        drop();
                    }
                    if(isInside && !isGradient && !isColor && selectbox.isPointInside(pos))
                    {
                        setPickedColor(getSelectedColor());
                    }
                    isGradient = isColor = false;
                }

                if(isGradient)
                {
                    if(pos.X < gradient.UpperLeftCorner.X)
                        pos.X = gradient.UpperLeftCorner.X;
                    if(pos.Y < gradient.UpperLeftCorner.Y)
                        pos.Y = gradient.UpperLeftCorner.Y;

                    if(pos.X > gradient.LowerRightCorner.X)
                        pos.X = gradient.LowerRightCorner.X;
                    if(pos.Y > gradient.LowerRightCorner.Y)
                        pos.Y = gradient.LowerRightCorner.Y;

                    colorpos = pos.Y - gradient.UpperLeftCorner.Y;
                    recalculatePickedColor();
                    recalculateAdvenced();
                }

                if(isColor)
                {
                    if(pos.X < box.UpperLeftCorner.X)
                        pos.X = box.UpperLeftCorner.X;
                    if(pos.Y < box.UpperLeftCorner.Y)
                        pos.Y = box.UpperLeftCorner.Y;

                    if(pos.X > box.LowerRightCorner.X)
                        pos.X = box.LowerRightCorner.X;
                    if(pos.Y > box.LowerRightCorner.Y)
                        pos.Y = box.LowerRightCorner.Y;

                    pickpos.X = pos.X - box.UpperLeftCorner.X;
                    pickpos.Y = pos.Y - box.UpperLeftCorner.Y;

                    recalculatePickedColor();
                    recalculateAdvenced();
                }

                if(isGradient || isColor)
                    return true;
            }

            if(event.EventType == EET_GUI_EVENT)
            {
                switch(event.GUIEvent.EventType)
                {
                    case EGET_BUTTON_CLICKED:
                        if(event.GUIEvent.Caller == close)
                        {
                            setSelectedColor(pickcolor);
                            SEvent event;
                            event.EventType = EET_GUI_EVENT;
                            event.GUIEvent.Caller = this;
                            event.GUIEvent.Element = 0;
                            event.GUIEvent.EventType = EGET_FILE_SELECTED;
                            Parent->OnEvent(event);
                        }
                        else
                        {
                            isExtended = !isExtended;
                            setAdvenced(isExtended);
                        }
                    break;
                    case EGET_SCROLL_BAR_CHANGED:
                    {
                        if(event.GUIEvent.Caller == scroll)
                            recalculatePickedColor();
                        else
                        {
                            updateFromScroll(
                                event.GUIEvent.Caller == sb[0] ||
                                event.GUIEvent.Caller == sb[1] ||
                                event.GUIEvent.Caller == sb[2]
                            );
                        }
                        return true;
                    }
                    break;
                }
            }

            return IGUIElement::OnEvent(event);
        }
        void IGUIColorPicker::setScrollValueHSV(bool set, int h, int s, int v) noexcept
        {
            core::stringw value;

            #define set(i, v) \
                value = L""; \
                value += v; \
                tx[i][1]->setText(value.c_str()); \
                if(set) sb[i]->setPos(v); \

            set(0, h);
            set(1, s);
            set(2, v);

            #undef set
        }
        void IGUIColorPicker::setScrollValueRGB(bool set, int r, int g, int b) noexcept
        {
            core::stringw value;

            #define set(i, v) \
                value = L""; \
                value += v; \
                tx[i][1]->setText(value.c_str()); \
                if(set) sb[i]->setPos(v); \

            set(3, r);
            set(4, g);
            set(5, b);

            #undef set
        }
        void IGUIColorPicker::updateFromScroll(bool HsvOrRgb) noexcept
        {
            if(HsvOrRgb)
            {
                colorpos = 150-sb[0]->getPos()/360.f*150.f;
                pickpos.X = sb[1]->getPos()*0.8f;
                pickpos.Y = 80-sb[2]->getPos()*0.8f;
                recalculatePickedColor();
                setScrollValueHSV(false, sb[0]->getPos(), sb[1]->getPos(), sb[2]->getPos());
                setScrollValueRGB(true, pickcolor.getRed(), pickcolor.getGreen(), pickcolor.getBlue());
            }
            else
            {
                setPickedColor({
                    (u32)scroll->getPos(),
                    (u32)sb[3]->getPos(),
                    (u32)sb[4]->getPos(),
                    (u32)sb[5]->getPos()
                });
            }
        }
        void IGUIColorPicker::recalculateAdvenced() noexcept
        {
            if(!isExtended)
                return;

            core::stringw value;

            core::vector3df hsv = RGBftoHSV({
                pickcolor.getRed()/255.f,
                pickcolor.getGreen()/255.f,
                pickcolor.getBlue()/255.f,
                pickcolor.getAlpha()/255.f
            });

            setScrollValueHSV(true, hsv.X, hsv.Z*100.f, hsv.Y*100.f);
            setScrollValueRGB(true, pickcolor.getRed(), pickcolor.getGreen(), pickcolor.getBlue());
        }
        void IGUIColorPicker::recalculatePickedColor() noexcept
        {
            u32 *tmp =(u32*)img[1]->lock();
            color.set(tmp[colorpos*img[1]->getOriginalSize().Width]);
            img[1]->unlock();

            alpha = color;
            alpha.setAlpha(0);

            video::SColor hcolor = color.getInterpolated(white, pickpos.X/80.f);
            pickcolor = black.getInterpolated(hcolor, pickpos.Y/80.f);
            pickcolor.setAlpha(scroll->getPos());
        }
        void IGUIColorPicker::setPickedColor(const video::SColor &c) noexcept
        {
            core::vector3df hsv = RGBftoHSV({
                c.getRed()/255.f,
                c.getGreen()/255.f,
                c.getBlue()/255.f,
                c.getAlpha()/255.f
            });

            colorpos = 150-hsv.X/360.f*150.f;
            pickpos.X = hsv.Z*80.f;
            pickpos.Y = 80-hsv.Y*80.f;

            scroll->setPos(c.getAlpha());
            recalculatePickedColor();

            setScrollValueHSV(true, hsv.X, hsv.Z*100, hsv.Y*100);
            setScrollValueRGB(true, c.getRed(), c.getGreen(), c.getBlue());
        }
        const video::SColor& IGUIColorPicker::getPickedColor() const noexcept
        {
            return pickcolor;
        }
        void IGUIColorPicker::setSelectedColor(const video::SColor &b) noexcept
        {
            selectcolor = b;
        }
        const video::SColor& IGUIColorPicker::getSelectedColor() const noexcept
        {
            return selectcolor;
        }
        void IGUIColorPicker::setBackgroundColor(const video::SColor &b) noexcept
        {
            background = b;
        }
        const video::SColor& IGUIColorPicker::getBackgroundColor() const noexcept
        {
            return background;
        }
        void IGUIColorPicker::updateAbsolutePosition()
        {
            IGUIElement::updateAbsolutePosition();

            box.UpperLeftCorner = AbsoluteRect.UpperLeftCorner;
            box.LowerRightCorner = AbsoluteRect.UpperLeftCorner;
            box.UpperLeftCorner.X += 5;
            box.UpperLeftCorner.Y += 5;
            box.LowerRightCorner.X += 85;
            box.LowerRightCorner.Y += 85;

            gradient.UpperLeftCorner = AbsoluteRect.UpperLeftCorner;
            gradient.LowerRightCorner = AbsoluteRect.UpperLeftCorner;
            gradient.UpperLeftCorner.X += 90;
            gradient.UpperLeftCorner.Y += 5;
            gradient.LowerRightCorner.X += 105;
            gradient.LowerRightCorner.Y += 155;

            pickbox.UpperLeftCorner = AbsoluteRect.UpperLeftCorner;
            pickbox.LowerRightCorner = AbsoluteRect.UpperLeftCorner;
            pickbox.UpperLeftCorner.X += 5;
            pickbox.UpperLeftCorner.Y += 90;
            pickbox.LowerRightCorner.X += 85;
            pickbox.LowerRightCorner.Y += 105;

            selectbox.UpperLeftCorner = AbsoluteRect.UpperLeftCorner;
            selectbox.LowerRightCorner = AbsoluteRect.UpperLeftCorner;
            selectbox.UpperLeftCorner.X += 5;
            selectbox.UpperLeftCorner.Y += 105;
            selectbox.LowerRightCorner.X += 85;
            selectbox.LowerRightCorner.Y += 120;
        }
        void IGUIColorPicker::draw()
        {
            if(!isVisible())
                return;

            Environment->getSkin()->draw3DSunkenPane(
                this, background,
                false, true,
                AbsoluteRect,
                &AbsoluteClippingRect
            );

            Environment->getVideoDriver()->draw2DImage(img[1], {
                AbsoluteRect.UpperLeftCorner.X+90,
                AbsoluteRect.UpperLeftCorner.Y+5
            });

            // 2 draw because the interpolation in the diagonal is not well rendered
            Environment->getVideoDriver()->draw2DRectangle(black, box, &AbsoluteClippingRect);
            Environment->getVideoDriver()->draw2DRectangle(box, white, color, alpha, alpha, &AbsoluteClippingRect);

            {
                const core::vector2di start =  {AbsoluteRect.UpperLeftCorner.X+90,  AbsoluteRect.UpperLeftCorner.Y+5+colorpos};
                const core::vector2di end =    {AbsoluteRect.UpperLeftCorner.X+105,  AbsoluteRect.UpperLeftCorner.Y+5+colorpos};
                const core::vector2di hstart = {box.UpperLeftCorner.X,  box.UpperLeftCorner.Y+pickpos.Y};
                const core::vector2di hend =   {box.LowerRightCorner.X, box.UpperLeftCorner.Y+pickpos.Y};
                const core::vector2di vstart = {box.UpperLeftCorner.X+pickpos.X, box.UpperLeftCorner.Y};
                const core::vector2di vend =   {box.UpperLeftCorner.X+pickpos.X, box.LowerRightCorner.Y};

                Environment->getVideoDriver()->draw2DLine({ start.X,    start.Y-1}, { end.X,    end.Y-1}, white);
                Environment->getVideoDriver()->draw2DLine({ start.X,    start.Y+1}, { end.X,    end.Y+1}, white);
                Environment->getVideoDriver()->draw2DLine({hstart.X,   hstart.Y-1}, {hend.X,   hend.Y-1}, white);
                Environment->getVideoDriver()->draw2DLine({hstart.X,   hstart.Y+1}, {hend.X,   hend.Y+1}, white);
                Environment->getVideoDriver()->draw2DLine({vstart.X-1,   vstart.Y}, {vend.X-1, vend.Y  }, white);
                Environment->getVideoDriver()->draw2DLine({vstart.X+1,   vstart.Y}, {vend.X+1, vend.Y  }, white);

                Environment->getVideoDriver()->draw2DLine(start,   end, black);
                Environment->getVideoDriver()->draw2DLine(hstart, hend, black);
                Environment->getVideoDriver()->draw2DLine(vstart, vend, black);
            }

            Environment->getVideoDriver()->draw2DImage(img[0], pickbox, pickbox);
            Environment->getVideoDriver()->draw2DRectangle(
                pickcolor, pickbox,
                &AbsoluteClippingRect
            );
            Environment->getVideoDriver()->draw2DImage(img[0], selectbox, selectbox);
            Environment->getVideoDriver()->draw2DRectangle(
                selectcolor, selectbox,
                &AbsoluteClippingRect
            );

            IGUIElement::draw();
        }
    }
}
