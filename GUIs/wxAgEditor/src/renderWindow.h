#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <vector>
#include <string>
#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "irrlicht.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class renderWindow : public wxGLCanvas
{
public:
	renderWindow(wxFrame *parent, const wxPoint &point=wxPoint(0,0), const wxSize &size=wxSize(100,100));
	~renderWindow();

	void OnPaint(wxPaintEvent &event);
	void OnResize(wxSizeEvent &event);

	void saveScene(const std::wstring &filename);
	void loadScene(const std::wstring &filename);

private:
	IrrlichtDevice *device;

	IVideoDriver *driver;
	ISceneManager *smgr;
	IGUIEnvironment *guienv;
	ICameraSceneNode *cam;

	std::vector<ISceneNode *> sceneNodes;

	bool deviceActive;

	DECLARE_EVENT_TABLE();
};

#endif
