#ifndef _MODULE_WIN32_GLWINDOWS
#define _MODULE_WIN32_GLWINDOWS

#include<windows.h>
#include"GLAN8/GLGX/GLGX.h"	



class Win32_GLWindow
{
public:
    int width;  // Current size of the window
    int height; // Current size of the window

    int originalWidth; // Original size of the desktop screen
    int originalHeight;// Original size of the desktop screen

    DWORD style;
    bool fullscreen;
    
    HINSTANCE hInstance;
    HWND	  hWnd;	
    HDC		  hDC;
    HGLRC	  hRC;

    bool bIsActive;
	bool bWantToExit;
    
    Win32_GLWindow(void);
    ~Win32_GLWindow(void);

	void SetScreenSize(int width, int height);
    
	bool IsActive(void);
	void IWantToExit(void);

    bool Create(char* title, int width, int height, int bits, bool fullscreenflag,DWORD dwStyle,WNDPROC WndProc);
    void Loop(void (*ptrFctFrameMove)(double,float),void (*ptrFctFrameRender)(double,float),void (*ptrFctOnDestroy)(void));
	void Kill(void);		
};

void OnFrameResize( int width , int height );

BOOL Win32_PopFileOpenDlg (HWND hwnd, char* pstrFileName, char* pstrTitleName);


#endif //_MODULE_WIN32_GLWINDOWS
