// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef OV_CALLBACKS_H
#define OV_CALLBACKS_H

#include "compile_config.h"

#ifdef HC1_ENABLE_OGG

#include "ogg/ogg.h"
#ifdef _IRR_ANDROID_PLATFORM_	// using tremor
	#include "ivorbiscodec.h"
	#include "ivorbisfile.h"
#else
	#include "vorbis/codec.h"
	#include "vorbis/vorbisenc.h"
	#include "vorbis/vorbisfile.h"
#endif

extern size_t cb_ov_irr_read(void *, size_t, size_t, void *);
extern int    cb_ov_irr_seek(void *, ogg_int64_t, int);
extern int    cb_ov_irr_close(void *);
extern long   cb_ov_irr_tell(void *);

extern ov_callbacks ov_callback_irr_fs;

#ifdef _IRR_ANDROID_PLATFORM_	// using tremor
// Just some stuff which is usually in vorbisfile.h but missing in ivorbisfile in tremor.
#ifndef OV_EXCLUDE_STATIC_CALLBACKS

/* a few sets of convenient callbacks, especially for use under
 * Windows where ov_open_callbacks() should always be used instead of
 * ov_open() to avoid problems with incompatible crt.o version linking
 * issues. */

static int _ov_header_fseek_wrap(FILE *f,ogg_int64_t off,int whence){
  if(f==NULL)return(-1);

#ifdef __MINGW32__
  return fseeko64(f,off,whence);
#elif defined (_WIN32)
  return _fseeki64(f,off,whence);
#else
  return fseek(f,off,whence);
#endif
}

/* These structs below (OV_CALLBACKS_DEFAULT etc) are defined here as
 * static data. That means that every file which includes this header
 * will get its own copy of these structs whether it uses them or
 * not unless it #defines OV_EXCLUDE_STATIC_CALLBACKS.
 * These static symbols are essential on platforms such as Windows on
 * which several different versions of stdio support may be linked to
 * by different DLLs, and we need to be certain we know which one
 * we're using (the same one as the main application).
 */

static ov_callbacks OV_CALLBACKS_DEFAULT = {
  (size_t (*)(void *, size_t, size_t, void *))  fread,
  (int (*)(void *, ogg_int64_t, int))           _ov_header_fseek_wrap,
  (int (*)(void *))                             fclose,
  (long (*)(void *))                            ftell
};
#endif	// !OV_EXCLUDE_STATIC_CALLBACKS
#endif // _IRR_ANDROID_PLATFORM_

#endif // HC1_ENABLE_OGG

#endif // OV_CALLBACKS_H
