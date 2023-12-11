// Implementierung der Klasse FraktalGenerator
#include "FraktalGenerator.h"

// ======================================================
// Initialisierung
// ======================================================

DunGen::CFraktalGenerator::CFraktalGenerator(CZufallsGenerator* _ZufallsGenerator, CZeitmesser* _Zeitmesser)
{
	// Werte uebernehmen
	ZufallsGenerator = _ZufallsGenerator;
	Zeitmesser = _Zeitmesser;

	// Initialwerte
	WinkelGier = 0.0;
	WinkelNick = 0.0;
	WinkelRoll = 0.0;
	StartRadius = 10.0;
	RadiusFaktor = 1.0;
	RadiusDekrementor = 0.0;
	Start = "F";
}

// ======================================================
// Funktionen zum Setzen von Parametern
// ======================================================

void DunGen::CFraktalGenerator::FuegeRegelHinzu(char _Symbol, std::string _Ersetzung)
{
	// suchen, ob Symbol schon vorhanden
	for(int i=0; i<ErsetzungsRegeln.size(); i++)
		if (ErsetzungsRegeln.at(i).Symbol == _Symbol)
		{
			// dann aktualisieren
			ErsetzungsRegeln.at(i).Ersetzung = _Ersetzung;
			return;
		}
	
	// sonst: Symbol noch nicht vorhanden, also hinzufuegen
	// neue Regel konstruieren
	SSymbolErsetzung neueRegel;
	neueRegel.Symbol = _Symbol;
	neueRegel.Ersetzung = _Ersetzung;
	// hinzufuegen
	ErsetzungsRegeln.push_back(neueRegel);
}
void DunGen::CFraktalGenerator::LoescheRegeln()
{
	// alle Regeln loeschen
	ErsetzungsRegeln.clear();
}
void DunGen::CFraktalGenerator::SetzeStart(std::string _Start)
{
	// Wert uebernehmen
	Start = _Start;
}
void DunGen::CFraktalGenerator::SetzeParameter(ELSystemEigenschaft _Parameter, double _Wert)
{
	// entsprechenden Wert zuweisen
	switch(_Parameter)
	{
		case _LSYSTEMEIGENSCHAFT_WINKEL_GIER_:			WinkelGier=_Wert;			break;
		case _LSYSTEMEIGENSCHAFT_WINKEL_NICK_:			WinkelNick=_Wert;			break;
		case _LSYSTEMEIGENSCHAFT_WINKEL_ROLL_:			WinkelRoll=_Wert;			break;
		case _LSYSTEMEIGENSCHAFT_START_RADIUS_:			StartRadius=_Wert;			break;
		case _LSYSTEMEIGENSCHAFT_RADIUS_FAKTOR_:		RadiusFaktor=_Wert;			break;
		case _LSYSTEMEIGENSCHAFT_RADIUS_DEKREMENTOR_:	RadiusDekrementor=_Wert;	break;
	}
}

// ======================================================
// Funktionen zum Setzen von Zufalls-Parametern
// ======================================================

std::string DunGen::CFraktalGenerator::GeneriereZufallsString(std::string _ErlaubteSymboleFuerErsetzung, unsigned int _MinLaenge, unsigned int _MaxLaenge)
{
	// String fuer Ersetung konstruieren
	std::string zufallsString = "";

	// Laenge der Regel bestimmen
	unsigned int laenge = ZufallsGenerator->GeneriereZufallsWert(_MinLaenge,_MaxLaenge);

	// Ersetzung ermitteln
	unsigned int index;
	// zufaelligen Index ermitteln
	index = ZufallsGenerator->GeneriereZufallsWert0Max_(_ErlaubteSymboleFuerErsetzung.length());
	// entsprechendes Symbol hinzufuegen
	zufallsString += _ErlaubteSymboleFuerErsetzung.at(index);
	
	// Check auf Sonderfaelle
	bool nurSymb1 = true;
	bool nurSymb2 = true;
	for (unsigned int i = 0; i<_ErlaubteSymboleFuerErsetzung.size(); i++)
	{
		if (_ErlaubteSymboleFuerErsetzung.at(i) != '|')
			nurSymb1 = false;
		if (_ErlaubteSymboleFuerErsetzung.at(i) != '$')
			nurSymb2 = false;
	}

	if (nurSymb1 || nurSymb2)
		return zufallsString;
	
	// Weitere Symbole hinzufuegen
	for (unsigned int i = 1; i<laenge; i++)
	{
		// zufaelligen Index ermitteln
		index = ZufallsGenerator->GeneriereZufallsWert0Max_(_ErlaubteSymboleFuerErsetzung.length());
		// Sinnfreie Kombinationen verhindern
		while ((zufallsString.at(i-1)=='[' && _ErlaubteSymboleFuerErsetzung.at(index) ==']')
			|| (zufallsString.at(i-1)=='+' && _ErlaubteSymboleFuerErsetzung.at(index) =='-')
			|| (zufallsString.at(i-1)=='-' && _ErlaubteSymboleFuerErsetzung.at(index) =='+')
			|| (zufallsString.at(i-1)=='u' && _ErlaubteSymboleFuerErsetzung.at(index) =='o')
			|| (zufallsString.at(i-1)=='o' && _ErlaubteSymboleFuerErsetzung.at(index) =='u')
			|| (zufallsString.at(i-1)=='g' && _ErlaubteSymboleFuerErsetzung.at(index) =='z')
			|| (zufallsString.at(i-1)=='z' && _ErlaubteSymboleFuerErsetzung.at(index) =='g')
			|| (zufallsString.at(i-1)=='|' && _ErlaubteSymboleFuerErsetzung.at(index) =='|')
			|| (zufallsString.at(i-1)=='$' && _ErlaubteSymboleFuerErsetzung.at(index) =='$'))
			index = ZufallsGenerator->GeneriereZufallsWert0Max_(_ErlaubteSymboleFuerErsetzung.length());

		// entsprechendes Symbol hinzufuegen
		zufallsString += _ErlaubteSymboleFuerErsetzung.at(index);
	}
	
	// Ergebnis zurueckliefern
	return zufallsString;
}


void DunGen::CFraktalGenerator::FuegeZufallsRegelHinzu(char _Symbol, std::string _ErlaubteSymboleFuerErsetzung, unsigned int _MinLaenge, unsigned int _MaxLaenge)
{
	// String fuer Ersetung konstruieren
	std::string ersetzung = GeneriereZufallsString(_ErlaubteSymboleFuerErsetzung,_MinLaenge,_MaxLaenge);

	// Regel hinzufuegen
	FuegeRegelHinzu(_Symbol, ersetzung);
}

void DunGen::CFraktalGenerator::SetzeZufallsStart(std::string _ErlaubteSymboleFuerErsetzung, unsigned int _MinLaenge, unsigned int _MaxLaenge)
{
	// Zufaelligen Start-String ermitteln und zuweisen
	Start = GeneriereZufallsString(_ErlaubteSymboleFuerErsetzung,_MinLaenge,_MaxLaenge);
}

void DunGen::CFraktalGenerator::SetzeZufallsParameter(ELSystemEigenschaft _Parameter, double _MinWert, double _MaxWert, int _Kommastellen)
{
	// Genauigkeit festlegen
	double umrechnungsfaktor = pow(10.0,_Kommastellen);

	// Schranken ausrechnen
	int minWert =  _MinWert * umrechnungsfaktor;
	if (_MinWert * umrechnungsfaktor > (double)minWert)	// Schranken einhalten
		minWert ++;
	int maxWert = _MaxWert * umrechnungsfaktor;
	if (maxWert < minWert) maxWert = minWert;	

	// finalen Wert berechnen
	int wert = ZufallsGenerator->GeneriereZufallsWert(minWert,maxWert);
	// Kommastellen wiederherstellen
	double resultat = ((double)wert)/umrechnungsfaktor;
	SetzeParameter(_Parameter, resultat);
}

// ======================================================
// Generierung der Strings
// ======================================================

inline std::string DunGen::CFraktalGenerator::Ersetze(std::string _AusgangsString)
{
	// initialisiere neuen String
	std::string neuerString = "";
	
	// laufe ueber Ausgangsstring
	bool existiertErsetzung;
	for (unsigned int i=0; i<_AusgangsString.size(); i++)
	{
		existiertErsetzung = false;
		// finde passende Ersetzung
		for (unsigned int j=0; j<ErsetzungsRegeln.size(); j++)
			if (_AusgangsString.at(i) == ErsetzungsRegeln.at(j).Symbol)
			{
				// ersetze
				neuerString += ErsetzungsRegeln.at(j).Ersetzung;
				existiertErsetzung = true;
				// Schleifenabbruch
				j = ErsetzungsRegeln.size();
			}
		// keine Ersetzung gefunden: uebernimm Symbol
		if (!existiertErsetzung) neuerString += _AusgangsString.at(i);
	}
			
	// gib Ergebnis der Ersetzung zurueck
	return neuerString;
}

// ======================================================
// Generierung des Fraktals
// ======================================================
DunGen::SLSystemAbleitungen* DunGen::CFraktalGenerator::GeneriereAbleitungen(unsigned int _MaxIteration)
{
	std::cout << "[Fraktalgenerator]: Beginne mit Generierung der L-System-Ableitungen..." << std::endl;
	Zeitmesser->Start(0);
	GibParameterAus();

	// neues Fraktalobjekt erstellen
	SLSystemAbleitungen* neuesFraktal = new SLSystemAbleitungen();
	// Werte uebernehmen
	neuesFraktal->WinkelGier = WinkelGier;
	neuesFraktal->WinkelNick = WinkelNick;
	neuesFraktal->WinkelRoll = WinkelRoll;
	neuesFraktal->StartRadius = StartRadius;
	neuesFraktal->RadiusFaktor = RadiusFaktor;
	neuesFraktal->RadiusDekrementor = RadiusDekrementor;

	// Iterationen generieren
	std::string aktString = Start;
	neuesFraktal->FraktalIteration.push_back(aktString);	 // Iteration 0 = Start

	for (unsigned int i=0 ; i<_MaxIteration; i++)
	{	
		// neuen String replizieren
		aktString = Ersetze(aktString);

		// Abbruch der Iteration wenn String zu gross
		if (aktString.size()>MAX_SIZE) break;

		// String der Fraktalbeschreibung hinzufuegen
		neuesFraktal->FraktalIteration.push_back(aktString);				
	}

	std::cout << "[Fraktalgenerator]: Generierung abgeschlossen." << std::endl
		<< "[Fraktalgenerator]: Erzeugte max. Iterationsstufe = " << neuesFraktal->FraktalIteration.size()-1
		<< " (mit Stringlaenge = " << neuesFraktal->FraktalIteration.at(neuesFraktal->FraktalIteration.size()-1).size() << ")" << std::endl;
	std::cout << "[Fraktalgenerator]: Benoetigte Zeit fuer Generierung der L-System-Ableitungen: ";
	Zeitmesser->Stop(0);

	// Fraktal zurueckgeben
	return neuesFraktal;
}

// ======================================================
// Lesezugriff
// ======================================================
std::string DunGen::CFraktalGenerator::LeseStart()
{
	return Start;
}

double DunGen::CFraktalGenerator::LeseParameter(ELSystemEigenschaft _Parameter)
{
	// entsprechenden Wert zurueckgeben
	switch(_Parameter)
	{
		case _LSYSTEMEIGENSCHAFT_WINKEL_GIER_:			return WinkelGier;			break;
		case _LSYSTEMEIGENSCHAFT_WINKEL_NICK_:			return WinkelNick;			break;
		case _LSYSTEMEIGENSCHAFT_WINKEL_ROLL_:			return WinkelRoll;			break;
		case _LSYSTEMEIGENSCHAFT_START_RADIUS_:			return StartRadius;			break;
		case _LSYSTEMEIGENSCHAFT_RADIUS_FAKTOR_:		return RadiusFaktor;		break;
		case _LSYSTEMEIGENSCHAFT_RADIUS_DEKREMENTOR_:	return RadiusDekrementor;	break;
	}
	return 0.0;
}

unsigned int DunGen::CFraktalGenerator::LeseRegelAnzahl()
{
	return ErsetzungsRegeln.size();
}

DunGen::SSymbolErsetzung DunGen::CFraktalGenerator::LeseRegel(unsigned int i)
{
	return ErsetzungsRegeln.at(i);
}

// ======================================================
// Ausgabe
// ======================================================
// Ausgabe aller Parameter
void DunGen::CFraktalGenerator::GibParameterAus()
{
	std::cout << "WinkelGier: " << WinkelGier << std::endl;
	std::cout << "WinkelNick: " << WinkelNick << std::endl;
	std::cout << "WinkelRoll: " << WinkelRoll << std::endl;
	std::cout << "RadiusFaktor: " << RadiusFaktor << std::endl;
	std::cout << "RadiusDekrementor: " << RadiusDekrementor << std::endl;
	std::cout << "StartRadius: " << StartRadius << std::endl;

	std::cout << "Start: " << Start << std::endl;
	for (unsigned int i=0; i<ErsetzungsRegeln.size(); i++)
		std::cout << ErsetzungsRegeln.at(i).Symbol << " -> " << ErsetzungsRegeln.at(i).Ersetzung << std::endl;
}
