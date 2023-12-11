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
#include "nvtraverser/Traverser.h"
#include "nvutil/TransformStack.h"


namespace nvtraverser
{

  //! Traverser that can handle the model view transform stack.
  /** Use this class as a base for traversers that need to maintain the
    * current model view transformation. 
    * \note All ModelViewTraverser based classes need a valid \link nvsg::ViewState nvsg::ViewState \endlink.
    * If the ViewState is not valid the runtime behaviour is not defined. Call setViewState() 
    * with a valid ViewState prior calling apply().*/
  class SharedModelViewTraverser : public SharedTraverser
  {
    protected:
      //! Protected constructor to prevent instantiation of a ModelViewTraverser.
      /** \note A SharedModelViewTraverser doesn't change anything in the scene graph, but might be used by a modifying traverser.
        * therefore, it gets the readOnly flag as a parameter to pass to Traverser. */
      NVSG_API SharedModelViewTraverser();

      //! Protected destructor to prevent instantiation of a SharedModelViewTraverser.
      NVSG_API virtual ~SharedModelViewTraverser();

      //! Provide special treatment of an AnimatedTransform node.
      /** An AnimatedTransform is handled as a Transform. */
      NVSG_API virtual void handleAnimatedTransform( const nvsg::AnimatedTransform *p //!< currently visited AnimatedTransform
                                                  );

      //! Provide special treatment of a Billboard node.
      /** On a Billboard the modelview matrix is modified, the children are traversed, and the modelview matrix is
        * restored. */
      NVSG_API virtual void handleBillboard( const nvsg::Billboard *p   //!<  Billboard to handle
                                           );

      NVSG_API virtual void handleLOD( const nvsg::LOD *lod );

      //! Provide special treatment of a Transform node.
      /** On a Transform the modelview matrix is modified, the children are traversed, and the modelview matrix is
        * restored. */
      NVSG_API virtual void handleTransform( const nvsg::Transform *p          //!<  Transform to handle
                                          );

      //! Handles actions to take between transform stack adjustment and traversal.
      /** In this base class, this is a NOP.  */
      NVSG_API virtual bool preTraverseTransform( const nvmath::Trafo *p        //!< Trafo of node to traverse next 
                                                );

      //! Handles actions to take between traversal and transform stack adjustment.
      /** When this function returns true, the subtree beneath is traversed. Otherwise it isn't.
        * \return true */
      NVSG_API virtual void postTraverseTransform( const nvmath::Trafo *p       //!< Trafo of node that was traversed immediately before this call. 
                                                );

      NVSG_API virtual void traverseCamera(const nvsg::Camera * camera);

    protected:
      nvmath::Vec3f           m_cameraPosition;
      nvmath::Vec3f           m_cameraUpVector;
      nvutil::TransformStack  m_transformStack; //!<  stack of transformations that holds the actual transformations while traversing a tree
  };

  //! Traverser that can handle the model view transform stack.
  /** Use this class as a base for traversers that need to maintain the
  * current model view transformation. 
  * \note All ModelViewTraverser based classes need a valid \link nvsg::ViewState nvsg::ViewState \endlink.
  * If the ViewState is not valid the runtime behaviour is not defined. Call setViewState() 
  * with a valid ViewState prior calling apply().*/
  class ExclusiveModelViewTraverser : public ExclusiveTraverser
  {
  protected:
    //! Protected constructor to prevent instantiation of a ModelViewTraverser.
    /** \note A ExclusiveModelViewTraverser doesn't change anything in the scene graph, but might be used by a modifying traverser.
    * therefore, it gets the readOnly flag as a parameter to pass to Traverser. */
    NVSG_API ExclusiveModelViewTraverser();

    //! Protected destructor to prevent instantiation of a ExclusiveModelViewTraverser.
    NVSG_API virtual ~ExclusiveModelViewTraverser();

    //! Provide special treatment of an AnimatedTransform node.
    /** An AnimatedTransform is handled as a Transform. */
    NVSG_API virtual void handleAnimatedTransform( nvsg::AnimatedTransform *p //!< currently visited AnimatedTransform
      );

    //! Provide special treatment of a Billboard node.
    /** On a Billboard the modelview matrix is modified, the children are traversed, and the modelview matrix is
    * restored. */
    NVSG_API virtual void handleBillboard( nvsg::Billboard *p   //!<  Billboard to handle
      );

    NVSG_API virtual void handleLOD( nvsg::LOD *lod );

    //! Provide special treatment of a Transform node.
    /** On a Transform the modelview matrix is modified, the children are traversed, and the modelview matrix is
    * restored. */
    NVSG_API virtual void handleTransform( nvsg::Transform *p          //!<  Transform to handle
      );

    //! Handles actions to take between transform stack adjustment and traversal.
    /** In this base class, this is a NOP.  */
    NVSG_API virtual bool preTraverseTransform( const nvmath::Trafo *p        //!< Trafo of node to traverse next 
      );

    //! Handles actions to take between traversal and transform stack adjustment.
    /** When this function returns true, the subtree beneath is traversed. Otherwise it isn't.
    * \return true */
    NVSG_API virtual void postTraverseTransform( const nvmath::Trafo *p       //!< Trafo of node that was traversed immediately before this call. 
      );

    NVSG_API virtual void traverseCamera(nvsg::Camera * camera);

  protected:
    nvmath::Vec3f           m_cameraPosition;
    nvmath::Vec3f           m_cameraUpVector;
    nvutil::TransformStack  m_transformStack; //!<  stack of transformations that holds the actual transformations while traversing a tree
  };
}
