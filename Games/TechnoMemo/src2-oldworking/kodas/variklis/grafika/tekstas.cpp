#include "tekstas.hpp"


KTekstas::KTekstas(): sukurtas(false)
{
}


KTekstas::~KTekstas()
{
}


#ifdef OS_WIN32
bool KTekstas::Sukurti(HDC hDC)
{
	return Sukurti(hDC, 12, "Tunga");
}


bool KTekstas::Sukurti(HDC hDC, int dydis, const char *sriftas)
{
	HFONT	font;										//Windows Font ID
	HFONT	oldfont;
	
	fontDispList = glGenLists(223);
	if(fontDispList == 0)
		return false;
	font = CreateFont(
		-dydis,							//Aukstis
		0,								//Plotis
		0,								//Angle Of Escapement
		0,								//Orientation Angle
		FW_NORMAL,						//Font Weight
		FALSE,							//Pasvires
		FALSE,							//Pabrauktas
		FALSE,							//Perbrauktas
		BALTIC_CHARSET,					//Koduote
		OUT_TT_PRECIS,					//Output Precision
		CLIP_DEFAULT_PRECIS,			//Clipping Precision
		ANTIALIASED_QUALITY,			//Output Quality
		FF_DONTCARE | DEFAULT_PITCH,	//Family And Pitch
		sriftas);					    //Sriftas
	if(font == NULL)
		return false;
	oldfont = (HFONT)SelectObject(hDC, font);
	if(oldfont == NULL || oldfont == HGDI_ERROR)
		return false;
	if(!wglUseFontBitmaps(hDC, 32, 223, fontDispList))
		return false;
	SelectObject(hDC, oldfont);
	if(DeleteObject(font) == 0)
		return false;
	
	sukurtas = true;
	return true;
}
#endif


#ifdef OS_UNIX
bool KTekstas::Sukurti(Display *display)
{
	return Sukurti(display, 12, "adobe-courier");
}


bool KTekstas::Sukurti(Display *display, int dydis, const char *sriftas)
{
	char	formatas[128];
	XFontStruct	*fontStruct;
	
	if(sukurtas){
		Naikinti();
		#ifdef DEBUG
		Logas::Log("Tekstas->Kuriamas dar karta");
		#endif
	}
	
	sprintf(formatas, "-%s-*-r-normal--%i-*", sriftas, dydis);
	fontStruct = XLoadQueryFont(display, formatas);
	if(fontStruct == NULL)
		return false;
	glXUseXFont(fontStruct->font_id, 32, 223, 32);
	//_TODO
		
	sukurtas = true;
	return true;
}
#endif


//Piesiama viena teksto eilute
void KTekstas::Piesti(const char *eil)
{
	if(eil == NULL)
		return;
	glPushAttrib(GL_LIST_BIT);
	glListBase(fontDispList - 32);
	glCallLists(strlen(eil), GL_UNSIGNED_BYTE, eil);
	glPopAttrib();
}


//Piesiama teksto eilute. Pries piesima formatuojama
void KTekstas::PiestiNF(const char *fmt, ...)
{
	char		text[256];								// Holds Our String
	va_list		ap;										// Pointer To List Of Arguments

	if (fmt == NULL)									// If There's No Text
		return;											// Do Nothing

	va_start(ap, fmt);									// Parses The String For Variables
    vsprintf(text, fmt, ap);                            // And Converts Symbols To Actual Numbers
	va_end(ap);											// Results Are Stored In Text

	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(fontDispList - 32);						// Sets The Base Character to 32
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
	glPopAttrib();										// Pops The Display List Bits
}


//Naikinami duomenys
void KTekstas::Naikinti()
{
	if(sukurtas)
    	glDeleteLists(fontDispList, 223);
    sukurtas = false;
}


//------------------------------------------------------------------------------

/*
KTekstF::KTekstF(): eilSk(0), dydis(16.0), uzkrautas(false)
{
	for(int i = 0; i < EIL_SK; i++)
		eil[i] = NULL;
}


KTekstF::~KTekstF()
{
}


//Tekstas skaitomas is tekstinio failo
void KTekstF::Uzkrauti(const char *pav)
{
	if(uzkrautas){
		Ispejimas("Failas '%s' uzkraunamas antra karta", pav);
		Naikinti();
	}
		
	#ifdef DEBUG
	cout<<"\n  Teksto failas: "<<pav;
	#endif
	
	char eilut[256];
    ifstream fd(pav);
    if(!fd.fail())
		while(fd.good()){
			if (eilSk >= EIL_SK){
				Ispejimas("Pasiektas teksto eiluciu limitas");
				break;
			}
        	fd.get(eilut, 255);
        	fd.ignore();
        	if(strlen(eilut) >= 255){
				Ispejimas("Eilute '%s' ilgesne nei 256", eilut);
				continue;
			}
        	if(strlen(eilut) < 1){
				#ifdef DEBUG
				cout<<" tekstas->Uzkrauti:tuscia eilute ";
				#endif
        		continue;
			}
			try{ eil[eilSk] = new char[strlen(eilut) + 1]; }
			catch(...){ Klaida("Klaida skaitant tekstini faila '%s': neisskirta atmintis teksto eilutei", pav); }
    		strcpy(eil[eilSk], eilut);
    		eilSk++;
    	}
    else
        Ispejimas("Nerastas failas '%s'", pav);
    fd.close();
    
    #ifdef DEBUG
    cout<<" eil: "<<eilSk;
    #endif
	
	uzkrautas = true;
}


//Piesiamas visas tekstas nurodytoj pozicijoj
void KTekstF::Piesti(GLfloat x, GLfloat y)
{
	if(!uzkrautas)
		return;
	
	for(int i = 0; i < eilSk; i++){
		glRasterPos2f(x, y - (i * dydis));
		tekstas.Piesti(eil[i]);
	}
}


//Naikinami duomenys
void KTekstF::Naikinti()
{
	if(!uzkrautas)
		return;
	for(int i = 0; i < eilSk; i++)
		delete[] eil[i];
	eilSk = 0;
	uzkrautas = false;
}
*/
