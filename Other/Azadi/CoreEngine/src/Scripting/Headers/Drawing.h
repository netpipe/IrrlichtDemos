#ifndef AZDRAWING_H
#define AZDRAWING_H

class azDrawing
{
    public:
        virtual ~azDrawing() {}

        virtual double getFps() = 0;

        virtual void prepare2D() = 0;
        virtual void prepare3D() = 0;

        virtual void clearScreen(const float &r, const float &g, const float &b, const float &i) = 0;

        virtual void drawTexturedRectWithColor(const int &x, const int &y, const int &w, const int &h, const std::string &textureName, const float &r, const float &g, const float &b) = 0;
        virtual void drawTexturedRect(const int &x, const int &y, const int &w, const int &h, const std::string &textureName) = 0;

        virtual void toggleAnisotropicFiltering(const float &amount) = 0;

        virtual void flipBuffers() = 0;
};

#endif
