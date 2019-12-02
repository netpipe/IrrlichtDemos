/*
Copyright (c) 2006-2008, Grégory Smialek

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted 
provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
  and the following disclaimer. 
* Redistributions in binary form must reproduce the above copyright notice, this list of
  conditions and the following disclaimer in the documentation and/or other materials provided with the distribution. 
* Neither the name of the Grégory Smialek nor the names of its contributors may be used
  to endorse or promote products derived from this software without specific prior written
  permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// GLAN8Viewer Version 9.5.7

/*
These library must be linked:
uuid.lib
Advapi32.lib
shell32.lib
comctl32.lib
User32.lib
Gdi32.lib
winmm.lib
opengl32.lib
glu32.lib
ole32.lib
oleaut32.lib
comdlg32.lib

glgx_*.lib
liban8_*.lib
*/

#include<windows.h>

#include"Win32_GLWindow.h"
#include"Engine.h"

// OpenGL Window
Win32_GLWindow mainGLWindow;

// OpenGL Viewer
Engine* engine = NULL;

// Top Left Menu
HMENU hMenu;
#define	IDM_OPEN	4001
#define	IDM_QUIT 	4002
#define	IDM_WIREMODE 4003
#define	IDM_FILLMODE 4004
#define	IDM_CULLFRONT 4007
#define	IDM_CULLBACK 4008
#define	IDM_CULLENABLE 4009
#define	IDM_TOONRENDER 4010

//--------------------------------------------------------------------------------------
// Function used to display messages (its pointer is given to the engine)
//--------------------------------------------------------------------------------------
void DisplayMessage(char* title, char* msg)
{
	MessageBox(mainGLWindow.hWnd,msg,title,MB_OK|MB_ICONEXCLAMATION);
}


//--------------------------------------------------------------------------------------
// WndProc 
//--------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc (HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN && wParam == VK_ESCAPE){mainGLWindow.IWantToExit();}

  switch (uMsg)											
  {
    case WM_CREATE:		
	  break;
		 
    case WM_SIZE:
	  mainGLWindow.SetScreenSize((int) LOWORD(lParam), (int) HIWORD(lParam)); // Resize the window
	  OnFrameResize((int) LOWORD(lParam), (int) HIWORD(lParam)); // Resize OpenGL viewer of the window
      InvalidateRect(hwnd,NULL,FALSE);       	
	  return 0;
		 
	case WM_SYSCOMMAND:							
	{
	  switch (wParam)							
	  {
	    case SC_SCREENSAVE:					
		case SC_MONITORPOWER:				
		return 0;				

		case IDM_OPEN:	// Load an another .an8 file
			{
				char filename[1024];
				char titlefilename[1024];
				if( Win32_PopFileOpenDlg(hwnd,filename,titlefilename) == TRUE )
				{
					engine->LoadFile(filename);
				}
			}
			return 0;

		case IDM_WIREMODE:	// Load an another .an8 file
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			CheckMenuItem(hMenu, IDM_WIREMODE, MF_CHECKED); 
			CheckMenuItem(hMenu, IDM_FILLMODE, MF_UNCHECKED); 
		}
		return 0;

		case IDM_FILLMODE:	// Load an another .an8 file
		{
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			CheckMenuItem(hMenu, IDM_FILLMODE, MF_CHECKED); 
			CheckMenuItem(hMenu, IDM_WIREMODE, MF_UNCHECKED); 
		}
		return 0;

		case IDM_CULLFRONT:
		{
			glFrontFace(GL_CCW);
			glEnable(GL_CULL_FACE);
			CheckMenuItem(hMenu, IDM_CULLFRONT, MF_CHECKED); 
			CheckMenuItem(hMenu, IDM_CULLBACK, MF_UNCHECKED); 
		}
		return 0;

		case IDM_CULLBACK:
		{
			glFrontFace(GL_CW);
			glEnable(GL_CULL_FACE);
			CheckMenuItem(hMenu, IDM_CULLBACK, MF_CHECKED); 
			CheckMenuItem(hMenu, IDM_CULLFRONT, MF_UNCHECKED); 
		}
		return 0;

		case IDM_CULLENABLE:
		{
			if( glIsEnabled(GL_CULL_FACE) == GL_TRUE)
			{
				glDisable(GL_CULL_FACE);
				CheckMenuItem(hMenu, IDM_CULLENABLE, MF_UNCHECKED);
				EnableMenuItem(hMenu, IDM_CULLBACK, MF_GRAYED); 
				EnableMenuItem(hMenu, IDM_CULLFRONT, MF_GRAYED); 
			}
			else
			{
				glEnable(GL_CULL_FACE);
				CheckMenuItem(hMenu, IDM_CULLENABLE, MF_CHECKED); 
				EnableMenuItem(hMenu, IDM_CULLBACK, MF_ENABLED );
				EnableMenuItem(hMenu, IDM_CULLFRONT, MF_ENABLED );
			}
		}
		return 0;

		case IDM_TOONRENDER:
		{
			if( engine != NULL)
			{
				if( engine->GetToonRenderingState() )
				{
					engine->SetToonRendering(false);
					CheckMenuItem(hMenu, IDM_TOONRENDER, MF_UNCHECKED); 
				}
				else
				{
					engine->SetToonRendering(true);
					CheckMenuItem(hMenu, IDM_TOONRENDER, MF_CHECKED); 
				}
			}

		}
		return 0;

		case IDM_QUIT:
			PostQuitMessage (0) ;
			return 0;
	  }
	  break;									
	}
		
	case WM_ACTIVATE:							
	{
	  if (!HIWORD(wParam))					
		{mainGLWindow.bIsActive = true;}
	  else
		{mainGLWindow.bIsActive = false;}
	  return 0;								
	}
		
	case WM_CLOSE:				
	  PostQuitMessage(0);	
	  return 0;     	   
   
	}													
	return DefWindowProc (hwnd, uMsg, wParam, lParam);																	
}


//--------------------------------------------------------------------------------------
// OnCreateDevice 
//--------------------------------------------------------------------------------------
bool OnCreateDevice( void )
{
	engine = new Engine(&DisplayMessage);
	engine->LoadFile("test.an8"); // Default scene
    return true;
}


//--------------------------------------------------------------------------------------
// Handle resize of the window
//--------------------------------------------------------------------------------------
void OnFrameResize( int width , int height )
{
	if( engine != NULL)
		engine->OnFrameResize(width,height);
}



//--------------------------------------------------------------------------------------
// Handle updates to the scene
//--------------------------------------------------------------------------------------
void OnFrameMove( double i_fTime, float i_fElapsedTime )
{
	// Nothing to do
}


//--------------------------------------------------------------------------------------
// Render the scene 
//--------------------------------------------------------------------------------------
void OnFrameRender( double i_fTime, float i_fElapsedTime )
{
	if( engine != NULL)
		engine->OnFrameRender(i_fTime,i_fElapsedTime);
}

//--------------------------------------------------------------------------------------
// Release resources created in the OnCreateDevice 
//--------------------------------------------------------------------------------------
void OnDestroyDevice( void )
{
	if(engine != NULL)
	{
		delete engine;
		engine = NULL;
	}
}

int WINAPI WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
#ifdef WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // Look for Memory leak !
	//_CrtSetBreakAlloc(2840); // To use to find the memory leak source
#endif

	// Create a window compatible with OpenGL 1.1
	if(!mainGLWindow.Create("GLAn8Viewer",640,480,32,false,WS_OVERLAPPEDWINDOW,WndProc))
	{
		MessageBox(NULL,"Can't create the window.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		char buffer[2048];
		sprintf(buffer,"%s",glewGetErrorString(err));
		MessageBox(NULL,buffer,"ERROR",MB_OK|MB_ICONEXCLAMATION);
	}

	int useGL3 = 6;//MessageBox(mainGLWindow.hWnd,"Do you want to use OpenGL 3.1 core ?","Information",MB_YESNO |MB_ICONEXCLAMATION);
	if( WGL_ARB_create_context && useGL3 == 6 )
	{
		HGLRC     hRC;
		int attribList[] =
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
			WGL_CONTEXT_MINOR_VERSION_ARB, 1,
			WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0, 0
		};

		// Try OpenGL 3.1
		if (!(hRC = wglCreateContextAttribsARB(mainGLWindow.hDC, 0, attribList)))
		{
			//MessageBox(NULL,"OpenGL 3.1 context is not supported by your system.\nAn OpenGL 3.0 context will be tried to be created instead.","Information",MB_OK|MB_ICONEXCLAMATION);

			// Try OpenGL 3.0
			attribList[3] = 0;
			if (!(hRC = wglCreateContextAttribsARB(mainGLWindow.hDC, 0, attribList)))
			{
				//MessageBox(NULL,"OpenGL 3.0 context is not supported by your system.\nAn OpenGL 2.x context will be used instead.","Information",MB_OK|MB_ICONEXCLAMATION);
				SetWindowText(mainGLWindow.hWnd,"GLAn8Viewer - Using OpenGL 2");
			}
			else
				SetWindowText(mainGLWindow.hWnd,"GLAn8Viewer - Using OpenGL 3.0");
		}
		else
		{
			SetWindowText(mainGLWindow.hWnd,"GLAn8Viewer - Using OpenGL 3.1");
		}

		if (!wglMakeCurrent(mainGLWindow.hDC, hRC))
			MessageBox(NULL,"wglMakeCurrent() failed for OpenGL 3.x context.","Information",MB_OK|MB_ICONEXCLAMATION);
	
		wglDeleteContext(mainGLWindow.hRC);
		mainGLWindow.hRC = hRC;
	}
	else
	{
		SetWindowText(mainGLWindow.hWnd,"GLAn8Viewer - Using OpenGL 2");
	}

	

	

	if( ! GLEW_ARB_shading_language_100 )
	{
		MessageBox(NULL,"OpenGL 2.0 is not supported by your hardware","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return 0;
	}
	
	//if(WGL_EXT_swap_control) 
	//	wglSwapIntervalEXT(0);

	// Initialize the engine and OpenGL Rendering
	OnCreateDevice();

	// Windows do not always have the requested size
	RECT rcClient;GetClientRect(mainGLWindow.hWnd,&rcClient);
	OnFrameResize(rcClient.right - rcClient.left ,rcClient.bottom - rcClient.top);

	// Menu creation
	hMenu= GetSystemMenu(mainGLWindow.hWnd, FALSE); 
	AppendMenu(hMenu, MF_STRING, IDM_OPEN, TEXT("Open"));
	AppendMenu(hMenu, MF_STRING, IDM_WIREMODE, TEXT("Wire Mode"));
	AppendMenu(hMenu, MF_STRING, IDM_FILLMODE, TEXT("Fill Mode"));
	CheckMenuItem(hMenu, IDM_FILLMODE, MF_CHECKED); 

	AppendMenu(hMenu, MF_STRING, IDM_CULLENABLE, TEXT("Enable Culling"));
	AppendMenu(hMenu, MF_STRING, IDM_CULLFRONT, TEXT("Cull Front Face"));
	AppendMenu(hMenu, MF_STRING, IDM_CULLBACK, TEXT("Cull Back Face"));
	CheckMenuItem(hMenu, IDM_CULLBACK, MF_CHECKED); 
	EnableMenuItem(hMenu, IDM_CULLBACK, MF_GRAYED); 
	EnableMenuItem(hMenu, IDM_CULLFRONT, MF_GRAYED); 

	AppendMenu(hMenu, MF_STRING, IDM_TOONRENDER, TEXT("Enable Toon Rendering"));
	CheckMenuItem(hMenu, IDM_TOONRENDER, MF_UNCHECKED); 
	

	AppendMenu(hMenu, MF_SEPARATOR,0,NULL);
	AppendMenu(hMenu, MF_STRING, IDM_QUIT, TEXT("Quit"));

	// Launch the message loop
	mainGLWindow.Loop(&OnFrameMove,&OnFrameRender,&OnDestroyDevice);

    mainGLWindow.Kill(); 
	OnDestroyDevice();

    return 0;
}
