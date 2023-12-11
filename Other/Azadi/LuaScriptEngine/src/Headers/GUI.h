#ifndef AZGUI_H
#define AZGUI_H

#include <string>

class azGUI
{
    public:
        virtual ~azGUI() {}

        virtual void updateArea(const char*, const std::string& data, int type) = 0;
		virtual void updateArea(const char*, int data, int type) = 0;
		virtual void updateArea(const char*, float data, int type) = 0;
		virtual void setActive(const std::string& itemName, bool) = 0;
};

#endif
