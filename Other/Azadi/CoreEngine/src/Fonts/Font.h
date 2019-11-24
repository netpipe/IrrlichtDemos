#ifndef FONT_H
#define FONT_H

#include <map>
#include <vector>
#include <cassert>
#include <string>
#include <sstream>

#ifdef __APPLE__
#include <GL/glew.h>
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#include "ft2build.h"
#include FT_FREETYPE_H
#include FT_GLYPH_H

typedef struct fontSet
{
    FT_Face face;
    std::map<unsigned int, std::vector<GLuint> > displayList;
    std::map<unsigned int, std::vector<long> > advancex;
    std::map<unsigned int, std::vector<long> > bearingx;
    std::map<unsigned int, std::vector<long> > bearingy;
    std::map<unsigned int, std::vector<GLfloat> > x;
    std::map<unsigned int, std::vector<GLfloat> > y;
    std::map<unsigned int, std::vector<int> > tops;
    std::map<unsigned int, long > height;
};

class Font
{
	public:
		Font(const std::string &workingDirectory);
		~Font();
		float textWidth(const std::string&, const std::string& font, const float &pt);
		float drawText(const std::string&, const std::string& font, const float &pt, const unsigned int &maxWidth);
		float drawText(float, const std::string& font, const float &pt, const unsigned int &maxWidth);
        float drawText(int, const std::string& font, const float &pt, const unsigned int &maxWidth);
		void loadFont(const std::string&);
		void genPointSize(std::string font, unsigned int size);
		long getFontHeight(const std::string &font, const float &pt);

		static Font* getInstance() { return instance; }
	private:
        static Font *instance;

        std::string workingDirectory;
        FT_Library library;
        std::map<std::string, fontSet> faces;
        std::map<std::string, std::vector<unsigned int> > loadedSizes;
        char alphabet[94];
};

#endif
