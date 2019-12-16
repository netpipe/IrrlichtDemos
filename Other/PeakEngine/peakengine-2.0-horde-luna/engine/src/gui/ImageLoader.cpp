#ifndef IMAGELOAD_H
#define  IMAGELOAD_H
#include "gui/ImageLoader.h"

#include <FreeImagePlus.h>
#include <guichan/opengl/openglimage.hpp>
#include <unistd.h>
#include <stdio.h>
//int main (){};
namespace peak
{
	ImageLoader::ImageLoader()
	{
	}
	ImageLoader::~ImageLoader()
	{
	}

	gcn::Image* ImageLoader::load(const std::string& filename, bool convertToDisplayFormat)
	{
		// Open image
		fipImage *image = new fipImage;
		FILE *file = fopen(filename.c_str(), "rb");
		if (file)
		{
			fclose(file);
		}
		else
		{
			printf("File does not exist.\n");
		}
		if (!image->load(filename.c_str()))
		{
			delete image;
			printf("-.-\n");
			throw GCN_EXCEPTION(std::string("Unable to load image file: ") + filename);
		}

		gcn::OpenGLImage *oglimage;
		image->convertTo32Bits();

		// Convert image data
		if (image->getBitsPerPixel() == 32)
		{
			unsigned int *imagedata = new unsigned int[image->getWidth() * image->getHeight()];
			unsigned char *data = image->accessPixels();
			for (unsigned int y = 0; y < image->getHeight(); y++)
			{
				for (unsigned int x = 0; x < image->getWidth(); x++)
				{
					imagedata[((image->getHeight() - y - 1) * image->getWidth() + x)] = *((unsigned int*)(data + (y * image->getWidth() + x) * 4));
				}
			}
			oglimage = new gcn::OpenGLImage(imagedata, image->getWidth(), image->getHeight(), convertToDisplayFormat);
			delete[] imagedata;
		}
		else if (image->getBitsPerPixel() == 24)
		{
			unsigned int *imagedata = new unsigned int[image->getWidth() * image->getHeight()];
			unsigned char *data = image->accessPixels();
			for (unsigned int y = 0; y < image->getHeight(); y++)
			{
				for (unsigned int x = 0; x < image->getWidth(); x++)
				{
					imagedata[((image->getHeight() - y - 1) * image->getWidth() + x)] = *((unsigned int*)(data + (y * image->getWidth() + x) * 3)) | 0xFF000000;
				}
			}
			oglimage = new gcn::OpenGLImage(imagedata, image->getWidth(), image->getHeight(), convertToDisplayFormat);
			delete[] imagedata;
		}
		else
		{
			delete image;
			throw GCN_EXCEPTION(std::string("Wrong image format: ") + filename);
		}

		delete image;

		return oglimage;
	}
}

#endif // IMAGELOAD_H
