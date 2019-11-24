#ifndef TEXTAREA_H
#define TEXTAREA_H

#include <string>
#include "../Fonts/Font.h"

namespace GUI
{
    class TextArea
    {
        public:
            TextArea(const unsigned int &x = 0, const unsigned int &y = 0, const unsigned int &w = 0, const unsigned int &h = 0, const std::string &fontName = "", const float &pointSize = 0, const bool &multiline = false);
            ~TextArea();

            void draw();
            void resize(const unsigned int &x, const unsigned int &y, const unsigned int &w, const unsigned int &h);

            TextArea& operator=(const std::string &newText)
            {
                textBuffer = newText;
                redraw = true;
                return *this;
            }

            TextArea& operator<<(const std::string &newText)
            {
                textBuffer += newText;
                redraw = true;
                return *this;
            }
        private:
            std::string textBuffer;
            unsigned int x, y, w, h;
            std::string font;
            float pointSize;
            bool multiline;

            bool redraw;
            GLint textList;
    };
};

#endif
