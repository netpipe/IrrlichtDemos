// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "ov_callbacks.h"

#ifdef HC1_ENABLE_OGG
#include <irrlicht.h>

ov_callbacks ov_callback_irr_fs =
{
  (size_t (*)(void *, size_t, size_t, void *))  cb_ov_irr_read,
  (int (*)(void *, ogg_int64_t, int))           cb_ov_irr_seek,
  (int (*)(void *))                             cb_ov_irr_close,
  (long (*)(void *))                            cb_ov_irr_tell
};


size_t cb_ov_irr_read(void * data, size_t size, size_t count, void * file)
{
	irr::io::IReadFile* irrFile = static_cast<irr::io::IReadFile*>(file);
	return (size_t)irrFile->read(data, size*count);
}

int cb_ov_irr_seek(void * file, ogg_int64_t offset, int whence)
{
	irr::io::IReadFile* irrFile = static_cast<irr::io::IReadFile*>(file);

	switch ( whence )
	{
		case SEEK_SET:
			return (int) irrFile->seek((long)offset, false);
		case SEEK_CUR:
			return (int) irrFile->seek((long)offset, true);
		case SEEK_END:
			return (int) (irrFile->getSize()-(long)offset);
	}
	return 0;
}

int cb_ov_irr_close(void * file)
{
	irr::io::IReadFile* irrFile = static_cast<irr::io::IReadFile*>(file);
	irrFile->drop();
	return 0;
}

long cb_ov_irr_tell(void * file)
{
	irr::io::IReadFile* irrFile = static_cast<irr::io::IReadFile*>(file);
	return (long)irrFile->getPos();
}

#endif // HC1_ENABLE_OGG

