#include "TheoraFile.h"

// Make sure to fix this header link
#include "../Encryption/encUserTools.h"

bool TheoraFile::open(const std::string &filename)
{
	vidReady = 0;
	frames = 0;

	file = fopen(filename.c_str(), "rb");
	if (file == NULL)
	{
		return false;
	}

	ogg_sync_init(&syncState);

	theora_comment_init(&theoraComment);
	theora_info_init(&theoraInfo);

	stateFlag = 0;

	theora_packet = 0;

	while (!stateFlag)
	{
		if (buffer_data() == 0)
			break;

		while(ogg_sync_pageout(&syncState, &oggPage) > 0)
		{
			if (!ogg_page_bos(&oggPage))
			{
				queue_page();
				stateFlag = 1;
				break;
			}

			ogg_stream_state temp;

			ogg_stream_init(&temp, ogg_page_serialno(&oggPage));
			ogg_stream_pagein(&temp, &oggPage);
			ogg_stream_packetout(&temp, &oggPacket);

			if (!theora_packet && theora_decode_header(&theoraInfo, &theoraComment, &oggPacket) >= 0)
			{
				memcpy(&theoraOut, &temp, sizeof(temp));
				theora_packet = 1;
			}
			else
			{
				ogg_stream_clear(&temp);
			}
		}
	}

	while(theora_packet && theora_packet < 3)
	{
		int result;

		while (theora_packet && (theora_packet < 3) && (result = ogg_stream_packetout(&theoraOut, &oggPacket)))
		{
			if (result < 0)
			{
				// Bad header parsing
				close();
				return false;
			}
			if (theora_decode_header(&theoraInfo, &theoraComment, &oggPacket))
			{
				// Bad stream headers
				close();
				return false;
			}
			++theora_packet;
		}

		if (ogg_sync_pageout(&syncState, &oggPage) > 0)
		{
			queue_page();
		}
		else
		{
			result = buffer_data();
			if (result == 0)
			{
				// abnormal EOF
				close();
				return false;
			}
		}
	}

	if (theora_packet)
	{
		theora_decode_init(&theoraState, &theoraInfo);
		theora_control(&theoraState, TH_DECCTL_GET_PPLEVEL_MAX, &ppLevelMax, sizeof(ppLevelMax));
		theora_control(&theoraState, TH_DECCTL_SET_PPLEVEL, &ppLevel, sizeof(ppLevel));

		ppIncoming = 0;
	}
	else
	{
		// There is no video @_@
		close();
		return false;
	}

	return true;
}

void TheoraFile::close()
{
	theora_info_clear(&theoraInfo);
	theora_comment_clear(&theoraComment);

	ogg_stream_clear(&theoraOut);
	ogg_sync_clear(&syncState);
	fclose(file);
}

void TheoraFile::queue_page()
{
	if (theora_packet)
		ogg_stream_pagein(&theoraOut, &oggPage);
}

int TheoraFile::buffer_data()
{
	char *buffer = ogg_sync_buffer(&syncState, 4096);
	int bytes = fread(buffer, 1, 4096, file);
	ogg_sync_wrote(&syncState, bytes);
	return (bytes);
}

long TheoraFile::getPosition()
{
	return long(ogg_page_granulepos(&oggPage));
}

void TheoraFile::setPosition(const long &position)
{
	theora_control(&theoraState, TH_DECCTL_SET_GRANPOS, (void*)&position, sizeof(position));
}

void TheoraFile::rewind()
{

}

void TheoraFile::getDimensions(unsigned int *w, unsigned int *h)
{
	*w = theoraInfo.width;
	*h = theoraInfo.height;
}

float TheoraFile::clip(const float &rhs) const
{
	return ((rhs > 255) ? 255 : rhs);
}

bool TheoraFile::getNextFrame(void *data)
{
	if (ogg_stream_packetout(&theoraOut, &oggPacket) > 0)
	{
		if (ppIncoming)
		{
			ppLevel += ppIncoming;
			theora_control(&theoraState, TH_DECCTL_SET_PPLEVEL, &ppLevel, sizeof(ppLevel));
			ppIncoming = 0;
		}

		if (oggPacket.granulepos >= 0)
		{
			theora_control(&theoraState, TH_DECCTL_SET_GRANPOS, &oggPacket.granulepos, sizeof(oggPacket.granulepos));
		}
		if (theora_decode_packetin(&theoraState, &oggPacket) == 0)
		{
			vidGranPos = long(theoraState.granulepos);
			vidTime = float(theora_granule_time(&theoraState, vidGranPos));
			++frames;

			if (vidTime >= 0.0f)
				vidReady = 1;
			else
			{
				ppIncoming = ppLevel > 0 ? -1: 0;
				// dropped a frame
			}
		}
	}

	if (feof(file)) 
		return false;

	if (!vidReady)
	{
		buffer_data();
		while (ogg_sync_pageout(&syncState, &oggPage) > 0)
			queue_page();

		return false;
	}

	if (vidReady)
	{
		yuv_buffer yuv;

		theora_decode_YUVout(&theoraState, &yuv);

		int crop_offset;
		crop_offset = theoraInfo.offset_x + yuv.y_stride * theoraInfo.offset_y;

		unsigned char *p = (unsigned char*)data;

		for (unsigned int i = 0; i < theoraInfo.height; ++i)
		{
			for (unsigned int ii = 0; ii < theoraInfo.width; ++ii)
			{
				*p++ = char(clip((298.082f * (yuv.y[(i*yuv.y_stride)+ii] - 16)) + (408.583f * (yuv.v[(i*yuv.uv_stride)+ii] - 128)) + 128));
				*p++ = char(clip((298.082f * (yuv.y[(i*yuv.y_stride)+ii] - 16)) - (100.291f * (yuv.u[(i*yuv.uv_stride)+ii] - 128)) - (208.120f * (yuv.v[(i*yuv.uv_stride)+ii] - 128))));
				*p++ = char(clip((298.082f * (yuv.y[(i*yuv.y_stride)+ii] - 16)) + (516.412f * (yuv.u[(i*yuv.uv_stride)+ii] - 128))));
			}
		}

		vidReady = 0;

		return true;
	}

	return false;
	/*
	th_ycbcr_buffer yuv_data;
	int stride;
	if (th_decode_ycbcr_out(decoderInstance, yuv_data) == 0)
	{
		*width = yuv_data[0].width;
		*height = yuv_data[0].height;
		stride = yuv_data[0].stride;
		
		char *p = (char*)data;
		
		for (unsigned int i = 0; i < *height; ++i)
		{
			for (unsigned int ii = 0; ii < *width; ++ii)
			{
				*p++ = (298.082 * yuv_data[0].data[(i*stride)+ii]											  + 408.583 * yuv_data[2].data[(i*stride)+ii]);	// R
				*p++ = (298.082 * yuv_data[0].data[(i*stride)+ii] - 100.291 * yuv_data[1].data[(i*stride)+ii] - 208.120 * yuv_data[2].data[(i*stride)+ii]);	// G
				*p++ = (298.082 * yuv_data[0].data[(i*stride)+ii] + 516.412 * yuv_data[1].data[(i*stride)+ii]				   							 );	// B			
			}
		}
		return true;
	}
	*/
}