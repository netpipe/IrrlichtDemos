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

#include "nvui/Manipulator.h"

namespace nvsg
{
  class Transform;
  class Path;
}

namespace nvui
{
  /*! \brief Base class for all kinds of transform manipulators.
   *  \par Namespace: nvui
   *  \remarks This class is the base class for all manipulators working on nvsg::Transforms. 
   *  The TransformManipulator needs a full path from the root of the tree to the controlled 
   *  nvsg::Transform node.
   *  \n\n
   *  See the TransformManipulatorViewer tutorial source code on how to use a 
   *  Transform manipulator in your application.
   *  \n\n
   *  If you use this manipulator your application must use a mechanism,
   *  such as using OnIdle() that calls updateFrame(). If updateFrame() returns true
   *  you should call triggerRedraw of the render area so the updated frame will be drawn.
   *  You will find some code samples in the description of the Manipulator class. Also 
   *  the SimpleViewer tutorial source code shows how to use a manipulator with MFC,
   *  the ConsoleViewer tutorial source code shows the details on how to use a manipulator, and 
   *  the ManipSampleViewer tutorial source code shows how to derive a new Manipulator class.
   *  \sa Manipulator*/
  class TransformManipulator : public Manipulator
  {
  public:
    /*! \brief Default constructor.
     *  \remarks Initialize the TransformManipulator object.*/
    NVSG_API TransformManipulator();
 
    /*! \brief Default destructor.
     *  \remarks The destructor takes care of decrementing the reference count of the 
     *  provided nvsg::Path object.
     *  \sa setTransformPath */ 
    NVSG_API virtual ~TransformManipulator();

    /*! \brief Set the nvsg::Path to the nvsg::Transform node.
     *  \param transformPath Complete path to the nvsg::Transform node. Null is a valid value to 
     *  disconnect the TransformManipulator from the controlled object.
     *  \remarks Attach the Manipulator to the desired nvsg::Transform in the tree by providing 
     *  a complete nvsg::Path from the root node to the nvsg::Transform node.\n
     *  This class takes care of incrementing and decrementing the reference count of the provided
     *  nvsg::Path object.
     *  \n\n 
     *  The application is responsible to make sure that the nvsg::Path stays 
     *  valid during the usage of the Manipulator.
     *  \sa getTransformPath */
    NVSG_API void setTransformPath(const nvsg::Path * transformPath);

    /*! \brief Get the nvsg::Path to the nvsg::Transform that currently is under control.
     *  \return Path to the controlled nvsg::Transform. If the TransformManipulator is not 
     *  connected this function returns NULL.
     *  \remarks  The application is responsible to make sure that the nvsg::Path stays 
     *  valid during the usage of the Manipulator.
     *  \sa setTransformPath */
    NVSG_API const nvsg::Path * getTransformPath() const;

  protected:
    const nvsg::Path * m_transformPath;  //!< Complete nvsg::Path to the nvsg::Transform node.
  };

  inline const nvsg::Path * TransformManipulator::getTransformPath() const
  {
    return( m_transformPath );
  }
}