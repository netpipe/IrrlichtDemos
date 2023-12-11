#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class MyApp : public wxApp
{
	public:
		virtual bool OnInit();
};
IMPLEMENT_APP(MyApp);

class MyFrame: public wxFrame
{
	public:
		MyFrame(wxFrame *frame, const wxString& title);
		~MyFrame();
};

bool MyApp::OnInit()
{
	MyFrame* frame = new MyFrame(0L, _("Azadi - Console"));
	frame->Show();
	return true;
}

MyFrame::MyFrame(wxFrame *frame, const wxString& title)
	: wxFrame(frame, -1, title)
{

}

MyFrame::~MyFrame()
{

}
