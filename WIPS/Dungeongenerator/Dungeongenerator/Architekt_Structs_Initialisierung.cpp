#include "Architekt.h"

//=================================
// Structs
//=================================
// Raumsektor: Konstruktor
DunGen::SRaumSektor::SRaumSektor()
{	
	// alle Himmelsrichtungen:
	for (unsigned int i=0; i<4; i++)
		// noch keine Hoehenkarten erstellt
		Karte[i] = NULL;
}
// Raumsektor: Destruktor
DunGen::SRaumSektor::~SRaumSektor()
{
	// eventuelle Hoehenkarten loeschen
	for (unsigned int i=0; i<4; i++)
		if (Karte[i] != NULL)
			delete Karte[i];
}

// Raumsektor-Karte: Konstruktor
DunGen::SRaumSektorAndockKarte::SRaumSektorAndockKarte(unsigned int _RaumSektorAusdehnung, unsigned int _GangBreite, unsigned int _AbstandVomBodenMax, int _SektorVoxelMin1, int _SektorVoxelMin2)
{
	// Ausdehnung der Hoehenkarte = Raumsektorausdehnung + GangBreiteHalbKlein + GangbreiteHalbGross + 1 Voxel Rand * 2 Seiten
	Ausdehnung1 = _RaumSektorAusdehnung+_GangBreite+1;
	Ausdehnung2 = Ausdehnung1 + _AbstandVomBodenMax; // Hoehenausdehnung braucht noch Platz fuer den Abstand zum Boden

	// Hoehenkarte anlegen: Ausdehnung1*Ausdehnung2
	Hoehenkarte = new int*[Ausdehnung1];
	for (unsigned int i=0; i<Ausdehnung1; i++)
		Hoehenkarte[i] = new int[Ausdehnung2];

	// valide Hoheninformationen: Ausdehnung1*Ausdehnung2
	ValideHoeheninformation = new unsigned char*[Ausdehnung1];
	for (unsigned int i=0; i<Ausdehnung1; i++)
		ValideHoeheninformation[i] = new unsigned char[Ausdehnung2];
		
	// alle Hoeheninformationen sind erst einmal valide
	for (unsigned int i=0; i<Ausdehnung1; i++)
		for (unsigned int j=0; j<Ausdehnung2; j++)
			ValideHoeheninformation[i][j]=1;

	// Offsets kalkulieren: Beziehung Voxel zu Karte (VoxelKoord+Offset = KartenKoord)
	// (GangbreiteHalb + 1 Voxel Gangrand + eventl. Abstand zum Boden) entspricht (SektorVoxelMin) in Voxelkoordinaten
	Offset1 = ((int)_GangBreite-1)/2 +1 - _SektorVoxelMin1;
	Offset2 = ((int)_GangBreite-1)/2 +1 + (int)_AbstandVomBodenMax - _SektorVoxelMin2;
}
// Raumsektor-Karte: Destruktor
DunGen::SRaumSektorAndockKarte::~SRaumSektorAndockKarte()
{
	// Hoehenkarte loeschen
	for (unsigned int i=0; i<Ausdehnung1; i++)
		delete [] Hoehenkarte[i];
	delete [] Hoehenkarte;
	for (unsigned int i=0; i<Ausdehnung1; i++)
		delete [] ValideHoeheninformation[i];
	delete [] ValideHoeheninformation;	
}

//=================================
// Architekt: Intialisierung
//=================================

// Konstruktor
DunGen::CArchitekt::CArchitekt(CVoxelRaum* _VoxelRaum, CDreiecksMesh* _DreiecksMesh, CZufallsGenerator* _ZufallsGenerator, CZeitmesser* _Zeitmesser)
{
	// Werte uebernehmen
	VoxelRaum = _VoxelRaum;
	DreiecksMesh = _DreiecksMesh;
	ZufallsGenerator = _ZufallsGenerator;
	Zeitmesser = _Zeitmesser;

	// noch keine Raumsektoren berechnet
	Raumsektor = NULL;
	
	// Werte initialisieren
	SetzeGangBreite(8);
	ZusaetzlicheAusfraestiefe = 1;
	AbstandVomBodenMax = 5;
	TiefeAbstandsTestMin = 2;
	AbstandAndockHoehleMin = 0;
	ZusaetzlicherSicherheitsabstand = 1.0;

	PlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene = 0;
	PlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene = 0;
	WahrscheinlichkeitZusaetzlicherGangRaumHoehle = 1.0;
	WahrscheinlichkeitZusaetzlicherGangRaumRaum = 1.0;
			
	AbstandSplineSegmente = 0.3;
	AbstandTexturYProAbstand1 = 0.5/GangBreite;
	 // Anzahl zusaetzlicher Detailstufen
	AnzahlSplineDetailstufen = 3;
}

void DunGen::CArchitekt::Initialisiere(irr::IrrlichtDevice* _IrrDevice)
{
	IrrDevice = _IrrDevice;

	/*/ ZUM TESTEN
	// weitere Gangeigenschaften
	QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(-1,-1));
	QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(-1,0));
	QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(-0.75,0.5));
	QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(-0.5,0.75));
	QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(0,1));
	//QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(0.5,0.75));
	//QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(0.75,0.5));
	QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(1,0));
	QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(1,-1));
	QuerschnittGang_Normiert.Punkt.push_back(irr::core::vector2d<double>(0,-1));
	
	QuerschnittGang_Normiert.TextureX.push_back(1.4);
	QuerschnittGang_Normiert.TextureX.push_back(0.9);
	QuerschnittGang_Normiert.TextureX.push_back(0.6);
	QuerschnittGang_Normiert.TextureX.push_back(0.3);
	QuerschnittGang_Normiert.TextureX.push_back(0);
	//QuerschnittGang_Normiert.TextureX.push_back(0.5);
	//QuerschnittGang_Normiert.TextureX.push_back(1.0);
	QuerschnittGang_Normiert.TextureX.push_back(0.9);
	QuerschnittGang_Normiert.TextureX.push_back(1.4);
	QuerschnittGang_Normiert.TextureX.push_back(2.0);

	// Raumvorlagen einladen
	CSubSzene* raum = new CSubSzene(IrrDevice,"RaumVorlagen/Raum_Quadratisch0001.irr");
	SubSzenen.push_back(raum);
	raum = new CSubSzene(IrrDevice,"RaumVorlagen/Raum_Quadratisch0001b.irr");
	SubSzenen.push_back(raum);
	raum = new CSubSzene(IrrDevice,"RaumVorlagen/Raum_Quadratisch0001c.irr");
	SubSzenen.push_back(raum);
	raum = new CSubSzene(IrrDevice,"RaumVorlagen/Raum_Quadratisch0001d.irr");
	SubSzenen.push_back(raum);

	// Detailobjektvorlagen erstellen
	irr::io::path meshPfad = "Detailobjekte/TokenBasis0001.3DS";

	SSplineDetailobjektVorlage* neueVorlage = new SSplineDetailobjektVorlage();
	neueVorlage->Name = "Testobjekt EINS";
	neueVorlage->MeshPfad = meshPfad;
	neueVorlage->Skalierung = irr::core::vector3d<double>(1.0,1.0,1.0);
	neueVorlage->Rotation = irr::core::vector3d<double>(20.0,-90.0,0.0);
	neueVorlage->Position = irr::core::vector2d<double>(1.0,0.0);
	neueVorlage->AbstandsAbtastung = AbstandSplineSegmente;
	neueVorlage->AbstandNumFaktor = 8;
	neueVorlage->AbstandNumMinErstesElement = 0;
	neueVorlage->AbstandNumMaxErstesElement = 0;
	neueVorlage->AbstandNumMin = 3;
	neueVorlage->AbstandNumMax = 6;
	neueVorlage->ObjektAn1 = true;

	DetailobjektVorlagen.push_back(neueVorlage);

	neueVorlage = new SSplineDetailobjektVorlage();
	neueVorlage->Name = "Testobjekt ZWEI";
	neueVorlage->MeshPfad = meshPfad;
	neueVorlage->Skalierung = irr::core::vector3d<double>(1.0,1.0,1.0);
	neueVorlage->Rotation = irr::core::vector3d<double>(20.0,90.0,0.0);
	neueVorlage->Position = irr::core::vector2d<double>(-1.0,0.0);
	neueVorlage->AbstandsAbtastung = AbstandSplineSegmente;
	neueVorlage->AbstandNumFaktor = 8;
	neueVorlage->AbstandNumMinErstesElement = 6;
	neueVorlage->AbstandNumMaxErstesElement = 6;
	neueVorlage->AbstandNumMin = 3;
	neueVorlage->AbstandNumMax = 3;
	neueVorlage->ObjektAn1 = false;
	DetailobjektVorlagen.push_back(neueVorlage);
	//*/
}


//=================================
// Architekt: Werte setzen
//=================================

void DunGen::CArchitekt::SetzeGangBreite(unsigned int _GangBreite)
{
	// Gangbreite setzen und deren Zerlegung bestimmen
	GangBreite = _GangBreite;
	GangBreiteHalbKlein = (GangBreite-1)/2;
	GangBreiteHalbGross = (GangBreite-1)-GangBreiteHalbKlein;
}
void DunGen::CArchitekt::SetzeAbstandSplineSegmente(double _AbstandSplineSegmente)
{
	AbstandSplineSegmente = _AbstandSplineSegmente;
}
void DunGen::CArchitekt::SetzeAbstandTexturYProAbstand1(double _AbstandTexturYProAbstand1)
{
	AbstandTexturYProAbstand1 = _AbstandTexturYProAbstand1;
}
void DunGen::CArchitekt::SetzeAnzahlSplineDetailstufen(unsigned int _AnzahlSplineDetailstufen)
{
	AnzahlSplineDetailstufen = _AnzahlSplineDetailstufen;
}
void DunGen::CArchitekt::SetzeZusaetzlicheAusfraestiefe(unsigned int _ZusaetzlicheAusfraestiefe)
{
	ZusaetzlicheAusfraestiefe = _ZusaetzlicheAusfraestiefe;
}
void DunGen::CArchitekt::SetzeAbstandVomBodenMax(unsigned int _AbstandVomBodenMax)
{
	AbstandVomBodenMax = _AbstandVomBodenMax;
}
void DunGen::CArchitekt::SetzeTiefeAbstandsTestMin(unsigned int _TiefeAbstandsTestMin)
{
	TiefeAbstandsTestMin = _TiefeAbstandsTestMin;
}
void DunGen::CArchitekt::SetzeAbstandAndockHoehleMin(unsigned int _AbstandAndockHoehleMin)
{
	AbstandAndockHoehleMin = _AbstandAndockHoehleMin;
}
void DunGen::CArchitekt::SetzeZusaetzlicherSicherheitsabstand(double _ZusaetzlicherSicherheitsabstand)
{
	ZusaetzlicherSicherheitsabstand = _ZusaetzlicherSicherheitsabstand;
}
void DunGen::CArchitekt::SetzePlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene(unsigned int _PlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene)
{
	PlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene = _PlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene;
}
void DunGen::CArchitekt::SetzePlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene(unsigned int _PlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene)
{
	PlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene = _PlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene;
}
void DunGen::CArchitekt::SetzeWahrscheinlichkeitZusaetzlicherGangRaumHoehle(double _WahrscheinlichkeitZusaetzlicherGangRaumHoehle)
{
	WahrscheinlichkeitZusaetzlicherGangRaumHoehle = _WahrscheinlichkeitZusaetzlicherGangRaumHoehle;
}
void DunGen::CArchitekt::SetzeWahrscheinlichkeitZusaetzlicherGangRaumRaum(double _WahrscheinlichkeitZusaetzlicherGangRaumRaum)
{
	WahrscheinlichkeitZusaetzlicherGangRaumRaum = _WahrscheinlichkeitZusaetzlicherGangRaumRaum;
}

//=================================
// Architekt: Werte auslesen
//=================================

unsigned int DunGen::CArchitekt::LeseGangBreite()
{
	return GangBreite;
}
double DunGen::CArchitekt::LeseAbstandSplineSegmente()
{
	return AbstandSplineSegmente;
}
double DunGen::CArchitekt::LeseAbstandTexturYProAbstand1()
{
	return AbstandTexturYProAbstand1;
}
unsigned int DunGen::CArchitekt::LeseAnzahlSplineDetailstufen()
{
	return AnzahlSplineDetailstufen;
}
unsigned int DunGen::CArchitekt::LeseZusaetzlicheAusfraestiefe()
{
	return ZusaetzlicheAusfraestiefe;
}
unsigned int DunGen::CArchitekt::LeseAbstandVomBodenMax()
{
	return AbstandVomBodenMax;
}
unsigned int DunGen::CArchitekt::LeseTiefeAbstandsTestMin()
{
	return TiefeAbstandsTestMin;
}
unsigned int DunGen::CArchitekt::LeseAbstandAndockHoehleMin()
{
	return AbstandAndockHoehleMin;
}
double DunGen::CArchitekt::LeseZusaetzlicherSicherheitsabstand()
{
	return ZusaetzlicherSicherheitsabstand;
}
unsigned int DunGen::CArchitekt::LesePlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene()
{
	return PlatzierungsWahrscheinlichkeitsFaktorAnHoehleLeereSubSzene;
}
unsigned int DunGen::CArchitekt::LesePlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene()
{
	return PlatzierungsWahrscheinlichkeitsFaktorAnRaumLeereSubSzene;
}
double DunGen::CArchitekt::LeseWahrscheinlichkeitZusaetzlicherGangRaumHoehle()
{
	return WahrscheinlichkeitZusaetzlicherGangRaumHoehle;
}
double DunGen::CArchitekt::LeseWahrscheinlichkeitZusaetzlicherGangRaumRaum()
{
	return WahrscheinlichkeitZusaetzlicherGangRaumRaum;
}