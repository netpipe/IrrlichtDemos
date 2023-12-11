#ifndef MOUSESCROLLAREA_H
#define MOUSESCROLLAREA_H

#include "../Input/Input.h"

namespace GUI
{
    class MouseScrollArea
    {
        public:
            MouseScrollArea();
            MouseScrollArea(const unsigned int &x, const unsigned int &y, const unsigned int &w, const unsigned int &h);
            ~MouseScrollArea();

            void parseInput(Input &input);
            void setDimensions(const unsigned int &x, const unsigned int &y, const unsigned int &w, const unsigned int &h);
            inline bool wheelUp() { return wheelScrolledUp; }
            inline bool wheelDown() { return wheelScrolledDown; }

        private:
            bool wheelScrolledUp, wheelScrolledDown;

            unsigned int x, y, w, h;
    };
};

#endif
