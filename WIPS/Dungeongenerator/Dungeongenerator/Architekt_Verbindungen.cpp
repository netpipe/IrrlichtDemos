#include "Architekt.h"

//==================================================================
// Verbinden von Raeumen mit anderen Raumen oder der Hoehle
//==================================================================

// 2 Raeume miteinander verbinden
void DunGen::CArchitekt::VerbindeRaeume(unsigned int _IdRaum1, unsigned int _HimmelsrichtungRaum1, unsigned int _IdRaum2, unsigned int _HimmelsrichtungRaum2)
{
	Zeitmesser->Fortsetzen(1);
	// neuen Gang erstellen
	CSpline* gang = new CSpline(
	// Querschnitt des Gangs
	QuerschnittGang,
	// Andockstellen
	Raeume.at(_IdRaum1)->Andockstelle[_HimmelsrichtungRaum1],
	Raeume.at(_IdRaum2)->Andockstelle[_HimmelsrichtungRaum2],
	// Position der Splineenden
	Raeume.at(_IdRaum1)->Andockstelle[_HimmelsrichtungRaum1].Mittelpunkt
		+ Raeume.at(_IdRaum1)->Andockstelle[_HimmelsrichtungRaum1].Normale*(Raeume.at(_IdRaum1)->Andockstelle[_HimmelsrichtungRaum1].Ausdehnung+GangBreite),
	Raeume.at(_IdRaum2)->Andockstelle[_HimmelsrichtungRaum2].Mittelpunkt
		+ Raeume.at(_IdRaum2)->Andockstelle[_HimmelsrichtungRaum2].Normale*(Raeume.at(_IdRaum2)->Andockstelle[_HimmelsrichtungRaum2].Ausdehnung+GangBreite),
	// Richtung an den Splineenden
	Raeume.at(_IdRaum1)->Andockstelle[_HimmelsrichtungRaum1].Normale*Raeume.at(_IdRaum1)->SubSzene->LeseAbleitungsFaktorRaumRaum()*RaumSektorAusdehnung,
	Raeume.at(_IdRaum2)->Andockstelle[_HimmelsrichtungRaum2].Normale*Raeume.at(_IdRaum2)->SubSzene->LeseAbleitungsFaktorRaumRaum()*RaumSektorAusdehnung,
	// Abstand zwischen den Splinesegmenten
	AbstandSplineSegmente,
	// Wieviel TexturY-Koordinate pro Abstand 1 entlang des Gangs
	AbstandTexturYProAbstand1,
	// Anzahl der zusaetzlichen Detailstufen
	AnzahlSplineDetailstufen);
	Zeitmesser->Pause(1);

	Zeitmesser->Fortsetzen(2);
	// Invarianz der Dungeonskonstruktion gegenueber Detailobjekt-Platzierungen gewaehrleisten
	// durch Beibehaltung des aktuellern Zufallsseeds
	unsigned int seed = ZufallsGenerator->GeneriereZufallsWertBasis();
	// Detailobjekte platzieren
	SSplineDetailobjektVorlage* detailObjektVorlage;
	for (unsigned int i=0; i<DetailobjektVorlagen.size(); i++)
	{
		detailObjektVorlage = DetailobjektVorlagen.at(i);
		gang->PlatziereDetailobjekt(detailObjektVorlage->Name, detailObjektVorlage->ObjektMesh, detailObjektVorlage->Skalierung, detailObjektVorlage->Rotation,
			((double)GangBreite/2.0)*detailObjektVorlage->Position, detailObjektVorlage->AbstandsAbtastung, detailObjektVorlage->AbstandNumFaktor,
			detailObjektVorlage->AbstandNumMin, detailObjektVorlage->AbstandNumMax,
			detailObjektVorlage->AbstandNumMinErstesElement, detailObjektVorlage->AbstandNumMaxErstesElement, detailObjektVorlage->ObjektAn1,
			ZufallsGenerator);
	}
	ZufallsGenerator->SetzeZufallsSeed(seed);
	Zeitmesser->Pause(2);

	// Gang-ID's setzen
	gang->SetzeAndocksstellenIds(_IdRaum1,_IdRaum2);
	// Raumverbindungsstellen: GangID merken
	Raeume.at(_IdRaum1)->GangID[_HimmelsrichtungRaum1] = Gaenge.size();
	Raeume.at(_IdRaum2)->GangID[_HimmelsrichtungRaum2] = Gaenge.size();
	// Gang merken
	Gaenge.push_back(gang);
}

// versuchen Raum per Gang mit Hoehle zu verbinden, Himmelsrichtung siehe oben (vom Raum bzw. Sektor aus gesehen)
// Erfolg: Gang ist erstellt, true wird zurueckgeliefert
// Misserfolg: kein Gang wurde erstellet, false wird zurueckgeliefert
bool DunGen::CArchitekt::VerbindeRaumMitHoehle(unsigned int _IdRaum, int _SektorX, int _SektorY, int _SektorZ,	unsigned int _Himmelsrichtung)
{
	SRaumSektorAndockKarte* andockKarte = Raumsektor[_SektorX][_SektorY][_SektorZ].Karte[_Himmelsrichtung];

	int scanMin, scanMax;
	int ausfraesTiefe;	// wird zum Andocken verwendet, gleich hier bestimmen da Kartengebiet sowieso ueberprueft wird

	bool bedingungenEingehalten;
	bool positiveRichtung = (_Himmelsrichtung==0)||(_Himmelsrichtung==3);
	bool temp;
	bool abstandZuKurz;

	// Zufaelligen Ausgangspunkt fuer die Suche der Andockmoeglichkeit waehlen
	int suchAnfang1 = ZufallsGenerator->GeneriereZufallsWert(0,RaumSektorAusdehnung-1);
	int suchAnfang2 = ZufallsGenerator->GeneriereZufallsWert(0,RaumSektorAusdehnung-1);
	int resKoord1, resKoord2;

	// Suche nach moeglicher Andockmoeglichkeit
	for (int i=0; i<RaumSektorAusdehnung; i++)
		for (int j=0; j<RaumSektorAusdehnung; j++)
		{
			resKoord1 = (suchAnfang1 + i) % RaumSektorAusdehnung;
			resKoord2 = (suchAnfang2 + j) % RaumSektorAusdehnung + AbstandVomBodenMax;

			scanMin = INT_MAX;
			scanMax = INT_MIN;
			ausfraesTiefe = positiveRichtung ? INT_MAX : INT_MIN;
			bedingungenEingehalten = true;

			// ueberpruefe Nachbarschaft: Gangbreite + 1 Voxel Rand pro Seite fuers Andocken
			for (int i2=resKoord1; i2<=resKoord1+(int)GangBreite+1; i2++)
				for (int j2=resKoord2; j2<=resKoord2+(int)GangBreite+1; j2++)
				{
					if (andockKarte->Hoehenkarte[i2][j2]<scanMin)
						scanMin = andockKarte->Hoehenkarte[i2][j2];
					if (andockKarte->Hoehenkarte[i2][j2]>scanMax)
						scanMax = andockKarte->Hoehenkarte[i2][j2];
					// invalide Hoeheninformation?
					if (andockKarte->ValideHoeheninformation[i2][j2] == 0)
						bedingungenEingehalten = false;

					// Rand betrachten fuer Ausfraestiefe
					if ((i2==resKoord1)||(j2==resKoord2)||(i2==resKoord1+(int)GangBreite+1)||(j2==resKoord2+(int)GangBreite+1))
					{
						// gesucht wird minimaler Hoehenwert
						if (positiveRichtung && (andockKarte->Hoehenkarte[i2][j2]<ausfraesTiefe))
							ausfraesTiefe = andockKarte->Hoehenkarte[i2][j2];
						// gesucht wird maximaler Hoehenwert
						else if (!positiveRichtung && (andockKarte->Hoehenkarte[i2][j2]>ausfraesTiefe))
							ausfraesTiefe = andockKarte->Hoehenkarte[i2][j2];
					}

				};

			// Andockmoeglichkeit ist valide wenn Punkte im Bereich der Andockstelle sich in einem nicht zu grossen Quader befinden
			bedingungenEingehalten = bedingungenEingehalten && ((scanMax<INT_MAX) && (scanMin>INT_MIN) && ((scanMax-scanMin)<=GangBreite));

			abstandZuKurz = false; // fuer Abstandstest zu Boden
			if (bedingungenEingehalten)
			{
				switch (_Himmelsrichtung)
				{
				case 0:	// in X positiv
					// Test auf korrekten Abstand der Andockmoeglichkeit zu Sektor des zu verbindenden Raums
					if ((scanMin - Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMaxX) < (int)AbstandAndockHoehleMin)
						bedingungenEingehalten = false;
					// Zu nah am Ende der Scankarte um vollen Bodenabstandstest durchzufuehren?
					if (Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMaxX+2*(int)RaumSektorAusdehnung - scanMax < (int)TiefeAbstandsTestMin)
						abstandZuKurz = true;
					break;
				case 1:	// in Z negativ
					// Test auf korrekten Abstand der Andockmoeglichkeit zu Sektor des zu verbindenden Raums
					if ((Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMinZ - scanMax) < (int)AbstandAndockHoehleMin)
						bedingungenEingehalten = false;
					// Zu nah am Ende der Scankarte um vollen Bodenabstandstest durchzufuehren?
					if (scanMin - Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMinZ-2*(int)RaumSektorAusdehnung < (int)TiefeAbstandsTestMin)
						abstandZuKurz = true;
					break;
				case 2:	// in X negativ
					// Test auf korrekten Abstand der Andockmoeglichkeit zu Sektor des zu verbindenden Raums
					if ((Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMinX - scanMax) < (int)AbstandAndockHoehleMin)
						bedingungenEingehalten = false;
					// Zu nah am Ende der Scankarte um vollen Bodenabstandstest durchzufuehren?
					if (scanMin - Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMinX-2*(int)RaumSektorAusdehnung < (int)TiefeAbstandsTestMin)
						abstandZuKurz = true;
					break;
				case 3:	// in Z positiv
					// Test auf korrekten Abstand der Andockmoeglichkeit zu Sektor des zu verbindenden Raums
					if ((scanMin - Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMaxZ) < (int)AbstandAndockHoehleMin)
						bedingungenEingehalten = false;
					// Zu nah am Ende der Scankarte um vollen Bodenabstandstest durchzufuehren?
					if (Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMaxZ+2*(int)RaumSektorAusdehnung - scanMax < (int)TiefeAbstandsTestMin)
						abstandZuKurz = true;
					break;
				}
			}

			// Test fuer korrekten Abstand zum Boden
			if (bedingungenEingehalten && (AbstandVomBodenMax>0))
			{
				// ersteinmal: kein gueltiger Abstand gefunden
				bedingungenEingehalten = false;

				// Scan bis zu gewuenschtem Abstand, beginnend vom unteren Gangrand
				for (int j2=resKoord2-1; j2>=resKoord2-(int)AbstandVomBodenMax; j2--)
				{
					temp = false;
					// Scan in Gangbreite
					for (int i2=resKoord1+1; i2<=resKoord1+(int)GangBreite; i2++)
					{
						// Wenn Abstand zum Scanbereichsende kurz, duerfen keine unbekannten Scaneintraege vorliegen
						if (abstandZuKurz && (andockKarte->Hoehenkarte[i2][j2]==INT_MAX)||(andockKarte->Hoehenkarte[i2][j2]==INT_MIN))
							temp = true; // Bedingung in dieser Scanline verletzt

						// Hinweis: positive Richtung bedeutet Gang in negative Richtung von Hoehle aus!
						if (positiveRichtung)
						{
							if ((andockKarte->Hoehenkarte[i2][j2]-scanMax < (int)TiefeAbstandsTestMin) && (andockKarte->Hoehenkarte[i2][j2]!=INT_MAX))
								temp = true; // Bedingung in dieser Scanline verletzt
						}
						else
							if ((scanMin-andockKarte->Hoehenkarte[i2][j2] < (int)TiefeAbstandsTestMin) && (andockKarte->Hoehenkarte[i2][j2]!=INT_MIN))
								temp = true; // Bedingung in dieser Scanline verletzt
					}
					// keine Bedingung verletzt -> Abstand ist eingehalten
					if (!temp)
						bedingungenEingehalten = true;
				}
			}

			// wenn alle notwendigen Bedingungen erfuellt -> Gang erstellen
			if (bedingungenEingehalten)
			{
				// Verbinde Raum mit Hoehle an entsprechenden Koordinaten
				if (VerbindeRaumMitHoehlenAndockposition(_IdRaum, _SektorX, _SektorY, _SektorZ, _Himmelsrichtung, resKoord1, resKoord2, scanMin, scanMax, ausfraesTiefe))
					// Andocken gelungen
					return true;
				// falls moegliche Kollision entdeckt (Rueckgabe: false) -> setze Suche fort
			}
		}

	// keine gueltige Andockmoeglichkeit gefunden
	return false;
}


// Verbindet Raum mit Andockposition
// Andockposition in Hoehenkartenkoordianten uebergeben: Eckpunkt der Gang-Begrenzung mit den kleinsten Koordinaten (links unten bzw. rechts unten)
bool DunGen::CArchitekt::VerbindeRaumMitHoehlenAndockposition(unsigned int _IdRaum, int _SektorX, int _SektorY, int _SektorZ,
			unsigned int _Himmelsrichtung, unsigned int _KartenKoordinate1, unsigned int _KartenKoordinate2,
			int _ScanMin, int _ScanMax, int _Ausfraestiefe)
{
	SRaumSektorAndockKarte* andockKarte  = Raumsektor[_SektorX][_SektorY][_SektorZ].Karte[_Himmelsrichtung];

	// Andockstelle fuer Hoehle, Gangparameter
	SSplineAndockStelle neueAndockstelle;
	irr::core::vector3d<double> position1;
	irr::core::vector3d<double> ableitung1;
	irr::core::vector3d<double> position2 = Raeume.at(_IdRaum)->Andockstelle[_Himmelsrichtung].Mittelpunkt
		+ Raeume.at(_IdRaum)->Andockstelle[_Himmelsrichtung].Normale * (Raeume.at(_IdRaum)->Andockstelle[_Himmelsrichtung].Ausdehnung+GangBreite);
	irr::core::vector3d<double> ableitung2 = Raeume.at(_IdRaum)->Andockstelle[_Himmelsrichtung].Normale * Raeume.at(_IdRaum)->SubSzene->LeseAbleitungsFaktorRaumHoehle()*RaumSektorAusdehnung;

	irr::core::vector3d<irr::f32> tempPunkt;

	// Variablen fuer Grenzen
	int voxelMinX, voxelMinZ, voxelMaxX, voxelMaxZ;
	int resAusfraestiefe = _Ausfraestiefe;
	// Y-Bereich festlegen: Gangbreite hoch (Rand um Gang bleibt unveraendert)
	int voxelMinY = (int)_KartenKoordinate2 - andockKarte->Offset2 + 1;
	int voxelMaxY = voxelMinY + (int)GangBreite - 1;

	// Andockstelle ausfraesen und erstellen
	switch(_Himmelsrichtung)
	{
	case 0:	// X positiv

		// Z-Bereich festlegen: Gangbreite breit (Rand um Gang bleibt unveraendert)
		voxelMinZ = _KartenKoordinate1 - (int)andockKarte->Offset1 + 1;
		voxelMaxZ = voxelMinZ + (int)GangBreite - 1;
		// Tatsaechlich veraenderter X-Bereich ist um 1 nach X negativ verschoben
		voxelMinX = _ScanMin-1;
		voxelMaxX = _ScanMax-1;
		// Ausfraestiefe justieren
		resAusfraestiefe -= ZusaetzlicheAusfraestiefe;
		resAusfraestiefe = std::max((int)CVoxelRaum::VoxelMinRandOffset,resAusfraestiefe);	// gueltigen Voxelbereich nicht ueberschreiten
		voxelMinX = std::min(voxelMinX,resAusfraestiefe-1);

		// Andockstelle erstellen
		neueAndockstelle.Normale = irr::core::vector3d<double>(-1,0,0);
		neueAndockstelle.Mittelpunkt = irr::core::vector3d<double>(resAusfraestiefe,((double)(voxelMinY+voxelMaxY+1))/2.0,((double)(voxelMinZ+voxelMaxZ+1))/2.0);
		neueAndockstelle.Ausdehnung = 0.5;	// auch im worst case: < 0.5 verwackelt

		// Gangparameter ermitteln
		position1 = neueAndockstelle.Mittelpunkt + neueAndockstelle.Normale * GangBreite,	// Andockstelle mit Ausdehnung eines Wuerfels mit Gangbreite Kantenlaenge
		ableitung1 = neueAndockstelle.Normale * Raeume.at(_IdRaum)->SubSzene->LeseAbleitungsFaktorHoehleRaum()*RaumSektorAusdehnung;
		// Test auf potentielle Kollision von Gang und Hoehle
		if (TesteKollisionHoehleGang(position1,position2,ableitung2,andockKarte,_Himmelsrichtung))
			return false;

		// Voxelbereich ausfraesen/markieren: vor(!) Berechnung der Vertex-Koordinaten
		for (int i=voxelMinX; i<=voxelMaxX; i++)
			for (int j=voxelMinY; j<=voxelMaxY; j++)
				for (int k=voxelMinZ; k<=voxelMaxZ; k++)
					if (i<resAusfraestiefe)
						VoxelRaum->Voxel[i][j][k]=10;	// Andockstelle im Voxelraum: Bit 1 + Bit 3
					else if (i<andockKarte->Hoehenkarte[k+andockKarte->Offset1][j+andockKarte->Offset2])
						VoxelRaum->Voxel[i][j][k]=1;	// Bereich freischaufeln/ausfraesen

		// Eck-Punkte hizufuegen: (im UZS in Gangrichtung, also gegen UZS in X positiv)
		for (int i=voxelMaxZ+1; i>=voxelMinZ; i--)		// Z-Achse: von (Ymin,Zmax+1) nach (Ymin,Zmin)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(resAusfraestiefe,voxelMinY,i);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMinY+1; i<=voxelMaxY; i++)		// Y-Achse: von (Ymin+1,Zmin) nach (Ymax,Zmin)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(resAusfraestiefe,i,voxelMinZ);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMinZ; i<=voxelMaxZ+1; i++)		// Z-Achse: von (Ymax+1,Zmin) nach (Ymax+1,Zmax+1)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(resAusfraestiefe,voxelMaxY+1,i);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMaxY; i>=voxelMinY+1; i--)		// Y-Achse: von (Ymax,Zmax+1) nach (Ymin+1,Zmax+1)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(resAusfraestiefe,i,voxelMaxZ+1);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}

		break;

	case 1:	// Z negativ

		// X-Bereich festlegen: Gangbreite breit (Rand um Gang bleibt unveraendert)
		voxelMinX = _KartenKoordinate1 - (int)andockKarte->Offset1 + 1;
		voxelMaxX = voxelMinX + (int)GangBreite - 1;
		// Tatsaechlich veraenderter Z-Bereich ist um 1 nach Z positiv verschoben
		voxelMinZ = _ScanMin+1;
		voxelMaxZ = _ScanMax+1;
		// Ausfraestiefe justieren
		resAusfraestiefe += ZusaetzlicheAusfraestiefe;
		resAusfraestiefe = std::min((int)CVoxelRaum::VoxelraumZ-1-(int)CVoxelRaum::VoxelMinRandOffset,resAusfraestiefe);	// gueltigen Voxelbereich nicht ueberschreiten
		voxelMaxZ = std::max(voxelMaxZ,resAusfraestiefe+1);

		// Andockstelle erstellen
		// resAusfraestiefe+1 , da hoehere Voxelkoordinaten fuer Vertex verwendet werden
		neueAndockstelle.Normale = irr::core::vector3d<double>(0,0,1);
		neueAndockstelle.Mittelpunkt = irr::core::vector3d<double>(((double)(voxelMinX+voxelMaxX+1))/2.0,((double)(voxelMinY+voxelMaxY+1))/2.0,resAusfraestiefe+1);
		neueAndockstelle.Ausdehnung = 0.5;	// auch im worst case: < 0.5 verwackelt

		// Gangparameter ermitteln
		position1 = neueAndockstelle.Mittelpunkt + neueAndockstelle.Normale * GangBreite,	// Andockstelle mit Ausdehnung eines Wuerfels mit Gangbreite Kantenlaenge
		ableitung1 = neueAndockstelle.Normale * Raeume.at(_IdRaum)->SubSzene->LeseAbleitungsFaktorHoehleRaum()*RaumSektorAusdehnung;
		// Test auf potentielle Kollision von Gang und Hoehle
		if (TesteKollisionHoehleGang(position1,position2,ableitung2,andockKarte,_Himmelsrichtung))
			return false;

		// Voxelbereich ausfraesen/markieren: vor(!) Berechnung der Vertex-Koordinaten
		for (int i=voxelMinX; i<=voxelMaxX; i++)
			for (int j=voxelMinY; j<=voxelMaxY; j++)
				for (int k=voxelMinZ; k<=voxelMaxZ; k++)
					if (k>resAusfraestiefe)
						VoxelRaum->Voxel[i][j][k]=10;	// Andockstelle im Voxelraum: Bit 1 + Bit 3
					else if (k>andockKarte->Hoehenkarte[i+andockKarte->Offset1][j+andockKarte->Offset2])
						VoxelRaum->Voxel[i][j][k]=1;	// Bereich freischaufeln/ausfraesen

		// Eck-Punkte hizufuegen: (im UZS in Gangrichtung, also gegen UZS in Z negativ)
		resAusfraestiefe++;	// resAusfraestiefe+1 wird verwendet
		for (int i=voxelMaxX+1; i>=voxelMinX; i--)		// X-Achse: von (Ymin,Xmax+1) nach (Ymin,Xmin)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(i,voxelMinY,resAusfraestiefe);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMinY+1; i<=voxelMaxY; i++)		// Y-Achse: von (Ymin+1,Xmin) nach (Ymax,Xmin)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(voxelMinX,i,resAusfraestiefe);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMinX; i<=voxelMaxX+1; i++)		// Z-Achse: von (Ymax+1,Xmin) nach (Ymax+1,Xmax+1)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(i,voxelMaxY+1,resAusfraestiefe);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMaxY; i>=voxelMinY+1; i--)		// X-Achse: von (Ymax,Xmax+1) nach (Ymin+1,Xmax+1)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(voxelMaxX+1,i,resAusfraestiefe);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}

		break;

	case 2:	// X negativ

		// Z-Bereich festlegen: Gangbreite breit (Rand um Gang bleibt unveraendert)
		voxelMinZ = _KartenKoordinate1 - (int)andockKarte->Offset1 + 1;
		voxelMaxZ = voxelMinZ + (int)GangBreite - 1;
		// Tatsaechlich veraenderter X-Bereich ist um 1 nach X positiv verschoben
		voxelMinX = _ScanMin+1;
		voxelMaxX = _ScanMax+1;
		// Ausfraestiefe justieren
		resAusfraestiefe += ZusaetzlicheAusfraestiefe;
		resAusfraestiefe = std::min((int)CVoxelRaum::VoxelraumX-1-(int)CVoxelRaum::VoxelMinRandOffset,resAusfraestiefe);	// gueltigen Voxelbereich nicht ueberschreiten
		voxelMaxX = std::max(voxelMaxX,resAusfraestiefe+1);

		// Andockstelle erstellen
		// resAusfraestiefe+1 , da hoehere Voxelkoordinaten fuer Vertex verwendet werden
		neueAndockstelle.Normale = irr::core::vector3d<double>(1,0,0);
		neueAndockstelle.Mittelpunkt = irr::core::vector3d<double>(resAusfraestiefe+1,((double)(voxelMinY+voxelMaxY+1))/2.0,((double)(voxelMinZ+voxelMaxZ+1))/2.0);
		neueAndockstelle.Ausdehnung = 0.5;	// auch im worst case: < 0.5 verwackelt

		// Gangparameter ermitteln
		position1 = neueAndockstelle.Mittelpunkt + neueAndockstelle.Normale * GangBreite,	// Andockstelle mit Ausdehnung eines Wuerfels mit Gangbreite Kantenlaenge
		ableitung1 = neueAndockstelle.Normale * Raeume.at(_IdRaum)->SubSzene->LeseAbleitungsFaktorHoehleRaum()*RaumSektorAusdehnung;
		// Test auf potentielle Kollision von Gang und Hoehle
		if (TesteKollisionHoehleGang(position1,position2,ableitung2,andockKarte,_Himmelsrichtung))
			return false;

		// Voxelbereich ausfraesen/markieren: vor(!) Berechnung der Vertex-Koordinaten
		for (int i=voxelMinX; i<=voxelMaxX; i++)
			for (int j=voxelMinY; j<=voxelMaxY; j++)
				for (int k=voxelMinZ; k<=voxelMaxZ; k++)
					if (i>resAusfraestiefe)
						VoxelRaum->Voxel[i][j][k]=10;	// Andockstelle im Voxelraum: Bit 1 + Bit 3
					else if (i>andockKarte->Hoehenkarte[k+andockKarte->Offset1][j+andockKarte->Offset2])
						VoxelRaum->Voxel[i][j][k]=1;	// Bereich freischaufeln/ausfraesen

		// Eck-Punkte hizufuegen: (im UZS in Gangrichtung, also gegen UZS in X negativ)
		resAusfraestiefe++;	// resAusfraestiefe+1 wird verwendet
		for (int i=voxelMaxZ+1; i>=voxelMinZ; i--)		// Z-Achse: von (Ymax+1,Zmax+1) nach (Ymax+1,Zmin)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(resAusfraestiefe,voxelMaxY+1,i);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMaxY; i>=voxelMinY+1; i--)		// Y-Achse: von (Ymax,Zmin) nach (Ymin+1,Zmin)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(resAusfraestiefe,i,voxelMinZ);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMinZ; i<=voxelMaxZ+1; i++)		// Z-Achse: von (Ymin,Zmin) nach (Ymin,Zmax+1)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(resAusfraestiefe,voxelMinY,i);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMinY+1; i<=voxelMaxY; i++)		// Y-Achse: von (Ymin+1,Zmax+1) nach (Ymax,Zmax+1)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(resAusfraestiefe,i,voxelMaxZ+1);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}

		break;

	case 3:	// Z positiv

		// X-Bereich festlegen: Gangbreite breit (Rand um Gang bleibt unveraendert)
		voxelMinX = _KartenKoordinate1 - (int)andockKarte->Offset1 + 1;
		voxelMaxX = voxelMinX + (int)GangBreite - 1;
		// Tatsaechlich veraenderter Z-Bereich ist um 1 nach Z negativ verschoben
		voxelMinZ = _ScanMin-1;
		voxelMaxZ = _ScanMax-1;
		// Ausfraestiefe justieren
		resAusfraestiefe -= ZusaetzlicheAusfraestiefe;
		resAusfraestiefe = std::max((int)CVoxelRaum::VoxelMinRandOffset,resAusfraestiefe);	// gueltigen Voxelbereich nicht ueberschreiten
		voxelMinZ = std::min(voxelMinZ,resAusfraestiefe-1);

		// Andockstelle erstellen
		neueAndockstelle.Normale = irr::core::vector3d<double>(0,0,-1);
		neueAndockstelle.Mittelpunkt = irr::core::vector3d<double>(((double)(voxelMinX+voxelMaxX+1))/2.0,((double)(voxelMinY+voxelMaxY+1))/2.0,resAusfraestiefe);
		neueAndockstelle.Ausdehnung = 0.5;	// auch im worst case: < 0.5 verwackelt

		// Gangparameter ermitteln
		position1 = neueAndockstelle.Mittelpunkt + neueAndockstelle.Normale * GangBreite,	// Andockstelle mit Ausdehnung eines Wuerfels mit Gangbreite Kantenlaenge
		ableitung1 = neueAndockstelle.Normale * Raeume.at(_IdRaum)->SubSzene->LeseAbleitungsFaktorHoehleRaum()*RaumSektorAusdehnung;
		// Test auf potentielle Kollision von Gang und Hoehle
		if (TesteKollisionHoehleGang(position1,position2,ableitung2,andockKarte,_Himmelsrichtung))
			return false;

		// Voxelbereich ausfraesen/markieren: vor(!) Berechnung der Vertex-Koordinaten
		for (int i=voxelMinX; i<=voxelMaxX; i++)
			for (int j=voxelMinY; j<=voxelMaxY; j++)
				for (int k=voxelMinZ; k<=voxelMaxZ; k++)
					if (k<resAusfraestiefe)
						VoxelRaum->Voxel[i][j][k]=10;	// Andockstelle im Voxelraum: Bit 1 + Bit 3
					else if (k<andockKarte->Hoehenkarte[i+andockKarte->Offset1][j+andockKarte->Offset2])
						VoxelRaum->Voxel[i][j][k]=1;	// Bereich freischaufeln/ausfraesen

		// Eck-Punkte hizufuegen: (im UZS in Gangrichtung, also gegen UZS in Z positiv)
		for (int i=voxelMaxX+1; i>=voxelMinX; i--)		// X-Achse: von (Ymax+1,Xmax+1) nach (Ymax+1,Xmin)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(i,voxelMaxY+1,resAusfraestiefe);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMaxY; i>=voxelMinY+1; i--)		// Y-Achse: von (Ymax,Xmin) nach (Ymin+1,Xmin)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(voxelMinX,i,resAusfraestiefe);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMinX; i<=voxelMaxX+1; i++)		// X-Achse: von (Ymin,Xmin) nach (Ymin,Xmax+1)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(i,voxelMinY,resAusfraestiefe);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}
		for (int i=voxelMinY+1; i<=voxelMaxY; i++)		// Y-Achse: von (Ymin+1,Xmax+1) nach (Ymax,Xmax+1)
		{
			tempPunkt = DreiecksMesh->BerechneVertexKoordinaten(voxelMaxX+1,i,resAusfraestiefe);
			neueAndockstelle.Punkt.push_back(irr::core::vector3d<double>(tempPunkt.X,tempPunkt.Y,tempPunkt.Z));
		}

		break;
	}

	Zeitmesser->Fortsetzen(1);
	// neuen Gang erstellen
	CSpline* gang = new CSpline(
	// Querschnitt des Gangs
	QuerschnittGang,
	// Andockstellen
	neueAndockstelle, Raeume.at(_IdRaum)->Andockstelle[_Himmelsrichtung],
	// Positionen und Ableitungen der Splineenden
	position1, position2, ableitung1, ableitung2,
	// Abstand zwischen den Splinesegmenten
	AbstandSplineSegmente,
	// Wieviel TexturY-Koordinate pro Abstand 1 entlang des Gangs
	AbstandTexturYProAbstand1,
	// Anzahl der zusaetzlichen Detailstufen
	AnzahlSplineDetailstufen);
	Zeitmesser->Pause(1);

	Zeitmesser->Fortsetzen(2);
	// Invarianz der Dungeonskonstruktion gegenueber Detailobjekt-Platzierungen gewaehrleisten
	// durch Beibehaltung des aktuellern Zufallsseeds
	unsigned int seed = ZufallsGenerator->GeneriereZufallsWertBasis();
	// Detailobjekte platzieren
	SSplineDetailobjektVorlage* detailObjektVorlage;
	for (unsigned int i=0; i<DetailobjektVorlagen.size(); i++)
	{
		detailObjektVorlage = DetailobjektVorlagen.at(i);
		gang->PlatziereDetailobjekt(detailObjektVorlage->Name, detailObjektVorlage->ObjektMesh, detailObjektVorlage->Skalierung, detailObjektVorlage->Rotation,
			((double)GangBreite/2.0)*detailObjektVorlage->Position, detailObjektVorlage->AbstandsAbtastung, detailObjektVorlage->AbstandNumFaktor,
			detailObjektVorlage->AbstandNumMin, detailObjektVorlage->AbstandNumMax,
			detailObjektVorlage->AbstandNumMinErstesElement, detailObjektVorlage->AbstandNumMaxErstesElement, detailObjektVorlage->ObjektAn1,
			ZufallsGenerator);
	}
	ZufallsGenerator->SetzeZufallsSeed(seed);
	Zeitmesser->Pause(2);

	// Gang-ID's setzen (Andockstelle 0: -1 fuer Hoehle)
	gang->SetzeAndocksstellenIds(-1,_IdRaum);
	// Raumverbindungsstelle: GangID merken
	Raeume.at(_IdRaum)->GangID[_Himmelsrichtung] = Gaenge.size();
	// Gang merken
	Gaenge.push_back(gang);

	// Revalidieren mit Konfliktgebiet = BoundingBox (Gang, Sektor) + 1 Voxel Zusatzabstand, damit mind. 2 Voxel abstand zwischen 2 Gangandockstellen
	RevalidiereAndockKarten(_SektorX,_SektorY,_SektorZ,
		std::min(voxelMinX, Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMinX)-1, std::max(voxelMaxX, Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMaxX)+1,
		std::min(voxelMinY, Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMinY)-1, std::max(voxelMaxY, Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMaxY)+1,
		std::min(voxelMinZ, Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMinZ)-1, std::max(voxelMaxZ, Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMaxZ)+1);
	//*/

	return true;
}

//=======================================
// Revalidierung von Andockinformationen
//=======================================

// Revalidiere alle Andockkarten bezueglich Konfliktgebiet
void DunGen::CArchitekt::RevalidiereAndockKarten(int _SektorX, int _SektorY, int _SektorZ,
	int _KonfliktgebietMinX, int _KonfliktgebietMaxX, int _KonfliktgebietMinY, int _KonfliktgebietMaxY,	int _KonfliktgebietMinZ, int _KonfliktgebietMaxZ)
{
	// Sektorgebiet absuchen
	for (int i=_SektorX-3; i<=_SektorX+3; i++)	// 2 Sektoren tief in X von jedem moeglichen Andockpunkt
		for (int j=_SektorY-1; j<=_SektorY+1; j++)
			for (int k=_SektorZ-3; k<=_SektorZ+3; k++) // 2 Sektoren tief in Z von jedem moeglichen Andockpunkt
				// gueltiger Sektor?
				if ((i>=0)&&(i<RaumSektorAnzahlX)&&(j>=0)&&(j<RaumSektorAnzahlY)&&(k>=0)&&(k<RaumSektorAnzahlZ))
					// Sektor ist nicht mit sich selbst im Konflikt!
					if ((i!=_SektorX)||(j!=_SektorY)||(k!=_SektorZ))
					{
						// Revalidiere Karten mit moeglichen Konflikten
						RevalidiereAndockKarte(i,j,k,0,_KonfliktgebietMinX, _KonfliktgebietMaxX, _KonfliktgebietMinY, _KonfliktgebietMaxY,	_KonfliktgebietMinZ, _KonfliktgebietMaxZ);
						RevalidiereAndockKarte(i,j,k,1,_KonfliktgebietMinX, _KonfliktgebietMaxX, _KonfliktgebietMinY, _KonfliktgebietMaxY,	_KonfliktgebietMinZ, _KonfliktgebietMaxZ);
						RevalidiereAndockKarte(i,j,k,2,_KonfliktgebietMinX, _KonfliktgebietMaxX, _KonfliktgebietMinY, _KonfliktgebietMaxY,	_KonfliktgebietMinZ, _KonfliktgebietMaxZ);
						RevalidiereAndockKarte(i,j,k,3,_KonfliktgebietMinX, _KonfliktgebietMaxX, _KonfliktgebietMinY, _KonfliktgebietMaxY,	_KonfliktgebietMinZ, _KonfliktgebietMaxZ);
					}
}

// Revalidiere eine Andockkarte bezueglich Konfliktgebiet
void DunGen::CArchitekt::RevalidiereAndockKarte(int _SektorX, int _SektorY, int _SektorZ, unsigned int _Himmelsrichtung,
	int _KonfliktgebietMinX, int _KonfliktgebietMaxX, int _KonfliktgebietMinY, int _KonfliktgebietMaxY,	int _KonfliktgebietMinZ, int _KonfliktgebietMaxZ)
{
	SRaumSektorAndockKarte* andockKarte = Raumsektor[_SektorX][_SektorY][_SektorZ].Karte[_Himmelsrichtung];
	if (andockKarte==NULL)
		return;

	// Hoehenkartenkoordinaten
	int karteMinKoord1, karteMaxKoord1;
	int karteMinKoord2, karteMaxKoord2;

	// Y-Bereich festlegen
	karteMinKoord2 = _KonfliktgebietMinY + andockKarte->Offset2;
	karteMaxKoord2 = _KonfliktgebietMaxY + andockKarte->Offset2;
	karteMinKoord2 = std::max(karteMinKoord2,0);
	karteMaxKoord2 = std::min(karteMaxKoord2,(int)andockKarte->Ausdehnung2-1);

	switch(_Himmelsrichtung)
	{
	case 0:	// X positiv

		// moeglicher Konflikt?
		if (!(_KonfliktgebietMaxX >= Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMinX)) return;

		karteMinKoord1 = _KonfliktgebietMinZ + andockKarte->Offset1;
		karteMaxKoord1 = _KonfliktgebietMaxZ + andockKarte->Offset1;
		karteMinKoord1 = std::max(karteMinKoord1,0);
		karteMaxKoord1 = std::min(karteMaxKoord1,(int)andockKarte->Ausdehnung1-1);

		for (int i=karteMinKoord1; i<=karteMaxKoord1; i++)
			for (int j=karteMinKoord2; j<=karteMaxKoord2; j++)
				// Verletzung des Gebietes?
				if (andockKarte->Hoehenkarte[i][j]>=_KonfliktgebietMinX)
					andockKarte->ValideHoeheninformation[i][j] = 0;
		break;

	case 1:	// Z negativ

		// moeglicher Konflikt?
		if (!(_KonfliktgebietMinZ <= Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMaxZ)) return;

		karteMinKoord1 = _KonfliktgebietMinX + andockKarte->Offset1;
		karteMaxKoord1 = _KonfliktgebietMaxX + andockKarte->Offset1;
		karteMinKoord1 = std::max(karteMinKoord1,0);
		karteMaxKoord1 = std::min(karteMaxKoord1,(int)andockKarte->Ausdehnung1-1);

		for (int i=karteMinKoord1; i<=karteMaxKoord1; i++)
			for (int j=karteMinKoord2; j<=karteMaxKoord2; j++)
				// Verletzung des Gebietes?
				if (andockKarte->Hoehenkarte[i][j]<=_KonfliktgebietMaxZ)
					andockKarte->ValideHoeheninformation[i][j] = 0;
		break;

	case 2:	// X negativ

		// moeglicher Konflikt?
		if (!(_KonfliktgebietMinX <= Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMaxX)) return;

		karteMinKoord1 = _KonfliktgebietMinZ + andockKarte->Offset1;
		karteMaxKoord1 = _KonfliktgebietMaxZ + andockKarte->Offset1;
		karteMinKoord1 = std::max(karteMinKoord1,0);
		karteMaxKoord1 = std::min(karteMaxKoord1,(int)andockKarte->Ausdehnung1-1);

		for (int i=karteMinKoord1; i<=karteMaxKoord1; i++)
			for (int j=karteMinKoord2; j<=karteMaxKoord2; j++)
				// Verletzung des Gebietes?
				if (andockKarte->Hoehenkarte[i][j]<=_KonfliktgebietMaxX)
					andockKarte->ValideHoeheninformation[i][j] = 0;
		break;

	case 3:	// Z positiv

		// moeglicher Konflikt?
		if (!(_KonfliktgebietMaxZ >= Raumsektor[_SektorX][_SektorY][_SektorZ].GrenzeMinZ)) return;

		karteMinKoord1 = _KonfliktgebietMinX + andockKarte->Offset1;
		karteMaxKoord1 = _KonfliktgebietMaxX + andockKarte->Offset1;
		karteMinKoord1 = std::max(karteMinKoord1,0);
		karteMaxKoord1 = std::min(karteMaxKoord1,(int)andockKarte->Ausdehnung1-1);

		for (int i=karteMinKoord1; i<=karteMaxKoord1; i++)
			for (int j=karteMinKoord2; j<=karteMaxKoord2; j++)
				// Verletzung des Gebietes?
				if (andockKarte->Hoehenkarte[i][j]>=_KonfliktgebietMinZ)
					andockKarte->ValideHoeheninformation[i][j] = 0;
		break;
	}

}

//=======================================
// Kollisionstest Hoehle <-> Gang
//=======================================

// Testet auf eine potentielle Kollision von Hoehle und Gang: true .. Kollision moeglich, false ... keine Kollision moeglich
// getestet werden alle Punkte auf gegebener Hoehenkarte (ohne Bereich fuer Bodenabstandstest)
bool DunGen::CArchitekt::TesteKollisionHoehleGang(irr::core::vector3d<double> _Position1, irr::core::vector3d<double> _Position2, irr::core::vector3d<double> _Ableitung2,
			SRaumSektorAndockKarte* _HoehenKarte, unsigned int _Himmelsrichtung)
{
	// Eckpunkte des Bounding-Tetraeders des Splineskeletts des Gangs: (Hermite-Spline als kubische Bezierkurve mit diesen 4 Punkten darstellbar)
	// Position1
	// Position1 + Ableitung1/3 ... braucht nicht betrachtet zu werden, da Ableitung vertikal zur Hoehenkarte (siehe Konstruktion)
	// Position2 - Ableitung2/3 ... bzw. Position2 + Ableitung2/3, da Ableitungen in Gangrichtung angegeben
	// Position2
	irr::core::vector3d<double> position3 = _Position2 + _Ableitung2/3.0;
	// Richtungsvektoren von Position1 zu anderen Eckpunkten
	irr::core::vector3d<double> richtung1 = _Position2 - _Position1;
	irr::core::vector3d<double> richtung2 = position3 - _Position1;

	// Sicherheitsabstand, da Koordinaten fuer Gangskelett angebeben:
	// Gang-Querschnitt hat maximalen Radius von Sqrt(2)*(Gangbreite/2) = Sqrt(0.5)*Gangbreite
	// 1 Voxel zusatzlich wg. Verwackeln
	double sicherheitsAbstand = sqrt(0.5) * ((double)GangBreite) * ZusaetzlicherSicherheitsabstand + 1.0;

	// Boundingbox
	double minK, maxK;	// K ... X oder Z
	double minY = std::min(std::min(_Position1.Y,_Position2.Y),position3.Y) - sicherheitsAbstand;
	double maxY = std::max(std::max(_Position1.Y,_Position2.Y),position3.Y) + sicherheitsAbstand;

	// Boundingbox des Gangs auf der Hoehenkarte -> zu testender Bereich
	int karteMin1, karteMax1;
	int karteMin2 = minY + _HoehenKarte->Offset2;
	int karteMax2 = maxY + _HoehenKarte->Offset2 + 1;			// +1 ... aufrunden wegen double
	karteMin2 = std::max(0,karteMin2);
	karteMax2 = std::min((int)_HoehenKarte->Ausdehnung2-1,karteMax2);

	// Anstiege in die 4 Kartenrichtungen, fuer "Kegel" mit rechteckigem Querschnitt
	double anstiegKoord1neg, anstiegKoord1pos, anstiegKoord2neg, anstiegKoord2pos;

	// Offsets fuer Umwandlung: Kartenkoordinaten -> delta
	int offsetY = - _HoehenKarte->Offset2 - _Position1.Y;
	int offsetK;

	// Delta-Werte zur Zentralposition (Splineursprung/Position1)
	double deltaK, deltaY;
	// erlaubte Hoehenwerte
	double erlaubteHoehe, erlaubteHoeheK, erlaubteHoeheY;

	switch (_Himmelsrichtung)
	{
		case 0: // Richtung X positiv -> Gang verlaeuft nach X negativ

			// Sonderfaelle herausfiltern: falsche Richtung der Richtungsvektoren
			if ((richtung1.X>0.0)||(richtung2.X>0.0))
				return true;

			minK = std::min(std::min(_Position1.Z,_Position2.Z),position3.Z) - sicherheitsAbstand;
			maxK = std::max(std::max(_Position1.Z,_Position2.Z),position3.Z) + sicherheitsAbstand;
			karteMin1 = minK + _HoehenKarte->Offset1 ;
			karteMax1 = maxK + _HoehenKarte->Offset1 + 1;
			karteMin1 = std::max(0,karteMin1);
			karteMax1 = std::min((int)_HoehenKarte->Ausdehnung1-1,karteMax1);

			// Anstieg in Z-Richtung positiv
			anstiegKoord1pos = FLT_MIN;
			if (richtung1.Z>0.0)
				anstiegKoord1pos = richtung1.X/richtung1.Z;
			if (richtung2.Z>0.0)
				anstiegKoord1pos = std::max(richtung2.X/richtung2.Z,anstiegKoord1pos);	// flachster Anstieg: Maximum, da Anstieg negativ
			if (anstiegKoord1pos == FLT_MIN)
				anstiegKoord1pos = -10.0*(double)CVoxelRaum::VoxelraumX; // -"unendlich"

			// Anstieg in Z-Richtung negativ
			anstiegKoord1neg = FLT_MAX;
			if (richtung1.Z<0.0)
				anstiegKoord1neg = richtung1.X/richtung1.Z;
			if (richtung2.Z<0.0)
				anstiegKoord1neg = std::min(richtung2.X/richtung2.Z,anstiegKoord1neg);	// flachster Anstieg: Minimum, da Anstieg positiv
			if (anstiegKoord1neg == FLT_MAX)
				anstiegKoord1neg = 10.0*(double)CVoxelRaum::VoxelraumX; // "unendlich"

			// Anstieg in Y-Richtung positiv
			anstiegKoord2pos = FLT_MIN;
			if (richtung1.Y>0.0)
				anstiegKoord2pos = richtung1.X/richtung1.Y;
			if (richtung2.Y>0.0)
				anstiegKoord2pos = std::max(richtung2.X/richtung2.Y,anstiegKoord2pos);
			if (anstiegKoord2pos == FLT_MIN)
				anstiegKoord2pos = -10.0*(double)CVoxelRaum::VoxelraumX; // -"unendlich"

			// Anstieg in Y-Richtung negativ
			anstiegKoord2neg = FLT_MAX;
			if (richtung1.Y<0.0)
				anstiegKoord2neg = richtung1.X/richtung1.Y;
			if (richtung2.Y<0.0)
				anstiegKoord2neg = std::min(richtung2.X/richtung2.Y,anstiegKoord2neg);
			if (anstiegKoord2neg == FLT_MAX)
				anstiegKoord2neg = 10.0*(double)CVoxelRaum::VoxelraumX; // "unendlich"

			// Offset Z fuer Umwandlung: Kartenkoordinaten -> delta
			offsetK = - _HoehenKarte->Offset1 - _Position1.Z;

			// Testen aller moeglichen Konflikte
			for (int i=karteMin1; i<=karteMax1; i++)
				for (int j=karteMin2; j<=karteMax2; j++)
					if (_HoehenKarte->Hoehenkarte[i][j]!=INT_MAX) // endlicher Wert?
					{
						// delta-Werte ermitteln
						deltaK = i + offsetK;
						deltaY = j + offsetY;

						// Z-Richtung ueberpruefen
						if (deltaK<0.0)
						{
							deltaK += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheK = anstiegKoord1neg*deltaK;
						}
						else
						{
							deltaK -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheK = anstiegKoord1pos*deltaK;
						}

						// Y-Richtung ueberpruefen
						if (deltaY<0.0)
						{
							deltaY += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheY = anstiegKoord2neg*deltaY;
						}
						else
						{
							deltaY -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheY = anstiegKoord2pos*deltaY;
						}

						// clipping
						erlaubteHoehe = std::min(erlaubteHoeheK,erlaubteHoeheY);
						erlaubteHoehe = std::min(0.0,erlaubteHoehe);

						erlaubteHoehe += _Position1.X;			// Bezugspunkt beachten
						erlaubteHoehe += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen (da Koordinaten fuer Gangskelett)

						// Verletzung der Hoehe durch Voxel?
						if (_HoehenKarte->Hoehenkarte[i][j]<erlaubteHoehe)
						{
							//std::cout << "!!!KOLLISIONSMOEGLICHKEIT bei Richtung " << _Himmelsrichtung << std::endl;
							return true;
						}

					}
		break; // ENDE: case 0: Richtung X positiv

		case 1: // Richtung Z negativ -> Gang verlaeuft nach Z positiv

			// Sonderfaelle herausfiltern: falsche Richtung der Richtungsvektoren
			if ((richtung1.Z<0.0)||(richtung2.Z<0.0))
				return true;

			minK = std::min(std::min(_Position1.X,_Position2.X),position3.X) - sicherheitsAbstand;
			maxK = std::max(std::max(_Position1.X,_Position2.X),position3.X) + sicherheitsAbstand;
			karteMin1 = minK + _HoehenKarte->Offset1 ;
			karteMax1 = maxK + _HoehenKarte->Offset1 + 1;
			karteMin1 = std::max(0,karteMin1);
			karteMax1 = std::min((int)_HoehenKarte->Ausdehnung1-1,karteMax1);

			// Anstieg in X-Richtung positiv
			anstiegKoord1pos = FLT_MAX;
			if (richtung1.X>0.0)
				anstiegKoord1pos = richtung1.Z/richtung1.X;
			if (richtung2.X>0.0)
				anstiegKoord1pos = std::min(richtung2.Z/richtung2.X,anstiegKoord1pos);	// flachster Anstieg: Minimum, da Anstieg positiv
			if (anstiegKoord1pos == FLT_MAX)
				anstiegKoord1pos = 10.0*(double)CVoxelRaum::VoxelraumZ; // "unendlich"

			// Anstieg in X-Richtung negativ
			anstiegKoord1neg = FLT_MIN;
			if (richtung1.X<0.0)
				anstiegKoord1neg = richtung1.Z/richtung1.X;
			if (richtung2.X<0.0)
				anstiegKoord1neg = std::max(richtung2.Z/richtung2.X,anstiegKoord1neg);	// flachster Anstieg: Maximum, da Anstieg negativ
			if (anstiegKoord1neg == FLT_MIN)
				anstiegKoord1neg = -10.0*(double)CVoxelRaum::VoxelraumZ; // -"unendlich"

			// Anstieg in Y-Richtung positiv
			anstiegKoord2pos = FLT_MAX;
			if (richtung1.Y>0.0)
				anstiegKoord2pos = richtung1.Z/richtung1.Y;
			if (richtung2.Y>0.0)
				anstiegKoord2pos = std::min(richtung2.Z/richtung2.Y,anstiegKoord2pos);
			if (anstiegKoord2pos == FLT_MAX)
				anstiegKoord2pos = 10.0*(double)CVoxelRaum::VoxelraumZ; // "unendlich"

			// Anstieg in Y-Richtung negativ
			anstiegKoord2neg = FLT_MIN;
			if (richtung1.Y<0.0)
				anstiegKoord2neg = richtung1.Z/richtung1.Y;
			if (richtung2.Y<0.0)
				anstiegKoord2neg = std::max(richtung2.Z/richtung2.Y,anstiegKoord2neg);
			if (anstiegKoord2neg == FLT_MIN)
				anstiegKoord2neg = -10.0*(double)CVoxelRaum::VoxelraumZ; // -"unendlich"

			// Offset Z fuer Umwandlung: Kartenkoordinaten -> delta
			offsetK = - _HoehenKarte->Offset1 - _Position1.X;

			// Testen aller moeglichen Konflikte
			for (int i=karteMin1; i<=karteMax1; i++)
				for (int j=karteMin2; j<=karteMax2; j++)
					if (_HoehenKarte->Hoehenkarte[i][j]!=INT_MIN) // endlicher Wert?
					{
						// delta-Werte ermitteln
						deltaK = i + offsetK;
						deltaY = j + offsetY;

						// Z-Richtung ueberpruefen
						if (deltaK<0.0)
						{
							deltaK += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheK = anstiegKoord1neg*deltaK;
						}
						else
						{
							deltaK -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheK = anstiegKoord1pos*deltaK;
						}

						// Y-Richtung ueberpruefen
						if (deltaY<0.0)
						{
							deltaY += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheY = anstiegKoord2neg*deltaY;
						}
						else
						{
							deltaY -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheY = anstiegKoord2pos*deltaY;
						}

						// clipping
						erlaubteHoehe = std::max(erlaubteHoeheK,erlaubteHoeheY);
						erlaubteHoehe = std::max(0.0,erlaubteHoehe);

						erlaubteHoehe += _Position1.Z;			// Bezugspunkt beachten
						erlaubteHoehe -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen (da Koordinaten fuer Gangskelett)

						// Verletzung der Hoehe durch Voxel?
						if (_HoehenKarte->Hoehenkarte[i][j]>erlaubteHoehe)
						{
							//std::cout << "!!!KOLLISIONSMOEGLICHKEIT bei Richtung " << _Himmelsrichtung << std::endl;
							return true;
						}

					}
		break; // ENDE: case 1: Richtung Z negativ

		case 2: // Richtung X negativ -> Gang verlaeuft nach X positiv

			// Sonderfaelle herausfiltern: falsche Richtung der Richtungsvektoren
			if ((richtung1.X<0.0)||(richtung2.X<0.0))
				return true;

			minK = std::min(std::min(_Position1.Z,_Position2.Z),position3.Z) - sicherheitsAbstand;
			maxK = std::max(std::max(_Position1.Z,_Position2.Z),position3.Z) + sicherheitsAbstand;
			karteMin1 = minK + _HoehenKarte->Offset1 ;
			karteMax1 = maxK + _HoehenKarte->Offset1 + 1;
			karteMin1 = std::max(0,karteMin1);
			karteMax1 = std::min((int)_HoehenKarte->Ausdehnung1-1,karteMax1);

			// Anstieg in Z-Richtung positiv
			anstiegKoord1pos = FLT_MAX;
			if (richtung1.Z>0.0)
				anstiegKoord1pos = richtung1.X/richtung1.Z;
			if (richtung2.Z>0.0)
				anstiegKoord1pos = std::min(richtung2.X/richtung2.Z,anstiegKoord1pos);	// flachster Anstieg: Minimum, da Anstieg positiv
			if (anstiegKoord1pos == FLT_MAX)
				anstiegKoord1pos = 10.0*(double)CVoxelRaum::VoxelraumX; // "unendlich"

			// Anstieg in Z-Richtung negativ
			anstiegKoord1neg = FLT_MIN;
			if (richtung1.Z<0.0)
				anstiegKoord1neg = richtung1.X/richtung1.Z;
			if (richtung2.Z<0.0)
				anstiegKoord1neg = std::max(richtung2.X/richtung2.Z,anstiegKoord1neg);	// flachster Anstieg: Maximum, da Anstieg negativ
			if (anstiegKoord1neg == FLT_MIN)
				anstiegKoord1neg = -10.0*(double)CVoxelRaum::VoxelraumX; // -"unendlich"

			// Anstieg in Y-Richtung positiv
			anstiegKoord2pos = FLT_MAX;
			if (richtung1.Y>0.0)
				anstiegKoord2pos = richtung1.X/richtung1.Y;
			if (richtung2.Y>0.0)
				anstiegKoord2pos = std::min(richtung2.X/richtung2.Y,anstiegKoord2pos);
			if (anstiegKoord2pos == FLT_MAX)
				anstiegKoord2pos = 10.0*(double)CVoxelRaum::VoxelraumX; // "unendlich"

			// Anstieg in Y-Richtung negativ
			anstiegKoord2neg = FLT_MIN;
			if (richtung1.Y<0.0)
				anstiegKoord2neg = richtung1.X/richtung1.Y;
			if (richtung2.Y<0.0)
				anstiegKoord2neg = std::max(richtung2.X/richtung2.Y,anstiegKoord2neg);
			if (anstiegKoord2neg == FLT_MIN)
				anstiegKoord2neg = -10.0*(double)CVoxelRaum::VoxelraumX; // -"unendlich"

			// Offset Z fuer Umwandlung: Kartenkoordinaten -> delta
			offsetK = - _HoehenKarte->Offset1 - _Position1.Z;

			// Testen aller moeglichen Konflikte
			for (int i=karteMin1; i<=karteMax1; i++)
				for (int j=karteMin2; j<=karteMax2; j++)
					if (_HoehenKarte->Hoehenkarte[i][j]!=INT_MIN) // endlicher Wert?
					{
						// delta-Werte ermitteln
						deltaK = i + offsetK;
						deltaY = j + offsetY;

						// Z-Richtung ueberpruefen
						if (deltaK<0.0)
						{
							deltaK += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheK = anstiegKoord1neg*deltaK;
						}
						else
						{
							deltaK -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheK = anstiegKoord1pos*deltaK;
						}

						// Y-Richtung ueberpruefen
						if (deltaY<0.0)
						{
							deltaY += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheY = anstiegKoord2neg*deltaY;
						}
						else
						{
							deltaY -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheY = anstiegKoord2pos*deltaY;
						}

						// clipping
						erlaubteHoehe = std::max(erlaubteHoeheK,erlaubteHoeheY);
						erlaubteHoehe = std::max(0.0,erlaubteHoehe);

						erlaubteHoehe += _Position1.X;			// Bezugspunkt beachten
						erlaubteHoehe -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen (da Koordinaten fuer Gangskelett)

						// Verletzung der Hoehe durch Voxel?
						if (_HoehenKarte->Hoehenkarte[i][j]>erlaubteHoehe)
						{
							//std::cout << "!!!KOLLISIONSMOEGLICHKEIT bei Richtung " << _Himmelsrichtung << std::endl;
							return true;
						}

					}
		break; // ENDE: case 2: Richtung X negativ

		case 3: // Richtung Z positiv -> Gang verlaeuft nach Z negativ

			// Sonderfaelle herausfiltern: falsche Richtung der Richtungsvektoren
			if ((richtung1.Z>0.0)||(richtung2.Z>0.0))
				return true;

			minK = std::min(std::min(_Position1.X,_Position2.X),position3.X) - sicherheitsAbstand;
			maxK = std::max(std::max(_Position1.X,_Position2.X),position3.X) + sicherheitsAbstand;
			karteMin1 = minK + _HoehenKarte->Offset1 ;
			karteMax1 = maxK + _HoehenKarte->Offset1 + 1;
			karteMin1 = std::max(0,karteMin1);
			karteMax1 = std::min((int)_HoehenKarte->Ausdehnung1-1,karteMax1);

			// Anstieg in X-Richtung positiv
			anstiegKoord1pos = FLT_MIN;
			if (richtung1.X>0.0)
				anstiegKoord1pos = richtung1.Z/richtung1.X;
			if (richtung2.X>0.0)
				anstiegKoord1pos = std::max(richtung2.Z/richtung2.X,anstiegKoord1pos);	// flachster Anstieg: Maximum, da Anstieg negativ
			if (anstiegKoord1pos == FLT_MIN)
				anstiegKoord1pos = -10.0*(double)CVoxelRaum::VoxelraumZ; // -"unendlich"

			// Anstieg in X-Richtung negativ
			anstiegKoord1neg = FLT_MAX;
			if (richtung1.X<0.0)
				anstiegKoord1neg = richtung1.Z/richtung1.X;
			if (richtung2.X<0.0)
				anstiegKoord1neg = std::min(richtung2.Z/richtung2.X,anstiegKoord1neg);	// flachster Anstieg: Minimum, da Anstieg positiv
			if (anstiegKoord1neg == FLT_MAX)
				anstiegKoord1neg = 10.0*(double)CVoxelRaum::VoxelraumZ; // "unendlich"

			// Anstieg in Y-Richtung positiv
			anstiegKoord2pos = FLT_MIN;
			if (richtung1.Y>0.0)
				anstiegKoord2pos = richtung1.Z/richtung1.Y;
			if (richtung2.Y>0.0)
				anstiegKoord2pos = std::max(richtung2.Z/richtung2.Y,anstiegKoord2pos);
			if (anstiegKoord2pos == FLT_MIN)
				anstiegKoord2pos = -10.0*(double)CVoxelRaum::VoxelraumZ; // -"unendlich"

			// Anstieg in Y-Richtung negativ
			anstiegKoord2neg = FLT_MAX;
			if (richtung1.Y<0.0)
				anstiegKoord2neg = richtung1.Z/richtung1.Y;
			if (richtung2.Y<0.0)
				anstiegKoord2neg = std::min(richtung2.Z/richtung2.Y,anstiegKoord2neg);
			if (anstiegKoord2neg == FLT_MAX)
				anstiegKoord2neg = 10.0*(double)CVoxelRaum::VoxelraumZ; // "unendlich"

			// Offset Z fuer Umwandlung: Kartenkoordinaten -> delta
			offsetK = - _HoehenKarte->Offset1 - _Position1.X;

			// Testen aller moeglichen Konflikte
			for (int i=karteMin1; i<=karteMax1; i++)
				for (int j=karteMin2; j<=karteMax2; j++)
					if (_HoehenKarte->Hoehenkarte[i][j]!=INT_MAX) // endlicher Wert?
					{
						// delta-Werte ermitteln
						deltaK = i + offsetK;
						deltaY = j + offsetY;

						// Z-Richtung ueberpruefen
						if (deltaK<0.0)
						{
							deltaK += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheK = anstiegKoord1neg*deltaK;
						}
						else
						{
							deltaK -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheK = anstiegKoord1pos*deltaK;
						}

						// Y-Richtung ueberpruefen
						if (deltaY<0.0)
						{
							deltaY += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheY = anstiegKoord2neg*deltaY;
						}
						else
						{
							deltaY -= sicherheitsAbstand;	// Sicherheitsabstand dazurechnen mit Richtung: Zentralposition
							erlaubteHoeheY = anstiegKoord2pos*deltaY;
						}

						// clipping
						erlaubteHoehe = std::min(erlaubteHoeheK,erlaubteHoeheY);
						erlaubteHoehe = std::min(0.0,erlaubteHoehe);

						erlaubteHoehe += _Position1.Z;			// Bezugspunkt beachten
						erlaubteHoehe += sicherheitsAbstand;	// Sicherheitsabstand dazurechnen (da Koordinaten fuer Gangskelett)

						// Verletzung der Hoehe durch Voxel?
						if (_HoehenKarte->Hoehenkarte[i][j]<erlaubteHoehe)
						{
							//std::cout << "!!!KOLLISIONSMOEGLICHKEIT bei Richtung " << _Himmelsrichtung << std::endl;
							return true;
						}

					}
		break; // ENDE: case 3: Richtung Z positiv
	}


	// Keine potentiellen Kollisionen
	return false;
}
