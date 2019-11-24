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
/** @file */

#include "nvsgcommon.h"
#include "nvmath/Vec3f.h"
#include "nvsg/OwnedObject.h"
#include "nvutil/BitMask.h"


namespace nvsg
{
  //  forward declarations
  class VertexAttributeSet;

  // VertexAttribute serves as a private helper class providing data management (so no need for docs)
# if ! defined( DOXYGEN_IGNORE )
  class VertexAttribute
  {
    public:
      // default constructs a VertexAttribute object
      VertexAttribute();
      // cleanup on destruction
      ~VertexAttribute();

      // Copies float data pointed to by fdata to the vertex attribute's internal data storage.
      // fdim specifies the dimension, in terms of float, of a single data element.
      // count specifies the number of data elements to be copied, so that always
      // count * fdim floats are copied to the internal data storage.
      // Behavior is undefined if fdata is an invalid pointer.
      void setData(unsigned int fdim, const float * fdata, size_t count);

      // Overrides the data element at position pos with the data element pointed to by fdata.
      // Behavior is undefined if either the specified position is invalid, or if fdata is an invalid pointer.
      // Behavior also is undefined if fdim doesn't match the dimension of the data element currently stored
      // at the specified position.
      void setData(unsigned int fdim, size_t pos, const float * fdata, size_t count);

      // Clear the data
      void clearData( void );

      // Returns a pointer to the float data stored in this vertex attrib, 
      // or a NULL pointer if no data is currently strored
      const float * getData() const;

      // Returns the dimension, in terms of float, of the stored data.
      // If no data is stored in this vertex attrib, the return value might be random. 
      unsigned int getDataDimension() const;

      // Returns the number of data elements currently stored in this vertex attribute
      size_t getNumberOfData() const;

    private:
      unsigned int        m_fdim;   // dimension of data elements in terms of float
      std::vector<float>  m_fdata;  // float data storage (contiguous memory)
      size_t              m_count;  // number of data elements (==m_fdata.size()/m_fdim)
                                    // serves as a cache to improve performance when querying the current data count
  };

  inline void VertexAttribute::clearData( void )
  {
    m_fdim = 0;
    m_count = 0;
    m_fdata.clear();
  }

  inline const float * VertexAttribute::getData() const
  {
    NVSG_TRACE();
    return !m_fdata.empty() ? &m_fdata[0] : NULL; 
  }

  inline unsigned int VertexAttribute::getDataDimension() const
  {
    NVSG_TRACE();
    NVSG_ASSERT(m_fdata.empty() || !(m_fdata.size()%m_fdim)); // this would be a serious error! 
    NVSG_ASSERT(m_fdata.empty() == !m_fdim);
    return m_fdim;
  }

  inline size_t VertexAttribute::getNumberOfData() const
  {
    NVSG_TRACE();
    NVSG_ASSERT(m_fdata.empty() == !m_fdim);
    NVSG_ASSERT(m_fdata.empty() || !(m_fdata.size()%m_fdim)); // this would be a serious error
    NVSG_ASSERT(m_fdata.empty() || m_count==m_fdata.size()/m_fdim); // count cache is not up to data
    // --> serious, as well
    return m_count;
  }
#endif    //  DOXYGEN_IGNORE

  //! Class to hold one set of vertex attributes.
  /** This class only holds the vertex and attribute data. Any information about how they are connected has to be
    * provided by topology specific classes (such as VertexAttributeSet, etc ...). */
  class VertexAttributeSet : public OwnedObject<PrimitiveSetHandle>
  {
    friend class PrimitiveSet;

    public:
      //! Vertex Attribute Register Numbers
      /** These represent valid indices identifying a certain vertex attribute. */
      enum 
      {
        ATTR0  = 0,  VERTEX_POSITION  = ATTR0,
        ATTR1  = 1,  VERTEX_WEIGHTS   = ATTR1,
        ATTR2  = 2,  NORMAL           = ATTR2,
        ATTR3  = 3,  PRIMARY_COLOR    = ATTR3,
        ATTR4  = 4,  SECONDARY_COLOR  = ATTR4,
        ATTR5  = 5,  FOG_COORDINATE   = ATTR5,
        ATTR6  = 6,  UNUSED_1         = ATTR6,
        ATTR7  = 7,  UNUSED_2         = ATTR7,
        ATTR8  = 8,  TEXTURE_COORD_0  = ATTR8,
        ATTR9  = 9,  TEXTURE_COORD_1  = ATTR9,
        ATTR10 = 10, TEXTURE_COORD_2  = ATTR10,
        ATTR11 = 11, TEXTURE_COORD_3  = ATTR11,
        ATTR12 = 12, TEXTURE_COORD_4  = ATTR12,
        ATTR13 = 13, TEXTURE_COORD_5  = ATTR13,
        ATTR14 = 14, TEXTURE_COORD_6  = ATTR14,
        ATTR15 = 15, TEXTURE_COORD_7  = ATTR15,
        NUMBER_OF_VERTEX_ATTRIBUTES
     };

      //! Format bits to reflect the interleaved format
#define ATTRBITS(i) \
  static const nvutil::bitset64 ATTR##i##_1F   = BIT64(i*4); \
  static const nvutil::bitset64 ATTR##i##_2F   = BIT64(i*4+1); \
  static const nvutil::bitset64 ATTR##i##_3F   = BIT64(i*4+2); \
  static const nvutil::bitset64 ATTR##i##_4F   = BIT64(i*4+3); \
  static const nvutil::bitset64 ATTR##i##_MASK = ATTR##i##_1F | ATTR##i##_2F | ATTR##i##_3F | ATTR##i##_4F; 

      ATTRBITS(0);      
      ATTRBITS(1);      
      ATTRBITS(2);      
      ATTRBITS(3);      
      ATTRBITS(4);      
      ATTRBITS(5);      
      ATTRBITS(6);      
      ATTRBITS(7);      
      ATTRBITS(8);      
      ATTRBITS(9);      
      ATTRBITS(10);      
      ATTRBITS(11);      
      ATTRBITS(12);      
      ATTRBITS(13);      
      ATTRBITS(14);      
      ATTRBITS(15);      
#undef ATTRBITS
      
      static const nvutil::bitset64 V3F_N3F =               ATTR0_3F | ATTR2_3F;      
      static const nvutil::bitset64 V3F_N3F_VC3F =          ATTR0_3F | ATTR2_3F | ATTR3_3F;
      static const nvutil::bitset64 V3F_N3F_VC4F =          ATTR0_3F | ATTR2_3F | ATTR3_4F;
      static const nvutil::bitset64 V3F_N3F_VC3F_T2F =      ATTR0_3F | ATTR2_3F | ATTR3_3F | ATTR8_2F;
      static const nvutil::bitset64 V3F_N3F_VC4F_T2F =      ATTR0_3F | ATTR2_3F | ATTR3_4F | ATTR8_2F;
      static const nvutil::bitset64 V3F_N3F_T2F =           ATTR0_3F | ATTR2_3F | ATTR8_2F;
      static const nvutil::bitset64 V3F_N3F_T2F_TG3F_BN3F = ATTR0_3F | ATTR2_3F | ATTR8_2F | ATTR9_3F | ATTR10_3F;

      //! Default-constructs an empty VertexAttributeSet.
      NVSG_API VertexAttributeSet(void);

      //! Copy Constructor.
      NVSG_API VertexAttributeSet( const VertexAttributeSet &rhs );

      //! Destructs a VertexAttributeSet.
      NVSG_API virtual ~VertexAttributeSet(void);

      //! Returns whether the data of this object is shared.
      /** \returns \c true if the object data is shared, \c false otherwise. */
      NVSG_API virtual bool isDataShared( void ) const;

      //! Returns a 64-bit integer value, which uniquely identifies the object data. 
      /** \returns A DataID, that is - a 64-bit integer value, which uniquely identifies the Object data. */
      NVSG_API virtual DataID getDataID( void ) const;

      //! Returns the interleave format
      /** \return Format bits - see V3F, VW1F, N3F, etc */
      NVSG_API const nvutil::bitset64& getInterleavedFormat() const;

      //! Get amount of data bytes currently stored in the vertex attribute specified by \a attrib.
      /** \returns The number of bytes used. 
        * \note The behavior is undefined when passing an invalid index for \a attrib. 
        * Valid indices are:\n
        * VertexAttributeSet::VERTEX_POSITION\n
        * VertexAttributeSet::VERTEX_WEIGHTS\n
        * VertexAttributeSet::NORMAL\n
        * VertexAttributeSet::PRIMARY_COLOR\n
        * VertexAttributeSet::SECONDARY_COLOR\n
        * VertexAttributeSet::FOG_COORDINATE\n
        * VertexAttributeSet::UNUSED_1\n
        * VertexAttributeSet::UNUSED_2\n
        * VertexAttributeSet::TEXTURE_COORD_0\n
        * VertexAttributeSet::TEXTURE_COORD_1\n
        * VertexAttributeSet::TEXTURE_COORD_2\n
        * VertexAttributeSet::TEXTURE_COORD_3\n
        * VertexAttributeSet::TEXTURE_COORD_4\n
        * VertexAttributeSet::TEXTURE_COORD_5\n
        * VertexAttributeSet::TEXTURE_COORD_6\n
        * VertexAttributeSet::TEXTURE_COORD_7\n
        * The indices listed here are a contiguous set of indices and range from 0 to 15. */
      NVSG_API size_t getNumberOfVertexAttributeBytes( size_t attrib    //!< Zero based index identifying the target vertex attribute. 
                                                     ) const;

      //! Get float data stored in the vertex attribute specified by \a attrib.  
      /** \returns A valid pointer to the vertex attribute's data if data was stored in the
        * vertex attribute, a NULL pointer otherwise.
        * \note The behavior is undefined when passing an invalid index for \a attrib. 
        * Valid indices are:\n
        * VertexAttributeSet::VERTEX_POSITION\n
        * VertexAttributeSet::VERTEX_WEIGHTS\n
        * VertexAttributeSet::NORMAL\n
        * VertexAttributeSet::PRIMARY_COLOR\n
        * VertexAttributeSet::SECONDARY_COLOR\n
        * VertexAttributeSet::FOG_COORDINATE\n
        * VertexAttributeSet::UNUSED_1\n
        * VertexAttributeSet::UNUSED_2\n
        * VertexAttributeSet::TEXTURE_COORD_0\n
        * VertexAttributeSet::TEXTURE_COORD_1\n
        * VertexAttributeSet::TEXTURE_COORD_2\n
        * VertexAttributeSet::TEXTURE_COORD_3\n
        * VertexAttributeSet::TEXTURE_COORD_4\n
        * VertexAttributeSet::TEXTURE_COORD_5\n
        * VertexAttributeSet::TEXTURE_COORD_6\n
        * VertexAttributeSet::TEXTURE_COORD_7\n
        * The indices listed here are a contiguous set of indices and range from 0 to 15. */
      NVSG_API const float * getVertexAttributeData( size_t attrib    //!< Zero based index identifying the target vertex attribute.
                                                   ) const;

      //! Determine the dimension, in terms of float, of the data stored in the vertex attribute specified by \a attrib.
      /** The native data format for data stored in a vertex attribute is float, thus the reason for expressing
        * a data element's dimension in terms of float. A single vertex position, for example, is of dimension 3, which is
        * one single-precision float value for representing a direction in a 3-dimensional space.
        * \returns The dimension, in terms of float, of the data elements stored in the specified vertex attribute.
        * If there is no data currently stored in that vertex attribute, the function returns 0.
        * \note The behavior is undefined when passing an invalid index for \a attrib. 
        * Valid indices are:\n
        * VertexAttributeSet::VERTEX_POSITION\n
        * VertexAttributeSet::VERTEX_WEIGHTS\n
        * VertexAttributeSet::NORMAL\n
        * VertexAttributeSet::PRIMARY_COLOR\n
        * VertexAttributeSet::SECONDARY_COLOR\n
        * VertexAttributeSet::FOG_COORDINATE\n
        * VertexAttributeSet::UNUSED_1\n
        * VertexAttributeSet::UNUSED_2\n
        * VertexAttributeSet::TEXTURE_COORD_0\n
        * VertexAttributeSet::TEXTURE_COORD_1\n
        * VertexAttributeSet::TEXTURE_COORD_2\n
        * VertexAttributeSet::TEXTURE_COORD_3\n
        * VertexAttributeSet::TEXTURE_COORD_4\n
        * VertexAttributeSet::TEXTURE_COORD_5\n
        * VertexAttributeSet::TEXTURE_COORD_6\n
        * VertexAttributeSet::TEXTURE_COORD_7\n
        * The indices listed here are a contiguous set of indices and range from 0 to 15. */
      NVSG_API unsigned int getVertexAttributeDataDim( size_t attrib    //!< Identifies the target vertex attribute.
                                                     ) const;

      //! Determine if data is available for a vertex attribute specified by \a attrib.
      /** \returns The number of stored data elements.
        * \note The behavior is undefined when passing an invalid index for \a attrib. 
        * Valid indices are:\n
        * VertexAttributeSet::VERTEX_POSITION\n
        * VertexAttributeSet::VERTEX_WEIGHTS\n
        * VertexAttributeSet::NORMAL\n
        * VertexAttributeSet::PRIMARY_COLOR\n
        * VertexAttributeSet::SECONDARY_COLOR\n
        * VertexAttributeSet::FOG_COORDINATE\n
        * VertexAttributeSet::UNUSED_1\n
        * VertexAttributeSet::UNUSED_2\n
        * VertexAttributeSet::TEXTURE_COORD_0\n
        * VertexAttributeSet::TEXTURE_COORD_1\n
        * VertexAttributeSet::TEXTURE_COORD_2\n
        * VertexAttributeSet::TEXTURE_COORD_3\n
        * VertexAttributeSet::TEXTURE_COORD_4\n
        * VertexAttributeSet::TEXTURE_COORD_5\n
        * VertexAttributeSet::TEXTURE_COORD_6\n
        * VertexAttributeSet::TEXTURE_COORD_7\n
        * The indices listed here are a contiguous set of indices and range from 0 to 15. */
      NVSG_API size_t getNumberOfVertexAttributeData( size_t attrib   //!< Identifies the target vertex attribute.
                                                    ) const;

      //! Set data for the vertex attribute specified by \a attrib.
      /** The function copies \a fdim * \a count float values from the location pointed to by \a fdata
        * to the vertex attribute specified by \a attrib. Pre-existing data of the specified vertex
        * attribute will be entirely erased.
        * \note The behavior is undefined when passing an invalid index for \a attrib. 
        * Valid indices are:\n
        * VertexAttributeSet::VERTEX_POSITION\n
        * VertexAttributeSet::VERTEX_WEIGHTS\n
        * VertexAttributeSet::NORMAL\n
        * VertexAttributeSet::PRIMARY_COLOR\n
        * VertexAttributeSet::SECONDARY_COLOR\n
        * VertexAttributeSet::FOG_COORDINATE\n
        * VertexAttributeSet::UNUSED_1\n
        * VertexAttributeSet::UNUSED_2\n
        * VertexAttributeSet::TEXTURE_COORD_0\n
        * VertexAttributeSet::TEXTURE_COORD_1\n
        * VertexAttributeSet::TEXTURE_COORD_2\n
        * VertexAttributeSet::TEXTURE_COORD_3\n
        * VertexAttributeSet::TEXTURE_COORD_4\n
        * VertexAttributeSet::TEXTURE_COORD_5\n
        * VertexAttributeSet::TEXTURE_COORD_6\n
        * VertexAttributeSet::TEXTURE_COORD_7\n
        * The indices listed here are a contiguous set of indices and range from 0 to 15.\n
        * The behavior is also undefined if either \a fdim doesn't match the correct dimension, in
        * terms of float, of the data pointed to by \a fdata, or if \a fdata is an invalid pointer,
        * or if count exceeds the actual number of source data elements. */
      NVSG_API void setVertexAttributeData( size_t          attrib  //!< Identifies the target vertex attribute.
                                          , unsigned int    fdim    //!< Dimension, in terms of float, of the source data elements.
                                          , const float   * fdata   //!< Address of source data.
                                          , size_t          count   //!< Number of source data elements.
                                          );

      //! Set data for the vertex attribute specified by \a attrib.
      /** The function copies \a fdim * \a count float values from the location pointed to by \a fdata
        * to the vertex attribute specified by \a attrib, starting at position \a pos of the pre-existing
        * vertex data. Pre-existing vertex data in the range [\a pos, \a pos + \a count) will be replaced.
        * Pre-existing data outside this range remains untouched.
        *
        * If you specify -1 for \a pos or if \a pos specifies the number of elements currently stored for
        * the vertex attribute specified by \a attrib, the data pointed to by \a fdata will be appended to
        * the pre-existing vertex data. 
        *
        * If you not specify -1 for \a pos or \a pos neither specifies the number of elements currently
        * stored for the specified vertex attribute nor does it specify a valid position inside the range
        * of pre-existing data, the behaviour is undefined!
        * 
        * \note The behavior is undefined when passing an invalid index for \a attrib. 
        * Valid indices are:\n
        * VertexAttributeSet::VERTEX_POSITION\n
        * VertexAttributeSet::VERTEX_WEIGHTS\n
        * VertexAttributeSet::NORMAL\n
        * VertexAttributeSet::PRIMARY_COLOR\n
        * VertexAttributeSet::SECONDARY_COLOR\n
        * VertexAttributeSet::FOG_COORDINATE\n
        * VertexAttributeSet::UNUSED_1\n
        * VertexAttributeSet::UNUSED_2\n
        * VertexAttributeSet::TEXTURE_COORD_0\n
        * VertexAttributeSet::TEXTURE_COORD_1\n
        * VertexAttributeSet::TEXTURE_COORD_2\n
        * VertexAttributeSet::TEXTURE_COORD_3\n
        * VertexAttributeSet::TEXTURE_COORD_4\n
        * VertexAttributeSet::TEXTURE_COORD_5\n
        * VertexAttributeSet::TEXTURE_COORD_6\n
        * VertexAttributeSet::TEXTURE_COORD_7\n
        * The indices listed here are a contiguous set of indices and range from 0 to 15.\n
        * The behavior is also undefined if either \a fdim doesn't match the correct dimension, in
        * terms of float, of the data element currently stored for the specified vertex attribute, or
        * if \a fdata is an invalid pointer. */
      NVSG_API void setVertexAttributeData( size_t          attrib  //!< Identifies the target vertex attribute.
                                          , unsigned int    fdim    //!< Dimension, in terms of float, of the source data element.
                                          , size_t          pos     //!< Position in contigious memory of the vertex attribute's data set.
                                          , const float   * fdata   //!< Address of source data.
                                          , size_t          count   //!< Number of source data elements.
                                          );

      //! Remove any data for the specified \a attrib.
      NVSG_API void clearVertexAttributeData( size_t attrib   //!< Identifies the target vertex attribute
                                            );

      //! Get number of vertices.
      /** \returns The number of vertices. 
        * \note Calling this function has the same effect as calling getNumberOfVertexAttributeData(VERTEX_POSITION).*/
      NVSG_API size_t getNumberOfVertices( void ) const;

      //! Get vertices.
      /** \returns A pointer to the vertices. 
        * \note Calling this function has the same effect as calling getVertexAttributeData(VERTEX_POSITION).
        * The only difference is, that this function returns a const Vec3f* rather than a const float* to reflect a
        * vertex position's dimension. One must keep this in mind when it comes to pointer arithmetics! */
      NVSG_API const nvmath::Vec3f * getVertices( void ) const;

      //! Set vertices for this VertexAttributeSet.
      /** Copies \a count vertices from the location pointed to by \a verts into this VertexAttributeSet.
        * Pre-existing vertices will be entirely erased.
        * \note The behaviour is undefined if either \a verts is invalid, or if \a count exceeds the actual 
        * number of vertices stored at \a verts. */
      NVSG_API void setVertices( const nvmath::Vec3f  * verts //!<  vertices to set
                               , size_t                 count //!<  number of vertices
                               );

      //! Set vertices for this VertexAttributeSet.
      /** Copies \a count vertices from the location pointed to by \a verts into this VertexAttributeSet, 
        * starting at position \a pos inside the range of pre-existing vertices. 
        * Pre-existing vertices in the range [\a pos, \a pos + \a count) will be replaced.
        * Pre-existing vertices outside this range remain untouched.
        *
        * If you specify -1 for \a pos or if \a pos specifies the number of vertices currently stored
        * the vertices pointed to by \a verts will be appended to the pre-existing vertices. 
        *
        * If you not specify -1 for \a pos or \a pos neither specifies the number of vertices currently
        * stored nor does it specify a valid position inside the range of pre-existing vertices, 
        * the behaviour is undefined!
        *
        * \note The behaviour is undefined if either \a verts is invalid, or if \a count exceeds 
        * the actual number of vertices stored at \a verts. */
      NVSG_API void setVertices( size_t                 pos   //!<  Start position inside the range of pre-existing vertices.
                               , const nvmath::Vec3f  * verts //!<  Start address of vertices to set.
                               , size_t                 count //!<  Number of vertices to copy.
                               );

      //! Get number of normals.
      /** \returns The number of normals. 
        * \note Calling this function has the same effect as calling getNumberOfVertexAttributeData(NORMAL).*/
      NVSG_API size_t getNumberOfNormals( void ) const;

      //! Get normals.
      /** \returns A pointer to the normals. 
        * \note Calling this function has the same effect as calling getVertexAttributeData(NORMAL).
        * The only difference is, that this function returns a const Vec3f* rather than a const float* to reflect a
        * normal's dimension. One must keep this in mind when it comes to pointer arithmetics! */
      NVSG_API const nvmath::Vec3f * getNormals( void ) const;

      //! Set normals for this VertexAttributeSet.
      /** Copies \a count normals from the location pointed to by \a nor into this VertexAttributeSet. 
        * Pre-existing normals will be entirely erased.
        * \note The behaviour is undefined if either \a nor is invalid, or if \a count exceeds 
        * the actual number of normals stored at \a nor. */
      NVSG_API void setNormals( const nvmath::Vec3f * nor   //!< Address of source normals.
                              , size_t                count //!< Number of normals to set.
                              );

      //! Set normals for this VertexAttributeSet.
      /** Copies \a count normals from the location pointed to by \a nor into this VertexAttributeSet, 
        * starting at position \a pos inside the range of pre-existing normals. 
        * Pre-existing normals in the range [\a pos, \a pos + \a count) will be replaced.
        * Pre-existing normals outside this range remain untouched.
        * If you specify -1 for \a pos or if \a pos specifies the number of normals currently stored,
        * the normals pointed to by \a nor will be appended to the pre-existing normals. 
        * If you not specify -1 for \a pos or \a pos neither specifies the number of normals currently
        * stored nor does it specify a valid position inside the range of pre-existing normals, 
        * the behaviour is undefined!
        * \note The behaviour is undefined if either \a nor is invalid, or if \a count exceeds 
        * the actual number of normals stored at \a nor. */
      NVSG_API void setNormals( size_t                pos   //!<  Start position inside the range of pre-existing normals.
                              , const nvmath::Vec3f * nor   //!<  Start address of normals to set.
                              , size_t                count //!<  Number of normals to copy.
                              );

      //! Determine if at least one texture coordinate array has valid texture coordinates.
      /** \returns  true, if this VertexAttributeSet has texture coordinates, false otherwise. */
      NVSG_API bool hasTexCoords( void ) const;

      //! Get number of texture coordinates in a specified texture unit.
      /** \returns  The number of texture coordinates in texture unit \a texUnit. 
        * \note Calling the function has the same effect as calling 
        * getNumberOfVertexAttributeData(TEXTURE_COORD_0 + \a texUnit). 
        * \note The behavior is undefined if \a texUnit specifies an invalid index. A valid index ranges from 0 to 7.*/
      NVSG_API size_t getNumberOfTexCoords( size_t texUnit    //!< The zero based index of the texture unit.
                                          ) const;

      //! Get texture coordinates for a specified texture unit.
      /** \returns A pointer to the texture coords.
        * \note Texture coordinates can be either 1, 2, 3, or 4-dimensional in terms of float. 
        * Call either getTexCoordDim(\a texUnit) or getVertexAttributeDataDim(TEXTURE_COORD_0 + \a texUnit) 
        * to determine the dimension of the texture coordinates stored in texture unit \a texUnit.
        * \note The behavior is undefined if \a texUnit specifies an invalid index. A valid index ranges from 0 to 7.*/
      NVSG_API const float * getTexCoords( size_t texUnit   //!< The zero based index of the texture unit. 
                                         ) const;

      //! Get the dimension of the texture coordinates stored in the specified texture unit.
      /** The dimension of texture coordinates is meant in terms of floats. Texture coordinates
        * can be either 1, 2, 3, or 4-dimensional.
        * \returns  The dimension of the texture coordinates stored in texture unit \a texUnit. If there are no
        * texture coordinates stored in that texture unit, the function returns 0.
        * \note The behavior is undefined if \a texUnit specifies an invalid index. A valid index ranges from 0 to 7.*/
      NVSG_API unsigned int getTexCoordsDim( size_t texUnit   //!< The zero based index of the texture unit. 
                                           ) const;

      //! Clear the texture coordinates for a specified texture unit.
      NVSG_API void clearTexCoords( size_t          texUnit //!< The zero based index of the texture unit. 
                                );

      //! Set the texture coordinates for a specified texture unit.
      /** Copies \a count texture coordinates of dimension \a dim from the location pointed to by \a coords 
        * into the texture unit \a texUnit of this VertexAttributeSet.\n
        * Valid values for \a fdim are either 1, 2, 3, or 4. A valid index for \a texUnit ranges from 0 to 7.
        * \note The behavior is undefined if either \a texUnit specifies an invalid index, or \a fdim doesn't match 
        * the correct dimension, in terms of float, of the data pointed to by \a coords, or \a coords is an invalid
        * pointer, or count exceeds the actual number of source coordinates. */
      NVSG_API void setTexCoords( size_t          texUnit //!< The zero based index of the texture unit. 
                                , unsigned int    fdim    //!< Dimension of coordinates, in terms of float
                                , const float   * coords  //!< Address of source coordinates
                                , size_t          count   //!< Number of coordinates to copy
                                );

      //! Set the texture coordinates for a specified texture unit.
      /** Copies \a count texture coordinates of dimension \a dim from the location pointed to by \a coords 
        * into the texture unit \a texUnit of this VertexAttributeSet, starting at position \a pos inside the range of
        * pre-existing texture coordinates. Valid values for \a fdim are either 1, 2, 3, or 4. 
        * A valid index for \a texUnit ranges from 0 to 7.
        * Pre-existing coordinates in the range [\a pos, \a pos + \a count) will be replaced.
        * Pre-existing coordinates outside this range remain untouched.
        * If you specify -1 for \a pos or if \a pos specifies the number of coordinates currently stored
        * the coordinates pointed to by \a coords will be appended to the pre-existing coordinates. 
        * If you not specify -1 for \a pos or \a pos neither specifies the number of coordinates currently
        * stored nor does it specify a valid position inside the range of pre-existing coordinates, 
        * the behaviour is undefined!
        * \note The behavior is undefined if either \a texUnit specifies an invalid index, or \a fdim doesn't match 
        * the correct dimension, in terms of float, of the data pointed to by \a coords, or \a coords is an invalid
        * pointer, or count exceeds the actual number of source coordinates. */
      NVSG_API void setTexCoords( size_t          texUnit //!< The zero based index of the texture unit. 
                                , unsigned int    fdim    //!< Dimension of coordinates, in terms of float
                                , size_t          pos     //!< Start position inside the range of pre-existing texture coordinates.
                                , const float   * coords  //!< Address of source coordinates
                                , size_t          count   //!< Number of coordinates to copy
                                );

      //! Get number of colors.
      /** \returns  The number of colors. */
      NVSG_API size_t getNumberOfColors( void ) const;

      //! Get constant pointer to colors.
      /** \note It is a mistake to call \c getColors on a \c VertexAttributeSet that has no colors available.
        * For this reason, the client code should either call \c hasColors or \c getNumberOfColors 
        * on that \c VertexAttributeSet to ensure that colors are available for that \c VertexAttributeSet. 
        * \returns  A constant pointer to The colors. */
      NVSG_API const float * getColors( void ) const;

      //! Get the dimension of the colors.
      /** The dimension of colors is conveyed in terms of floats. With this in mind, colors can
        * be either 3-dimensional - occupying 3 floating point values per color, or 4-dimensional -  
        * occupying 4 floating point values per color.
        * \returns The dimension of the colors stored. If there are no colors stored, the function return 0. */
      NVSG_API unsigned int getColorsDim( void ) const;

      //! Set colors for this VertexAttributeSet.
      /** Copies \a count colors of dimension \a fdim from the location pointed to by \a cols to this VertexAttributeSet.\n 
        * Valid values for \a fdim are 3 and 4.
        * \note The behavior is undefined if either \a fdim doesn't match the correct dimension, in terms of float, 
        * of the colors pointed to by \a cols, or \a cols is an invalid pointer, or count exceeds the actual number 
        * of source colors. */
      NVSG_API void setColors( unsigned int   fdim  //!< Dimension of colors, in terms of float
                             , const float  * cols  //!< Address of source colors
                             , size_t         count //!< Number of colors to copy
                             );

      //! Set colors for this VertexAttributeSet.
      /** Copies \a count colors of dimension \a fdim from the location pointed to by \a cols to this VertexAttributeSet,
        * starting at position \a pos inside the range of pre-existing colors.
        * Valid values for \a fdim are 3 and 4.
        * Pre-existing colors in the range [\a pos, \a pos + \a count) will be replaced.
        * Pre-existing colors outside this range remain untouched.
        * If you specify -1 for \a pos or if \a pos specifies the number of colors currently stored
        * the colors pointed to by \a cols will be appended to the pre-existing colors. 
        * If you not specify -1 for \a pos or \a pos neither specifies the number of colors currently
        * stored nor does it specify a valid position inside the range of pre-existing colors, 
        * the behaviour is undefined!
        * \note The behavior is undefined if either \a fdim doesn't match the correct dimension, in terms of float, 
        * of the colors pointed to by \a cols, or \a cols is an invalid pointer, or count exceeds the actual number 
        * of source colors. */
      NVSG_API void setColors( unsigned int   fdim  //!< Dimension of colors, in terms of float
                             , size_t         pos   //!< Start position inside the range of pre-existing colors.
                             , const float  * cols  //!< Address of source colors
                             , size_t         count //!< Number of colors to copy
                             );

      //! Get number of secondary colors.
      /** \returns  The number of secondary colors. */
      NVSG_API size_t getNumberOfSecondaryColors( void ) const;

      //! Get constant pointer to secondary colors.
      /** \returns A pointer to the secondary colors. */
      NVSG_API const float * getSecondaryColors( void ) const;

      //! Get the dimension of the secondary colors.
      /** \returns The dimension of the secondary colors stored. If there are no 
      * secondary colors stored, the function returns 0. */
      NVSG_API unsigned int getSecondaryColorsDim( void ) const;

      //! Set secondary colors for this VertexAttributeSet.
      /** Copies \a count colors of dimension \a fdim from the location pointed to by \a cols to this VertexAttributeSet.\n 
        * Valid values for \a fdim are 3 and 4.
        * \note The behavior is undefined if either \a fdim doesn't match the correct dimension, in terms of float, 
        * of the colors pointed to by \a cols, or if \a cols is an invalid pointer, or if count exceeds the actual number 
        * of source colors. */
      NVSG_API void setSecondaryColors( unsigned int    fdim  //!< Dimension of colors, in terms of float.
                                      , const float   * cols  //!< Address of source colors.
                                      , size_t          count //!< Number of colors to copy.
                                      );

      //! Set secondary colors for this VertexAttributeSet.
      /** Copies \a count colors of dimension \a fdim from the location pointed to by \a cols to this VertexAttributeSet,
        * starting at position \a pos inside the range of pre-existing colors.
        * Valid values for \a fdim are 3 and 4.
        * Pre-existing colors in the range [\a pos, \a pos + \a count) will be replaced.
        * Pre-existing colors outside this range remain untouched.
        * If you specify -1 for \a pos or if \a pos specifies the number of colors currently stored
        * the colors pointed to by \a cols will be appended to the pre-existing colors. 
        * If you not specify -1 for \a pos or \a pos neither specifies the number of colors currently
        * stored nor does it specify a valid position inside the range of pre-existing colors, 
        * the behaviour is undefined!
        * \note The behavior is undefined if either \a fdim doesn't match the correct dimension, in terms of float, 
        * of the colors pointed to by \a cols, or \a cols is an invalid pointer, or count exceeds the actual number 
        * of source colors. */
      NVSG_API void setSecondaryColors( unsigned int    fdim  //!< Dimension of colors, in terms of float
                                      , size_t          pos   //!< Start position inside the range of pre-existing colors.
                                      , const float   * cols  //!< Address of source colors
                                      , size_t          count //!< Number of colors to copy
                                      );

      //! Get number of fog coordinates.
      /** \returns  The number of fog coordinates. */
      NVSG_API size_t getNumberOfFogCoords( void ) const;

      //! Get fog coordinates.
      /** \returns A pointer to the fog coordiantes. */
      NVSG_API const float * getFogCoords( void ) const;

      //! Set the fog coordinates.
      /** Copies \a count fog coordinates from the location pointed to by \a coords to this VertexAttributeSet. 
        * \note The behaviour is undefined if either \a coords is an invalid pointer, or count exceeds the actual number
        * of source coordinates. */
      NVSG_API void setFogCoords( const float * coords  //!< Address of source coordinates. 
                                , size_t        count   //!< Number of coordinates to copy.
                                );

      //! Set the fog coordinates.
      /** Copies \a count fog coordinates from the location pointed to by \a coords to this VertexAttributeSet,
        * starting at position \a pos inside the range of pre-existing coordinates. 
        * Pre-existing coordinates in the range [\a pos, \a pos + \a count) will be replaced.
        * Pre-existing coordinates outside this range remain untouched.
        * If you specify -1 for \a pos or if \a pos specifies the number of coordinates currently stored
        * the coordinates pointed to by \a coords will be appended to the pre-existing coordinates. 
        * If you not specify -1 for \a pos or \a pos neither specifies the number of coordinates currently
        * stored nor does it specify a valid position inside the range of pre-existing coordinates, 
        * the behaviour is undefined!
        * \note The behaviour is undefined if either \a coords is an invalid pointer, or count exceeds the actual number
        * of source coordinates. */
      NVSG_API void setFogCoords( size_t        pos     //!< Start position inside the range of pre-existing coordinates
                                , const float * coords  //!< Address of source coordinates. 
                                , size_t        count   //!< Number of coordinates to copy.
                                );

      //! Assignment operator
      /** Performs a deep copy. */
      NVSG_API VertexAttributeSet & operator=(const VertexAttributeSet & rhs);

      //! Test for equivalence with an other VertexAttributeSet.
      /** \return true if the VertexAttributeSet \a p is equivalent to this VertexAttributeSet. */
      NVSG_API virtual bool isEquivalent( const Object *p   //!< VertexAttributeSet to test for equivalence with
                                        , bool ignoreNames  //!< are the names to be ignored?
                                        , bool deepCompare //!< deep compare?
                                        ) const;

    protected:
      //! Invalidate the Incarnation.
      /** This function is called from the framework when the Incarnation of this
        * VertexAttributeSet has increased and now is to be marked as invalid. It first
        * increases the Incarnation of all it's owners, then it's own Incarnation is
        * marked as invalid. */
      NVSG_API virtual void invalidateIncarnation( void );      // from bottom to top

    private:
      void releaseCachedData();

    private:
      nvutil::bitset64                                  m_formatBits; // specifies the interleaved format 
      nvutil::RCPtr<nvutil::RCVector<VertexAttribute> > m_vertexAttributes;    // shareable vertex attribs
  };


  //! Type to specify the type of texture coordinate to generate.
  /** There are three different types of texture coordinate generation available: cylindrical, planar, and spherical.*/
  typedef enum
  {
    TCT_CYLINDRICAL,      //!<  Puts a cylinder around the \c BoundingSphere with the z-axis as it's axis and projects the vertices of the \c VertexAttributeSet on it to get the texture coordinates.
    TCT_PLANAR,           //!<  Puts a plane in the x-y-plane and vertically projects the vertices of the \c VertexAttributeSet on that plane to the the texture coordinates.
    TCT_SPHERICAL,        //!<  Projects the vertices of the \c VertexAttributeSet radially on the \c BoundingSphere to get the texture coordinates.
  } TextureCoordType;

  /*! \relates VertexAttributeSet
   *  Generate texture coordinates into a texture unit.
   *  Calculates either spherical, cylindrical, or planar 
   *  two-dimensional texture coordinates into texture unit \a
   *  tu. The \a sphere can be either the bounding sphere of 
   *  this PrimitiveSet to calculate local texture coordinates or
   *  some other for non-local texture coordinates.        */
  NVSG_API void generateTexCoords( VertexAttributeSet * vas       //!< pointer to the VertexAttributeSet to use
                                 , TextureCoordType tct           //!< type of texture coordinates to generate
                                 , const nvmath::Sphere3f &sphere //!< bounding sphere to map coordinates to
                                 , size_t tu = 0                  //!< texture unit to generate the coordinates in
                                 );


  inline const nvutil::bitset64& VertexAttributeSet::getInterleavedFormat() const
  {
    NVSG_TRACE();
    return m_formatBits;
  }

  inline const float * VertexAttributeSet::getVertexAttributeData( size_t attrib ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT(attrib < NUMBER_OF_VERTEX_ATTRIBUTES );
    return( (*m_vertexAttributes)[attrib].getData() );
  }

  inline size_t VertexAttributeSet::getNumberOfVertexAttributeData( size_t attrib ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT(attrib < NUMBER_OF_VERTEX_ATTRIBUTES);
    return( (*m_vertexAttributes)[attrib].getNumberOfData() );
  }

  inline unsigned int VertexAttributeSet::getVertexAttributeDataDim( size_t attrib ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT(attrib < NUMBER_OF_VERTEX_ATTRIBUTES );
    return( (*m_vertexAttributes)[attrib].getDataDimension() );
  }

  inline size_t VertexAttributeSet::getNumberOfVertices( void ) const 
  {
    NVSG_TRACE();
    return getNumberOfVertexAttributeData(VERTEX_POSITION);
  }

  inline const nvmath::Vec3f * VertexAttributeSet::getVertices( void )  const
  {
    NVSG_TRACE();
    return (const nvmath::Vec3f *)getVertexAttributeData(VERTEX_POSITION);
  }

  inline void VertexAttributeSet::setVertices( const nvmath::Vec3f * verts, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(VERTEX_POSITION, 3, (const float*)verts, count);
  }

  inline void VertexAttributeSet::setVertices( size_t pos, const nvmath::Vec3f * verts, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(VERTEX_POSITION, 3, pos, (const float*)verts, count);
  }

  inline size_t VertexAttributeSet::getNumberOfNormals( void ) const 
  {
    NVSG_TRACE();
    return getNumberOfVertexAttributeData(NORMAL);
  }

  inline const nvmath::Vec3f * VertexAttributeSet::getNormals( void ) const 
  {
    NVSG_TRACE();
    return (const nvmath::Vec3f *)getVertexAttributeData(NORMAL);
  }

  inline void VertexAttributeSet::setNormals( const nvmath::Vec3f * nor, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(NORMAL, 3, (const float*)nor, count);
    //^ normals always are 3-dimensional
  }

  inline void VertexAttributeSet::setNormals( size_t pos, const nvmath::Vec3f * nor, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(NORMAL, 3, pos, (const float*)nor, count );
  }

  inline bool VertexAttributeSet::hasTexCoords( void ) const
  {
    NVSG_TRACE();
    return(  getNumberOfVertexAttributeData(TEXTURE_COORD_0)
          || getNumberOfVertexAttributeData(TEXTURE_COORD_1)
          || getNumberOfVertexAttributeData(TEXTURE_COORD_2)
          || getNumberOfVertexAttributeData(TEXTURE_COORD_3)
          || getNumberOfVertexAttributeData(TEXTURE_COORD_4)
          || getNumberOfVertexAttributeData(TEXTURE_COORD_5)
          || getNumberOfVertexAttributeData(TEXTURE_COORD_6)
          || getNumberOfVertexAttributeData(TEXTURE_COORD_7) );
  }

  inline size_t VertexAttributeSet::getNumberOfTexCoords( size_t texUnit ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT(texUnit <= 7);
    return getNumberOfVertexAttributeData(TEXTURE_COORD_0+texUnit);
  }

  inline const float * VertexAttributeSet::getTexCoords( size_t texUnit ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT(texUnit <= 7);
    return getVertexAttributeData(TEXTURE_COORD_0+texUnit);
  }

  inline unsigned int VertexAttributeSet::getTexCoordsDim( size_t texUnit ) const
  {
    NVSG_TRACE();
    NVSG_ASSERT(texUnit <= 7);
    return getVertexAttributeDataDim(TEXTURE_COORD_0+texUnit);
  }

  inline void VertexAttributeSet::clearTexCoords( size_t texUnit )
  {
    NVSG_TRACE();
    NVSG_ASSERT(texUnit <= 7);    
    clearVertexAttributeData(TEXTURE_COORD_0+texUnit);
  }

  inline void VertexAttributeSet::setTexCoords( size_t texUnit, unsigned int fdim, const float* coords, size_t count )
  {
    NVSG_TRACE();
    NVSG_ASSERT(texUnit <= 7);
    setVertexAttributeData(TEXTURE_COORD_0+texUnit, fdim, coords, count);
  }

  inline void VertexAttributeSet::setTexCoords( size_t texUnit, unsigned int fdim, size_t pos, const float* coords, size_t count )
  {
    NVSG_TRACE();
    NVSG_ASSERT(texUnit <= 7);
    setVertexAttributeData(TEXTURE_COORD_0+texUnit, fdim, pos, coords, count);
  }

  inline size_t VertexAttributeSet::getNumberOfColors( void ) const
  {
    NVSG_TRACE();
    return getNumberOfVertexAttributeData(PRIMARY_COLOR);
  }

  inline const float * VertexAttributeSet::getColors( void ) const
  {
    NVSG_TRACE();
    return getVertexAttributeData(PRIMARY_COLOR);
  }

  inline unsigned int VertexAttributeSet::getColorsDim( void ) const
  {
    NVSG_TRACE();
    return getVertexAttributeDataDim(PRIMARY_COLOR);
  }

  inline void VertexAttributeSet::setColors( unsigned int fdim, const float * cols, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(PRIMARY_COLOR, fdim, cols, count);
  }

  inline void VertexAttributeSet::setColors( unsigned int fdim, size_t pos, const float * cols, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(PRIMARY_COLOR, fdim, pos, cols, count);
  }

  inline size_t VertexAttributeSet::getNumberOfSecondaryColors( void ) const
  {
    NVSG_TRACE();
    return getNumberOfVertexAttributeData(SECONDARY_COLOR);
  }

  inline const float * VertexAttributeSet::getSecondaryColors( void ) const
  {
    NVSG_TRACE();
    return getVertexAttributeData(SECONDARY_COLOR);
  }

  inline unsigned int VertexAttributeSet::getSecondaryColorsDim( void ) const
  {
    NVSG_TRACE();
    return getVertexAttributeDataDim(SECONDARY_COLOR);
  }

  inline void VertexAttributeSet::setSecondaryColors( unsigned int fdim, const float* cols, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(SECONDARY_COLOR, fdim, cols, count);
  }

  inline void VertexAttributeSet::setSecondaryColors( unsigned int fdim, size_t pos, const float* cols, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(SECONDARY_COLOR, fdim, pos, cols, count);
  }

  inline size_t VertexAttributeSet::getNumberOfFogCoords( void ) const
  {
    NVSG_TRACE();
    return getNumberOfVertexAttributeData(FOG_COORDINATE);
  }

  inline const float * VertexAttributeSet::getFogCoords( void ) const
  {
    NVSG_TRACE();
    return getVertexAttributeData(FOG_COORDINATE);
  }

  inline void VertexAttributeSet::setFogCoords( const float * coords, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(FOG_COORDINATE, 1, coords, count);
    //^ fog coords always are 1-dimensional
  }

  inline void VertexAttributeSet::setFogCoords( size_t pos, const float * coords, size_t count )
  {
    NVSG_TRACE();
    setVertexAttributeData(FOG_COORDINATE, 1, pos, coords, count);
    //^ fog coords always are 1-dimensional
  }

} //  namespace nvsg
