KObj::KObj(): objSk(0), multiTekst(0)
{
}

KObj::~KObj()
{
}

void KObj::Uzkrauti(char *pavad)
{
	FUzkrovimas failai;
	char *pav1, *pav2;
	
	failai.Pradeti(pavad, "3DO");
	if(failai.sk > 0){
		while((pav1 = failai.Gauti()) != NULL){
			if(objSk >= OBJ_SK){
				Klaida("Pasiektas objektu limitas (%d)", OBJ_SK);
				break;
			}
            try{
				if(multiTekst)
					obj[objSk].UzkrautiMulti(pav1);
				else
					obj[objSk].Uzkrauti(pav1);
			}
            catch(const char *klaida){
				Klaida("Klaida uzkraunant objekta '%s': %s", pav1, klaida);
				continue;
			}
			pav2 = utils.GautiPav(pav1);
			strcpy(pav[objSk], pav2);
			obj[objSk].tekst = teksturos.ID(obj[objSk].teks);
			
			#ifdef DEBUG
				cout<<"\n  "<<pav[objSk]<<" spind: "<<obj[objSk].spind
					<<" Coll.: "<<(int)obj[objSk].collSk<<"  pag: "<<int(obj[objSk].pagSk)<<"  tekst: "<<obj[objSk].teks;
			#endif
			
			objSk++;
        }
	}
    else
        Ispejimas("Nerasta objektu aplanke '%s'", pavad);
}

void KObj::Piesti(unsigned int num, int LOD)
{
	if(num < 0 || num >= objSk){
		Klaida("Klaida piesiant objekta: netinkamas numeris");
		return;
	}
	if(LOD < 0 || LOD >= 3){
		Klaida("Klaida piesiant objekta: netinkamas LOD");
		return;
	}
	
	glBindTexture(GL_TEXTURE_2D, obj[num].tekst);
	glCallList(obj[num].dispList + LOD);
}

TDOFailas * KObj::ID(const char *pavad)
{
	for(int i = 0; i < objSk; i++)
        if(strcmp(pav[i], pavad) == 0)
            return &obj[i];
    Ispejimas("Nerastas objektas '%s'", pavad);
    return NULL;
}

unsigned int KObj::Num(const char *pavad)
{
	for(int i = 0; i < objSk; i++)
        if(strcmp(pav[i], pavad) == 0)
            return i;
    Ispejimas("Nerastas objektas '%s'", pavad);
    return 0;
}

void KObj::Naikinti()
{
	//for(int i = 0; i < objSk; i++)
	for(int i = objSk - 1; i >= 0; i--)
		obj[i].Naikinti();
	objSk = 0;
}
