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
#include "CMain.h"
#include <irrlicht.h>

#include "CDecal.h"
#include "CGUIEventDispatcher.h"
irr::scene::IAnimatedMeshSceneNode* node;
CMain::CMain()
{

    DecalBuffer = 0;

    //ctor
    Device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2du(1024,768));
    irr::scene::ISceneManager* smgr = Device->getSceneManager();
    irr::video::IVideoDriver* driver = Device->getVideoDriver();

    GUIEvents = new CGUIEventDispatcher;
    Device->getGUIEnvironment()->setUserEventReceiver(GUIEvents);

    Decal = new CDecal(driver);

    Device->getFileSystem()->addFileArchive("media/map-20kdm2.pk3");

    node = smgr->addAnimatedMeshSceneNode(smgr->getMesh("20kdm2.bsp"));
    //node->setMaterialFlag(irr::video::EMF_WIREFRAME, true);
    node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    node->setScale(irr::core::vector3df(0.1));
    irr::scene::ITriangleSelector* select = smgr->createTriangleSelector(node);
    node->setTriangleSelector(select);

    MetaSelect = smgr->createMetaTriangleSelector();
    MetaSelect->addTriangleSelector(select);
    select->drop();


    irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0, 100, 0.1);
    //irr::scene::ICameraSceneNode* cam = smgr->addCameraSceneNode();
    cam->setPosition(irr::core::vector3df(15.f));
    cam->setTarget(irr::core::vector3df(0.f));

    Device->setEventReceiver(this);
}

CMain::~CMain()
{
    //dtor
    if (DecalBuffer)
        DecalBuffer->drop();

    MetaSelect->drop();
}

bool CMain::OnEvent(const irr::SEvent& event)
{
    if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
    {
        if (event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN)
        {
            irr::scene::ISceneCollisionManager* coll = Device->getSceneManager()->getSceneCollisionManager();
            irr::core::line3d<irr::f32> ray = coll->getRayFromScreenCoordinates(irr::core::vector2di(event.MouseInput.X, event.MouseInput.Y));
            irr::core::vector3df point;
            irr::core::triangle3df triangle;
             irr::scene::ISceneNode* blubb;
            if (coll->getCollisionPoint(ray, MetaSelect, point, triangle, blubb))
            {
                Decal->updateTransform(point/*+triangle.getNormal().normalize()*/, (ray.getVector()), 5);
                //Decal->updateTransform(point+triangle.getNormal().normalize()*1.f, triangle.getNormal().normalize(), 20);
                if (!DecalBuffer)
                {
                    DecalBuffer = new irr::scene::SMeshBuffer();
                }

                irr::core::triangle3df* triangles = new irr::core::triangle3df[MetaSelect->getTriangleCount()];
                irr::s32 count;
                irr::core::matrix4 mat = irr::core::IdentityMatrix;
                //mat.setScale(irr::core::vector3df(0.1));

                MetaSelect->getTriangles(triangles, MetaSelect->getTriangleCount(), count, &mat);
                irr::scene::IMeshBuffer* temp = Decal->createBuffer(triangles, count);
                DecalBuffer->append(temp->getVertices(), temp->getVertexCount(), temp->getIndices(), temp->getIndexCount());
                delete temp;
                delete [] triangles;
            }
            //
        }
        else if (event.MouseInput.Event == irr::EMIE_RMOUSE_PRESSED_DOWN)
        {
            if (DecalBuffer)
            {
                delete DecalBuffer;
                DecalBuffer = 0;
            }
        }
        else if (event.MouseInput.Event == irr::EMIE_MMOUSE_PRESSED_DOWN)
        {
            Device->getSceneManager()->getActiveCamera()->setInputReceiverEnabled(!Device->getSceneManager()->getActiveCamera()->isInputReceiverEnabled());
        }
        else if (event.MouseInput.Event == irr::EMIE_MOUSE_WHEEL)
            node->setMaterialFlag(irr::video::EMF_WIREFRAME, event.MouseInput.Wheel > 0 ? true : false);
    }
    return false;
}

void CMain::run(void)
{
    irr::scene::ISceneManager* smgr = Device->getSceneManager();
    irr::video::IVideoDriver* driver = Device->getVideoDriver();

    /*bool r = true;
    do
    {
        driver->beginScene();
        r = Device->run();
        if (r)
        {
            smgr->drawAll();
            if (DecalBuffer)
            {
                driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
                driver->drawMeshBuffer(DecalBuffer);
            }
        }
        driver->endScene();
    }while(r);*/

    irr::video::SMaterial mat;
    mat.Lighting = false;
    //mat.NormalizeNormals = true;
    mat.TextureLayer[0].TextureWrapU = irr::video::ETC_CLAMP_TO_BORDER;
    mat.TextureLayer[0].TextureWrapV = irr::video::ETC_CLAMP_TO_BORDER;
    mat.setTexture(0, Device->getVideoDriver()->getTexture("leafs.png"));
    mat.ZWriteEnable = false;
    mat.MaterialType = irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL;

    irr::video::SMaterial wireDecal;
    wireDecal.Lighting = false;
    wireDecal.Wireframe = true;
    wireDecal.DiffuseColor = irr::video::SColor(255,255,0,0);
    wireDecal.AmbientColor = irr::video::SColor(255,255,0,0);
    wireDecal.EmissiveColor = irr::video::SColor(255,255,0,0);
    wireDecal.SpecularColor = irr::video::SColor(255,255,0,0);
    wireDecal.setTexture(0, Device->getVideoDriver()->getTexture("red.bmp"));

    irr::video::SMaterial groundDecal;
    wireDecal.Lighting = false;
    wireDecal.Wireframe = true;
    while(Device->run())
    {
        driver->beginScene();//true, true, irr::video::SColor(255,200,200,200));
        smgr->drawAll();
        if (DecalBuffer)
        {
            driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);


            //driver->setMaterial(groundDecal);
            //driver->drawMeshBuffer(DecalBuffer);

            driver->setMaterial(mat);
            driver->drawMeshBuffer(DecalBuffer);

            driver->setMaterial(wireDecal);
            driver->drawMeshBuffer(DecalBuffer);

            Decal->drawTripod();
        }
        driver->endScene();
    }
}

int main(void)
{
    CMain Main;
    Main.run();
    return 0;
}
