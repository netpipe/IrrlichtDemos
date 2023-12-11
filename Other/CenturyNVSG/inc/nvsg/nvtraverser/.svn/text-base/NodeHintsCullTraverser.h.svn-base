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

namespace nvsg
{
  class Node;
}

namespace nvtraverser
{ 
  /*! \brief
    * CullTraverser that culls based on NodeHints information.
    * \par
    * Namespace: nvtraverser
    * \remarks
    * The NodeHintsCullTraverser determines the nvsg::CullCode of nvsg::Node objects in a
    * nvsg::Scene with respect to the NodeHints specified for that node. Specifically, if
    * the \a NodeHints of a \a node under consideration match those of this traverser (specified at
    * construction, or using \a setHints, \a addHints, \a removeHints), \a cullCode returns \a CC_OUT.
    * \n\n
    * As a CullTraverser, it creates a tree of nvsg::CullData from these nvsg::CullCode's.
    * \n\n
    * NodeHintsCullTraverser needs a valid ViewState, therefore use setViewState() prior to apply().
    * \sa
    * CullTraverser, nvsg::CullCode, nvsg::CullData, FrustumCullTraverser */
  class NodeHintsCullTraverser : public CullTraverser
  {
  public:
    /*! \brief
      * Default Constructor
      * \param
      * uisNodeHints NodeHints information on the basis of which to cull.
      * \remarks
      * NodeHintsCullTraverser does not modify the scene. */    
    NVSG_API NodeHintsCullTraverser(unsigned short uisNodeHints = nvsg::Node::NH_CULL_NEVER);

    /*! \brief
      * Get the current hints which determinine the culling.
      * \param
      * mask An optional unsigned short to mask the hints to get. With the default mask,
      * every hint is returned.
      * \return
      * An unsigned short whose bits encode the hints determining containment.
      * \sa
      * setHints, addHints, removeHints, nvsg::Node::NodeHints */
    NVSG_API unsigned short getHints( unsigned short mask = 0xFFFF ) const;

    /*! \brief
      * Set the hints which determine the culling.
      * \param
      * hints An unsigned short whose bits encode the hints determining containment.
      * \sa
      * getHints, addHints, removeHints, nvsg::Node::NodeHints */
    NVSG_API void setHints( unsigned short hints );

    /*! \brief
      * Convenience function to add hints to the current set of hints.
      * \param hints An unsigned short whose bits encode the hints to add to the existin set of hints.
      * \sa getHints, setHints, removeHints, nvsg::Node::NodeHints */
    NVSG_API void addHints( unsigned short hints );

    /*! \brief
      * Convenience function to remove some hints from the current set of hints.
      * \param
      * hints An unsigned short whose bits encode the hints to remove from the set of
      * hints of this traverser.
      * \remarks
      * The current set of hints is bitwise and'ed with the bitwise complement of the existing hints.
      * \sa
      * getHints, setHints, addHints, nvsg::Node::NodeHints */
    NVSG_API void removeHints( unsigned short hints );

  protected:
    /*! \brief 
      * Protected destructor to prevent explicit creation.
      * \remarks
      * The destructor is never used explicitly, but only when the reference count of
      * the NodeHintsCullTraverser is decremented to zero. */
    NVSG_API virtual ~NodeHintsCullTraverser(void);

    /*! \brief Re-implementation of the cullCode method.
     *  \param A constant reference to the bounding sphere of wich the nvsg::CullCode is to be computed.
     *  \return The nvsg::CullCode of the bounding sphere \a bs wrt the NodeHints.
     *  \sa CullTraverser */
    NVSG_API virtual nvsg::CullCode cullCode( const nvmath::Sphere3f & bs ) const;

  protected:
    // The current set of active hints
    unsigned short   m_NodeHints;
  };

  inline unsigned short NodeHintsCullTraverser::getHints( unsigned short mask ) const
  {
    return( m_NodeHints & mask );
  }

  inline void NodeHintsCullTraverser::setHints( unsigned short hints )
  {
    m_NodeHints = hints;
  }

  inline void NodeHintsCullTraverser::addHints( unsigned short hints )
  {
    m_NodeHints |= hints;
  }

  inline void NodeHintsCullTraverser::removeHints( unsigned short hints )
  {
    m_NodeHints &= ~hints;
  }
}