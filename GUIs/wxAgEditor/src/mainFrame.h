#ifndef MAINFRAME_H
#define MAINFRAME_H

#include "wx/wx.h"
#include "renderWindow.h"
#include "Config/config.h"

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString &title);

	void OnQuit(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
	void OnPaint(wxPaintEvent &event);
	void OnResize(wxSizeEvent &event);

	void OnOpen(wxCommandEvent &event);
	void OnSave(wxCommandEvent &event);

private:
	Config *configuration;

	wxMenu *fileMenu;
	wxMenu *helpMenu;
	wxMenu *createMenu;
	wxMenuBar *menuBar;

	wxPanel *leftPanel;
	renderWindow *renderWin;

	bool sceneSaved;

	DECLARE_EVENT_TABLE();
};

#endif
