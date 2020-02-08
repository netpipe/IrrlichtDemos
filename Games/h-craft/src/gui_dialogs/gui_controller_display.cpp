// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "gui_controller_display.h"
#include "../globals.h"
#include "../controller.h"

using namespace irr;
using namespace gui;

ControllerDisplay::ControllerDisplay(gui::IGUIEnvironment * environment_, gui::IGUIElement * parent_, s32 id_, core::rect< s32 > rectangle_)
 : gui::IGUIElement(gui::EGUIET_ELEMENT, environment_, parent_, id_, rectangle_)
{
    mController = NULL;
}

ControllerDisplay::~ControllerDisplay()
{
}

void ControllerDisplay::SetController(Controller * controller_)
{
    mController = controller_;
}

void ControllerDisplay::draw()
{
    if ( !IsVisible)
        return;

    float posX = 0.f;
    float posY = 0.f;
    if ( mController )
    {
        posX = mController->GetRotation();
        posY = mController->GetPower();
    }

    const int SIZE = 5;
    core::position2d<int> t1(0, -SIZE);
    core::position2d<int> t2(-SIZE, SIZE);
    core::position2d<int> t3(SIZE, SIZE);
    int radius = (int)((float)MIN(AbsoluteRect.getWidth(), AbsoluteRect.getHeight()) * 0.5f);
    radius -= SIZE;

    IGUISkin* skin = Environment->getSkin();

    video::IVideoDriver* driver = Environment->getVideoDriver();
    core::position2d<int> center = AbsoluteRect.getCenter ();
    center.X -= (int)(posX * (float)radius);
    center.Y -= (int)(posY * (float)radius);
    t1 += center;
    t2 += center;
    t3 += center;

    driver->draw2DRectangle( skin->getColor(gui::EGDC_3D_FACE),	AbsoluteRect, &AbsoluteClippingRect);
    driver->draw2DLine (t1, t2, video::SColor(255, 255, 255, 255));
    driver->draw2DLine (t2, t3, video::SColor(255, 255, 255, 255));
    driver->draw2DLine (t3, t1, video::SColor(255, 255, 255, 255));

    IGUIElement::draw();
}
