#include "tdo.hpp"


TDOFailas::TDOFailas(): pagSk(0), collSk(0), pag(NULL), coll(NULL), tekst(0),
  dispList(0), uzkrauta(false)
{
}


TDOFailas::~TDOFailas()
{
}


void TDOFailas::Uzkrauti(const char *pav) throw(const char*)
{
	int ltSk[3],	//Tasku skaicius
		ltrSk[3],	//Trikampiu skaicius
		*ltr[3];	//L1, L2, L3 trikampiai
	float *lt[3];	//L1, L2, L3 taskai
	
	unsigned char ch, head[4];
	int i;
	FILE *f;
	
	f = fopen(pav, "rb");
	
	//Headerio skaitymas
	if(f == NULL)
		throw("neatidarytas failas");
	fread(&head, 4, 1, f);
	if(head[0] != 0xAE)
		throw("netinkamas failas");
	ID = head[1];		//ID
	collSk = head[2];	//Collision tasku skaicius
	pagSk = head[3];	//Pagalbiniu tasku skaicius
	
	fread(&spind, 4, 1, f);		//Apskritimas
	for(i = 0; i < 3; i++){
		fread(&ltSk[i], 4, 1, f);	//LOD[i] tasku skaicius
		fread(&ltrSk[i], 4, 1, f);	//LOD[i] trikampiu skaicius
	}
	fread(teks, 4, 8, f);		//Teksturos pavadinimas
	fread(bounding, 4, 4, f);	//Bounding Box reiksmes
	
	//Atminties isskyrimas
	if(collSk > 0){
		try{ coll = new float[collSk * 2]; }
		catch(...){ throw("Neisskirta atmintis"); }
	}
	if(pagSk > 0){
		try{ pag = new float[pagSk * 6]; }
		catch(...){ throw("Neisskirta atmintis"); }
	}
	for(i = 0; i < 3; i++){
		try{ lt[i] = new float[ltSk[i] * 8]; }
		catch(...){ throw("Neisskirta atmintis"); }
		try{ ltr[i] = new int[ltrSk[i] * 3]; }
		catch(...){ throw("Neisskirta atmintis"); }
	}
	
	//Duomenu skaitymas
	if(collSk > 0) fread(coll, 4, collSk * 2, f);
	if(pagSk > 0) fread(pag, 4, pagSk * 6, f);
	for(i = 0; i < 3; i++){
		fread(lt[i], 4, ltSk[i] * 8, f);
		fread(ltr[i], 4, ltrSk[i] * 3, f);
	}
		
	fclose(f);
	
	
	//Sukurti Display list'us LOD'ams ------------------------------------------
	dispList = glGenLists(TDO_DISP_LIST);
	glEnable(GL_TEXTURE_2D);
	if(dispList == 0)
		throw("Nesukurti Display Listai");
	else{
		glColor4f(1.0, 1.0, 1.0, 1.0);
		int list;
		for(int L = 0; L < TDO_DISP_LIST; L++){
			list = L;
			/*if(list == 3){
				list = 0;	//Seseliui naudoti nulini LOD'a
				glDisable(GL_TEXTURE_2D);
				glColor4fv(seseliaiSp);
			}*/
			glNewList(dispList + L, GL_COMPILE);
			
			#ifdef DEBUG
				GLfloat kampas;
				/*glDisable(GL_TEXTURE_2D);
				glColor3f(1.0, 0.0, 0.0);
				glBegin(GL_POINTS);
					glVertex3f(0.0, 0.0, 0.1);
				glEnd();*/
				
				//Piesiamas apskritimas aplink objekta
				/*glBegin(GL_LINE_LOOP);
					//glColor4f(1.0, 1.0, 1.0, 1.0);
					for(i = 0; i < 32; i++){
						kampas = 2 * M_PI * i / 32;
						glVertex3f(cos(kampas) * spind, sin(kampas) * spind, 0.1);
					}
				glEnd();*/
				
				/*//Piesiamos Collision atkarpos
				glDisable(GL_TEXTURE_2D);
				glBegin(GL_LINE_LOOP);
					glColor3f(1.0, 1.0, 0.0);
					for(i = 0; i < collSk; i++)
						glVertex3f(coll[i * 2], coll[i * 2 + 1], 1.0);
				glEnd();
				glEnable(GL_TEXTURE_2D);*/
			#endif
			
			glBegin(GL_TRIANGLES);
			for(i = 0; i < ltrSk[list] * 3; i++){
				glTexCoord2f(lt[list][ltr[list][i] * 8 + 6], lt[list][ltr[list][i] * 8 + 7]);
				glNormal3f(lt[list][ltr[list][i] * 8 + 3], lt[list][ltr[list][i] * 8 + 4], lt[list][ltr[list][i] * 8 + 5]);
				glVertex3f(lt[list][ltr[list][i] * 8], lt[list][ltr[list][i] * 8 + 1], lt[list][ltr[list][i] * 8 + 2]);
			}
			glEnd();
			glEndList();
		}
	}
	//--------------------------------------------------------------------------

	for(int i = 0; i < TDO_DISP_LIST; i++){
		delete[] lt[i];
		delete[] ltr[i];
	}
	
	uzkrauta = true;
}


void TDOFailas::Naikinti()
{
	if(!uzkrauta){
		#ifdef DEBUG
		Logas::Log("Naikinamas neegzistuojantis objektas");
		#endif
		return;
	}
	if(coll)
		delete[] coll;
	
	if(pag)
		delete[] pag;
	glDeleteLists(dispList, TDO_DISP_LIST);
	uzkrauta = false;
}
