#include "utils.hpp"
#include <string.h>
#include <stdio.h>
time_t KUtils::laikas = 0;
time_t KUtils::laikasPask = 0;
int KUtils::sum = 0;


//------------------------------------------------------------------------------
KUtils::KUtils()
{
}


//------------------------------------------------------------------------------
KUtils::~KUtils()
{
}


//------------------------------------------------------------------------------
//Win komandines eilutes parametrai keiciami i argv tipo eilutes. NULL reiskia,
//  kad daugiau eiluciu nera. Jei pati pirma eilute NULL - reiskia nebuvo parametru.
char ** KUtils::CmdToArgv(char *cmd)
{
	char **argv;

	int kiekis = 0;		//Parametru kiekis
	char *kop = cmd;	//Atsargine adreso kopija
	int ilg;			//Vieno parametro ilgis

	//Skaiciuojamas parametru kiekis
	while(kop[0] != '\0'){
		if(kop[0] == ' ')
			kop++;
		if(kop[0] == '\"'){
			kop++;
			if(strstr(kop, "\"") + 1 < kop + strlen(kop))
				ilg = strstr(kop, "\"") - kop;
			else
				ilg = strlen(kop) - 1;
			kop += ilg + 1;
		}
		else{
			if(strstr(kop, " "))
				ilg = strstr(kop, " ") - kop;
			else
				ilg = strlen(kop);
			kop += ilg;
		}
		++kiekis;
	}
	if(kiekis == 0){
		argv = new char*[1];
		argv[0] = NULL;
		return argv;
	}

	argv = new char*[kiekis + 1];
	argv[kiekis] = NULL;	//NULL reiskia, kad tai paskutine eilute
	kiekis = 0;
	//Parametrai isskiriami is eilutes ir surasomi i masyva
	while(cmd[0] != '\0'){
		if(cmd[0] == ' ')
			cmd++;
		if(cmd[0] == '\"'){
			cmd++;
			if(strstr(cmd, "\"") + 1 < cmd + strlen(cmd))
				ilg = strstr(cmd, "\"") - cmd;
			else
				ilg = strlen(cmd) - 1;
			argv[kiekis] = new char[ilg + 1];
			strncpy(argv[kiekis], cmd, ilg);
			cmd += ilg + 1;
		}
		else{
			if(strstr(cmd, " "))
				ilg = strstr(cmd, " ") - cmd;
			else
				ilg = strlen(cmd);
			argv[kiekis] = new char[ilg + 1];
			strncpy(argv[kiekis], cmd, ilg);
			cmd += ilg;
		}
		argv[kiekis][ilg] = '\0';
		++kiekis;
	}

	return argv;
}


//------------------------------------------------------------------------------
//Gaunama zaidimo direktorija. Adresas uzbaigiamas simboliu '/' arba '\'
char * KUtils::GautiDir()
{
    #define MAX_PATH 40
	int poz = 0;
	char *eil = new char[MAX_PATH];

	#ifdef OS_WIN32		//Windows kodas
	GetModuleFileName(NULL, eil, MAX_PATH);
	for(int i = 0; i < strlen(eil); i++)
		if(eil[i] == '\\' || eil[i] == '/')
			poz = i;
	if(poz > 0)
		eil[poz + 1] = '\0';
	#endif
	#ifdef OS_UNIX		//GNU/Linux kodas
	getcwd(eil, MAX_PATH);
	strcat(eil, "/\0");
	#endif

	return eil;
}


//------------------------------------------------------------------------------
//Gaunama zaidimo direktorija, rezultatas irasomas i 'eil'
void KUtils::GautiDir(char *eil)
{
	int poz = 0;
	#ifdef OS_WIN32
	GetModuleFileName(NULL, eil, MAX_PATH);
	for(int i = 0; i < strlen(eil); i++)
		if(eil[i] == '\\' || eil[i] == '/')
			poz = i;
	if(poz > 0)
		eil[poz + 1] = '\0';
	#endif
	#ifdef OS_UNIX
	getcwd(eil, MAX_PATH);
	strcat(eil, "/\0");
	#endif
}


//------------------------------------------------------------------------------
//If pilno failo adreso gaunamas failo vardas.
//Grazinama rodykle i vardo pradzia adrese.
char * KUtils::GautiPav(char *eil)
{
	int poz = 0;
	for(int i = 0; i < strlen(eil); i++)
		if(eil[i] == '\\' || eil[i] == '/')
			poz = i;
	if(poz == 0)
		return eil;
	else
		return &eil[poz + 1];
}


//------------------------------------------------------------------------------
//If pilno failo adreso (arba tik vardo) gaunamas failo pletinys.
//Grazinama rodykle i pletinio pradzia adrese.
char * KUtils::GautiPlet(char *eil)
{
	int poz = 0;
	for(int i = 0; i < strlen(eil); i++)
		if(eil[i] == '.')
			poz = i;
	if(poz == 0)
		return NULL;
	else
		return &eil[poz + 1];
}


//------------------------------------------------------------------------------
//Gaunama data (metai-menuo-diena), irasoma i "rez". Menesio ir dienos numeriai
//rasomi dvizenkliai - jei reikia, pridedami nuliai.
void KUtils::GautiData(char *rez)
{
	tm *laik;
	laikasPask = laikas;
	time(&laikas);
	laik = localtime(&laikas);
	sprintf(rez, "%i-%2i-%2i", laik->tm_year + 1900, laik->tm_mon + 1, laik->tm_mday);
	if(laik->tm_mon < 9)
		rez[5] = '0';
	if(laik->tm_mday < 10)
		rez[8] = '0';
}


//------------------------------------------------------------------------------
//Gaunamas pilnas laikas (metai-menuo-diena valanda:minute:sekunde),
//irasoma i "rez"
void KUtils::GautiLaika(char *rez)
{
	tm *laik;
	laikasPask = laikas;
	time(&laikas);
	laik = localtime(&laikas);
	sprintf(rez, "%i-%2i-%2i %2i-%2i-%2i", laik->tm_year + 1900, laik->tm_mon + 1, laik->tm_mday,
			laik->tm_hour, laik->tm_min, laik->tm_sec);
	if(laik->tm_mon < 9)
		rez[5] = '0';
	if(laik->tm_mday < 10)
		rez[8] = '0';
	if(laik->tm_hour < 10)
		rez[11] = '0';
	if(laik->tm_min < 10)
		rez[14] = '0';
	if(laik->tm_sec < 10)
		rez[17] = '0';
}


//------------------------------------------------------------------------------
//Padaromas screenshot'as. Issaugomas pavadinimu "data_laikas.bmp" i programos
//saknini kataloga.
void KUtils::ScreenShot(int x, int y)
{
	//_EDIT: 2010-01-30 LTGameJam 2010: nereikia screenshot'u
	/*char pav[256];
	GautiDir(pav);
	GautiLaika(pav + strlen(pav));
	if(laikas == laikasPask)
		sprintf(pav + strlen(pav), " %i", ++sum);
	else
		sum = 0;
	strcat(pav, ".bmp");

	KBmp bmp;
	unsigned char *duom = new unsigned char[x * y * 3];
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, x, y, GL_BGR, GL_UNSIGNED_BYTE, duom);
	bmp.Issaugoti(x, y, 24, duom, pav);
	delete[] duom;

	#ifdef DEBUG
	Logas::Log("ScreenShot: %s", pav);
	#endif*/
}
