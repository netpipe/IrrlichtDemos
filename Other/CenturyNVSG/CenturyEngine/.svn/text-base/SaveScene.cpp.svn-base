
#include <stdio.h>
#include <nvsg/CoreTypes.h>
#include <nvsg/nvsgapi.h> // NVSG storage-class defines
#include <nvsg/PlugInterface.h>
#include <nvsg/PlugInterfaceID.h>
#include <nvutil/PlugIn.h>
#include <nvutil/Tools.h>
#include <nvutil/IOLayer.h>
#include <nvutil/Handle.h>
#include <nvsg/ErrorHandling.h>
#include <nvgl/nvgl.h>
#include <nvgl/GLContext.h>
#include <nvsg/ErrorHandling.h>
#include <nvsg/Scene.h>
#include <nvsg/TextureImage.h>
#include <nvutil/RCObject.h>

#include <string>
#include <sstream>
#include <vector>

#include <nvutil/DbgNew.h> // this must be the last include

using namespace std;
using namespace nvgl;
using namespace nvsg;
using namespace nvutil;

// define a unique plug-interface ID for SceneLoader 
const UPITID PITID_SCENE_SAVER(UPITID_SCENE_SAVER, UPITID_VERSION);

//
// Save the scene in a format that NVSG understands
// 
bool 
DumpScene( const string & filename, SceneHandle * scene, 
                                         ViewStateHandle * viewState )
{
  printf( "Dumping Scene as: %s\n", filename.c_str() );

  string modulePath, dir;
  vector<string> searchPaths;
  GetCurrentDir(dir);  
  searchPaths.push_back(dir);

  GetModulePath(modulePath);
  GetDirFromPath(modulePath, dir);
  searchPaths.push_back(dir);

  //
  // extract file extension
  //
  string ext;
  GetFileExtFromPath(filename, ext);

  UPIID piid = UPIID(ext.c_str(), PITID_SCENE_SAVER);

  PlugIn * plug;

  if ( getInterface( searchPaths, piid, plug ) )
  {
    SceneSaver *ss = reinterpret_cast<SceneSaver *>(plug);
    bool retval;
#ifndef _DEBUG
    try
#endif
    {
      retval = ss->save( scene, viewState, filename );
    }
#ifndef _DEBUG
    catch(const NVSGException& e) // catch unexpected NVSG errors
    {
      printf( "DumpFile: NVSG Critical Error: %s\n", 
                                                e.getErrorMessage().c_str() );
      releaseInterface(piid);
      return false; // cannot proceed
    }
    catch(...) // catch all others
    { 
      printf("DumpFile: An unknown error occurred! Cannot proceed!\n");
      releaseInterface(piid);
      return false; // cannot proceed
    }
#endif     
  }
  else
  {
    printf("Unable to find scene saver!!\n");
  }

  releaseInterface(piid);

  return true;
}

//
// Save a screen shot in a format that NVSG understands
// 
bool 
DumpScreen( const string & filename, int width, int height )
{
  printf( "Dumping Screenshot as: %s\n", filename.c_str() );

  //
  // Read data from front buffer
  //
  GLPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  GLPixelStorei(GL_PACK_ALIGNMENT, 1);
  GLPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  GLPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  GLPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  // assume this is set already - as we don't want to mess with it
  //GLViewport(0, 0, win_x, win_y);
  int win_x = width;
  int win_y = height;

  GLRasterPos2f(0,0);
  void *image = malloc(win_x*win_y*3);
  GLReadBuffer( GL_FRONT );
  GLReadPixels(0, 0, win_x, win_y, GL_BGR, GL_UNSIGNED_BYTE, image);
  GLReadBuffer( GL_BACK );

  //
  // extract file extension
  //
  string ext;
  GetFileExtFromPath(filename, ext);

  UPIID piid = UPIID(ext.c_str(), 
                       UPITID(UPITID_TEXTURE_SAVER, UPITID_VERSION) );

  PlugIn * plug;

  string modulePath, dir;
  vector<string> searchPaths;
  GetCurrentDir(dir);  
  searchPaths.push_back(dir);

  GetModulePath(modulePath);
  GetDirFromPath(modulePath, dir);
  searchPaths.push_back(dir);

  if ( getInterface( searchPaths, piid, plug ) )
  {
    TextureSaver *ss = reinterpret_cast<TextureSaver *>(plug);
    bool retval;
#ifndef _DEBUG
    try
#endif
    {
      // convert image into TextureImage
      RCObject_AutoPtr<TextureImageHandle> 
                              hTi( CreateHandle( TextureImageHandle ) );

      // create the image from the data
      {
        WritableObject< TextureImage > ti( hTi.get() );

        Image::PixelFormat format = Image::IMG_BGR;
        Image::PixelDataType type = Image::IMG_UNSIGNED_BYTE;

        int depth = 1;
        size_t size = ti->createImage ( win_x, win_y, depth,
                                        format, type, image );
        free( image );
      }

      retval = ss->save( hTi.get(), filename );

      releaseInterface(piid);
      return retval;
    }
#ifndef _DEBUG
    catch(const NVSGException& e) // catch unexpected NVSG errors
    {
      printf( "ScreenShot: NVSG Critical Error: %s\n", e.getErrorMessage().c_str() );
      releaseInterface(piid);
      free(image);
      return false; // cannot proceed
    }
    catch(...) // catch all others
    { 
      printf("ScreenShot: An unknown error occurred! Cannot proceed!\n");
      releaseInterface(piid);
      free(image);
      return false; // cannot proceed
    }
#endif     
  }
  else
  {
    free( image );
    printf( "Error: Unable to find TextureSaver interface!!\n" );
    releaseInterface(piid);

    return false;
  }
}
