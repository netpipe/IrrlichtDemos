#ifndef TEXTINPUTAREA_H
#define TEXTINPUTAREA_H

#include <string>
#include <sstream>

#include <GL/glew.h>
#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <SDL/SDL.h>

#include "../Input/Input.h"
#include "../Fonts/Font.h"
#include "Drawing.h"

namespace GUI
{
    class TextInputArea : public KeyCatcher
    {
        public:
            TextInputArea();
            TextInputArea(const int &x, const int &y, const int &w, const int &h, const std::string &font, const unsigned int &pointSize, const bool &drawBorder);

            TextInputArea& setX(const int &X);
            TextInputArea& setY(const int &Y);
            TextInputArea& setW(const int &W);
            TextInputArea& setH(const int &H);
            TextInputArea& setFont(const std::string &font);
            TextInputArea& setPointSize(const unsigned int &ps);

            void parseInput(Input &input);
            void keyPressed(const SDLKey &sym, const short &ch);
            void draw(const int &sw, const int &sh);

            inline bool hasFocus() { return Focus; }
            inline void setFocus(const bool &flag) { Focus = flag; }
            inline std::string getEnteredText() { return textBuffer; }
        private:
            int x, y, w, h;
            bool drawBorder;
            std::string font;
            unsigned int pointSize;

            unsigned int cursorPosition;
            std::string textBuffer;
            std::string enterBuffer;

            bool Focus;
            bool recompileText;

            GLint TextList;
    };
}

#endif
