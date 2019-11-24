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
#include "nvsg/StateAttribute.h"


namespace nvsg
{
  /*! \brief Enumeration to identify a blend function
   *  \par Namespace: nvsg
   *  \remarks A BlendFunction indicates how a blending function is to be computed. In the
   *  description of the possible values, source and destination are indicated with the suffix
   *  s and d, respectively. Subtraction of quadruplets means subtracting them component-wise.
   *  \sa BlendAttribute */
  typedef enum
  {
    BF_ZERO                           //!< (0,0,0,0); available for source and destination
  , BF_ONE                            //!< (1,1,1,1); available for source and destination
  , BF_DESTINATION_COLOR              //!< (Rd,Gd,Bd,Ad); available for source only
  , BF_SOURCE_COLOR                   //!< (Rs,Gs,Bs,As); available for destination only
  , BF_ONE_MINUS_DESTINATION_COLOR    //!< (1,1,1,1)-(Rd,Gd,Bd,Ad); available for source only
  , BF_ONE_MINUS_SOURCE_COLOR         //!< (1,1,1,1)-(Rs,Gs,Bs,As); available for destination only
  , BF_SOURCE_ALPHA                   //!< (As,As,As,As); available for source and destination
  , BF_ONE_MINUS_SOURCE_ALPHA         //!< (1,1,1,1)-(As,As,As,As); available for source and destination
  , BF_DESTINATION_ALPHA              //!< (Ad,Ad,Ad,Ad); available for source and destination
  , BF_ONE_MINUS_DESTINATION_ALPHA    //!< (1,1,1,1)-(Ad,Ad,Ad,Ad); available for source and destination
  , BF_SOURCE_ALPHA_SATURATE          //!< (f,f,f,1) with f=min(As,1-Ad); available for source only
  } BlendFunction;

  /*! \brief StateAttribute that determines the blending modes to use.
   *  \par Namespace: nvsg
   *  \remarks A BlendAttribute defines a nvsg::BlendFunction for both the source and the destination
   *  fragment. Blending can be completely controlled through use of a BlendAttribute.
   *  \note A BlendAttribute takes precedence over both the blending of transparent geometries
   *  as well as the blending of antialiased lines and points.
   *  \sa BlendFunction, LineAttribute, PointAttribute, StateAttribute,  */
  class BlendAttribute : public StateAttribute
  {
    public:
      /*! \brief Default-constructs a BlendAttribute. 
       *  \remarks A default-constructed BlendAttribute is initialized with BF_ONE as source 
       *  function and BF_ZERO as destination function. */
      NVSG_API BlendAttribute();

      /*! \brief Constructs a BlendAttribute from a StateAttribute. 
       *  \param rhs StateAttribute to construct the BlendAttribute from.
       *  \remarks A BlendAttribute constructed from a StateAttribute is initialized with BF_ONE 
       *  as source function and BF_ZERO as destination function. */
      NVSG_API explicit BlendAttribute( const StateAttribute &rhs );

      /*! \brief Constructs a BlendAttribute as a copy from another BlendAttribute.
       *  \param rhs Source BlendAttribute. */ 
      NVSG_API BlendAttribute( const BlendAttribute &rhs );

      /*! \brief Destructs a BlendAttribute. */
      NVSG_API virtual ~BlendAttribute();

      /*! \brief Set the blending functions for source and destination.
       *  \param sourceFunction       Blending function for the source
       *  \param destinationFunction  Blending function for the destination
       *  \remarks This function controls how color values in the fragment being processed (the source) are
       *  combined with those already stored in the framebuffer (the destination). The argument \a
       *  sourceFunction indicates how to compute a source blending function; the argument \a
       *  destinationFunction indicates how to compute a destination blending function.
       *  \sa BlendFunction */
      NVSG_API void setBlendFunctions( BlendFunction sourceFunction
                                     , BlendFunction destinationFunction
                                     );

      /*! \brief Get the source blending function.
       *  \return nvsg::BlendFunction indicating the source blending function.
       *  \sa getDestinationFunction, setBlendFunctions */
      NVSG_API BlendFunction getSourceFunction() const;

      /*! \brief Get the destination blending function.
       *  \return nvsg::BlendFunction indicating the destination blending function.
       *  \sa getSourceFunction, setBlendFunctions */
      NVSG_API BlendFunction getDestinationFunction() const;

      /*! \brief Query the transparency status of this BlendAttribute.
       *  \return \c true, if the BlendAttribute is transparent, otherwise \c false.
       *  \remarks A BlendAttribute is considered to be transparent if the destination function
       *  is not BF_ZERO or if the source function is one of the following: BF_DESTINATION_COLOR,
       *  BF_ONE_MINUS_DESTINATION_COLOR, BF_DESTINATION_ALPHA, BF_ONE_MINUS_DESTINATION_ALPHA,
       *  or BF_SOURCE_ALPHA_SATURATE.
       *  \sa StateAttribute::isTransparent */
      NVSG_API virtual bool isTransparent() const;

      /*! \brief Test for equivalence with another BlendAttribute.
       *  \param p            Pointer to the constant BlendAttribute to test for equivalence with.
       *  \param ignoreNames  Optional parameter to ignore the names of the objects; default is \c true
       *  \param deepCompare  Optional parameter to perform a deep comparsion; default is \c false
       *  \return \c true if the BlendAttribute \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as StateAttribute, they are considered to be
       *  equivalent if they have the same nvsg::BlendFunction for the source and the destination.
       *  \note The behaviour is undefined if \a p is not a BlendAttribute nor derived from one.
       *  \sa StateAttribute, getDestinationFunction, getSourceFunction */
      NVSG_API virtual bool isEquivalent( const Object *p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

      /*! \brief Assignment operator
      *  \param rhs A reference to the constant BlendAttribute to copy from
      *  \return A reference to the assigned BlendAttribute
      *  \remarks The assignment operator calls the assignment operator of StateAttribute and
      *  copies the BlendAttribute specific data then.
      *  \sa PrimitiveAttribute */
      NVSG_API BlendAttribute & operator=(const BlendAttribute & rhs);

    private:
      BlendFunction  m_sourceFunction;
      BlendFunction  m_destinationFunction;
  };

  inline BlendFunction BlendAttribute::getSourceFunction() const
  {
    return( m_sourceFunction );
  }

  inline BlendFunction BlendAttribute::getDestinationFunction() const
  {
    return( m_destinationFunction );
  }

} // namespace nvsg
