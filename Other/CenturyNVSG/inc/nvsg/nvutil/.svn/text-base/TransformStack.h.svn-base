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

#include <stack>
#include "nvmath/Mat44f.h"
 
namespace nvutil
{
  //! Utility class to hold a stack of concatenated transformation matrices.
  /** This class makes it easy to maintain all steps of the transformation pipeline from model space via world space and
    * view space to clip space (and vice versa).  \n
    * Individiual objects are defined in an local coordinate system called \e model space. There is often a hierarchy of
    * modeling coordinate systems that are maintained by this class by using \c popModelToWorld and \c pushModelToWorld.
    * \n
    * Objects are then transformed into the \e world space. \n
    * The \e view space is used to define a view volume. This system is used so that, with the eye or camera at the
    * origin looking toward -z, decreasing values of z are farther away from the eye, x is to the right and y is up. \n
    * From \e view space, we next go to the \e clip space, that is we're using the projection to get logical 2D device
    * coordinates (In contrast to physical device coordinates, which additionally need the viewport information).
    * \par
    * With these terms, the transformation pipeline looks like that:
    * \code
    *   model <-> world <-> view <-> clip
    * \endcode
    */
  class TransformStack
  {
    public:
      //! Default constructor.
      /** Initializes the model <-> world transformation stacks.  */
      TransformStack();

    public:
      //! Get the transformation from \e clip space to \e model space.
      NVSG_API const nvmath::Mat44f    getClipToModel( void )  const;

      //! Get the transformtion from \e clip space to \e view space.
      NVSG_API const nvmath::Mat44f  & getClipToView( void )   const;

      //! Get the transformtion from \e model space to \e clip space.
      NVSG_API const nvmath::Mat44f    getModelToClip( void )  const;

      //! Get the transformtion from \e model space to \e view space.
      NVSG_API const nvmath::Mat44f    getModelToView( void )  const;

      //! Get the transformtion from \e model space to \e world space.
      NVSG_API const nvmath::Mat44f  & getModelToWorld( void ) const;

      //! Get the transformtion from \e view space to \e clip space.
      NVSG_API const nvmath::Mat44f  & getViewToClip( void )   const;

      //! Get the transformtion from \e view space to \e model space.
      NVSG_API const nvmath::Mat44f    getViewToModel( void )  const;

      //! Get the transformtion from \e view space to \e world space.
      NVSG_API const nvmath::Mat44f  & getViewToWorld( void )  const;

      //! Get the transformtion from \e world space to \e model space.
      NVSG_API const nvmath::Mat44f  & getWorldToModel( void ) const;

      //! Get the transformtion from \e world space to \e view space.
      NVSG_API const nvmath::Mat44f  & getWorldToView( void )  const;

      //! Get the depth of the model <-> world transformation stack.
      NVSG_API size_t    getStackDepth( void )   const;

      //! Pop the top element of the model <-> world transformation stack.
      NVSG_API void      popModelToWorld( void );

      //! Push a new pair of matrices on the model <-> world transformation stack.
      /** \note \a modelWorld and \a worldModel should be inverse to each other to get correct results. */
      NVSG_API void      pushModelToWorld( const nvmath::Mat44f &modelWorld    //!<  additional model to world transformation 
                                         , const nvmath::Mat44f &worldModel    //!<  additional world to model transformation
                                         );

      //! Set the pair of matrices for the view <-> clip transformation.
      /** \note \a viewClip and \a clipView should be inverse to each other to get correct results. */
      NVSG_API void      setViewToClip( const nvmath::Mat44f &viewClip         //!<  new view to clip transformation (projection)
                                      , const nvmath::Mat44f &clipView         //!<  new clip to view transformation
                                      );

      //! Set the pair of matrices for the world <-> view transformation.
      /** \note \a worldView and \a viewWorld should be inverse to each other to get correct results. */
      NVSG_API void      setWorldToView( const nvmath::Mat44f &worldView       //!<  new world to view transformation
                                       , const nvmath::Mat44f &viewWorld       //!<  new view to world transformation
                                       );

    private :
      std::stack<nvmath::Mat44f> m_modelWorld;     //!<  stack of concatentated model to world matrices
      std::stack<nvmath::Mat44f> m_worldModel;     //!<  stack of concatentated world to model matrices
      nvmath::Mat44f        m_worldView;      //!<  world to view matrix
      nvmath::Mat44f        m_viewWorld;      //!<  view to world matrix
      nvmath::Mat44f        m_viewClip;       //!<  view to eye matrix (projection)
      nvmath::Mat44f        m_clipView;       //!<  eye to view matrix (inverse projection)
  };


  inline  TransformStack::TransformStack()
  {
    m_modelWorld.push( nvmath::cIdentity44f );
    m_worldModel.push( nvmath::cIdentity44f );
  }

  inline  const nvmath::Mat44f    TransformStack::getClipToModel( void )  const
  {
    return( m_clipView * m_viewWorld * m_worldModel.top() );
  }

  inline  const nvmath::Mat44f  & TransformStack::getClipToView( void )   const
  {
    return( m_clipView );
  }

  inline  const nvmath::Mat44f    TransformStack::getModelToClip( void )  const
  {
    return( m_modelWorld.top() * m_worldView * m_viewClip );
  }

  inline  const nvmath::Mat44f    TransformStack::getModelToView( void )  const
  {
    return( m_modelWorld.top() * m_worldView );
  }

  inline  const nvmath::Mat44f  & TransformStack::getModelToWorld( void ) const
  {
    return( m_modelWorld.top() );
  }
  
  inline  const nvmath::Mat44f  & TransformStack::getViewToClip( void )   const
  {
    return( m_viewClip );
  }
  
  inline  const nvmath::Mat44f    TransformStack::getViewToModel( void )  const
  {
    return( m_viewWorld * m_worldModel.top() );
  }
  
  inline  const nvmath::Mat44f  & TransformStack::getViewToWorld( void )  const
  {
    return( m_viewWorld );
  }
  
  inline  const nvmath::Mat44f  & TransformStack::getWorldToModel( void ) const
  {
    return( m_worldModel.top() );
  }
  
  inline  const nvmath::Mat44f  & TransformStack::getWorldToView( void )  const
  {
    return( m_worldView );
  }

  inline  size_t  TransformStack::getStackDepth( void ) const
  {
    NVSG_ASSERT( m_modelWorld.size() == m_worldModel.size() );
    return( m_modelWorld.size() );
  }

  inline  void  TransformStack::popModelToWorld( void )
  {
    m_modelWorld.pop();
    m_worldModel.pop();
  }

  inline  void  TransformStack::pushModelToWorld( const nvmath::Mat44f &modelWorld, const nvmath::Mat44f &worldModel )
  {
    NVSG_TRACE();
    NVSG_TRACE_MAT44F(modelWorld);
    NVSG_TRACE_MAT44F(worldModel);
    NVSG_TRACE_MAT44F((worldModel*modelWorld));
    m_modelWorld.push( modelWorld * m_modelWorld.top() );
    m_worldModel.push( m_worldModel.top() * worldModel );
  }

  inline  void  TransformStack::setViewToClip( const nvmath::Mat44f &viewClip, const nvmath::Mat44f &clipView )
  {
    NVSG_TRACE();
    NVSG_TRACE_MAT44F(viewClip);
    NVSG_TRACE_MAT44F(clipView);
    m_viewClip = viewClip;
    m_clipView = clipView;
  }

  inline  void  TransformStack::setWorldToView( const nvmath::Mat44f &worldView, const nvmath::Mat44f &viewWorld )
  {
    NVSG_TRACE();
    NVSG_TRACE_MAT44F(worldView);
    NVSG_TRACE_MAT44F(viewWorld);
    m_worldView = worldView;
    m_viewWorld = viewWorld;
  }
}
