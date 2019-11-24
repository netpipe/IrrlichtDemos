#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <string>

#include "../Input/Input.h"
#include "../Texture/TextureFactory.h"

namespace GUI
{
    class Scrollbar
    {
        public:

            Scrollbar(const int &x = 0, const int &y = 0, const int &w = 0, const int &h = 0, const int &direction = 0, const std::string &texture = "", const std::string &texture2 = "");

            ~Scrollbar();

            Scrollbar& setX(const int &x);
            Scrollbar& setY(const int &y);
            Scrollbar& setW(const int &w);
            Scrollbar& setH(const int &h);

            Scrollbar& setDirection(const int &direction);

            Scrollbar& setBarTexture(const std::string &texture);
            Scrollbar& setSliderTexture(const std::string &texture);

            Scrollbar& setBarMin(const int &min);
            Scrollbar& setBarMax(const int &max);
            int getBarMin();
            int getBarMax();

            Scrollbar& setCurrentValue(const int &val);
            int getCurrentValue();

            Scrollbar& incCurrentValue();
            Scrollbar& decCurrentValue();

            Scrollbar& setMinVisiblePixels(const int &minVis);

            inline bool hasInput() { return sHeld; }

            void updateSlider();
            void parseInput(Input &input);

            void draw();
        private:
            int x, y, w, h, direction;
            int min, max;
            int cValue;
            std::string barTexName, scrollTexName;
            int lmousex, lmousey;

            int sX, sY;
            int sW, sH;
            bool sHeld;

            int minVisPixels;

            TextureFactory *texture_factory;
    };
};

#endif
