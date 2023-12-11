#ifndef AZADICORE_H
#define AZADICORE_H

#include <string>

class azCore
{
    public:
        virtual ~azCore() {}

        virtual void setView2D(const bool &flag) = 0;
        virtual void setConstrainedAxis(const unsigned short &axis) = 0;
        virtual void setCameraPosition(const float &x, const float &y, const float &z) = 0;
        virtual void remapMovementAxis(const unsigned short &a1, const unsigned short &a2, const unsigned short &a3) = 0;

        virtual void setMovementSpeed(const float &moveSpeed) = 0;

        virtual void setFov(const double &fov) = 0;
        virtual void delay(const unsigned int &delayAmount) = 0;

        virtual void captureCursor() = 0;
        virtual void releaseCursor() = 0;

        virtual void toggleAntiAlias(const unsigned short &samples) = 0;

        virtual void outputConsole(const std::string &text) = 0;
        virtual unsigned long int getTicks() = 0;
};

#endif
