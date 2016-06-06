#ifndef _SKIN_LOADER_H_
#define _SKIN_LOADER_H_

#include "CImageGUISkin.h"
#include "irrString.h"

class CConfigMap;

namespace irr
{
namespace io
{
    class IFileSystem;
}
namespace video
{
    class IVideoDriver;
}

core::stringc GetContextPathFromFilename( const core::stringc& filename );

//! Loads a .cfg file and stores all the entries in a config map that is
//! returned. Returns NULL if the file could not be opened. You must delete the
//! config map when you are done using it.
CConfigMap* LoadConfigFromFile( io::IFileSystem* fileSystem, const core::stringc& filename );

//! Loads a skin configuration from a file. Texture filenames will be treated
//! relative to the filename's path.
gui::SImageGUISkinConfig LoadGUISkinFromFile( io::IFileSystem* fileSystem, video::IVideoDriver* driver, const core::stringc& filename );


}

#endif

