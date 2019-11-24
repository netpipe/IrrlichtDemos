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
#include "nvmath/Sphere3f.h"
#include "nvsg/CullData.h"
#include "nvtraverser/ModelViewTraverser.h"
#include <map>

// forward declarations
namespace nvsg
{
  class Drawable;
  class Node;
  class GeoNode;
  class Group;
  class Scene;
}

namespace nvtraverser
{
  /*! \brief Base class for culling parts of a scene.
   *  \par Namespace: nvsg
   *  \remarks This is the base class for a culling traverser. A CullTraverser creates a tree of
   *  nvsg::CullData, representing the culled tree. After completion, that tree of nvsg::CullData
   *  is put into the nvsg::ViewState. From then on, the Traverser pays respect to that cull tree.\n
   *  That said, it is easy to concatenate two or more culling traversers, by just applying them
   *  sequentially to the scene. In each culling traversal the cull tree from a previous culling
   *  is used, that is only the cull state of objects being at least partially in are to be
   *  determined by subsequent CullTraverser traversals.\n
   *  A Traverser derived from CullTraverser essentially has to override the function cullCode().
   *  \sa FrustumCullTraverser, ModelViewTraverser */
  class CullTraverser : public SharedModelViewTraverser
  {
    public:

    protected:
      /*! \brief Protected default constructor to prevent explicit creation. */
      NVSG_API CullTraverser();

      /*! \brief Protected destructor to prevent explicit creation.
       *  \remarks The destructor is never used explicitly, but only when the reference count of
       *  the CullTraverser is decremented to zero. */
      NVSG_API virtual ~CullTraverser(void);

      /*! \brief Override of the doApply method.
       *  \param scene A pointer to the constant nvsg::Scene to cull.
       *  \remarks The doApply method is the traversal entry point of a Traverser. After the
       *  traversal is done by ModelViewTraverser::doApply(), the resulting tree of nvsg::CullData
       *  is balanced and set as the current cull tree into the nvsg::ViewState.
       *  \sa CullData, nvsg::Scene, ModelViewTraverser, nvsg::ViewState */
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      NVSG_API virtual void postApply(nvsg::NodeHandle * root);

      /*! \brief  Determine the CullCode of the sphere \a bs.
       *  \param bs a constant reference to a bounding sphere.
       *  \return The nvsg::CullCode of the bounding sphere \a bs.
       *  \remarks The default implementation returns the nvsg::CullCode CC_IN. */
      NVSG_API virtual nvsg::CullCode cullCode( const nvmath::Sphere3f & bs ) const;

      /*! \brief Determine the nvsg::CullData of \a p.
       *  \param p A pointer to the constant nvsg::Billboard to handle.
       *  \remarks If \a p is partially visible (CC_PART), it is traversed and the nvsg::CullData
       *  of all its children are gathered beneath the nvsg::CullData of \a p.
       *  \sa handleGroup, handleLOD, handleSwitch, handleTransform */
      NVSG_API virtual void handleBillboard( const nvsg::Billboard * p );

      /*! \brief Determine the nvsg::CullData of \a p.
       *  \param p A pointer to the constant nvsg::GeoNode to handle.
       *  \remarks If \a p is partially visible (CC_PART), it is traversed and the nvsg::CullData
       *  of all its nvsg::Drawable objects is gathered beneath the nvsg::CullData of \a p.
       *  \sa traversePrimitiveSet */
      NVSG_API virtual void handleGeoNode( const nvsg::GeoNode * p );

      /*! \brief Determine the nvsg::CullData of \a p.
       *  \param p A pointer to the constant nvsg::Group to handle.
       *  \remarks If \a p is partially visible (CC_PART), it is traversed and the nvsg::CullData
       *  of all its children are gathered beneath the nvsg::CullData of \a p.
       *  \sa handleBillboard, handleLOD, handleSwitch, handleTransform */
      NVSG_API virtual void handleGroup( const nvsg::Group * p );

      /*! \brief Determine the nvsg::CullData of \a p.
       *  \param p A pointer to the constant nvsg::LOD to handle.
       *  \remarks If \a p is partially visible (CC_PART), it is traversed and the nvsg::CullData
       *  of all its children are gathered beneath the nvsg::CullData of \a p.
       *  \sa handleBillboard, handleGroup, handleSwitch, handleTransform */
      NVSG_API virtual void handleLOD( const nvsg::LOD * p );

      /*! \brief Determine the nvsg::CullData of \a p.
       *  \param p A pointer to the constant nvsg::Switch to handle.
       *  \remarks If \a p is partially visible (CC_PART), it is traversed and the nvsg::CullData
       *  of all its children are gathered beneath the nvsg::CullData of \a p.
       *  \sa handleBillboard, handleGroup, handleLOD, handleTransform */
      NVSG_API virtual void handleSwitch( const nvsg::Switch * p );

      /*! \brief Determine the nvsg::CullData of \a p.
       *  \param p A pointer to the constant nvsg::Transform to handle.
       *  \remarks If \a p is partially visible (CC_PART), it is traversed and the nvsg::CullData
       *  of all its children are gathered beneath the nvsg::CullData of \a p.
       *  \sa handleBillboard, handleGroup, handleLOD, handleSwitch */
      NVSG_API virtual void handleTransform( const nvsg::Transform * p );

      /*! \brief Determine the nvsg::CullData of \a p.
       *  \param p A pointer to the nvsg::VolumeNodeHandle to handle.
       *  \remarks The nvsg::CullCode of the nvsg::VolumeNodeHandle \a p is determined and the
       *  result is recorded in the nvsg::CullData tree. */
      NVSG_API virtual void handleVolumeNode( const nvsg::VolumeNode * p );

      /*! \brief Handle a nvsg::PrimitiveSet by determining its nvsg::CullCode.
       *  \param p A pointer to the constant nvsg::PrimitiveSet to handle.
       *  \remarks The nvsg::CullCode of the nvsg::PrimitiveSet \a p is determined via the
       *  function cullCode() and the results are recorded in the nvsg::CullData tree.
       *  \sa cullCode */
      NVSG_API virtual void traversePrimitiveSet( const nvsg::PrimitiveSet * p );

      /*! \brief Get the hints of the currently handled Node.
       *  \return The hints of the currently handled Node.
       *  \remarks On traversal, the hints and the hints containment of each Node is stored in
       *  this CullTraverser, to have access to it in functions that don't have a direct access
       *  to that Node, like cullCode.
       *  \sa getCurrentHintsContainment */
      NVSG_API unsigned short getCurrentHints() const;

      /*! \brief Get the hints containment of the currently handled Node.
       *  \return The hints containment of the currently handled Node.
       *  \remarks On traversal, the hints and the hints containment of each Node is stored in
       *  this CullTraverser, to have access to it in functions that don't have a direct access
       *  to that Node, like cullCode.
       *  \sa getCurrentHints */
      NVSG_API unsigned short getCurrentHintsContainment() const;

    private:
      typedef std::map<nvsg::ObjectHandle*,nvsg::CullData*> ObjectCullMap;

    private:
      nvsg::CullCode determineCullCode( const nvmath::Sphere3f & bs ) const;
      nvsg::CullData * determineCullData( nvsg::ObjectHandle * p, const nvmath::Sphere3f & bs );
      bool isPartial( const nvsg::Node * p );
      bool isPartial( nvsg::ObjectHandle * oh, const nvmath::Sphere3f & bs );
      void postTraverseObject( const ObjectCullMap::iterator it, nvsg::CullData *cd );
      void postTraverseVolumeBricks( const nvsg::VolumeNode * p
                                   , nvsg::VolumeBrickHandle * vbh
                                   , unsigned int level, nvsg::CullData * cd
                                   , ObjectCullMap * ocm );
      void registerCullCodes( const nvsg::GeoNode * p );
      void registerCullCodes( const nvsg::Group * p );
      void registerCullCodes( nvsg::VolumeBrickHandle * volBrick );

    private:
      std::stack<nvsg::CullData *>    m_cullDataStack;
      std::stack<ObjectCullMap>       m_objectCullMapStack;
      nvsg::CullData                * m_cullTree;
      unsigned short                  m_currentHints;
      unsigned short                  m_currentHintsContainment;
  };


  inline unsigned short CullTraverser::getCurrentHints() const
  {
    return( m_currentHints );
  }

  inline unsigned short CullTraverser::getCurrentHintsContainment() const
  {
    return( m_currentHintsContainment );
  }

}
