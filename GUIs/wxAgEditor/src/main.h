#ifndef MAIN_H
#define MAIN_H

#include "wx/wx.h"
#include "mainFrame.h"

class AgEditor : public wxApp
{
public:
	virtual bool OnInit();
};

DECLARE_APP(AgEditor);

#endif
