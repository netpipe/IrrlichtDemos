#ifndef DEFAULT_FILE_H
#define DEFAULT_FILE_H


#include "audiere.h"
#include "internal.h"


namespace audiere {

  ADR_EXPORT(File*) AdrOpenFile(const char* filename, bool writeable);

}


#endif
