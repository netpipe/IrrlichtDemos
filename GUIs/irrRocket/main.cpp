/*
  Copyright (C) 2011 Daniel Sudmann

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Daniel Sudmann suddani@googlemail.com
*/
#include <irrlicht.h>

#include "CIrrRocketGUI.h"

irr::core::rect<irr::s32> getOnScreenRect(irr::scene::ISceneNode* node, irr::scene::ISceneCollisionManager* csmgr)
{
    irr::core::aabbox3df box = node->getTransformedBoundingBox();

    irr::core::vector3df vec[8];
    box.getEdges(vec);

    irr::core::array<irr::core::vector2di> spos;
    for (int i=0; i<8; i++)
        spos.push_back(csmgr->getScreenCoordinatesFrom3DPosition(vec[i]));

    irr::core::rect<irr::s32> b(spos[0], spos[0]);
    for (int i=0; i<8; i++)
        b.addInternalPoint(spos[i]);
    return b;
}

int main()
{
    irr::IrrlichtDevice *device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(1024,768));

    irr::scene::ISceneManager* smgr = device->getSceneManager();
    irr::video::IVideoDriver* driver = device->getVideoDriver();

    //events e(context);
    //device->setEventReceiver(&e);


    irr::scene::ISceneNode* node = smgr->addCubeSceneNode(10);

    smgr->addCameraSceneNodeFPS();

    CIrrRocketGUI gui(device);




    while(device->run())
    {
        if (device->isWindowActive())
        {
            driver->beginScene(true, true, irr::video::SColor(255,0,0,200));
            smgr->drawAll();

            irr::video::SMaterial mat;
            mat.Lighting = false;
            driver->setMaterial(mat);
            driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
            {
                irr::core::vector3df p1(-20,0,-10);
                irr::core::vector3df p2(-20,0,10);
                irr::core::vector3df p3(20,0,10);
                irr::core::vector3df p4(20,0,-10);

                irr::core::matrix4 mat;
                irr::core::vector3df rot = smgr->getActiveCamera()->getRotation();
                rot.X = 0;
                rot.Z = 0;
                mat.setRotationDegrees(rot);


                irr::core::line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(irr::core::vector2di(512, 384));
                irr::core::plane3df plane(irr::core::vector3df(0), irr::core::vector3df(0,1,0));
                irr::core::vector3df out;
                if (plane.getIntersectionWithLine(ray.start, ray.getVector(), out))
                {
                    mat.setTranslation(out);

                    mat.transformVect(p1);
                    mat.transformVect(p2);
                    mat.transformVect(p3);
                    mat.transformVect(p4);

                    irr::core::triangle3df t1(p1, p2, p3);
                    irr::core::triangle3df t2(p3, p4, p1);

                    driver->draw3DTriangle(t1);
                    driver->draw3DTriangle(t2);

                    irr::core::aabbox3df box = node->getTransformedBoundingBox();

                    irr::core::rect<irr::s32> b = getOnScreenRect(node, smgr->getSceneCollisionManager());

                    driver->draw3DBox(box);

                    driver->draw2DRectangleOutline(b);
                }
            }


            gui.run();
            driver->endScene();
        }
        else
            device->yield();
    }

    device->drop();
    return 0;
}
