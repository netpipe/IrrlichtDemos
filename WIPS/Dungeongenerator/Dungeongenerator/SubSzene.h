// zur Verhinderung doppelter Definition
#ifndef SUBSZENE_H
#define SUBSZENE_H

#include <iostream>
#include <vector>
#include <stack>
#include <irrlicht.h>
#include "Spline.h"

// Namespace DunGen : DungeonGenerator
namespace DunGen
{
	// Die Adjazenzlisten werden fuer das Bestimmen der Andockstellen verwendet
	struct SAdjazenterVertex
	{
		bool Mehrfach;
		unsigned int Index;
	};
	struct SAdjazenzListe
	{
		std::vector<SAdjazenterVertex> AdjazenteVertices;
		void AdjazenzHinzufuegen(unsigned int _AdjazenterVertex);
	};

	// Die Klasse der SubSzene
	class CSubSzene
	{
	public:
		// Konstruktor: Szene einladen
		CSubSzene(irr::IrrlichtDevice* _IrrDevice, const irr::io::path _Dateiname);
		// Destruktor
		~CSubSzene();

		// Szene in anderen Szenegraphen kopieren
		void SzeneHinzufuegen(irr::scene::ISceneNode* _Elternknoten, irr::scene::ISceneManager* _SceneManager);

		// Ableitungsfaktoren setzen
		void SetzeAbleitungsFaktorRaumRaum(double _AbleitungsFaktorRaumRaum);
		void SetzeAbleitungsFaktorRaumHoehle(double _AbleitungsFaktorRaumHoehle);
		void SetzeAbleitungsFaktorHoehleRaum(double _AbleitungsFaktorHoehleRaum);
		// Platzierungswahrscheinlichkeitsfaktoren setzen
		void SetzePlatzierungsWahrscheinlichkeitsFaktorAnHoehle(unsigned int _PlatzierungsWahrscheinlichkeitsFaktorAnHoehle);
		void SetzePlatzierungsWahrscheinlichkeitsFaktorAnRaum(unsigned int _PlatzierungsWahrscheinlichkeitsFaktorAnRaum);
		// Skalierung setzen
		void SetzeSkalierung(irr::core::vector3d<double> _Skalierung);
		// Benennung setzen
		void SetzeName(irr::core::stringc _Name);

		// Andockstellen auslesen
		SSplineAndockStelle LeseAndockstelle(unsigned int _Andockstelle);
		
		// Ableitungsfaktoren auslesen
		double LeseAbleitungsFaktorRaumRaum();
		double LeseAbleitungsFaktorRaumHoehle();
		double LeseAbleitungsFaktorHoehleRaum();
		// Platzierungswahrscheinlichkeitsfaktoren auslesen
		unsigned int LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehle();
		unsigned int LesePlatzierungsWahrscheinlichkeitsFaktorAnRaum();
		// Skalierung auslesen
		irr::core::vector3d<double> LeseSkalierung();
		// Benennung auslesen
		irr::core::stringc LeseName();

		// Pfad auslesen (fuer GUI)
		irr::io::path LesePfad();

	private:
		// Andockstellen berechnen
		void AndockstelleBerechnen(unsigned int _Andockstelle);

		// eigener Szenemanager zum Laden und Verwalten der SubSzene
		irr::scene::ISceneManager* EigenerSceneManager;

		// Andockstellen in Reihenfolge: Nord (X positiv), Ost, Sued, West
		SSplineAndockStelle Andockstelle[4];

		// Skalierung der SubSzene bei Verwendung als Raum des Dungeons
		irr::core::vector3d<double> Skalierung;

		// Ableitungsfaktoren fuer moegliche Gaenge
		double AbleitungsFaktorRaumRaum;
		double AbleitungsFaktorRaumHoehle;
		double AbleitungsFaktorHoehleRaum;

		// Platzierungswahrscheinlichkeitsfaktoren
		unsigned int PlatzierungsWahrscheinlichkeitsFaktorAnHoehle;
		unsigned int PlatzierungsWahrscheinlichkeitsFaktorAnRaum;

		// Benennung
		irr::core::stringc Name;
		// Pfad der Subszene (fuer GUI)
		irr::io::path Pfad;
	};

} // END NAMESPACE DunGen

#endif