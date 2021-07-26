/*********************************************************************
Copyright (c) 2020-2021 TheMrCerebro
http://themrcerebro.com

irrTiled - Zlib license.

This software is provided 'as-is', without any express or
implied warranty. In no event will the authors be held
liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented;
you must not claim that you wrote the original software.
If you use this software in a product, an acknowledgment
in the product documentation would be appreciated but
is not required.

2. Altered source versions must be plainly marked as such,
and must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any
source distribution.
*********************************************************************/

#include <irrlicht.h>

//======================
#include "../irrTiled.h"
//======================

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

//
int main()
{
    IrrlichtDevice* device = createDevice(EDT_OPENGL);
    device->setWindowCaption(L"irrTiled v1.1 - by TheMrCerebro");

    IVideoDriver* driver = device->getVideoDriver();

    //======================================================
    irrTiled* tld = new irrTiled("media/shapes.tmx",device);
    //======================================================

    while(device->run())
    {
        driver->beginScene(true, true, SColor(0, 200, 200, 200));

        // Extracts data from all groups of objects.
        for (u32 i = 0; i < tld->objectGroup.size(); ++i)
        {
            OBJECTGROUP og = tld->objectGroup[i];

            // Extract data from all objects.
            for (u32 j = 0; j < og.object.size(); ++j)
            {
                OBJECT obj = og.object[j];

                // Extracts the position and the size of the object.
                s32 x = obj.x;
                s32 y = obj.y;
                s32 w = obj.width;
                s32 h = obj.height;

                switch (obj.shape)
                {
                case SHAPE::RECT:// Draw rectangles.
                    driver->draw2DRectangleOutline(recti(vector2di(x, y), dimension2du(w, h)), SColor(255, 255, 0, 0));
                    break;
                case SHAPE::ELLIPSE:// Draw circles.You should draw ellipses with different sizes but for now you can't do that.
                    driver->draw2DPolygon(vector2di(x+w/2, y+h/2), w/2, SColor(255, 0, 255, 0), 16);
                    break;
                case SHAPE::POINT:// Draw points.On this occasion to show them, I have used a crossand a circle.
                {
                    driver->draw2DLine(vector2di(x-4, y), vector2di(x+4, y), SColor(255, 255, 0, 255));
                    driver->draw2DLine(vector2di(x, y-4), vector2di(x, y+4), SColor(255, 255, 0, 255));
                    driver->draw2DPolygon(vector2di(x, y),4, SColor(255, 255, 0, 255));
                }
                break;
                case SHAPE::POLYGON:// Draw polygons using the information extracted from the attribute.
                {
                    for (u32 p = 0; p < obj.points.size(); ++p)
                    {
                        vector2di in, out;
                        in = obj.points[p];
                        if(p == obj.points.size()-1)
                            out = obj.points[0];
                        else
                            out = obj.points[p + 1];

                        // Draw polygons using lines.
                        driver->draw2DLine(vector2di(x,y) + in, vector2di(x, y) + out, SColor(255, 0, 0, 255));
                    }
                }
                break;
                default:
                    break;
                }
            }
        }

        driver->endScene();
    }

    device->drop();

    return 0;
}
