// Copyright NVIDIA Corporation 2002-2005
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once
/** \file */

#include "nvsgcommon.h"

#include  "nvmath/nvmath.h"
#include  "nvmath/Vec3f.h"

namespace nvsg
{  
  /*! \brief A helper class for use in animation.
   *  \par Namespace: nvsg
   *  \remarks A VNVector holds a vector of vertices and normals. It is used to produce animations
   *  on vertices/normals.
   *  \sa AnimatedTriangles, AnimatedQuads */
  class VNVector
  {
    public:
      //! Equality operator.
      /** \returns \c true if the all vertices and normals of the \c this are equal to the
        * corresponding vertices and normals of \a v, otherwise \c false */
      bool operator==( const VNVector &v ) const;

    public:
      std::vector<nvmath::Vec3f> normals;   //!<  vector of normals
      std::vector<nvmath::Vec3f> vertices;  //!<  vector of vertices
  };

  inline bool VNVector::operator==( const VNVector &v ) const
  {
    return( ( normals == v.normals ) && ( vertices == v.vertices ) );
  }
}

namespace nvmath
{
  //! Linear interpolation between two \c VNVector
  /** Performs a linear interpolation v = v0 + alpha * ( v1 - v0 ) of each corresponding vertex and normal of the two
    * \c VNVector. The interpolated normals are normalized then.
    * \note The two \c VNVector are assumed to be the same size.
    * \return a \c VNVector with the interpolated vertices and normals. */
  inline nvsg::VNVector lerp( float alpha                 //!<  interpolation parameter
                            , const nvsg::VNVector &v0    //!<  start value of interpolation
                            , const nvsg::VNVector &v1    //!<  end value of interpolation
                            )
  {
    nvsg::VNVector  vnv;

    NVSG_ASSERT( v0.vertices.size() == v1.vertices.size() );
    vnv.vertices.resize( v0.vertices.size() );
    for ( size_t i=0 ; i<v0.vertices.size() ; i++ )
    {
      vnv.vertices[i] = lerp( alpha, v0.vertices[i], v1.vertices[i] );
                      //^^^^ invoke general lerp template
    }

    NVSG_ASSERT( v0.normals.size()  == v1.normals.size() );
    vnv.normals.resize( v0.normals.size() );
    for ( size_t i=0 ; i<v0.normals.size() ; i++ )
    {
      vnv.normals[i] = normalize( lerp( alpha, v0.normals[i], v1.normals[i] ) );
                                //^^^^ invoke general lerp template
    }

    return( vnv );
  }
}
