#ifndef THEORAFILE_H
#define THEORAFILE_H

#include <string>
#include <ogg/ogg.h>
#include <theora/theora.h>
#include <theora/theoradec.h>
#include <vorbis/codec.h>

class TheoraFile
{
public:
	bool open(const std::string &filename);
	void close();

	long getPosition();
	void setPosition(const long &position);

	void rewind();
	void getDimensions(unsigned int *w, unsigned int *h);
	
	bool getNextFrame(void *data);
private:
	void queue_page();
	int buffer_data();
	float clip(const float &rhs) const;

	FILE *file;
	int stateFlag, ppLevelMax, ppLevel, ppIncoming;
	int theora_packet, vorbis_packet;

	ogg_sync_state syncState;
	ogg_stream_state streamState;
	ogg_stream_state vorbisOut;
	ogg_stream_state theoraOut;
	ogg_page oggPage;
	ogg_packet oggPacket;
	theora_state theoraState;
	theora_comment theoraComment;
	theora_info theoraInfo;

	long vidGranPos;
	float vidTime;
	unsigned int vidReady;

	unsigned int frames;
	
	char *buffer;
};

#endif
