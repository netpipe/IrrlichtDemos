// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and most of the code belongs to Pazystamo from the Irrlicht forums.
// Original code from Pazystamo's ATMOSphere

#include "IAtmosphereSkySceneNode.h"
#include <ISceneManager.h>
#include <ICameraSceneNode.h>
#include <IVideoDriver.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

IAtmosphereSkySceneNode::IAtmosphereSkySceneNode(irr::video::ITexture* const tex,irr::scene::ISceneNode* const parent, irr::scene::ISceneManager* const mgr, irr::s32 faces, irr::s32 id)
      : irr::scene::ISceneNode(parent, mgr, id)
{
        //AutomaticCullingEnabled = false;
        irr::video::SMaterial mat;
        mat.Lighting = false;
        //mat.Wireframe = true;
        mat.ZBuffer = false;
        //mat.ZWriteEnable = false;
        mat.setFlag(irr::video::EMF_BILINEAR_FILTER,true);
        //mat.NormalizeNormals=true;
        //mat.AnisotropicFilter=true;
        //mat.GouraudShading=false;
        //mat.TrilinearFilter = true;
        //mat.BackfaceCulling = false;
        face=faces;
        Vertices = new irr::video::S3DVertex[face+1];
        indices = new irr::u16[face*3];
        Material = mat;
        Material.setTexture(0,tex);
        irr::f64 angle = 0.0f;                //start positions
        irr::f64 angle2 = 360.0f/face;        //angle to add
        vert=0;                          //vertice nr
        irr::s32 nr = -3;                     //indices nr
        //top vertice
        Vertices[0]=irr::video::S3DVertex(0.0f, 100.0f, -0.0f,
         0.0568988f, 0.688538f, -0.722965f,
          irr::video::SColor(255,255,255,255), 0.0f, 0.1f);
        for (irr::u16 n=1;n<face+1;++n){
            vert=vert+1;
            nr=nr+3;                        //indices number
            irr::f64 x=cos(angle*0.017453292519943295769236907684886f)*100;//vertice x coord
            irr::f64 z=sin(angle*0.017453292519943295769236907684886f)*100;//vertice z coord
            Vertices[vert]=irr::video::S3DVertex(x, -5.0f, z,
               0.0568988f, 0.688538f, -0.722965f,
               irr::video::SColor(255,255,255,255), 0.0f, 0.9f);
            angle=angle+angle2;
            //connects face
            indices[nr]=0;                  //top vertice
            indices[nr+1]=vert;             //bottom vertice
            indices[nr+2]=vert+1;           //next bottom vertice
        }
        indices[nr+2]=1;                 //connect last bottom vertice with first
}

void IAtmosphereSkySceneNode::OnRegisterSceneNode()
{
      if (IsVisible)
         SceneManager->registerNodeForRendering(this,ESNRP_SKY_BOX);
      ISceneNode::OnRegisterSceneNode();
}

void IAtmosphereSkySceneNode::render()
{
      irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();
        irr::scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();
       if (!camera || !driver)
         return;
       irr::core::matrix4 mat;
       mat.setTranslation(camera->getAbsolutePosition());
       //attach node to camera
       driver->setTransform(video::ETS_WORLD, mat);
       //don't attach camera
       //driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
       driver->setMaterial(Material);
       //update uv maping
        for (int i=1;i<face+1;++i){
            Vertices[i].TCoords=core::vector2d< irr::f32 >(uvX,0.98f);
        }
        Vertices[0].TCoords=core::vector2d< irr::f32 >(uvX,0.01f);
        driver->drawIndexedTriangleList(&Vertices[0],vert+1,&indices[0],face);
}

const irr::core::aabbox3d<irr::f32>& IAtmosphereSkySceneNode::getBoundingBox() const
{
      return Box;
}

irr::u32 IAtmosphereSkySceneNode::getMaterialCount() const
{
      return 1;
}

irr::video::SMaterial& IAtmosphereSkySceneNode::getMaterial(irr::u32 i)
{
      return Material;
}

//change sky texture
irr::video::SMaterial& IAtmosphereSkySceneNode::setMaterial(irr::video::ITexture* const tex)
{
      Material.setTexture(0, tex);
      return Material;
}

//update uv maping x coordinate
void IAtmosphereSkySceneNode::setuvX(irr::f64 v)
{
      uvX=v;
}

IAtmosphereSkySceneNode::~IAtmosphereSkySceneNode()
{
    delete Vertices;
    delete indices;
}
