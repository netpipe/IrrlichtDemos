#ifndef PANE_H
#define PANE_H

#include <string>
#include "../Texture/TextureFactory.h"

namespace GUI
{
    class Pane
    {
        public:
            Pane(const int &x = 0, const int &y = 0, const int &w = 0, const int &h = 0, const std::string &textureName = "");
            ~Pane();

            Pane& setX(const int &x);
            Pane& setY(const int &y);
            Pane& setW(const int &w);
            Pane& setH(const int &h);

            Pane& setTexture(const std::string &textureName);

            void draw();
        private:
            int x, y, w, h;
            std::string textureName;
    };
};

#endif
