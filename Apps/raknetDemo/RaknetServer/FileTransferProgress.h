#ifndef FILETRANSFERPROGRESS_H_INCLUDED
#define FILETRANSFERPROGRESS_H_INCLUDED
#include "RakPeerInterface.h"
#include "FileListTransfer.h"
#include "FileListTransferCBInterface.h"

class FiletransferProgress : public RakNet::FileListProgress
{
	virtual void OnFilePush(const char *fileName, unsigned int fileLengthBytes, unsigned int offset, unsigned int bytesBeingSent, bool done, RakNet::SystemAddress targetSystem)
	{
		printf("Sending %s bytes=%i offset=%i\n", fileName, bytesBeingSent, offset);
	}
} testFileListProgress;

#endif // FILETRANSFERPROGRESS_H_INCLUDED
