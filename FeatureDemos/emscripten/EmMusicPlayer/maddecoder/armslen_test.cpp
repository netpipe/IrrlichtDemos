/****************************************************************************
 * armslen_test                                       (c) 2004 J. A. Robson
 * 
 * This runs in conjunction with armslen.exe. For details see armslen.cpp.
 *
 * To compile, see 'Makefile' comments.
 ****************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>


char *pipe_srv = "\\\\.\\pipe\\madArmsLen";
HANDLE hf;
int twentyfour = 24;
unsigned long pbuffsz = 256+twentyfour;
bool tf;


/* 
 * Program entry point
 * note: argv error checking is minimal (to say the least) 
 */
void main(int argc, char *argv[]) {

	char pmsg[256];
	unsigned long bytes_read;
	bool rslt;

	printf("Testing armslen.cpp named pipe...\n");


	/* Create named pipe client (for
	 * read only) */
	
    hf = CreateFile(pipe_srv,
			GENERIC_READ,
			FILE_SHARE_READ,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
            NULL);
	if(hf == INVALID_HANDLE_VALUE) {
		printf("CreateFile() FAILED: err=%d\n", GetLastError());
        return;
    }


	/*
	 * Typically at this point you would parse the 
	 * returned string and use the data pieces to
	 * update status
	 */
	
	do {
		rslt = ReadFile(hf, &pmsg, 256, &bytes_read, NULL); 
		printf("%s\n", pmsg);
	} while( rslt && bytes_read != 0 );
	
	
	return;	

}



