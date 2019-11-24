#ifndef MENU_H
#define MENU_H

#include "../Input/Input.h"

namespace GUI
{
    class Menu
    {
        public:
            virtual ~Menu(){};
            void setVisible(const bool &flag);
            virtual void parseInput(Input &input) {};
            virtual void draw(const int &sw, const int &sh) {};
        protected:
            int x, y, w, h;
            bool visible;
    };
};

#endif
