/***********************************************************
	WAV lib'as

	© 2009 A-E Software ®
	Arvydas Burdulis
	http://ae.gamedev.lt  aab@auto.lt


	WAV failu uzkrovimas, lygiu generavimas.
	Palaikomi formatai: unsigned 8-bit mono, signed 8-bit stereo, signed 16-bit mono.
	Garsas turi buti PCM standarto, suspaudimas nepalaikomas.
	Lygiu generavimas - tai garsumo skaiciavimas
	'WAV_DAZNIS' kartu per sekunde. Suskaiciuotus
	lygius galima issaugoti i faila. Lygiu generavimui formatas
	turi buti signed 16-bit mono.

	Pradeta:
	    2008-02-21
	Paskutini karta keista:
	    2009-01-18
************************************************************/

/*      WAV HEADERIS (12 baitu)
 	0-3		char			chunkID[4];		//Visada 'RIFF'
 	4-7		unsigned int	chunkSize;		//Viso failo dydis (be pirmu 12 baitu)
 	8-11	unsigned int	formatas;		//Visada 'WAVE' (WAV failams)

 	Toliau eina blokai, prasidedantys 8 baitu aprasymu (char[4] ir unsigned int)
	Char eilute nurodo, kuri dalis aprasoma bloke, unsigned int - kiek baitu uzima
	visas blokas (neskaitant siu 8 baitu bloko aprasymui).
	WAV faile butinai turi buti "fmt " ir "data" blokai. "fmt " blokas apraso
	garso formata (dazni, kanalu kieki...), jo dydis visada 16 baitu.
	"data" blokas saugo garso duomenis. Papildomai gali buti ir kitokie blokai.
*/


#ifndef WAV_HPP
#define WAV_HPP
#include <defines.hpp>
#ifdef DEBUG
#include <variklis/logas.hpp>
#endif
#include <AL/al.h>
#include <stdio.h>
#define	WAV_DAZNIS 60


class KWav{
	public:

	//Ivedimui reikalingi kintamieji
	FILE			*failas;
	bool			formIv,
					duomIv,
					genLyg;
	char			chunkID[5];
	unsigned int	chunkSize;

	//Garso formatas
	unsigned short	formatas,
					kanalu,
					blockAlign,
					bitsPerSample;
	unsigned int	sampleRate,
					byteRate,
					alFormatas;

	//Garso duomenys
	float			trukme;
	signed short	*dm;
	unsigned int	dmDydis;
	unsigned int	*duom;
	unsigned int	duomDydis;
	unsigned int	duomDaznis;

	KWav();
	~KWav();
	void Uzkrauti(const char*) throw(const char*);
	void GenLygius();
	void IssaugLygius(char*);
	void Naikinti();

	private:
	void SkaitHead();
	void SkaitFormat();
	void SkaitDuom();
};


#endif
