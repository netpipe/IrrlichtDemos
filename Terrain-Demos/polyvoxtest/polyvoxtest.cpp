/*******************************************************************************
Copyright (c) 2005-2009 David Williams

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
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

    3. This notice may not be removed or altered from any source
    distribution.
*******************************************************************************/

//#include "OpenGLWidget.h"

//#include "PolyVoxCore/MaterialDensityPair.h"
//#include "PolyVoxCore/Material.h"
//#include "PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
//#include "PolyVoxCore/SurfaceMesh.h"
//#include "PolyVoxCore/SimpleVolume.h"

#include "include/PolyVoxCore/MaterialDensityPair.h"
#include "include/PolyVoxCore/Material.h"
#include "include/PolyVoxCore/CubicSurfaceExtractorWithNormals.h"
#include "include/PolyVoxCore/SurfaceMesh.h"
#include "include/PolyVoxCore/SimpleVolume.h"

//#include <QApplication>
#include <stdio.h>
#include <math.h>
#include <ctime>
#include <irrlicht.h>
//#ifdef _IRR WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
//#pragma comment((linker, "/subsystem:windows /ENTRY:mainCRTStartup")
//#endif

//using namespace PolyVox;
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int volumevoxel=63;
PolyVox::SimpleVolume<uint8_t> volData(PolyVox::Region(PolyVox::Vector3DInt32(0,0,0), PolyVox::Vector3DInt32(volumevoxel, volumevoxel, volumevoxel)));

const unsigned int size=63;
#define RANGE 1.2
const double xlow=-RANGE;
const double xhigh=RANGE;
const double ylow=-RANGE;
const double yhigh=RANGE;
const double zlow=-RANGE;
const double zhigh=RANGE;

const unsigned int maxiterations=80;
const double mandpow=8.0;
double r;

double valInRange(double low, double high, unsigned int size, unsigned int off)
{
  return low+((high-low)/(double)size)*(double)off;
}

unsigned int doPoint(double cx, double cy, double cz)
{
  // program from http://www.treblig.org/3dbrot/3dbrot.c
  double x,y,z;
  double newx,newy,newz;
  double theta,phi,rpow;
  //double r;
  unsigned int i;
  x=0.0;
  y=0.0;
  z=0.0;

  for(i=0;(i<maxiterations) && ((x*x+y*y+z*z) < 2.0);i++)
  {
   /* These maths from http://www.skytopia.com/project/fractal/mandelbulb.html */
    r = sqrt(x*x + y*y + z*z );
    theta = atan2(sqrt(x*x + y*y) , z);
    phi = atan2(y,x);
    rpow = pow(r,mandpow);

    newx = rpow * sin(theta*mandpow) * cos(phi*mandpow);
    newy = rpow * sin(theta*mandpow) * sin(phi*mandpow);
    newz = rpow * cos(theta*mandpow);

    x=newx+cx;
    y=newy+cy;
    z=newz+cz;
  }
  return i;
}


void createMandelbulb(PolyVox::SimpleVolume<uint8_t>& volData)
{
   uint8_t uValue=1;
   //This three-level for loop iterates over every voxel in the volume
   for (int z = 0; z < volData.getWidth(); z++)
   {
      double fz=valInRange(zlow, zhigh, size, z);
      for (int y = 0; y < volData.getHeight(); y++)
      {
         double fy=valInRange(ylow, yhigh, size, y);
         for (int x = 0; x < volData.getDepth(); x++)
         {
            double fx=valInRange(xlow, xhigh, size, x);
            unsigned int val=doPoint(fx,fy,fz);
            //printf("%i ",val);
            if (val>=maxiterations-1)
            {
               uValue=r*12; //for example use r for material
               volData.setVoxelAt(x, y, z, uValue);
            }
         }
      }
   }
}


void createRandom(PolyVox::SimpleVolume<PolyVox::Material8>& volData)
{
   PolyVox::Material8 uValue=1;
   //This three-level for loop iterates over every voxel in the volume
   for (int z = 0; z < volData.getWidth(); z++)
   {
      double fz=valInRange(zlow, zhigh, size, z);
      for (int y = 0; y < volData.getHeight(); y++)
      {
               uValue=r*12; //for example use r for material
//               volData.setVoxelAt(x, y, z, uValue);
      }
   }
}


irr::video::SColor ConvertColour(uint8_t materialID)
{
    irr::video::SColor colour;

    switch(materialID)
    {
    case 0://black
        colour.setRed(0);
        colour.setGreen(0);
        colour.setBlue(0);
        break;
    case 1://red
        colour.setRed(255);
        colour.setGreen(0);
        colour.setBlue(0);
        break;
    case 2://orange
        colour.setRed(255);
        colour.setGreen(128);
        colour.setBlue(0);
        break;
    case 3://yellow
        colour.setRed(255);
        colour.setGreen(255);
        colour.setBlue(0);
        break;
    case 4://green
        colour.setRed(0);
        colour.setGreen(255);
        colour.setBlue(0);
        break;
    case 5://lightblue
        colour.setRed(0);
        colour.setGreen(255);
        colour.setBlue(255);
        break;
    case 6://blue
        colour.setRed(0);
        colour.setGreen(0);
        colour.setBlue(255);
        break;
    case 7://pink
        colour.setRed(255);
        colour.setGreen(0);
        colour.setBlue(255);
        break;
    case 8://darkred
        colour.setRed(128);
        colour.setGreen(0);
        colour.setBlue(0);
        break;
    case 9://darkgreen
        colour.setRed(0);
        colour.setGreen(128);
        colour.setBlue(0);
        break;
    case 10://darkblue
        colour.setRed(0);
        colour.setGreen(0);
        colour.setBlue(128);
        break;
    case 11://purple
        colour.setRed(128);
        colour.setGreen(0);
        colour.setBlue(128);
        break;
    case 12://turquoise
        colour.setRed(0);
        colour.setGreen(128);
        colour.setBlue(255);
        break;
    case 13://grey
        colour.setRed(150);
        colour.setGreen(150);
        colour.setBlue(150);
        break;
    case 14://white
        colour.setRed(255);
        colour.setGreen(255);
        colour.setBlue(255);
        break;
    case 15://random
        colour.setRed((rand()%255)+1);
        colour.setGreen((rand()%255)+1);
        colour.setBlue((rand()%255)+1);
        break;
    case 254://random
        colour.setRed((rand()%255)+1);
        colour.setGreen((rand()%255)+1);
        colour.setBlue((rand()%255)+1);
        break;
    default://black
        colour.setRed(0);
        colour.setGreen(0);
        colour.setBlue(0);
    }
    return colour;
}


irr::scene::IDynamicMeshBuffer *mb = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_STANDARD, irr::video::EIT_32BIT);
//irr::scene::IDynamicMeshBuffer* ConvertMesh(PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal>& mesh)
irr::scene::IMeshBuffer* ConvertMesh(PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal>& mesh)
{
    const std::vector<uint32_t>& indices = mesh.getIndices();
    const std::vector<PolyVox::PositionMaterialNormal>& vertices = mesh.getVertices();

    //irr::scene::IDynamicMeshBuffer *mb = new irr::scene::CDynamicMeshBuffer(irr::video::EVT_STANDARD, irr::video::EIT_32BIT);
    mb->getVertexBuffer().set_used(vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i)
    {
        const PolyVox::Vector3DFloat& position = vertices[i].getPosition();
        const PolyVox::Vector3DFloat& normal = vertices[i].getNormal();
        mb->getVertexBuffer()[i].Pos.X = position.getX();
        mb->getVertexBuffer()[i].Pos.Y = position.getY();
        mb->getVertexBuffer()[i].Pos.Z = position.getZ();
        mb->getVertexBuffer()[i].Normal.X = normal.getX();
        mb->getVertexBuffer()[i].Normal.Y = normal.getY();
        mb->getVertexBuffer()[i].Normal.Z = normal.getZ();

        //mb->getVertexBuffer()[i].TCoords.X = position.getX();
        //mb->getVertexBuffer()[i].TCoords.Y = position.getY();

        if(fabsf(normal.getX())>fabsf(normal.getY()) && fabsf(normal.getX())>fabsf(normal.getZ())) {
            mb->getVertexBuffer()[i].TCoords.X = position.getY();
            mb->getVertexBuffer()[i].TCoords.Y = position.getZ();
        } else if(fabsf(normal.getY())>fabsf(normal.getX()) && fabsf(normal.getY())>fabsf(normal.getZ())) {
            mb->getVertexBuffer()[i].TCoords.X = position.getX();
            mb->getVertexBuffer()[i].TCoords.Y = position.getZ();
        } else {
            mb->getVertexBuffer()[i].TCoords.X = position.getX();
            mb->getVertexBuffer()[i].TCoords.Y = position.getY();
        }

        //if (position.getY()==0 && position.getZ()>0)mb->getVertexBuffer()[i].TCoords.Y = position.getZ();
//uint8_t material = vertex.getMaterial() + 0.5;+
        mb->getVertexBuffer()[i].Color = ConvertColour(vertices[i].getMaterial()+0.5);
        //mb->getVertexBuffer()[i].Color = ConvertColour2(vertices[i].getMaterial()+0.5,1,0.5,0);
    }
    mb->getIndexBuffer().set_used(indices.size());
    for (size_t i = 0; i < indices.size(); ++i)
    {
        mb->getIndexBuffer().setValue(i, indices[i]);
    }
    mb->recalculateBoundingBox();
    return mb;
}





int main(int argc, char *argv[])
{
    wchar_t cBuffer[100];
    int move;

    srand(unsigned(time(NULL)));
//video::EDT_DIRECT3D9
//    IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9 , core::dimension2d<u32>(1024, 768),
  //      32, false, false, false, 0);
    IrrlichtDevice* device = createDevice(video::EDT_OPENGL, core::dimension2d<u32>(1024, 768), 16,false,false);
    if (device == 0)
        return 1; // could not create selected driver.
    gui::IGUIEnvironment* gui = device->getGUIEnvironment();
    video::IVideoDriver* driver = device->getVideoDriver();
    irr::scene::ISceneManager* smgr = device->getSceneManager();

    irr::SKeyMap keyMap[5];
    keyMap[0].Action = irr::EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = irr::KEY_KEY_W;
    keyMap[1].Action = irr::EKA_MOVE_BACKWARD;
    keyMap[1].KeyCode = irr::KEY_KEY_S;
    keyMap[2].Action = irr::EKA_STRAFE_LEFT;
    keyMap[2].KeyCode = irr::KEY_KEY_A;
    keyMap[3].Action = irr::EKA_STRAFE_RIGHT;
    keyMap[3].KeyCode = irr::KEY_KEY_D;
    keyMap[4].Action = irr::EKA_JUMP_UP;
    keyMap[4].KeyCode = irr::KEY_SPACE;

    irr::scene::ICameraSceneNode *cam = smgr->addCameraSceneNodeFPS(0,30.0f, 0.1f, -1, keyMap, 5);

    cam->setPosition(core::vector3df(0, 30, -60));


    createMandelbulb(volData);

    //Extract the surface
    PolyVox::SurfaceMesh<PolyVox::PositionMaterialNormal> polyvoxmesh;
    PolyVox::CubicSurfaceExtractorWithNormals<PolyVox::SimpleVolume< uint8_t >> surfaceExtractor(&volData, volData.getEnclosingRegion(), &polyvoxmesh);
    surfaceExtractor.execute();

SMesh * testmesh = new SMesh();


    irr::scene::IMeshBuffer * testbuffer = ConvertMesh(polyvoxmesh);
    printf("%i ",testbuffer->getVertexCount());
    printf("%i ",testbuffer->getIndexCount());
    testmesh->addMeshBuffer(testbuffer);
    testmesh->recalculateBoundingBox();


    scene::ISceneNode * testnode = smgr->addMeshSceneNode(testmesh); //, core::vector3df(2000, 1000, 2000),core::vector3df(0, 100, 0),core::vector3df(20.0F, 20.0F, 20.0F));


    testnode->setMaterialFlag(EMF_LIGHTING, true);

    testnode->setMaterialType( video::EMT_SOLID );

    testnode->setMaterialTexture(0, driver->getTexture("textureatlas.bmp"));
    testnode->getMaterial(0).getTextureMatrix(0).setRotationDegrees(
                        core::vector3d<f32>(0.0f, 0.0f, 180 ));
    testnode->getMaterial(0).getTextureMatrix(0).setTextureTranslate(0.5f,0.5f);
    testnode->setRotation(core::vector3df(0.0f, 180.0f, 0.0f));


    ILightSceneNode *lightnode = smgr->addLightSceneNode(0, vector3df(0,100,0),
        SColorf(1.0f, 1.0f, 1.0f, 1.0f), 200.0f);

    gui::IGUIFont* font =
        device->getGUIEnvironment()->getFont("arialbold.bmp");


    while(device->run()){
            if(device->isWindowActive()){


                driver->beginScene(true, true, SColor(0xff444444));
                smgr->drawAll();


                driver->endScene();

            }
    }
    device->drop();
    return 1;

}
