/* Copyright (c) <2009> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#ifndef __MAIN_FRAME__
#define __MAIN_FRAME__


#include "SceneManager.h"

class NewtonSdkApp: public wxApp
{
	virtual bool OnInit();
};

class RenderCanvas;
class DGCamera;

class NewtonFrame: public wxFrame, public SceneManager
{
public:

	NewtonFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~NewtonFrame();

	DGCamera* GetDGCamera ();	

	private:

	void OnNewScene(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);
	void OnFileOpenVisualScene(wxCommandEvent& event);
	void OnFileOpenPhysicsScene(wxCommandEvent& event);
	void OnFileSaveVisualScene(wxCommandEvent& event);
	void OnFileSavePhysicsScene(wxCommandEvent& event);
	void onSDKDemo (wxCommandEvent& event);
	void OnShowCollision(wxCommandEvent& event);


	void LoadIcon (char* name, wxBitmap& map);
	

	void IniWorld ();
	

	RenderCanvas* m_canvas;
	wxToolBarBase *m_toolBar;	
	DECLARE_EVENT_TABLE()
};


#endif
