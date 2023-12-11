
#ifndef OPENAL_STUFF_H
#define OPENAL_STUFF_H

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
#include <AL/al.h>
#include <AL/alc.h>
#endif

// this checks for openal errors
bool check_openal_error();

#endif
