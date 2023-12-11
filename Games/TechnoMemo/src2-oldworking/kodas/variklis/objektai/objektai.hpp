#ifndef OBJEKTAI_HPP
#define OBJEKTAI_HPP
#define OBJ_SK 64
#define OBJ_PAV_ILG 32


class KObj{
    public:
	TDOFailas obj[OBJ_SK];
	char	pav[OBJ_SK][OBJ_PAV_ILG];
	int		objSk;
	bool	multiTekst;		//Ar naudoti MultiTexture?
	
	KObj();
	~KObj();
    void Uzkrauti(char*);
    void Piesti(unsigned int num, int LOD);
    TDOFailas * ID(const char*);
    unsigned int Num(const char*);
    void Naikinti();
} objektai;


#include "objektai.cpp"
#endif
