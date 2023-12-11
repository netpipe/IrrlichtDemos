#include "perf_testas.hpp"


KPerfTestas::KPerfTestas()
{
}


void KPerfTestas::Pradeti()
{
	QueryPerformanceFrequency(&daznis);
	//Skaiciuojama laiko gavimo trukme
	QueryPerformanceCounter(&prad);
	QueryPerformanceCounter(&pab);
	lQuery = (float)(pab.QuadPart - prad.QuadPart) / daznis.QuadPart;
	//Gaunamas testo pradzios laikas
	QueryPerformanceCounter(&prad);
}


void KPerfTestas::Baigti()
{
	//Gaunamas testo pabaigos laikas
	QueryPerformanceCounter(&pab);
}


void KPerfTestas::Issaugoti(const char *pav, const char *fPav, int kartojimu)
{
	if(kartojimu < 1)
		kartojimu = 1;
	lTest = (float)(pab.QuadPart - prad.QuadPart) / daznis.QuadPart;
	lIter = lTest / kartojimu;
	ofstream fr(fPav);
	fr<<setprecision(9)<<fixed;
	fr<<"<HTML><HEAD><TITLE>"<<pav<<"</TITLE></HEAD>";
	fr<<"\n<BODY><CENTER><TABLE style='BORDER: #0000A0 1px solid;'>";
	fr<<"\n<TR><TD>Testas: "<<pav<<"<BR />";
	fr<<"\n<TR><TD>Kartojimu: "<<kartojimu<<"<BR />";
	fr<<"\n<TR><TD><BR />";
	fr<<"\n<TR><TD><B>Veiksmas</B><TD><B>Trukme</B><BR />";
	fr<<"\n<TR><TD>Laiko gavimas<TD>"<<lQuery<<"<BR />";
	fr<<"\n<TR><TD>Testas<TD>"<<lTest<<"<BR />";
	fr<<"\n<TR><TD>1 iteracija<TD>"<<lIter<<"<BR />";
	fr<<"\n</TABLE>";
	fr<<"\n</CENTER></BODY></HTML>";
	fr.close();
}
