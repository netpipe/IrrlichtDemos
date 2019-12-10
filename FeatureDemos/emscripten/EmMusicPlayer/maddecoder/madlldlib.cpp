/****************************************************************************
 * madlldlib                                        (c) 2004 J. A. Robson   *
 *                                                                          *
 * madlldlib.cpp -- a library based on madlld that creates a DLL with an    *
 * easy interface--a single function with a callback. (Also changed output  * 
 * bit order to be little-endian.)                                          *
 *                                                                          *
 * How to use:                                                              *
 * 	Define a callback function matching the parameters defined in the       *
 * 	CbMpegAudioDecoder() parameter list. Call said function passing file    *
 * 	path/names and the callback. See inline function comments for           *
 * 	additional details.  See 'test.cpp' for a full example.                 *
 *                                                                          *
 * 	Example:                                                                *
 * 	                                                                        *
 * 	Status=CbMpegAudioDecoder("in.mp3","out.pcm", 0, CallbackFunction);     *
 *                                                                          *
 *  To compile, see Makefile comments.                                      *
 *                                                                          *
 *                                                                          *
 * ---Original code comment below----                                       *
 ****************************************************************************
 * [from madlld.c] (c) 2001--2004 Bertrand Petit							*
 *																			*
 * Redistribution and use in source and binary forms, with or without		*
 * modification, are permitted provided that the following conditions		*
 * are met:																	*
 *																			*
 * 1. Redistributions of source code must retain the above copyright		*
 *    notice, this list of conditions and the following disclaimer.			*
 *																			*
 * 2. Redistributions in binary form must reproduce the above				*
 *    copyright notice, this list of conditions and the following			*
 *    disclaimer in the documentation and/or other materials provided		*
 *    with the distribution.												*
 * 																			*
 * 3. Neither the name of the author nor the names of its contributors		*
 *    may be used to endorse or promote products derived from this			*
 *    software without specific prior written permission.					*
 * 																			*
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''		*
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED		*
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A			*
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR		*
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,				*
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT			*
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF			*
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND		*
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,		*
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT		*
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF		*
 * SUCH DAMAGE.																*
 *																			*
 ****************************************************************************/

/*
 * $Name:  $
 * $Date: 2004/07/12 21:41:33 $
 * $Revision: 1.7 $
 */

/****************************************************************************
 * Includes																	*
 ****************************************************************************/
#include "madlldlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>  /* for fstat routine */
#include <fcntl.h>		/* "" */
#include <sys/io.h>			/* "" */
#include <limits.h>
#include <errno.h>
#include <math.h> /* for pow() and log10() */
#include "bstdfile.h"

//#include <ofstream>
#include <unistd.h>


/****************************************************************************
 * Return an error string associated with a mad error code.					*
 ****************************************************************************/
/* Mad version 0.14.2b introduced the mad_stream_errorstr() function.
 * For previous library versions a replacement is provided below.
 */
#if (MAD_VERSION_MAJOR>=1) || \
    ((MAD_VERSION_MAJOR==0) && \
     (((MAD_VERSION_MINOR==14) && \
       (MAD_VERSION_PATCH>=2)) || \
      (MAD_VERSION_MINOR>14)))
#define MadErrorString(x) mad_stream_errorstr(x)
#else
static const char *MadErrorString(const struct mad_stream *Stream)
{
	switch(Stream->error)
	{
		/* Generic unrecoverable errors. */
		case MAD_ERROR_BUFLEN:
			return("input buffer too small (or EOF)");
		case MAD_ERROR_BUFPTR:
			return("invalid (null) buffer pointer");
		case MAD_ERROR_NOMEM:
			return("not enough memory");

		/* Frame header related unrecoverable errors. */
		case MAD_ERROR_LOSTSYNC:
			return("lost synchronization");
		case MAD_ERROR_BADLAYER:
			return("reserved header layer value");
		case MAD_ERROR_BADBITRATE:
			return("forbidden bitrate value");
		case MAD_ERROR_BADSAMPLERATE:
			return("reserved sample frequency value");
		case MAD_ERROR_BADEMPHASIS:
			return("reserved emphasis value");

		/* Recoverable errors */
		case MAD_ERROR_BADCRC:
			return("CRC check failed");
		case MAD_ERROR_BADBITALLOC:
			return("forbidden bit allocation value");
		case MAD_ERROR_BADSCALEFACTOR:
			return("bad scalefactor index");
		case MAD_ERROR_BADFRAMELEN:
			return("bad frame length");
		case MAD_ERROR_BADBIGVALUES:
			return("bad big_values count");
		case MAD_ERROR_BADBLOCKTYPE:
			return("reserved block_type");
		case MAD_ERROR_BADSCFSI:
			return("bad scalefactor selection info");
		case MAD_ERROR_BADDATAPTR:
			return("bad main_data_begin pointer");
		case MAD_ERROR_BADPART3LEN:
			return("bad audio data length");
		case MAD_ERROR_BADHUFFTABLE:
			return("bad Huffman table select");
		case MAD_ERROR_BADHUFFDATA:
			return("Huffman data overrun");
		case MAD_ERROR_BADSTEREO:
			return("incompatible block_type for JS");

		/* Unknown error. This switch may be out of sync with libmad's
		 * defined error codes.
		 */
		default:
			return("Unknown error code");
	}
}
#endif


/****************************************************************************
 * 'Borrowed' from audio.c in the madplay source
 * NAME:	Prng()
 * DESCRIPTION:	32-bit pseudo-random number generator
 ****************************************************************************/
static inline unsigned long Prng(unsigned long state)
{
  return (state * 0x0019660dL + 0x3c6ef35fL) & 0xffffffffL;
}


/****************************************************************************
 * A little more advanced scaling routine based on madplay's 
 * audio_linear_dither() function. Rather than simply rounding the 24 bit
 * number down to 16 bits (as MadFixedToSshort() in madlld does), it performs 
 * dithering, which is the addition of a random number to the least 
 * significant bits (LSB) of the sample that targets the LSB at the 16 bit
 * mark. CbMpegAudioDecoder() uses this to perform its scaling before output.
 ****************************************************************************/
static inline signed short Scale(mad_fixed_t sample, mad_fixed_t *gRandom)
{
	unsigned int scalebits;
	mad_fixed_t output, mask, rnd;

	/* bias */
	output = sample + (1L << (MAD_F_FRACBITS + 1 - 16 - 1));	
	
  	scalebits = MAD_F_FRACBITS + 1 - 16;
  	mask = (1L << scalebits) - 1;

  	/* dither */
	rnd = Prng(*gRandom);
  	output += (rnd & mask) - (*gRandom & mask);
	*gRandom = rnd;
	
  	/* clip */
	if (output >= MAD_F_ONE)
    	output = MAD_F_ONE - 1;
	else if (output < -MAD_F_ONE)
    	output = -MAD_F_ONE;

  	/* quantize */
  	output &= ~mask;

	/* scale */
	return output >> scalebits;
}


/****************************************************************************
 * This is based on the MpegAudioDecoder() function in the madlld source.
 * It differs in that it performs the file opens, taking the path/filenames 
 * as arguments, and passes the following information to a callback 
 * function while in the decoding loop, to be handled by the calling code 
 * (generally used for reporting--see test.cpp for an example):
 *
 *			Frame count			
 *			Total input bytes read
 *			A pointer to a mad_header struct (MP3 info)
 *
 * Since this is used as a library function, errors and information are 
 * reported back to a char pointer (StatMsg) passed as a parameter from the 
 * calling code.
 * 
 * It also uses a flag (WavPad) passed from the calling code which tells it
 * whether to add a RIFF/WAVE header (making OutFN a WAV file). If WavPad==1
 * then 44 null bytes are written to the beginning of OutFN before decoding 
 * occurs, and the WAV header is constructed and added after decoding.
 ****************************************************************************/
#define INPUT_BUFFER_SIZE	(5*8192)
#define OUTPUT_BUFFER_SIZE	8192 /* Must be an integer multiple of 4. */
 int CbMpegAudioDecoder(
		const char *InFN, 
		const char *OutFN,
	    int WavPad,
		char *StatMsg,	
		void (*CbFunc)(unsigned long FrameCnt, unsigned long InByteTot, struct mad_header *Header) 
		)
{

	FILE *InputFp; 
	FILE *OutputFp;
	struct mad_stream	Stream;
	struct mad_frame	Frame;
	struct mad_synth	Synth;
	mad_timer_t			Timer;
	unsigned char		InputBuffer[INPUT_BUFFER_SIZE+MAD_BUFFER_GUARD],
						OutputBuffer[OUTPUT_BUFFER_SIZE],
						*OutputPtr=OutputBuffer,
						*GuardPtr=NULL;
	const unsigned char	*OutputBufferEnd=OutputBuffer+OUTPUT_BUFFER_SIZE;
	int					Status=0,
						i;
	unsigned long		FrameCount=0;
	unsigned long		ByteCount=0;			/* count total input bytes */
	bstdfile_t			*BstdFile;
	char 				padwav[44] = "";		/* WAV padding */
	wavhdr 				wavh;					/* my WAV header */		
	int					wavflg = 1;

	
	/* Random number for generating dither. 
	 * Used by Scale() below in preparing
	 * the samples for 16 bit output. 
	 */
	mad_fixed_t gRand = 0xa8b9ff7e;
	

	/* Open passed files for reading and writing */
	if ( !(InputFp = fopen(InFN, "rb")) ) {				/* input */
		sprintf(StatMsg,"%s not found!\n", InFN);
		return(1);
	}
	if ( !(OutputFp = fopen(OutFN, "wb")) ) { 			/* output */
		sprintf(StatMsg,"Couldn't open %s for write!\n", OutFN);
		return(1);
	}

	
	/* Write the WAV header padding if flagged to
	 * do so.
	 */
	if (WavPad == 1) {
		fwrite(&padwav, 1, sizeof(padwav), OutputFp);
	}
	
	
	/* First the structures used by libmad must be initialized. */
	mad_stream_init(&Stream);
	mad_frame_init(&Frame);
	mad_synth_init(&Synth);
	mad_timer_reset(&Timer);

	/* Decoding options can here be set in the options field of the
	 * Stream structure.
	 */

	/* {1} When decoding from a file we need to know when the end of
	 * the file is reached at the same time as the last bytes are read
	 * (see also the comment marked {3} bellow). Neither the standard
	 * C fread() function nor the POSIX read() system call provides
	 * this feature. We thus need to perform our reads through an
	 * interface having this feature, this is implemented here by the
	 * bstdfile.c module.
	 */
	BstdFile=NewBstdFile(InputFp);
	if(BstdFile==NULL)
	{
		sprintf(StatMsg,"Can't create a new bstdfile_t (%s).\n", strerror(errno));
		return(1);
	}

	/* This is the decoding loop. */
	do
	{
		/* The input bucket must be filled if it becomes empty or if
		 * it's the first execution of the loop.
		 */
		if(Stream.buffer==NULL || Stream.error==MAD_ERROR_BUFLEN)
		{
			size_t			ReadSize,
							Remaining;
			unsigned char	*ReadStart;

			/* {2} libmad may not consume all bytes of the input
			 * buffer. If the last frame in the buffer is not wholly
			 * contained by it, then that frame's start is pointed by
			 * the next_frame member of the Stream structure. This
			 * common situation occurs when mad_frame_decode() fails,
			 * sets the stream error code to MAD_ERROR_BUFLEN, and
			 * sets the next_frame pointer to a non NULL value. (See
			 * also the comment marked {4} bellow.)
			 *
			 * When this occurs, the remaining unused bytes must be
			 * put back at the beginning of the buffer and taken in
			 * account before refilling the buffer. This means that
			 * the input buffer must be large enough to hold a whole
			 * frame at the highest observable bit-rate (currently 448
			 * kb/s). XXX=XXX Is 2016 bytes the size of the largest
			 * frame? (448000*(1152/32000))/8
			 */
			if(Stream.next_frame!=NULL)
			{
				Remaining=Stream.bufend-Stream.next_frame;
				memmove(InputBuffer,Stream.next_frame,Remaining);
				ReadStart=InputBuffer+Remaining;
				ReadSize=INPUT_BUFFER_SIZE-Remaining;
			}
			else
				ReadSize=INPUT_BUFFER_SIZE,
					ReadStart=InputBuffer,
					Remaining=0;

			/* Fill-in the buffer. If an error occurs print a message
			 * and leave the decoding loop. If the end of stream is
			 * reached we also leave the loop but the return status is
			 * left untouched.
			 */
			ReadSize=BstdRead(ReadStart,1,ReadSize,BstdFile);
			ByteCount+=ReadSize;		/* passed to CbFunc */
			if(ReadSize<=0)
			{
				if(ferror(InputFp))
				{
					sprintf(StatMsg,"Read error on bit-stream (%s)\n",strerror(errno));
					Status=1;
				}
				if(feof(InputFp))
					sprintf(StatMsg,"End of input stream\n"); 
				break;
			}

			/* {3} When decoding the last frame of a file, it must be
			 * followed by MAD_BUFFER_GUARD zero bytes if one wants to
			 * decode that last frame. When the end of file is
			 * detected we append that quantity of bytes at the end of
			 * the available data. Note that the buffer can't overflow
			 * as the guard size was allocated but not used the the
			 * buffer management code. (See also the comment marked
			 * {1}.)
			 *
			 * In a message to the mad-dev mailing list on May 29th,
			 * 2001, Rob Leslie explains the guard zone as follows:
			 *
			 *    "The reason for MAD_BUFFER_GUARD has to do with the
			 *    way decoding is performed. In Layer III, Huffman
			 *    decoding may inadvertently read a few bytes beyond
			 *    the end of the buffer in the case of certain invalid
			 *    input. This is not detected until after the fact. To
			 *    prevent this from causing problems, and also to
			 *    ensure the next frame's main_data_begin pointer is
			 *    always accessible, MAD requires MAD_BUFFER_GUARD
			 *    (currently 8) bytes to be present in the buffer past
			 *    the end of the current frame in order to decode the
			 *    frame."
			 */
			if(BstdFileEofP(BstdFile))
			{
				GuardPtr=ReadStart+ReadSize;
				memset(GuardPtr,0,MAD_BUFFER_GUARD);
				ReadSize+=MAD_BUFFER_GUARD;
			}

			/* Pipe the new buffer content to libmad's stream decoder
             * facility.
			 */
			mad_stream_buffer(&Stream,InputBuffer,ReadSize+Remaining);
			Stream.error=(mad_error)0;
		}

		/* Decode the next MPEG frame. The streams is read from the
		 * buffer, its constituents are break down and stored the the
		 * Frame structure, ready for examination/alteration or PCM
		 * synthesis. Decoding options are carried in the Frame
		 * structure from the Stream structure.
		 *
		 * Error handling: mad_frame_decode() returns a non zero value
		 * when an error occurs. The error condition can be checked in
		 * the error member of the Stream structure. A mad error is
		 * recoverable or fatal, the error status is checked with the
		 * MAD_RECOVERABLE macro.
		 *
		 * {4} When a fatal error is encountered all decoding
		 * activities shall be stopped, except when a MAD_ERROR_BUFLEN
		 * is signaled. This condition means that the
		 * mad_frame_decode() function needs more input to complete
		 * its work. One shall refill the buffer and repeat the
		 * mad_frame_decode() call. Some bytes may be left unused at
		 * the end of the buffer if those bytes forms an incomplete
		 * frame. Before refilling, the remaining bytes must be moved
		 * to the beginning of the buffer and used for input for the
		 * next mad_frame_decode() invocation. (See the comments
		 * marked {2} earlier for more details.)
		 *
		 * Recoverable errors are caused by malformed bit-streams, in
		 * this case one can call again mad_frame_decode() in order to
		 * skip the faulty part and re-sync to the next frame.
		 */
		if(mad_frame_decode(&Frame,&Stream))
		{
			if(MAD_RECOVERABLE(Stream.error))
			{
				/* Do not print a message if the error is a loss of
				 * synchronization or this loss is due to the end of
				 * stream guard bytes. (See the comments marked {3}
				 * supra for more informations about guard bytes.)
				 */
				if( Stream.error!=MAD_ERROR_LOSTSYNC ||
				    Stream.this_frame!=GuardPtr ||
					/* Suppress error if ID3 or TAG(??)
					 * tag was cause:
					 */
				   (Stream.this_frame[0] !=  'I' &&
					Stream.this_frame[1] !='D' &&
					Stream.this_frame[2] !='3') ||				   
					(Stream.this_frame[0] !='T' &&
					Stream.this_frame[1] != 'A' &&
					Stream.this_frame[2] != 'G') )
				{
					
						sprintf(StatMsg,"Recoverable frame level error (%s)\n",
								MadErrorString(&Stream));

				}
				continue;
			}
			else
				if(Stream.error==MAD_ERROR_BUFLEN)
					continue;
				else
				{
					sprintf(StatMsg,"Unrecoverable frame level error (%s).\n",
							MadErrorString(&Stream));
					Status=1;
					break;
				}
		}


		/* Accounting. The computed frame duration is in the frame
		 * header structure. It is expressed as a fixed point number
		 * whole data type is mad_timer_t. It is different from the
		 * samples fixed point format and unlike it, it can't directly
		 * be added or subtracted. The timer module provides several
		 * functions to operate on such numbers. Be careful there, as
		 * some functions of libmad's timer module receive some of
		 * their mad_timer_t arguments by value!
		 */
		FrameCount++;
		mad_timer_add(&Timer,Frame.header.duration);


		/* Send accumulated information to the callback
		 * This gets called every loop.
		 */
		CbFunc(FrameCount, ByteCount, &Frame.header);		

		
		/* Set the channel and sample rate if WAV header 
		 * Use 'wavflg' so this only fires once
		 */
		if (wavflg && WavPad == 1) {
			/* assign number of channels */
//			if (Frame.header.mode > MAD_MODE_SINGLE_CHANNEL) {
				wavh.nChannels = 2;
//			}
//			else {
//				wavh.nChannels = 1;
//			}
			wavh.nSamplesPerSec = (int)Frame.header.samplerate;
			wavflg = NULL;		/* so this is only called once */
		}

		
		/* Once decoded the frame is synthesized to PCM samples. No errors
		 * are reported by mad_synth_frame();
		 */
		mad_synth_frame(&Synth,&Frame);

		/* Synthesized samples must be converted from libmad's fixed
		 * point number to the consumer format. Here we use unsigned
		 * 16 bit big endian integers on two channels. Integer samples
		 * are temporarily stored in a buffer that is flushed when
		 * full.
		 */
		for(i=0;i<Synth.pcm.length;i++)
		{
			signed short	Sample;

			/* Left channel */
			Sample=Scale(Synth.pcm.samples[0][i], &gRand);

			/* output in 16 bit little-endian */			
			/*
			*(OutputPtr++)=Sample>>8;       //Originally big-endian
			*(OutputPtr++)=Sample&0xff;
			*/
			*(OutputPtr++)=((Sample>>0) & 0xff);
			*(OutputPtr++)=((Sample>>8) & 0xff);
			


			/* Right channel. If the decoded stream is monophonic then
			 * the right output channel is the same as the left one.
			 */
			if(MAD_NCHANNELS(&Frame.header)==2)
				Sample=Scale(Synth.pcm.samples[1][i], &gRand);

			/* output in 16 bit little-endian */			
			/*
			*(OutputPtr++)=Sample>>8;		//Originally big-endian
			*(OutputPtr++)=Sample&0xff;
			*/
			*(OutputPtr++)=((Sample>>0) & 0xff);			
			*(OutputPtr++)=((Sample>>8) & 0xff);


			/* Flush the output buffer if it is full. */
			if(OutputPtr==OutputBufferEnd)
			{
				if(fwrite(OutputBuffer,1,OUTPUT_BUFFER_SIZE,OutputFp)!=OUTPUT_BUFFER_SIZE)
				{
					sprintf(StatMsg,"PCM write error (%s).\n",
							strerror(errno));
					Status=2;
					break;
				}
				OutputPtr=OutputBuffer;
			}
		}
	}while(1);

	/* The input file was completely read; the memory allocated by our
	 * reading module must be reclaimed.
	 */
	BstdFileDestroy(BstdFile);

	/* Mad is no longer used, the structures that were initialized must
     * now be cleared.
	 */
	mad_synth_finish(&Synth);
	mad_frame_finish(&Frame);
	mad_stream_finish(&Stream);

	/* If the output buffer is not empty and no error occurred during
     * the last write, then flush it.
	 */
	if(OutputPtr!=OutputBuffer && Status!=2)
	{
		size_t	BufferSize=OutputPtr-OutputBuffer;

		if(fwrite(OutputBuffer,1,BufferSize,OutputFp)!=BufferSize)
		{
			sprintf(StatMsg,"PCM write error (%s).\n",
					strerror(errno));
			Status=2;
		}
	}

	/* Accounting report if no error occurred. */
	if(!Status)
	{
		char	Buffer[80];

		/* The duration timer is converted to a human readable string
		 * with the versatile, but still constrained mad_timer_string()
		 * function, in a fashion not unlike strftime(). The main
		 * difference is that the timer is broken into several
		 * values according some of it's arguments. The units and
		 * fracunits arguments specify the intended conversion to be
		 * executed.
		 *
		 * The conversion unit (MAD_UNIT_MINUTES in our example) also
		 * specify the order and kind of conversion specifications
		 * that can be used in the format string.
		 *
		 * It is best to examine libmad's timer.c source-code for details
		 * of the available units, fraction of units, their meanings,
		 * the format arguments, etc.
		 */
		mad_timer_string(Timer,Buffer,"%lu:%02lu.%03u",
						 MAD_UNITS_MINUTES,MAD_UNITS_MILLISECONDS,0);
		sprintf(StatMsg,"%lu frames decoded (%s).\n",
				FrameCount,Buffer);		

	}


	/* close files */
	fclose(InputFp);
	fclose(OutputFp);


	/* Write the WAV header to the file */
	if (WavPad == 1) {
		
		/* get OutFN file size (raw PCM) */
		struct stat fstt;
		int iflde = open(OutFN, O_RDONLY);
		fstat(iflde, &fstt);
		close(iflde);
		
		/* setup header */
		wavh.RIFF = 0x46464952;						/* 'RIFF' */
		wavh.size = fstt.st_size-8;	/* total size of wave file, including header */
		wavh.WAVE =  0x45564157;					/* 'WAVE' */
		wavh.fmt = 0x20746D66;						/* 'fmt ' */
		wavh.wBitsPerSample = 16;
		wavh.wFormatLength = 16;
		wavh.wFormatTag = 1;
		wavh.data = 0x61746164;						/* 'data' */
		wavh.datasize = (int)fstt.st_size-sizeof(wavh);
		wavh.nBlockAlign = (short)(wavh.wBitsPerSample / 8 * wavh.nChannels);
		wavh.nAvgBytesPerSec = wavh.nSamplesPerSec * wavh.nBlockAlign;

		/* write wav header */
		OutputFp = fopen(OutFN, "rb+");
		fwrite(&wavh, 1, sizeof(wavh), OutputFp);		
		fclose(OutputFp);

	}


	/* That's the end of the world (in the H. G. Wells way). */
	return(Status);
}

/****************************************************************************
 * End of file madlldlib.cpp                                                *
 ****************************************************************************/
