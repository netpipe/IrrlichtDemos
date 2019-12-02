#include "Spline.h"

// ======================================================
// Berechnung von t, Position und Ableitung
// ======================================================

// Berechnung der Position zu gegebenem T
inline irr::core::vector3d<double> DunGen::CSpline::BestimmePosition(double _T)
{
	return (_T*_T*_T* PositionsKoeffizienten[0] + _T*_T* PositionsKoeffizienten[1] + _T* PositionsKoeffizienten[2] + PositionsKoeffizienten[3]);
}

// Berechnung der Ableitung zu gegebenem T
inline irr::core::vector3d<double> DunGen::CSpline::BestimmeAbleitung(double _T)
{
	return (_T*_T* AbleitungsKoeffizienten[0] + _T* AbleitungsKoeffizienten[1] + AbleitungsKoeffizienten[2]);
}

// Berechnung des naechsten t-Werts, fuer Punkt in angegebenem Abstand
// Berechnung per Bisektionsverfahren
// Rueckgabe: (t,Abstand)
inline irr::core::vector2d<double> DunGen::CSpline::ErmittleT(irr::core::vector3d<double> _LetztePosition, double _LetztesT, double _Abstand)
{
	// Startwerte setzen
	double t = 1.0;
	irr::core::vector3d<double> neuePosition = BestimmePosition(t);
	// Quadrate des Abstands (schnellere Berechnung)
	double aktuellerAbstandQ = (neuePosition-_LetztePosition).getLengthSQ();
	double abstandQ = _Abstand*_Abstand;
	
	// Wenn Abstand zu t=1.0 (Endpunkt) schon zu klein -> kein groesserer Abstand ermittelbar
	if (aktuellerAbstandQ <= abstandQ)
		return irr::core::vector2d<double>(t,sqrt(aktuellerAbstandQ));

	// Intervallgrenzen: in ihnen wird der gewuenschte Wert fuer t gesucht
	double obereGrenze = 1.0;
	double untereGrenze = _LetztesT;	
	
	// solange iterieren bis genau genug
	while (! ( (aktuellerAbstandQ < (_Abstand+Genauigkeit)*(_Abstand+Genauigkeit))
			&& (aktuellerAbstandQ > (_Abstand-Genauigkeit)*(_Abstand-Genauigkeit)) )
			//&& (obereGrenze-untereGrenze > Toleranz) )	// relevant, falls richtiges t durch numerische Ungenauigkeit uebersprungen wird
			)
	{
		// Abstand zu gross -> Abstand verringern, also untere Intervallhaelfte weiterverwenden
		if (aktuellerAbstandQ>abstandQ)
			obereGrenze = t;
		// Abstand zu klein -> Abstand vergroessern, also obere Intervallhaelfte weiterverwenden
		else
			untereGrenze = t;
		
		// Werte aktualisieren:
		// t ist Mittelwert aus oberer Grenze und unterer Grenze
		t = untereGrenze+(obereGrenze-untereGrenze)/2;
		
		// neue Position und Abstand bestimmen
		neuePosition = BestimmePosition(t);
		aktuellerAbstandQ = (neuePosition-_LetztePosition).getLengthSQ();
	}

	// Ergebnis zurueckliefern
	return irr::core::vector2d<double>(t,_Abstand);
}

// ======================================================
// Hilfsfunktionen
// ======================================================

// Berechnung des Interpolationsfaktors fuer Position P neuer Vertices in Andockstelle, P liegt auf AB vom Dreieck AMB
inline double DunGen::CSpline::BerechneInterpolationsFaktorPosition(irr::core::vector2d<double> _MA, irr::core::vector2d<double> _MB, double _WinkelAMP)
{
	double ma = _MA.getLength();
	double mb = _MB.getLength();
	irr::core::vector2d<double> vecAB = _MB-_MA;
	double ab = (vecAB).getLength();

	if (ma == 0 || ab == 0)
		return 0;
	
	// Winkel berechnen durch Skalarprodukt: 0 bis PI
	double winkelBAM = acos(-_MA.dotProduct(vecAB) / (ma * ab));
	// Innenwinkelsatz Dreieck
	double winkelMPA = M_PI - winkelBAM - _WinkelAMP;
	// Berechnung durch Sinussatz
	double ap = sin(_WinkelAMP)/sin(winkelMPA) * ma;

	return ap/ab;
}

// Projiziere einen Vektor auf eine Ebene mit gegebenen Links und Oben Vektor (beide normiert)
// Rueckgabe: (X,Y)-Koordinaten in der Ebene mit Xpos ... -links, Ypos ... oben
inline irr::core::vector2d<double>  DunGen::CSpline::ProjiziereVektorAufEbene(irr::core::vector3d<double> _Vektor, irr::core::vector3d<double> _Links, irr::core::vector3d<double> _Oben)
{
	return irr::core::vector2d<double>(-_Vektor.dotProduct(_Links),_Vektor.dotProduct(_Oben));
}


// ======================================================
// Erstellung des Splines
// ======================================================

// Erstellung des Dreiecksnetzes des Meshes
void DunGen::CSpline::ErstelleSpline(SSplineQuerschnitt _QuerSchnitt, SSplineAndockStelle _AndockStelle1, SSplineAndockStelle _AndockStelle2,
	double _Abstand, double _AbstandTexturYProAbstand1, unsigned int _Detailstufe)
{
	// Splineparameter t
	double t;
	// Position und Ableitung fuer aktuellen t-Wert
	irr::core::vector3d<double> aktuellePosition;
	irr::core::vector3d<double> aktuelleAbleitung;
	// lokales Koordinatensystem: aehnlich dem Frenetschen Koordinatensystem
	irr::core::vector3d<double> vorn;
	irr::core::vector3d<double> links;
	irr::core::vector3d<double> oben;

	// Interessante Regionen fuer Visibility-Test: moeglichst hoher Abstand zu Achse P[1]-P[0] -> lok. Maxima (es sollte genau 1 oder 2 geben)
	irr::core::vector3d<double> roiAchse = (Position[1]-Position[0]).normalize();
	SSplineViewRegionOfInterest aktRegionOfInterest;
	double aktAbstandSQ = 0.0;
	double letzterAbstandSQ;
	bool roiGemerkt = false;
	// letzter eingefuegter Index der Regions Of Interest (fuer nicht Original-Detailstufen)
	unsigned int aktIndexROI = 0;

	// Sicherheitslimit fuer Meshbuffer: ein Querschnittssegment
	unsigned int resMaxAnzahlVertices = MaxAnzahlVertices - _QuerSchnitt.Punkt.size();
	// neuen Mesh erstellen
	irr::scene::SMesh* mesh = new irr::scene::SMesh();

	// ersten Buffer erstellen
	irr::scene::SMeshBuffer* meshBuffer = new irr::scene::SMeshBuffer();
	// hinzufuegen
	mesh->addMeshBuffer(meshBuffer);
	meshBuffer->drop(); // droppen, da nun am Mesh haengend -> wird beim Loeschen des Meshs mit geloescht	
	// erstmal fuer den worst-case reservieren
	meshBuffer->Vertices.set_used(MaxAnzahlVertices); 
	meshBuffer->Indices.set_used(6*MaxAnzahlVertices);	// 6 Indices pro Vertex	(2 Dreiecke je Vertex)	
	unsigned int bufferVertices = 0;
	unsigned int bufferIndizes = 0;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// erstes Spline-Segment hinzufuegen
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	double aktuelleTexturKoordinateY = 0;
	irr::core::vector3d<double> temp;

	t = 0.0;
	// Position und Ableitung fuer aktuellen t-Wert
	aktuellePosition = Position[0];
	aktuelleAbleitung = Ableitung[0];
	// lokales Koordinatensystem errechnen: aehnlich dem Frenetschen Koordinatensystem
	vorn = aktuelleAbleitung.normalize();
	links = vorn.crossProduct(ObenStandard);	// linkshaendiges System!
	links.normalize(); // normalisieren da Ausgangsvektoren nicht zwangslaeufig senkrecht aufeinander standen
	oben = links.crossProduct(vorn);

	// fuer jeden Punkt des Querschnitts:
	for (unsigned int i = 0; i<_QuerSchnitt.Punkt.size(); i++)
	{
		// neuen Vertex erstellen und zum Buffer hinzufuegen: (-links,oben)*(X,Y) bzw. (rechts,oben)*(X,Y)
		temp = aktuellePosition - _QuerSchnitt.Punkt.at(i).X*links + _QuerSchnitt.Punkt.at(i).Y*oben;
		irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices++];
		v.Pos.set(temp.X,temp.Y,temp.Z);
		
		// Normale berechnen
		temp = aktuellePosition - temp;
		temp.normalize();
		v.Normal.set(temp.X,temp.Y,temp.Z);

		// Texturkoordinaten einstellen
		v.TCoords.set(_QuerSchnitt.TextureX.at(i),aktuelleTexturKoordinateY);
	}

	// erstes Segment ist auch eine Region of Interest
	if(_Detailstufe==0)
	{
		aktRegionOfInterest.Position = aktuellePosition;
		aktRegionOfInterest.Links = links;
		aktRegionOfInterest.Oben = oben;
		ViewTestRegionen.push_back(aktRegionOfInterest);
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Spline weiter fortfuehren
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	unsigned int numSegmente = 0;
	// Abstastrate = 2^_Detailstufe
	unsigned int abtastRate = 1;
	for (unsigned int i=0; i<_Detailstufe;i++)
		abtastRate += abtastRate;
	bool roiErreicht;

	// Spline ablaufen
	irr::core::vector2d<double> ermittleTResultat;
	while (t < 1.0)
	{
		numSegmente++;
		// aktualisieren
		ermittleTResultat = ErmittleT(aktuellePosition, t, _Abstand); // Berechnung von (t,Abstand) aus letzter Position und dem gewuenschten Abstand
		t = ermittleTResultat.X;
		aktuelleTexturKoordinateY+= ermittleTResultat.Y * _AbstandTexturYProAbstand1; // Y-Texturkoordinate weiterzaehlen
		aktuellePosition = BestimmePosition(t);

		// Test ob ROI erreicht -> Segment muss eingefuegt werden
		roiErreicht = false;
		if ( (_Detailstufe!=0) && (aktIndexROI<ROINumAbtast.size()) )
			if (numSegmente==ROINumAbtast.at(aktIndexROI))
			{
				roiErreicht = true;
				aktIndexROI++;
			}
		
		// Segment einfuegen?
		if ((numSegmente%abtastRate==0) || (t==1.0) || roiErreicht)
		{
			aktuelleAbleitung = BestimmeAbleitung(t);
			vorn = aktuelleAbleitung.normalize();
			links = vorn.crossProduct(ObenStandard);	// linkshaendiges System!
			links.normalize(); // normalisieren da Ausgangsvektoren nicht zwangslaeufig senkrecht aufeinander standen
			oben = links.crossProduct(vorn);

			// Original-Detailstufe: Regions of Interest suchen und merken
			if(_Detailstufe==0)
			{
				// Test auf Region of Interest bzgl. Visibility-Test: moeglichst hoher Abstand von Achse P[1]-P[0]
				letzterAbstandSQ = aktAbstandSQ;
				temp = aktuellePosition-Position[0];
				aktAbstandSQ = (temp - temp.dotProduct(roiAchse)*roiAchse).getLengthSQ();
				// Abstand groesser geworden -> Daten merken
				if (aktAbstandSQ>letzterAbstandSQ)
				{
					aktRegionOfInterest.Position = aktuellePosition;
					aktRegionOfInterest.Links = links;
					aktRegionOfInterest.Oben = oben;
					roiGemerkt = false;
				}
				// Abstand wird wieder kleiner -> Region of Interest speichern
				else if (!roiGemerkt)
				{
					// Daten des letzten Segments werden verwendet: also genau die des lokalen Maximums
					ViewTestRegionen.push_back(aktRegionOfInterest);
					ROINumAbtast.push_back(numSegmente-1);
					roiGemerkt = true;
				}
			}

			// fuer jeden Punkt des Querschnitts:
			for (unsigned int i = 0; i<_QuerSchnitt.Punkt.size(); i++)
			{
				// neuen Vertex erstellen und zum Buffer hinzufuegen: (-links,oben)*(X,Y) bzw. (rechts,oben)*(X,Y)
				temp = aktuellePosition - _QuerSchnitt.Punkt.at(i).X*links + _QuerSchnitt.Punkt.at(i).Y*oben;
				irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices++];
				v.Pos.set(temp.X,temp.Y,temp.Z);
		
				// Normale berechnen
				temp = aktuellePosition - temp;
				temp.normalize();
				v.Normal.set(temp.X,temp.Y,temp.Z);

				// Texturkoordinaten einstellen
				v.TCoords.set(_QuerSchnitt.TextureX.at(i),aktuelleTexturKoordinateY);
			}
		
			// Dreiecke erstellen:
			// Indices gegen UZS fuer korrekte Face-Normalen, ABER noch Umrechnung in Irrlicht-Koord.sys. also im USZ
			// => im UZS (vom Innern des Schlauches aus gesehen) -> gegen UZS von aussen

			// Verbindung von Anfang und Ende:
			// Dreieck 1
			meshBuffer->Indices[bufferIndizes++] = bufferVertices - _QuerSchnitt.Punkt.size() - 1;	// letzter Punkt des vorherigen Querschnitts
			meshBuffer->Indices[bufferIndizes++] = bufferVertices - _QuerSchnitt.Punkt.size();		// erster Punkt des jetzigen Querschnitts
			meshBuffer->Indices[bufferIndizes++] = bufferVertices - 1;								// letzer Punkt des jetzigen Querschnitts

			// Dreieck 2
			meshBuffer->Indices[bufferIndizes++] = bufferVertices - _QuerSchnitt.Punkt.size() - 1;	// letzter Punkt des vorherigen Querschnitts
			meshBuffer->Indices[bufferIndizes++] = bufferVertices - 2*_QuerSchnitt.Punkt.size();	// erster Punkt des vorherigen Querschnitts
			meshBuffer->Indices[bufferIndizes++] = bufferVertices - _QuerSchnitt.Punkt.size();		// erster Punkt des jetzigen Querschnitts
	
			// Rest:
			for (unsigned int i = 1; i<_QuerSchnitt.Punkt.size(); i++)
			{
				// Dreieck 1
				meshBuffer->Indices[bufferIndizes++] = bufferVertices - 2*_QuerSchnitt.Punkt.size() + i - 1;
				meshBuffer->Indices[bufferIndizes++] = bufferVertices - _QuerSchnitt.Punkt.size() + i;
				meshBuffer->Indices[bufferIndizes++] = bufferVertices - _QuerSchnitt.Punkt.size() + i - 1;			

				// Dreieck 2
				meshBuffer->Indices[bufferIndizes++] = bufferVertices - 2*_QuerSchnitt.Punkt.size() + i - 1;
				meshBuffer->Indices[bufferIndizes++] = bufferVertices - 2*_QuerSchnitt.Punkt.size() + i;
				meshBuffer->Indices[bufferIndizes++] = bufferVertices - _QuerSchnitt.Punkt.size() + i;			
			}

			// Falls Meshbuffer zu voll:
			if (bufferVertices > resMaxAnzahlVertices && t < 1.0)
			{
				//  alter Buffer: finale Buffergroesse setzen
				meshBuffer->Vertices.reallocate(bufferVertices);
				meshBuffer->Indices.reallocate(bufferIndizes);
				// alter Buffer: Bounding Box berechnen
				meshBuffer->recalculateBoundingBox();

				// neuer Buffer: erstellen
				meshBuffer = new irr::scene::SMeshBuffer();
				bufferVertices = 0;	
				bufferIndizes = 0;
				// neuer Buffer: hinzufuegen
				mesh->addMeshBuffer(meshBuffer);
				meshBuffer->drop();
				// neuer Buffer: erstmal fuer den worst-case reservieren
				meshBuffer->Vertices.set_used(MaxAnzahlVertices);
				meshBuffer->Indices.set_used(6*MaxAnzahlVertices);

				// Vertices des letzten Segmentes erneut hinzufuegen:
				// fuer jeden Punkt des Querschnitts:
				for (unsigned int i = 0; i<_QuerSchnitt.Punkt.size(); i++)
				{
					// neuen Vertex erstellen und zum Buffer hinzufuegen: (-links,oben)*(X,Y) bzw. (rechts,oben)*(X,Y)
					temp = aktuellePosition - _QuerSchnitt.Punkt.at(i).X*links + _QuerSchnitt.Punkt.at(i).Y*oben;
					irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices++];
					v.Pos.set(temp.X,temp.Y,temp.Z);
		
					// Normale berechnen
					temp = aktuellePosition - temp;
					temp.normalize();
					v.Normal.set(temp.X,temp.Y,temp.Z);

					// Texturkoordinaten einstellen
					v.TCoords.set(_QuerSchnitt.TextureX.at(i),aktuelleTexturKoordinateY);
				}
			}
			
		} // END: if (numSegmente%abtastRate==0)
	} // END: while

	// finale Werte des Meshbuffers errechnen
	meshBuffer->Vertices.reallocate(bufferVertices);
	meshBuffer->Indices.reallocate(bufferIndizes);
	meshBuffer->recalculateBoundingBox();

	// finale Werte des Meshes errechen
	mesh->recalculateBoundingBox();	
	// Mesh merken
	MeshSpline.push_back(mesh);

	// Adapter erstellen
	if (_Detailstufe == 0)
	{
		// letztes Segment ist auch eine Region of Interest
		aktRegionOfInterest.Position = aktuellePosition;
		aktRegionOfInterest.Links = links;
		aktRegionOfInterest.Oben = oben;
		ViewTestRegionen.push_back(aktRegionOfInterest);

		// Adapter erstellen
		ErstelleAdapter(0,_AndockStelle1,_QuerSchnitt, 0, _AbstandTexturYProAbstand1);
		ErstelleAdapter(1,_AndockStelle2,_QuerSchnitt, aktuelleTexturKoordinateY, _AbstandTexturYProAbstand1);		
	}
}

// ======================================================
// Erstellung der Adapter
// ======================================================

void DunGen::CSpline::ErstelleAdapter(unsigned int _Ende, SSplineAndockStelle _Andockstelle, SSplineQuerschnitt _QuerSchnitt, double _LetztesTextureY, double _AbstandTexturYProAbstand1)
{
	// neuen Mesh erstellen
	MeshAdapter[_Ende] = new irr::scene::SMesh();
	
	// wenn Adockstelle ohne Punkte -> keine Andockstelle gewuenscht -> Abbruch
	if (_Andockstelle.Punkt.size() == 0)
	{
		MeshAdapter[_Ende]->recalculateBoundingBox();
		return;
	}
	
	// ersten Buffer erstellen
	irr::scene::SMeshBuffer* meshBuffer = new irr::scene::SMeshBuffer();
	// hinzufuegen
	MeshAdapter[_Ende]->addMeshBuffer(meshBuffer);
	meshBuffer->drop(); // droppen, da nun am Mesh haengend -> wird beim Loeschen des Meshs mit geloescht	
	// erstmal fuer den worst-case reservieren
	meshBuffer->Vertices.set_used(MaxAnzahlVertices); 
	meshBuffer->Indices.set_used(3*MaxAnzahlVertices);	// 3 Indices pro Vertex	(fuer jeden Vertex ein Dreieck)
	unsigned int bufferVertices = 0;
	unsigned int bufferIndizes = 0;

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Splinesegment vorbereiten
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	// Position und Ableitung
	irr::core::vector3d<double> aktuellePosition = Position[_Ende];
	irr::core::vector3d<double> aktuelleAbleitung = Ableitung[_Ende];
	// lokales Koordinatensystem errechnen: aehnlich dem Frenetschen Koordinatensystem
	irr::core::vector3d<double> vorn = aktuelleAbleitung.normalize();
	irr::core::vector3d<double> links = vorn.crossProduct(ObenStandard);	// linkshaendiges System!
	links.normalize(); // normalisieren da Ausgangsvektoren nicht zwangslaufig senkrecht aufeinander standen
	irr::core::vector3d<double>	oben = links.crossProduct(vorn);

	double winkel1, winkel2;
	unsigned int startIndex, aktuellerIndex;

	// Winkel fuer die Punkte berechnen: Winkel im UZS, 0 Grad: links-Vektor
	std::vector<double> winkelReihenfolge;
	for (unsigned int i = 0; i<_QuerSchnitt.Punkt.size(); i++)
	{
		// atan2: -PI bis PI
		winkel1 = atan2( _QuerSchnitt.Punkt.at(i).Y,- _QuerSchnitt.Punkt.at(i).X);
		// umwandeln nach 0 bis 2*PI
		//if (winkel1 < 0) winkel1 = 2*M_PI + winkel1;
		winkelReihenfolge.push_back(winkel1);
	}
		
	// Neu Sortieren: Index 0 -> kleinster Winkel
	// Startindex bestimmen	
	winkel1 = 3*M_PI;
	for (unsigned int i = 0; i<winkelReihenfolge.size(); i++)
		if (winkelReihenfolge.at(i)<winkel1)
		{
			winkel1 = winkelReihenfolge.at(i);
			startIndex = i;
		};

	// Neu erstellen, vom Startindex angefangen
	SSplineQuerschnitt querschnittSplineSortiert;
	std::vector<double> winkelSplinesegmentSortiert;
	
	// Anfangsindex
	aktuellerIndex = startIndex;
	querschnittSplineSortiert.Punkt.push_back(_QuerSchnitt.Punkt.at(aktuellerIndex));
	querschnittSplineSortiert.TextureX.push_back(_QuerSchnitt.TextureX.at(aktuellerIndex));
	winkelSplinesegmentSortiert.push_back(winkelReihenfolge.at(aktuellerIndex));
	aktuellerIndex++;
	if (aktuellerIndex==_QuerSchnitt.Punkt.size())
		aktuellerIndex = 0;
	
	// restliche Indices
	while (aktuellerIndex != startIndex)
	{
		querschnittSplineSortiert.Punkt.push_back(_QuerSchnitt.Punkt.at(aktuellerIndex));
		querschnittSplineSortiert.TextureX.push_back(_QuerSchnitt.TextureX.at(aktuellerIndex));
		winkelSplinesegmentSortiert.push_back(winkelReihenfolge.at(aktuellerIndex));
		aktuellerIndex++;
		if (aktuellerIndex==_QuerSchnitt.Punkt.size())
			aktuellerIndex = 0;
	}

	// zugehoerige Vertices erstellen:
	// Vertices zusaetzlich mit Genauigkeit double merken, fuer einige weitere Berechnungen
	std::vector<irr::core::vector3d<double>> splineVertices;
	irr::core::vector3d<double> temp;
	// fuer jeden Punkt des Querschnitts:
	for (unsigned int i = 0; i<querschnittSplineSortiert.Punkt.size(); i++)
	{
		// neuen Vertex erstellen und zum Buffer hinzufuegen: (-links,oben)*(X,Y) bzw. (rechts,oben)*(X,Y)
		temp = aktuellePosition - querschnittSplineSortiert.Punkt.at(i).X*links + querschnittSplineSortiert.Punkt.at(i).Y*oben;
		splineVertices.push_back(temp);

		irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices++];
		v.Pos.set(temp.X,temp.Y,temp.Z);
		
		// Normale berechnen
		temp = aktuellePosition - temp;
		temp.normalize();
		v.Normal.set(temp.X,temp.Y,temp.Z);

		// Texturkoordinaten einstellen
		v.TCoords.set(querschnittSplineSortiert.TextureX.at(i),_LetztesTextureY);
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Andockstelle vorbereiten
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Koordinatensytem der Ebene: links-Vektor und oben-Vektor berechnen
	irr::core::vector3d<double> linksADS = _Andockstelle.Normale.crossProduct(ObenStandard);	// linkshaendiges System!
	linksADS.normalize(); // normalisieren da Ausgangsvektoren nicht zwangslaufig senkrecht aufeinander standen
	irr::core::vector3d<double> obenADS = linksADS.crossProduct(_Andockstelle.Normale);
	
	// Winkel fuer die Punkte berechnen: Winkel im UZS, 0 Grad: links-Vektor
	winkelReihenfolge.clear();
	for (unsigned int i = 0; i<_Andockstelle.Punkt.size(); i++)
	{
		// Bezugspunkt: Mittelpunkt
		temp = _Andockstelle.Punkt.at(i)-_Andockstelle.Mittelpunkt;
		
		// atan2: -PI bis PI
		winkel1 = atan2(temp.dotProduct(obenADS),temp.dotProduct(linksADS));	// Projizierte 2D-Koordinaten verwenden
		// umwandeln nach 0 bis 2*PI
		//if (winkel1 < 0) winkel1 = 2*M_PI + winkel1;
		winkelReihenfolge.push_back(winkel1);
	}

	// Neu Sortieren: Index 0 -> kleinster Winkel
	// Startindex bestimmen
	winkel1 = 3*M_PI;
	for (unsigned int i = 0; i<winkelReihenfolge.size(); i++)
		if (winkelReihenfolge.at(i)<winkel1)
		{
			winkel1 = winkelReihenfolge.at(i);
			startIndex = i;
		};

	// Neu erstellen, vom Startindex angefangen
	SSplineAndockStelle andockstelleSortiert;
	std::vector<double> winkelAndockstelleSortiert;
	
	// Anfangsindex
	aktuellerIndex = startIndex;
	andockstelleSortiert.Punkt.push_back(_Andockstelle.Punkt.at(aktuellerIndex));
	winkelAndockstelleSortiert.push_back(winkelReihenfolge.at(aktuellerIndex));
	aktuellerIndex++;
	if (aktuellerIndex==_Andockstelle.Punkt.size())
		aktuellerIndex = 0;
	
	// restliche Indices
	while (aktuellerIndex != startIndex)
	{
		andockstelleSortiert.Punkt.push_back(_Andockstelle.Punkt.at(aktuellerIndex));
		winkelAndockstelleSortiert.push_back(winkelReihenfolge.at(aktuellerIndex));
		aktuellerIndex++;
		if (aktuellerIndex==_Andockstelle.Punkt.size())
			aktuellerIndex = 0;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Erstellung der Zeichenanweisung des Adapters
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	std::vector<SSplineAdapterZeichenanweisung> adapterZeichenAnweisung;

	unsigned int aktuellerIndexAndockstelle = 0;
	unsigned int aktuellerIndexSplinesegment = 0;
	unsigned int letzerIndexAndockstelle = andockstelleSortiert.Punkt.size()-1;
	unsigned int letzterIndexSplinesegment = querschnittSplineSortiert.Punkt.size()-1;
	double interpolationsFaktor;

	// alle Vertices ablaufen
	bool geschlossenAndockstelle = false;
	bool geschlossenSplinesegment = false;

	// solange Schlauch nicht geschlossen:
	while(!(geschlossenAndockstelle&&geschlossenSplinesegment))
	{	

		// Welches ist der naechste Vertex? Der mit dem kleineren Winkel:
		if (
			geschlossenAndockstelle ||
			((winkelSplinesegmentSortiert.at(aktuellerIndexSplinesegment)<winkelAndockstelleSortiert.at(aktuellerIndexAndockstelle)) && !geschlossenSplinesegment)	
			)
		// Spline-Vertex
		{
			// Interpolationsfaktor bestimmen: Winkel ermitteln
			winkel1 =  winkelSplinesegmentSortiert.at(aktuellerIndexSplinesegment) - winkelAndockstelleSortiert.at(letzerIndexAndockstelle);
			//if (winkel1<0) winkel1 = 2*M_PI + winkel1;
			
			interpolationsFaktor = BerechneInterpolationsFaktorPosition(
				//  Vektor (p-m) in projizierten Koordinaten
				ProjiziereVektorAufEbene( andockstelleSortiert.Punkt.at(letzerIndexAndockstelle)-_Andockstelle.Mittelpunkt, linksADS, obenADS),
				ProjiziereVektorAufEbene( andockstelleSortiert.Punkt.at(aktuellerIndexAndockstelle)-_Andockstelle.Mittelpunkt, linksADS, obenADS),
				winkel1);
			
			// Koordinaten berechnen
			temp = (1-interpolationsFaktor) * andockstelleSortiert.Punkt.at(letzerIndexAndockstelle)
						+ interpolationsFaktor * andockstelleSortiert.Punkt.at(aktuellerIndexAndockstelle);

			// neue Zeichenanweisung mit passenden Koordinaten, Texturkoordinaten und Indexzuordnung
			SSplineAdapterZeichenanweisung neueAnweisung;
			neueAnweisung.Punkt = temp;
			neueAnweisung.korrespondierenderIndex = aktuellerIndexSplinesegment;
			neueAnweisung.Winkel = winkelSplinesegmentSortiert.at(aktuellerIndexSplinesegment);

			neueAnweisung.TexturXvonSpline = true;
			// TexturX von Spline-Punkt uebernehmen
			neueAnweisung.Textur.X = querschnittSplineSortiert.TextureX.at(aktuellerIndexSplinesegment);
			// TexturY ueber Abstand zu Splinepunkt bestimmen
			neueAnweisung.Textur.Y = (neueAnweisung.Punkt - splineVertices.at(aktuellerIndexSplinesegment)).getLength() * _AbstandTexturYProAbstand1;
			if (_Ende==0)
				neueAnweisung.Textur.Y = -neueAnweisung.Textur.Y;
			else
				neueAnweisung.Textur.Y += _LetztesTextureY;

			// Anweisung merken
			adapterZeichenAnweisung.push_back(neueAnweisung);

			// Indices weiterzaehlen
			letzterIndexSplinesegment = aktuellerIndexSplinesegment;
			aktuellerIndexSplinesegment++;
			if (aktuellerIndexSplinesegment == querschnittSplineSortiert.Punkt.size())
			{
				aktuellerIndexSplinesegment = 0;
				geschlossenSplinesegment = true;
			}

		}
		else if (geschlossenSplinesegment || (winkelAndockstelleSortiert.at(aktuellerIndexAndockstelle)<winkelSplinesegmentSortiert.at(aktuellerIndexSplinesegment)) )
		// Andockstellen-Vertex
		{
			// neue Anweisung erstellen
			SSplineAdapterZeichenanweisung neueAnweisung;
			neueAnweisung.Punkt = andockstelleSortiert.Punkt.at(aktuellerIndexAndockstelle);
			neueAnweisung.Winkel = winkelAndockstelleSortiert.at(aktuellerIndexAndockstelle);			

			// suche Index von naheliegerendem Punkt
			winkel1 = abs(winkelAndockstelleSortiert.at(aktuellerIndexAndockstelle) - winkelSplinesegmentSortiert.at(aktuellerIndexSplinesegment));
			if (winkel1>M_PI) winkel1 = 2*M_PI - winkel1; // kleinerer Winkel bzgl. 2 PI
			winkel2 = abs(winkelAndockstelleSortiert.at(aktuellerIndexAndockstelle) - winkelSplinesegmentSortiert.at(letzterIndexSplinesegment));
			if (winkel2>M_PI) winkel2 = 2*M_PI - winkel2; // kleinerer Winkel bzgl. 2 PI
			// Index dem Andockstellen-Vertex zuordnen
			if (winkel1<winkel2)
				neueAnweisung.korrespondierenderIndex = aktuellerIndexSplinesegment;
			else
				neueAnweisung.korrespondierenderIndex = letzterIndexSplinesegment;		

			// Textur-KoordinateX wird interpoliert
			neueAnweisung.TexturXvonSpline = false;
			
			// Textur-KoordinateY bestimmen:			
			// Interpolationsfaktor bestimmen: Winkel ermitteln
			winkel1 =  neueAnweisung.Winkel - winkelSplinesegmentSortiert.at(letzterIndexSplinesegment);
			//if (winkel1<0) winkel1 = 2*M_PI + winkel1;
			interpolationsFaktor = BerechneInterpolationsFaktorPosition(
				// Koordinaten sind bereits in Ebene
				querschnittSplineSortiert.Punkt.at(letzterIndexSplinesegment),
				querschnittSplineSortiert.Punkt.at(aktuellerIndexSplinesegment),
				winkel1);
			// Koordinaten des temporaeren Spline-Vertex berechnen
			temp = (1-interpolationsFaktor) * splineVertices.at(letzterIndexSplinesegment)
					+ interpolationsFaktor * splineVertices.at(aktuellerIndexSplinesegment);

			// TexturY ueber Abstand zu temporaerem Splinepunkt bestimmen
			neueAnweisung.Textur.Y = (neueAnweisung.Punkt - temp).getLength() * _AbstandTexturYProAbstand1;
			if (_Ende==0)
				neueAnweisung.Textur.Y = -neueAnweisung.Textur.Y;
			else
				neueAnweisung.Textur.Y += _LetztesTextureY;

			// Anweisung merken
			adapterZeichenAnweisung.push_back(neueAnweisung);

			// Indices weiterzaehlen
			letzerIndexAndockstelle = aktuellerIndexAndockstelle;
			aktuellerIndexAndockstelle++;	
			if (aktuellerIndexAndockstelle == andockstelleSortiert.Punkt.size())
			{
				aktuellerIndexAndockstelle = 0;
				geschlossenAndockstelle = true;
			}

		}
		else
		// Winkelgleichheit
		{
			// neue Anweisung erstellen
			SSplineAdapterZeichenanweisung neueAnweisung;
			neueAnweisung.Punkt = andockstelleSortiert.Punkt.at(aktuellerIndexAndockstelle);
			neueAnweisung.Winkel = winkelAndockstelleSortiert.at(aktuellerIndexAndockstelle);
			neueAnweisung.korrespondierenderIndex = aktuellerIndexSplinesegment;
			
			// TexturX von Spline-Punkt uebernehmen
			neueAnweisung.TexturXvonSpline = true;
			neueAnweisung.Textur.X = querschnittSplineSortiert.TextureX.at(aktuellerIndexSplinesegment);
			// TexturY ueber Abstand zu Splinepunkt bestimmen
			neueAnweisung.Textur.Y = (neueAnweisung.Punkt - splineVertices.at(aktuellerIndexSplinesegment)).getLength() * _AbstandTexturYProAbstand1;
			if (_Ende==0)
				neueAnweisung.Textur.Y = -neueAnweisung.Textur.Y;
			else
				neueAnweisung.Textur.Y += _LetztesTextureY;

			// Anweisung merken
			adapterZeichenAnweisung.push_back(neueAnweisung);

			// Indices weiterzaehlen
			letzterIndexSplinesegment = aktuellerIndexSplinesegment;
			aktuellerIndexSplinesegment++;
			if (aktuellerIndexSplinesegment == querschnittSplineSortiert.Punkt.size())
			{
				aktuellerIndexSplinesegment = 0;
				geschlossenSplinesegment = true;
			}
			// Indices weiterzaehlen
			letzerIndexAndockstelle = aktuellerIndexAndockstelle;
			aktuellerIndexAndockstelle++;	
			if (aktuellerIndexAndockstelle == andockstelleSortiert.Punkt.size())
			{
				aktuellerIndexAndockstelle = 0;
				geschlossenAndockstelle = true;
			}
		}

	} // END while

	// TexturX aktualisieren (die Werte die nicht TexturXvonSpline sind)
	double gesamtLaenge, aktuelleLaenge;
	unsigned int interpolationsIndex;

	// Texturkoordinate wird ueber auf Andockstellen-Ebene projizierten Kantenzug verteilt
	for (unsigned int i = 0; i<adapterZeichenAnweisung.size(); i++)
		if (adapterZeichenAnweisung.at(i).TexturXvonSpline)
		{
			// Gesamtlaenge des Kantenzugs zwischen 2 TexturXvonSpline's bestimmen
			gesamtLaenge = 0;
			letzerIndexAndockstelle = i;
			aktuellerIndexAndockstelle = i+1;	// Wiederverwertung der Indexvariablen von oben
			if (aktuellerIndexAndockstelle>=adapterZeichenAnweisung.size())
				aktuellerIndexAndockstelle=0;

			// naechsten Index mit TexturXvonSpline suchen
			while (!adapterZeichenAnweisung.at(aktuellerIndexAndockstelle).TexturXvonSpline)
			{
				gesamtLaenge += ProjiziereVektorAufEbene(adapterZeichenAnweisung.at(aktuellerIndexAndockstelle).Punkt-adapterZeichenAnweisung.at(letzerIndexAndockstelle).Punkt, linksADS, obenADS).getLength();
				letzerIndexAndockstelle = aktuellerIndexAndockstelle;
				aktuellerIndexAndockstelle++;
				if (aktuellerIndexAndockstelle>=adapterZeichenAnweisung.size())
					aktuellerIndexAndockstelle=0;
			}
			gesamtLaenge += ProjiziereVektorAufEbene(adapterZeichenAnweisung.at(aktuellerIndexAndockstelle).Punkt-adapterZeichenAnweisung.at(letzerIndexAndockstelle).Punkt, linksADS, obenADS).getLength();
			

			// nun TexturX im gesamten Kurvenzug interpolieren
			interpolationsIndex = aktuellerIndexAndockstelle;
			aktuelleLaenge = 0;
			letzerIndexAndockstelle = i;
			aktuellerIndexAndockstelle = i+1;
			if (aktuellerIndexAndockstelle>=adapterZeichenAnweisung.size())
				aktuellerIndexAndockstelle=0;

			// bis zum naechsten Index mit TexturXvonSpline laufen
			while (!adapterZeichenAnweisung.at(aktuellerIndexAndockstelle).TexturXvonSpline)
			{
				aktuelleLaenge += ProjiziereVektorAufEbene(adapterZeichenAnweisung.at(aktuellerIndexAndockstelle).Punkt-adapterZeichenAnweisung.at(letzerIndexAndockstelle).Punkt, linksADS, obenADS).getLength();
				
				// Interpolationsfaktor ueber Laengenverhaeltnis bestimmt
				interpolationsFaktor = aktuelleLaenge/gesamtLaenge;
				adapterZeichenAnweisung.at(aktuellerIndexAndockstelle).Textur.X = (1-interpolationsFaktor) * adapterZeichenAnweisung.at(i).Textur.X
						 + interpolationsFaktor * adapterZeichenAnweisung.at(interpolationsIndex).Textur.X;

				letzerIndexAndockstelle = aktuellerIndexAndockstelle;
				aktuellerIndexAndockstelle++;
				if (aktuellerIndexAndockstelle>=adapterZeichenAnweisung.size())
					aktuellerIndexAndockstelle=0;
			}
		};
	
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Zeichnen des Adapters
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	unsigned int letzterKorrespondierenderIndex = adapterZeichenAnweisung.at(0).korrespondierenderIndex;
	
	// neuen Vertex erstellen und zum Buffer hinzufuegen: erster Vertex der Anweisung
	irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices++];
	temp = adapterZeichenAnweisung.at(0).Punkt;
	v.Pos.set(temp.X,temp.Y,temp.Z);
		
	// Normale berechnen:
	temp = _Andockstelle.Mittelpunkt - temp;
	// auf Ebene projizieren (i.A. besseres Resultat)
	temp -= _Andockstelle.Normale.dotProduct(temp)*_Andockstelle.Normale;

	temp.normalize();
	v.Normal.set(temp.X,temp.Y,temp.Z);

	// Texturkoordinaten einstellen
	v.TCoords.set(adapterZeichenAnweisung.at(0).Textur.X, adapterZeichenAnweisung.at(0).Textur.Y);

	// restliche Vertices
	for (unsigned int i = 1; i<adapterZeichenAnweisung.size(); i++)
	{

		// neuen Vertex erstellen und zum Buffer hinzufuegen
		irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices++];
		temp = adapterZeichenAnweisung.at(i).Punkt;
		v.Pos.set(temp.X,temp.Y,temp.Z);
		
		// Normale berechnen:
		temp = _Andockstelle.Mittelpunkt - temp;
		// auf Ebene projizieren (i.A. besseres Resultat)
		temp -= _Andockstelle.Normale.dotProduct(temp)*_Andockstelle.Normale;

		temp.normalize();
		v.Normal.set(temp.X,temp.Y,temp.Z);

		// Texturkoordinaten einstellen
		v.TCoords.set(adapterZeichenAnweisung.at(i).Textur.X, adapterZeichenAnweisung.at(i).Textur.Y);

		// 1 Dreieck einfuegen?
		if (adapterZeichenAnweisung.at(i).korrespondierenderIndex == letzterKorrespondierenderIndex)
		{
			meshBuffer->Indices[bufferIndizes++] = bufferVertices-1;
					
			if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
			{
				meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(i).korrespondierenderIndex;
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-2;
			}
			else
			{
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-2;
				meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(i).korrespondierenderIndex;
			}
		}
		// 2 Dreiecke eingefuegen?
		else		
		{
			// welcher der beiden Vertices ist naeher an dem korrespondierenden Vertex des anderen (bzgl. des Winkels)
			// -> dieser Vertex liegt dann auf Diagonalachse der beiden Dreiecke

			winkel1 = abs(adapterZeichenAnweisung.at(i).Winkel - winkelSplinesegmentSortiert.at(letzterKorrespondierenderIndex));
			if (winkel1>M_PI) winkel1 = 2*M_PI - winkel1; // kleinerer Winkel bzgl. 2 PI
			winkel2 = abs(adapterZeichenAnweisung.at(i-1).Winkel  - winkelSplinesegmentSortiert.at(adapterZeichenAnweisung.at(i).korrespondierenderIndex));
			if (winkel2>M_PI) winkel2 = 2*M_PI - winkel2; // kleinerer Winkel bzgl. 2 PI

			if (winkel1<winkel2)
			{
				// Punkt i liegt auf Diagonalachse, ebenso letzterKorrespondierenderIndex

				// Dreieck einfuegen:
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-1;
				if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
				{
					meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
					meshBuffer->Indices[bufferIndizes++] = bufferVertices-2;
				}
				else
				{
					meshBuffer->Indices[bufferIndizes++] = bufferVertices-2;
					meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
				}
				// Dreieck einfuegen:
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-1;
				if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
				{
					meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(i).korrespondierenderIndex;
					meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
				}
				else
				{
					meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
					meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(i).korrespondierenderIndex;
				}
			}
			else
			{
				// Punkt i-1 liegt auf Diagonalachse, ebenso der aktuelle Korrespondierende Index

				// Dreieck einfuegen:
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-2;
				if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
				{
					meshBuffer->Indices[bufferIndizes++] = bufferVertices-1;
					meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(i).korrespondierenderIndex;					
				}
				else
				{					
					meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(i).korrespondierenderIndex;
					meshBuffer->Indices[bufferIndizes++] = bufferVertices-1;
				}
				// Dreieck einfuegen:
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-2;
				if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
				{
					meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(i).korrespondierenderIndex;
					meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
				}
				else
				{
					meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
					meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(i).korrespondierenderIndex;
				}
			}

		} // END: Dreiecke eingefuegen

		letzterKorrespondierenderIndex = adapterZeichenAnweisung.at(i).korrespondierenderIndex;
	
	} // END for

	// ----------------------------------
	// Abschluss des Adapters
	// ----------------------------------

	// 1 Dreieck einfuegen?
	if (adapterZeichenAnweisung.at(0).korrespondierenderIndex == letzterKorrespondierenderIndex)
	{
		meshBuffer->Indices[bufferIndizes++] = bufferVertices-adapterZeichenAnweisung.size();
					
		if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
		{
			meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(0).korrespondierenderIndex;
			meshBuffer->Indices[bufferIndizes++] = bufferVertices-1;
		}
		else
		{
			meshBuffer->Indices[bufferIndizes++] = bufferVertices-1;
			meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(0).korrespondierenderIndex;
		}
	}
	// 2 Dreiecke eingefuegen?
	else		
	{

		// welcher der beiden Vertices ist naeher an dem korrespondierenden Vertex des anderen (bzgl. des Winkels)
		// -> dieser Vertex liegt dann auf Diagonalachse der beiden Dreiecke
		winkel1 = abs(adapterZeichenAnweisung.at(0).Winkel - winkelSplinesegmentSortiert.at(letzterKorrespondierenderIndex));
		if (winkel1>M_PI) winkel1 = 2*M_PI - winkel1; // kleinerer Winkel bzgl. 2 PI
		winkel2 = abs(adapterZeichenAnweisung.back().Winkel  - winkelSplinesegmentSortiert.at(adapterZeichenAnweisung.at(0).korrespondierenderIndex));
		if (winkel2>M_PI) winkel2 = 2*M_PI - winkel2; // kleinerer Winkel bzgl. 2 PI

		if (winkel1<winkel2)
		{
			// Dreieck einfuegen:
			meshBuffer->Indices[bufferIndizes++] = bufferVertices-adapterZeichenAnweisung.size(); // erster Andockstellen-Vertex

			if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
			{
				meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-1;
			}
			else
			{
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-1;
				meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
			}

			// Dreieck einfuegen:
			meshBuffer->Indices[bufferIndizes++] = bufferVertices-adapterZeichenAnweisung.size(); // erster Andockstellen-Vertex

			if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
			{
				meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(0).korrespondierenderIndex;
				meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
			}
			else
			{
				meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
				meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(0).korrespondierenderIndex;
			}
		}
		else
		{
			// Dreieck einfuegen:
			meshBuffer->Indices[bufferIndizes++] = bufferVertices-1; // letzer Andockstellen-Vertex

			if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
			{
				meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(0).korrespondierenderIndex;
				meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
			}
			else
			{				
				meshBuffer->Indices[bufferIndizes++] = letzterKorrespondierenderIndex;
				meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(0).korrespondierenderIndex;
			}

			// Dreieck einfuegen:
			meshBuffer->Indices[bufferIndizes++] = bufferVertices-1; // letzer Andockstellen-Vertex

			if (_Ende == 0)	// Richtung ist davon abhaengig, ob Anfang (Adapter ist vor Spline) oder Ende (Adapter ist nach Spline) des Splines
			{
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-adapterZeichenAnweisung.size();
				meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(0).korrespondierenderIndex;

			}
			else
			{
				meshBuffer->Indices[bufferIndizes++] = adapterZeichenAnweisung.at(0).korrespondierenderIndex;
				meshBuffer->Indices[bufferIndizes++] = bufferVertices-adapterZeichenAnweisung.size();
			}
		}
	
	} // END: Dreiecke eingefuegen

	// finale Werte des Meshbuffers errechnen
	meshBuffer->Vertices.reallocate(bufferVertices);
	meshBuffer->Indices.reallocate(bufferIndizes);
	meshBuffer->recalculateBoundingBox();

	// finale Werte des Meshes errechen
	MeshAdapter[_Ende]->recalculateBoundingBox();	
}
