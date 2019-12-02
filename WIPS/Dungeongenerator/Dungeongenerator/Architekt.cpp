#include "Architekt.h"

//=================================
// Raumsektor-Daten ermitteln
//=================================

void DunGen::CArchitekt::BerechneRaumSektoren(unsigned int _RaumSektorAusdehnung)
{
	std::cout << "[Architekt]: Berechne Raumsektoren... " << std::endl;
	Zeitmesser->Start(0);
	RaumSektorAusdehnung = _RaumSektorAusdehnung;

	// wird bereits in COptionen geprueft:
	/*unsigned int maxVoxelDim =  std::max(std::max(CVoxelRaum::VoxelraumX,CVoxelRaum::VoxelraumY),CVoxelRaum::VoxelraumZ);
	if (RaumSektorAusdehnung < maxVoxelDim/16)
		RaumSektorAusdehnung = maxVoxelDim/16;
	if (RaumSektorAusdehnung > maxVoxelDim)
		RaumSektorAusdehnung = maxVoxelDim;*/
	
	// alte Raumsektoren loeschen falls vorhanden
	if (Raumsektor!=NULL)
	{
		for (unsigned int i=0; i<RaumSektorAnzahlX; i++)
		{
			for (unsigned int j=0; j<RaumSektorAnzahlY; j++)
				delete [] Raumsektor[i][j];
			delete [] Raumsektor[i];
		}
		delete [] Raumsektor;
	}

	// Anzahl der Raumsektoren
	RaumSektorAnzahlX = CVoxelRaum::VoxelraumX/RaumSektorAusdehnung;
	// mind. 2 Sektoren Rand auf jeder Seite, und ungerade Anzahl von Sektoren (somit existiert ein Zentralsektor)
	// aufrunden
	if ((CVoxelRaum::VoxelraumX%RaumSektorAusdehnung)>0)
		RaumSektorAnzahlX++;	
	// Randsektoren
	RaumSektorAnzahlX+=4;
	// ungerade machen
	if ((RaumSektorAnzahlX%2)==0)
		RaumSektorAnzahlX++;

	RaumSektorAnzahlY = CVoxelRaum::VoxelraumY/RaumSektorAusdehnung;
	// kein Seitenrand notwendig, und ungerade Anzahl von Sektoren (somit existiert ein Zentralsektor)
	// aufrunden
	if ((CVoxelRaum::VoxelraumY%RaumSektorAusdehnung)>0)
		RaumSektorAnzahlY++;	
	// ungerade machen
	if ((RaumSektorAnzahlY%2)==0)
		RaumSektorAnzahlY++;

	RaumSektorAnzahlZ = CVoxelRaum::VoxelraumZ/RaumSektorAusdehnung;
	// mind. 2 Sektoren Rand auf jeder Seite, und ungerade Anzahl von Sektoren (somit existiert ein Zentralsektor)
	// aufrunden
	if ((CVoxelRaum::VoxelraumZ%RaumSektorAusdehnung)>0)
		RaumSektorAnzahlZ++;	
	// Randsektoren
	RaumSektorAnzahlZ+=4;
	// ungerade machen
	if ((RaumSektorAnzahlZ%2)==0)
		RaumSektorAnzahlZ++;

	// Raumsektoren anlegen
	Raumsektor = new SRaumSektor**[RaumSektorAnzahlX];
	for (unsigned int i=0; i<RaumSektorAnzahlX; i++)
	{
		Raumsektor[i] = new SRaumSektor*[RaumSektorAnzahlY];
		for (unsigned int j=0; j<RaumSektorAnzahlY; j++)
			Raumsektor[i][j] = new SRaumSektor[RaumSektorAnzahlZ];
	}
	
	unsigned int numVoxel = 0;
	unsigned int numFrei = 0;
	unsigned int numFreiMitVoxelNachbar = 0;	

	// fuer alle Raumsektoren: Initialisierung 
	for (unsigned int i=0; i<RaumSektorAnzahlX; i++)
		for (unsigned int j=0; j<RaumSektorAnzahlY; j++)
			for (unsigned int k=0; k<RaumSektorAnzahlZ; k++)
			{
				// Grenzen des Sektors bestimmen
				Raumsektor[i][j][k].GrenzeMinX = (int)CVoxelRaum::VoxelraumX/2 + (i-(int)RaumSektorAnzahlX/2)*(int)RaumSektorAusdehnung - (int)RaumSektorAusdehnung/2;
				Raumsektor[i][j][k].GrenzeMinY = (int)CVoxelRaum::VoxelraumY/2 + (j-(int)RaumSektorAnzahlY/2)*(int)RaumSektorAusdehnung - (int)RaumSektorAusdehnung/2;
				Raumsektor[i][j][k].GrenzeMinZ = (int)CVoxelRaum::VoxelraumZ/2 + (k-(int)RaumSektorAnzahlZ/2)*(int)RaumSektorAusdehnung - (int)RaumSektorAusdehnung/2;
				Raumsektor[i][j][k].GrenzeMaxX = Raumsektor[i][j][k].GrenzeMinX + (int)RaumSektorAusdehnung-1;
				Raumsektor[i][j][k].GrenzeMaxY = Raumsektor[i][j][k].GrenzeMinY + (int)RaumSektorAusdehnung-1;
				Raumsektor[i][j][k].GrenzeMaxZ = Raumsektor[i][j][k].GrenzeMinZ + (int)RaumSektorAusdehnung-1;

				Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_;

				// Karten sind noch nicht erstellt, also vorherige Karten eventuell loeschen
				for (unsigned int h=0; h<4; h++)
					if (Raumsektor[i][j][k].Karte[h] != NULL)
					{
						delete Raumsektor[i][j][k].Karte[h];
						Raumsektor[i][j][k].Karte[h] = NULL;
					}
			};

	int scanMinX, scanMaxX, scanMinY, scanMaxY, scanMinZ, scanMaxZ;
	
	// fuer nicht auessere Raumsektoren: Markierungen der Voxel-belegten Sektoren
	for (unsigned int i=2; i<RaumSektorAnzahlX-2; i++)
		for (unsigned int j=0; j<RaumSektorAnzahlY; j++)
			for (unsigned int k=2; k<RaumSektorAnzahlZ-2; k++)
			{
				// Scanbereich festlegen
				scanMinX = std::max(0,Raumsektor[i][j][k].GrenzeMinX);				
				scanMaxX = std::min((int)CVoxelRaum::VoxelraumX-1,Raumsektor[i][j][k].GrenzeMaxX);
				scanMinY = std::max(0,Raumsektor[i][j][k].GrenzeMinY);				
				scanMaxY = std::min((int)CVoxelRaum::VoxelraumY-1,Raumsektor[i][j][k].GrenzeMaxY);
				scanMinZ = std::max(0,Raumsektor[i][j][k].GrenzeMinZ);				
				scanMaxZ = std::min((int)CVoxelRaum::VoxelraumZ-1,Raumsektor[i][j][k].GrenzeMaxZ);

				// Sektor nach Voxeln abtasten
				for (int i2=scanMinX; i2<=scanMaxX; i2++)
					for (int j2=scanMinY; j2<=scanMaxY; j2++)
						for (int k2=scanMinZ; k2<=scanMaxZ; k2++)
							if (VoxelRaum->Voxel[i2][j2][k2]==1)
								Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_VOXEL_;
			}

	SRaumSektorAndockKarte* aktKarte;
	bool scanXpos, scanXneg, scanZpos, scanZneg;

	// fuer alle Raumsektoren: Markierung der Voxel-Nachbarsektoren und Erstellung der Andockkarten
	for (unsigned int i=0; i<RaumSektorAnzahlX; i++)
		for (unsigned int j=0; j<RaumSektorAnzahlY; j++)
			for (unsigned int k=0; k<RaumSektorAnzahlZ; k++)
			{
				std::cout << ".";
				if (Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_FREI_)
				{
					scanXpos = false;
					scanXneg = false;
					scanZpos = false;
					scanZneg = false;

					// Testen welche Richtungen gescannt werden sollen: 2 Sektoren weit schauen
					if (i>1) if (Raumsektor[i-2][j][k].Belegung == ERaumSektorBelegung::_VOXEL_)
						{scanXneg = true; Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR2_;}
					if (i<RaumSektorAnzahlX-3) if (Raumsektor[i+2][j][k].Belegung == ERaumSektorBelegung::_VOXEL_)
						{scanXpos = true; Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR2_;}
					if (k>1) if (Raumsektor[i][j][k-2].Belegung == ERaumSektorBelegung::_VOXEL_)
						{scanZneg = true; Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR2_;}
					if (k<RaumSektorAnzahlZ-3) if (Raumsektor[i][j][k+2].Belegung == ERaumSektorBelegung::_VOXEL_)
						{scanZpos = true; Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR2_;}

					if (i>0) if (Raumsektor[i-1][j][k].Belegung == ERaumSektorBelegung::_VOXEL_)
						{scanXneg = true; Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR_;}
					if (i<RaumSektorAnzahlX-2) if (Raumsektor[i+1][j][k].Belegung == ERaumSektorBelegung::_VOXEL_)
						{scanXpos = true; Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR_;}
					if (k>0) if (Raumsektor[i][j][k-1].Belegung == ERaumSektorBelegung::_VOXEL_)
						{scanZneg = true; Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR_;}
					if (k<RaumSektorAnzahlZ-2) if (Raumsektor[i][j][k+1].Belegung == ERaumSektorBelegung::_VOXEL_)
						{scanZpos = true; Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR_;}
										
					// Scanbereich Y (Hoehe)
					scanMinY = Raumsektor[i][j][k].GrenzeMinY - (int)GangBreiteHalbKlein-1 - (int)AbstandVomBodenMax;	//  Rand am Sektor mitscannen
					scanMinY = std::max(0,scanMinY);
					scanMaxY = Raumsektor[i][j][k].GrenzeMaxY + (int)GangBreiteHalbGross+1;
					scanMaxY = std::min((int)CVoxelRaum::VoxelraumY-1,scanMaxY);
					
					// Schaue nach X negativ: Sued (Index 2)
					if (scanXneg)
					{
						// Hoehen-Scankarte erstellen
						Raumsektor[i][j][k].Karte[2] = new SRaumSektorAndockKarte(RaumSektorAusdehnung, GangBreite, AbstandVomBodenMax, Raumsektor[i][j][k].GrenzeMinZ, Raumsektor[i][j][k].GrenzeMinY);
						aktKarte = Raumsektor[i][j][k].Karte[2];

						// Initialisierung der Hoehenkarte: ZY-Ebene
						for (int i2=0; i2<aktKarte->Ausdehnung1; i2++)
							for (int j2=0; j2<aktKarte->Ausdehnung2; j2++)
								aktKarte->Hoehenkarte[i2][j2] = INT_MIN;

						// Scanbereich X,Z festlegen
						scanMinX = Raumsektor[i][j][k].GrenzeMinX - 2*(int)RaumSektorAusdehnung;		// 2 Sektoren tiefer Scan
						scanMinX = std::max(0,scanMinX);
						scanMaxX = Raumsektor[i][j][k].GrenzeMaxX;										// + Tiefe des eigenen Sektors wegen Scankartenrand
						scanMaxX = std::min((int)CVoxelRaum::VoxelraumX-1,scanMaxX);

						scanMinZ = Raumsektor[i][j][k].GrenzeMinZ - (int)GangBreiteHalbKlein - 1;	//  Rand am Sektor mitscannen
						scanMinZ = std::max(0,scanMinZ);
						scanMaxZ = Raumsektor[i][j][k].GrenzeMaxZ + (int)GangBreiteHalbGross + 1;
						scanMaxZ = std::min((int)CVoxelRaum::VoxelraumZ-1,scanMaxZ);						
				
						// Scan der Hoehenwerte
						for (int i2=scanMinX; i2<=scanMaxX; i2++)
							for (int j2=scanMinY; j2<=scanMaxY; j2++)
								for (int k2=scanMinZ; k2<=scanMaxZ; k2++)
									if (VoxelRaum->Voxel[i2][j2][k2]==1)
										if (i2>aktKarte->Hoehenkarte[k2+aktKarte->Offset1][j2+aktKarte->Offset2])
											aktKarte->Hoehenkarte[k2+aktKarte->Offset1][j2+aktKarte->Offset2] = i2;						

					}; // ENDE: X negativ
					
					// Schaue nach X positiv: Nord (Index 0)
					if (scanXpos)
					{
						// Hoehen-Scankarte erstellen
						Raumsektor[i][j][k].Karte[0] = new SRaumSektorAndockKarte(RaumSektorAusdehnung, GangBreite, AbstandVomBodenMax, Raumsektor[i][j][k].GrenzeMinZ, Raumsektor[i][j][k].GrenzeMinY);
						aktKarte = Raumsektor[i][j][k].Karte[0];

						// Initialisierung der Hoehenkarte: ZY-Ebene
						for (int i2=0; i2<aktKarte->Ausdehnung1; i2++)
							for (int j2=0; j2<aktKarte->Ausdehnung2; j2++)
								aktKarte->Hoehenkarte[i2][j2] = INT_MAX;

						// Scanbereich X,Z festlegen
						scanMinX = Raumsektor[i][j][k].GrenzeMinX;										// + Tiefe des eigenen Sektors wegen Scankartenrand
						scanMinX = std::max(0,scanMinX);
						scanMaxX = Raumsektor[i][j][k].GrenzeMaxX + 2*(int)RaumSektorAusdehnung;		// 2 Sektoren tiefer Scan
						scanMaxX = std::min((int)CVoxelRaum::VoxelraumX-1,scanMaxX);

						scanMinZ = Raumsektor[i][j][k].GrenzeMinZ - (int)GangBreiteHalbKlein - 1;	//  Rand am Sektor mitscannen
						scanMinZ = std::max(0,scanMinZ);
						scanMaxZ = Raumsektor[i][j][k].GrenzeMaxZ + (int)GangBreiteHalbGross + 1;
						scanMaxZ = std::min((int)CVoxelRaum::VoxelraumZ-1,scanMaxZ);

						// Scan der Hoehenwerte
						for (int i2=scanMinX; i2<=scanMaxX; i2++)
							for (int j2=scanMinY; j2<=scanMaxY; j2++)
								for (int k2=scanMinZ; k2<=scanMaxZ; k2++)
									if (VoxelRaum->Voxel[i2][j2][k2]==1)
										if (i2<aktKarte->Hoehenkarte[k2+aktKarte->Offset1][j2+aktKarte->Offset2])
											aktKarte->Hoehenkarte[k2+aktKarte->Offset1][j2+aktKarte->Offset2] = i2;

					}; // ENDE: X positiv

					// Schaue nach Z negativ: Ost (Index 1)
					if (scanZneg)
					{
						// Hoehen-Scankarte erstellen
						Raumsektor[i][j][k].Karte[1] = new SRaumSektorAndockKarte(RaumSektorAusdehnung, GangBreite, AbstandVomBodenMax, Raumsektor[i][j][k].GrenzeMinX, Raumsektor[i][j][k].GrenzeMinY);
						aktKarte = Raumsektor[i][j][k].Karte[1];

						// Initialisierung der Hoehenkarte: ZY-Ebene
						for (int i2=0; i2<aktKarte->Ausdehnung1; i2++)
							for (int j2=0; j2<aktKarte->Ausdehnung2; j2++)
								aktKarte->Hoehenkarte[i2][j2] = INT_MIN;

						// Scanbereich X,Z festlegen
						scanMinZ = Raumsektor[i][j][k].GrenzeMinZ - 2*(int)RaumSektorAusdehnung;		// 2 Sektoren tiefer Scan
						scanMinZ = std::max(0,scanMinZ);
						scanMaxZ = Raumsektor[i][j][k].GrenzeMaxZ;										// + Tiefe des eigenen Sektors wegen Scankartenrand
						scanMaxZ = std::min((int)CVoxelRaum::VoxelraumZ-1,scanMaxZ);

						scanMinX = Raumsektor[i][j][k].GrenzeMinX - (int)GangBreiteHalbKlein - 1;	//  Rand am Sektor mitscannen
						scanMinX = std::max(0,scanMinX);
						scanMaxX = Raumsektor[i][j][k].GrenzeMaxX + (int)GangBreiteHalbGross + 1;
						scanMaxX = std::min((int)CVoxelRaum::VoxelraumX-1,scanMaxX);						

						// Scan der Hoehenwerte
						for (int i2=scanMinX; i2<=scanMaxX; i2++)
							for (int j2=scanMinY; j2<=scanMaxY; j2++)
								for (int k2=scanMinZ; k2<=scanMaxZ; k2++)
									if (VoxelRaum->Voxel[i2][j2][k2]==1)
										if (k2>aktKarte->Hoehenkarte[i2+aktKarte->Offset1][j2+aktKarte->Offset2])
											aktKarte->Hoehenkarte[i2+aktKarte->Offset1][j2+aktKarte->Offset2] = k2;

					}; // ENDE: Z negativ
					
					// Schaue nach Z positiv: West (Index 3)
					if (scanZpos)
					{
						// Hoehen-Scankarte erstellen
						Raumsektor[i][j][k].Karte[3] = new SRaumSektorAndockKarte(RaumSektorAusdehnung, GangBreite, AbstandVomBodenMax, Raumsektor[i][j][k].GrenzeMinX, Raumsektor[i][j][k].GrenzeMinY);
						aktKarte = Raumsektor[i][j][k].Karte[3];

						// Initialisierung der Hoehenkarte: ZY-Ebene
						for (int i2=0; i2<aktKarte->Ausdehnung1; i2++)
							for (int j2=0; j2<aktKarte->Ausdehnung2; j2++)
								aktKarte->Hoehenkarte[i2][j2] = INT_MAX;

						// Scanbereich X,Z festlegen
						scanMinZ = Raumsektor[i][j][k].GrenzeMinZ;										// + Tiefe des eigenen Sektors wegen Scankartenrand
						scanMinZ = std::max(0,scanMinZ);
						scanMaxZ = Raumsektor[i][j][k].GrenzeMaxZ + 2*(int)RaumSektorAusdehnung;		// 2 Sektoren tiefer Scan
						scanMaxZ = std::min((int)CVoxelRaum::VoxelraumZ-1,scanMaxZ);

						scanMinX = Raumsektor[i][j][k].GrenzeMinX - (int)GangBreiteHalbKlein - 1;	//  Rand am Sektor mitscannen
						scanMinX = std::max(0,scanMinX);
						scanMaxX = Raumsektor[i][j][k].GrenzeMaxX + (int)GangBreiteHalbGross + 1;
						scanMaxX = std::min((int)CVoxelRaum::VoxelraumX-1,scanMaxX);						
						
						// Scan der Hoehenwerte
						for (int i2=scanMinX; i2<=scanMaxX; i2++)
							for (int j2=scanMinY; j2<=scanMaxY; j2++)
								for (int k2=scanMinZ; k2<=scanMaxZ; k2++)
									if (VoxelRaum->Voxel[i2][j2][k2]==1)
										if (k2<aktKarte->Hoehenkarte[i2+aktKarte->Offset1][j2+aktKarte->Offset2])
											aktKarte->Hoehenkarte[i2+aktKarte->Offset1][j2+aktKarte->Offset2] = k2;
					}; // ENDE: Z positiv

				}; // ENDE: wenn Sektor frei

				if (Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_VOXEL_)
					numVoxel++;
				if (Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_FREI_)
					numFrei++;
				if ((Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR_)||(Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR2_))
					numFreiMitVoxelNachbar++;

			}; // ENDE: Erstellung der Andockkarten				

	std::cout << std::endl << "[Architekt]: Berechnung der Raumsektoren abgeschlossen. Belegung der Sektoren:" << std::endl;
	std::cout << "[Architekt]: Voxel: " << numVoxel << " , Frei(ohne VoxelNachbarn): " << numFrei << " , Frei(mit V.N.): " << numFreiMitVoxelNachbar << std::endl;
	std::cout << "[Architekt]: Benoetigte Zeit fuer Berechnung der Raumsektoren: ";
	Zeitmesser->Stop(0);
}

//=================================
// Raum erstellen
//=================================

void DunGen::CArchitekt::ErstelleRaum(CSubSzene* _SubSzene, irr::core::vector3d<double> _Position, irr::core::vector3d<double> _Rotation)
{
	// neuen Raum erstellen
	SDungeonRaum* raum = new SDungeonRaum();
	// Werte zuweisen
	raum->SubSzene = _SubSzene;
	raum->Position = _Position;
	raum->Rotation = _Rotation;
	raum->Skalierung = _SubSzene->LeseSkalierung();
		
	// Andockstellen berechnen:
	// Transformationsmatrix bestimmen
	irr::core::matrix4 transformation;
	transformation.makeIdentity();
	// Extramatrix erforderlich da .setRotation() die Skalierung ueberschreibt
	irr::core::matrix4 transformationSkalierung;
	transformationSkalierung.makeIdentity();

	transformationSkalierung.setScale(irr::core::vector3df(raum->Skalierung.X,raum->Skalierung.Y,raum->Skalierung.Z));
	transformation.setRotationDegrees(irr::core::vector3df(_Rotation.X,_Rotation.Y,_Rotation.Z));
	transformation.setTranslation(irr::core::vector3df(_Position.X,_Position.Y,_Position.Z));

	// Matrizen zusammenfassen
	transformation = transformation * transformationSkalierung;

	// Offset festlegen & entsprechend der Rotation zuweisen (Rotation im UZS!)
	raum->Offset = 0;
	if ((_Rotation.Y > 45) && (_Rotation.Y <= 135))
		raum->Offset = 3;
	else if ((_Rotation.Y > 135) && (_Rotation.Y <= 225))
		raum->Offset = 2;
	else if ((_Rotation.Y > 225) && (_Rotation.Y <= 315))
		raum->Offset = 1;
	
	// Andockstellen auslesen und transformieren
	for (unsigned int i=0; i<4; i++)
	{
		raum->Andockstelle[i] = _SubSzene->LeseAndockstelle((i+raum->Offset)%4);
		raum->Andockstelle[i].TransformiereAndockstelle(transformation);
		// noch keine Gaenge verbunden
		raum->GangID[i] = -1;
	}

	// nun Offset-Richtung umkehren (fuer spaeteren Zugriff auf die zu einer Himmelsrichtung zugehoerigen Geometrien)
	raum->Offset = 4-raum->Offset;
	if (raum->Offset==4)
		raum->Offset = 0;

	// Raum merken
	Raeume.push_back(raum);
}

//==================================================
// Dungeon-Erstellung: Raeume und Gaenge platzieren
//==================================================
// Konstruiere Raeume und Gaenge des Dungeons ausgehend von gegebener Voxelhoehle
void DunGen::CArchitekt::KonstruiereDungeon(unsigned int _RaumSektorAusdehnung)
{
	// bereits vorhandene Raeume und Gaenge loeschen
	for (unsigned int i=0; i<Raeume.size(); i++)
		delete Raeume.at(i);
	Raeume.clear();
	for (unsigned int i=0; i<Gaenge.size(); i++)
		delete Gaenge.at(i);
	Gaenge.clear();

	// finalen Gangquerschnitt ermitteln:
	QuerschnittGang.Punkt.clear();
	QuerschnittGang.TextureX.clear();
	// Punktkoordinaten mit halber Gangbreite multiplizieren
	for (unsigned int i=0; i<QuerschnittGang_Normiert.Punkt.size(); i++)
	{
		QuerschnittGang.Punkt.push_back(0.5*GangBreite*QuerschnittGang_Normiert.Punkt.at(i));
		QuerschnittGang.TextureX.push_back(QuerschnittGang_Normiert.TextureX.at(i));
	}

	// Raumsektoren berechnen
	BerechneRaumSektoren(_RaumSektorAusdehnung);

	// fuer Raeume minimaler Abstand in Y-Richtung zum Rand des Raumsektors
	double raumHoehenToleranz = (double)RaumSektorAusdehnung/4.0;

	std::cout << "[Architekt]: Beginne mit Konstruktion der Raeume und Gaenge... " << std::endl;
	Zeitmesser->Start(0); // gesamte Konstruktion
	Zeitmesser->Start(1); // Gaenge inkl. LOD
	Zeitmesser->Pause(1);
	Zeitmesser->Start(2); // Detailobjekte entlang von Gaengen
	Zeitmesser->Pause(2);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Raeume mit Verbindung zur Hoehle platzieren und verbinden
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::cout << "[Architekt]: Platziere Raeume mit direkter Verbindung zur Hoehle ... " << std::endl;
	
	// Auswahl per Wahrscheinlichkeit vorbereiten
	std::vector<CSubSzene*> subSzenenAuswahl;
	for (unsigned int i=0; i<PlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene; i++)
		subSzenenAuswahl.push_back(NULL);
	for (unsigned int i=0; i<SubSzenen.size(); i++)
		for (unsigned int j=0; j<SubSzenen.at(i)->LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehle(); j++)
			subSzenenAuswahl.push_back(SubSzenen.at(i));

	bool platziert;
	unsigned int auswahlWert;
	std::vector<unsigned int> richtungsAuswahl;

	// 2-pass Durchlauf: direkte Voxel-Nachbar-Sektoren priorisieren
	int modus = 0; // modus 0: _FREI_MIT_VOXELNACHBAR_, modus 1: _FREI_MIT_VOXELNACHBAR2_
	if (subSzenenAuswahl.size()-PlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene>0) // wenn Subszenen zum platzieren da
	while (modus<2)
	{
		// passende Sektoren suchen
		for (unsigned int i=0; i<RaumSektorAnzahlX; i++)
			for (unsigned int j=0; j<RaumSektorAnzahlY; j++)
				for (unsigned int k=0; k<RaumSektorAnzahlZ; k++)
					// schauen nach Voxelnachbarn mit Priorisierung
					if (((modus==0)&&(Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR_))||
						((modus==1)&&(Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR2_)))
					{
						//std::cout << "Raumsektor (" << i << "," << j << "," << k << ")" << std::endl;
						platziert = false;	
										
						// Raum auswuerfeln
						auswahlWert = ZufallsGenerator->GeneriereZufallsWert0Max_(subSzenenAuswahl.size());
						// wenn nicht der leere Raum gewuerfelt
						if (subSzenenAuswahl.at(auswahlWert)!=NULL)
						{
							// Raum erstellen
							ErstelleRaum(subSzenenAuswahl.at(auswahlWert),
								irr::core::vector3d<double>(
								((double)(Raumsektor[i][j][k].GrenzeMinX + Raumsektor[i][j][k].GrenzeMaxX))/2.0,
								((double)(Raumsektor[i][j][k].GrenzeMinY+raumHoehenToleranz + ZufallsGenerator->GeneriereZufallsWert01()*((double)RaumSektorAusdehnung-2.0*raumHoehenToleranz))),
								((double)(Raumsektor[i][j][k].GrenzeMinZ + Raumsektor[i][j][k].GrenzeMaxZ))/2.0
								),
								irr::core::vector3d<double>(0.0,ZufallsGenerator->GeneriereZufallsWert_01()*360.0,0.0) // zufaelliges Rotieren um Y-Achse
								);

							// moegliche Richtungen fuer Gaenge ermitteln
							for (unsigned int h=0; h<4; h++)
								if (Raumsektor[i][j][k].Karte[h] != NULL)
									richtungsAuswahl.push_back(h);

							// alle moeglichen Richtungen abarbeiten
							while (!richtungsAuswahl.empty())
							{
								// Richtung auswuerfeln
								auswahlWert = ZufallsGenerator->GeneriereZufallsWert0Max_(richtungsAuswahl.size());
								//std::cout << "auswahlwert = " << auswahlWert << " -> Richtung " << richtungsAuswahl.at(auswahlWert);

								// Platzierungsversuch wenn noch kein Gang vorliegt oder mit entsprechender Wahrscheinlichkeit fuer zusaetzlichen Gang
								if ((!platziert)||(ZufallsGenerator->GeneriereZufallsWert_01()<=WahrscheinlichkeitZusaetzlicherGangRaumHoehle))
								{
									// Versuchen Gang zur Hoehle zu legen
									if (VerbindeRaumMitHoehle(Raeume.size()-1,i,j,k,richtungsAuswahl.at(auswahlWert)))
									{
										// Raum wurde verbunden
										platziert = true;
										Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_RAUM_;
										Raumsektor[i][j][k].RaumID = Raeume.size()-1;

										// eventuell Sektor durch Gang belegt markieren (falls Verbindung insgesamt 3 Sektoren betrifft, statt nur 2)
										switch (richtungsAuswahl.at(auswahlWert))
										{
										case 0: if (i<RaumSektorAnzahlX-1)
													if (Raumsektor[i+1][j][k].Belegung != ERaumSektorBelegung::_VOXEL_)
														Raumsektor[i+1][j][k].Belegung = ERaumSektorBelegung::_GANG_;
												break;
										case 1: if (k>0)
													if (Raumsektor[i][j][k-1].Belegung != ERaumSektorBelegung::_VOXEL_)
														Raumsektor[i][j][k-1].Belegung = ERaumSektorBelegung::_GANG_;
												break;
										case 2: if (i>0)
													if (Raumsektor[i-1][j][k].Belegung != ERaumSektorBelegung::_VOXEL_)
														Raumsektor[i-1][j][k].Belegung = ERaumSektorBelegung::_GANG_;
												break;
										case 3: if (k<RaumSektorAnzahlZ-1)
													if (Raumsektor[i][j][k+1].Belegung != ERaumSektorBelegung::_VOXEL_)
														Raumsektor[i][j][k+1].Belegung = ERaumSektorBelegung::_GANG_;
												break;
										}

										//std::cout << "Plazierung erfolgreich" << std::endl;
									}
									//else  std::cout << "Plazierung fehlgeschlagen" << std::endl;
								}

								// Richtung abgearbeitet -> aus Auswahl loeschen
								richtungsAuswahl.erase(richtungsAuswahl.begin()+auswahlWert);
						
							} // END while Richtungen uebrig

							// kein Gang moeglich -> Raum wird wieder geloescht
							if (!platziert)
							{						
								delete Raeume.back();
								Raeume.pop_back();
								// Raumsektor umklassifizieren
								Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_FREI_;
							}

						} // END if (SubSzenen.at(auswahlWert)!=NULL)

					} // END if ERaumSektorBelegung::_FREI_MIT_VOXELNACHBAR_
		
		modus++; // naechste Durchlauf
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Raeume mit Verbindung zur anderen Raeumen platzieren und verbinden
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::cout << "[Architekt]: Platziere Raeume mit Verbindung zu anderen Raeumen ... " << std::endl;

	// Auswahl per Wahrscheinlichkeit vorbereiten
	subSzenenAuswahl.clear();
	for (unsigned int i=0; i<PlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene; i++)
		subSzenenAuswahl.push_back(NULL);
	for (unsigned int i=0; i<SubSzenen.size(); i++)
		for (unsigned int j=0; j<SubSzenen.at(i)->LesePlatzierungsWahrscheinlichkeitsFaktorAnRaum(); j++)
			subSzenenAuswahl.push_back(SubSzenen.at(i));

	// wenn Subszenen zum platzieren da
	if (subSzenenAuswahl.size()-PlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene>0)
	{
		// Sektoren mit Nachbarraeumen markieren
		for (unsigned int i=1; i<RaumSektorAnzahlX-1; i++)
			for (unsigned int j=0; j<RaumSektorAnzahlY; j++)
				for (unsigned int k=1; k<RaumSektorAnzahlZ-1; k++)
					if (Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_RAUM_)
					{
						if (Raumsektor[i-1][j][k].Belegung == ERaumSektorBelegung::_FREI_)
							Raumsektor[i-1][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_RAUMNACHBAR_;
						if (Raumsektor[i+1][j][k].Belegung == ERaumSektorBelegung::_FREI_)
							Raumsektor[i+1][j][k].Belegung = ERaumSektorBelegung::_FREI_MIT_RAUMNACHBAR_;
						if (Raumsektor[i][j][k-1].Belegung == ERaumSektorBelegung::_FREI_)
							Raumsektor[i][j][k-1].Belegung = ERaumSektorBelegung::_FREI_MIT_RAUMNACHBAR_;
						if (Raumsektor[i][j][k+1].Belegung == ERaumSektorBelegung::_FREI_)
							Raumsektor[i][j][k+1].Belegung = ERaumSektorBelegung::_FREI_MIT_RAUMNACHBAR_;
					}



		// Raume platzieren
		for (unsigned int i=0; i<RaumSektorAnzahlX; i++)
			for (unsigned int j=0; j<RaumSektorAnzahlY; j++)
				for (unsigned int k=0; k<RaumSektorAnzahlZ; k++)
					if (Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_FREI_MIT_RAUMNACHBAR_)
					{
						// Raum auswuerfeln
						auswahlWert = ZufallsGenerator->GeneriereZufallsWert0Max_(subSzenenAuswahl.size());
						// wenn nicht der leere Raum gewuerfelt
						if (subSzenenAuswahl.at(auswahlWert)!=NULL)
						{
							// Raum erstellen
							ErstelleRaum(subSzenenAuswahl.at(auswahlWert),
								irr::core::vector3d<double>(
								((double)(Raumsektor[i][j][k].GrenzeMinX + Raumsektor[i][j][k].GrenzeMaxX))/2.0,
								((double)(Raumsektor[i][j][k].GrenzeMinY+raumHoehenToleranz + ZufallsGenerator->GeneriereZufallsWert01()*((double)RaumSektorAusdehnung-2.0*raumHoehenToleranz))),
								((double)(Raumsektor[i][j][k].GrenzeMinZ + Raumsektor[i][j][k].GrenzeMaxZ))/2.0
								),
								irr::core::vector3d<double>(0.0,ZufallsGenerator->GeneriereZufallsWert_01()*360.0,0.0) // zufaelliges Rotieren um Y-Achse
								);

							// moegliche Richtungen fuer Gaenge ermitteln
							richtungsAuswahl.clear();
							if (i<RaumSektorAnzahlX-1)	if (Raumsektor[i+1][j][k].Belegung == ERaumSektorBelegung::_RAUM_)	richtungsAuswahl.push_back(0);
							if (k>0)					if (Raumsektor[i][j][k-1].Belegung == ERaumSektorBelegung::_RAUM_)	richtungsAuswahl.push_back(1);
							if (i>0)					if (Raumsektor[i-1][j][k].Belegung == ERaumSektorBelegung::_RAUM_)	richtungsAuswahl.push_back(2);
							if (k<RaumSektorAnzahlZ-1)	if (Raumsektor[i][j][k+1].Belegung == ERaumSektorBelegung::_RAUM_)	richtungsAuswahl.push_back(3);

							// Richtung auswuerfeln
							auswahlWert = ZufallsGenerator->GeneriereZufallsWert0Max_(richtungsAuswahl.size());
							// Raeume verbinden
							switch (richtungsAuswahl.at(auswahlWert))
							{
							case 0:		VerbindeRaeume(Raeume.size()-1,0, Raumsektor[i+1][j][k].RaumID, 2);									
										break;
							case 1:		VerbindeRaeume(Raeume.size()-1,1, Raumsektor[i][j][k-1].RaumID, 3);
										break;
							case 2:		VerbindeRaeume(Raeume.size()-1,2, Raumsektor[i-1][j][k].RaumID, 0);
										break;
							case 3:		VerbindeRaeume(Raeume.size()-1,3, Raumsektor[i][j][k+1].RaumID, 1);
										break;
							}

							// Raumsektor ist nun mit Raum belegt
							Raumsektor[i][j][k].Belegung = ERaumSektorBelegung::_RAUM_;
							Raumsektor[i][j][k].RaumID = Raeume.size()-1;

						} // END if nicht der leere Raum gewuerfelt

					} // END if ERaumSektorBelegung::_FREI_MIT_RAUMNACHBAR_
	} //END if (subSzenenAuswahl.size()>0)
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Weitere Gaenge zwischen Raeumen einfuegen
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::cout << "[Architekt]: Fuege zusaetzliche Gaenge von Raum zu Raum ein ... " << std::endl;

	for (unsigned int i=0; i<RaumSektorAnzahlX-1; i++)
		for (unsigned int j=0; j<RaumSektorAnzahlY; j++)
			for (unsigned int k=0; k<RaumSektorAnzahlZ-1; k++)
				// Sektor mit Belegung Raum
				if (Raumsektor[i][j][k].Belegung == ERaumSektorBelegung::_RAUM_)
				{
					// Gang in X-Richtung: noch kein Gang vorhanden, aber moeglich?
					if (Raumsektor[i+1][j][k].Belegung == ERaumSektorBelegung::_RAUM_)
						if (Raeume.at(Raumsektor[i][j][k].RaumID)->GangID[0]==-1)
							if (ZufallsGenerator->GeneriereZufallsWert_01()<=WahrscheinlichkeitZusaetzlicherGangRaumRaum)
								VerbindeRaeume(	Raumsektor[i][j][k].RaumID,0,Raumsektor[i+1][j][k].RaumID,2);
					// Gang in Z-Richtung: noch kein Gang vorhanden, aber moeglich?
					if (Raumsektor[i][j][k+1].Belegung == ERaumSektorBelegung::_RAUM_)
						if (Raeume.at(Raumsektor[i][j][k].RaumID)->GangID[3]==-1)
							if (ZufallsGenerator->GeneriereZufallsWert_01()<=WahrscheinlichkeitZusaetzlicherGangRaumRaum)
								VerbindeRaeume(	Raumsektor[i][j][k].RaumID,3,Raumsektor[i][j][k+1].RaumID,1);
				}
	
	std::cout << "[Architekt]: Konstruktion komplett. Anzahl Raeume: " << Raeume.size() << " , Anzahl Gaenge: " << Gaenge.size() << std::endl;
	// Zaehlen der blickdichten Gaenge
	auswahlWert  = 0;	// Recyling der schon bestehenden Variable
	for (unsigned int i=0; i<Gaenge.size(); i++)
		if (Gaenge.at(i)->LeseDefinitivBlickdicht())
			auswahlWert++;
	std::cout << "==> davon Anzahl definitiv blickdichter Gaenge: " << auswahlWert << std::endl;

	std::cout << "[Architekt]: Benoetigte Zeit fuer Konstruktion: ";
	Zeitmesser->Stop(0);
	std::cout << "==> dafuer fuer Gaenge inkl. Detailstufen und Visibility-Berechnung, exklusive Detailobjekten: ";
	Zeitmesser->Fortsetzen(1);
	Zeitmesser->Stop(1);
	std::cout << "==> und fuer Detailobjekte: ";
	Zeitmesser->Fortsetzen(2);
	Zeitmesser->Stop(2);
}

