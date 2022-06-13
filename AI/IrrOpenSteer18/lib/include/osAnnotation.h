/*! \file osAnnotation.h
    ----------------------------------------------------------------------------<br>
    Irrlicht implementation by A.Buschhüter<br>
    http://abusoft.g0dsoft.com<br>
    abusoft@g0dsoft.com<br>
    ----------------------------------------------------------------------------<br>
    OpenSteer -- Steering Behaviors for Autonomous Characters<br>
    <br>
    Copyright (c) 2002-2005, Sony Computer Entertainment America<br>
    Original author: Craig Reynolds <craig_reynolds@playstation.sony.com><br>
    <br>
    Permission is hereby granted, free of charge, to any person obtaining a<br>
    copy of this software and associated documentation files (the "Software"),<br>
    to deal in the Software without restriction, including without limitation<br>
    the rights to use, copy, modify, merge, publish, distribute, sublicense,<br>
    and/or sell copies of the Software, and to permit persons to whom the<br>
    Software is furnished to do so, subject to the following conditions:<br>
    <br>
    The above copyright notice and this permission notice shall be included in<br>
    all copies or substantial portions of the Software.<br>
    <br>
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR<br>
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,<br>
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL<br>
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER<br>
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING<br>
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER<br>
    DEALINGS IN THE SOFTWARE.<br>
    ----------------------------------------------------------------------------<br>
*/
#include "../../../../config.h"
#ifdef OPENSTEER
#ifndef OPENSTEER_ANNOTATION_H
#define OPENSTEER_ANNOTATION_H

#include "IrrOpenSteer.h"
using namespace irr;

//! namespace OpenSteer
namespace OpenSteer{

  extern bool enableAnnotation;
  extern bool drawPhaseActive;

  //! graphical annotation: master on/off switch
  inline bool annotationIsOn(void){ return enableAnnotation; }
  //! graphical annotation: master on/off switch
  inline void setAnnotationOn(void){ enableAnnotation = true; }
  //! graphical annotation: master on/off switch
  inline void setAnnotationOff(void){ enableAnnotation = false; }
  //! graphical annotation: master on/off switch
  inline bool toggleAnnotationState(void){ return (enableAnnotation = !enableAnnotation); }

  //! struct for annotation (master, eg. for mixed arrays)
  struct datAnnotation{
  // 3D Annotation
    core::matrix4 AbsoluteTransformation;       /*!< absolute world transormation */
    // for line drawings
    core::array<video::S3DVertex> lineVertices; /*!< list of vertices for lines */
    core::array<u16> lineIndices;               /*!< list of indices for lines */
    // for face drawings
    core::array<video::S3DVertex> faceVertices; /*!< list of vertices for faces */
    core::array<u16> faceIndices;

    datAnnotation(){
      AbsoluteTransformation = core::matrix4();
    }
    /*!< list of indices for faces */
    //! set the color of the mesh
    void setMeshColor(video::SColor col){
      for(u32 t = 0; t < lineVertices.size(); ++t)
        lineVertices[t].Color = col;
      for(u32 t = 0; t < faceVertices.size(); ++t)
        faceVertices[t].Color = col;
    }
    //! set the 3d position
    void setPosition(core::vector3df p){
      AbsoluteTransformation.setTranslation(p);
    }
    //! set the 3d rotation
    void setRotation(core::vector3df r){
      AbsoluteTransformation.setRotationDegrees(r);
    }
    //! render annotation (3d)
    void render(video::IVideoDriver* driver){
      if(!driver) return;
      driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
      if(faceIndices.size() > 2){
        driver->drawVertexPrimitiveList(
            &faceVertices[0], faceVertices.size(),
            &faceIndices[0], faceIndices.size() / 3,
            video::EVT_STANDARD,
            scene::EPT_TRIANGLES,
            video::EIT_16BIT);
      }
      if(lineIndices.size() > 1){
        driver->drawVertexPrimitiveList(
            &lineVertices[0], lineVertices.size(),
            &lineIndices[0], lineIndices.size() / 2,
            video::EVT_STANDARD,
            scene::EPT_LINES,
            video::EIT_16BIT);
      }
    }

  // 2D Annotation
    //! draw annotation (2d)
    void draw(gui::IGUIEnvironment* env){}


  // Show Both  Annotations (2D and 3D)
    //! show annotation
    void show(video::IVideoDriver* driver, gui::IGUIEnvironment* env){
      render(driver);
      draw(env);
    }
    //! show annotation
    void show(IrrlichtDevice* device){
      if(device){
        render(device->getVideoDriver());
        draw(device->getGUIEnvironment());
      }
    }
  };

  //! clear the mesh
  inline void clearAnnotation_Mesh(datAnnotation* mesh){
    mesh->faceIndices.clear();
    mesh->faceVertices.clear();
    mesh->lineIndices.clear();
    mesh->lineVertices.clear();
  }

  //! add a line to the mesh
  inline void addLineToAnnotation_Mesh(datAnnotation* mesh, Vec3 p1, Vec3 p2, video::SColor col=video::SColor(255, 0,200,200)){
    //! add line to mesh
    // Vertex 1
    { video::S3DVertex v(p1.vector3df(),  // Position
      core::vector3df(0,1,0),             // Normal
      col,                                // Color
      core::vector2df(1,1));              // TCoord
      mesh->lineVertices.push_back(v);    // Index = 0
    }
    // Vertex 2
    { video::S3DVertex v(p2.vector3df(),  // Position
      core::vector3df(0,1,0),             // Normal
      col,                                // Color
      core::vector2df(1,1));              // TCoord
      mesh->lineVertices.push_back(v);    // Index = 0
    }
    // Indices
    mesh->lineIndices.push_back(mesh->lineVertices.size() - 2); // Vertex 1
    mesh->lineIndices.push_back(mesh->lineVertices.size() - 1); // Vertex 2
  }
  //! add a point to a annotation trail mesh (maxV = 0 -> no limit, not recomended for performance)
// TODO (Acki#2#): max-length for trail mesh
  inline void addPointToAnnotation_Trail(datAnnotation* mesh, Vec3 p, video::SColor col=video::SColor(255, 200,0,0)){
    if(mesh->lineVertices.size() == 0){
      //! add 1st vertex
      video::S3DVertex v(p.vector3df(),  // Position
      core::vector3df(0,1,0),             // Normal
      col,                                // Color
      core::vector2df(1,1));              // TCoord
      mesh->lineVertices.push_back(v);    // Index = 0
    }else{
      //! add vertex
      if(distance(p, mesh->lineVertices[mesh->lineVertices.size()-1].Pos) > 1.0){
        video::S3DVertex v(p.vector3df(),  // Position
        core::vector3df(0,1,0),             // Normal
        col,                                // Color
        core::vector2df(1,1));              // TCoord
        mesh->lineVertices.push_back(v);
      }
    }
    // Indices
    if(mesh->lineVertices.size() > 1){
      mesh->lineIndices.push_back(mesh->lineVertices.size() - 2); // Vertex 1
      mesh->lineIndices.push_back(mesh->lineVertices.size() - 1); // Vertex 2
    }
  }


  //! get mesh for ground
  inline void getAnnotationMesh_Ground(datAnnotation* mesh,
            float sizeX, int countX,
            float sizeZ, int countZ,
            bool filled = true,
            bool drawLines = false,
            video::SColor col1 = video::SColor(255, 150,150,150),
            video::SColor col2 = video::SColor(255, 200,200,200),
            video::SColor col3 = video::SColor(255, 0,0,0)){
    // Clear Mesh
    clearAnnotation_Mesh(mesh);

    // Square-Size
    float sqW = sizeX / (float)countX;
    float sqH = sizeZ / (float)countZ;
    // Start-Vertex
    video::S3DVertex vert(core::vector3df(sizeX * -0.5, 0, sizeZ * -0.5),
      core::vector3df(0,1,0), col1, core::vector2df(1,1));
    bool start1st = true;
    for(int z = 0; z < countZ; ++z){
      bool use1st = start1st = !start1st;
      for(int x = 0; x < countX; ++x){
        //! create mesh for faces
        if(filled){
          // change color
          use1st = !use1st;
          use1st ? vert.Color = col1 : vert.Color = col2;
          //! vertices
          int v0 = mesh->faceVertices.size();
          // 0 - unten links
          { video::S3DVertex v = vert;
            mesh->faceVertices.push_back(v);
          }
          // 1 - unten recht
          { video::S3DVertex v = vert;
            v.Pos.X += sqW;
            mesh->faceVertices.push_back(v);
          }
          // 2 - oben recht
          { video::S3DVertex v = vert;
            v.Pos.X += sqW;
            v.Pos.Z += sqH;
            mesh->faceVertices.push_back(v);
          }
          // 3 - oben links
          { video::S3DVertex v = vert;
            v.Pos.Z += sqH;
            mesh->faceVertices.push_back(v);
          }
          //! indices
          // triangle 1
          mesh->faceIndices.push_back(v0 + 0);
          mesh->faceIndices.push_back(v0 + 2);
          mesh->faceIndices.push_back(v0 + 1);
          // triangle 2
          mesh->faceIndices.push_back(v0 + 0);
          mesh->faceIndices.push_back(v0 + 3);
          mesh->faceIndices.push_back(v0 + 2);
        }
        //! create mesh for lines
        if(drawLines){
          // change color
          vert.Color = col3;
          //! vertices
          int v0 = mesh->lineVertices.size();
          // 0 - unten links
          { video::S3DVertex v = vert;
            mesh->lineVertices.push_back(v);
          }
          // 1 - unten recht
          { video::S3DVertex v = vert;
            v.Pos.X += sqW;
            mesh->lineVertices.push_back(v);
          }
          // 2 - oben recht
          { video::S3DVertex v = vert;
            v.Pos.X += sqW;
            v.Pos.Z += sqH;
            mesh->lineVertices.push_back(v);
          }
          // 3 - oben links
          { video::S3DVertex v = vert;
            v.Pos.Z += sqH;
            mesh->lineVertices.push_back(v);
          }
          //! indices
          // line 1
          mesh->lineIndices.push_back(v0 + 0); mesh->lineIndices.push_back(v0 + 1);
          // line 2
          mesh->lineIndices.push_back(v0 + 1); mesh->lineIndices.push_back(v0 + 2);
          // line 3
          mesh->lineIndices.push_back(v0 + 2); mesh->lineIndices.push_back(v0 + 3);
          // line 4
          mesh->lineIndices.push_back(v0 + 3); mesh->lineIndices.push_back(v0 + 0);
        }
        vert.Pos.X += sqW;
      }
      vert.Pos.X = sizeX * -0.5;
      vert.Pos.Z += sqH;
    }
    // create mesh for lines

  }


  //! get mesh for disk
  inline void getAnnotationMesh_Disk(datAnnotation* mesh, float radius=1.0, float height=0.0, bool showDirIndicator=true, video::SColor col=video::SColor(255, 0,200,0), int slices=16){
    clearAnnotation_Mesh(mesh);

    u32 vertexcount = slices;
    core::vector3df normal(0,1,0);
    normal.normalize();

    //The equation of this circle is P + sV1 + tV2
    //where v1 id (0, c, -b) and v2 is (c, 0, -a), we need at least one nonzero one.
    core::vector3df v1 = core::vector3df(0, normal.Z, -normal.Y);
    core::vector3df v2 = core::vector3df(normal.Z, 0, -normal.X);
    if(v1 != core::vector3df(0,0,0)){
      v1 = v1 / sqrt(1.0 - (normal.X * normal.X));
      v2 = v1.crossProduct(normal);
    }else{
      v2 = v2 / sqrt(1.0 - (normal.Y * normal.Y));
      v1 = v2.crossProduct(normal);
    }

    double tmp = 2.0 * irr::core::PI / vertexcount;
    // lower level
    irr::u32 v1st = mesh->lineVertices.size();
    for(irr::u32 x = 0; x < vertexcount; ++x){
      core::vector3df pos = radius * cos(tmp * x) * v1 + radius * sin(tmp * x) * v2;
      pos.Y = 0;
      { video::S3DVertex v(pos,             // Position
        core::vector3df(0,1,0),             // Normal
        col,                                // Color
        core::vector2df(1,1));              // TCoord
        mesh->lineVertices.push_back(v);    // Index = 0
      }      // lower circle line
      mesh->lineIndices.push_back(mesh->lineVertices.size() - 1);
      mesh->lineIndices.push_back(mesh->lineVertices.size());
    }
    mesh->lineIndices[mesh->lineIndices.size() - 1] = v1st;
    // upper level
    v1st = mesh->lineVertices.size();
    for(irr::u32 x = 0; x < vertexcount; ++x){
      core::vector3df pos = radius * cos(tmp * x) * v1 + radius * sin(tmp * x) * v2;
      pos.Y = height;
      { video::S3DVertex v(pos,             // Position
        core::vector3df(0,1,0),             // Normal
        col,                                // Color
        core::vector2df(1,1));              // TCoord
        mesh->lineVertices.push_back(v);    // Index = 0
      }      // lower circle line
      // connection line
      mesh->lineIndices.push_back(mesh->lineVertices.size() - 1);
      mesh->lineIndices.push_back(mesh->lineVertices.size() - 1 - vertexcount);
      // upper circle line
      mesh->lineIndices.push_back(mesh->lineVertices.size() - 1);
      mesh->lineIndices.push_back(mesh->lineVertices.size());
    }
    mesh->lineIndices[mesh->lineIndices.size() - 1] = v1st;

    if(showDirIndicator){
      irr::video::S3DVertex vert;
      //! faces (direction indicator)
      // 0 - front
      { video::S3DVertex v(core::vector3df(0,height * 0.5,radius),
                           core::vector3df(0,1,0), col, core::vector2df(1,1));
        mesh->faceVertices.push_back(v);
      }
      double tmp2 = 2.0 * irr::core::PI / 16;
      // 1 - right back
      { int idx = 2;
        video::S3DVertex v(core::vector3df(0,0,0),
                           core::vector3df(0,1,0), col, core::vector2df(1,1));
        v.Pos = radius * cos(tmp2 * idx) * v1 + radius * sin(tmp2 * idx) * v2;
        v.Pos.Y = height * 0.5;
        mesh->faceVertices.push_back(v);
      }
      // 2 - left back
      { int idx = 14;
        video::S3DVertex v(core::vector3df(0,0,0),
                           core::vector3df(0,1,0), col, core::vector2df(1,1));
        v.Pos = radius * cos(tmp2 * idx) * v1 + radius * sin(tmp2 * idx) * v2;
        v.Pos.Y = height * 0.5;
        mesh->faceVertices.push_back(v);
      }
      // 3 - middle (back)
      { video::S3DVertex v(core::vector3df(0, height * 0.5, radius * -0.3),
                           core::vector3df(0,1,0), col, core::vector2df(1,1));
        mesh->faceVertices.push_back(v);
      }
      // lest faces
      mesh->faceIndices.push_back(2);
      mesh->faceIndices.push_back(3);
      mesh->faceIndices.push_back(0);
      mesh->faceIndices.push_back(2);
      mesh->faceIndices.push_back(0);
      mesh->faceIndices.push_back(3);
      // right faces
      mesh->faceIndices.push_back(3);
      mesh->faceIndices.push_back(1);
      mesh->faceIndices.push_back(0);
      mesh->faceIndices.push_back(3);
      mesh->faceIndices.push_back(0);
      mesh->faceIndices.push_back(1);
    }
  }
  //! get mesh for line
  inline void getAnnotationMesh_Line(datAnnotation* mesh, Vec3 p1, Vec3 p2, video::SColor col=video::SColor(255, 0,200,200)){
    clearAnnotation_Mesh(mesh);
    addLineToAnnotation_Mesh(mesh, p1, p2, col);
  }
  //! get mesh for trail
  inline void getAnnotationMesh_Trail(datAnnotation* mesh, Vec3 p, video::SColor col=video::SColor(255, 200,0,0)){
    clearAnnotation_Mesh(mesh);
    addPointToAnnotation_Trail(mesh, p, col);
  }
  //! get mesh for sphere
  inline void getAnnotationMesh_Sphere(datAnnotation* mesh, float radius=1.0, video::SColor col=video::SColor(255, 200,200,200)){
    clearAnnotation_Mesh(mesh);
    video::S3DVertex vert(core::vector3df(0,0,0), core::vector3df(0,1,0), col, core::vector2df(1,1));

    // \note I know I can calculate it...  ;))
    vert.Pos = irr::core::vector3df(0.000000,1.000000,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,0.865317,0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.250000,0.865317,0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.433013,0.865317,0.250000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.500000,0.865317,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,0.500000,0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.433013,0.500000,0.749861);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.750000,0.500000,0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.866025,0.500000,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,0.000000,1.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.500000,0.000000,0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.866025,0.000000,0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(1.000000,0.000000,0.000000);
    mesh->lineVertices.push_back(vert);
    mesh->lineIndices.push_back(0); mesh->lineIndices.push_back(1);
    mesh->lineIndices.push_back(0); mesh->lineIndices.push_back(2);
    mesh->lineIndices.push_back(0); mesh->lineIndices.push_back(3);
    mesh->lineIndices.push_back(0); mesh->lineIndices.push_back(4);
    mesh->lineIndices.push_back(1); mesh->lineIndices.push_back(2);
    mesh->lineIndices.push_back(2); mesh->lineIndices.push_back(3);
    mesh->lineIndices.push_back(3); mesh->lineIndices.push_back(4);
    mesh->lineIndices.push_back(1); mesh->lineIndices.push_back(5);
    mesh->lineIndices.push_back(2); mesh->lineIndices.push_back(6);
    mesh->lineIndices.push_back(3); mesh->lineIndices.push_back(7);
    mesh->lineIndices.push_back(4); mesh->lineIndices.push_back(8);
    mesh->lineIndices.push_back(5); mesh->lineIndices.push_back(6);
    mesh->lineIndices.push_back(6); mesh->lineIndices.push_back(7);
    mesh->lineIndices.push_back(7); mesh->lineIndices.push_back(8);
    mesh->lineIndices.push_back(5); mesh->lineIndices.push_back(9);
    mesh->lineIndices.push_back(6); mesh->lineIndices.push_back(10);
    mesh->lineIndices.push_back(7); mesh->lineIndices.push_back(11);
    mesh->lineIndices.push_back(8); mesh->lineIndices.push_back(12);
    mesh->lineIndices.push_back(9); mesh->lineIndices.push_back(10);
    mesh->lineIndices.push_back(10); mesh->lineIndices.push_back(11);
    mesh->lineIndices.push_back(11); mesh->lineIndices.push_back(12);

    int p1st = mesh->lineVertices.size();
    vert.Pos = irr::core::vector3df(0.000000,-1.000000,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,-0.865317,0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.250000,-0.865317,0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.433013,-0.865317,0.250000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.500000,-0.865317,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,-0.500000,0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.433013,-0.500000,0.749861);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.750000,-0.500000,0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.866025,-0.500000,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,-0.000000,1.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.500000,-0.000000,0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.866025,-0.000000,0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(1.000000,-0.000000,0.000000);
    mesh->lineVertices.push_back(vert);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 1);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 5);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 4); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 9);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 8); mesh->lineIndices.push_back(p1st + 12);
    mesh->lineIndices.push_back(p1st + 9); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 10); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 11); mesh->lineIndices.push_back(p1st + 12);

    p1st = mesh->lineVertices.size();
    vert.Pos = irr::core::vector3df(-0.000000,1.000000,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,0.865317,0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.250000,0.865317,0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.433013,0.865317,0.250000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.500000,0.865317,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,0.500000,0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.433013,0.500000,0.749861);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.750000,0.500000,0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.866025,0.500000,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,0.000000,1.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.500000,0.000000,0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.866025,0.000000,0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-1.000000,0.000000,0.000000);
    mesh->lineVertices.push_back(vert);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 1);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 5);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 4); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 9);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 8); mesh->lineIndices.push_back(p1st + 12);
    mesh->lineIndices.push_back(p1st + 9); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 10); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 11); mesh->lineIndices.push_back(p1st + 12);

    p1st = mesh->lineVertices.size();
    vert.Pos = irr::core::vector3df(-0.000000,-1.000000,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,-0.865317,0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.250000,-0.865317,0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.433013,-0.865317,0.250000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.500000,-0.865317,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,-0.500000,0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.433013,-0.500000,0.749861);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.750000,-0.500000,0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.866025,-0.500000,0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,-0.000000,1.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.500000,-0.000000,0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.866025,-0.000000,0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-1.000000,-0.000000,0.000000);
    mesh->lineVertices.push_back(vert);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 1);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 5);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 4); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 9);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 8); mesh->lineIndices.push_back(p1st + 12);
    mesh->lineIndices.push_back(p1st + 9); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 10); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 11); mesh->lineIndices.push_back(p1st + 12);

    p1st = mesh->lineVertices.size();
    vert.Pos = irr::core::vector3df(0.000000,1.000000,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,0.865317,-0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.250000,0.865317,-0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.433013,0.865317,-0.250000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.500000,0.865317,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,0.500000,-0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.433013,0.500000,-0.749861);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.750000,0.500000,-0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.866025,0.500000,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,0.000000,-1.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.500000,0.000000,-0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.866025,0.000000,-0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(1.000000,0.000000,-0.000000);
    mesh->lineVertices.push_back(vert);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 1);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 5);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 4); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 9);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 8); mesh->lineIndices.push_back(p1st + 12);
    mesh->lineIndices.push_back(p1st + 9); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 10); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 11); mesh->lineIndices.push_back(p1st + 12);

    p1st = mesh->lineVertices.size();
    vert.Pos = irr::core::vector3df(0.000000,-1.000000,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,-0.865317,-0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.250000,-0.865317,-0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.433013,-0.865317,-0.250000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.500000,-0.865317,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,-0.500000,-0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.433013,-0.500000,-0.749861);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.750000,-0.500000,-0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.866025,-0.500000,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.000000,-0.000000,-1.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.500000,-0.000000,-0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(0.866025,-0.000000,-0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(1.000000,-0.000000,-0.000000);
    mesh->lineVertices.push_back(vert);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 1);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 5);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 4); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 9);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 8); mesh->lineIndices.push_back(p1st + 12);
    mesh->lineIndices.push_back(p1st + 9); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 10); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 11); mesh->lineIndices.push_back(p1st + 12);

    p1st = mesh->lineVertices.size();
    vert.Pos = irr::core::vector3df(-0.000000,1.000000,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,0.865317,-0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.250000,0.865317,-0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.433013,0.865317,-0.250000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.500000,0.865317,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,0.500000,-0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.433013,0.500000,-0.749861);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.750000,0.500000,-0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.866025,0.500000,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,0.000000,-1.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.500000,0.000000,-0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.866025,0.000000,-0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-1.000000,0.000000,-0.000000);
    mesh->lineVertices.push_back(vert);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 1);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 5);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 4); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 9);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 8); mesh->lineIndices.push_back(p1st + 12);
    mesh->lineIndices.push_back(p1st + 9); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 10); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 11); mesh->lineIndices.push_back(p1st + 12);

    p1st = mesh->lineVertices.size();
    vert.Pos = irr::core::vector3df(-0.000000,-1.000000,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,-0.865317,-0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.250000,-0.865317,-0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.433013,-0.865317,-0.250000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.500000,-0.865317,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,-0.500000,-0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.433013,-0.500000,-0.749861);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.750000,-0.500000,-0.432874);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.866025,-0.500000,-0.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.000000,-0.000000,-1.000000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.500000,-0.000000,-0.865887);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-0.866025,-0.000000,-0.500000);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(-1.000000,-0.000000,-0.000000);
    mesh->lineVertices.push_back(vert);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 1);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 0); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 2);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 3);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 4);
    mesh->lineIndices.push_back(p1st + 1); mesh->lineIndices.push_back(p1st + 5);
    mesh->lineIndices.push_back(p1st + 2); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 3); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 4); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 6);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 7);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 8);
    mesh->lineIndices.push_back(p1st + 5); mesh->lineIndices.push_back(p1st + 9);
    mesh->lineIndices.push_back(p1st + 6); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 7); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 8); mesh->lineIndices.push_back(p1st + 12);
    mesh->lineIndices.push_back(p1st + 9); mesh->lineIndices.push_back(p1st + 10);
    mesh->lineIndices.push_back(p1st + 10); mesh->lineIndices.push_back(p1st + 11);
    mesh->lineIndices.push_back(p1st + 11); mesh->lineIndices.push_back(p1st + 12);

//    float r = radius * 0.5;
    for(irr::u32 t = 0; t < mesh->lineVertices.size(); ++t)
      mesh->lineVertices[t].Pos *= radius;

  }
  //! get mesh for plane
  inline void getAnnotationMesh_Plane(datAnnotation* mesh, float w, float h, video::SColor col=video::SColor(255, 200,0,200)){
    clearAnnotation_Mesh(mesh);

    video::S3DVertex vert(core::vector3df(0,0,0), core::vector3df(0,1,0), col, core::vector2df(1,1));
    vert.Pos = irr::core::vector3df(w * -0.5, h * -0.5, 0.0);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w *  0.5, h * -0.5, 0.0);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w *  0.5, h *  0.5, 0.0);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w * -0.5, h *  0.5, 0.0);
    mesh->lineVertices.push_back(vert);

    mesh->lineIndices.push_back(0); mesh->lineIndices.push_back(1);
    mesh->lineIndices.push_back(1); mesh->lineIndices.push_back(2);
    mesh->lineIndices.push_back(2); mesh->lineIndices.push_back(3);
    mesh->lineIndices.push_back(3); mesh->lineIndices.push_back(0);

  }
  //! get mesh for box
  inline void getAnnotationMesh_Box(datAnnotation* mesh, float w, float h, float d, video::SColor col=video::SColor(255, 50,150,250)){
    clearAnnotation_Mesh(mesh);

    video::S3DVertex vert(core::vector3df(0,0,0), core::vector3df(0,1,0), col, core::vector2df(1,1));
    vert.Pos = irr::core::vector3df(w * -0.5, h * -0.5, d * -0.5);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w *  0.5, h * -0.5, d * -0.5);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w *  0.5, h * -0.5, d *  0.5);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w * -0.5, h * -0.5, d *  0.5);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w * -0.5, h *  0.5, d * -0.5);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w *  0.5, h *  0.5, d * -0.5);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w *  0.5, h *  0.5, d *  0.5);
    mesh->lineVertices.push_back(vert);
    vert.Pos = irr::core::vector3df(w * -0.5, h *  0.5, d *  0.5);
    mesh->lineVertices.push_back(vert);

    mesh->lineIndices.push_back(0); mesh->lineIndices.push_back(1);
    mesh->lineIndices.push_back(1); mesh->lineIndices.push_back(2);
    mesh->lineIndices.push_back(2); mesh->lineIndices.push_back(3);
    mesh->lineIndices.push_back(3); mesh->lineIndices.push_back(0);

    mesh->lineIndices.push_back(4); mesh->lineIndices.push_back(5);
    mesh->lineIndices.push_back(5); mesh->lineIndices.push_back(6);
    mesh->lineIndices.push_back(6); mesh->lineIndices.push_back(7);
    mesh->lineIndices.push_back(7); mesh->lineIndices.push_back(4);

    mesh->lineIndices.push_back(0); mesh->lineIndices.push_back(4);
    mesh->lineIndices.push_back(1); mesh->lineIndices.push_back(5);
    mesh->lineIndices.push_back(2); mesh->lineIndices.push_back(6);
    mesh->lineIndices.push_back(3); mesh->lineIndices.push_back(7);

  }

// TODO (Acki#1#): text (3d/2d)

  template <class Super>
  class AnnotationMixin : public Super{
    private:
      // trails
      int trailVertexCount;       // number of vertices in array (ring buffer)
      int trailIndex;             // array index of most recently recorded point
      float trailDuration;        // duration (in seconds) of entire trail
      float trailSampleInterval;  // desired interval between taking samples
      float trailLastSampleTime;  // global time when lat sample was taken
      int trailDottedPhase;       // dotted line: draw segment or not
      Vec3 curPosition;           // last reported position of vehicle
      Vec3* trailVertices;        // array (ring) of recent points along trail
      char* trailFlags;           // array (ring) of flag bits for trail points

    public:
      // constructor
      AnnotationMixin();
      // destructor
      virtual ~AnnotationMixin();

      // ------------------------------------------------------------------------
      // trails / streamers
      // these routines support visualization of a vehicle's recent path
      // XXX conceivable trail/streamer should be a separate class,
      // XXX Annotation would "has-a" one (or more))
      // ------------------------------------------------------------------------
      //! record a position for the current time, called once per update
      void recordTrailVertex(const float currentTime, const Vec3& position);
      //! draw the trail as a dotted line, fading away with age
      void drawTrail(void){
        drawTrail (grayColor (0.7f), gWhite);
      }
      void drawTrail(const Color& trailColor, const Color& tickColor);
      //! set trail parameters: the amount of time it represents and the
      //! number of samples along its length.  re-allocates internal buffers.
      void setTrailParameters(const float duration, const int vertexCount);
      //! forget trail history: used to prevent long streaks due to teleportation
      void clearTrailHistory(void);

      // ------------------------------------------------------------------------
      // drawing of lines, circles and (filled) disks to annotate steering
      // behaviors.  When called during OpenSteerDemo's simulation update phase,
      // these functions call a "deferred draw" routine which buffer the
      // arguments for use during the redraw phase.
      // note: "circle" means unfilled
      //       "disk" means filled
      //       "XZ" means on a plane parallel to the X and Z axes (perp to Y)
      //       "3d" means the circle is perpendicular to the given "axis"
      //       "segments" is the number of line segments used to draw the circle
      // ------------------------------------------------------------------------
      //! draw an opaque colored line segment between two locations in space
      void annotationLine(const Vec3& startPoint, const Vec3& endPoint, const Color& color) const;
      //! draw a circle on the XZ plane
      void annotationXZCircle(const float radius, const Vec3& center, const Color& color, const int segments) const{
        annotationXZCircleOrDisk(radius, center, color, segments, false);
      }
      //! draw a disk on the XZ plane
      void annotationXZDisk(const float radius, const Vec3& center, const Color& color, const int segments) const{
        annotationXZCircleOrDisk(radius, center, color, segments, true);
      }
      //! draw a circle perpendicular to the given axis
      void annotation3dCircle(const float radius, const Vec3& center, const Vec3& axis, const Color& color, const int segments) const{
        annotation3dCircleOrDisk(radius, center, axis, color, segments, false);
      }
      //! draw a disk perpendicular to the given axis
      void annotation3dDisk(const float radius, const Vec3& center, const Vec3& axis, const Color& color, const int segments) const{
        annotation3dCircleOrDisk(radius, center, axis, color, segments, true);
      }
      //! support for annotation circles
      void annotationXZCircleOrDisk(const float radius, const Vec3& center, const Color& color, const int segments, const bool filled) const{
        annotationCircleOrDisk(radius, Vec3::zero, center, color, segments, filled, false); // "not in3d" -> on XZ plane
      }
      void annotation3dCircleOrDisk(const float radius, const Vec3& center, const Vec3& axis, const Color& color, const int segments, const bool filled) const{
        annotationCircleOrDisk(radius, axis, center, color, segments, filled, true); // "in3d"
      }
      void annotationCircleOrDisk(const float radius, const Vec3& axis, const Vec3& center, const Color& color, const int segments, const bool filled, const bool in3d) const;

  };

} // namespace OpenSteer

// Constructor and destructor
template<class Super>
OpenSteer::AnnotationMixin<Super>::AnnotationMixin(void){
  trailVertices = NULL;
  trailFlags = NULL;
  // xxx I wonder if it makes more sense to NOT do this here, see if the
  // xxx vehicle class calls it to set custom parameters, and if not, set
  // xxx these default parameters on first call to a "trail" function.  The
  // xxx issue is whether it is a problem to allocate default-sized arrays
  // xxx then to free them and allocate new ones
  setTrailParameters(5, 100);  // 5 seconds with 100 points along the trail
}
template<class Super>
OpenSteer::AnnotationMixin<Super>::~AnnotationMixin(void){
  delete[] trailVertices;
  delete[] trailFlags;
}

// set trail parameters: the amount of time it represents and the number of
// samples along its length.  re-allocates internal buffers.
template<class Super>
void OpenSteer::AnnotationMixin<Super>::setTrailParameters(const float duration, const int vertexCount){
  // record new parameters
  trailDuration = duration;
  trailVertexCount = vertexCount;

  // reset other internal trail state
  trailIndex = 0;
  trailLastSampleTime = 0;
  trailSampleInterval = trailDuration / trailVertexCount;
  trailDottedPhase = 1;

  // prepare trailVertices array: free old one if needed, allocate new one
  delete[] trailVertices;
  trailVertices = new Vec3[trailVertexCount];

  // prepare trailFlags array: free old one if needed, allocate new one
  delete[] trailFlags;
  trailFlags = new char[trailVertexCount];

  // initializing all flags to zero means "do not draw this segment"
  for(int i = 0; i < trailVertexCount; ++i) trailFlags[i] = 0;
}

// forget trail history: used to prevent long streaks due to teleportation
// XXX perhaps this coudl be made automatic: triggered when the change in
// XXX position is well out of the range of the vehicles top velocity
template<class Super>
void OpenSteer::AnnotationMixin<Super>::clearTrailHistory(void){
  // brute force implementation, reset everything
  setTrailParameters(trailDuration, trailVertexCount);
}

// record a position for the current time, called once per update
template<class Super>
void OpenSteer::AnnotationMixin<Super>::recordTrailVertex(const float currentTime, const Vec3& position){
  const float timeSinceLastTrailSample = currentTime - trailLastSampleTime;
  if(timeSinceLastTrailSample > trailSampleInterval){
    trailIndex = (trailIndex + 1) % trailVertexCount;
    trailVertices [trailIndex] = position;
    trailDottedPhase = (trailDottedPhase + 1) % 2;
    const int tick = (floorXXX (currentTime) > floorXXX (trailLastSampleTime));
    trailFlags [trailIndex] = trailDottedPhase | (tick ? '\2' : '\0');
    trailLastSampleTime = currentTime;
  }
  curPosition = position;
}

// draw the trail as a dotted line, fading away with age
template<class Super>
void OpenSteer::AnnotationMixin<Super>::drawTrail(const Color& trailColor, const Color& tickColor){
  if(enableAnnotation){
    int index = trailIndex;
    for(int j = 0; j < trailVertexCount; ++j){
      // index of the next vertex (mod around ring buffer)
      const int next = (index + 1) % trailVertexCount;

      // "tick mark": every second, draw a segment in a different color
      const int tick = ((trailFlags [index] & 2) || (trailFlags [next] & 2));
      const Color color = tick ? tickColor : trailColor;

      // draw every other segment
      if(trailFlags[index] & 1){
        if(j == 0){
          // draw segment from current position to first trail point
//                    drawLineAlpha (curPosition,
//                                   trailVertices [index],
//                                   color,
//                                   1);
        }else{
          // draw trail segments with opacity decreasing with age
          const float minO = 0.05f; // minimum opacity
          const float fraction = (float) j / trailVertexCount;
          const float opacity = (fraction * (1 - minO)) + minO;
//                    drawLineAlpha (trailVertices [index],
//                                   trailVertices [next],
//                                   color,
//                                   opacity);
        }
      }
      index = next;
    }
  }
}

// request (deferred) drawing of a line for graphical annotation
// This is called during OpenSteerDemo's simulation phase to annotate behavioral
// or steering state.  When annotation is enabled, a description of the line
// segment is queued to be drawn during OpenSteerDemo's redraw phase.
template<class Super>
void OpenSteer::AnnotationMixin<Super>::annotationLine(const Vec3& startPoint, const Vec3& endPoint, const Color& color) const{
}

// request (deferred) drawing of a circle (or disk) for graphical annotation
// This is called during OpenSteerDemo's simulation phase to annotate behavioral
// or steering state.  When annotation is enabled, a description of the
// "circle or disk" is queued to be drawn during OpenSteerDemo's redraw phase.
template<class Super>
void OpenSteer::AnnotationMixin<Super>::annotationCircleOrDisk(const float radius, const Vec3& axis, const Vec3& center, const Color& color, const int segments, const bool filled, const bool in3d) const{
}

#endif // OPENSTEER_ANNOTATION_H
#endif
