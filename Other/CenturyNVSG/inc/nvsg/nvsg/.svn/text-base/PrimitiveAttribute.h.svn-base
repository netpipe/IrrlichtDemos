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

#include "nvsg/StateAttribute.h"

namespace nvsg
{
  /*! \brief Base class for all primitive related StateAttribute classes.
   *  \par Namespace: nvsg
   *  \remarks For each primitive type like points, lines, or faces, there is one specific
   *  PrimitiveAttribute, like PointAttribute, LineAttribute, and FaceAttribute. The different
   *  kinds of PrimitiveAttributes do not interfere.
   *  \sa FaceAttribute, LineAttribute, PointAttribute, StateAttribute */
  class PrimitiveAttribute : public StateAttribute
  {
    public:
      /*! \brief Destructs a PrimitiveAttribute. */
      NVSG_API virtual ~PrimitiveAttribute();

    protected:
      /*! \brief Protected default constructor to prevent explicit creation.
       *  \remarks This default constructor is accessible only by derived classes. */
      NVSG_API PrimitiveAttribute();

      /*! \brief Protected copy constructor to prevent explicit creation.
       *  \param rhs A reference to the constant PrimitiveAttribute to copy from
       *  \remarks This copy constructor is accessible only by derived classes. */
      NVSG_API PrimitiveAttribute( const PrimitiveAttribute& rhs );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant PrimitiveAttribute to copy from.
       *  \return A reference to the assigned PrimitiveAttribute.
       *  \remarks This assignment operator is accessible only by derived classes. It calls the
       *  assignment operator of StateAttibute. */
      NVSG_API PrimitiveAttribute & operator=(const PrimitiveAttribute & rhs);
  };
}
