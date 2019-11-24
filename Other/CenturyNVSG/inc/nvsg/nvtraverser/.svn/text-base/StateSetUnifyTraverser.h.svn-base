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

#include <set>
#include "nvsgcommon.h"
#include "nvtraverser/OptimizeTraverser.h"

namespace nvsg
{
  class StateAttribute;
  class TextureAttributeItem;
}

namespace nvtraverser
{
  //! Traverser that unifies identical StateSets.
  /** Identical StateSets are recognized and linked to the same one.
    * Identical StateAttributes are recognized and linked to the same one.
    * This way, redundant objects are removed, thereby reducing storage space as well as run time. */
  class StateSetUnifyTraverser : public OptimizeTraverser
  {
    public:
      //! Constructor
      NVSG_API StateSetUnifyTraverser( void );

    protected:
      //! Protected destructor to prevent instantiation of a StateSetUnifyTraverser on stack.
      NVSG_API virtual ~StateSetUnifyTraverser( void );

      /*! \brief Override of the doApply method.
       *  \param scene A pointer to the constant Scene to unify StateSet objects in.
       *  \remarks The doApply method is the traversal entry point of a Traverser. The local data
       *  is cleared and OptimizeTraverser::doApply() is called to start traversal.
       *  \sa nvsg::Scene, Traverser */
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      //! Routine to handle a \link nvsg::GeoNode GeoNode \endlink object while traversing the scene graph.
      /** After traversing the GeoNode \a p, all StateSets used in this GeoNode are tested for equivalence with others.
        * All StateSets that are equivalent to an other already encountered one are replaced by that. */
      NVSG_API virtual void handleGeoNode( nvsg::GeoNode *p );

      //! Routine to handle a \link nvsg::StatePass StatePass \endlink object while traversing the scene graph.
      /** After traversing the StatePass \a p, all StateAttributes used in this StatePass are tested for equivalence with
        * others. All StateAttributes that are equivalent to an other already encountered one are replaced by that. */
      NVSG_API virtual void handleStatePass( nvsg::StatePass * p );

      //! Routine to handle a \link nvsg::StateSet StateSet \endlink object while traversing the scene graph.
      /** After traversing the StateSet \a p, all StateVariants used in this StateSet are tested for equivalence with
        * others. All StateVariants that are equivalent to an other already encountered one are replaced by that. */
      NVSG_API virtual void handleStateSet( nvsg::StateSet * p );

      //! Routine to handle a \link nvsg::StateVariant StateVariant \endlink object while traversing the scene graph.
      /** After traversing the StateVariant \a p, all StatePasses used in this StateVariant are tested for equivalence with
        * others. All StatePasses that are equivalent to an other already encountered one are replaced by that. */
      NVSG_API virtual void handleStateVariant( nvsg::StateVariant * p );

      //! Routine to handle a \link nvsg::TextureAttribute TextureAttribute \endlink object while traversing the scene graph.
      /** After traversing the TextureAttribute \a p, all TextureAttributeItems used in this TextureAttribute are tested for
        * equivalence with others. All TextureAttributeItems that are equivalent to an other already encountered one are
        * replaced by that. */
      NVSG_API virtual void handleTextureAttribute( nvsg::TextureAttribute * p );

    private:
      std::map<unsigned int, std::set<nvsg::StateAttributeHandle *> > m_stateAttributes;
      std::set<nvsg::StatePassHandle *>                               m_statePasses;
      std::set<nvsg::StateSetHandle *>                                m_stateSets;
      std::set<nvsg::StateVariantHandle *>                            m_stateVariants;
      std::set<nvsg::TextureAttributeItemHandle *>                    m_textureAttributeItems;
  };

}
