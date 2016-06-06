#include "nustatymai.hpp"


KNustatymai::KNustatymai(): pav(NULL), pozX(0), pozY(0), dydisX(640), dydisY(480),
  fps(60), bpp(32), zBpp(24), accumBpp(0), stencilBpp(0),
  fullScr(false), ribotiFps(true)
{
	strcpy(pavad, "Langas");
	strcpy(wndClass, "Klase");
	strcpy(kalba, "LT");
	gpuStr = NULL;
}


KNustatymai::~KNustatymai()
{
}


void KNustatymai::GautiPav(char *_pav)
{
	if(_pav != NULL){
		//Naudoti per parametrus gauta pavadinima
		if(pav != NULL){
			delete[] pav;
		}
		pav = new char[strlen(_pav) + 1];
		strcpy(pav, _pav);
	}
	else if(pav == NULL){
		//Naudoti standartini nustatymu faila
		pav = new char[15];
		strcpy(pav, "nustatymai.cfg");
		//Gaunama zaidimo direktorija
		/*utils.GautiDir(eil);
		langas.dir = new char[strlen(eil) + 1];
		strcpy(langas.dir, eil);
		strcat(eil, "nustatymai.cfg");*/
	}
}


void KNustatymai::Uzkrauti(char *_pav)
{	
	int sk;
	char eil[256];
	GautiPav(_pav);
	ifstream fd(pav);
	#ifdef DEBUG
	if(!fd.good())
		Logas::Log("Nerastas nustatymu failas %s, naudojami standartiniai", pav);
	#endif
	while(fd.good()){
		//Praleidziami komentarai ir tuscios eilutes
		while(fd.peek() == '/' || fd.peek() == '\n')
			fd.getline(eil, 256);
		fd.getline(eil, 256, ' ');
		while(fd.peek() == ' ' || fd.peek() == '=' || fd.peek() == '\t')
			fd.ignore();
		
		//Tikrinama, kuria reiksme nuskaityti
		if(strcmp(eil, "pavad") == 0){
			fd.getline(pavad, 32);
		}
		if(strcmp(eil, "kalba") == 0){
			fd.getline(kalba, 3);
		}
		else if(strcmp(eil, "gpu") == 0){
			fd.getline(eil, 256);
			if(gpuStr != NULL)
				delete[] gpuStr;
			gpuStr = new char[strlen(eil) + 1];
			strcpy(gpuStr, eil);
		}
		else if(strcmp(eil, "class") == 0){
			fd.getline(wndClass, 32);
		}
		else if(strcmp(eil, "poz") == 0){
			fd>>pozX;
			while(fd.peek() == ' ' || fd.peek() == ',' || fd.peek() == '\t')
				fd.ignore();
			fd>>pozY;
			fd.getline(eil, 256);
		}
		else if(strcmp(eil, "dydis") == 0){
			fd>>dydisX;
			while(fd.peek() == ' ' || fd.peek() == ',' || fd.peek() == '\t')
				fd.ignore();
			fd>>dydisY;
			fd.getline(eil, 256);
		}
		else if(strcmp(eil, "fps") == 0){
			fd>>fps;
			fd.getline(eil, 256);
		}
		else if(strcmp(eil, "bpp") == 0){
			fd>>bpp;
			fd.getline(eil, 256);
		}
		else if(strcmp(eil, "z_bpp") == 0){
			fd>>zBpp;
			fd.getline(eil, 256);
		}
		else if(strcmp(eil, "accum_bpp") == 0){
			fd>>accumBpp;
			fd.getline(eil, 256);
		}
		else if(strcmp(eil, "stencil_bpp") == 0){
			fd>>stencilBpp;
			fd.getline(eil, 256);
		}
		else if(strcmp(eil, "fullscr") == 0){
			fd>>fullScr;
			fd.getline(eil, 256);
		}
		else if(strcmp(eil, "v-sync") == 0){
			fd>>ribotiFps;
			fd.getline(eil, 256);
		}
		else fd.getline(eil, 256);
	}
	fd.close();
}


void KNustatymai::Issaugoti(char *_pav)
{
	GautiPav(_pav);
	ofstream fr(pav);
	
	fr<<"pavad"<<" = "<<pavad<<endl;
	fr<<"kalba"<<" = "<<kalba<<endl;
	if(gpuStr != NULL)
		fr<<"gpu"<<" = "<<gpuStr<<endl;
	fr<<"class"<<" = "<<wndClass<<endl;
	fr<<"poz"<<" = "<<pozX<<", "<<pozY<<endl;
	fr<<"dydis"<<" = "<<dydisX<<", "<<dydisY<<endl;
	fr<<"fps"<<" = "<<fps<<endl;
	fr<<"bpp"<<" = "<<bpp<<endl;
	fr<<"z_bpp"<<" = "<<zBpp<<endl;
	fr<<"accum_bpp"<<" = "<<accumBpp<<endl;
	fr<<"stencil_bpp"<<" = "<<stencilBpp<<endl;
	fr<<"fullscr"<<" = "<<fullScr<<endl;
	fr<<"v-sync"<<" = "<<ribotiFps;

	fr.close();
}


void KNustatymai::Naikinti()
{
	if(gpuStr != NULL){
		delete[] gpuStr;
		gpuStr = NULL;
	}
	if(pav != NULL){
		delete[] pav;
		pav = NULL;
	}
}


#ifdef DEBUG
void KNustatymai::Log()
{
	cout<<pozX<<"  "<<pozY<<"  "<<dydisX<<"  "<<dydisY<<"  "<<fps<<"  "<<
	  bpp<<"  "<<zBpp<<"  "<<accumBpp<<"  "<<stencilBpp<<"  "<<fullScr<<"  "<<
	  ribotiFps<<"  \n"<<pavad<<"  "<<wndClass<<"  "<<&(gpuStr)<<"  "<<kalba<<endl;
}
#endif
