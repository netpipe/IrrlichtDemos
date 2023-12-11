#ifdef OPENSTEER
/*! Irrlicht implementation by A.Buschhüter<br> (http://abusoft.g0dsoft.com<br>)
----------------------------------------------------------------------------<br>
//
//
// OpenSteer -- Steering Behaviors for Autonomous Characters
//
// Copyright (c) 2002-2004, Sony Computer Entertainment America
// Original author: Craig Reynolds <craig_reynolds@playstation.sony.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
//
// ----------------------------------------------------------------------------<br>
//
//
// OpenSteer Obstacle classes
//
// 10-28-04 cwr: split off from Obstacle.h
//
//
// ----------------------------------------------------------------------------<br>
*/
#include "osObstacle.h"

namespace OpenSteer {

//! Sphere Obstacle
void SphereObstacle::createMesh(){
  lstVertices.clear();
  lstIndices.clear();
  float radius = _radius;
  //! 1 von 8 (+ + +)
  {irr::core::vector3df vScale(0.0 + radius, 0.0 + radius, 0.0 + radius);
    int idTop = lstVertices.size();
    //! Vertices
    // 1. Ebene (Top)
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,1.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 2. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.866666,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.250000,0.866666,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.866666,0.250000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.866666,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 3. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.500000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.500000,0.750000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.750000,0.500000,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.500000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 4. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.000000,1.000000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.000000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.000000,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(1.000000,0.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    //! Indices
    // 1. Ebene nach 2. Ebene
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 1);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 4);
    // 2. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 4);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 1);
    // 2. Ebene nach 3. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 5);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 8);
    // 3. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 8);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 5);
    // 3. Ebene nach 4. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 9);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 12);
    // 4. Ebene
    lstIndices.push_back(idTop + 9); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 10); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 11); lstIndices.push_back(idTop + 12);
    lstIndices.push_back(idTop + 12); lstIndices.push_back(idTop + 9);
  }
  //! 2 von 8 (- + +)
  {irr::core::vector3df vScale(0.0 - radius, 0.0 + radius, 0.0 + radius);
    int idTop = lstVertices.size();
    //! Vertices
    // 1. Ebene (Top)
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,1.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 2. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.866666,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.250000,0.866666,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.866666,0.250000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.866666,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 3. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.500000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.500000,0.750000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.750000,0.500000,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.500000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 4. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.000000,1.000000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.000000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.000000,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(1.000000,0.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    //! Indices
    // 1. Ebene nach 2. Ebene
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 1);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 4);
    // 2. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 4);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 1);
    // 2. Ebene nach 3. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 5);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 8);
    // 3. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 8);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 5);
    // 3. Ebene nach 4. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 9);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 12);
    // 4. Ebene
    lstIndices.push_back(idTop + 9); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 10); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 11); lstIndices.push_back(idTop + 12);
    lstIndices.push_back(idTop + 12); lstIndices.push_back(idTop + 9);
  }
  //! 3 von 8 (+ + -)
  {irr::core::vector3df vScale(0.0 + radius, 0.0 + radius, 0.0 - radius);
    int idTop = lstVertices.size();
    //! Vertices
    // 1. Ebene (Top)
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,1.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 2. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.866666,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.250000,0.866666,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.866666,0.250000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.866666,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 3. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.500000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.500000,0.750000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.750000,0.500000,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.500000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 4. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.000000,1.000000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.000000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.000000,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(1.000000,0.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    //! Indices
    // 1. Ebene nach 2. Ebene
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 1);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 4);
    // 2. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 4);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 1);
    // 2. Ebene nach 3. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 5);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 8);
    // 3. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 8);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 5);
    // 3. Ebene nach 4. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 9);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 12);
    // 4. Ebene
    lstIndices.push_back(idTop + 9); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 10); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 11); lstIndices.push_back(idTop + 12);
    lstIndices.push_back(idTop + 12); lstIndices.push_back(idTop + 9);
  }
  //! 4 von 8 (- + -)
  {irr::core::vector3df vScale(0.0 - radius, 0.0 + radius, 0.0 - radius);
    int idTop = lstVertices.size();
    //! Vertices
    // 1. Ebene (Top)
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,1.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 2. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.866666,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.250000,0.866666,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.866666,0.250000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.866666,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 3. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.500000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.500000,0.750000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.750000,0.500000,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.500000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 4. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.000000,1.000000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.000000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.000000,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(1.000000,0.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    //! Indices
    // 1. Ebene nach 2. Ebene
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 1);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 4);
    // 2. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 4);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 1);
    // 2. Ebene nach 3. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 5);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 8);
    // 3. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 8);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 5);
    // 3. Ebene nach 4. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 9);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 12);
    // 4. Ebene
    lstIndices.push_back(idTop + 9); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 10); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 11); lstIndices.push_back(idTop + 12);
    lstIndices.push_back(idTop + 12); lstIndices.push_back(idTop + 9);
  }
  //! 5 von 8 (- - -)
  {irr::core::vector3df vScale(0.0 - radius, 0.0 - radius, 0.0 - radius);
    int idTop = lstVertices.size();
    //! Vertices
    // 1. Ebene (Top)
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,1.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 2. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.866666,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.250000,0.866666,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.866666,0.250000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.866666,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 3. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.500000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.500000,0.750000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.750000,0.500000,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.500000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 4. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.000000,1.000000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.000000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.000000,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(1.000000,0.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    //! Indices
    // 1. Ebene nach 2. Ebene
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 1);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 4);
    // 2. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 4);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 1);
    // 2. Ebene nach 3. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 5);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 8);
    // 3. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 8);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 5);
    // 3. Ebene nach 4. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 9);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 12);
    // 4. Ebene
    lstIndices.push_back(idTop + 9); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 10); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 11); lstIndices.push_back(idTop + 12);
    lstIndices.push_back(idTop + 12); lstIndices.push_back(idTop + 9);
  }
  //! 6 von 8 (+ - -)
  {irr::core::vector3df vScale(0.0 + radius, 0.0 - radius, 0.0 - radius);
    int idTop = lstVertices.size();
    //! Vertices
    // 1. Ebene (Top)
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,1.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 2. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.866666,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.250000,0.866666,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.866666,0.250000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.866666,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 3. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.500000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.500000,0.750000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.750000,0.500000,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.500000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 4. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.000000,1.000000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.000000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.000000,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(1.000000,0.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    //! Indices
    // 1. Ebene nach 2. Ebene
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 1);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 4);
    // 2. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 4);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 1);
    // 2. Ebene nach 3. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 5);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 8);
    // 3. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 8);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 5);
    // 3. Ebene nach 4. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 9);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 12);
    // 4. Ebene
    lstIndices.push_back(idTop + 9); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 10); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 11); lstIndices.push_back(idTop + 12);
    lstIndices.push_back(idTop + 12); lstIndices.push_back(idTop + 9);
  }
  //! 7 von 8 (- - +)
  {irr::core::vector3df vScale(0.0 - radius, 0.0 - radius, 0.0 + radius);
    int idTop = lstVertices.size();
    //! Vertices
    // 1. Ebene (Top)
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,1.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 2. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.866666,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.250000,0.866666,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.866666,0.250000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.866666,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 3. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.500000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.500000,0.750000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.750000,0.500000,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.500000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 4. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.000000,1.000000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.000000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.000000,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(1.000000,0.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    //! Indices
    // 1. Ebene nach 2. Ebene
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 1);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 4);
    // 2. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 4);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 1);
    // 2. Ebene nach 3. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 5);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 8);
    // 3. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 8);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 5);
    // 3. Ebene nach 4. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 9);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 12);
    // 4. Ebene
    lstIndices.push_back(idTop + 9); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 10); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 11); lstIndices.push_back(idTop + 12);
    lstIndices.push_back(idTop + 12); lstIndices.push_back(idTop + 9);
  }
  //! 8 von 8 (+ - +)
  {irr::core::vector3df vScale(0.0 + radius, 0.0 - radius, 0.0 + radius);
    int idTop = lstVertices.size();
    //! Vertices
    // 1. Ebene (Top)
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,1.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 2. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.866666,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.250000,0.866666,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.866666,0.250000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.866666,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 3. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.500000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.433333,0.500000,0.750000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.750000,0.500000,0.433333) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.500000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    // 4. Ebene
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.000000,0.000000,1.000000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.500000,0.000000,0.866666) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(0.866666,0.000000,0.500000) * vScale;
      lstVertices.push_back(v);
    }
    {irr::video::S3DVertex v;
      v.Pos = irr::core::vector3df(1.000000,0.000000,0.000000) * vScale;
      lstVertices.push_back(v);
    }
    //! Indices
    // 1. Ebene nach 2. Ebene
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 1);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop); lstIndices.push_back(idTop + 4);
    // 2. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 2);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 3);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 4);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 1);
    // 2. Ebene nach 3. Ebene
    lstIndices.push_back(idTop + 1); lstIndices.push_back(idTop + 5);
    lstIndices.push_back(idTop + 2); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 3); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 4); lstIndices.push_back(idTop + 8);
    // 3. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 6);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 7);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 8);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 5);
    // 3. Ebene nach 4. Ebene
    lstIndices.push_back(idTop + 5); lstIndices.push_back(idTop + 9);
    lstIndices.push_back(idTop + 6); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 7); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 8); lstIndices.push_back(idTop + 12);
    // 4. Ebene
    lstIndices.push_back(idTop + 9); lstIndices.push_back(idTop + 10);
    lstIndices.push_back(idTop + 10); lstIndices.push_back(idTop + 11);
    lstIndices.push_back(idTop + 11); lstIndices.push_back(idTop + 12);
    lstIndices.push_back(idTop + 12); lstIndices.push_back(idTop + 9);
  }

  for(irr::u32 t = 0; t < lstVertices.size(); ++t){
    lstVertices[t].Color = irr::video::SColor(255, 200,0,0);

  }

}
//! Render a wire frame model
void SphereObstacle::render(irr::video::IVideoDriver* driver){
  static irr::core::matrix4 AbsoluteTransformation;
  static irr::video::SMaterial Material;
  //! create mesh
  static float rOld;
  if(rOld != _radius){
    lstIndices.clear();
    rOld = _radius;
  }
  if(!lstIndices.size()) createMesh();
  //! render mesh
  if(driver && lstIndices.size()){
    Material.Lighting = false;
    Material.EmissiveColor = irr::video::SColor(255, 255,255,255);
    driver->setMaterial(Material);
    AbsoluteTransformation.setTranslation(_position.vector3df());
    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&lstVertices[0], lstVertices.size(), &lstIndices[0], lstIndices.size() / 2, irr::video::EVT_STANDARD, irr::scene::EPT_LINES, irr::video::EIT_16BIT);
  }
}
//! find first intersection of a vehicle's path with this obstacle
void SphereObstacle::findIntersectionWithVehiclePath (const AbstractVehicle& vehicle, PathIntersection& pi) const{
    // This routine is based on the Paul Bourke's derivation in:
    //   Intersection of a Line and a Sphere (or circle)
    //   http://www.swin.edu.au/astronomy/pbourke/geometry/sphereline/
    // But the computation is done in the vehicle's local space, so
    // the line in question is the Z (Forward) axis of the space which
    // simplifies some of the calculations.
    float b, c, d, p, q, s;
    Vec3 lc;

    // initialize pathIntersection object to "no intersection found"
    pi.intersect = false;

    // find sphere's "local center" (lc) in the vehicle's coordinate space
    lc = vehicle.localizePosition (_position);
    pi.vehicleOutside = lc.length () > _radius;

	// if obstacle is seen from inside, but vehicle is outside, must avoid
	// (noticed once a vehicle got outside it ignored the obstacle 2008-5-20)
	if (pi.vehicleOutside && (seenFrom () == inside))
	{
		pi.intersect = true;
		pi.distance = 0.0f;
		pi.steerHint = (_position - vehicle.position()).normalize();
		return;
	}

    // compute line-sphere intersection parameters
    const float r = _radius + vehicle.radius();
    b = -2 * lc.z;
    c = square (lc.x) + square (lc.y) + square (lc.z) - square (r);
    d = (b * b) - (4 * c);

    // when the path does not intersect the sphere
    if (d < 0) return;

    // otherwise, the path intersects the sphere in two points with
    // parametric coordinates of "p" and "q".  (If "d" is zero the two
    // points are coincident, the path is tangent)
    s = sqrtXXX (d);
    p = (-b + s) / 2;
    q = (-b - s) / 2;

    // both intersections are behind us, so no potential collisions
    if ((p < 0) && (q < 0)) return;

    // at least one intersection is in front, so intersects our forward
    // path
    pi.intersect = true;
    pi.obstacle = this;
    pi.distance =
        ((p > 0) && (q > 0)) ?
        // both intersections are in front of us, find nearest one
        ((p < q) ? p : q) :
        // otherwise one is ahead and one is behind: we are INSIDE obstacle
        (seenFrom () == outside ?
         // inside a solid obstacle, so distance to obstacle is zero
         0.0f :
         // hollow obstacle (or "both"), pick point that is in front
         ((p > 0) ? p : q));
    pi.surfacePoint =
        vehicle.position() + (vehicle.forward() * pi.distance);
    pi.surfaceNormal = (pi.surfacePoint - _position).normalize();
    switch (seenFrom ())
    {
    case outside:
        pi.steerHint = pi.surfaceNormal;
        break;
    case inside:
        pi.steerHint = -pi.surfaceNormal;
        break;
    case both:
        pi.steerHint = pi.surfaceNormal * (pi.vehicleOutside ? 1.0f : -1.0f);
        break;
    }
}
void SphereObstacle::position(Vec3 p){
  _position = p;
}
Vec3 SphereObstacle::position(){
  return _position;
}
void SphereObstacle::radius(float r){
  _radius = r;
  lstIndices.clear();
}
float SphereObstacle::radius(){
  return _radius;
}
void SphereObstacle::position3df(irr::core::vector3df p){
  _position = Vec3(p);
}
irr::core::vector3df SphereObstacle::position3df(){
  return _position.vector3df();
}

//! Box Obstacle
void BoxObstacle::createMesh(){
  lstVertices.clear();
  lstIndices.clear();
  float width = _width;
  float height = _height;
  float depth = _depth;
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * 0.5, height * 0.5, depth * 0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * -0.5, height * 0.5, depth * 0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * 0.5, height * 0.5, depth * -0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * -0.5, height * 0.5, depth * -0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * 0.5, height * -0.5, depth * 0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * -0.5, height * -0.5, depth * 0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * 0.5, height * -0.5, depth * -0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * -0.5, height * -0.5, depth * -0.5);
    lstVertices.push_back(v);
  }
  lstIndices.push_back(0); lstIndices.push_back(1);
  lstIndices.push_back(1); lstIndices.push_back(3);
  lstIndices.push_back(3); lstIndices.push_back(2);
  lstIndices.push_back(2); lstIndices.push_back(0);
  lstIndices.push_back(4); lstIndices.push_back(5);
  lstIndices.push_back(5); lstIndices.push_back(7);
  lstIndices.push_back(7); lstIndices.push_back(6);
  lstIndices.push_back(6); lstIndices.push_back(4);
  lstIndices.push_back(0); lstIndices.push_back(4);
  lstIndices.push_back(1); lstIndices.push_back(5);
  lstIndices.push_back(2); lstIndices.push_back(6);
  lstIndices.push_back(3); lstIndices.push_back(7);

  for(irr::u32 t = 0; t < lstVertices.size(); ++t){
    lstVertices[t].Color = irr::video::SColor(255, 200,0,0);

  }

}
void BoxObstacle::render(irr::video::IVideoDriver* driver){
  static irr::core::matrix4 AbsoluteTransformation;
  static irr::video::SMaterial Material;

  //! create mesh
  static float wOld, hOld, dOld;
  if((wOld != _width) || (hOld != _height) || (dOld != _depth)){
    lstIndices.clear();
    wOld = _width;
    hOld = _height;
    dOld = _depth;
  }
  if(!lstIndices.size()) createMesh();

  if(driver && lstIndices.size()){
    Material.Lighting = false;
    Material.EmissiveColor = irr::video::SColor(255, 255,255,255);
    driver->setMaterial(Material);
    AbsoluteTransformation.setTranslation(position().vector3df());
    AbsoluteTransformation.setRotationDegrees(forward().vector3df().getHorizontalAngle());
    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&lstVertices[0], lstVertices.size(), &lstIndices[0], lstIndices.size() / 2, irr::video::EVT_STANDARD, irr::scene::EPT_LINES, irr::video::EIT_16BIT);
  }
}
//! find first intersection of a vehicle's path with this obstacle
void BoxObstacle::findIntersectionWithVehiclePath (const AbstractVehicle& vehicle, PathIntersection& pi) const{
    // abbreviations
    const float w = _width; // dimensions
    const float h = _height;
    const float d = _depth;
    const Vec3 s = side (); // local space
    const Vec3 u = up ();
    const Vec3 f = forward ();
    const Vec3 p = position ();
    const Vec3 hw = s * (0.5f * _width); // offsets for face centers
    const Vec3 hh = u * (0.5f * _height);
    const Vec3 hd = f * (0.5f * _depth);
    const seenFromState sf = seenFrom ();

    // the box's six rectangular faces
    RectangleObstacle r1 (w, h,  s,  u,  f, p + hd, sf); // front
    RectangleObstacle r2 (w, h, -s,  u, -f, p - hd, sf); // back
    RectangleObstacle r3 (d, h, -f,  u,  s, p + hw, sf); // side
    RectangleObstacle r4 (d, h,  f,  u, -s, p - hw, sf); // other side
    RectangleObstacle r5 (w, d,  s, -f,  u, p + hh, sf); // top
    RectangleObstacle r6 (w, d, -s, -f, -u, p - hh, sf); // bottom

    // group the six RectangleObstacle faces together
    ObstacleGroup faces;
    faces.push_back (&r1);
    faces.push_back (&r2);
    faces.push_back (&r3);
    faces.push_back (&r4);
    faces.push_back (&r5);
    faces.push_back (&r6);

    // find first intersection of vehicle path with group of six faces
    PathIntersection next;
    firstPathIntersectionWithObstacleGroup (vehicle, faces, pi, next);

    // when intersection found, adjust PathIntersection for the box case
    if (pi.intersect)
    {
        pi.obstacle = this;
        pi.steerHint = ((pi.surfacePoint - position ()).normalize () *
                        (pi.vehicleOutside ? 1.0f : -1.0f));
    }
}
void BoxObstacle::size(Vec3 s){
  _width = s.x;
  _height = s.y;
  _depth = s.z;
  lstIndices.clear();
}
Vec3 BoxObstacle::size(){
  return Vec3(_width, _height, _depth);
}
float BoxObstacle::width(){
  return _width;
}
float BoxObstacle::height(){
  return _height;
}
float BoxObstacle::depth(){
  return _depth;
}
void BoxObstacle::width(float w){
  _width = w;
  lstIndices.clear();
}
void BoxObstacle::height(float h){
  _height = h;
  lstIndices.clear();
}
void BoxObstacle::depth(float d){
  _depth = d;
  lstIndices.clear();
}
void BoxObstacle::size3df(irr::core::vector3df s){
  _width = s.X;
  _height = s.Y;
  _depth = s.Z;
  lstIndices.clear();
}
irr::core::vector3df BoxObstacle::size3df(){
  return irr::core::vector3df(_width, _height, _depth);
}

//! Plane Obstacle
void PlaneObstacle::createMesh(){
  lstVertices.clear();
  lstIndices.clear();
// TODO (Acki#5#): create mesh (PlaneObstacle: keine AHnung wegen Größe !?!?! )

  for(irr::u32 t = 0; t < lstVertices.size(); ++t)
    lstVertices[t].Color = irr::video::SColor(255, 200,0,0);

}
void PlaneObstacle::render(irr::video::IVideoDriver* driver){
  static irr::core::matrix4 AbsoluteTransformation;
  AbsoluteTransformation.setTranslation(position().vector3df());
  AbsoluteTransformation.setRotationDegrees(forward().vector3df().getHorizontalAngle());

  static irr::video::SMaterial Material;
  Material.Lighting = false;
  Material.EmissiveColor = irr::video::SColor(255, 255,255,255);

  //! create mesh
  if(!lstIndices.size()) createMesh();

  if(driver && lstIndices.size()){
    driver->setMaterial(Material);
    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&lstVertices[0], lstVertices.size(), &lstIndices[0], lstIndices.size() / 2, irr::video::EVT_STANDARD, irr::scene::EPT_LINES, irr::video::EIT_16BIT);
  }
}
//! find first intersection of a vehicle's path with this obstacle
void PlaneObstacle::findIntersectionWithVehiclePath (const AbstractVehicle& vehicle, PathIntersection& pi) const{
    // initialize pathIntersection object to "no intersection found"
    pi.intersect = false;

    const Vec3 lp =  localizePosition (vehicle.position ());
    const Vec3 ld = localizeDirection (vehicle.forward ());

    // no obstacle intersection if path is parallel to XY (side/up) plane
    if (ld.dot (Vec3::forward) == 0.0f) return;

    // no obstacle intersection if vehicle is heading away from the XY plane
    if ((lp.z > 0.0f) && (ld.z > 0.0f)) return;
    if ((lp.z < 0.0f) && (ld.z < 0.0f)) return;

    // no obstacle intersection if obstacle "not seen" from vehicle's side
    if ((seenFrom () == outside) && (lp.z < 0.0f)) return;
    if ((seenFrom () == inside)  && (lp.z > 0.0f)) return;

    // find intersection of path with rectangle's plane (XY plane)
    const float ix = lp.x - (ld.x * lp.z / ld.z);
    const float iy = lp.y - (ld.y * lp.z / ld.z);
    const Vec3 planeIntersection (ix, iy, 0.0f);

    // no obstacle intersection if plane intersection is outside 2d shape
    if (!xyPointInsideShape (planeIntersection, vehicle.radius ())) return;

    // otherwise, the vehicle path DOES intersect this rectangle
    const Vec3 localXYradial = planeIntersection.normalize ();
    const Vec3 radial = globalizeDirection (localXYradial);
    const float sideSign = (lp.z > 0.0f) ? +1.0f : -1.0f;
    const Vec3 opposingNormal = forward () * sideSign;
    pi.intersect = true;
    pi.obstacle = this;
    pi.distance = (lp - planeIntersection).length ();
    pi.steerHint = opposingNormal + radial; // should have "toward edge" term?
    pi.surfacePoint = globalizePosition (planeIntersection);
    pi.surfaceNormal = opposingNormal;
    pi.vehicleOutside = lp.z > 0.0f;
}

//! Rectangle Obstacle
void RectangleObstacle::createMesh(){
  // oder doch Zylinder !?!?!
  lstVertices.clear();
  lstIndices.clear();

  float width = _width;
  float height = _height;

  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * 0.5, height * 0.5, width * 0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * -0.5, height * 0.5, width * 0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * 0.5, height * 0.5, width * -0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * -0.5, height * 0.5, width * -0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * 0.5, height * -0.5, width * 0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * -0.5, height * -0.5, width * 0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * 0.5, height * -0.5, width * -0.5);
    lstVertices.push_back(v);
  }
  {irr::video::S3DVertex v;
    v.Pos = irr::core::vector3df(width * -0.5, height * -0.5, width * -0.5);
    lstVertices.push_back(v);
  }
  lstIndices.push_back(0); lstIndices.push_back(1);
  lstIndices.push_back(1); lstIndices.push_back(2);
  lstIndices.push_back(2); lstIndices.push_back(3);
  lstIndices.push_back(3); lstIndices.push_back(0);
  lstIndices.push_back(4); lstIndices.push_back(5);
  lstIndices.push_back(5); lstIndices.push_back(6);
  lstIndices.push_back(6); lstIndices.push_back(7);
  lstIndices.push_back(7); lstIndices.push_back(4);
  lstIndices.push_back(0); lstIndices.push_back(4);
  lstIndices.push_back(1); lstIndices.push_back(5);
  lstIndices.push_back(2); lstIndices.push_back(6);
  lstIndices.push_back(3); lstIndices.push_back(7);

  for(irr::u32 t = 0; t < lstVertices.size(); ++t){
    lstVertices[t].Color = irr::video::SColor(255, 200,0,0);

  }

}
void RectangleObstacle::render(irr::video::IVideoDriver* driver){
  static irr::core::matrix4 AbsoluteTransformation;
  static irr::video::SMaterial Material;

  //! create mesh
  static float wOld, hOld;
  if((wOld != _width) || (hOld != _height)){
    lstIndices.clear();
    wOld = _width;
    hOld = _height;
  }
  if(!lstIndices.size()) createMesh();

  if(driver && lstIndices.size()){
    Material.Lighting = false;
    Material.EmissiveColor = irr::video::SColor(255, 255,255,255);
    driver->setMaterial(Material);
    AbsoluteTransformation.setTranslation(position().vector3df());
    AbsoluteTransformation.setRotationDegrees(forward().vector3df().getHorizontalAngle());
    driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
    driver->drawVertexPrimitiveList(&lstVertices[0], lstVertices.size(), &lstIndices[0], lstIndices.size() / 2, irr::video::EVT_STANDARD, irr::scene::EPT_LINES, irr::video::EIT_16BIT);
  }
}
//! determines if a given point on XY plane is inside obstacle shape
bool RectangleObstacle::xyPointInsideShape (const Vec3& point, float _radius) const{
    const float w = _radius + (_width * 0.5f);
    const float h = _radius + (_height * 0.5f);
    return !((point.x >  w) || (point.x < -w) || (point.y >  h) || (point.y < -h));
}
float RectangleObstacle::width(){
  return _width;
}
float RectangleObstacle::height(){
  return _height;
}
void RectangleObstacle::width(float w){
  _width = w;
  lstIndices.clear();
}
void RectangleObstacle::height(float h){
  _height = h;
  lstIndices.clear();
}

// Obstacle compute steering for a vehicle to avoid this obstacle, if needed
Vec3 Obstacle::steerToAvoid (const AbstractVehicle& vehicle, const float minTimeToCollision) const{
    // find nearest intersection with this obstacle along vehicle's path
    PathIntersection pi;
    findIntersectionWithVehiclePath (vehicle, pi);

    // return steering for vehicle to avoid intersection, or zero if non found
    return pi.steerToAvoidIfNeeded (vehicle, minTimeToCollision);
}
// Obstacle static method to apply steerToAvoid to nearest obstacle in an ObstacleGroup
Vec3 Obstacle::steerToAvoidObstacles (const AbstractVehicle& vehicle, const float minTimeToCollision, const ObstacleGroup& obstacles){
    PathIntersection nearest, next;

    // test all obstacles in group for an intersection with the vehicle's
    // future path, select the one whose point of intersection is nearest
    firstPathIntersectionWithObstacleGroup (vehicle, obstacles, nearest, next);

    // if nearby intersection found, steer away from it, otherwise no steering
    return nearest.steerToAvoidIfNeeded (vehicle, minTimeToCollision);
}
// Obstacle static method to find first vehicle path intersection in an ObstacleGroup
// returns its results in the PathIntersection argument "nearest",
// "next" is used to store internal state.
void Obstacle::firstPathIntersectionWithObstacleGroup (const AbstractVehicle& vehicle, const ObstacleGroup& obstacles, PathIntersection& nearest, PathIntersection& next){
    // test all obstacles in group for an intersection with the vehicle's
    // future path, select the one whose point of intersection is nearest
    next.intersect = false;
    nearest.intersect = false;
    for (ObstacleIterator o = obstacles.begin(); o != obstacles.end(); o++)
    {
        // find nearest point (if any) where vehicle path intersects obstacle
        // o, storing the results in PathIntersection object "next"
        (**o).findIntersectionWithVehiclePath (vehicle, next);

        // if this is the first intersection found, or it is the nearest found
        // so far, store it in PathIntersection object "nearest"
        const bool firstFound = !nearest.intersect;
        const bool nearestFound = (next.intersect &&
                                   (next.distance < nearest.distance));
        if (firstFound || nearestFound) nearest = next;
    }
}
// PathIntersection determine steering once path intersections have been found
Vec3 Obstacle::PathIntersection::steerToAvoidIfNeeded (const AbstractVehicle& vehicle, const float minTimeToCollision) const{
    // if nearby intersection found, steer away from it, otherwise no steering
    const float minDistanceToCollision = minTimeToCollision * vehicle.speed();
    if (intersect && (distance < minDistanceToCollision))
    {
        // compute avoidance steering force: take the component of
        // steerHint which is lateral (perpendicular to vehicle's
        // forward direction), set its length to vehicle's maxForce
        Vec3 lateral = steerHint.perpendicularComponent (vehicle.forward ());
        return lateral.normalize () * vehicle.maxForce ();
    }
    else
    {
        return Vec3::zero;
    }
}


} // namespace OpenSteer
#endif
