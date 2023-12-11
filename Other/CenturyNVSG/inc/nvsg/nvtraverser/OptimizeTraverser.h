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

#include <list>
#include "nvsgcommon.h"
#include "nvtraverser/Traverser.h"

namespace nvtraverser
{
  //! Base for Traversers that perform some optimizations on a scene.
  class OptimizeTraverser : public ExclusiveTraverser
  {
    public:
      //! Constructor
      NVSG_API OptimizeTraverser( void );

    public:
      //! Get the 'ignore names' flag.
      /** If the 'ignore names' flag is set, the names of the GeoNodes are ignored.
        * \return true if the names will be ignored, otherwise false */
      NVSG_API bool getIgnoreNames( void ) const;

      //! Set the 'ignore names' flags.
      /** If the 'ignore names' flag is set, the names of the GeoNodes are ignored. */
      NVSG_API void setIgnoreNames( bool ignore   //!<  set true to ignore names on comparison
                                  );

      //! Query if the latest traversal did modify the tree.
      /** \returns \c true if the tree was modified, otherwise false */
      NVSG_API bool getTreeModified( void ) const;

    protected:
      //! Protected destructor to prevent instantiation of a TreeOptimizeTraverser on stack.
      NVSG_API virtual ~OptimizeTraverser( void );

      //! doApply override
      NVSG_API virtual void doApply( nvsg::NodeHandle * root );

      NVSG_API void setTreeModified( void );

    private:
      bool  m_ignoreNames;
      bool  m_treeModified;
  };

  inline bool OptimizeTraverser::getIgnoreNames( void ) const
  {
    return( m_ignoreNames );
  }

  inline void OptimizeTraverser::setIgnoreNames( bool ignore )
  {
    m_ignoreNames = ignore;
  }

  inline bool OptimizeTraverser::getTreeModified( void ) const
  {
    return( m_treeModified );
  }

  inline void OptimizeTraverser::setTreeModified( void )
  {
    m_treeModified = true;
  }
}