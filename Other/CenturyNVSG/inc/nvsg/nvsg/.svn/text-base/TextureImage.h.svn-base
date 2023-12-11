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
#include "nvsg/DAL.h"
#include "nvsg/CoreTypes.h"
#include "nvmath/nvmath.h"
#include "nvutil/BitMask.h"
#include "nvutil/PlugIn.h"
#include <math.h>
#include <vector>
#include <map>
#include <string>

// VERTICAL_CROSS_FORMAT(w,h) macro evaluates to true if 
// width and height comply with vertical cross format:
//      
//        ___     
//       |   |
// 3  ___|___|____
//   |   |   |    |    
// 2 |___|___|____|    
//       |   |          3xheight == 4xwidth
// 1     |___|
//       |   |
// 0     |___|
//
//     0   1   2
#define VERTICAL_CROSS_FORMAT(w,h) ((3*(h))==(4*(w)))

namespace nvsg
{
  class ScaleFilter;
  class TextureLoader;

  /*! \brief Helper structure to hold the data of an image.
   *  \par Namespace: nvsg
   *  \sa insert, rescale */
  struct Image
  {
    //! Enumeration type to describe a pixel data format
    enum PixelFormat
    {
        IMG_UNKNOWN_FORMAT = -1 //!< unknown pixel format
      , IMG_COLOR_INDEX = 0 //!< color index format
      , IMG_RGB							//!< RGB format	
      , IMG_RGBA						//!< RGBA format	
      , IMG_BGR							//!< BGR format	
      , IMG_BGRA						//!< BGRA format	
      , IMG_LUMINANCE				//!< luminance format 	
      , IMG_LUMINANCE_ALPHA	//!< luminance alpha format	
      , IMG_NUM_FORMATS //!< specifies number of valid format descriptors
    };

    //! Enumeration type to describe the data type of pixel data
    enum PixelDataType
    {
        IMG_UNKNOWN_TYPE = -1 //!< unknown data type
      , IMG_BYTE = 0          //!< signed byte type (8-bit)
      , IMG_UNSIGNED_BYTE     //!< unsigned byte type (8-bit)
      , IMG_SHORT             //!< signed short type (16-bit)  
      , IMG_UNSIGNED_SHORT    //!< unsigned short type (16-bit)
      , IMG_INT               //!< signed integer type (32-bit)
      , IMG_UNSIGNED_INT      //!< unsigned integer type (32-bit)
      , IMG_FLOAT             //!< single-precision floating point (32-bit)
      , IMG_HALF              //!< half-precision floating point (16-bit)
      , IMG_NUM_TYPES //!< specifies number of valid type descriptors
    };

    /*! \brief Default constructor of Image.
     *  \remarks Initializes an empty Image with no data. */
    NVSG_API Image();

    /*! \brief Constructor of an Image structure.
     *  \param width The width of the Image.
     *  \param height The height of the Image.
     *  \param depth The depth of the Image.
     *  \param format The PixelFormat of the Image.
     *  \param type The PixelDataType of the Image.
     *  \remarks This constructor allocates no memory for the pixel data, it just initialized the
     *  structure Image to hold the information for it.
     *  \note The bytes per pixel can be determined out of \a format (giving number of components)
     *  and \a type (giving size of components).
     *  \note The \a overlap is defined to be part of size of the Image. That is, if an Image has
     *  a \a width of e.g. 32 and a \a overlap of 2, the inner width of the Image is only 28. The
     *  first two and the last two pixels of the 32 pixels are the overlap pixels.
     *  \note When \a overlap is not zero, \a width, \a height, and \a depth has to be either one
     *  (for zero-, one-, or two-dimensional images respectively) or at least three times \a
     *  overlap.
     *  \note For displaying purposes, the size an Image often has to be power of two. Here, the
     *  overall size, including overlaps, are supposed to be powers of two. */
    NVSG_API Image( unsigned int width, unsigned int height, unsigned int depth
                  , PixelFormat format, PixelDataType type );
  
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // NOTE:
    // We need the copy constructor performing just a flat copy. That is, the 
    // copy must not allocate its own pixel storage and copy pixels from the 
    // source, but have m_pixels pointing to the same storage instead!
    //
    // This is exactly what a default copy constructor does! 
    // Therefore, we don't need to implement a copy constructor by our own!
    // Same is true for the assignment operator!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    unsigned int    m_width;      //!< width of the texture in pixels
    unsigned int    m_height;     //!< height of the texture in pixels
    unsigned int    m_depth;      //!< depth of the texture in pixels
    PixelFormat     m_format;     //!< specifies format of pixel data 
    PixelDataType   m_type;       //!< specifies type of pixel data
    unsigned int    m_bpp;        //!< bytes per pixel
    unsigned int    m_bpl;        //!< bytes per scanline
    unsigned int    m_bps;        //!< bytes per slice (plane)
    unsigned int    m_nob;        //!< number of bytes (complete image)        
    unsigned char * m_pixels;     //!< raw image data (we use unsigned char as internal format)
  };

#if defined(NDEBUG)
  /*! \brief Helper function to allocate memory for pixel data.
   *  \param nbytes The number of bytes to allocate.
   *  \return A pointer to the allocated memory.
   *  \remarks This function allocates the memory and assures it to be eight byte aligned. You
   *  should use freeImagePixels to free that memory.
   *  \sa freeImagePixels */
  NVSG_API unsigned char * allocImagePixels( unsigned int nbytes );
#else
  /*! \brief Same as allocImagePixels + leak detection */
  NVSG_API unsigned char * dbgAllocImagePixels( unsigned int nbytes, size_t line );
  // This macro allows the same API usage as in the release mode
  #define allocImagePixels(nbytes) dbgAllocImagePixels( nbytes, __LINE__)
#endif

  /*! \brief Helper function to free memory of pixel data.
   *  \param pixels A pointer to the pixel data to free.
   *  \remarks This function assumes that \a pixels has been allocated by allocImagePixels.
   *  \sa allocImagePixels */
  NVSG_API void freeImagePixels( unsigned char * pixels );

  /*! \brief Helper function to determine the number of components of a PixelFormat.
   *  \param format The PixelFormat to determine the number of components from.
   *  \return The number of components described by \a format.
   *  \sa sizeOfComponents */
  NVSG_API unsigned int numberOfComponents( Image::PixelFormat format );

  /*! \brief Helper function to determine the size of components of a PixelDataType.
   *  \param type The PixelDataType to determine the size of components from.
   *  \return The size of components described by \a type.
   *  \sa numberOfComponents */
  NVSG_API unsigned int sizeOfComponents( Image::PixelDataType type );

  /*! \brief Helper function to determine the contributions of a ScaleFilter on scaling an Image.
   *  \param contribs A reference to a vector of vector of pairs of an int and a float. The i'th
   *  element of \a contribs holds the vector of contributions for the i'th destination value.
   *  The j'th element of \a contribs[i] holds the index and the contribution of the source
   *  values.
   *  \param srcSize The number of the source values.
   *  \param dstSize The number of the destination values.
   *  \param filter A pointer to the ScaleFilter to use for contribution determination.
   *  \param overlapSize An optional parameter giving the overlap size of the source.
   *  \remarks To determine the contributions of \a filter on a multidimensional array of values
   *  call calculateContributions once per dimension.
   *  \par Example
   *  \code
   *  vector<vector<pair<int,float> > > contributions[3];
   *  //  contributions[0..2]: contributions in x-, y-, and z-direction
   *  calculateContributions( contributions[0], srcWidth, dstWidth, sf );
   *  calculateContributions( contributions[1], srcHeight, dstHeight, sf );
   *  calculateContributions( contributions[2], srcDepth, dstDepth, sf );
   *  rescale( src, dst, contributions );
   *  \endcode
   *  \sa insert, rescale, TextureImage::filter */
  NVSG_API void calculateContributions( std::vector<std::vector<std::pair<int,float> > > &contribs
                                      , size_t srcSize, size_t dstSize
                                      , const ScaleFilter * filter );

  /*! \brief Helper function to insert a source image into a destination image at a given position.
   *  \param src A pointer to the constant source image.
   *  \param dst A pointer to the destination image.
   *  \param x x-position of the source image in the destination image.
   *  \param y y-position of the source image in the destination image.
   *  \param z y-position of the source image in the destination image.
   *  \remarks It is assumed the the source image offset by x/y/z completely fits into the
   *  destination image.
   *  \note If the source image has overlaps, the overlap pixels are not copied (and need not fit
   *  into the destination image).
   *  \sa rescale */
  NVSG_API void insert( const Image * src, Image * dst, size_t x, size_t y, size_t z );

  /*! \brief Helper function to rescale an Image using the contributions of a ScaleFilter.
   *  \param src A pointer to the constant source Image to scale.
   *  \param dst A pointer to the destination Image.
   *  \param contributions A pointer to three vectors of vectors of pairs of an int and a float
   *  that holds the contributions of each source pixel to each destination pixel in x, y, and z
   *  direction.
   *  \remarks The \a contributions can be calculated by calculateContributions.
   *  \note If the destination image is defined to have overlaps, those pixel values are not set.
   *  \sa calculateContributions */
  NVSG_API void rescale( const Image * src, Image *dst
                       , std::vector<std::vector<std::pair<int,float> > > *contributions );

#if !defined(NDEBUG)
  /*! \brief Asserts mipmap chain to be complete for the specified TextureImage */ 
  NVSG_API void assertMipMapChainComplete(const TextureImage * img);
#else
# define assertMipMapChainComplete(i) static_cast<void*>(0)    
#endif

  //! A texture image abstraction class.
  class TextureImage : public nvutil::HandledObject, public DALHost  // can host DAL data
  {
  public:

    //! Flags considered for image processing
    enum 
    {
      F_PRESERVE_IMAGE_DATA_AFTER_UPLOAD = BIT0 /*!<
        Serves as a hint to the upload code to keep the image data in process memory even after image upload. */
    , F_IMAGE_STREAM = BIT2 /*!<
        Forces the image to be treated as an image stream, that is, no pixel data will be copied to internal structures. */
    , F_NO_IMAGE_CREATION = BIT4  /*!<
        Only create the information data for this TextureImage, don't load the texture data. */
    
    , F_SCALE_FILTER_BOX = 0                //!< scale the image with a box filter (nearest)
    , F_SCALE_FILTER_TRIANGLE = BIT7        //!< scale the image with a triangle filter (linear)
    , F_SCALE_FILTER_BELL = BIT8            //!< scale the image with a bell filter
    , F_SCALE_FILTER_BSPLINE = BIT7 | BIT8  //!< scale the image with a bspline filter
    , F_SCALE_FILTER_LANCZOS3 = BIT9        //!< scale the image with a lanczos3 filter
    , F_SCALE_FILTER_MASK = BIT7 | BIT8 | BIT9

    , F_SCALE_POT_NEAREST = 0  /*!< always scale to nearest power of two if scaling is required (default) */
    , F_SCALE_POT_ABOVE = BIT16 /*!< always scale to the next power of two if scaling is required */
    , F_SCALE_POT_BELOW = BIT17 /*!< always scale to the highest power of two below if scaling is required */
    , F_SCALE_POT_MASK= F_SCALE_POT_ABOVE | F_SCALE_POT_BELOW
    };

    //! Texture target specification
    /** The texture target of a TextureImage is determined by the number 
      * and dimension of images it contains. */
    enum
    {
      TEXTURE_UNSPECIFIED_TARGET = ~0, //!< indicates an unspecified target
      TEXTURE_1D = 0, //!< indicates a 1-dimensional texture target
      TEXTURE_2D, //!< indicates a 2-dimensional texture target
      TEXTURE_3D, //!< indicates a 3-dimensional texture target
      TEXTURE_CUBE, //!< indicates a cube map target
      NUM_TEXTURE_TARGETS 
    };

    /*! \brief Instantiates a TextureImage object filled with image data loaded from a file.
      * \param filename Specifies the name of the file to load the image data from.
      * \param searchPaths Paths to search for the file.
      * \param creationFlags Optional creation flags.
      * \return The function returns a pointer to a valid TextureImageHandle if succeeded.
      * Otherwise a null pointer will be returned.
      * \remarks
      * The function instantiates a TextureImage object and initializes it with image data loaded from the
      * file specified by \a filename. 
      *
      * The actual image loading will be performed through PlugIns, thus the available PlugIns determine
      * which image formats are supported for loading.
      * By default, NVSG will look for TextureLoader PlugIns inside the application directory. 
      * Use addPlugInSearchPath() to specify additional directories, if your loader PlugIns lie somewhere else.
      *
      * Client code may provide optional creation flags:
      *
      * Specify the F_PRESERVE_IMAGE_DATA_AFTER_UPLOAD to give a hint to the upload code to keep the image data 
      * in process memory even after image upload. */
    NVSG_API static TextureImageHandle * createFromFile( const std::string& filename
                                                       , const std::vector<std::string>& searchPaths
                                                       , unsigned int creationFlags = 0 );

    /*! \brief Default-constructs a TextureImage.
     * A default created TextureImage uses a triangle filter for scaling and mipmap creation.
     * Image data later specified through createImage will be copied to the internal data 
     * structures and released after image upload. */
    NVSG_API TextureImage();

    /*! \brief Constructs a TextureImage as a copy of another TextureImage. */
    NVSG_API TextureImage(const TextureImage&);

    /*! Destructs a TextureImage */
    NVSG_API virtual ~TextureImage();

    /*! \brief Sets creation flags to consider for further image processing. 
      * \param flags Creation flags to consider for further image processing.
      * \remarks
      * \remarks
      * This function lets you specify certain creation flags which affect subsequent 
      * image processing.\n
      * Possible creation flags are:\n
      * F_PRESERVE_IMAGE_DATA_AFTER_UPLOAD\n 
      * F_NO_SCALING\n
      * F_IMAGE_STREAM\n
      * F_SCALE_FILTER_BOX\n
      * F_SCALE_FILTER_BELL\n
      * F_SCALE_FILTER_BSPLINE\n
      * F_SCALE_FILTER_LANCZOS3\n
      * F_SCALE_FILTER_MASK\n */
    NVSG_API void setCreationFlags(unsigned int flags);

    /*! \brief Returns the name of the file from which the TextureImage was created. 
     * \returns The function returns the name of the file from which the TextureImage was created.
     * If the TextureImage was not created from a file, an empty string will be returned.
     * \sa createFromFile, createImage */
    NVSG_API const std::string& getFileName() const;

    //! Creates an image.
    /** Use this function to create a single image of the specified dimensions, pixel format,
      * and pixel data type.
      * 
      * If the F_IMAGE_STREAM flag was not specified at object instantiation, the pixel data
      * pointed to by \a pixels will be copied to the internal data structures. 
      * If, on the other hand, the F_IMAGE_STREAM flag was specified at object instantiation, 
      * no pixel data will be copied to the internal structures but only the address specified
      * by \a pixels will be stored. The image will be treated as an image stream in the latter case.
      *
      * Note, that the pixel data pointed to by \a pixels always must match the specified dimensions,
      * pixel format, and data type. Otherwise this would result in undefined behavior.
      *
      * The behavior also is undefined if an invalid pointer will be passed through \a pixels.
      *
      * In addition to the level 0 image, additional mipmap levels can be specified through createImage.
      * The user can do so by passing the pointers to the raw pixel data of the additional mipmap levels
      * through \a mipmaps. If an incomplete mipmap chain is specified, the missing levels are added 
      * using the current filter settings. 
      * Note that, specifying an incomplete chain with F_IMAGE_STREAM set is an error; no mipmaps are 
      * created in this case.
      *
      * The passed mipmap levels are assumed to be sorted from level 1 to level \a n. The dimensions
      * of the mipmaps are assumed to be according to the particular level, that is - level 1 mipmap
      * is assumed of dimension (with(level 0) / 2) x (height(level 0) / 2) x (depth(level 0)/2) pixel, level 2 
      * mipmap is assumed of dimension (with(level 1) / 2) x (height(level 1) / 2) x (depth(level 1)/2) pixels,
      * and so on, down to 1 x 1 x 1 pixels for the last level. 
      *
      * The additional mipmap levels are assumed to have the same bytes per pixel as the level 0 image.
      * The additional mipmap levels are also assumed to be of the same pixel format, and data type as
      * the level 0 image.
      *
      * \returns The zero based index of the image if creation was successful.
      * Otherwise the function returns -1 indicating that the image creation failed.
      * \sa Image */
    NVSG_API size_t createImage( unsigned int           width      //!< width in pixels
                               , unsigned int           height     //!< height in pixels
                               , unsigned int           depth      //!< depth in pixels 
                               , Image::PixelFormat     format     //!< format of image data
                               , Image::PixelDataType   type       //!< type of image data
                               , const void           * pixels     //!< raw pixel data 
                               , const std::vector<const void*>& mipmaps = std::vector<const void*>() /**!< 
                                                                 additional mipmap levels (optional)*/
                               );

    /*! \brief Add an Image structure without pixel data to the TextureImage.
     *  \param width The width of the Image to add.
     *  \param height The height of the Image to add.
     *  \param depth The depth of the Image to add.
     *  \param format the PixelFormat of the Image to add.
     *  \return The index of the Image in this TextureImage.
     *  \remarks This function adds an Image structure to the TextureImage, but does not allocate
     *  memory for the pixel data.
     *  \sa createImage, setImageData, Image */
    NVSG_API size_t addImage( unsigned int width
                            , unsigned int height
                            , unsigned int depth
                            , Image::PixelFormat format
                            , Image::PixelDataType type );

    /*! \brief Set the pixel data of an Image.
     *  \param image The index of the Image to set the data.
     *  \param pixels A pointer to the pixel data to set.
     *  \param mipmaps An optional vector of pointers to pixel data for the mipmaps.
     *  \remarks If the TextureImage is a streaming image (F_IMAGE_STREAM is set), the pointer to
     *  the pixel data of the Image at index \a image just points to \a pixels. Any data in
     *  \a mipmaps is ignored then.\n
     *  Otherwise the memory for the pixels are allocated if needed and copied to the Image at
     *  index \a image. If there are mipmaps provided, the data is also copied.
     *  \note The behavior is undefined if \a image is greater or equal to the number of images
     *  in this TextureImage. Furthermore, it is assumed that \a pixels point to memory that is
     *  large enough to hold the complete data of the Image. If \a mipmaps are provided, it is
     *  assumed that each of the pointers in \a mipmaps point to memory that is large enough to
     *  hold the complete data of the corresponding mipmap level of the Image. If an incomplete mipmap chain 
     *  is specified, the missing levels are added using the current filter settings. 
     *  Note that, specifying an incomplete chain with F_IMAGE_STREAM set is an error; no mipmaps are 
     *  created in this case.
     *  \sa createImage, addImage, Image */
    NVSG_API void setImageData( size_t image
                              , const void * pixels
                              , const std::vector<const void *> & mipmaps = std::vector<const void *>() );

    //! Releases all contained image data. 
    /** The function frees all contained image data including all mipmap levels that might have been created before. */
    /*! \brief Release the memory used for the image pixels.
     *  \remarks The function frees all image pixels of the TextureImage. Only size and format
     *  informations are kept.
     *  \sa reloadImagePixels */
    NVSG_API void releaseImagePixels(); 

    /*! \brief Reload the TextureImage.
     *  \return \c true if the images could be reloaded, otherwise \c false.
     *  \remarks If for example the images data has been release at some point, it can be
     *  reloaded using this function.
     *  \sa load, releaseImagePixels */
    NVSG_API bool reloadImagePixels();

    /*! \brief Increments (increases by one) the mipmap use count
     *  The function increments the  mipmap use count for this Texture image by one.
     *
     *  This function should be called by the client whenever the corresponding minification
     *  filter settings change from a non-mipmap filter mode to a mipmap filter mode. 
     *
     *  The function serves sharing of this TextureImage between multiple clients that
     *  probably have different corresponding minification filter settings. Mipmaps are
     *  required if at least one client has mipmap filtering on.
     *
     *  \sa decrementMipmapUseCount, getMipmapUseCount
     */
    NVSG_API void incrementMipmapUseCount();

    /*! \brief Decrements (decreases by one) the mipmap use count
     *  The function increments the  mipmap use count for this Texture image by one.
     *
     *  This function should be called by the client whenever the corresponding minification
     *  filter settings change from a mipmap filter mode to a non-mipmap filter mode.
     *
     *  If at upload time the mipmap use count is zero, no mipmap generation will be forced, 
     *  nor will existing mipmaps be uploaded to the graphics hardware.
     *
     *  \sa incrementMipmapUseCount, getMipmapUseCount
     */
    NVSG_API void decrementMipmapUseCount();

    /*! \brief Returns the actual mipmap use count.
     *  \returns The function returns the actual mipmap use count for this TextureImage.
     *  If the mipmap use count is non-zero, at least one client has corresponding 
     *  minification filter settings that require mipmaps to be generated and uploaded 
     *  for this TextureImage. Upload code should evaluate this count for conditional
     *  mipmap creation and upload. Mipmap creation, if required, can be performed by
     *  calling createMipmaps.
     *  \sa incrementMipmapUseCount, decrementMipmapUseCount, createMipmaps
     */
    NVSG_API size_t getMipmapUseCount() const;

    /*! \brief Create all mipmap levels for all included images.
     *  \return \c true if all levels have been created successfully, \c false otherwise.
     *  \remarks This function explicitly creates all mipmap levels for all images included.
     *  Previously created levels will be released.
     *  \note The behavior is undefined if any of the images is not a power-of-two image or if
     *  any of the images has overlaps. */
    NVSG_API bool createMipmaps();

    /*! \brief Get the number of Images of the TextureImage (e.g. 6 for cubemaps).
     *  \return The number of images of the corresponding TextureImage.
     *  \sa getNumberOfMipmaps */
    NVSG_API size_t getNumberOfImages() const; 
    
    /*! \brief Get the number of mipmap levels for the specified image.
     *  \param image An optional index of the image to get the number of mipmap levels for.
     *  \return The number of mipmap levels of the \a image.
     *  \note The behavior is undefined if \a image is larger than or equal to the number of
     *  images this TextureImage holds.
     *  \sa getNumberOfImages */
    NVSG_API size_t getNumberOfMipmaps( size_t image=0 //!< Zero-based image index.
                                      ) const;

    //! Returns the PixelFormat of the specified mipmap level for a given image.
    /** The function returns the PixelFormat of the pixel data stored at mipmap level \a mipmap 
      * of the image specified by \a image. See the PixelFormat enumeration type for known pixel formats.
      * A mipmap level of 0 always identifies the image itself.
      *
      * Passing an invalid image index as well as an invalid mipmap level results in undefined behavior! 
      * A valid image index ranges from 0 to getNumberOfImages()-1.
      * A valid mipmap level ranges from 0 to getNumberOfMipmaps(image)-1.
      *
      * \returns A PixelFormat enum specifying the pixel format. */
    NVSG_API Image::PixelFormat getFormat( size_t image=0  //!< Zero-based image index.
                                         , size_t mipmap=0 //!< Zero-based mipmap level.
                                         ) const;

    //! Returns the PixelDataType of the specified image.
    /** The function returns the PixelDataType of the pixel data stored at mipmap level \a mipmap 
      * of the image specified by \a image. See the PixelDataType enumeration type for known types. 
      * A mipmap level of 0 always identifies the image itself.
      *
      * Passing an invalid image index as well as an invalid mipmap level results in undefined behavior! 
      * A valid image index ranges from 0 to getNumberOfImages()-1.
      * A valid mipmap level ranges from 0 to getNumberOfMipmaps(image)-1.
      *
      * \returns A PixelDataType enum specifying the pixel data type. */
    NVSG_API Image::PixelDataType getType( size_t image=0  //!< Zero-based image index.
                                         , size_t mipmap=0 //!< Zero-based mipmap level.
                                         ) const;

    /*! \brief Get the width of the specified image in pixels.
     *  \param image Optional index of the image to get width of. Default is zero.
     *  \param mipmap Optional index of the mipmap level of the specified image to get width of.
     *  Default is zero, that is the top level image.
     *  \return The width of the specified image in pixels.
     *  \note The behavior is undefined if \a image is larger or equal to the number of images in
     *  this TextureImage, or if \a mipmap is larger or equal to the number of mipmap levels of
     *  the specified image.
     *  \note The returned value is the width over the complete image, including overlaps.
     *  \sa getHeight, getDepth, getOverlapSize */
    NVSG_API unsigned int getWidth( size_t image=0
                                  , size_t mipmap=0
                                  ) const;

    /*! \brief Get the height of the specified image in pixels.
     *  \param image Optional index of the image to get width of. Default is zero.
     *  \param mipmap Optional index of the mipmap level of the specified image to get width of.
     *  Default is zero, that is the top level image.
     *  \return The height of the specified image in pixels.
     *  \note The behavior is undefined if \a image is larger or equal to the number of images in
     *  this TextureImage, or if \a mipmap is larger or equal to the number of mipmap levels of
     *  the specified image.
     *  \note The returned value is the height over the complete image, including overlaps.
     *  \sa getWidth, getDepth, getOverlapSize */
    NVSG_API unsigned int getHeight( size_t image=0
                                   , size_t mipmap=0
                                   ) const;

    /*! \brief Get the depth of the specified image in pixels.
     *  \param image Optional index of the image to get width of. Default is zero.
     *  \param mipmap Optional index of the mipmap level of the specified image to get width of.
     *  Default is zero, that is the top level image.
     *  \return The depth of the specified image in pixels.
     *  \note The behavior is undefined if \a image is larger or equal to the number of images in
     *  this TextureImage, or if \a mipmap is larger or equal to the number of mipmap levels of
     *  the specified image.
     *  \note The returned value is the depth over the complete image, including overlaps.
     *  \sa getWidth, getHeight, getOverlapSize */
    NVSG_API unsigned int getDepth( size_t image=0
                                  , size_t mipmap=0
                                  ) const;

    //! Returns the number of bytes per pixel of the specified mipmap.
    /** The function returns the number of bytes per pixel of the mipmap at level \a mipmap of the image 
      * specified by \a image. A mipmap level of 0 always identifies the image itself. 
      *
      * Passing an invalid image index as well as an invalid mipmap level results in undefined behavior! 
      * A valid image index ranges from 0 to getNumberOfImages()-1.
      * A valid mipmap level ranges from 0 to getNumberOfMipmaps(image)-1.
      *
      * \returns The number of bytes per pixel of the specified mipmap. */
    NVSG_API unsigned int getBytesPerPixel( size_t image=0  //!< Zero-based image index.
                                          , size_t mipmap=0 //!< Zero-based mipmap level.
                                          ) const;

    /*! \brief Get the number of bytes per line of the specified mipmap level of the specified image.
     *  \param image An optional index of the image to get information from. The default value
     *  is zero.
     *  \param mipmap An optional index of the mipmap level to get information from. The
     *  default value is zero.
     *  \return The number of bytes per line of the Image at mipmap level \a mipmap at the
     *  index \a image.
     *  \remarks A mipmap level of zero identifies the image itself.
     *  \note The behavior is undefined if \a image is larger than or equal to the number of
     *  images or if \a mipmap is larger than or equal to the number of mipmap levels of the
     *  Image \a image this TextureImageInfo holds information for.
     *  \sa getNumberOfImages, getNumberOfMipmaps, getBytesPerPixel, getBytesPerPlane,
     *  getNumberOfBytes */
    NVSG_API unsigned int getBytesPerScanLine( size_t image=0, size_t mipmap=0 ) const;
    
    /*! \brief Get the number of bytes per plane (slice) of the specified mipmap level of the specified image.
     *  \param image An optional index of the image to get information from. The default value
     *  is zero.
     *  \param mipmap An optional index of the mipmap level to get information from. The
     *  default value is zero.
     *  \return The number of bytes per plane (slice) of the Image at mipmap level \a mipmap at
     *  the index \a image.
     *  \remarks A mipmap level of zero identifies the image itself.
     *  \note The behavior is undefined if \a image is larger than or equal to the number of
     *  images or if \a mipmap is larger than or equal to the number of mipmap levels of the
     *  Image \a image this TextureImageInfo holds information for.
     *  \sa getNumberOfImages, getNumberOfMipmaps, getBytesPerPixel, getBytesPerScanLine,
     *  getNumberOfBytes */
    NVSG_API unsigned int getBytesPerPlane( size_t image=0, size_t mipmap=0 ) const;

    /*! \brief Get the number of bytes of the specified mipmap level of the specified image.
     *  \param image An optional index of the image to get information from. The default value
     *  is zero.
     *  \param mipmap An optional index of the mipmap level to get information from. The
     *  default value is zero.
     *  \return The number of bytes of the Image at mipmap level \a mipmap at the index \a image.
     *  \remarks A mipmap level of zero identifies the image itself.
     *  \note The behavior is undefined if \a image is larger than or equal to the number of
     *  images or if \a mipmap is larger than or equal to the number of mipmap levels of the
     *  Image \a image this TextureImageInfo holds information for.
     *  \sa getNumberOfImages, getNumberOfMipmaps, getBytesPerPixel, getBytesPerScanLine,
     *  getBytesPerPlane */
    NVSG_API unsigned int getNumberOfBytes( size_t image=0, size_t mipmap=0 ) const;

    /*! \brief Returns the number of bytes used for all included images and mipmap levels.
     * \return The number of bytes used for all included images and mipmap levels. */
    NVSG_API unsigned int getTotalNumberOfBytes() const;

    /*! \brief Returns the TextureImage's corresponding texture target.
     \return The function returns the TextureImage's corresponding texture target. 
     Valid targets are TEXTURE_1D, TEXTURE_2D, TEXTURE_3D, and TEXTURE_CUBE. 
     If no images are specified for the TextureImage, or if no valid target can be
     derived from the number and dimension of the currently contained images, the
     function returns TEXTURE_UNSPECIFIED_TARGET.
     \remarks The corresponding texture target is determined by the number and dimension
     of the containing images. Valid targets are TEXTURE_1D, TEXTURE_2D, TEXTURE_3D, 
     and TEXTURE_CUBE. */
    NVSG_API unsigned int getTextureTarget() const;

    //! Returns a pointer to the raw pixel data of the specified mipmap.
    /** The function returns a pointer to the raw pixel data of the mipmap at level \a mipmap of the image
      * specified by \a image. A mipmap level of 0 always identifies the image itself. 
      *
      * Passing an invalid image index as well as an invalid mipmap level results in undefined behavior! 
      * A valid image index ranges from 0 to getNumberOfImages()-1.
      * A valid mipmap level ranges from 0 to getNumberOfMipmaps(image)-1.
      *
      * \returns A pointer to the raw pixel data of the specified mipmap. */
    NVSG_API const void * getPixels( size_t image = 0  //!< Zero-based image index.
                                   , size_t mipmap = 0 //!< Zero-based mipmap level.
                                   ) const;
    
    /*! \brief getSubImagePixels copies a portion of an existing 1-, 2-, or 3-dimensional image.
      * \param 
      * mimap Zero-based mipmap level
      * \param 
      * x_offset Pixel offset in the x direction of the image
      * \param 
      * y_offset Pixel offset in the y direction of the image
      * \param 
      * z_offset Pixel offset in the z direction of the image
      * \param
      * width Width of the sub image
      * \param
      * height Height of the sub image
      * \param
      * depth Depth of the sub image
      * \return The function returns \c true on success. 
      * Otherwise it returns \c false (for example if the image does not exist (e.g. after upload),
      * or if the sub image boundary exceeds the existing image)
      * \remarks
      * The function copies \a width x \a height x \a depth x getBytesPerPixel() raw pixel data from
      * the specified offset of the existing image to the buffer pointed to by \a subPixels.
      * \n \n
      * The user is responsible for providing a buffer large enough to hold the requested data.
      * \sa setSubImagePixels
      */
    NVSG_API bool getSubImagePixels( size_t image
                                   , size_t mipmap
                                   , unsigned int x_offset
                                   , unsigned int y_offset
                                   , unsigned int z_offset
                                   , unsigned int width
                                   , unsigned int height
                                   , unsigned int depth
                                   , void * subPixels
                                   ) const;

    /*! \brief setSubImagePixels sets a portion of an existing 1-, 2-, or 3-dimensional image.
     * \param 
     * mimap Zero-based mipmap level
     * \param 
     * x_offset Pixel offset in the x direction of the image
     * \param 
     * y_offset Pixel offset in the y direction of the image
     * \param 
     * z_offset Pixel offset in the z direction of the image
     * \param
     * width Width of the sub image
     * \param
     * height Height of the sub image
     * \param
     * depth Depth of the sub image
     * \return The function returns \c true on success. 
     * Otherwise it returns \c false (for example if the image does not exist (e.g. after upload),
     * or if the sub image boundary exceeds the existing image)
     * \remarks
     * The function sets \a width x \a height x \a depth x getBytesPerPixel() pixel data from
     * the specified offset of the existing image to that of the raw data in the buffer pointed to by \a subPixels.
     * \n \n
     * The PixelDataType of the data pointed to by subPixels must match the PixelDataType 
     * of the existing image. Behavior would be undefined otherwise.
     * \sa getSubImagePixels
     */
    NVSG_API bool setSubImagePixels( size_t image
                                   , size_t mipmap
                                   , unsigned int x_offset
                                   , unsigned int y_offset
                                   , unsigned int z_offset
                                   , unsigned int width
                                   , unsigned int height
                                   , unsigned int depth
                                   , const void * subPixels
                                   );

    //! Returns the alignment for the start of each pixel row in memory. 
    NVSG_API unsigned int getUnpackAlignment() const;

    /*! \brief Scale the specified image
     *  \param image The index of the image to scale.
     *  \param width The desired width of the scaled image.
     *  \param height The optional height of the scaled image. The default is 1.
     *  \param depth The optional depth of the scaled image. The default is 1.
     *  \return \c true if the scaling was successful, \c false otherwise.
     *  \remarks The function scales the image specified by \a image according to the input
     *  dimensions passed. The filter to use is specified by the creation flags.
     *  \note The behavior is undefined if \a image is larger than or equal to the number of
     *  images in this TextureImage, of if that image has overlaps. */
    NVSG_API bool scale( size_t image            //!< Zero-based index of the image to scale.  
                       , unsigned int width      //!< Desired width of the scaled image.
                       , unsigned int height = 1 //!< Desired height of the scaled image.
                       , unsigned int depth = 1  //!< Desired depth of the scaled image.
                       );

    
    /*! \brief Scales containing images to power-of-two
     * \param sizeLimit 
     * Specifies the platform dependent size limit that must not be exceeded during scaling.
     * \return 
     * \c true if scaling was successful, \c false otherwise. 
     *
     * The function scales all containing images to become power of two in each dimension. 
     * By default, the function always scales to the nearest power of two. 
     * This behavior can be changed by specifying a certain creation flag at TextureImage 
     * instantiation. If you specified F_SCALE_POT_BELOW, the single dimensions will always 
     * be scaled down to the highest power of two below the actual size. If you specified 
     * F_SCALE_POT_ABOVE, the single dimensions always will be scaled up to the very next 
     * power of two. If both flags were specified at instantiation, the function falls back
     * to the default algorithm. Independent of the algorithm used for scaling, the function 
     * always clamps each dimension to the specified \a sizeLimit.
     * 
     * The behavior is undefined if called for a TextureImage that was instantiated with the
     * F_IMAGE_STREAM flag specified. The behavior also is undefined if called for a TextureImage
     * that does not contain any images or for which all images already have been released 
     * (e.g. after upload).
     *
     * \note 
     * If the scaling failed, the function left behind the pixel data of containing images 
     * undefined!*/
    NVSG_API bool scaleToPowerOfTwo(unsigned int sizeLimit);

    //! Returns whether the texture image represents a 1-dimensional texture image.
    /** \returns \c true if the texture image represents a 1-dimensional texture image, \c false otherwise. */
    NVSG_API bool is1D()            const;
    
    //! Returns whether the texture image represents a 2-dimensional texture image.
    /** \returns \c true if the texture image represents a 2-dimensional texture image, \c false otherwise. */
    NVSG_API bool is2D()            const;
    
    //! Returns whether the texture image represents a 3-dimensional texture image.
    /** \returns \c true if the texture image represents a 3-dimensional texture image, \c false otherwise. */
    NVSG_API bool is3D()            const;
    
    //! Returns whether the texture image represents a cube map.
    /** A cube map consists of 6 images of dimension 2.
    /** \returns \c true if the texture image represents a cube map, \c false otherwise. */
    NVSG_API bool isCubeMap()       const;
    
    //! Returns whether the texture image represents a floating point texture image.
    /** \returns \c true if the texture image represents a floating point texture image, \c false otherwise. */
    NVSG_API bool isFloatingPoint() const;

    //! Returns whether the texture image is treated as an image stream.
    /** Image streams are images for which the image data is not managed inside the TextureImage object. 
      * \returns \c true if the image is treated as a image stream, \c false otherwise. */
    NVSG_API bool isImageStream() const;

    //! Will convert a 2D image to 6 cubemap faces.
    /** The 2D image must contain all faces in the vertical cross format,
      * as it can be exported e.g. from HDRShop.
      * Thus, the height of the 2D image must be 4/3 of the width. 
      * This function does not work with image streams. */    
    NVSG_API void convert2DToCubeMap();

    //! Forces a re-upload of the texture image.
    /** The function marks the texture image as outdated, and hence, forces a re-upload with the
      * very next render step.
      *
      * The function in particular is useful for stream images, that is - images for which the image data
      * is not managed inside the TextureImage object. For these kind of images the TextureImage object
      * does not automatically realize any changes made to the image data but needs a hint from the user.
      * Whenever the image data have changed and, as a consequence, a re-upload of the texture image is
      * required, the user should call demandUpload to force the re-upload with the very next render step.
      *
      * Note, that a re-upload of the texture image will not allocate a new platform dependent texture
      * resource object but will reuse the former resource. */
    NVSG_API void demandUpload();

    //! Returns whether a re-upload of the texture image is required.
    /** \returns \c true if a re-upload is required, \c false otherwise. */ 
    NVSG_API bool isUploadDemanded() const;

    //! Finishes an image upload.
    /** The function should be called after the image upload succeed. It marks the image as being uploaded. 
      * If neither the F_PRESERVE_IMAGE_DATA_AFTER_UPLOAD nor the F_IMAGE_STREAM flag was specified
      * at TextureImage object instantiation, the function releases all internal image data. 
      * \note A re-upload is not possible after the internal image data was released. */
    NVSG_API void finishUpload();

    //! Returns the creation flag status of the TextureImage object.
    /** \returns An unsigned integer value representing the actual creation flag status of this object. */
    NVSG_API unsigned int getCreationFlags() const;

    //! Assigns new contents to a TextureImage object.
    /** The contents of the object specified by \a rhs will be assigned to this object. */
    NVSG_API TextureImage& operator=(const TextureImage& rhs);

    //! Mirror the image over the x-axis
    /** This is equivalent to reversing the rows, thus converting between origin upper left and lower left. */
    NVSG_API void mirrorX( size_t image            //!< Zero-based index of the image to mirror.
      );

    //! Mirror the image over the y-axis
    /** This is equivalent to reversing the columns. */
    NVSG_API void mirrorY( size_t image            //!< Zero-based index of the image to mirror.
      );

  private:


    // internal flags
    enum
    {
      F_UPLOAD_DEMANDED = BIT0
    };

    // helper with copying images. in particular pixel data
    void copyImages( const std::vector<std::vector<Image> > & srcImages );

    // mipmap management helpers
    bool createMipmaps( std::vector<Image> &images, const std::vector<const void*>& mipmaps );
    bool createMipmaps( std::vector<Image> &images );
    void releaseMipmaps();
    void releaseMipmaps( std::vector<Image> &images );

    // private helper to recalculate the total amount of image bytes 
    void recalcTotalNumberOfBytes();
    void recalcTextureTarget();
    
    //  scaling helper functions
    void filter( const Image * src, Image * dst, ScaleFilter * sf );

    unsigned int                      m_creationFlags;
    unsigned int                      m_internalFlags;
    unsigned int                      m_totalNumBytes; 
    unsigned int                      m_textureTarget;
    size_t                            m_mipmapUseCount;
    std::string                       m_filename; // if created from file
    std::vector<std::vector<Image> >  m_images;

    typedef std::map<std::string, nvsg::TextureLoader*> LoaderMap;
    static LoaderMap m_loaders;
    static TextureLoader * getLoader( const std::string & filename );
  };

  inline const std::string& TextureImage::getFileName() const
  {
    return m_filename;
  }

  inline size_t TextureImage::getMipmapUseCount() const
  {
    return m_mipmapUseCount;
  }

  inline bool TextureImage::isUploadDemanded() const
  {
    return m_internalFlags & F_UPLOAD_DEMANDED;
  }

  inline unsigned int TextureImage::getCreationFlags() const
  {
    return m_creationFlags;
  }

  inline size_t TextureImage::getNumberOfImages() const
  {
    NVSG_TRACE();
    return m_images.size();
  }

  inline size_t TextureImage::getNumberOfMipmaps(size_t image) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( 0 < m_images[image].size() ) );
    return( m_images[image].size() - 1 );
  }

  inline Image::PixelFormat TextureImage::getFormat(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_format );
  }

  inline Image::PixelDataType TextureImage::getType(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_type );
  }

  inline unsigned int TextureImage::getWidth(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_width );
  }

  inline unsigned int TextureImage::getHeight(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_height );
  }

  inline unsigned int TextureImage::getDepth(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_depth );
  }

  inline unsigned int TextureImage::getBytesPerPixel(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_bpp );
  }

  inline unsigned int TextureImage::getBytesPerScanLine(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_bpl );
  }

  inline unsigned int TextureImage::getBytesPerPlane(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_bps );
  }

  inline unsigned int TextureImage::getNumberOfBytes(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_nob );
  }

  inline unsigned int TextureImage::getTotalNumberOfBytes() const
  {
    NVSG_TRACE();
#if !defined(NDEBUG)
    unsigned int dbgTotalNumBytes = 0;
    for ( size_t i=0; i<m_images.size(); ++i )
    {
      for ( size_t j=0; j<m_images[i].size(); ++j )
      {
        dbgTotalNumBytes += m_images[i][j].m_nob;
      }
    }
    NVSG_ASSERT(dbgTotalNumBytes==m_totalNumBytes);
#endif
    return m_totalNumBytes;
  }

  inline unsigned int TextureImage::getTextureTarget() const 
  {
    NVSG_TRACE();
    NVSG_ASSERT((m_textureTarget==TEXTURE_CUBE)==isCubeMap()); 
    NVSG_ASSERT((m_textureTarget==TEXTURE_2D)==(is2D() && !isCubeMap())); 
    NVSG_ASSERT((m_textureTarget==TEXTURE_3D)==is3D()); 
    NVSG_ASSERT((m_textureTarget==TEXTURE_1D)==is1D()); 
    return m_textureTarget;
  }

  inline const void * TextureImage::getPixels(size_t image, size_t mipmap) const
  {
    NVSG_TRACE();
    NVSG_ASSERT( ( image < m_images.size() ) && ( mipmap < m_images[image].size() ) );
    return( m_images[image][mipmap].m_pixels );
  }

  inline unsigned int TextureImage::getUnpackAlignment() const
  {
    NVSG_TRACE();
    return getBytesPerPixel()==4 ? 4 : 1;
  }

  inline bool TextureImage::is1D() const
  {
    NVSG_TRACE();
    return (  1==m_images.size() 
            && 1==m_images[0][0].m_depth 
            && 1==m_images[0][0].m_height
            && 1<m_images[0][0].m_width );
  }

  inline bool TextureImage::is2D() const
  {
    NVSG_TRACE();
    return (  1==m_images.size() 
            && 1==m_images[0][0].m_depth 
            && 1<m_images[0][0].m_width
            && 1<m_images[0][0].m_height );
  }

  inline bool TextureImage::is3D() const
  {
    NVSG_TRACE();
    return (  1==m_images.size() 
            && 1<m_images[0][0].m_depth
            && 1<m_images[0][0].m_width
            && 1<m_images[0][0].m_height );
  }

  inline bool TextureImage::isCubeMap() const
  {
    NVSG_TRACE();
    return (  6==m_images.size() 
            && 1==m_images[0][0].m_depth
            && 1<m_images[0][0].m_width
            && 1<m_images[0][0].m_height );
  }

  inline bool TextureImage::isImageStream() const
  {
    NVSG_TRACE();
    return !!(m_creationFlags & F_IMAGE_STREAM);
  }

  inline void TextureImage::demandUpload()
  {
    NVSG_TRACE();

    m_internalFlags |= F_UPLOAD_DEMANDED;
  }

  //! Scale Filter Functor
  /** Functor base class used in texture image scaling.\n
    * When scaling a TextureImage, a ScaleFilter is used to determine the weight for a source texel in a given
    * distance to the destination texel.\n
    * With these ScaleFilters, the image is resampled in two passes, stretching or shrinking the image first
    * horizontally and then vertically (or vice versa) with potentially different scale factors. The two-pass
    * approach has a significantly lower run-time cost, \b O(imageWidth*imageHeight*(filterWidth+filterHeight)),
    * than straightforward 2-D filtering, \b O(imageWidth*imageHeight*filterWidth*filterHeight)). The performance
    * gain is even higher in 3-D filtering.\n
    * For magnification, the ScaleFilter is centered around each source texel and scaled to the height of that
    * texel. For each destination texel, the corresponding location in the source image is calculated. The sum
    * of the weighted filter functions at this point determines the value of the destination texel.\n
    * For minification, to eliminate all frequency components above the resampling Nyquist frequency, the
    * ScaleFilter is stretched by the image reduction factor.\n
    * Because the scaling process for each row or column is identical, the placement and area of the ScaleFilter
    * is fixed. Therefore the contributors to each destination texel and the corresponding filter weight are
    * precomputed once, using the calling operator, and then applied to each destination texel. */
  class ScaleFilter : public nvutil::RCObject
  {
    public:
      //! Get the support width of this scale filter.
      /** \return the maximal distance between a source and a destination texel, that is used to determine
      * the source texels that contribute to the destination texel. */
      NVSG_API virtual float getSupportWidth( void ) const = 0;

      //! Weight calculation operator.
      /** \return the weight for a source texel that is \a t units from the destination texel to compute. */
      NVSG_API virtual float operator()( float t         //!< distance between source and destination texel
                                       ) const = 0;
  };

  //! ScaleFilter functor providing box filter functionality
  /** A BoxFilter has a support width of 0.5. For distances between source and destination texel
    * of up to 0.5, the weight is 1.0, otherwise 0.0. */
  class BoxFilter : public ScaleFilter
  {
    public:
      //! Get the support width of the BoxFilter.
      /** \return the support width of the BoxFilter is 0.5. */
      NVSG_API float getSupportWidth( void ) const;

      //! Weight calculation operator.
      /** \return 1.0, if \a t is less than or equal to 0.5, otherwise 0.0. */
      NVSG_API float operator()( float t ) const;
  };

  inline float BoxFilter::getSupportWidth( void ) const
  {
    return( 0.5f );
  }

  inline float BoxFilter::operator()( float t ) const
  {
    NVSG_ASSERT ( 0.0f <= t );
    return( ( t <= 0.5f ) ? 1.0f : 0.0f );
  }

  //! ScaleFilter functor providing triangle filter functionality
  /** A TriangleFilter has a support width of 1.0. For distances \a t between source and destination
    * texel of up to 1.0, the weight is <tt>( 1.0 - t )</tt>, otherwise 0.0. */
  class TriangleFilter : public ScaleFilter
  {
    public:
      //! Get the support width of the TriangleFilter.
      /** \return the support width of the TriangleFilter is 1.0. */
      NVSG_API float getSupportWidth( void ) const;

      //! Weight calculation operator.
      /** \return <tt>( 1.0 - t )</tt>, if \a t is less than or equal to 1.0, otherwise 0.0. */
      NVSG_API float operator()( float t ) const;
  };

  inline float TriangleFilter::getSupportWidth( void ) const
  {
    return( 1.0f );
  }

  inline float TriangleFilter::operator()( float t ) const
  {
    NVSG_ASSERT ( 0.0f <= t );
    return( ( t <= 1.0f ) ? 1.0f - t : 0.0f );
  }

  //! ScaleFilter functor providing bell filter functionality
  /** A BellFilter has a support width of 1.5. For distances \a t between source and destination
    * texel in the half open interval [0.0,0.5), the weight is <tt>( 0.75 - t^2 )</tt>, if \a t
    * is in the half open interval [0.5,1.5), the weight is <tt>( 0.5 * ( 1.5 - t )^2 )</tt>,
    * otherwise 0.0. */
  class BellFilter : public ScaleFilter
  {
    public:
      //! Get the support width of the BellFilter.
      /** \return the support width of the BellFilter is 1.5. */
      NVSG_API float getSupportWidth( void ) const;

      //! Weight calculation operator.
      /** \return <tt>( 0.75 - t^2 )</tt>, if \a t is in the interval [0.0,0.5),
        * <tt>( 0.5 * ( 1.5 - \a t )^2 )</tt> if \a t is in the interval [0.5,1.5), otherwise 0.0. */
      NVSG_API float operator()( float t ) const;
  };

  inline float BellFilter::getSupportWidth( void ) const
  {
    return( 1.5f );
  }

  //! ScaleFilter functor providing BSpline filter functionality
  /** A BSplineFilter has a support width of 2.0. For distances \a t between source and destination
    * texel in the half open interval [0.0,1.0), the weight is <tt>( 2.0/3.0 - t^2 + 0.5 * t^3 )</tt>,
    * if \a t is in the half open interval [1.0,2.0), the weight is <tt>( 1.0/6.0 * ( 2.0 - t )^3 )</tt>,
    * otherwise 0.0. */
  class BSplineFilter : public ScaleFilter
  {
    public:
      //! Get the support width of the BSplineFilter.
      /** \return the support width of the BSplineFilter is 2.0. */
      NVSG_API float getSupportWidth( void ) const;

      //! Weight calculation operator.
      /** \return <tt>( 2.0/3.0 - t^2 + 0.5 * t^3 )</tt>, if \a t is in the interval [0.0,1.0),
        * <tt>( 1.0/6.0 * ( 2.0 - t )^3 )</tt> if \a t is in the interval [1.0,2.0), otherwise 0.0. */
      NVSG_API float operator()( float t ) const;
  };

  inline float BSplineFilter::getSupportWidth( void ) const
  {
    return( 2.0f );
  }

  //! ScaleFilter functor providing lanczos3 filter functionality
  /** A Lanczos3Filter has a support width of 3.0. For distances \a t between source and destination
    * texel in the half open interval [0.0,3.0), the weight is <tt>( sinc(x) * sinc(x/3.0) )</tt>,
    * with <tt>x = t * PI</tt>, otherwise 0.0. */
  class Lanczos3Filter : public ScaleFilter
  {
    public:
      //! Get the support width of the Lanczos3Filter.
      /** \return the support width of the Lanczos3Filter is 3.0. */
      NVSG_API float getSupportWidth( void ) const;

      //! Weight calculation operator.
      /** \return <tt>( sinc(x) * sinc(x/3.0) )</tt>, with <tt>x=t * PI</tt>, if \a t is in the
        * interval [0.0,3.0), otherwise 0.0. */
      NVSG_API float operator()( float t ) const;

    private:
      float sinc( float x ) const;
  };

  inline float Lanczos3Filter::getSupportWidth( void ) const
  {
    return( 3.0f );
  }

  inline float Lanczos3Filter::operator()( float t ) const
  {
    NVSG_ASSERT ( 0.0f <= t );
    return( ( t < 3.0f ) ? sinc(t) * sinc(t/3.0f) : 0.0f );
  }

  inline float Lanczos3Filter::sinc( float x ) const
  {
    x *= nvmath::PI;
    return( ( FLT_EPSILON < x ) ? sin(x) / x : 1.0f );
  }
} //  namespace nvsg
