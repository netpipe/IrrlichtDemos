
#ifdef OPENSTEER
#include "cMeshTool.h"

datMesh getMesh_VehicleDisk(float radius, float height, bool showDirIndicator){
  datMesh mRet;
  mRet.Visible = true;

  irr::u32 vertexcount = 16;
  irr::core::vector3df normal(0,1,0);
  normal.normalize();

  //The equation of this circle is P + sV1 + tV2
  //where v1 id (0, c, -b) and v2 is (c, 0, -a), we need at least one nonzero one.
  irr::core::vector3df v1 = irr::core::vector3df(0, normal.Z, -normal.Y);
  irr::core::vector3df v2 = irr::core::vector3df(normal.Z, 0, -normal.X);
  if(v1 != irr::core::vector3df(0,0,0)){
    v1 = v1 / sqrt(1 - (normal.X * normal.X));
    v2 = v1.crossProduct(normal);
  }else{
    v2 = v2 / sqrt(1 - (normal.Y * normal.Y));
    v1 = v2.crossProduct(normal);
  }

  mRet.lineIndices.clear();
  mRet.lineVertices.clear();
  mRet.faceIndices.clear();
  mRet.faceVertices.clear();

//! wire frame (body)
  // lower level
  irr::video::S3DVertex vert;
  vert.Color = irr::video::SColor(255, 0,0,255);
  irr::u32 v1st = mRet.lineVertices.size();
  for(irr::u32 x = 0; x < vertexcount; ++x){
    vert.Pos = radius * cos(2 * irr::core::PI / vertexcount * x) * v1 + radius * sin(2 * irr::core::PI / vertexcount * x) * v2;
    mRet.lineVertices.push_back(vert);
    // lower circle line
    mRet.lineIndices.push_back(mRet.lineVertices.size() - 1);
    mRet.lineIndices.push_back(mRet.lineVertices.size());
  }
  mRet.lineIndices[mRet.lineIndices.size() - 1] = v1st;
  // upper level
  v1st = mRet.lineVertices.size();
  for(irr::u32 x = 0; x < vertexcount; ++x){
    vert.Pos = radius * cos(2 * irr::core::PI / vertexcount * x) * v1 + radius * sin(2 * irr::core::PI / vertexcount * x) * v2;
    vert.Pos.Y = height;
    mRet.lineVertices.push_back(vert);
    // connection line
    mRet.lineIndices.push_back(mRet.lineVertices.size() - 1);
    mRet.lineIndices.push_back(mRet.lineVertices.size() - 1 - vertexcount);
    // upper circle line
    mRet.lineIndices.push_back(mRet.lineVertices.size() - 1);
    mRet.lineIndices.push_back(mRet.lineVertices.size());
  }
  mRet.lineIndices[mRet.lineIndices.size() - 1] = v1st;

  if(showDirIndicator){
  //! faces (direction indicator)
    // 0 - front
    vert = mRet.lineVertices[8];
    vert.Pos.Y = height * 0.5;
    mRet.faceVertices.push_back(vert);
    // 1 - right back
    vert = mRet.lineVertices[2];
    vert.Pos.Y = height * 0.5;
    mRet.faceVertices.push_back(vert);
    // 2 - left back
    vert = mRet.lineVertices[14];
    vert.Pos.Y = height * 0.5;
    mRet.faceVertices.push_back(vert);
    // 3 - middle (back)
    vert.Pos = irr::core::vector3df(0, height * 0.5, radius * -0.3);
    mRet.faceVertices.push_back(vert);
    // lest faces
    mRet.faceIndices.push_back(2);
    mRet.faceIndices.push_back(3);
    mRet.faceIndices.push_back(0);
    mRet.faceIndices.push_back(2);
    mRet.faceIndices.push_back(0);
    mRet.faceIndices.push_back(3);
    // right faces
    mRet.faceIndices.push_back(3);
    mRet.faceIndices.push_back(1);
    mRet.faceIndices.push_back(0);
    mRet.faceIndices.push_back(3);
    mRet.faceIndices.push_back(0);
    mRet.faceIndices.push_back(1);
  }

  return mRet;
}

datMesh getMesh_Sphere(float radius){
  datMesh mRet;
  mRet.Visible = true;

  mRet.lineIndices.clear();
  mRet.lineVertices.clear();
  mRet.faceIndices.clear();
  mRet.faceVertices.clear();

  irr::video::S3DVertex vert;

  vert.Pos = irr::core::vector3df(0.000000,1.000000,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,0.865317,0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.250000,0.865317,0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.433013,0.865317,0.250000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.500000,0.865317,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,0.500000,0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.433013,0.500000,0.749861);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.750000,0.500000,0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.866025,0.500000,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,0.000000,1.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.500000,0.000000,0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.866025,0.000000,0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(1.000000,0.000000,0.000000);
  mRet.lineVertices.push_back(vert);
  mRet.lineIndices.push_back(0); mRet.lineIndices.push_back(1);
  mRet.lineIndices.push_back(0); mRet.lineIndices.push_back(2);
  mRet.lineIndices.push_back(0); mRet.lineIndices.push_back(3);
  mRet.lineIndices.push_back(0); mRet.lineIndices.push_back(4);
  mRet.lineIndices.push_back(1); mRet.lineIndices.push_back(2);
  mRet.lineIndices.push_back(2); mRet.lineIndices.push_back(3);
  mRet.lineIndices.push_back(3); mRet.lineIndices.push_back(4);
  mRet.lineIndices.push_back(1); mRet.lineIndices.push_back(5);
  mRet.lineIndices.push_back(2); mRet.lineIndices.push_back(6);
  mRet.lineIndices.push_back(3); mRet.lineIndices.push_back(7);
  mRet.lineIndices.push_back(4); mRet.lineIndices.push_back(8);
  mRet.lineIndices.push_back(5); mRet.lineIndices.push_back(6);
  mRet.lineIndices.push_back(6); mRet.lineIndices.push_back(7);
  mRet.lineIndices.push_back(7); mRet.lineIndices.push_back(8);
  mRet.lineIndices.push_back(5); mRet.lineIndices.push_back(9);
  mRet.lineIndices.push_back(6); mRet.lineIndices.push_back(10);
  mRet.lineIndices.push_back(7); mRet.lineIndices.push_back(11);
  mRet.lineIndices.push_back(8); mRet.lineIndices.push_back(12);
  mRet.lineIndices.push_back(9); mRet.lineIndices.push_back(10);
  mRet.lineIndices.push_back(10); mRet.lineIndices.push_back(11);
  mRet.lineIndices.push_back(11); mRet.lineIndices.push_back(12);

  int p1st = mRet.lineVertices.size();
  vert.Pos = irr::core::vector3df(0.000000,-1.000000,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,-0.865317,0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.250000,-0.865317,0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.433013,-0.865317,0.250000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.500000,-0.865317,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,-0.500000,0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.433013,-0.500000,0.749861);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.750000,-0.500000,0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.866025,-0.500000,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,-0.000000,1.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.500000,-0.000000,0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.866025,-0.000000,0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(1.000000,-0.000000,0.000000);
  mRet.lineVertices.push_back(vert);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 1);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 5);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 4); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 9);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 8); mRet.lineIndices.push_back(p1st + 12);
  mRet.lineIndices.push_back(p1st + 9); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 10); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 11); mRet.lineIndices.push_back(p1st + 12);

  p1st = mRet.lineVertices.size();
  vert.Pos = irr::core::vector3df(-0.000000,1.000000,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,0.865317,0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.250000,0.865317,0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.433013,0.865317,0.250000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.500000,0.865317,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,0.500000,0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.433013,0.500000,0.749861);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.750000,0.500000,0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.866025,0.500000,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,0.000000,1.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.500000,0.000000,0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.866025,0.000000,0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-1.000000,0.000000,0.000000);
  mRet.lineVertices.push_back(vert);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 1);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 5);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 4); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 9);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 8); mRet.lineIndices.push_back(p1st + 12);
  mRet.lineIndices.push_back(p1st + 9); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 10); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 11); mRet.lineIndices.push_back(p1st + 12);

  p1st = mRet.lineVertices.size();
  vert.Pos = irr::core::vector3df(-0.000000,-1.000000,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,-0.865317,0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.250000,-0.865317,0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.433013,-0.865317,0.250000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.500000,-0.865317,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,-0.500000,0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.433013,-0.500000,0.749861);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.750000,-0.500000,0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.866025,-0.500000,0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,-0.000000,1.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.500000,-0.000000,0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.866025,-0.000000,0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-1.000000,-0.000000,0.000000);
  mRet.lineVertices.push_back(vert);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 1);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 5);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 4); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 9);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 8); mRet.lineIndices.push_back(p1st + 12);
  mRet.lineIndices.push_back(p1st + 9); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 10); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 11); mRet.lineIndices.push_back(p1st + 12);

  p1st = mRet.lineVertices.size();
  vert.Pos = irr::core::vector3df(0.000000,1.000000,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,0.865317,-0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.250000,0.865317,-0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.433013,0.865317,-0.250000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.500000,0.865317,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,0.500000,-0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.433013,0.500000,-0.749861);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.750000,0.500000,-0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.866025,0.500000,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,0.000000,-1.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.500000,0.000000,-0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.866025,0.000000,-0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(1.000000,0.000000,-0.000000);
  mRet.lineVertices.push_back(vert);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 1);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 5);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 4); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 9);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 8); mRet.lineIndices.push_back(p1st + 12);
  mRet.lineIndices.push_back(p1st + 9); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 10); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 11); mRet.lineIndices.push_back(p1st + 12);

  p1st = mRet.lineVertices.size();
  vert.Pos = irr::core::vector3df(0.000000,-1.000000,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,-0.865317,-0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.250000,-0.865317,-0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.433013,-0.865317,-0.250000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.500000,-0.865317,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,-0.500000,-0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.433013,-0.500000,-0.749861);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.750000,-0.500000,-0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.866025,-0.500000,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.000000,-0.000000,-1.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.500000,-0.000000,-0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(0.866025,-0.000000,-0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(1.000000,-0.000000,-0.000000);
  mRet.lineVertices.push_back(vert);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 1);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 5);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 4); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 9);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 8); mRet.lineIndices.push_back(p1st + 12);
  mRet.lineIndices.push_back(p1st + 9); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 10); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 11); mRet.lineIndices.push_back(p1st + 12);

  p1st = mRet.lineVertices.size();
  vert.Pos = irr::core::vector3df(-0.000000,1.000000,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,0.865317,-0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.250000,0.865317,-0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.433013,0.865317,-0.250000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.500000,0.865317,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,0.500000,-0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.433013,0.500000,-0.749861);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.750000,0.500000,-0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.866025,0.500000,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,0.000000,-1.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.500000,0.000000,-0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.866025,0.000000,-0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-1.000000,0.000000,-0.000000);
  mRet.lineVertices.push_back(vert);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 1);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 5);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 4); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 9);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 8); mRet.lineIndices.push_back(p1st + 12);
  mRet.lineIndices.push_back(p1st + 9); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 10); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 11); mRet.lineIndices.push_back(p1st + 12);

  p1st = mRet.lineVertices.size();
  vert.Pos = irr::core::vector3df(-0.000000,-1.000000,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,-0.865317,-0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.250000,-0.865317,-0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.433013,-0.865317,-0.250000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.500000,-0.865317,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,-0.500000,-0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.433013,-0.500000,-0.749861);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.750000,-0.500000,-0.432874);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.866025,-0.500000,-0.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.000000,-0.000000,-1.000000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.500000,-0.000000,-0.865887);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-0.866025,-0.000000,-0.500000);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(-1.000000,-0.000000,-0.000000);
  mRet.lineVertices.push_back(vert);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 1);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 0); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 2);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 3);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 4);
  mRet.lineIndices.push_back(p1st + 1); mRet.lineIndices.push_back(p1st + 5);
  mRet.lineIndices.push_back(p1st + 2); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 3); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 4); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 6);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 7);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 8);
  mRet.lineIndices.push_back(p1st + 5); mRet.lineIndices.push_back(p1st + 9);
  mRet.lineIndices.push_back(p1st + 6); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 7); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 8); mRet.lineIndices.push_back(p1st + 12);
  mRet.lineIndices.push_back(p1st + 9); mRet.lineIndices.push_back(p1st + 10);
  mRet.lineIndices.push_back(p1st + 10); mRet.lineIndices.push_back(p1st + 11);
  mRet.lineIndices.push_back(p1st + 11); mRet.lineIndices.push_back(p1st + 12);

  float r = radius * 0.5;
  for(irr::u32 t = 0; t < mRet.lineVertices.size(); ++t)
    mRet.lineVertices[t].Pos *= irr::core::vector3df(r,r,r);

  return mRet;
}

datMesh getMesh_Plane(float w, float h){
  datMesh mRet;
  mRet.Visible = true;

  mRet.lineIndices.clear();
  mRet.lineVertices.clear();
  mRet.faceIndices.clear();
  mRet.faceVertices.clear();

  irr::video::S3DVertex vert;
  vert.Pos = irr::core::vector3df(w * -0.5, h * -0.5, 0.0);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w *  0.5, h * -0.5, 0.0);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w *  0.5, h *  0.5, 0.0);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w * -0.5, h *  0.5, 0.0);
  mRet.lineVertices.push_back(vert);

  mRet.lineIndices.push_back(0); mRet.lineIndices.push_back(1);
  mRet.lineIndices.push_back(1); mRet.lineIndices.push_back(2);
  mRet.lineIndices.push_back(2); mRet.lineIndices.push_back(3);
  mRet.lineIndices.push_back(3); mRet.lineIndices.push_back(0);

  return mRet;
}

datMesh getMesh_Box(float w, float h, float d){
  datMesh mRet;
  mRet.Visible = true;

  mRet.lineIndices.clear();
  mRet.lineVertices.clear();
  mRet.faceIndices.clear();
  mRet.faceVertices.clear();

  irr::video::S3DVertex vert;
  vert.Pos = irr::core::vector3df(w * -0.5, d * -0.5, h * -0.5);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w *  0.5, d * -0.5, h * -0.5);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w *  0.5, d * -0.5, h *  0.5);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w * -0.5, d * -0.5, h *  0.5);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w * -0.5, d *  0.5, h * -0.5);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w *  0.5, d *  0.5, h * -0.5);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w *  0.5, d *  0.5, h *  0.5);
  mRet.lineVertices.push_back(vert);
  vert.Pos = irr::core::vector3df(w * -0.5, d *  0.5, h *  0.5);
  mRet.lineVertices.push_back(vert);

  mRet.lineIndices.push_back(0); mRet.lineIndices.push_back(1);
  mRet.lineIndices.push_back(1); mRet.lineIndices.push_back(2);
  mRet.lineIndices.push_back(2); mRet.lineIndices.push_back(3);
  mRet.lineIndices.push_back(3); mRet.lineIndices.push_back(0);

  mRet.lineIndices.push_back(4); mRet.lineIndices.push_back(5);
  mRet.lineIndices.push_back(5); mRet.lineIndices.push_back(6);
  mRet.lineIndices.push_back(6); mRet.lineIndices.push_back(7);
  mRet.lineIndices.push_back(7); mRet.lineIndices.push_back(4);

  mRet.lineIndices.push_back(0); mRet.lineIndices.push_back(4);
  mRet.lineIndices.push_back(1); mRet.lineIndices.push_back(5);
  mRet.lineIndices.push_back(2); mRet.lineIndices.push_back(6);
  mRet.lineIndices.push_back(3); mRet.lineIndices.push_back(7);

  return mRet;
}

datMesh getMesh_Path(OpenSteer::PolylineSegmentedPathwaySegmentRadii* path){
  datMesh mRet;
  mRet.Visible = true;
  mRet.lineIndices.clear();
  mRet.lineVertices.clear();
  mRet.faceIndices.clear();
  mRet.faceVertices.clear();

  for(int t = 0; t < path->pointCount(); ++t){
    irr::video::S3DVertex vert;
    vert.Pos = path->point(t).vector3df();
    vert.Pos.Y += 0.1;
    mRet.lineVertices.push_back(vert);
    mRet.lineIndices.push_back(t); mRet.lineIndices.push_back(t + 1);
  }
  mRet.lineIndices.erase(mRet.lineIndices.size()-1);
  mRet.lineIndices.erase(mRet.lineIndices.size()-1);

  return mRet;
}
datMesh getMesh_Path(OpenSteer::PolylineSegmentedPathwaySingleRadius* path){
  datMesh mRet;
  mRet.Visible = true;
  mRet.lineIndices.clear();
  mRet.lineVertices.clear();
  mRet.faceIndices.clear();
  mRet.faceVertices.clear();

  for(int t = 0; t < path->pointCount(); ++t){
    irr::video::S3DVertex vert;
    vert.Pos = path->point(t).vector3df();
    vert.Pos.Y += 0.1;
    mRet.lineVertices.push_back(vert);
    mRet.lineIndices.push_back(t); mRet.lineIndices.push_back(t + 1);
  }
  mRet.lineIndices.erase(mRet.lineIndices.size()-1);
  mRet.lineIndices.erase(mRet.lineIndices.size()-1);

  return mRet;
}
#endif
