#include "Spline.h"

// ======================================================
// Detailobjekte
// ======================================================
// wichtig: Max>=Min, _AbstandNumMin>=1, _AbstandNumMinErstesElement>=0
void DunGen::CSpline::PlatziereDetailobjekt(irr::core::stringc _Name, irr::scene::IMesh* _ObjektMesh, irr::core::vector3d<double> _Skalierung, irr::core::vector3d<double> _Rotation, irr::core::vector2d<double> _Position,
	double _AbstandsAbtastung, unsigned int _AbstandNumFaktor, unsigned int _AbstandNumMin, unsigned int _AbstandNumMax,
	unsigned int _AbstandNumMinErstesElement, unsigned int _AbstandNumMaxErstesElement,	bool _ObjektAn1, CZufallsGenerator* _ZufallsGenerator)
{
	// neues Detailobjekt erstellen
	SSplineDetailobjekt* neuesDetailobjekt = new SSplineDetailobjekt();
	// konstante Werte zuweisen
	neuesDetailobjekt->Name = _Name;
	neuesDetailobjekt->ObjektMesh = _ObjektMesh;
	neuesDetailobjekt->Skalierung = _Skalierung;
	
	// Matrix der Basisrotation aufstellen
	irr::core::CMatrix4<double> basisRotationsMatrix;
	basisRotationsMatrix.makeIdentity();
	basisRotationsMatrix.setRotationDegrees(_Rotation);
	
	// temporaere Werte fuer Position und Rotation
	irr::core::vector3d<double> position;
	irr::core::vector3d<double> rotation;
	irr::core::CMatrix4<double> splineRotationsMatrix;

	// Objekt entlang des Splines positionieren und ausrichten
	// zu erreichendes Vielfaches des Abstands
	unsigned int abstandsFaktor = _AbstandNumFaktor*_ZufallsGenerator->GeneriereZufallsWert(_AbstandNumMinErstesElement,_AbstandNumMaxErstesElement);
	// Splineparameter t
	double t = 0.0;
	// Position und Ableitung fuer aktuellen t-Wert
	irr::core::vector3d<double> aktuellePosition = BestimmePosition(t);
	irr::core::vector3d<double> aktuelleAbleitung;
	// lokales Koordinatensystem: aehnlich dem Frenetschen Koordinatensystem
	irr::core::vector3d<double> vorn;
	irr::core::vector3d<double> links;
	irr::core::vector3d<double> oben;

	// Wenn passender Abstand erreicht:
	if (abstandsFaktor==0)
	{
		aktuelleAbleitung = BestimmeAbleitung(t);	
		vorn = aktuelleAbleitung.normalize();
		links = vorn.crossProduct(ObenStandard);	// linkshaendiges System!
		links.normalize(); // normalisieren da Ausgangsvektoren nicht zwangslaufig senkrecht aufeinander standen
		oben = links.crossProduct(vorn);

		// resultierende Position: (-links,oben)*(X,Y) bzw. (rechts,oben)*(X,Y)
		position = aktuellePosition - _Position.X*links + _Position.Y*oben;
		neuesDetailobjekt->Position.push_back(position);

		// Winkel zwischen (0,0,1) und vorn ermitteln
		rotation = vorn.getHorizontalAngle();
		// Rotationsmatrix dazu aufstellen
		splineRotationsMatrix.makeIdentity();
		splineRotationsMatrix.setRotationDegrees(rotation);
		// Basisrotation und Splinerotation zusammenfassen
		rotation = (splineRotationsMatrix*basisRotationsMatrix).getRotationDegrees();
		neuesDetailobjekt->Rotation.push_back(rotation);

		// neuen Abstandsfaktor im angegebenen Bereich auswuerfeln
		abstandsFaktor = _AbstandNumFaktor*_ZufallsGenerator->GeneriereZufallsWert(_AbstandNumMin,_AbstandNumMax);
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Spline weiter fortfuehren
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Spline ablaufen
	irr::core::vector2d<double> ermittleTResultat;
	while (t < 1.0)
	{
		// aktualisieren
		ermittleTResultat = ErmittleT(aktuellePosition, t, _AbstandsAbtastung); // Berechnung von (t,Abstand) aus letzter Position und dem gewuenschten Abstand
		t = ermittleTResultat.X;
		aktuellePosition = BestimmePosition(t);
		abstandsFaktor--;

		// wenn nicht genuegend Abstand: Abbruch (kann beim letzten t auftreten)
		if ((ermittleTResultat.Y < (_AbstandsAbtastung-Genauigkeit)) && (!_ObjektAn1))
			break;

		// Wenn passender Abstand erreicht:
		if ((abstandsFaktor==0)||(t==1.0))
		{
			aktuelleAbleitung = BestimmeAbleitung(t);
			vorn = aktuelleAbleitung.normalize();
			links = vorn.crossProduct(ObenStandard);	// linkshaendiges System!
			links.normalize(); // normalisieren da Ausgangsvektoren nicht zwangslaufig senkrecht aufeinander standen
			oben = links.crossProduct(vorn);

			// resultierende Position: (-links,oben)*(X,Y) bzw. (rechts,oben)*(X,Y)
			position = aktuellePosition - _Position.X*links + _Position.Y*oben;
			neuesDetailobjekt->Position.push_back(position);

			// Winkel zwischen (0,0,1) und vorn ermitteln
			rotation = vorn.getHorizontalAngle();
			// Rotationsmatrix dazu aufstellen
			splineRotationsMatrix.makeIdentity();
			splineRotationsMatrix.setRotationDegrees(rotation);
			// Basisrotation und Splinerotation zusammenfassen
			rotation = (splineRotationsMatrix*basisRotationsMatrix).getRotationDegrees();
			neuesDetailobjekt->Rotation.push_back(rotation);

			// neuen Abstandsfaktor im angegebenen Bereich auswuerfeln
			abstandsFaktor = _AbstandNumFaktor*_ZufallsGenerator->GeneriereZufallsWert(_AbstandNumMin,_AbstandNumMax);
		}
	}
	
	// Detailobjekt speichern
	Detailobjekte.push_back(neuesDetailobjekt);
}

// ======================================================
// Funktionen fuer Sichtbarkeitberechnungen
// ======================================================

// Testet ob eine Region of Interest das ausserhalb des Vieving Volumes liegt -> ja bedeutet blickdicht
inline bool DunGen::CSpline::IstBlickdicht(SSplineViewingVolume* _ViewingVolume, SSplineViewRegionOfInterest* _RegionOfInterest)
{
	irr::core::vector2d<double> tempVec2;
	irr::core::vector3d<double> tempVec3;

	// roiBB ... projizierte BB der ROI
	irr::core::rect<double> roiBB;
		
	// BB RegionOfInterest auf Ebene des ViewingVolume projizieren
	// dazu alle 4 Eckpunkte projieren und zur VV-BB hinzufuegen
	tempVec3 = -_RegionOfInterest->QuerschnittBB.UpperLeftCorner.X * _RegionOfInterest->Links + _RegionOfInterest->QuerschnittBB.UpperLeftCorner.Y * _RegionOfInterest->Oben;	// als 3D-Vektor
	tempVec3 += _RegionOfInterest->Position - _ViewingVolume->Bezugspunkt;
	tempVec2.X = -tempVec3.dotProduct(_ViewingVolume->Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(_ViewingVolume->Oben);
	roiBB.UpperLeftCorner = tempVec2; // Erster Pkt. der roiBB
	roiBB.LowerRightCorner = tempVec2;

	tempVec3 = -_RegionOfInterest->QuerschnittBB.LowerRightCorner.X * _RegionOfInterest->Links + _RegionOfInterest->QuerschnittBB.LowerRightCorner.Y * _RegionOfInterest->Oben;	// als 3D-Vektor
	tempVec3 += _RegionOfInterest->Position - _ViewingVolume->Bezugspunkt;
	tempVec2.X = -tempVec3.dotProduct(_ViewingVolume->Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(_ViewingVolume->Oben);
	roiBB.addInternalPoint(tempVec2);	// Hinzufuegen

	tempVec3 = -_RegionOfInterest->QuerschnittBB.UpperLeftCorner.X * _RegionOfInterest->Links + _RegionOfInterest->QuerschnittBB.LowerRightCorner.Y * _RegionOfInterest->Oben;	// als 3D-Vektor
	tempVec3 += _RegionOfInterest->Position - _ViewingVolume->Bezugspunkt;
	tempVec2.X = -tempVec3.dotProduct(_ViewingVolume->Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(_ViewingVolume->Oben);
	roiBB.addInternalPoint(tempVec2);	// Hinzufuegen

	tempVec3 = -_RegionOfInterest->QuerschnittBB.LowerRightCorner.X * _RegionOfInterest->Links + _RegionOfInterest->QuerschnittBB.UpperLeftCorner.Y * _RegionOfInterest->Oben;	// als 3D-Vektor
	tempVec3 += _RegionOfInterest->Position - _ViewingVolume->Bezugspunkt;
	tempVec2.X = -tempVec3.dotProduct(_ViewingVolume->Links);	// Projektion
	tempVec2.Y = tempVec3.dotProduct(_ViewingVolume->Oben);
	roiBB.addInternalPoint(tempVec2);	// Hinzufuegen
	
	// Querschnitt ist nun vollstaendig projiziert -> Test auf Ueberschneidung
	return (!roiBB.isRectCollided(_ViewingVolume->ViewingVolumeBB));	
}

void DunGen::CSpline::VisibilityTest(SSplineQuerschnitt _QuerSchnitt, SSplineAndockStelle _AndockStelle1, SSplineAndockStelle _AndockStelle2)
{	
	std::cout << "[Spline]: Starte VisTest mit #VisROIs = "  << ViewTestRegionen.size()+2;	// gefundenen ROIs+Andockstellen
	DefinitivBlickdicht = false;

	// Ausmasse des Splinequerschnitts
	irr::core::rect<double> splineQuerschnittBB;
	// Regions of Interests fuer View-Tests
	SSplineViewRegionOfInterest rOIAndockstelle[2];				// Andockstelle 1 und 2
	
	// SplineQuerschnittBB berechnen
	splineQuerschnittBB.UpperLeftCorner.X = 0.0;
	splineQuerschnittBB.UpperLeftCorner.Y = 0.0;
	splineQuerschnittBB.LowerRightCorner.X = 0.0;
	splineQuerschnittBB.LowerRightCorner.Y = 0.0;
	for (unsigned int i=0; i<_QuerSchnitt.Punkt.size(); i++)
	{
		// upperleft: kleinste Werte (Irrlicht Def. fuer valides Rect)
		if (_QuerSchnitt.Punkt.at(i).X < splineQuerschnittBB.UpperLeftCorner.X)
			splineQuerschnittBB.UpperLeftCorner.X = _QuerSchnitt.Punkt.at(i).X;
		if (_QuerSchnitt.Punkt.at(i).Y < splineQuerschnittBB.UpperLeftCorner.Y)
			splineQuerschnittBB.UpperLeftCorner.Y = _QuerSchnitt.Punkt.at(i).Y;
		
		// lowerright: groesste Werte
		if (_QuerSchnitt.Punkt.at(i).X > splineQuerschnittBB.LowerRightCorner.X)
			splineQuerschnittBB.LowerRightCorner.X = _QuerSchnitt.Punkt.at(i).X;
		if (_QuerSchnitt.Punkt.at(i).Y > splineQuerschnittBB.LowerRightCorner.Y)
			splineQuerschnittBB.LowerRightCorner.Y = _QuerSchnitt.Punkt.at(i).Y;
	}

	// Spline-Querschnitt zu ViewTestRegionen uebertragen
	for (unsigned int i=0; i<ViewTestRegionen.size(); i++)
		ViewTestRegionen.at(i).QuerschnittBB = splineQuerschnittBB;

	// ROIs der Andockstellen berechnen
	irr::core::vector3d<double> vorn;
	SSplineAndockStelle* andockStelle[2];
	andockStelle[0] = &_AndockStelle1;
	andockStelle[1] = &_AndockStelle2;
	irr::core::vector2d<double> tempVec2d;		

	for (unsigned int i=0; i<2; i++)
	{
		vorn = andockStelle[i]->Normale;
		rOIAndockstelle[i].Links = (vorn.crossProduct(ObenStandard)).normalize(); // normalisieren da Ausgangsvektoren nicht zwangslaufig senkrecht aufeinander standen
		rOIAndockstelle[i].Oben = (rOIAndockstelle[i].Links).crossProduct(vorn);
		if (i==0)
			rOIAndockstelle[i].Position = andockStelle[i]->Mittelpunkt + andockStelle[i]->Ausdehnung*andockStelle[i]->Normale;	// am inneren Ende der ADS
		else
			rOIAndockstelle[i].Position = andockStelle[i]->Mittelpunkt - andockStelle[i]->Ausdehnung*andockStelle[i]->Normale;	// am inneren Ende der ADS
		
		rOIAndockstelle[i].QuerschnittBB.UpperLeftCorner.X = 0.0;
		rOIAndockstelle[i].QuerschnittBB.UpperLeftCorner.Y = 0.0;
		rOIAndockstelle[i].QuerschnittBB.LowerRightCorner.X = 0.0;
		rOIAndockstelle[i].QuerschnittBB.LowerRightCorner.Y = 0.0;

		for (unsigned int j=0; j<andockStelle[i]->Punkt.size(); j++)
		{
			// Querschnittberechnung -> Punkte muessen projiziert werden
			//tempVec2d = ProjiziereVektorAufEbene(andockStelle[i]->Punkt.at(j) - rOIAndockstelle[i].Position, rOIAndockstelle[i].Links, rOIAndockstelle[i].Oben);
			tempVec2d = ProjiziereVektorAufEbene(andockStelle[i]->Punkt.at(j) - andockStelle[i]->Mittelpunkt, rOIAndockstelle[i].Links, rOIAndockstelle[i].Oben);

			// upperleft: kleinste Werte (Irrlicht Def. fuer valides Rect)
			if (tempVec2d.X < rOIAndockstelle[i].QuerschnittBB.UpperLeftCorner.X)
				rOIAndockstelle[i].QuerschnittBB.UpperLeftCorner.X = tempVec2d.X;

			if (tempVec2d.Y < rOIAndockstelle[i].QuerschnittBB.UpperLeftCorner.Y)
				rOIAndockstelle[i].QuerschnittBB.UpperLeftCorner.Y = tempVec2d.Y;
		
			// lowerright: groesste Werte
			if (tempVec2d.X > rOIAndockstelle[i].QuerschnittBB.LowerRightCorner.X)
				rOIAndockstelle[i].QuerschnittBB.LowerRightCorner.X = tempVec2d.X;

			if (tempVec2d.Y > rOIAndockstelle[i].QuerschnittBB.LowerRightCorner.Y)
				rOIAndockstelle[i].QuerschnittBB.LowerRightCorner.Y = tempVec2d.Y;		
		}

		// da Adapter zwischen Splinequerschnitt und Andockstellenquerschnitt interpoliert, muss die BB beide umfassen
		rOIAndockstelle[i].QuerschnittBB.addInternalPoint(splineQuerschnittBB.LowerRightCorner);
		rOIAndockstelle[i].QuerschnittBB.addInternalPoint(splineQuerschnittBB.UpperLeftCorner);
	}

	// Viewtest-Regionen zusammenfassen
	std::vector<SSplineViewRegionOfInterest*> viewTestRegionenKomplett;
	viewTestRegionenKomplett.push_back(&rOIAndockstelle[0]);
	for (unsigned int i=0; i<ViewTestRegionen.size(); i++)
		viewTestRegionenKomplett.push_back(&ViewTestRegionen.at(i));
	viewTestRegionenKomplett.push_back(&rOIAndockstelle[1]);

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Viewing-Volumes erzeugen
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	std::vector<SSplineViewingVolume*> viewingVolumes;
	// Vergleichsindexbereich (alle Vergleichsobjekte muessen zwischen den beiden Enden des Viewing Volumes liegen)
	std::vector<int> testIndexStart;
	std::vector<int> testIndexEnde;
	for (int i=0; i<viewTestRegionenKomplett.size(); i++)
		for (int j=i+2; j<viewTestRegionenKomplett.size(); j++)	// mind. eine Region dazwischen frei
		{
			viewingVolumes.push_back(new SSplineViewingVolume(viewTestRegionenKomplett.at(i), viewTestRegionenKomplett.at(j)) );
			// alle Regionen zwischen den "Eltern"-Regionen testen
			testIndexStart.push_back(i+1);
			testIndexEnde.push_back(j-1);
		}
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Tests durchfuehren
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	
	for (int i=0; i<viewingVolumes.size(); i++)
		for (int j=testIndexStart.at(i); j<=testIndexEnde.at(i); j++)
			DefinitivBlickdicht = DefinitivBlickdicht || IstBlickdicht(viewingVolumes.at(i),viewTestRegionenKomplett.at(j));

	// Loeschen der neu-erzeugten Objekte
	for (int i=0; i<viewingVolumes.size(); i++)
		delete viewingVolumes.at(i);
		
	std::cout << " ... abgeschlossen. Blickdicht = "  << DefinitivBlickdicht << std::endl;

	/*/ TESTMARKIERUNG
	if (!DefinitivBlickdicht)
	{
		MeshAdapter[0] = new irr::scene::SMesh();
		MeshAdapter[0]->recalculateBoundingBox();
		MeshAdapter[1] = new irr::scene::SMesh();
		MeshAdapter[1]->recalculateBoundingBox();	
	}
	//*/
}