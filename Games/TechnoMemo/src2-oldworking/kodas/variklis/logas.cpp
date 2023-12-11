#include "logas.hpp"


FILE *Logas::failas = NULL;

Logas::Logas()
{
}


Logas::~Logas()
{
	/*if(failas != NULL){
		fclose(failas);
		failas = NULL;
	}*/
}


void Logas::LogFailas(char *pav)
{
	/*if(failas != NULL)
		fclose(failas);
	failas = fopen(pav, "a");*/
}


void Logas::Log(const char* ei, ...)
{
	/*char eil[256];
    va_list	ap;

	if (ei == NULL)
	   return;
	va_start(ap, ei);
    vsprintf(eil, ei, ap);
	va_end(ap);
	strcat(eil, "\n");

	if(failas == NULL)
		printf(eil);
	else
		fprintf(failas, "%s", eil);
*/
}


void Logas::Klaida(char* ei, ...)
{
	//_TODO: Parse'nimo iskelimas i Utils klase
/*	char eil[256];
    va_list	ap;

	if (ei == NULL)
	   return;
	va_start(ap, ei);
    vsprintf(eil, ei, ap);
	va_end(ap);

	#ifdef OS_WIN32	//Windows
    MessageBox(NULL, eil, "Klaida", MB_OK | MB_ICONERROR);
    #endif
    #ifdef OS_UNIX	//GNU/Linux
    //_TODO: "normalus" Unix kalidu pranesimai
    cout<<"GNU klaida: "<<eil<<endl;
    #endif*/
}

void Logas::Ispejimas(char* ei, ...)
{
	/*char eil[256];
    va_list	ap;

	if (ei == NULL)
	   return;
	va_start(ap, ei);
    vsprintf(eil, ei, ap);
	va_end(ap);

	#ifdef OS_WIN32	//Windows
    MessageBox(NULL, eil, "Ispejimas", MB_OK | MB_ICONWARNING);
    #endif
    #ifdef OS_UNIX	//GNU/Linux
    //_TODO
    cout<<"GNU ispejimas: "<<eil<<endl;
    #endif*/
}
