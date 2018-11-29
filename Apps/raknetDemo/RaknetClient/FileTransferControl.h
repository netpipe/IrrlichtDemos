#ifndef FILETRANSFERCONTROL_H_INCLUDED
#define FILETRANSFERCONTROL_H_INCLUDED
#include "RakPeerInterface.h"
#include "FileListTransfer.h"
#include "MessageIdentifiers.h"
#include "FileListTransferCBInterface.h"
#include "SuperFastHash.h"
#include <stdio.h>

class FileTransferControl : public RakNet::FileListTransferCBInterface
{
public:
	bool OnFile(
		OnFileStruct *onFileStruct)
	{
		printf("%i. (100%%) %i/%i %s %ib / %ib\n",
			onFileStruct->setID,
			onFileStruct->fileIndex+1,
			onFileStruct->numberOfFilesInThisSet,
			onFileStruct->fileName,
			onFileStruct->byteLengthOfThisFile,
			onFileStruct->byteLengthOfThisSet);
        char str[256];
        strcpy(str, onFileStruct->fileName);
        fileCopy = str;
		FILE *fp = fopen(fileCopy.C_String(), "wb");
		if(fp){

            fwrite(onFileStruct->fileData, onFileStruct->byteLengthOfThisFile, 1, fp);
		}
		else{
		printf("ERROR");
		}
		printf("File Writing\n");
		fclose(fp);
        printf("File Finished\n");


		// Return true to have RakNet delete the memory allocated to hold this file.
		// False if you hold onto the memory, and plan to delete it yourself later
		return true;
	}

	virtual void OnFileProgress(FileProgressStruct *fps)
	{
		printf("%i partCount=%i partTotal=%i (%i%%) %i/%i %s %ib / %ib\n",
			fps->onFileStruct->setID,
			fps->partCount, fps->partTotal, (int) (100.0*(double)fps->partCount/(double)fps->partTotal),
			fps->onFileStruct->fileIndex+1,
			fps->onFileStruct->numberOfFilesInThisSet,
			fps->onFileStruct->fileName,
			fps->onFileStruct->byteLengthOfThisFile,
			fps->onFileStruct->byteLengthOfThisSet,
			fps->firstDataChunk);
	}

	virtual bool OnDownloadComplete(DownloadCompleteStruct *dcs)
	{
		printf("Download complete.\n");

		// Returning false automatically deallocates the automatically allocated handler that was created by DirectoryDeltaTransfer
		return false;
	}

private:
    RakNet::RakString fileCopy;

} transferCallback;


#endif // FILETRANSFERCONTROL_H_INCLUDED
