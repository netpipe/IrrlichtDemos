#include "main.h"

IMPLEMENT_APP(AgEditor);

bool AgEditor::OnInit()
{
	MainFrame *frame = new MainFrame(wxT("Agnostic Games Editor v0.1a"));

	frame->Show();

	return true;
}