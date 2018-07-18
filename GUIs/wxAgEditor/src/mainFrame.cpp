#include "mainFrame.h"

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
	EVT_MENU(wxID_EXIT, MainFrame::OnQuit)
	EVT_MENU(wxID_OPEN, MainFrame::OnOpen)
	EVT_MENU(wxID_SAVEAS, MainFrame::OnSave)
	EVT_SIZE(MainFrame::OnResize)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title)
: wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(640, 480))
{
	sceneSaved = false;

	fileMenu = new wxMenu;
	helpMenu = new wxMenu;
	createMenu = new wxMenu;

	helpMenu->Append(wxID_ABOUT, wxT("&About\tF1"), wxT("Display about dialogue"));
	fileMenu->Append(wxID_OPEN, wxT("&Open Scene\tF2"), wxT("Open Scene File"));
	fileMenu->Append(wxID_SAVEAS, wxT("Save &As\tF3"), wxT("Save Scene File"));
	fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"), wxT("Exit Program"));

	menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, wxT("&File"));
	menuBar->Append(createMenu, wxT("&Create"));
	menuBar->Append(helpMenu, wxT("&Help"));

	SetMenuBar(menuBar);

	CreateStatusBar(2);
	SetStatusText(wxT("Test"));

	leftPanel = new wxPanel(this, wxID_ANY, wxPoint(1, 1), wxSize(200, 412));
	leftPanel->Show();

	renderWin = new renderWindow(this, wxPoint(202, 1), wxSize(429, 412));
	renderWin->Show();

	configuration = new Config();
	if (configuration->load("./AgEditor.cfg") != -1)
	{
		this->SetSize(Integer::parseInt(configuration->fields->getProperty("WindWidth", "640")), Integer::parseInt(configuration->fields->getProperty("WindHeight", "480")));
	}
}

void MainFrame::OnResize(wxSizeEvent &event)
{
//	if (!this->IsVisible())
//		return;
//	int w, h;
//	this->GetSize(&w, &h);
//	leftPanel->SetSize(200, h-68);
	//renderWin->SetSize(w-211, h-68);
}

void MainFrame::OnOpen(wxCommandEvent &event)
{
	wxFileDialog openFile(this, wxT("Open Scene"), wxT("./"), wxEmptyString, wxT("SCN files (*.scn)|*.scn|IRR files (*.irr)|*.irr|XML files (*.xml)|*.xml"),wxFD_OPEN);
	if (openFile.ShowModal() == wxID_OK)
	{
		renderWin->loadScene(std::wstring(openFile.GetPath().c_str()));
	}
	wxPaintEvent temp;
	renderWin->OnPaint(temp);
}

void MainFrame::OnSave(wxCommandEvent &event)
{
	wxFileDialog saveFile(this, wxT("Save Scene"), wxT("./"), wxEmptyString, wxT("SCN files (*.scn)|*.scn|IRR files (*.irr)|*.irr|XML files (*.xml)|*.xml"), wxFD_SAVE);
	if (saveFile.ShowModal() == wxID_OK)
	{
		renderWin->saveScene(std::wstring(saveFile.GetPath().c_str()));
	}
	wxPaintEvent temp;
	renderWin->OnPaint(temp);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
	wxString msg;
	msg.Printf(wxT("AgEditor v0.1a"));

	wxMessageBox(msg, wxT("About AgEditor"), wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnQuit(wxCommandEvent &event)
{
	int w, h;
	this->GetSize(&w, &h);
	configuration->fields->setProperty("WindWidth", Integer::toInt(w));
	configuration->fields->setProperty("WindHeight", Integer::toInt(h));
	configuration->save("./AgEditor.cfg");
	delete configuration;
	Close();
}
