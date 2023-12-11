// zur Verhinderung doppelter Definition
#ifndef ZUFALLSGENERATOR_H
#define ZUFALLSGENERATOR_H

#include <iostream>

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// Generator fuer Zufallszahlen
	class CZufallsGenerator
	{
	public:		

		//==================================
		// Initialisierung, Parameter setzen/lesen
		//==================================
		
		CZufallsGenerator()
		{
			SetzeZufallsSeed(0);

			// Werte angeleht an Beispiel fuer rand() nach IEEE-Std 1003.1, 2004 Edition
			// Quelle: http://pubs.opengroup.org/onlinepubs/009695399/functions/rand.html
			SetzeA(1103515245);
			SetzeC(12345);
			SetzeM(32768);

			// Werte fuer rand()-Funktion nach BSD-Unix
			// Quelle: http://cplus.kompf.de/artikel/random.html
			//SetzeA(1103515245);
			//SetzeC(12345);
			//SetzeM(1<<31);	// 2^31
			
			// Werte fuer rand48()-Funktionen nach IEEE-Std 1003.1, 2004 Edition
			// Quelle: http://pubs.opengroup.org/onlinepubs/009695399/functions/drand48.html
			// Achtung Datentyp unsigned int nicht fuer 2^48 ausreichend! unsigned int hat i.A. nur 32 Bit Groesse
			//SetzeA(0x5DEECE66D16);
			//SetzeC(0xB);
			//SetzeM(1<<48);	// 2^48	-> Datentyp nicht ausreichend
		}
		
		// setzt den Seed fuer die danach generierten Zufallswerte
		void SetzeZufallsSeed(unsigned int _Seed)
		{
			X = _Seed;
		}

		// Parameter setzen
		void SetzeA(unsigned int _A)
		{
			A = _A;
			//std::cout << "[Zufallsgenerator]: Setze A = " << A << std::endl;
		}
		void SetzeC(unsigned int _C)
		{
			C = _C;
			//std::cout << "[Zufallsgenerator]: Setze C = " << C << std::endl;
		}
		void SetzeM(unsigned int _M)
		{			
			M = _M;
			if (M<2) M = 2; // Division durch 0 vermeiden

			MaximalWertPlus1 = M;
			MaximalWert = MaximalWertPlus1 - 1.0;
			//std::cout << "[Zufallsgenerator]: Setze M = " << M << std::endl;
		}

		// Parameter lesen
		unsigned int LeseA()
		{
			return A;
		}
		unsigned int LeseC()
		{
			return C;
		}
		unsigned int LeseM()
		{
			return M;
		}

		//==================================
		// Zufallsgenerierungsfunktionen
		//==================================

		// Basis-Zufalls-Funktion
		unsigned int GeneriereZufallsWertBasis()
		{
			return ZufallsZahl();
		}

		// generiert Zufallswert im Intervall [_Minimum,_Maximum]
		// wichtig: Maximum >= Minimum muss gelten!
		int GeneriereZufallsWert(int _Minimum, int _Maximum)
		{
			return ZufallsZahl()%(_Maximum-_Minimum+1)+_Minimum;
		}

		// generiert Zufallswert im Intervall [0,_Maximum)
		// wichtig: Maximum > 0 muss gelten!
		unsigned int GeneriereZufallsWert0Max_(unsigned int _Maximum)
		{
			return ZufallsZahl()%_Maximum;
		}

		// generiert Zufallswert im Intervall [0,1]
		double GeneriereZufallsWert01()
		{
			return ((double)ZufallsZahl()) / MaximalWert;
		}

		// generiert Zufallswert im Intervall (0,1]
		double GeneriereZufallsWert_01()
		{
			return ((double)(ZufallsZahl()+1)) / MaximalWertPlus1;
		}

	private:
		
		inline unsigned int ZufallsZahl()
		{
			// Zufallsgenerierung nach x = (a * x + c) % m
			X = (A * X + C) % M;
			return X;
		}
		
		// Parameter
		unsigned int A;
		unsigned int C;
		unsigned int M;		// maximaler Wert + 1
		unsigned int X;		// aktueller Seed
		// Vorkonvertierung in double
		double MaximalWert;
		double MaximalWertPlus1;

	};

} // END NAMESPACE DunGen

#endif