
#ifndef __C_GUI_COLOR_PICKER_HEADER__
#define __C_GUI_COLOR_PICKER_HEADER__

#include <IGUIElement.h>
#include <S3DVertex.h>

// Copyright (C) <2014> <Jehan-antoine vayssade>
// Ovan/Magun contact on irrlicht-fr.org or ovan@sleek-think.ovh
// Code is under the zlib license (same as Irrlicht)
// For conditions of distribution and use, see copyright notice in irrlicht.h

namespace irr
{
    namespace gui
    {
        class IGUIButton;
        class IGUIStaticText;
        class IGUIScrollBar;

        class IGUIColorPicker : public IGUIElement
        {
            public:
                IGUIColorPicker(IGUIEnvironment *environment, IGUIElement *parent, s32 id = 0, bool useAdvenced = true) noexcept;
                ~IGUIColorPicker() noexcept;

                virtual void setEnabled(bool);
                virtual void setRelativePosition(const core::recti &r);

                virtual bool OnEvent(const SEvent&) noexcept;
                virtual void updateAbsolutePosition();

                virtual void setAdvenced(bool) noexcept;
                virtual bool getAdvenced() const noexcept;

                virtual void setPickedColor(const video::SColor&) noexcept;
                virtual const video::SColor& getPickedColor() const noexcept;

                virtual void setSelectedColor(const video::SColor&) noexcept;
                virtual const video::SColor& getSelectedColor() const noexcept;

                virtual void setBackgroundColor(const video::SColor&) noexcept;
                virtual const video::SColor& getBackgroundColor() const noexcept;

                IGUIButton *getCloseButton() const noexcept;
                IGUIButton *getExtendButton() const noexcept;

                virtual void draw();
            protected:
                virtual void setScrollValueHSV(bool set, int h, int s, int v) noexcept;
                virtual void setScrollValueRGB(bool set, int r, int g, int b) noexcept;
                virtual void updateFromScroll(bool HsvOrRgb) noexcept;

                virtual void recalculateAdvenced() noexcept;
                virtual void recalculatePickedColor() noexcept;

                virtual void createAlphaTexture() noexcept;
                virtual void createGradientTexture() noexcept;
            protected:
                bool isGradient, isColor, isInside,
                     isExtended, useAdvenced;

                int colorpos;
                core::vector2di  pickpos;

                video::SColor pickcolor, selectcolor, color;
                video::SColor background, white, black, alpha;
                core::recti   box, selectbox, pickbox, gradient;

                IGUIButton      *close, *extend;
                IGUIScrollBar   *scroll;
                IGUIScrollBar   *sb[6];
                IGUIStaticText  *tx[6][2];
                video::ITexture *img[2];
        };
    }
}

#endif

