#ifndef WINDOW_H
#define WINDOW_H

#include <GL/glew.h>

#ifndef __APPLE__
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif

#include <string>
#include <vector>

#include "../Input/Input.h"
#include "../Texture/TextureFactory.h"

namespace GUI
{
    class Window
    {
        public:
            Window();
            Window(const unsigned int &x, const unsigned int &y, const unsigned int &w, const unsigned int &h, const std::string &texture, const bool &resizeable);
            virtual ~Window() {}

            void parseInput(Input &input);
            virtual void draw(const int &sw, const int &sh) = 0;

            Window& setX(const int &x);
            Window& setY(const int &y);
            Window& setW(const int &w);
            Window& setH(const int &h);
            Window& setResizeable(const bool &flag);

            inline int getX() { return x; }
            inline int getY() { return y; }
            inline int getW() { return w; }
            inline int getH() { return h; }
            inline bool isResizeable() { return resizeable; }

            void setVisible(const bool &flag);
            inline bool isVisible() { return visible; }
        protected:
            int x, y, w, h;
            std::string texture;
            bool resizeable;
            bool visible;
            bool hasFocus;

            int mouseStartDragX;
            int mouseStartDragY;

            int mouseStartResizeX;
            int mouseStartResizeY;

            bool isDragging;
            bool isResizing;
    };
}

#endif
