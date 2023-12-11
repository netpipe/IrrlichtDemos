/****************************************************************************
 * madlldlib                                        (c) 2004 J. A. Robson   *
 *																			*
 * Definitions file. See inline comments for details. See comments in       *
 * source files 'madlldlib.cpp' and 'test.cpp' for information on           *
 * programming madlldlib. To compile, see Makefile comments.                *
 *																			*
 ****************************************************************************/

#ifndef _MADLLDLIB_H_
#define _MADLLDLIB_H_

/* program includes */
#include <mad.h> /* must include this path in compile */


/* WAV structure 
 * Used in CbMpegAudioDecoder when instructed to write a
 * WAV header to the output file.
 */
typedef struct tag_wavhdr {	
		int RIFF;                // 'R','I','F','F'                
		int size;                   // size of wave file from here on
		int WAVE;             // 'W','A','V','E'
		int fmt;			 //'f','m','t',' '
		int   wFormatLength;          // The length of the TAG format    
		short    wFormatTag;             // should be 1 for PCM type data  
		short    nChannels;              // should be 1 for MONO type data
		int   nSamplesPerSec;         // should be 11025, 22050, 44100  
		int   nAvgBytesPerSec;        // Average Data Rate              
		short    nBlockAlign;            // 1 for 8 bit data, 2 for 16 bit
		short    wBitsPerSample;         // 8 for 8 bit data, 16 for 16 bit
		int data;                // 'd','a','t','a'                        
		int   datasize;               // size of data from here on              
} wavhdr;


/* function export */

/* callback decoder function (based on MpegAudioDecoder) */
int CbMpegAudioDecoder(
		const char *InFN, 		/* input filename */
		const char *OutFN, 		/* output filename */
		int WavPad, 			/* 1=WAV file output, other = PCM */
		char *StatMsg, 			/* status/error reporting */
		/* Callback definition below 
		 * The callback is used by this function to send
		 * reporting information back to the calling code
		 * while it is decoding.
		 */
		void (*CbFunc)(unsigned long FrameCnt, unsigned long InByteTot, struct mad_header *Header)
		);


#endif




