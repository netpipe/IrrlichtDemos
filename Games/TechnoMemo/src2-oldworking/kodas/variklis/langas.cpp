#include "langas.hpp"


//SLangas KLangas::*langai = new SLangas[LANGAS_SK];
//vector<SLangas> KLangas::langai(0);
bool KLangas::baigti = false;

KLangas::KLangas(): sukurta(false)
{
}


KLangas::~KLangas()
{
}


bool KLangas::Sukurti(bool rod)
{
	//_TODO: Windowed rezime kompensuoti remeliu uzimama plota - padidinti lango matmenis
	int pozX, pozY;
	if(nust.fullScr){
		pozX = 0;
		pozY = 0;
	}
	else{
		pozX = nust.pozX;
		pozY = nust.pozY;
	}
	
	if(sukurta){
		Naikinti();
		sukurta = false;
	}
	
	int			PixelFormat;
	WNDCLASS	wc;
	DWORD		dwExStyle;
	DWORD		dwStyle;
	RECT		wndRect;
	wndRect.left 	= long(pozX);
	wndRect.right	= long(pozX + nust.dydisX);
	wndRect.top		= long(pozY);
	wndRect.bottom	= long(pozY + nust.dydisY);

	hInst				= GetModuleHandle(NULL);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc		= Callback;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInst;
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= NULL;
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= nust.wndClass;

	if (!RegisterClass(&wc))									//Attempt To Register The Window Class
	{
		MessageBox(NULL, "Klaida registruojant 'wc'", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (nust.fullScr)
	{
		DEVMODE mode;						//Device Mode
		memset(&mode, 0, sizeof(mode));		//Isvalyti atminti
		mode.dmSize = sizeof(mode);
		mode.dmPelsWidth	= nust.dydisX;			//Lango plotis
		mode.dmPelsHeight	= nust.dydisY;			//Lango aukstis
		mode.dmBitsPerPel	= nust.bpp;				//Bits per pixel
		mode.dmDisplayFrequency = nust.fps;       	//FPS
		mode.dmFields = DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT|DM_DISPLAYFREQUENCY;

		if (ChangeDisplaySettingsEx(
		  nust.gpuStr,			//GPU pavadinimas
		  &mode,				//Nustatymai
		  NULL,					//Nenaudojamas, NULL
		  CDS_FULLSCREEN,		//Naudoti FullScreen
		  NULL					//VIDEOPARAMETERS struktura
		) != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBox(NULL, "Parinkti vaizdo nustatymai nesuderinami su Jusu sistema", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
            return false;
		}
	}

	if (nust.fullScr)
	{
		dwExStyle = WS_EX_APPWINDOW;	//Window Extended Style
		dwStyle = WS_POPUP;				//Windows Style
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW |
		 WS_EX_WINDOWEDGE;
		dwStyle =
		 WS_BORDER |					//Lango remeliai
		 WS_CAPTION |					//Lango pavadinimas
         WS_SYSMENU |					//Sisteminis meniu
         WS_MINIMIZEBOX |				//Minimize/Close spaudukai
         WS_CLIPSIBLINGS |
         WS_CLIPCHILDREN;
	}

	AdjustWindowRectEx(&wndRect, dwStyle, FALSE, dwExStyle);	//Nustatyti lango dydi

	//Kurti langa
	if (!(hWnd = CreateWindowEx(
		dwExStyle,					//Extended Style For The Window
		nust.wndClass,				//Class Name
		nust.pavad,					//Lango pavadinimas
		dwStyle |					//Defined Window Style
		WS_CLIPSIBLINGS |			//Required Window Style
		WS_CLIPCHILDREN,			//Required Window Style
		pozX, pozY,		//Lango pozicija
		nust.dydisX,				//Lango ilgis
		nust.dydisY,				//Lango aukstis
		NULL,						//No Parent Window
		NULL,						//No Menu
		hInst,						//Instance
		NULL)))						//Papildomi duomenys
	{
		MessageBox(NULL, "Klaida kuriant langa: CreateWindow", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		Naikinti();
		return false;
	}

	static	PIXELFORMATDESCRIPTOR pfd =	//pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),	//Size Of This Pixel Format Descriptor
		1,								//Version Number
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,				//Buferio formatas
		PFD_TYPE_RGBA,					//Spalvu formatas
		nust.bpp,						//Bits per pixel
		0, 0, 0, 0, 0, 0,				//Color Bits Ignored
		0,								//No Alpha Buffer
		0,								//Shift Bit Ignored
		nust.accumBpp,					//Accumulation buferio gylis
		0, 0, 0, 0,						//Accumulation Bits Ignored
		nust.zBpp,						//Z-Buferio gylis
		nust.stencilBpp,				//Stencil buferio gylis
		0,								//Auxiliary buferio gylis
		PFD_MAIN_PLANE,					//Main Drawing Layer
		0,								//Reserved
		0, 0, 0							//Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))			//Ar gautas Device Context?
	{
		Naikinti();
		MessageBox(NULL, "Klaida nustatant 'GL Device Context'", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	//Ar gautas Pixel Format?
	{
		Naikinti();
		MessageBox(NULL, "Klaida pasirenkant PixelFormat", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!SetPixelFormat(hDC, PixelFormat, &pfd))		//Ar pavyko nustatyti Pixel Format?
	{
		Naikinti();
		MessageBox(NULL, "Klaida nustatant PixelFormat", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if (!(hRC = wglCreateContext(hDC)))				//Ar gautas Rendering Context?
	{
		Naikinti();
		MessageBox(NULL,"Klaida kuriant 'GL Rendering Context'","KLAIDA",MB_OK|MB_ICONEXCLAMATION);
		return false;
	}

	if(!wglMakeCurrent(hDC, hRC))					//Ar aktyvuotas Rendering Context
	{
		Naikinti();
		MessageBox(NULL, "Klaida aktyvuojant 'GL Rendering Context'", "KLAIDA", MB_OK|MB_ICONEXCLAMATION);
		return false;
	}
	
	/*SLangas *_lang = new SLangas;
	_lang->lang = this;
	_lang->hWnd = hWnd;
	langai.push_back(*_lang);*/
	
	if(rod)
		Rodyti();

	sukurta = true;
	return true;
}


void KLangas::KeistiDydi(int _x, int _y)
{
}


void KLangas::Rodyti()
{
	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	if(nust.fullScr)
		ShowCursor(false);
}


LRESULT CALLBACK KLangas::Callback(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
/*	//Zinute persiunciama reikiamam langui
	for(int i = 0; i < langai.size(); i++)
		if(wnd == langai[i].hWnd){
			return langai[i].lang->Zinute(msg, wParam, lParam);
		}
	return 0;
}


LRESULT KLangas::Zinute(UINT msg, WPARAM wParam, LPARAM lParam)
{*/
	switch(msg){
		case WM_CREATE:
			return 0;
		case WM_ACTIVATE:
			if(LOWORD(wParam) == WA_INACTIVE){
				//Langas neaktyvus
			}
			else{
				//Langas aktyvuojamas
			}
			if(HIWORD(wParam) != 0){
				//Minimize
				//baigti = false;
			}
			else{
				//Ne minimize
				//baigti = true;
			}
			return 0;
		case WM_SYSCOMMAND:
			switch(wParam){
				case SC_SCREENSAVE:{
					//Ijungiamas ScreenSaver'is
					return 0;
				}
				case SC_MONITORPOWER:{
					//Uzmigo monitorius
					return 0;
				}
			}
			break;
		case WM_KILLFOCUS:
			return 0;
		case WM_SETFOCUS:
			return 0;
		case WM_CLOSE:					//Gauta zinute Close?
			baigti = true;
			return 0;
		/*case WM_MOVE:
			//x = LOWORD(lParam);
			//y = HIWORD(lParam);
			return 0;*/
		case WM_SIZE:					//Keisti lango dydi
			if(wParam == SIZE_MAXIMIZED){
				//Maximize
			}
			if(wParam == SIZE_MINIMIZED){
				//Minimize
			}
			KeistiDydi(LOWORD(lParam), HIWORD(lParam));
			return 0;
		
		//Klaviaturos valdymas
		case WM_KEYDOWN:				//Paspaustas spaudukas?
			klav.Down(wParam);
			return 0;
		case WM_KEYUP:					//Atleistas spaudukas?
			klav.Up(wParam);
			return 0;
		
		//Peles valdymas
		case WM_MOUSEMOVE:
			//x = GET_X_LPARAM(lParam); 
			//y = GET_Y_LPARAM(lParam); 
			return 0;
		case WM_LBUTTONDOWN:
			pele.Kair(true);
			return 0;
		case WM_LBUTTONUP:
			pele.Kair(false);
			return 0;
		case WM_RBUTTONDOWN:
			pele.Des(true);
			return 0;
		case WM_RBUTTONUP:
			pele.Des(false);
			return 0;
		case WM_MBUTTONDOWN:
			pele.Vid(true);
			return 0;
		case WM_MBUTTONUP:
			pele.Vid(false);
			return 0;
		case WM_MOUSEWHEEL:
			pele.Ratukas(GET_WHEEL_DELTA_WPARAM(wParam) / 120);
			return 0;
	}
	return DefWindowProc(wnd, msg, wParam, lParam);
}


void KLangas::Atnaujinti()
{
	#ifdef DEBUG
	if(!sukurta)
		Logas::Klaida("Langas->Atnaujinti: nesukurtas");
	#endif
	while(PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))	//Gauta zinute?
	{
		if (msg.message == WM_QUIT)
			baigti = true;
		else
		{
			TranslateMessage(&msg);		//Konvertuoti zinute
			DispatchMessage(&msg);		//Perduoti zinute Callback funkcijai
		}
	}
}


void KLangas::SukeistiBuferius()
{
	SwapBuffers(hDC);
}


void KLangas::Naikinti()
{
	//Naikinti langa
	//
	
	//Langa pasalinti is saraso
	/*vector<SLangas>::const_iterator iter;
	for(iter = langai.begin(); iter != langai.end(); iter++)
		if(iter->hWnd == hWnd){
			langai.erase(iter);
			break;
		}*/
	
	/*for(int i = 0; i < langai.size(); i++)
		if(langai[i].hWnd == hWnd){
			langai.erase(langai.begin() + i);
			break;
		}*/

	if (nust.fullScr)
		ChangeDisplaySettings(NULL, 0);

	if (hRC)											//Yra Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))				//Ar galima atlaisvinti DC ir RC Contextus?
		{
			Logas::Klaida("Klaida atlaisvinant DC ir RC");
		}

		if (!wglDeleteContext(hRC))						//Ar galima naikinti RC?
		{
			Logas::Klaida("Klaida atlaisvinant Rendering Context");
		}
		hRC = NULL;
	}

	if (hDC) if(!ReleaseDC(hWnd,hDC))					//Ar galima atlaisvinti DC?
	{
		Logas::Klaida("Klaida atlaisvinant Device Context");
		hDC = NULL;
	}

	if (hWnd) if(!DestroyWindow(hWnd))					//Ar galima naikinti langa?
	{
		Logas::Klaida("Klaida atlaisvinant hWnd.");
		hWnd = NULL;
	}

	if (!UnregisterClass(nust.wndClass, hInst))
	{
		Logas::Klaida("Klaida atlaisvinant hInstance");
		hInst = NULL;
	}
	
	ShowCursor(true);
	sukurta = false;
}
