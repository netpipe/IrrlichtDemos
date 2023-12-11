// zur Verhinderung doppelter Definition
#ifndef ZEITMESSER_H
#define ZEITMESSER_H

#include <iostream>
#include <time.h>

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// Klasse Zeitmesser
	class CZeitmesser
	{
	public:
		void Start(unsigned int i)
		{
			// Zeitmessung starten
			ZeitStart[i] = clock();
			ZwischenZeit[i] = 0;
		}

		void Pause(unsigned int i)
		{
			// Zeitmessung pausieren
			ZeitEnde = clock();
			ZwischenZeit[i] += ZeitEnde - ZeitStart[i];
		}

		void Fortsetzen(unsigned int i)
		{
			// Zeitmessung wiederaufnehmen
			ZeitStart[i] = clock();
		}

		void Stop(unsigned int i)
		{
			// Zeitmessung vollenden
			ZeitEnde = clock();
			ZwischenZeit[i] += ZeitEnde - ZeitStart[i];
			std::cout << "CPU-Zeit = " << ZwischenZeit[i] << " bei CLOCKS_PER_SEC = " << CLOCKS_PER_SEC << std::endl;
		}

	private:
		// Zeitmessung
		clock_t ZeitStart[4], ZwischenZeit[4], ZeitEnde;
	};
	
} // END NAMESPACE DunGen

#endif