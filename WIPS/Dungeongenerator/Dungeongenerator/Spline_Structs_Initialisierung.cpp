#include "Spline.h"

// ======================================================
// Structs
// ======================================================

void DunGen::SSplineAndockStelle::TransformiereAndockstelle(irr::core::matrix4 _TransformationsMatrix)
{
	// Matrix in double-Werte umwandeln
	irr::core::CMatrix4<double> transformationsMatrix;
	for (unsigned i=0; i<16; i++)
		transformationsMatrix[i] = _TransformationsMatrix[i];
	double tempVec[4];	// temporaere 1x4 Matrix

	// neue Werte fuer Punkte berechnen
	std::vector<irr::core::vector3d<double>> tempPunkte;
	for (unsigned i=0; i<Punkt.size(); i++)
	{
		tempVec[0] = Punkt.at(i).X;
		tempVec[1] = Punkt.at(i).Y;
		tempVec[2] = Punkt.at(i).Z;
		tempVec[3] = 1;
		transformationsMatrix.multiplyWith1x4Matrix(tempVec);
		tempPunkte.push_back(irr::core::vector3d<double>(tempVec[0],tempVec[1],tempVec[2]));
	}
	// und uebernehmen
	Punkt.clear();
	for (unsigned i=0; i<tempPunkte.size(); i++)
		Punkt.push_back(tempPunkte.at(i));

	// Mittelpunkt transformieren
	tempVec[0] = Mittelpunkt.X;
	tempVec[1] = Mittelpunkt.Y;
	tempVec[2] = Mittelpunkt.Z;
	tempVec[3] = 1;
	transformationsMatrix.multiplyWith1x4Matrix(tempVec);
	Mittelpunkt = irr::core::vector3d<double>(tempVec[0],tempVec[1],tempVec[2]);

	// Ausdehnung transformieren (Ausdehnung kann 0 sein, deswegen nicht Normale gleich mitbestimmbar)
	tempVec[0] = Ausdehnung * Normale.X;
	tempVec[1] = Ausdehnung * Normale.Y;
	tempVec[2] = Ausdehnung * Normale.Z;
	tempVec[3] = 0;
	transformationsMatrix.multiplyWith1x4Matrix(tempVec);
	Ausdehnung = irr::core::vector3d<double>(tempVec[0],tempVec[1],tempVec[2]).getLength();

	// Normale transformieren
	tempVec[0] = Normale.X;
	tempVec[1] = Normale.Y;
	tempVec[2] = Normale.Z;
	tempVec[3] = 0;
	transformationsMatrix.multiplyWith1x4Matrix(tempVec);
	Normale = irr::core::vector3d<double>(tempVec[0],tempVec[1],tempVec[2]);
	Normale.normalize();	// da Transformation auch Skalierung einbezieht
}

DunGen::SSplineViewingVolume::SSplineViewingVolume(SSplineViewRegionOfInterest* _Region1, SSplineViewRegionOfInterest* _Region2)
{
	// Mittelpunkt der Projektionsebene
	Bezugspunkt = _Region1->Position;
	// Vektoren die die Projektionsebene aufspannen
	irr::core::vector3d<double> tempVec3 = (_Region2->Position - _Region1->Position).normalize();	// Vorn
	Links = (tempVec3.crossProduct(CSpline::ObenStandard)).normalize();
	Oben = Links.crossProduct(tempVec3);	
	irr::core::vector2d<double> tempVec2;

	// BB 1 projizieren
	// dazu alle 4 Eckpunkte projieren und zur VV-BB hinzufuegen
	tempVec3 = -_Region1->QuerschnittBB.UpperLeftCorner.X * _Region1->Links + _Region1->QuerschnittBB.UpperLeftCorner.Y * _Region1->Oben;	// als 3D-Vektor
	tempVec2.X = -tempVec3.dotProduct(Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(Oben);
	ViewingVolumeBB.UpperLeftCorner = tempVec2; // Erster Pkt. der VV-BB
	ViewingVolumeBB.LowerRightCorner = tempVec2;

	tempVec3 = -_Region1->QuerschnittBB.LowerRightCorner.X * _Region1->Links + _Region1->QuerschnittBB.LowerRightCorner.Y * _Region1->Oben;	// als 3D-Vektor
	tempVec2.X = -tempVec3.dotProduct(Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(Oben);
	ViewingVolumeBB.addInternalPoint(tempVec2);	// Hinzufuegen

	tempVec3 = -_Region1->QuerschnittBB.UpperLeftCorner.X * _Region1->Links + _Region1->QuerschnittBB.LowerRightCorner.Y * _Region1->Oben;	// als 3D-Vektor
	tempVec2.X = -tempVec3.dotProduct(Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(Oben);
	ViewingVolumeBB.addInternalPoint(tempVec2);	// Hinzufuegen

	tempVec3 = -_Region1->QuerschnittBB.LowerRightCorner.X * _Region1->Links + _Region1->QuerschnittBB.UpperLeftCorner.Y * _Region1->Oben;	// als 3D-Vektor
	tempVec2.X = -tempVec3.dotProduct(Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(Oben);
	ViewingVolumeBB.addInternalPoint(tempVec2);	// Hinzufuegen
	
	// BB 2 projizieren
	// dazu alle 4 Eckpunkte projieren und zur VV-BB hinzufuegen
	tempVec3 = -_Region2->QuerschnittBB.UpperLeftCorner.X * _Region2->Links + _Region2->QuerschnittBB.UpperLeftCorner.Y * _Region2->Oben;	// als 3D-Vektor
	tempVec2.X = -tempVec3.dotProduct(Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(Oben);
	ViewingVolumeBB.addInternalPoint(tempVec2);	// Hinzufuegen

	tempVec3 = -_Region2->QuerschnittBB.LowerRightCorner.X * _Region2->Links + _Region2->QuerschnittBB.LowerRightCorner.Y * _Region2->Oben;	// als 3D-Vektor
	tempVec2.X = -tempVec3.dotProduct(Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(Oben);
	ViewingVolumeBB.addInternalPoint(tempVec2);	// Hinzufuegen

	tempVec3 = -_Region2->QuerschnittBB.UpperLeftCorner.X * _Region2->Links + _Region2->QuerschnittBB.LowerRightCorner.Y * _Region2->Oben;	// als 3D-Vektor
	tempVec2.X = -tempVec3.dotProduct(Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(Oben);
	ViewingVolumeBB.addInternalPoint(tempVec2);	// Hinzufuegen

	tempVec3 = -_Region2->QuerschnittBB.LowerRightCorner.X * _Region2->Links + _Region2->QuerschnittBB.UpperLeftCorner.Y * _Region2->Oben;	// als 3D-Vektor
	tempVec2.X = -tempVec3.dotProduct(Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(Oben);
	ViewingVolumeBB.addInternalPoint(tempVec2);	// Hinzufuegen
}


// ======================================================
// Initialisierung & De-Initialisierung & Auslesen
// ======================================================
// Genauigkeit festlegen
const double DunGen::CSpline::Genauigkeit = 0.00001;
// Toleranz festlegen
//const double DunGen::CSpline::Toleranz = 0.001 * Genauigkeit;

// Hoehenachse festlegen
const irr::core::vector3d<double> DunGen::CSpline::ObenStandard = irr::core::vector3d<double>(0,1,0);	// Y ist Hoechenachse


// Konstruktor: Aufruf der Splineerstellung mit vorheriger Richtungsanpassung
DunGen::CSpline::CSpline(SSplineQuerschnitt _QuerSchnitt, SSplineAndockStelle _AndockStelle1, SSplineAndockStelle _AndockStelle2,
			irr::core::vector3d<double> _Position1, irr::core::vector3d<double> _Position2, irr::core::vector3d<double> _Ableitung1, irr::core::vector3d<double> _Ableitung2,
			double _Abstand, double _AbstandTexturYProAbstand1, unsigned int _NumDetailstufen)
{
	std::cout << "[Spline]: Neuer Spline wird generiert. Anzahl Detailstufen = " << _NumDetailstufen << " ..." << std::endl;
	/*std::cout << "Position1 = " << _Position1.X << ", " << _Position1.Y << ", " << _Position1.Z << std::endl;
	std::cout << "Position2 = " << _Position2.X << ", " << _Position2.Y << ", " << _Position1.Z << std::endl;
	std::cout << "Ableitung1 = " << _Ableitung1.X << ", " << _Ableitung1.Y << ", " << _Ableitung1.Z << std::endl;
	std::cout << "-Ableitung2 = " << _Ableitung2.X << ", " << _Ableitung2.Y << ", " << _Ableitung2.Z << std::endl;*/

	// Werte uebernehmen
	Position[0] = _Position1;
	Position[1] = _Position2;
	Ableitung[0] = _Ableitung1;
	Ableitung[1] = -_Ableitung2; // Ableitung umkehren
	// passende Koeffizienten berechnen
	BerechneResultierendeKoeffizienten();
		
	// Reihenfolge der Punkte der Andockstelle 2 umdrehen
	SSplineAndockStelle andockstelle2;
	for (unsigned int i=1; i<=_AndockStelle2.Punkt.size(); i++)
		andockstelle2.Punkt.push_back(_AndockStelle2.Punkt.at(_AndockStelle2.Punkt.size()-i));
	andockstelle2.Normale = -_AndockStelle2.Normale;
	andockstelle2.Mittelpunkt = _AndockStelle2.Mittelpunkt;
	andockstelle2.Ausdehnung = _AndockStelle2.Ausdehnung;
	
	// Eigentliche Erstellung aufrufen mit angepasster Andockstelle 2 und umgekehrter Ableitung 2
	for (unsigned i=0; i<=_NumDetailstufen; i++)
		ErstelleSpline(_QuerSchnitt,_AndockStelle1,andockstelle2,_Abstand,_AbstandTexturYProAbstand1, i);

	// Visibility-Test durchfuehren
	VisibilityTest(_QuerSchnitt, _AndockStelle1, andockstelle2);
		
	std::cout << "[Spline]: Spline-Generierung abgeschlossen. # Meshbuffer(Original) = " << MeshSpline.at(0)->getMeshBufferCount() << std::endl;
}

DunGen::CSpline::~CSpline()
{
	// loeschen der Meshes anstossen, werden geloescht wenn nirgenswo anders mehr referenziert
	for (unsigned int i=0; i<MeshSpline.size(); i++)
		MeshSpline.at(i)->drop();
	MeshAdapter[0]->drop();
	MeshAdapter[1]->drop();

	// Detailobjekte loeschen
	for (unsigned int i=0; i<Detailobjekte.size(); i++)
		delete Detailobjekte.at(i);
	Detailobjekte.clear();
}


// Positions- und Ableitungskoeffizienten berechnen
void DunGen::CSpline::BerechneResultierendeKoeffizienten()
{
	// Positions-Koeffizienten berechnen
	PositionsKoeffizienten[0] = 2 * Position[0] - 2 * Position[1] + Ableitung[0] + Ableitung[1];
	PositionsKoeffizienten[1] = -3 * Position[0] + 3 * Position[1] - 2 * Ableitung[0] - Ableitung[1];
	PositionsKoeffizienten[2] = Ableitung[0];
	PositionsKoeffizienten[3] = Position[0];

	// Ableitungs-Koeffizienten berechnen
	AbleitungsKoeffizienten[0] = 6 * Position[0] -6 * Position[1] + 3 * Ableitung[0] + 3 * Ableitung[1];
	AbleitungsKoeffizienten[1] = -6 * Position[0] + 6 * Position[1] - 4 * Ableitung[0] - 2 * Ableitung[1];
	AbleitungsKoeffizienten[2] = Ableitung[0];
}

// ID's der Andockstellen einstellen (Informationen womit der Spline an diesen Enden verbunden ist)
void  DunGen::CSpline::SetzeAndocksstellenIds(int _IDAndockstelle0, int _IDAndockstelle1)
{
	IDAndockstelle0 = _IDAndockstelle0;
	IDAndockstelle1 = _IDAndockstelle1;
}

// Andockstellen-ID's auslesen
int DunGen::CSpline::LeseAndocksstellenId0()
{
	return IDAndockstelle0;
}

int DunGen::CSpline::LeseAndocksstellenId1()
{
	return IDAndockstelle1;
}

// Blickdichtigkeit auslesen
bool DunGen::CSpline::LeseDefinitivBlickdicht()
{
	return DefinitivBlickdicht;
}

// Position und Ableitung auslesen
irr::core::vector3d<double> DunGen::CSpline::LesePosition(unsigned int _Ende)
{
	return Position[_Ende];
}

irr::core::vector3d<double> DunGen::CSpline::LeseAbleitung(unsigned int _Ende)
{
	return Ableitung[_Ende];
}