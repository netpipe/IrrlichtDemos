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

#include "nvmath/Vec2f.h"
#include "nvmath/Vec3f.h"

namespace nvutil
{
  
  //! Trackball class for simulating a virtual trackball.
  /** This class simulates a virtual trackball using the last and current mouse position.
    * It simply projects the stuff onto a sphere or hyperbolic sheet and calculates the 
    * axis and angle that is needed to rotate the trackball from the last to the current 
    * position.
    */
  class Trackball
  {
    public:
      //! Default constructor
      Trackball(void);

      //! Default destructor
      virtual ~Trackball(void);

      //! Start trackball calculation
      /** Calculate the axis and the angle (radians) 
        * by the given mouse coordinates.
        * Project the points onto the virtual
        * trackball, then figure out the axis of rotation, which is the cross
        * product of p0 p1 and O p0 (O is the center of the ball, 0,0,0)
        * \note This is a deformed trackball-- is a trackball in the center,
        * but is deformed into a hyperbolic sheet of rotation away from the
        * center.
        */
      void  apply( const nvmath::Vec2f &p0   //!< Last mouse position - components must be scaled to [-1,1]
                 , const nvmath::Vec2f &p1   //!< Current mouse position - components must be scaled to [-1,1]
                 , nvmath::Vec3f & axis      //!< Resulting axis
                 , float & rad       //!< Resulting angle
                 );
      
      //! Set trackball size.
      /** Define the size of the trackball. Default size is 0.8f.
       *  This size should really be based on the distance from the center of
       *  rotation to the point on the object underneath the mouse.  That
       *  point would then track the mouse as closely as possible.
       */
      void  setSize( float size   //!< %Trackball size
                   );

      //! Determine current size of the trackball.
      /** \return %Trackball size
        */
      float getSize( void );
      
    protected:
      //! Project the x,y mouse position onto a sphere.
      /** Project an x,y pair onto a sphere of radius r OR a hyperbolic sheet
        * if we are away from the center of the sphere.
        */
      float projectOntoTBSphere( const nvmath::Vec2f & p );

      float m_tbsize;  //!< %Trackball size (default is 0.8f)
  };

  inline  void  Trackball::setSize( float size )
  {
    NVSG_ASSERT( m_tbsize > 0.f );
    m_tbsize = size;
  }

  inline float Trackball::getSize( void )
  {
    return( m_tbsize );
  }

} //  namespace nvui
