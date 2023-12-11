// zur Verhinderung doppelter Definition
#ifndef FRAKTALGENERATOR_H
#define FRAKTALGENERATOR_H

//#include <math.h>
#include <string>
#include <vector>
#include <iostream>

#include "Hilfsfunktionen.h"
#include "ZufallsGenerator.h"
#include "Zeitmesser.h"

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// Parameter eines L-Systems
	enum ELSystemEigenschaft {
		_LSYSTEMEIGENSCHAFT_WINKEL_GIER_,
		_LSYSTEMEIGENSCHAFT_WINKEL_NICK_,
		_LSYSTEMEIGENSCHAFT_WINKEL_ROLL_,
		_LSYSTEMEIGENSCHAFT_START_RADIUS_,
		_LSYSTEMEIGENSCHAFT_RADIUS_FAKTOR_,
		_LSYSTEMEIGENSCHAFT_RADIUS_DEKREMENTOR_
	};

	// Ableitungen eines Lindenmayer-Systems
	struct SLSystemAbleitungen
	{
		std::vector<std::string> FraktalIteration;
		double WinkelGier, WinkelNick, WinkelRoll, StartRadius, RadiusFaktor, RadiusDekrementor;		
	};

	// Ersetzungsregel
	struct SSymbolErsetzung
	{
		char Symbol;
		std::string Ersetzung;
	};

	// Generator fuer Lindenmayer-Systeme
	class CFraktalGenerator
	{
	public:
		// Initialisierung
		CFraktalGenerator(CZufallsGenerator* _ZufallsGenerator, CZeitmesser* _Zeitmesser);

		// Generierung der Fraktal-Zeichenanweisung
		SLSystemAbleitungen* GeneriereAbleitungen(unsigned int _MaxIteration);
		
		// Regeln hinzufuegen oder loeschen
		void FuegeRegelHinzu(char _Symbol, std::string _Ersetzung);
		void LoescheRegeln();

		// Parameter einstellen
		void SetzeStart(std::string _Start);
		void SetzeParameter(ELSystemEigenschaft _Parameter, double _Wert);

		// Zufallserzeugung
		void FuegeZufallsRegelHinzu(char _Symbol, std::string _ErlaubteSymboleFuerErsetzung, unsigned int _MinLaenge, unsigned int _MaxLaenge);
		void SetzeZufallsStart(std::string _ErlaubteSymboleFuerErsetzung, unsigned int _MinLaenge, unsigned int _MaxLaenge);
		void SetzeZufallsParameter(ELSystemEigenschaft _Parameter, double _MinWert, double _MaxWert, int _Kommastellen);
				// _Kommastellen: positiv - Stellen nach dem Komma, negativ - Stellen vor dem Komma, 0 - ganze Zahl
				// _minWert und _maxWert werden ebenfalls entsprechend gerundet

		// Lese-Zugriff
		std::string LeseStart();
		double LeseParameter(ELSystemEigenschaft _Parameter);
		unsigned int LeseRegelAnzahl();
		SSymbolErsetzung LeseRegel(unsigned int i);
		
		// Parameter ausgeben
		void GibParameterAus();

	private:		
		// Ersetze nach Regeln
		inline std::string Ersetze(std::string _AusgangsString);

		// generiere Zufallstring
		std::string GeneriereZufallsString(std::string _ErlaubteSymboleFuerErsetzung, unsigned int _MinLaenge, unsigned int _MaxLaenge);

		// Zeitmessung
		CZeitmesser* Zeitmesser;
		// Zufallsgenerator
		CZufallsGenerator* ZufallsGenerator;

		// Maximal zulaessige Stringlaenge fuer generierte Ableitungen
		static const unsigned int MAX_SIZE = 1000000;

		// Parameter
		std::string Start;
		std::vector<SSymbolErsetzung> ErsetzungsRegeln;
		double WinkelGier, WinkelNick, WinkelRoll, StartRadius, RadiusFaktor, RadiusDekrementor;
	};

} // END NAMESPACE DunGen

#endif