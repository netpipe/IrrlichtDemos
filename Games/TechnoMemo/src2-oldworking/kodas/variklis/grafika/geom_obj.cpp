#include "geom_obj.hpp"


GLuint Geom::dl[DYDIS] = {};


Geom::Geom()
{
}


Geom::~Geom()
{
}


void Geom::Sukurti()
{
	//GenDL();
	dl[PLOKST]		= PlokstDL();
	dl[PLOKST_WIRE]	= PlokstWireDL();
	dl[APSKRITIMAS]	= ApskritimasDL();
	dl[SKRITULYS]	= SkritulysDL();
	dl[ZIEDAS_2D]	= Ziedas2DDL();
	dl[KOORD]		= KoordDL();
	dl[KUBAS]		= KubasDL();
	dl[KUBAS_SP]	= KubasSpDL();
	dl[KUBAS_WIRE]	= KubasWireDL();
	dl[ZIEDAS]		= ZiedasDL();
	dl[ZIEDAS_WIRE]	= ZiedasWireDL();
}


void Geom::Naikinti()
{
	for(int i = 0; i < DYDIS; i++)
		glDeleteLists(dl[i], 1);
}


//-- 2D ------------------------------------------------------------------------

void Geom::Plokst(GLfloat plotis = 1.0)
{
	glPushMatrix();
	glScalef(plotis, plotis, 1.0);
	glCallList(dl[PLOKST]);
	//glScalef(1.0 / plotis, 1.0 / plotis, 1.0);
	glPopMatrix();
}


void Geom::Plokst(GLfloat plotis, GLfloat aukstis)
{
	glPushMatrix();
	glScalef(plotis, aukstis, 1.0);
	glCallList(dl[PLOKST]);
	//glScalef(1.0 / plotis, 1.0 / aukstis, 1.0);
	glPopMatrix();
}


GLuint Geom::PlokstDL()
{
	GLuint _dl = glGenLists(1);
	GLfloat plotis = 1.0;
	glNewList(_dl, GL_COMPILE);
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0);	glVertex3f(0.0, 0.0, 0.0);
		glTexCoord2f(1.0, 0.0);	glVertex3f(plotis, 0.0, 0.0);
		glTexCoord2f(1.0, 1.0);	glVertex3f(plotis, plotis, 0.0);
		glTexCoord2f(0.0, 1.0);	glVertex3f(0.0, plotis, 0.0);
	glEnd();
	glEndList();
	return _dl;
}


void Geom::PlokstWire(GLfloat plotis = 1.0)
{
	glPushMatrix();
	glScalef(plotis, plotis, 1.0);
	glCallList(dl[PLOKST_WIRE]);
	//glScalef(1.0 / plotis, 1.0 / plotis, 1.0);
	glPopMatrix();
}


void Geom::PlokstWire(GLfloat plotis, GLfloat aukstis)
{
	glPushMatrix();
	glScalef(plotis, aukstis, 1.0);
	glCallList(dl[PLOKST_WIRE]);
	//glScalef(1.0 / plotis, 1.0 / aukstis, 1.0);
	glPopMatrix();
}


GLuint Geom::PlokstWireDL()
{
	GLuint _dl = glGenLists(1);
	float plotis = 1.0;
	glNewList(_dl, GL_COMPILE);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(plotis, 0.0, 0.0);
		glVertex3f(plotis, plotis, 0.0);
		glVertex3f(0.0, plotis, 0.0);
	glEnd();
	glEndList();
	return _dl;
}


void Geom::Apskritimas(GLfloat dydis)
{
	glPushMatrix();
	glScalef(dydis, dydis, 0.0);
	glCallList(dl[APSKRITIMAS]);
	glPopMatrix();
}


GLuint Geom::ApskritimasDL(GLfloat dydis, int kokybe)
{
	GLuint _dl = glGenLists(1);
	float kamp;
	glNewList(_dl, GL_COMPILE);
	glBegin(GL_LINE_LOOP);
	for(int i = 0; i < kokybe; i++){
		kamp = (float)i / (float)kokybe * M_PI * 2.0;
		glVertex3f(dydis * cos(kamp), dydis * sin(kamp), 0.0);
	}
	glEnd();
	glEndList();
	return _dl;
}


void Geom::Skritulys(GLfloat dydis)
{
	glPushMatrix();
	glScalef(dydis, dydis, 0.0);
	glCallList(dl[SKRITULYS]);
	glPopMatrix();
}


GLuint Geom::SkritulysDL(GLfloat dydis, int kokybe)
{
	GLuint _dl = glGenLists(1);
	float kamp;
	glNewList(_dl, GL_COMPILE);
	glBegin(GL_TRIANGLE_FAN);
	glNormal3f(0.0, 0.0, 1.0);
	glVertex3f(0.0, 0.0, 0.0);
	for(int i = 0; i <= kokybe; i++){
		kamp = (float)i / (float)kokybe * M_PI * 2.0;
		glVertex3f(dydis * cos(kamp), dydis * sin(kamp), 0.0);
	}
	glEnd();
	glEndList();
	return _dl;
}


void Geom::Ziedas2D(GLfloat dydis)
{
	glPushMatrix();
	glScalef(dydis, dydis, 0.0);
	glCallList(dl[ZIEDAS_2D]);
	glPopMatrix();
}


GLuint Geom::Ziedas2DDL(GLfloat dydis1, GLfloat dydis2, int kokybe)
{
	GLuint _dl = glGenLists(1);
	float kamp;
	glNewList(_dl, GL_COMPILE);
	glBegin(GL_TRIANGLE_STRIP);
	glNormal3f(0.0, 0.0, 1.0);
	for(int i = 0; i <= kokybe; i++){
		kamp = (float)i / (float)kokybe * M_PI * 2.0;
		glVertex3f(dydis1 * cos(kamp), dydis1 * sin(kamp), 0.0);
		glVertex3f(dydis2 * cos(kamp), dydis2 * sin(kamp), 0.0);
	}
	glEnd();
	glEndList();
	return _dl;
}


//-- 3D ------------------------------------------------------------------------


void Geom::Koord()
{
	glCallList(dl[KOORD]);
}


GLuint Geom::KoordDL(GLfloat dydis, int tarpu)
{
	GLuint _dl = glGenLists(1);
	GLfloat tarpas = dydis / (GLfloat)tarpu;

	glNewList(_dl, GL_COMPILE);
	glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-dydis, 0.0, 0.0);
		glVertex3f(dydis, 0.0, 0.0);

		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, -dydis, 0.0);
		glVertex3f(0.0, dydis, 0.0);

		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, -dydis);
		glVertex3f(0.0, 0.0, dydis);

		for(GLfloat n = -dydis; n < dydis; n += tarpas){
			glColor3f(1.0, 0.0, 0.0);

			glVertex3f(n, -tarpas / 5.0, 0.0);
			glVertex3f(n, tarpas / 5.0, 0.0);

			glVertex3f(n, 0.0, -tarpas / 5.0);
			glVertex3f(n, 0.0, tarpas / 5.0);


			glColor3f(0.0, 1.0, 0.0);

			glVertex3f(-tarpas / 5.0, n, 0.0);
			glVertex3f(tarpas / 5.0, n, 0.0);

			glVertex3f(0.0, n, -tarpas / 5.0);
			glVertex3f(0.0, n, tarpas / 5.0);


			glColor3f(0.0, 0.0, 1.0);

			glVertex3f(-tarpas / 5.0, 0.0, n);
			glVertex3f(tarpas / 5.0, 0.0, n);

			glVertex3f(0.0, -tarpas / 5.0, n);
			glVertex3f(0.0, tarpas / 5.0, n);
		}
	glEnd();
	glEndList();
	glColor4f(1.0, 1.0, 1.0, 1.0);
	
	return _dl;
}


void Geom::Kubas(GLfloat x = 1.0)
{
	glPushMatrix();
	glScalef(x, x, x);
	glCallList(dl[KUBAS]);
	//glScalef(1.0 / x, 1.0 / x, 1.0 / x);
	glPopMatrix();
}


void Geom::Kubas(GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();
	glScalef(x, y, z);
	glCallList(dl[KUBAS]);
	//glScalef(1.0 / x, 1.0 / y, 1.0 / z);
	glPopMatrix();
}


GLuint Geom::KubasDL()
{
	GLuint _dl = glGenLists(1);
	GLfloat plotis = 1.0;
	glNewList(_dl, GL_COMPILE);
	//X neigiama plokstuma
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glTranslatef(-plotis / 2.0, -plotis / 2.0, plotis / 2.0);
	Plokst(plotis);
	//X teigiama
	glRotatef(180.0, 0.0, 1.0, 1.0);
	glTranslatef(-plotis, 0.0, plotis);
	Plokst(plotis);
	//Y-
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -plotis, 0.0);
	Plokst(plotis);
	//Y+
	glRotatef(180.0, 0.0, 1.0, 0.0);
	glTranslatef(-plotis, 0.0, plotis);
	Plokst(plotis);
	//Z-
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glTranslatef(-plotis, 0.0, 0.0);
	Plokst(plotis);
	//Z+
	glRotatef(180.0, 0.0, 1.0, 0.0);
	glTranslatef(-plotis, 0.0, plotis);
	Plokst(plotis);
	//Atstatoma pradine transformacija
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glTranslatef(plotis / 2.0, -plotis / 2.0, -plotis / 2.0);
	glEndList();
	return _dl;
}


void Geom::KubasSp(GLfloat x = 1.0)
{
	glPushMatrix();
	glScalef(x, x, x);
	glCallList(dl[KUBAS_SP]);
	//glScalef(1.0 / x, 1.0 / x, 1.0 / x);
	glPopMatrix();
}


void Geom::KubasSp(GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();
	glScalef(x, y, z);
	glCallList(dl[KUBAS_SP]);
	//glScalef(1.0 / x, 1.0 / y, 1.0 / z);
	glPopMatrix();
}


GLuint Geom::KubasSpDL()
{
	GLuint _dl = glGenLists(1);
	GLfloat plotis = 1.0;
	glNewList(_dl, GL_COMPILE);
	//X neigiama plokstuma
	glColor3f(1.0, 0.0, 0.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glTranslatef(-plotis / 2.0, -plotis / 2.0, plotis / 2.0);
	Plokst(plotis);
	//X teigiama
	glColor3f(1.0, 0.0, 0.0);
	glRotatef(180.0, 0.0, 1.0, 0.0);
	glTranslatef(-plotis, 0.0, plotis);
	Plokst(plotis);
	//Y-
	glColor3f(0.0, 1.0, 0.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -plotis, 0.0);
	Plokst(plotis);
	//Y+
	glRotatef(180.0, 0.0, 1.0, 0.0);
	glTranslatef(-plotis, 0.0, plotis);
	Plokst(plotis);
	//Z-
	glColor3f(0.0, 0.0, 1.0);
	glRotatef(-90.0, 0.0, 1.0, 0.0);
	glTranslatef(-plotis, 0.0, 0.0);
	Plokst(plotis);
	//Z+
	glRotatef(180.0, 0.0, 1.0, 0.0);
	glTranslatef(-plotis, 0.0, plotis);
	Plokst(plotis);
	//Atstatoma pradine transformacija
	glRotatef(90.0, 0.0, 0.0, 1.0);
	glTranslatef(plotis / 2.0, -plotis / 2.0, -plotis / 2.0);
	glEndList();
	
	return _dl;
}


void Geom::KubasWire(GLfloat x = 1.0)
{
	glPushMatrix();
	glScalef(x, x, x);
	glCallList(dl[KUBAS_WIRE]);
	//glScalef(1.0 / x, 1.0 / x, 1.0 / x);
	glPopMatrix();
}


void Geom::KubasWire(GLfloat x, GLfloat y, GLfloat z)
{
	glPushMatrix();
	glScalef(x, y, z);
	glCallList(dl[KUBAS_WIRE]);
	//glScalef(1.0 / x, 1.0 / y, 1.0 / z);
	glPopMatrix();
}


GLuint Geom::KubasWireDL()
{
	GLuint _dl = glGenLists(1);
	GLfloat plotis = 0.5;
	glNewList(_dl, GL_COMPILE);
	//Pagrindinis karkasas
	glBegin(GL_LINE_LOOP);
		glVertex3f(-plotis, -plotis, -plotis);
		glVertex3f(plotis, -plotis, -plotis);
		glVertex3f(plotis, plotis, -plotis);
		glVertex3f(-plotis, plotis, -plotis);
		glVertex3f(-plotis, plotis, plotis);
		glVertex3f(plotis, plotis, plotis);
		glVertex3f(plotis, -plotis, plotis);
		glVertex3f(-plotis, -plotis, plotis);
	glEnd();
	//Trukstamos briaunos
	glBegin(GL_LINES);
		glVertex3f(-plotis, -plotis, -plotis);
		glVertex3f(-plotis, plotis, -plotis);
		glVertex3f(-plotis, -plotis, plotis);
		glVertex3f(-plotis, plotis, plotis);
		glVertex3f(plotis, -plotis, -plotis);
		glVertex3f(plotis, -plotis, plotis);
		glVertex3f(plotis, plotis, -plotis);
		glVertex3f(plotis, plotis, plotis);
	glEnd();
	glEndList();
	
	return _dl;
}


void Geom::Ziedas()
{
	glCallList(dl[ZIEDAS]);
}


GLuint Geom::ZiedasDL(int kok, int kok2, float dyd, float dyd2)
{
	GLuint _dl = glGenLists(1);
	//Issaugoti ziedo tasku koordinates
	float *duom = new float[kok2 * 2 + 2];
	float f, f2;
	for(int i = 0; i <= kok2; i++){
		f = M_PI * 2.0 / kok2 * i;
		duom[i * 2] = sin(f) * dyd2;
		duom[i * 2 + 1] = cos(f) * dyd2;
	}
	
	//Sukurti solid display lista
	glNewList(_dl, GL_COMPILE);
	for(int i = 0; i < kok; i++){
		f = M_PI * 2.0 / kok * i;
		f2 = M_PI * 2.0 / kok * (i + 1);
		glBegin(GL_QUAD_STRIP);
		for(int j = 0; j <= kok2; j++){
			glNormal3f(sin(f) * duom[j * 2], cos(f) * duom[j * 2], duom[j * 2 + 1]);
			glTexCoord2f(3.0 * i / kok, float(j) / kok2);
			glVertex3f(sin(f) * duom[j * 2] + sin(f) * dyd, cos(f) * duom[j * 2] + cos(f) * dyd, duom[j * 2 + 1]);
			
			glNormal3f(sin(f2) * duom[j * 2], cos(f2) * duom[j * 2], duom[j * 2 + 1]);
			glTexCoord2f(3.0 * (i + 1) / kok, float(j) / kok2);
			glVertex3f(sin(f2) * duom[j * 2] + sin(f2) * dyd, cos(f2) * duom[j * 2] + cos(f2) * dyd, duom[j * 2 + 1]);
		}
		glEnd();
	}
	glEndList();
	
	delete[] duom;
	
	return _dl;
}


void Geom::ZiedasWire()
{
	glCallList(dl[ZIEDAS_WIRE]);
}


GLuint Geom::ZiedasWireDL(int kok, int kok2, float dyd, float dyd2)
{
	GLuint _dl = glGenLists(1);
	//Issaugoti ziedo tasku koordinates
	float *duom = new float[kok2 * 2 + 2];
	float f;
	for(int i = 0; i <= kok2; i++){
		f = M_PI * 2.0 / kok2 * i;
		duom[i * 2] = sin(f) * dyd2;
		duom[i * 2 + 1] = cos(f) * dyd2;
	}
	
	//Sukurti wire display lista
	glNewList(_dl, GL_COMPILE);
	//Piesiami mazieji ziedai
	for(int i = 0; i < kok; i++){
		f = M_PI * 2.0 / kok * i;
		glBegin(GL_LINE_LOOP);
		for(int j = 0; j < kok2; j++){
			glVertex3f(sin(f) * duom[j * 2] + sin(f) * dyd, cos(f) * duom[j * 2] + cos(f) * dyd, duom[j * 2 + 1]);
		}
		glEnd();
	}
	//Piesiami didieji ziedai
	for(int i = 0; i < kok2; i++){
		glBegin(GL_LINE_LOOP);
		for(int j = 0; j < kok; j++){
			f = M_PI * 2.0 / kok * j;
			glVertex3f(sin(f) * duom[i * 2] + sin(f) * dyd, cos(f) * duom[i * 2] + cos(f) * dyd, duom[i * 2 + 1]);
		}
		glEnd();
	}
	glEndList();
	
	delete[] duom;
	
	return _dl;
}
