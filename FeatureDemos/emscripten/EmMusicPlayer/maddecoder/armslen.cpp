/****************************************************************************
 * armslen                                            (c) 2004 J. A. Robson
 * 
 * Creates a named pipe '\\.\pipe\madArmsLen', and once a client process 
 * (i.e. armslen_test.cpp) accesses the pipe, begins converting the given
 * file from  MP3 using madlldlib (and thus libmad), and passes the encoding
 * information as a formatted stream to the pipe. The formatting is simple--
 * a comma-delimited string with name/value pairs:
 *
 * 		fc			Frame count
 * 		tb			Total bytes converted
 * 		(MP3 related):
 * 		lyr			MP3 layer
 * 		md			Mode
 * 		mdx			Mode extension
 * 		emp			Emphasis
 *		br			Bitrate	
 *		sr			Samplerate
 *
 * The client application can access the pipe, read and parse out the above 
 * data, and then use it to update status indicators.
 * 
 * This is provided as a simple example in case you do not want to include 
 * madlldlib as a DLL, but still need to update a display in your 
 * application. To test this example, armslen_test.cpp must also be compiled.
 * After compilation, first run armslen.exe, then, in a different console,
 * run armslen_test.exe.
 ****************************************************************************/

#include <windows.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <madlldlib.h>


char *pipe_srv = "\\\\.\\pipe\\madArmsLen";
HANDLE ha;
int twentyfour = 24;
unsigned long pbuffsz = 256+twentyfour;
bool tf;



/*
 * The callback function is used for reporting purposes and
 * garnering information about the source MP3 file. This callback 
 * simply prints information to STDOUT, but it usually would be 
 * used to increment a counter, calculate a percentage, or advance
 * a status bar.
 */
void __stdcall mycb (unsigned long fcnt, unsigned long bcnt, struct mad_header *mhdr) {

	char pmsg[256];
	unsigned long bamt = 0;
	
	/*
	 * If this is the first iteration (frame count is one)
	 * then print out the MP3 layer information. Using this logic
	 * one can retrieve information about the MP3 file, such as 
	 * channels, layer, etc., that might be useful to the calling
	 * code.
	 */	
	if (fcnt == 1) {
		

		sprintf(pmsg, "fc=%d,tb=%d,lyr=%d,md=%d,mdx=%d,emp=%d,br=%d,sr=%d",
				fcnt,
				bcnt,
				mhdr->layer, 
				mhdr->mode,
				mhdr->mode_extension,
				mhdr->emphasis,
				mhdr->bitrate,
				mhdr->samplerate);


		if (WriteFile(ha, &pmsg, 256, &bamt, NULL) == 0) {
		
			printf("WriteFile failed: err=%d\n", GetLastError());
			return;		
		
		}


	}
	else {
		
		sprintf(pmsg, "fc=%d,tb=%d", fcnt, bcnt);

		if (WriteFile(ha, &pmsg, 256, &bamt, NULL) == 0) {
		
			printf("WriteFile failed: err=%d\n", GetLastError());
			return;		
		
		}		


	}
	
}

/* 
 * Program entry point
 * note: argv error checking is minimal (to say the least) 
 */
int main(int argc, char *argv[]) {


	/* status/error reporting */
	char statmsg[256];
	int status;
	
	
	/* if not the exact number of command 
	 * line parameters, quit */
	if (argc != 4) {
		printf("usage: %s 'input.mp3' 'output file' 'pcm|wav'\n", argv[0]);
		return 1;
	}

	
	
	/* Create named pipe server (for
	 * write only) */
	
	ha = CreateNamedPipe(pipe_srv,
        PIPE_ACCESS_OUTBOUND,
        PIPE_TYPE_BYTE,
        2,              // two connections
        pbuffsz,              // input buffer size
        pbuffsz,              // output buffer size
        INFINITE,       // timeout
        NULL);          // security



	if (ha == INVALID_HANDLE_VALUE) 
	{
    	printf("CreateNamedPipe() FAILED: err=%d\n", 
				GetLastError());
		return 1;
		
  	}
	
	
	/* Do not execute conversion until
	 * client has attached */
	
	printf("Pipe %s waiting for client...\n", pipe_srv);	
	tf = ConnectNamedPipe(ha, NULL);

	
	/* call the decoding function with input file,
	 * output file, and callback (defined above). 
	 *
	 * Note on return values: CbMpegAudioDecoder() 
	 * returns 0 if successful, non-zero if it has 
	 * problems opening/reading the files passed. 
	 * libmad, the library concerned with the actual 
	 * MP3 decoding, passes detailed error information 
	 * specific to the MP3 conversion, which gets 
	 * set into the string statmsg. 
	 * (CbMpegAudioDecoder() returns a non-zero 
	 * status at this time as well.) So the upshot 
	 * of this is that both indicators handle 
	 * slightly overlapping but different events,
	 * and are hence both useful. */
	
	if (strcmp(argv[3], "pcm")==0) {		/* output raw PCM */
		status = CbMpegAudioDecoder(argv[1], argv[2], 0, statmsg, mycb);
	}
	else {									/* output as WAV */
		status = CbMpegAudioDecoder(argv[1], argv[2], 1, statmsg, mycb);
	}


	/* relay any errors */	
	if (statmsg)
		printf("%s", statmsg);		

	return 0;	

}
