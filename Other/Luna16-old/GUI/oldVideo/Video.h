#ifndef VIDEO_H
#define VIDEO_H

#include "TheoraFile.h"
#include <irrlicht.h>

namespace agEngine
{
	using namespace irr;

	using namespace core;
	using namespace scene;
	using namespace video;

	class Video
	{
	public:
		Video(const std::string &filename, IVideoDriver *video);
		~Video();

		bool play();
		void stop();

		void update();

		ITexture* getTexData();
	private:
		IVideoDriver *driver;
		char *buffer;
		TheoraFile *file;
		ITexture *texture;
		unsigned int width, height;
	};
}

#endif
