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
#include "nvsg/CoreTypes.h"
#include "nvutil/PlugIn.h" // general PlugIn class definition

namespace nvsg
{
  //! Pure virtual base class for NVSG scene loader plug-ins
  /** User defined NVSG scene loader plug-ins must provide the \c SceneLoader
    * interface.
    * \par
    * Client code that needs to utilize a certain \c SceneLoader interface should
    * first query for a unique plug interface type ID through a call to \c nvutil::queryInterfaceType.
    * The unique plug interface type ID for a SceneLoader will be constructed from the \c UPITID_SCENE_SAVER
    * define and the actual NVSG version, which is coded in the UPITID_VERSION define, as shown in the code 
    * snippet below.
    * A call to \c nvutil::queryInterfaceType returns a list of all unique interface IDs (UPIIDSs)
    * found at given search paths.\n
    * After that, the client code can take the different UPIIDs to grab a scene loader interface 
    * through a call to \c nvutil::getInterface.\n
    * After usage, the client code should release the interface through a call to \c nvutil::releaseInterface:
    *
    * \code
    *   // Example:
    *   // --------
    *   // Get a scene loader interface capable to load 'nvb' files
    *   
    *   // 
    *   vector<string> searchPathes;
    *   // Add appropriate search paths here. Not relevant for the example here.
    *   // ...
    *
    *   // define the unique plug interface type ID for SceneLoaders
    *   const nvutil::UPITID PITID_SCENE_LOADER(UPITID_SCENE_LOADER, UPITID_VERSION);
    *
    *   nvsg::Scene * theScene = NULL;
    *   nvsg::ViewState * viewState = NULL;
    *   nvutil::UPIID nvbLoaderInterfaceID;
    *   bool foundAppropriate = false;
    *   vector<nvutil::UPIID> piids;
    *   if ( nvutil::queryInterfaceType(searchPathes, PITID_SCENE_LOADER, piids) )
    *   {
    *     vector<nvutil::UPIID>::iterator it = piids.begin();
    *     for ( ; it != piids.end(); ++it )
    *     {
    *       if ( !stricmp((*it).getPlugSpecificIDString(), ".nvb") )
    *       {
    *         // found, copy the ID
    *         nvbLoaderInterfaceID = *it;
    *         foundAppropriate = true;
    *         break; // look no further
    *       }
    *     }
    *     if ( foundAppropriate )
    *     {
    *       nvutil::PlugIn * plug; 
    *       if ( nvutil::getInterface(searchPathes, nvbLoaderInterfaceID, plug) )
    *       {
    *         nvsg::SceneLoader * loader = reinterpret_cast<SceneLoader*>(plug);
    *         theScene = loader->load("c:\\nvb\\sample.nvb", searchPathes, viewState);
    *         if ( theScene ) 
    *         { 
    *           theScene->addRef();
    *           if ( viewState )
    *           {
    *             viewState->addRef();
    *           }
    *         }
    *         nvutil::releaseInterface(nvbLoaderInterfaceID);
    *       }
    *     }
    *   }
    * \endcode
    */
  // SceneLoader interface
  class SceneLoader : public nvutil::PlugIn
  {
    public:
      //! Loading a scene
      /** Loads an NVSG scene from a file specified by \a filename. 
        * The function tries to look up this file as follows:
        * -# Looks at the specified location.
        * -# Looks at the current directory.
        * -# Uses the search paths stored in \a searchPaths to look up the file.
        * \returns A pointer to a SceneHandle specifying the loaded scene if successful, a null pointer otherwise.
        */
      NVSG_API virtual SceneHandle * load( const std::string& filename            //!< A string that holds the name of the scene file to be loaded.
                                         , const std::vector<std::string> &searchPaths //!< A collection of search pathes used to look up the file.
                                         , ViewStateHandle *& viewState              /*!< If the function succeeded, this points to the optional
                                                                                          ViewState stored with the scene. */
                                         ) = 0;

    protected:
      //! Protected virtual destructor
      /** Prohibits ordinary client code from  
        * - creating a \c SceneLoader derived object on stack and
        * - calling \c delete on a pointer to \c SceneLoader. 
        * \note This will not prohibit the client code from calling \c delete
        * on a pointer to a \c SceneLoader derived object. Normally this 
        * is not that critical, because one almost always uses
        * a pointer to the base class when working with \c SceneLoader plug-ins.
        * \par
        * Nevertheless, it would be far less error prone if classes that
        * inherit from \c SceneLoader also make their destructor protected.
        */
      NVSG_API virtual ~SceneLoader();
  };

  //! Pure virtual base class for NVSG scene saver plug-ins
  /** User defined NVSG scene saver plug-ins must provide the \c SceneSaver
    * interface. */
  class SceneSaver : public nvutil::PlugIn
  {
    public:
      //! Saving a scene
      /** Saves an NVSG \a scene and a \a viewState to a file specified by \a filename. 
        * \returns \c true if successful, \c false otherwise.
        */
      NVSG_API virtual bool save( nvsg::SceneHandle *scene
                                , nvsg::ViewStateHandle *viewState
                                , const std::string& filename ) = 0;

    protected:
      //! Protected virtual destructor
      /** Prohibits ordinary client code from  
        * - creating a \c SceneSaver derived object on stack and
        * - calling \c delete on a pointer to \c SceneSaver.
        * \note This will not prohibit the client code from calling \c delete
        * on a pointer to a \c SceneSaver derived object. Normally this 
        * is not that critical, because one almost always uses
        * a pointer to the base class when working with \c SceneSaver plug-ins.
        * \par
        * Nevertheless, it would be far less error prone if classes that
        * inherit from \c SceneSaver, also make their destructor protected.
        */
      NVSG_API virtual ~SceneSaver();
  };

  //! Pure virtual base class for texture loader plugins
  /** \note TextureLoader are not yet implemented as plugins. */
  class TextureLoader : public nvutil::PlugIn
  {
    public:
      /*! \brief Function to load a TextureImage.
       *  \param filename The name of the texture file to load.
       *  \param searchPaths An optional set of search paths to use for files referenced within
       *  the texture file specified by \a filename.
       *  \param creationFlags An optional set of creation flags
       *  \return A pointer to the created TextureImageHandle, or NULL if the loading failed.
       *  \remarks Creates a TextureImageHandle and calls \c onLoad() with \a filename, the
       *  created TextureImage, and \a searchPaths.
       *  \note The behavior is undefined if the file \a filename does not exist or is not of
       *  the appropriate type.
       *  \sa reload, onLoad */
      NVSG_API TextureImageHandle * load( const std::string& filename
                                        , const std::vector<std::string>& searchPaths
                                          = std::vector<std::string>()
                                        , const unsigned int& creationFlags = 0 );

      /*! \brief Function to reload a TextureImage.
       *  \param tih A pointer to the TextureImage to reload.
       *  \param searchPaths An optional set of search paths to use for files referenced within
       *  the texture file specified by \a filename.
       *  \return \c true if reloading the TextureImage was successful, otherwise \c false.
       *  \remarks Any previously loaded pixel data is released, and the file specified by
       *  \a tih is reloaded. It is assumed that the TextureImage is cleared, that is, it has no
       *  images.
       *  \sa load, onLoad */
      NVSG_API bool reload( TextureImageHandle * tih
                          , const std::vector<std::string>& searchPaths
                            = std::vector<std::string>() );

    protected:
      //! Protected virtual destructor
      /** Prohibits ordinary client code from  
        * - creating a \c TextureLoader derived object on stack and
        * - calling \c delete on a pointer to \c TextureLoader.
        * \note This will not prohibit client code from calling \c delete
        * on a pointer to a \c TextureLoader derived object. Normally this 
        * is not that critical, because one almost always uses
        * a pointer to the base class when working with \c TextureLoader plug-ins.
        * \par
        * Nevertheless, it would be far less error prone if classes that
        * inherit from \c TextureLoader also make their destructor protected.
        */
      NVSG_API virtual ~TextureLoader();

      /*! \brief Virtual function to create a TextureImageHandle.
       *  \return A pointer to a created TextureImageHandle.
       *  \remarks This function is called in load. The default implementation just creates a
       *  TextureImageHandle. A class derived from TextureLoader can decide to overwrite this
       *  function to create an object that is derived from TextureImageHandle.
       *  \sa VolLoader */
      NVSG_API virtual TextureImageHandle * createTextureImageHandle();

      /*! \brief Pure virtual interface called on loading a TextureImage file.
       *  \param filename The name of the texture file to load.
       *  \param texImg A pointer to the TextureImage to load the data into.
       *  \param searchPaths A set of search paths used for files referenced within the texture
       *  file.
       *  \return \c true, if the TextureImage was successfully loaded, otherwise \c false.
       *  \remarks Loads a TextureImage from the file specified by \a filename. The PlugIn must
       *  create the image(s) in the OpenGL compatible orientation, that has its image origin in
       *  the lower left. TextureImage offers mirrorX() and mirrorY() routines that a Loader
       *  PlugIn can use to achieve this orientation.
       *  \sa load, reload */
      NVSG_API virtual bool onLoad( const std::string & filename
                                  , nvsg::TextureImage * texImg
                                  , const std::vector<std::string> & searchPaths ) = 0;
  };

  //! Pure virtual base class for texture saver plugins
  /** \note TextureSaver are not yet implemented as plugins. */
  class TextureSaver : public nvutil::PlugIn
  {
    public:
      //! Pure virtual interface function to save a TextureImage.
      /** Saves the texture image \a image to the file given by \a filename.
        * \returns  \c true, if the TextureImage was saved successfully, otherwise \c false.
        */
      NVSG_API virtual bool save( TextureImageHandle * image
                                , const std::string & fileName ) = 0;

    protected:
      //! Protected virtual destructor
      /** Prohibits ordinary client code from  
        * - creating a \c TextureSaver derived object on stack and
        * - calling \c delete on a pointer to \c TextureSaver.
        * \note This will not prohibit client code from calling \c delete
        * on a pointer to a \c TextureSaver derived object. Normally this 
        * is not that critical, because one almost always uses
        * a pointer to the base class when working with \c TextureSaver plug-ins.
        * \par
        * Nevertheless, it would be far less error prone if classes that
        * inherit from \c TextureSaver also make their destructor protected.
        */
      NVSG_API virtual ~TextureSaver();
  };

} //  namespace nvsg
