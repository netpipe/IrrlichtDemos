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
#include <map>
#include <vector>

#if defined(LINUX)
# include <errno.h>
#endif

namespace nvutil
{
  /*! \brief Helper class to ease efficient reading and writing of large files.
   *  \remarks This is the base class for class ReadMapping and class WriteMapping, that can be used
   *  to hide platform dependencies on reading or writing. This is done by selectively mapping parts
   *  of a file in and out.
   *  \note This class is not intended to be directly instantiated.
   *  \sa ReadMapping, WriteMapping */
  class FileMapping
  {
    public :
      /*! \brief Get the last error.
       *  \return An platform dependent unsigned int that describes the last error on using a mapped
       *  file.
       *  \par Example:
       *  const void * ptr = rm->mapIn( offset, count*sizeof(int) );
       *  if ( ! ptr )
       *  {
       *    unsigned int errCode = rm->getLastError();
       *    // ...
       *  }
       *  \code
       *  \endcode */
      NVSG_API unsigned int getLastError() const;

      /*! \brief Test if a FileMapping is valid.
       *  \return \c true, if the FileMapping is currently valid, otherwise \c false.
       *  \remarks Before using a FileMapping, you should test for its validity.
       *  \par Example:
       *  \code
       *    ReadMapping rm = new ReadMapping( fileName );
       *    if ( rm->isValid() )
       *    {...}
       *  \endcode */
      NVSG_API bool isValid() const;

      /*! \brief Maps out a previously mapped in part of a file.
       *  \param offsetPtr The constant pointer to void that was previously returned by a call to
       *  mapIn.
       *  \par Example:
       *  \code
       *  const void * ptr = rm->mapIn( offset, count*sizeof(int) );
       *  if ( ptr )
       *  {
       *    // ...
       *    rm->mapOut( ptr );
       *  }
       *  \endcode
       *  \sa mapIn */
      NVSG_API void mapOut( const void * offsetPtr );

    protected:
      /*! \brief Protected default constructor.
       *  \remarks The constructor of FileMapping is protected to prevent explicit instantiation.
       *  \sa ReadMapping, WriteMapping */
      NVSG_API FileMapping();

      /*! \brief Protected destructor.
       *  \remarks The destructor of FileMapping is protected to prevent explicit instantiation.
       *  \sa ReadMapping, WriteMapping */
      NVSG_API ~FileMapping();

      /*! \brief Protected base function to map in part of the file.
       *  \param offset The offset that has to be part of the mapping.
       *  \param numBytes The number of bytes that, starting from \a offset, are to be part of
       *  the mapping.
       *  \return A pointer to the mapped memory location, or NULL if the mapping failed.
       *  \sa mapOut */
      NVSG_API void * mapIn( size_t offset, size_t numBytes );

    private:
      struct ViewHeader
      {
        ViewHeader( size_t start, size_t size, void * base )
          : basePtr(base)
          , startOffset(start)
          , endOffset(start+size)
          , refCnt(0)
        {}

        void  * basePtr;      // points to the beginning of the view
        size_t  startOffset;  // the view's start offset inside the mapped file 
        size_t  endOffset;    // the view's 'past tht end' offset inside the mapped file
        int     refCnt;       // reflects if the view is in use or not
      };

    protected:
      unsigned int                m_accessType;       //!< read-only or read/write
#if defined(_WIN32)
      HANDLE                      m_file;             //!< file handle returned by CreateFile
      HANDLE                      m_fileMapping;      //!< handle of file mapping object returned by CreateFileMapping
#elif defined(LINUX)
      int                         m_file;             //!< file descriptor returned by open()
#endif
      size_t                      m_mappingSize;      //!< actual size of the file mapping
      bool                        m_isValid;          //!< file mapping is valid

    private :
      std::vector<ViewHeader*>    m_mappedViews;      // collection of currently mapped views
      std::map<void*,ViewHeader*> m_offsetPtrViewMap; // mapping offset pointers to their associated mapped view
  };

  /*! \brief Helper class to ease efficient reading of large files.
   *  \sa FileMapping */
  class ReadMapping : public FileMapping
  {
    public :
      /*! \brief Constructor using the name of the file to read.
       *  \param fileName The name of the file to read.
       *  \note If the file \a fileName does not exist, the resulting ReadMapping is marked as
       *  invalid. Only valid ReadMappings can be used for reading.
       *  \par Example:
       *  \code
       *    ReadMapping rm = new ReadMapping( fileName );
       *    if ( rm->isValid() )
       *    {...}
       *  \endcode */
      NVSG_API ReadMapping( const std::string & fileName );

      /*! \brief Destructor of a read-only mapping.
       *  \remarks If the ReadMapping is valid, the opened file is closed. */
      NVSG_API ~ReadMapping();

      /*! \brief Map a view of the file.
       *  \param offset The offset that has to be part of the mapping.
       *  \param numBytes The number of bytes that, starting from \a offset, are to be part of
       *  the mapping.
       *  \return A pointer to the constant mapped memory location, or NULL if the mapping failed.
       *  \sa mapOut */
      NVSG_API const void * mapIn( size_t offset, size_t numBytes );
  };

  /*! \brief Helper class to ease efficient writing of large files.
   *  \sa FileMapping */
  class WriteMapping : public FileMapping
  {
    public:
      /*! \brief Constructor using the name of the file to write.
       *  \param fileName The name of the file to write.
       *  \param fileSize The size of the file to write.
       *  \remarks If there is enough free space on the disk specified by \a fileName, a file of
       *  that with the name \a fileName is created.
       *  \par Example:
       *  \code
       *    WriteMapping wm = new WriteMapping( fileName, preCalculatedFileSize );
       *    if ( wm->isValid() )
       *    {...}
       *  \endcode */
      NVSG_API WriteMapping( const std::string & fileName, size_t fileSize );

      /*! \brief Destructor of a writable mapping.
      *  \remarks If the WriteMapping is valid, the written file is closed. */
      NVSG_API ~WriteMapping();

      /*! \brief Map a view of the file.
       *  \param offset The offset that has to be part of the mapping.
       *  \param numBytes The number of bytes that, starting from \a offset, are to be part of
       *  the mapping.
       *  \return A pointer to the mapped memory location, or NULL if the mapping failed.
       *  \sa mapOut */
      NVSG_API void * mapIn( size_t offset, size_t numBytes );

    private :
      size_t  m_endOffset;    // last accessed offset in file => eof
  };


  inline unsigned int FileMapping::getLastError() const
  {
#if defined(_WIN32)
    return( GetLastError() );
#elif defined(LINUX)
    return( errno );
#else
    NVSG_CTASSERT( false );
#endif
  }

  inline bool FileMapping::isValid() const
  {
    return( m_isValid );
  }


  inline const void * ReadMapping::mapIn( size_t offset, size_t numBytes )
  {
    return( FileMapping::mapIn( offset, numBytes ) );
  }

} // namespace nvutil
