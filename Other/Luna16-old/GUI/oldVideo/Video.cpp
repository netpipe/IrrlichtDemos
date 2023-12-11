#include "Video.h"

namespace agEngine
{
	Video::Video(const std::string &filename, IVideoDriver *vdriver)
	{
		driver = vdriver;
		file = new TheoraFile;
		file->open(filename);
		file->getDimensions(&width, &height);
		buffer = new char[width*height*24];
		for (unsigned int i = 0; i < width*height; ++i)
			buffer[i] = 0;

		texture = driver->addTexture("video", driver->createImageFromData(ECF_R8G8B8, dimension2d<s32>(width, height), buffer, true, false));
	}

	Video::~Video()
	{
		file->close();
		delete file;
	}

	bool Video::play()
	{
		return false;
	}

	void Video::stop()
	{

	}

	void Video::update()
	{
		if (file->getNextFrame(buffer))
		{
			texture->drop();
			texture = driver->addTexture("video", driver->createImageFromData(ECF_R8G8B8, dimension2d<s32>(width, height), buffer, true, false));
		}
	}

	ITexture *Video::getTexData()
	{
		return texture;
	}
}

