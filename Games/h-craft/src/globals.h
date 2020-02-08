// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef GLOBALS_H
#define GLOBALS_H

#include "compile_config.h"

#define MAX_PLAYERS 4
#define MAX_SPAWNS 5
#define MAX_BOT_SETTINGS 12
#define TIME_START_COUNTDOWN 3000
#define MAX_RECORD_SIZE 3000000
#define MAX_RIVALS 4

#ifndef MIN
  #define MIN(x,y) (((x)<(y))?(x):(y))
#endif
#ifndef MAX
  #define MAX(x,y) (((x)>(y))?(x):(y))
#endif

#define swprintf_hc swprintf

#ifdef __GNUWIN32__

#undef swprintf_hc
#define swprintf_hc   _snwprintf

#endif  // __GNUWIN32__


#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif // MAXPATHLEN

// Groups got simplified when the profiler got added to Irrlicht, so defines are a little strange now.
#define PROFILE_ADD_GROUP(groupId_, name_)

#if defined(HC1_ENABLE_PROFILE) && (IRR_REV >= 3)
    #define HC1_PROFILE(X) X
    #define PROFILE_ADD(id_, group_, name_) getProfiler().add(id_, name_, #group_)
    #define PROFILE_START(id_) 				getProfiler().start(id_)
    #define PROFILE_STOP(id_) 				getProfiler().stop(id_)
#else
    #define HC1_PROFILE(X)
    #define PROFILE_ADD(id_, group_, name_)
    #define PROFILE_START(id_)
    #define PROFILE_STOP(id_)
#endif

#endif // GLOBALS_H
