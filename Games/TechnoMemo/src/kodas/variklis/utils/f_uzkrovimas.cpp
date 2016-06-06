#include "f_uzkrovimas.hpp"
#include <string.h>

FUzkrovimas::FUzkrovimas(): num(0), sk(0), uzkrauta(false)
{
	for(int i = 0; i < F_UZKR_PAV; i++)
		pav[i] = NULL;
}


FUzkrovimas::~FUzkrovimas()
{
}


//Gaunami visi aplanko dir failai, turintys pletini plet.
//Aplanko adresas turi baigtis '/' arba '\' simboliu.
//Failo pletinys rasomas be tasko.
//Jei pletinys nenurodomas (NULL), randami visi failai
void FUzkrovimas::Pradeti(char *dir, char *plet)
{
	char eil[512];
	Naikinti();

	#ifdef DEBUG
		#ifdef OS_WIN32
		char simb_skyr[] = "\\";
		#else
		char simb_skyr[] = "/";
		#endif
	    if(strlen(dir) > 0)
		    if(dir[strlen(dir) - 1] != '/' && dir[strlen(dir) - 1] != '\\'){
//		    	Logas::Log("Blogas aplanko adresas: '%s', truksta '%s' simbolio", dir, simb_skyr);
		    	char *temp = dir;
		    	dir = new char[strlen(dir) + 2];
		    	strcpy(dir, temp);
		    	strcat(dir, simb_skyr);
			}
	#endif


	#ifdef OS_WIN32		//Windows kodas -----
	WIN32_FIND_DATA failas;
    HANDLE handlius;

    strcpy(eil, dir);
    strcat(eil, "*.");
    if(plet != NULL)
    	strcat(eil, plet);
    else
    	strcat(eil, "*");
    handlius = FindFirstFile(eil, &failas);
    if(handlius != INVALID_HANDLE_VALUE)
        do{
			if(sk >= F_UZKR_PAV - 1){
				#ifdef DEBUG
				Logas::Log("Aplanke '%s' per daug failu su pletiniu '.%s', maksimalus skaicius yra %d",
				  dir, plet, F_UZKR_PAV - 1);
				#endif
				break;
			}
			//Failo pavadinima itraukti i sarasa
			pav[sk] = new char[strlen(dir) + strlen(failas.cFileName) + 1];
            strcpy(pav[sk], dir);
            strcat(pav[sk], failas.cFileName);
            #ifdef DEBUG
            //Logas::Log(pav[sk]);
            #endif
            sk++;
        }
		while(FindNextFile(handlius, &failas));
    /*else{
		#ifdef DEBUG
        if(plet != NULL)
			Ispejimas("Aplanke '%s' nerasta failu su pletiniu '.%s'", dir, plet);
		else
			Ispejimas("Aplanke '%s' nerasta failu", dir);
        #endif
	}*/
    FindClose(handlius);
    #endif

    #ifdef OS_UNIX			//GNU/Linux kodas ------

    /*char *pavad = new char[strlen(dir) + 11];
    strcpy(pavad, dir);
    strcat(pavad, "failai.txt");
    ifstream fd(pavad);
    if(fd.fail())
    	Ispejimas("Aplanke '%s' nerastas failu sarasas '%s'", dir, pavad);
    else{
		while(fd.good()){
			fd.get(eil, 511);
			fd.ignore();
			//Patikrinti, ar tinka failo pletinys
			if(strcmp(&eil[strlen(eil) - strlen(plet)], plet) != 0)
				continue;

			if(sk >= F_UZKR_PAV - 1){
				#ifdef DEBUG
				Ispejimas("Aplanke '%s' per daug failu su pletiniu '.%s', maksimalus skaicius yra %d",
				  dir, plet, F_UZKR_PAV - 1);
				#endif
				break;
			}
			pav[sk] = new char[strlen(dir) + strlen(eil) + 1];
			strcpy(pav[sk], dir);
			strcat(pav[sk], eil);
			cout<<pav[sk]<<endl;
			sk++;
		}
	}
    fd.close();*/

    DIR *diras;
    char *pletRod;
    char plet1[16], plet2[16];	//Reikalingi, kad butu panaikintas 'case sensitive'
	struct dirent *failas;
	if(plet != NULL){
		strcpy(plet1, plet);
		strlwr(plet1);
	}
	diras = opendir(dir);
	if(dir != NULL){
		while(failas = readdir(diras))
			if(strlen(failas->d_name) > 3){
				//Patikrinti, ar nepasiektas failu limitas
				if(sk >= F_UZKR_PAV - 1){
					#ifdef DEBUG
					if(plet != NULL)
						Logas::Log("Aplanke '%s' per daug failu su pletiniu '%s'", dir, plet);
					else
						Logas::Log("Aplanke '%s' per daug failu", dir);
					#endif
					break;
				}
				//Jei pletinys nenurodytas, failas tinkamas
				if(plet == NULL){
					pav[sk] = new char[strlen(failas->d_name) + strlen(dir) + 1];
					strcpy(pav[sk], dir);
					strcat(pav[sk], failas->d_name);
					sk++;
					continue;
				}
				//Patikrinti, ar sutampa pletinys
				pletRod = KUtils::GautiPlet(failas->d_name);
				strcpy(plet2, pletRod);
				strlwr(plet2);
				if(strcmp(plet1, plet2) == 0){
					//Pletiniai sutampa
					pav[sk] = new char[strlen(failas->d_name) + strlen(dir) + 1];
					strcpy(pav[sk], dir);
					strcat(pav[sk], failas->d_name);
					sk++;
				}
			}
		closedir(diras);
	}
	else
		#ifdef DEBUG
		Logas::Log("Nepavyko atidaryti aplanko '%s'", dir);
		#endif
    #endif

    if(sk > 0){
		#ifdef DEBUG
		//Logas::Log("f_uzkr (%s): %i", plet, sk);
		#endif
		uzkrauta = true;
	}
}


//Gaunamas sekantis failo pavadinimas
char * FUzkrovimas::Gauti()
{
	if(!uzkrauta)
		return NULL;
	else{
		#ifdef DEBUG
		if(num > sk)
			Logas::Log("FUzkrovimas: bandoma gauti failo pavadinima su per dideliu indeksu");
		#endif
		return pav[num++];
	}
}


//Naikinami duomenys
void FUzkrovimas::Naikinti()
{
	if(uzkrauta)
		for(int i = 0; i < sk; i++){
			delete[] pav[i];
			pav[i] = NULL;
		}
	sk = 0;
	num = 0;
	uzkrauta = false;
}
