#include "../../../../config.h"
#ifdef OPENSTEER
/*! \file osTerrainRayTest.h
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
#ifndef __TERRAINRAYTEST__
#define __TERRAINRAYTEST__

#include <irrlicht.h>

// This is so that everything can be changed to double precision if needed:
//#define TRT_DOUBLE_PRECISION

// This controls whether or not the data set is transformed
// or just the query values are transformed into the local coordinate system.
// This should probably be left commented out unless the entire pipeline is double precision
//#define TRT_TRANSFORM_DATA

// You may want to get better performance by precomputing normals,
// at the expense of a little extra computation time at load and more memory consumption.
// If you want the normals pre-computed, define the following:
#define TRT_PRECOMPUTE_NORMALS

// If you do not precompute the normals, you may not want the tester
// to normalize the collision normals if you have to rescale them later.
// This will save you a little computation for every collision.
// If you want the ray test normals pre-normalized, define the following:
//#define TRT_NORMALIZE


// Set up the typedef for floating point values
#include <float.h>
#ifdef TRT_DOUBLE_PRECISION
	typedef double TRTScalar;
	#define	TRT_INFINITY	DBL_MAX
#else
  //! typedef for floating point values
	typedef float TRTScalar;
	#define	TRT_INFINITY	FLT_MAX
#endif

//! The structure for raytest results
struct RayTestInfo {
	bool hitOccurred;				/*!< Infinite ray test collission */
	TRTScalar t;					  /*!< Normal scale to intersect point */
	TRTScalar pos[3];				/*!< Intersect point */
	TRTScalar norm[3];      /*!< Normal */
};


//! The ray tester object
class RayTester{
  private:
    //! Debug render struff
    irr::core::array<irr::video::S3DVertex> lstVertices;
    irr::core::array<irr::u16> lstIndices;
    void createMesh();
  public:
    RayTester();						// simple constructor
    ~RayTester();						// destructor
    //! load terrain data from file
    void LoadData(char *fname,	TRTScalar xMin=0, TRTScalar xMax=0,
                  TRTScalar yMin=0, TRTScalar yMax=0,
                  TRTScalar zMin=0, TRTScalar zMax=0 );
    //! load terrain data from a mesh
    void LoadData(irr::scene::IMesh* mesh,	irr::core::vector3df scale = irr::core::vector3df(1,1,1),	irr::core::vector3df pos = irr::core::vector3df(0,0,0));
    //! load terrain data from a node
    /*! this must be an ITerrainSceneNode, otherwise the result may be corrupted !!! */
    void LoadData(irr::scene::ITerrainSceneNode* terrainNode);
#ifdef USE_IrrExtensions_Terrain2
    /*! this must be an ITerrainSceneNode2, otherwise the result may be corrupted !!! */
 ///   void LoadData(irr::scene::ITerrainSceneNode2* terrainNode);
#endif // USE_IrrExtensions_Terrain2

    //! do a ray cast on the terrain
    void RayCast(RayTestInfo &results, const TRTScalar *eyePos, const TRTScalar *viewNorm, TRTScalar maxt=TRT_INFINITY ) const;

    //! Irrlicht render methode
    void render(irr::video::IVideoDriver* driver);

  private:
    int width, height;

    struct GridCell {
      TRTScalar maxy;
      TRTScalar pos[3];

      #ifdef TRT_PRECOMPUTE_NORMALS
        TRTScalar upLeftNorm[3];
        TRTScalar lowRightNorm[3];
      #endif
    };
    GridCell *data;
    bool transformData;

    TRTScalar minx,maxx,xrange,xstep;
    TRTScalar miny,maxy,yrange;
    TRTScalar minz,maxz,zrange,zstep;

    #ifndef TRT_TRANSFORM_DATA
      TRTScalar _xMin,_xRange;
      TRTScalar _yMin,_yRange;
      TRTScalar _zMin,_zRange;
    #endif

    void RayCastTriangle( RayTestInfo &results, const TRTScalar *eyePos, const TRTScalar *viewNorm,
                const TRTScalar *vert0, const TRTScalar *vert1, const TRTScalar *vert2 ) const;

    void RectifyResults( RayTestInfo &results ) const;

    void GetNormal( TRTScalar *r, const TRTScalar *u, const TRTScalar *v, const TRTScalar *w ) const;
    void Normalize( TRTScalar *v ) const;

};



#endif
#endif
