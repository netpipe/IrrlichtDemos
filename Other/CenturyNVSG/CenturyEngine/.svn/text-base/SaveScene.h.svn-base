
#pragma once

#include <string>
#include <nvutil/Handle.h>

namespace nvsg
{
  class Scene;
  class ViewState;
}

//
// Save the scene in a format that NVSG understands
// 
bool 
DumpScene( const std::string & filename, nvsg::SceneHandle * scene, 
                                         nvsg::ViewStateHandle * viewState );
//
// Save a screen shot in a format that NVSG understands
// 
bool 
DumpScreen( const std::string & filename, int width, int height );
