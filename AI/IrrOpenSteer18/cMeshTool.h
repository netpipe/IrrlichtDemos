
#ifdef OPENSTEER
#ifndef CMESHTOOL_H
#define CMESHTOOL_H

#include <irrlicht.h>
#include "cOpenSteerDemo.h"

struct datMesh{
  irr::core::matrix4 Transform;
  bool Visible;
  // for line drawings
  irr::core::array<irr::video::S3DVertex> lineVertices;
  irr::core::array<irr::u16> lineIndices;
  // for face drawings
  irr::core::array<irr::video::S3DVertex> faceVertices;
  irr::core::array<irr::u16> faceIndices;

  void setColor(irr::video::SColor col){
    for(irr::u32 t = 0; t < lineVertices.size(); ++t)
      lineVertices[t].Color = col;
    for(irr::u32 t = 0; t < faceVertices.size(); ++t)
      faceVertices[t].Color = col;
  }
  void draw(irr::video::IVideoDriver* driver){
    if(lineIndices.size() > 0)
      driver->drawVertexPrimitiveList(
          &lineVertices[0], lineVertices.size(),
          &lineIndices[0], lineIndices.size() / 2,
          irr::video::EVT_STANDARD,
          irr::scene::EPT_LINES,
          irr::video::EIT_16BIT);
    if(faceIndices.size() > 0)
      driver->drawVertexPrimitiveList(
          &faceVertices[0], faceVertices.size(),
          &faceIndices[0], faceIndices.size() / 3,
          irr::video::EVT_STANDARD,
          irr::scene::EPT_TRIANGLES,
          irr::video::EIT_16BIT);
  }

  void getBoundingbox(irr::core::aabbox3df* bRet){
    if(lineVertices.size() > 0)
      bRet->reset(lineVertices[0].Pos);
    else if(faceVertices.size() > 0)
      bRet->reset(faceVertices[0].Pos);
    else
      bRet->reset(irr::core::vector3df(0,0,0));
    for(irr::u32 t = 0; t < lineVertices.size(); ++t)
      bRet->addInternalPoint(lineVertices[t].Pos);
    for(irr::u32 t = 0; t < faceVertices.size(); ++t)
      bRet->addInternalPoint(faceVertices[t].Pos);
  }
  irr::core::aabbox3df getBoundingbox(){
    irr::core::aabbox3df bRet;
    if(lineVertices.size() > 0)
      bRet.reset(lineVertices[0].Pos);
    else if(faceVertices.size() > 0)
      bRet.reset(faceVertices[0].Pos);
    else
      bRet.reset(irr::core::vector3df(0,0,0));
    for(irr::u32 t = 0; t < lineVertices.size(); ++t)
      bRet.addInternalPoint(lineVertices[t].Pos);
    for(irr::u32 t = 0; t < faceVertices.size(); ++t)
      bRet.addInternalPoint(faceVertices[t].Pos);
    return bRet;
  }

};

datMesh getMesh_VehicleDisk(float radius=1.0, float height=1.0, bool showDirIndicator=true);
datMesh getMesh_Sphere(float radius);
datMesh getMesh_Plane(float w, float h);
datMesh getMesh_Box(float w, float h, float d);
datMesh getMesh_Path(OpenSteer::PolylineSegmentedPathwaySingleRadius* path);
datMesh getMesh_Path(OpenSteer::PolylineSegmentedPathwaySegmentRadii* path);

#endif // CMESHTOOL_H
#endif
