/***********************************************************
	Linkas

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	Linko sukurimas. Parametrai: linko adresas, linkinamo
	failo adresas, linko komentaras, ikona, darbinis katalogas.

	Pradeta:
	    2008-03-26
	Paskutini karta keista:
	    2009-01-18
************************************************************/


#ifndef LINKAS_HPP
#define LINKAS_HPP


class KLinkas{
	public:
	char	*lPav,		//Linko adresas
			*fPav,		//Linkinamo failo adresas
			*apr,		//Aprasymas
			*ikona,		//Ikona
			*dir;		//Darbinis katalogas
	
	KLinkas();
	~KLinkas();
	
	void Sukurti() throw(const char*);
	static void Sukurti(const char*, const char*, const char*,
			const char*, const char*) throw(const char*);
};


#endif
