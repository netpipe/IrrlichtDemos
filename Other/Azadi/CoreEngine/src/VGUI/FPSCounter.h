#ifndef FPSCOUNTER_H
#define FPSCOUNTER_H

#include "../GUI/Pane.h"
#include "../GUI/TextArea.h"
#include "../GUI/Drawing.h"

#include <sstream>

class FPSCounter
{
    public:
        FPSCounter(const int &x, const int &y);
        ~FPSCounter();

        void draw();
    private:
        int x, y;
        GUI::Pane *cbg;
        GUI::TextArea *tar;
};

#endif
