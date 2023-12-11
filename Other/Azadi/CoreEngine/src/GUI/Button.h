#ifndef BUTTON_H
#define BUTTON_H

#include <string>

#include "../Input/Input.h"
#include "../Texture/TextureFactory.h"

namespace GUI
{
    class Button
    {
        public:
            Button();
            ~Button();

            Button& setX(const int &x);
            Button& setY(const int &y);
            Button& setW(const int &w);
            Button& setH(const int &h);
            Button& setTexture(const std::string &textureName);
            Button& setPressedTexture(const std::string &textureName);
            Button& setHoverTexture(const std::string &textureName);

            Button& setCallBack(const std::string &callback);

            void draw();
            void parseInput(Input &input);
            bool isPressed();
            bool isHeld();
            bool isHoveredOver();
        private:
            std::string callBack;
            int x, y, w, h;
            std::string normalTextureName;
            std::string pressedTextureName;
            std::string hoveringTextureName;
            bool pressed;
            bool held;
            bool hovering;

            TextureFactory *texture_factory;
    };
};

#endif
