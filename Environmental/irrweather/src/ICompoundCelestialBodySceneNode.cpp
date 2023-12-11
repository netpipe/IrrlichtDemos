// This file is part of the "irrWeatherManager" weather management library for the Irrlicht rendering engine.
// The code in this file was separated from its original file, and the code belongs to Pazystamo from the Irrlicht forums.
// Original code from Pazystamo's ATMOSphere

#include "ICompoundCelestialBodySceneNode.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

ICompoundCelestialBodySceneNode::ICompoundCelestialBodySceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr,
    s32 id,vector3df position,const core::dimension2d<f32>& sunSize,const core::dimension2d<f32>& moonSize)
      : scene::ISceneNode(parent, mgr, id,position), SunSize(sunSize), MoonSize(moonSize)
{
        Material.ZBuffer = false;

   //setSize(size);

//   AutomaticCullingEnabled = false;

   indices[0] = 0;
   indices[1] = 2;
   indices[2] = 1;
   indices[3] = 0;
   indices[4] = 3;
   indices[5] = 2;

   vertices[0].TCoords.set(1.0f, 1.0f);
   vertices[0].Color = 0xffffffff;

   vertices[1].TCoords.set(1.0f, 0.0f);
   vertices[1].Color = 0xffffffff;

   vertices[2].TCoords.set(0.0f, 0.0f);
   vertices[2].Color = 0xffffffff;

   vertices[3].TCoords.set(0.0f, 1.0f);
   vertices[3].Color = 0xffffffff;

   //MoonMaterial();
   MoonMaterial.setTexture(0, SceneManager->getVideoDriver()->getTexture("moon.tga"));
   MoonMaterial.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;
   MoonMaterial.Lighting = false;

   MoonPosition = vector3df(0,0,0);
}

//virtual void setSize(const core::dimension2d<f32>& size);
void ICompoundCelestialBodySceneNode::OnRegisterSceneNode()
{
    if (IsVisible)
    {
        SceneManager->registerNodeForRendering(this,ESNRP_SKY_BOX);
        ISceneNode::updateAbsolutePosition();//realy helps from sun pos lag
    }
    ISceneNode::OnRegisterSceneNode();
}

void ICompoundCelestialBodySceneNode::render()
{
   video::IVideoDriver* driver = SceneManager->getVideoDriver();
   ICameraSceneNode* camera = SceneManager->getActiveCamera();

   if (!camera || !driver)
      return;

   // make billboard look to camera
   core::vector3df pos = getAbsolutePosition();
   core::vector3df campos = camera->getAbsolutePosition();
   core::vector3df target = camera->getTarget();
   core::vector3df up = camera->getUpVector();
   core::vector3df view = target - campos;
   view.normalize();

   core::vector3df horizontal = up.crossProduct(view);
   horizontal.normalize();

   core::vector3df vertical = horizontal.crossProduct(view);
   vertical.normalize();

   vector3df horizontal2 = horizontal;
   vector3df vertical2 = vertical;

   horizontal *= 0.5f * SunSize.Width;
   vertical *= 0.5f * SunSize.Height;

   vertices[0].Pos = pos + campos + horizontal + vertical;
   vertices[1].Pos = pos + campos + horizontal - vertical;
   vertices[2].Pos = pos + campos - horizontal - vertical;
   vertices[3].Pos = pos + campos - horizontal + vertical;

   view *= -1.0f;

   for (s32 i=0; i<4; ++i)
      vertices[i].Normal = view;

   // draw

   if (DebugDataVisible)
   {
      driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
      video::SMaterial m;
      m.Lighting = false;
      driver->setMaterial(m);
      driver->draw3DBox(BBox, video::SColor(0,208,195,152));
   }

   core::matrix4 mat;
    driver->setTransform(video::ETS_WORLD, mat);

   driver->setMaterial(Material);

   driver->drawIndexedTriangleList(vertices, 4, indices, 2);


   // Draw the moon
   horizontal2 *= 0.5f * MoonSize.Width;
   vertical2 *= 0.5f * MoonSize.Height;

   MoonPosition = -pos;

   vertices[0].Pos = MoonPosition + campos + horizontal2 + vertical2;
   vertices[1].Pos = MoonPosition + campos + horizontal2 - vertical2;
   vertices[2].Pos = MoonPosition + campos - horizontal2 - vertical2;
   vertices[3].Pos = MoonPosition + campos - horizontal2 + vertical2;

   view *= -1.0f;

   for (s32 i=0; i<4; ++i)
     vertices[i].Normal = view;

   driver->setTransform(video::ETS_WORLD, mat);

   driver->setMaterial(MoonMaterial);

   driver->drawIndexedTriangleList(vertices, 4, indices, 2);
}

const core::aabbox3d<f32>& ICompoundCelestialBodySceneNode::getBoundingBox() const
{
      return BBox;
}

//! sets the size of the billboard
/*void ICompoundCelestialBodySceneNode::setSize(const core::dimension2d<f32>& size)
{
   Size = size;

   if (Size.Width == 0.0f)
      Size.Width = 1.0f;

   if (Size.Height == 0.0f )
      Size.Height = 1.0f;

   f32 avg = (size.Width + size.Height)/6;
   BBox.MinEdge.set(-avg,-avg,-avg);
   BBox.MaxEdge.set(avg,avg,avg);
}*/

u32 ICompoundCelestialBodySceneNode::getMaterialCount() const
{
      return 1;
}

video::SMaterial& ICompoundCelestialBodySceneNode::getMaterial(u32 i)
{
      return Material;
}

//change sky texture
video::SMaterial& ICompoundCelestialBodySceneNode::setMaterial(video::ITexture* tex)
{
        Material.setTexture(0,tex);
        return Material;
}

//update uv maping x coordinate
/*const core::dimension2d<f32>& ICompoundCelestialBodySceneNode::getSize()
{
    return Size;
}*/
