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
#include "nvsg/CullData.h"
#include "nvtraverser/CullTraverser.h"

namespace nvtraverser
{
  /*! \brief CullTraverser culling on frustum information.
   *  \par Namespace: nvsg
   *  \remarks The FrustumCullTraverser determines the nvsg::CullCode of all nvsg::Node objects in
   *  a nvsg::Scene with respect to the view frustum. As a CullTraverser, it creates a tree of
   *  nvsg::CullData from those nvsg::CullCode.
   *  \note The FrustumCullTraverser needs a valid ViewState, so use setViewState() prior to
   *  apply().  
   *  \sa CullTraverser, nvsg::CullCode, nvsg::CullData */
  class FrustumCullTraverser : public CullTraverser
  {
    public:
      /*! \brief Default Constructor
       *  \remarks A FrustumCullTraverser does not change the Scene. */
      NVSG_API FrustumCullTraverser(void);

    protected:
      /*! \brief Protected destructor to prevent explicit creation.
       *  \remarks The destructor is never used explicitly, but only when the reference count of
       *  the FrustumCullTraverser is decremented to zero. */
      NVSG_API virtual ~FrustumCullTraverser(void);

      NVSG_API virtual void doApply( nvsg::NodeHandle * root );
        
      /*! \brief Re-implementation of the preTraverseTransform method.
       *  \param p A pointer to the constant nvmath::Trafo of the nvsg::Transform to traverse.
       *  \return \c true, if the nvsg::Transform can be traversed, otherwise \c false.
       *  \remarks This method stores the scaling information of the nvmath::Trafo \a p for usage
       *  in the nvsg::CullCode calculations and then returns the result of
       *  CullTraverser::preTraverseTransform().
       *  \sa ModelViewTraverser */
      NVSG_API virtual bool preTraverseTransform( const nvmath::Trafo *p );

      /*! \brief Re-implementation of the postTraverseTransform method.
       *  \param p A pointer to the constant nvmath::Trafo of the nvsg::Transform just traversed.
       *  \remarks This method calls CullTraverser::postTraverseTransform() and restores the
       *  scaling information to the state without this nvsg::Transform.
       *  \sa ModelViewTraverser */
      NVSG_API virtual void postTraverseTransform( const nvmath::Trafo *p );

      /*! \brief Re-implementation of the cullCode method.
       *  \param bs A constant reference to the bounding sphere to determine the nvsg::CullCode of.
       *  \return The nvsg::CullCode of the bounding sphere \a bs relative to the view frustum.
       *  \note If \a bs is empty, it is considered to be in.
       *  \sa CullTraverser */
      NVSG_API virtual nvsg::CullCode cullCode( const nvmath::Sphere3f & bs ) const;

    private:
      const nvsg::Camera  * m_camera;
      std::stack<float>     m_scaleFactors;
  };

}

