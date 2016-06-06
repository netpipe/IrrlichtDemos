KTekstas::KTekstas(): sukurtas(false)
{
}


KTekstas::~KTekstas()
{
}


//Sukuriamas standartinis tekstas
#ifndef GNU
bool KTekstas::Sukurti(int bold)
#else
bool KTekstas::Sukurti(Display *display)
#endif
{
	#ifndef GNU
	HFONT	font;
	HFONT	oldfont;
	#else
	char	formatas[] = "-adobe-courier-*-r-normal--12-*";
	XFontStruct	*fontStruct;
	#endif

	if(sukurtas){
		Naikinti();
		#ifdef DEBUG
		cout<<"Tekstas kuriamas dar karta";
		#endif
	}
	dydis = 52;
	
	#ifndef GNU		//Windows kodas
	font = CreateFont(
		-dydis,							//Aukstis
		0,								//Plotis
		0,								//Angle Of Escapement
		0,								//Orientation Angle
		bold,							//Font Weight
		FALSE,							//Pasvires
		FALSE,							//Pabrauktas
		FALSE,							//Perbrauktas
		BALTIC_CHARSET,					//Koduote
		OUT_TT_ONLY_PRECIS,					//Output Precision
		CLIP_DEFAULT_PRECIS,			//Clipping Precision
		ANTIALIASED_QUALITY,			//Output Quality
		FF_DONTCARE | DEFAULT_PITCH,	//Family And Pitch
		"Courier New");					    //Sriftas
	if(font == NULL)
		return false;
	oldfont = (HFONT)SelectObject(hDC, font);
	if(oldfont == NULL || oldfont == HGDI_ERROR)
		return false;
		
	if(!GautiDuom())
		return false;
	
	SelectObject(hDC, oldfont);
	if(DeleteObject(font) == 0)
		return false;
	
	#else			//GNU/Linux kodas
	fontStruct = XLoadQueryFont(display, formatas);
	if(fontStruct == NULL)
		return false;
	glXUseXFont(fontStruct->font_id, 32, 223, 32);
	#endif
		
	sukurtas = true;
	return true;
}


//Sukuriamas tekstas, nurodomas dydis ir sriftas
#ifndef GNU
bool KTekstas::Sukurti(int _dydis, const char *sriftas)
#else
bool KTekstas::Sukurti(Display *display, int _dydis, const char *sriftas)
#endif
{
	#ifndef GNU
	HFONT	font;										//Windows Font ID
	HFONT	oldfont;
	#else
	char	formatas[128];
	XFontStruct	*fontStruct;
	#endif
	
	if(sukurtas){
		Naikinti();
		#ifdef DEBUG
		cout<<"Tekstas kuriamas dar karta";
		#endif
	}
	dydis = _dydis;
	
	#ifndef GNU		//Windows kodas
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
	if(!GautiDuom())
		return false;
	SelectObject(hDC, oldfont);
	if(DeleteObject(font) == 0)
		return false;
	
	#else			//GNU/Linux kodas
	sprintf(formatas, "-%s-*-r-normal--%i-*", sriftas, dydis);
	fontStruct = XLoadQueryFont(display, formatas);
	if(fontStruct == NULL)
		return false;
	glXUseXFont(fontStruct->font_id, 32, 223, 32);
	#endif
		
	sukurtas = true;
	return true;
}


bool KTekstas::GautiDuom()
{
	//cout<<(int)(unsigned char)('à')<<" "<<(unsigned int)(unsigned char)('è')<<endl;
	//Gauti sriftu bitmap'us
	DWORD rez;
	POINT poz;
	GLYPHMETRICS met;
	MAT2 mat;
	dydis2 = 1;
	while(dydis2 < dydis + 12)
		dydis2 *= 2;
	unsigned char *dm = new unsigned char[dydis2 * dydis2];
	unsigned char *buf = new unsigned char[dydis2 * dydis2 * 4];
	//cout<<"Dydis: "<<dydis2<<endl;
	duomDydis = dydis;
	
	int x;
	int y;
	int plotis;
	
	poz.x = 0;
	poz.y = 0;
	met.gmBlackBoxX = 10;
	met.gmBlackBoxY = 10;
	met.gmptGlyphOrigin.x = 0;
	met.gmptGlyphOrigin.y = 0;
	met.gmCellIncX = 12;
	met.gmCellIncY = 0;
	
	//Sukuriama identity matrica
	mat.eM11.fract = 0;
	mat.eM11.value = 1;
	mat.eM12.fract = 0;
	mat.eM12.value = 0;
	mat.eM21.fract = 0;
	mat.eM21.value = 0;
	mat.eM22.fract = 0;
	mat.eM22.value = 1;
	
	//glGenTextures(224, &tek[32]);
	//cout<<(unsigned int)'a'<<" "<<(unsigned int)(unsigned char)'à'<<endl;
	for(int n = 0; n < 256; n++){
		memset(dm, 0, dydis2 * dydis2);
		duom[n] = new unsigned char[duomDydis * duomDydis];
		memset(duom[n], 0, duomDydis * duomDydis);
		if(n <= 32 || n == 160){// || n == 195 || n == 200 || n == 208 || n == 211 || n == 218){
			//SukurtiTekstura(tek[n], duom);
			tek[n] = tekstura.Sukurti(dm, dydis2, dydis2, GL_ALPHA, GL_ALPHA8);
			param[n].x = dydis / 2;
			param[n].y = dydis / 2;
			param[n].poz_x = 0;
			param[n].poz_y = 0;
			param[n].posl_x = dydis / 2;
			param[n].posl_y = 0;
			continue;
		}
		//Gauti duomenis
		rez = GetGlyphOutline(hDC, (unsigned char)n, GGO_BITMAP, &met, dydis * dydis * 4, buf, &mat);
		if(rez == GDI_ERROR){
			#ifdef DEBUG
			Klaida("GDI: %i", GetLastError());
			#endif
			return false;
		}
		
		//for(int i = 0; i < 100; i++)
		//	cout<<i<<":\t"<<(int)buf[i]<<endl;
		
		//Transformuoti i 8 bpp formata
		x = met.gmBlackBoxX;
		y = met.gmBlackBoxY;
		if(x != 0 && y != 0){
			//Apskaiciuojamas plotis (32 b patern)
			plotis = ((int)((x + 7) / 8) + 3) / 4;
			plotis *= 4;
			for(int i = 0; i < y; i++){
				//cout<<(int)buf[i * 4]<<":\t";
				for(int j = 0; j < x; j++){
					//duom[(dydis - i - 3) * dydis + (j + 2)] = ((buf[i * 4 + (j / 8)] >> (7 - (j % 8))) & 0x01) ? 255 : 0;
					dm[(dydis2 - 2 - i - 6) * dydis2 + (j + 6)] = ((buf[i * plotis + (j / 8)] >> (7 - (j % 8))) & 0x01) ? 255 : 0;
					if(i < duomDydis && j < duomDydis)
						duom[n][(duomDydis - i - 1) * duomDydis + j] = ((buf[i * plotis + (j / 8)] >> (7 - (j % 8))) & 0x01) ? 255 : 0;
					
					//if(n == 'D' || n == 'E' || n == 'M' || n == 'K' || n == (unsigned char)'Ë')
					//	cout<<((int)duom[n][(duomDydis - i - 1) * duomDydis + j] == 0 ? "." : "#");
				}
				//if(n == 'D' || n == 'E' || n == 'M' || n == 'K' || n == (unsigned char)'Ë')
				//	cout<<endl;
			}
			//if(n == 'D' || n == 'E' || n == 'M' || n == 'K' || n == (unsigned char)'Ë')
			//	cout<<endl;
			//Issaugoti duomenis
			param[n].x = met.gmBlackBoxX;
			param[n].y = met.gmBlackBoxY;
			param[n].poz_x = met.gmptGlyphOrigin.x;
			param[n].poz_y = met.gmptGlyphOrigin.y;
			param[n].posl_x = met.gmCellIncX;
			param[n].posl_y = met.gmCellIncY;
			//if((int)param[n].poz_y > 128)
			//	param[n].poz_y = 256 - param[n].poz_y;
			
			/*if(n == 'a' || n == 'd'){
				cout<<(char)n<<endl;
				cout<<(int)param[n].x<<endl;
				cout<<(int)param[n].y<<endl;
				cout<<(int)param[n].poz_x<<endl;
				cout<<(int)param[n].poz_y<<endl;
				cout<<(int)param[n].posl_x<<endl;
				cout<<(int)param[n].posl_y<<endl<<endl;
			}*/
		}
		
		//Sukurti tekstura
	    //if(!SukurtiTekstura(tek[n], duom))
	    //	return false;
	    tek[n] = tekstura.Sukurti(dm, dydis2, dydis2, GL_ALPHA, GL_ALPHA8);
		
		//cout<<x<<" "<<y<<endl;
	}

	delete[] dm;
	delete[] buf;
	return true;
}


//Sukuriama tekstura
/*bool KTekstas::SukurtiTekstura(GLuint tek, unsigned char *duom)
{
	GLenum klaida;
	glBindTexture(GL_TEXTURE_2D, tek);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, dydis2, dydis2, 0, GL_ALPHA8, GL_UNSIGNED_BYTE, duom);
	if((klaida = gluBuild2DMipmaps(GL_TEXTURE_2D, GL_ALPHA8, dydis2, dydis2, GL_ALPHA, GL_UNSIGNED_BYTE, duom)) != 0){
		#ifdef DEBUG
		const GLubyte *kl = gluErrorString(klaida);
		char str[128];
		strcpy(str, (const char*)kl);
		Klaida(str);
		#endif
		return false;
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	if(!glIsTexture(tek)){
		#ifdef DEBUG
		Klaida("KL");
		#endif
		return false;
	}
	return true;
}*/


//Piesiama viena teksto eilute
void KTekstas::Piesti(const char *str)
{
	if(str == NULL)
		return;
	for(int i = 0; i < strlen(str); i++){
		Simbolis(str[i]);
	}
}


//Piesiama teksto eilute. Pries piesima formatuojama
void KTekstas::PiestiNeform(const char *fmt, ...)
{
	char		text[256];
	va_list		ap;

	if (fmt == NULL)
		return;

	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

	Piesti(text);
}


//Naikinami duomenys
void KTekstas::Naikinti()
{
	//glDeleteTextures(255, tek);
    sukurtas = false;
}


//Simbolio piesimas
void KTekstas::Simbolis(char ch)
{
	uch = (unsigned char)ch;
	glBindTexture(GL_TEXTURE_2D, tek[uch]);
	glTranslatef(param[uch].poz_x, -dydis + param[uch].poz_y, 0.0);// param[str[i]].poz_y - param[str[i]].y, 0.0);
	geom.Plokst(dydis);
	glTranslatef(param[uch].x, dydis - param[uch].poz_y, 0.0);// -param[str[i]].poz_y + param[str[i]].y, 0.0);
	//glTranslatef(param[str[i]].x, -param[str[i]].poz_y + param[str[i]].y, 0.0);
}
