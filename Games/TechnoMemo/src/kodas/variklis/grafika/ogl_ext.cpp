#include "ogl_ext.hpp"


HDC OglExt::hDC = NULL;
HGLRC OglExt::hRC = NULL;
HWND OglExt::hWnd = NULL;
HINSTANCE OglExt::hInstance = NULL;
int OglExt::PixelFormat = 0;
const char * OglExt::exts = NULL;
GLubyte * (APIENTRY * OglExt::wglGetExtensionsStringARB)(HDC) = NULL;
BOOL (APIENTRY * OglExt::wglChoosePixelFormatARB)(HDC, int const *, FLOAT const *, UINT, int *, UINT *) = NULL;
GLvoid (APIENTRY * OglExt::glSampleCoverage)(GLclampf, GLboolean) = NULL;
GLvoid (APIENTRY * OglExt::glActiveTexture)(GLenum) = NULL; 
GLvoid (APIENTRY * OglExt::glMultiTexCoord2f)(GLenum, GLfloat, GLfloat) = NULL;

bool OglExt::WGLMultisample = false;
bool OglExt::Multitexture = false;
bool OglExt::Cubemap = false;
bool OglExt::Anisotropic = false;


void OglExt::Sukurti(HDC hDC) throw(const char*)
{
	//Sukuriamas OpenGL kontekstas
	//if(!InitGL())
	//	throw("Nepavyko sukurti lango");

	//Gauti extension'u sarasa
	if(hDC != NULL){
		//Gauti funkcijos wglGetExtensionsStringARB adresa
		wglGetExtensionsStringARB = (GLubyte * (APIENTRY*)(HDC)) wglGetProcAddress("wglGetExtensionsStringARB");
		if(wglGetExtensionsStringARB == NULL)
			throw("Nepavyko gauti funkcijos wglGetExtensionStringARB adreso");
		
		//Gauti extension'us
		exts = (const char*) wglGetExtensionsStringARB(hDC);
	}
	else{
		exts = (const char*) glGetString(GL_EXTENSIONS);
	}
	
	
	//Tikrinti, kurie extension'ai palaikomi
	if(!strstr(exts, "WGL_ARB_multisample")){
		WGLMultisample = false;
		//throw("Nepalaikomas WGL_ARB_multisample");
	}
	else{
		WGLMultisample = true;
	}
	
	if(!strstr(exts, "GL_ARB_multitexture")){
		Multitexture = false;
	}
	else{
		Multitexture = true;
	}
	
	if(!strstr(exts, "GL_EXT_texture_cube_map")){
		Cubemap = false;
	}
	else{
		Cubemap = true;
	}
	
	if(!strstr(exts, "GL_EXT_texture_filter_anisotropic")){
		Anisotropic = false;
	}
	else{
		Anisotropic = true;
	}
	
	
	//Gauti funkciju adresus
	//Gauti funkcijos wglChoosePixelFormatARB adresa
	wglChoosePixelFormatARB = (BOOL (APIENTRY*)(HDC, int const *, FLOAT const *, UINT, int *, UINT *)) wglGetProcAddress("wglChoosePixelFormatARB");   
	if(wglChoosePixelFormatARB == NULL)
		throw("Nepavyko gauti funkcijos wglChoosePixelFormatARB adreso");
	
	//Gauti funkcijos glSampleCoverage adresa
	glSampleCoverage = (GLvoid (APIENTRY*)(GLclampf, GLboolean)) wglGetProcAddress("glSampleCoverage");   
	if(glSampleCoverage == NULL)
		throw("Nepavyko gauti funkcijos glSampleCoverage adreso");
	
	//Gauti funkcijos glActiveTextureERB adresa
	glActiveTexture = (GLvoid (APIENTRY*)(GLenum)) wglGetProcAddress("glActiveTexture");
	if(glActiveTexture == NULL)
		throw("Nepavyko gauti funkcijos glActiveTexture adreso");
	
	//Gauti funkcijos glMultiTexCoord2f adresa
	glMultiTexCoord2f = (GLvoid (APIENTRY*)(GLenum, GLfloat, GLfloat)) wglGetProcAddress("glMultiTexCoord2f");
	if(glMultiTexCoord2f == NULL)
		throw("Nepavyko gauti funkcijos glMultiTexCoord2f adreso");
	
	//Naikinamas OpenGL kontekstas
		//CloseGL();
}


/*bool OglExt::InitGL() throw(const char*)
{
	WNDCLASS	wc;						//Windows Class Structure
	DWORD		dwStyle;				//Window Style
	RECT		WindowRect;				//Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			//Left
	WindowRect.right = (long)640;		//Right
	WindowRect.top = (long)0;			//Top
	WindowRect.bottom = (long)480;		//Bottom

	hInstance			= GetModuleHandle(NULL);				//Grab An Instance For Our Window
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	//Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC)WndProc;						//WndProc Handles Messages
	wc.cbClsExtra		= 0;									//No Extra Window Data
	wc.cbWndExtra		= 0;									//No Extra Window Data
	wc.hInstance		= hInstance;							//Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			//Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			//Load The Arrow Pointer
	wc.hbrBackground	= NULL;									//No Background Required For GL
	wc.lpszMenuName		= NULL;									//We Don't Want A Menu
	wc.lpszClassName	= "OpenGL";								//Set The Class Name

	if (!RegisterClass(&wc))									//Attempt To Register The Window Class
	{
		MessageBox(NULL, "Klaida registruojant \"wc\"", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	dwStyle = WS_OVERLAPPEDWINDOW;							//Windows Style

	//AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);	//Nustatyti lango dydi

	//Kurti langa
	if (!(hWnd = CreateWindow(
								"OpenGL",							//Class Name
								"langas",							//Lango pavadinimas
								dwStyle |							//Defined Window Style
								WS_CLIPSIBLINGS |					//Required Window Style
								WS_CLIPCHILDREN,					//Required Window Style
								0, 0,								//Lango pozicija
								WindowRect.right-WindowRect.left,	//Lango ilgis
								WindowRect.bottom-WindowRect.top,	//Lango aukstis
								NULL,								//No Parent Window
								NULL,								//No Menu
								hInstance,							//Instance
								NULL)))								//Dont Pass Anything To WM_CREATE
	{
		CloseGL();
		MessageBox(NULL, "Klaida kuriant langa", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	static	PIXELFORMATDESCRIPTOR pfd =				//pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				//Size Of This Pixel Format Descriptor
		1,											//Version Number
		PFD_DRAW_TO_WINDOW |						//Format Must Support Window
		PFD_SUPPORT_OPENGL |						//Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							//Must Support Double Buffering
		PFD_TYPE_RGBA,								//Request An RGBA Format
		32,											//Bits per pixel
		0, 0, 0, 0, 0, 0,							//Color Bits Ignored
		0,											//No Alpha Buffer
		0,											//Shift Bit Ignored
		0,											//Accumulation Buffer
		0, 0, 0, 0,									//Accumulation Bits Ignored
		16,											//Z-Buferio gylis 
		0,											//No Stencil Buffer
		0,											//No Auxiliary Buffer
		0,											//Main Drawing Layer
		0,											//Reserved
		0, 0, 0										//Layer Masks Ignored
	};
	
	if (!(hDC = GetDC(hWnd)))						//Ar gautas Device Context?
	{
		CloseGL();
		MessageBox(NULL, "Klaida nustatant \"GL Device Context\"", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))){	//Ar gautas Pixel Format?
		CloseGL();
		MessageBox(NULL, "Klaida pasirenkant PixelFormat", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!SetPixelFormat(hDC, PixelFormat, &pfd))		//Ar pavyko nustatyti Pixel Format?
	{
		CloseGL();
		MessageBox(NULL, "Klaida nustatant PixelFormat", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (!(hRC = wglCreateContext(hDC)))				//Ar gautas Rendering Context?
	{
		CloseGL();
		MessageBox(NULL,"Klaida kuriant \"GL Rendering Context\"","KLAIDA",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!wglMakeCurrent(hDC, hRC))					//Ar aktyvuotas Rendering Context
	{
		CloseGL();
		MessageBox(NULL, "Klaida aktyvuojant \"GL Rendering Context\"", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	ShowWindow(hWnd, SW_SHOW);						//Rodyti langa
	SetForegroundWindow(hWnd);						//Slightly Higher Priority
	SetFocus(hWnd);									//Sets Keyboard Focus To The Window
	
	return true;
}


void OglExt::CloseGL()
{
	if (hRC)											//Yra Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))				//Ar galima atlaisvinti DC ir RC Contextus?
		{
			MessageBox(NULL, "Klaida atlaisvinant DC ir RC", "KLAIDA", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						//Ar galima naikinti RC?
		{
			MessageBox(NULL, "Klaida atlaisvinant Rendering Context", "KLAIDA", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					//Ar galima atlaisvinti DC
	{
		MessageBox(NULL, "Klaida atlaisvinant Device Context", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}

	if (hWnd && !DestroyWindow(hWnd))					//Ar galima naikinti langa?
	{
		MessageBox(NULL, "Klaida atlaisvinant hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}

	if (!UnregisterClass("OpenGL",hInstance))
	{
		MessageBox(NULL, "Klaida atlaisvinant hInstance", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}
}


//LRESULT CALLBACK
LRESULT CALLBACK OglExt::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	MessageBox(NULL, "WND proc", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
	
	switch (uMsg)									//Check For Windows Messages
	{
		case WM_ACTIVATE:							//Watch For Window Activate Message
		{
			return 0;
		}

		case WM_SYSCOMMAND:							//Intercept System Commands
		{
			switch (wParam)							//Check System Calls
			{
				case SC_SCREENSAVE:					//Jungiamas Sreensaveris?
				case SC_MONITORPOWER:				//Monitorius uzmigo?
				return 0;
			}
			break;
		}

		case WM_CLOSE:								//Gauta zinute Close?
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}*/
