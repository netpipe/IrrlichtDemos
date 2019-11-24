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
#include "nvsg/Triangles.h" // base class definition
// additional dependencies
#include "nvsg/Animation.h"
#include "nvmath/Trafo.h"
#include "nvmath/Mat44f.h"

namespace nvsg
{
  /*! \brief An internal helper class for SkinnedTriangles
   *  \par Namespace: nvsg
   *  \remarks Instead of the vertices and normals from Triangles, SkinnedTriangles use a special
   *  construction that has a vector of "Bones" per vertex, from which the vertices and normals
   *  are calculated. This calculation depends on the animation step. A Bone essentially consists
   *  of an Animation of type nvmath::Trafo, an offset, and a weight. Out of these values a vertex
   *  is calculated using a vector of Bones.
   *  \sa Animation, SkinnedTriangles */
  class Bone
  {
    public:
      /*! \brief Default constructor
       *  \remarks The default constructor creates an empty Bone. */
      NVSG_API Bone();

      /*! \brief Copy constructor
       *  \param rhs A reference to the constant Bone to copy from.
       *  \remarks The reference count of the animation of \a rhs is incremented. All the other
       *  data is just copied.
       *  \sa Animation */
      NVSG_API Bone( const Bone &rhs );

      /*! \brief Destructor
       *  \remarks The reference count of the animation is decremented.
       *  \sa Animation */
      NVSG_API ~Bone();

      /*! \brief Get the Animation of type nvmath::Trafo.
       *  \return A pointer to the constant Animation of type nvmath::Trafo of this Bone.
       *  \sa Animation, nvmath::Trafo */
      NVSG_API TrafoAnimationHandle * getAnimation() const;

      /*! \brief Calculate the transformation of an animation frame.
       *  \param frame Index of the frame of the animation to get the transformation.
       *  \return A reference to the constant transformation as an nvmath::Trafo.
       *  \note The behavior is undefined if \a frame is larger than or equal to the number of
       *  frames of the Animation of this Bone.
       *  \sa Animation, nvmath::Mat44f, nvmath::Trafo */
      NVSG_API const nvmath::Mat44f & getMatrix( size_t frame ) const;

      /*! \brief Get the weight of this Bone.
       *  \return The weight of this Bone. */
      NVSG_API float getWeight() const;

      /*! \brief Set the Animation of type nvmath::Trafo.
       *  \param animation A pointer to the constant Animation of type nvmath::Trafo
       *  \remarks The reference count of \a animation is increased, and the reference count of
       *  any previously bound Animation of type nvmath::Trafo is decreased.
       *  \sa Animation, nvmath::Trafo */
      NVSG_API void setAnimation( TrafoAnimationHandle * animation );

      /*! \brief Set the base inverse.
       *  \param inverse A reference to the constant base inverse transformation to set.
       *  \remarks All animation steps are performed relative to this base inverse. Usually it is
       *  calculated out of the first animation step.
       *  \sa nvmath::Mat44f */
      NVSG_API void setInverse( const nvmath::Mat44f & inverse );

      /*! \brief Set the transformation to use.
       *  \param trafo A reference to the constant transformation to use
       *  \remarks Explicitly sets the transformation to use and invalidates the animation step.
       *  \sa nvmath::Trafo */
      NVSG_API void setMatrix( const nvmath::Mat44f & trafo );

      /*! \brief Set the weight.
       *  \param weight The weight to use.
       *  \remarks The weight is used for this Bones contribution to the vertex calculation.
       *  of a Skin. */
      NVSG_API void setWeight( float weight );

      /*! \brief Function call operator to calculate a vertex.
       *  \param frame The animation step for the vertex to get.
       *  \param v A reference to the constant reference point.
       *  \note The behavior is undefined if \a frame is larger than or equal to the number of
       *  frames of the Animation of this Bone.
       *  \return The modified point at the given frame. */
      NVSG_API nvmath::Vec4f operator()( size_t frame, const nvmath::Vec4f & v ) const;

      /*! \brief Equality operator
       *  \param b A reference to the constant Bone to compare with.
       *  \return \c true, if \c this and \a b are equal, otherwise \c false.
       *  \remarks Two Bone objects are considered to be equal if they have the same base
       *  inverse, the same weight, and the same Animation of type nvmath::Trafo (not just two
       *  equivalent animations).
       *  \sa Animation, nvmath::Trafo */
      NVSG_API bool operator==( const Bone & b ) const;

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant Bone to copy from.
       *  \return A reference to the assigned Bone.
       *  \remarks The reference count of any previously bound Animation of type nvmath::Trafo is
       *  decremented, all the data from \a rhs is copied, and the reference count of the
       *  Animation of type nvmath::Trafo from \a rhs is incremented.
       *  \sa Animation, nvmath::Trafo */
      NVSG_API Bone & operator=( const Bone &rhs );

    private:
      TrafoAnimationHandle * m_animation;
      nvmath::Mat44f m_baseInverse;
      float m_weight;

      mutable size_t  m_currFrame;
      mutable nvmath::Mat44f  m_currTrafo;
  };

  inline TrafoAnimationHandle * Bone::getAnimation() const
  {
    NVSG_TRACE();
    return( m_animation );
  }

  inline float Bone::getWeight() const
  {
    NVSG_TRACE();
    return( m_weight );
  }

  inline void Bone::setInverse( const nvmath::Mat44f &inverse )
  {
    NVSG_TRACE();
    m_baseInverse = inverse;
  }

  inline void Bone::setMatrix( const nvmath::Mat44f &trafo )
  {
    NVSG_TRACE();
    m_currTrafo = trafo;
    m_currFrame = 0xFFFFFFFF;
  }

  inline void Bone::setWeight( float weight )
  {
    NVSG_TRACE();
    m_weight = weight;
  }

  inline bool Bone::operator==( const Bone &b ) const
  {
    NVSG_TRACE();
    return(   ( m_baseInverse == b.m_baseInverse )
          &&  ( m_animation   == b.m_animation   )
          &&  ( m_weight      == b.m_weight      ) );
  }

  /*! \brief Internal helper class for SkinnedTriangles.
   *  \par Namespace: nvsg
   *  \remarks Instead of the vertices and normals from Triangles, SkinnedTriangles use a special
   *  construction that has a vector of "Bones" per vertex, from which the vertices and normals
   *  are calculated. This calculation depends on the animation step.\n
   *  The vertex and the normal of a Skin are starting values, that are calculated out of the
   *  first animation step of each bone, together with their offset and weight. These values are
   *  used as starting values on the following animation steps.
   *  \sa Bone */
  typedef struct
  {
    std::vector<Bone> m_bones;  //!< vector of "Bones" from which a vertex and a normal is calculated
    nvmath::Vec3f     m_normal; //!< current normal, calculated from the vector of "Bones"
    nvmath::Vec3f     m_vertex; //!< current vertex, calculated from the vector of "Bones"
  } Skin;

  /*! \brief Class representing a skinned Triangles.
   *  \par Namespace: nvsg
   *  \remarks This class has additional information for calculating the vertices and normals of a
   *  Triangles (based on the animation step) from a "Skin" on every vertex.
   *  \sa Bone, Skin, Triangles */
  class SkinnedTriangles : public Triangles
  {
    public:
      /*! \brief Default-constructs a SkinnedTriangles. */
      NVSG_API SkinnedTriangles();

      /*! \brief Constructs a SkinnedTriangles from a Triangles. */
      NVSG_API SkinnedTriangles( const Triangles &rhs );

      /*! \brief Constructs a SkinnedTriangles as a copy from anothe SkinnedTriangles. */
      NVSG_API SkinnedTriangles( const SkinnedTriangles &rhs );

      /*! \brief Destructs a SkinnedTriangles. */
      NVSG_API virtual ~SkinnedTriangles();

      /*! \brief Query if the data of this SkinnedTriangles is shared.
       *  \return \c true if the data is shared, otherwise \c false.
       *  \sa getDataID */
      NVSG_API virtual bool isDataShared() const;

      /*! \brief Get the DataID, which uniquely identifies the sharable object data.
       *  \return The unique DataID, that identifies the object data.
       *  \sa isDataShared */
      NVSG_API virtual DataID getDataID() const;

      /*! \brief Get the number of animation frames that are part of this SkinnedTriangles.
       *  \return Number of frames in the Animation. */
      NVSG_API virtual size_t getNumberOfFrames() const;

      /*! \brief Set the current frame.
       *  \param frame The index of the frame to set as current.
       *  \param calculateTangentSpace if \c true, tangents and binormals are recalculated
       *  \remarks SkinnedTriangles contain animation information on each vertex. When the
       *  animation frame is set, the vertices and normals are recalculated.
       *  \note The behaviour is undefined if \a frame is larger than or equal to the number of
       *  frames of the Animation objects of this SkinnedTriangles.
       *  \sa Animation, Bone, Skin */
      NVSG_API void setAnimationFrame( size_t frame
                                     , bool calculateTangentSpace );

      /*! \brief Get the number of skins.
       *  \return The number of skins.
       *  \remarks In addition to vertices, normals, and so on, a SkinnedTriangles contains
       *  so-called "Skins". A Skin is the information from which one vertex and its normal can be
       *  calculated, depending on the animation frame.
       *  \sa Skin */
      NVSG_API size_t getNumberOfSkins() const;

      /*! \brief Get a pointer to the constant Skin.
       *  \return A pointer to the constant Skin.
       *  \sa Skin */
      NVSG_API const Skin * getSkins() const;

      /*! \brief Set the skins.
       *  \param skins A pointer to the constant skins to use.
       *  \param numSkins The number of skins.
       *  \remarks A Skin contains information for calculating a vertex and a normal for
       *  each animation step.
       *  \note The behavior is undefined if there are less than \a numSkins value of type Skin
       *  located at \a skins.
       *  \sa Bone, Skin */
      NVSG_API void setSkins( const Skin * skins
                            , size_t numSkins );

      /*! \brief Set a subset of skins of this SkinnedTriangles.
       *  \param pos The start position inside the range of pre-existing skins.
       *  \param skins A pointer to the constant skins to use.
       *  \param numSkins The number of skins.
       *  \remarks Copies \a numSkins values of type Skin from \a skins into this
       *  SkinnedTriangles, starting at position \a pos inside the range of pre-existing skins.\n
       *  Pre-existing skins in the range [\a pos, \a pos + \a numSkins) will be replaced.
       *  Pre-existing skins outside this range remain untouched.\n
       *  If you specifiy (-1) for \a pos or \a pos specifies the number of skins currently
       *  stored, the skins pointed to by \a skins will be appended to the pre-existing skins.\n
       *  \note The behavior is undefined if \a pos is neither a valid position inside the range
       *  of pre-existing skins, nor the number of skins currently stored, nor (-1).
       *  \note The behavior is undefined if there are less than \a numSkins values of type
       *  Skin located at \a skins.
       *  \sa hasFaces, getNumberOfFaces, getFaces */
      NVSG_API void setSkins( size_t pos
                            , const Skin * skins
                            , size_t numSkins );

      /*! \brief Assignment operator
       *  \param rhs A reference to the constant SkinnedTriangles to copy from.
       *  \return A reference to the assigned SkinnedTriangles.
       *  \remarks The assignment operator calls the assignment operator of Triangles and copies
       *  the skins. */
      NVSG_API SkinnedTriangles & operator=(const SkinnedTriangles & rhs);

      /*! \brief Test for equivalence with another SkinnedTriangles. 
       *  \param p Pointer to the constant SkinnedTriangles to test for equivalence with.
       *  \param ignoreNames Optional parameter to ignore the names of the objects; default is \c true.
       *  \param deepCompare Optional parameter to perform a deep comparsion; default is \c false.
       *  \return \c true if the SkinnedTriangles \a p is equivalent to \c this, otherwise \c false.
       *  \remarks If \a p and \c this are equivalent as Triangles, they are considered to be
       *  equivalent, if their skins are pair-wise equal.
       *  \note The behavior is undefined if \a p is not a SkinnedTriangles nor derived from one.
       *  \sa Object, Skin, Triangles */
      NVSG_API virtual bool isEquivalent( const Object * p
                                        , bool ignoreNames
                                        , bool deepCompare ) const;

    protected:
      /*! \brief Validate the Incarnation. 
       *  \remarks This function is called from the framework when the Incarnation is requested
       *  and it is marked as invalid. Then it is validated and the Incarnation of each Animation
       *  of each Bone of each Skin is requested to continue validation down the tree hierarchy.
       *  \a Bone, Incarnation, Skin */
      NVSG_API virtual void validateIncarnation() const;  // from top to bottom

    private:
      nvutil::RCPtr<nvutil::RCVector<Skin> >  m_skins; // sharable skins
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - 
  // inlines
  // - - - - - - - - - - - - - - - - - - - - - - - - - 

  inline size_t SkinnedTriangles::getNumberOfSkins()  const
  { 
    NVSG_TRACE();
    return( m_skins->size() ); 
  }

  inline const Skin * SkinnedTriangles::getSkins()  const 
  { 
    NVSG_TRACE();
    return( &(*m_skins)[0] ); 
  }

} //  namespace nvsg
