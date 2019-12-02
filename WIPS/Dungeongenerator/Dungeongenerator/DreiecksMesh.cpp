// Implementierung der Klasse DreiecksMesh
#include "DreiecksMesh.h"

// ======================================================
// Umwandeln
// ======================================================
void DunGen::CDreiecksMesh::VoxelInMesh()
{
	// neuen Mesh erstellen
	std::cout << "[Dreiecksmesh]: Starte Umwandlung des Voxelraums in Dreicksnetz..." << std::endl;
	Zeitmesser->Start(0);
	irr::scene::SMesh* mesh = new irr::scene::SMesh();
		
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Vertex-Anzahl in allen Octree-Knoten bestimmen:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::cout << "Zaehlen benoetigter Vertices..." << std::endl;

	SOctreeKnoten* aktuellerOctreeKnoten;
	unsigned int aktuelleSweepEbene1=0;
	unsigned int aktuelleSweepEbene2=1;
	unsigned int tempInt;

	//std::cout << "Zaehle Vertices in Blattknoten ";
	// Vertex-Anzahl der Blattknoten berechnen
	for (unsigned int i=0; i<OcTree.size(); i++)
	{
		aktuellerOctreeKnoten = OcTree.at(i);
		aktuellerOctreeKnoten->VertexAnzahl = 0;

		// nur wenn Blatt: Vertices zaehlen
		if (aktuellerOctreeKnoten->Kind[0] == NULL)
		{
			// SweepEbenen initialisieren: geben an, ob Vertex benoetigt wird oder nicht
			// (0 ... wird nicht benoetigt, 1 ... wird benoetigt)
			for (unsigned int k=aktuellerOctreeKnoten->GrenzeMinY; k<=aktuellerOctreeKnoten->GrenzeMaxY+1; k++)
				for (unsigned int l=aktuellerOctreeKnoten->GrenzeMinZ; l<=aktuellerOctreeKnoten->GrenzeMaxZ+1; l++)
				{
					GlobaleSweepEbene[aktuelleSweepEbene1][k][l] = 0;
					GlobaleSweepEbene[aktuelleSweepEbene2][k][l] = 0;
				}
			
			// ueber X-Achse sweepen:
			for (unsigned int j=aktuellerOctreeKnoten->GrenzeMinX; j<=aktuellerOctreeKnoten->GrenzeMaxX; j++)
			{
				// Vertices zaehlen				
				for (unsigned int k=aktuellerOctreeKnoten->GrenzeMinY; k<=aktuellerOctreeKnoten->GrenzeMaxY; k++)
					for (unsigned int l=aktuellerOctreeKnoten->GrenzeMinZ; l<=aktuellerOctreeKnoten->GrenzeMaxZ; l++)
					{
						// Wenn aktueller Voxel auf 1 und Nachbarn auf 0 -> Vertices werden benoetigt
						if (VoxelRaum->Voxel[j][k][l] == 1)
						{
							if (VoxelRaum->Voxel[j-1][k][l]==0)
							{
								GlobaleSweepEbene[aktuelleSweepEbene1][k][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene1][k+1][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene1][k][l+1] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene1][k+1][l+1] = 1;
							}
							if (VoxelRaum->Voxel[j+1][k][l]==0)
							{
								GlobaleSweepEbene[aktuelleSweepEbene2][k][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k+1][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k][l+1] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k+1][l+1] = 1;
							}
							if (VoxelRaum->Voxel[j][k-1][l]==0)
							{
								GlobaleSweepEbene[aktuelleSweepEbene1][k][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene1][k][l+1] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k][l+1] = 1;
							}
							if (VoxelRaum->Voxel[j][k+1][l]==0)
							{
								GlobaleSweepEbene[aktuelleSweepEbene1][k+1][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene1][k+1][l+1] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k+1][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k+1][l+1] = 1;
							}
							if (VoxelRaum->Voxel[j][k][l-1]==0)
							{
								GlobaleSweepEbene[aktuelleSweepEbene1][k][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene1][k+1][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k][l] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k+1][l] = 1;
							}
							if (VoxelRaum->Voxel[j][k][l+1]==0)
							{
								GlobaleSweepEbene[aktuelleSweepEbene1][k][l+1] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene1][k+1][l+1] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k][l+1] = 1;
								GlobaleSweepEbene[aktuelleSweepEbene2][k+1][l+1] = 1;
							}
						}
					}

				// genutzten Bereich der SweepEbene loeschen und Vertices hochzaehlen
				for (unsigned int k=aktuellerOctreeKnoten->GrenzeMinY; k<=aktuellerOctreeKnoten->GrenzeMaxY+1; k++)
					for (unsigned int l=aktuellerOctreeKnoten->GrenzeMinZ; l<=aktuellerOctreeKnoten->GrenzeMaxZ+1; l++)
					{
						aktuellerOctreeKnoten->VertexAnzahl += GlobaleSweepEbene[aktuelleSweepEbene1][k][l];
						GlobaleSweepEbene[aktuelleSweepEbene1][k][l] = 0;
					}
					
				// SweepEbenen vertauschen
				tempInt = aktuelleSweepEbene1;
				aktuelleSweepEbene1 = aktuelleSweepEbene2;
				aktuelleSweepEbene2 = tempInt;

			} // ENDE: ueber X-Achse sweepen
			
			// restliche Vertices hochzaehlen
			for (unsigned int k=aktuellerOctreeKnoten->GrenzeMinY; k<=aktuellerOctreeKnoten->GrenzeMaxY+1; k++)
				for (unsigned int l=aktuellerOctreeKnoten->GrenzeMinZ; l<=aktuellerOctreeKnoten->GrenzeMaxZ+1; l++)
					aktuellerOctreeKnoten->VertexAnzahl += GlobaleSweepEbene[aktuelleSweepEbene1][k][l];

		} // ENDE: nur wenn Blatt
			
	} // ENDE: Vertex-Anzahl berechnen

	// Vertexanzahl der Nicht-Blattknoten bestimmen
	// Achtung: die tatsaechlich benoetigte Vertexanzahl kann niedriger liegen, da die ueberlappenden Randvertices der Kinder nun nicht mehrfach auftauchen
	//std::cout << " Addiere Vertices fuer restliche Knoten." << std::endl;
	OcTree.at(0)->ZaehleVertices(); // von der Wurzel aus zaehlen
	std::cout << "Anzahl Vertices fuer Octree-Wurzel = " << OcTree.at(0)->VertexAnzahl << std::endl;

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Umwandlung in Dreicksmesh:
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	std::cout << "Umwandlung beginnt..." << std::endl;
	SVoxelGebietVisibilityEigenschaften* visEigenschaften;

	// Queue erstellen und Wurzel heineinladen
	std::queue<SOctreeKnoten*> umwandlungsQueue;
	umwandlungsQueue.push(OcTree.at(0));

	// bis Umwandlung abgeschlossen
	while (!umwandlungsQueue.empty())
	{
		aktuellerOctreeKnoten = umwandlungsQueue.front();

		// zuviele Vertices: Kinderknoten verwenden
		if (aktuellerOctreeKnoten->VertexAnzahl > MaxAnzahlVertices)
			for (unsigned int i=0; i<8; i++)
				umwandlungsQueue.push(aktuellerOctreeKnoten->Kind[i]);
		
		// sonst: umwandeln
		else if (aktuellerOctreeKnoten->VertexAnzahl > 0)
		{
			std::cout << "neuer Meshbuffer wird erstellt..."  << std::endl;
			// Eigenschaften des Gebiets merken: Grenzen, Reihenfolge identisch zu erzeugten Meshbuffern
			visEigenschaften = new SVoxelGebietVisibilityEigenschaften();
			visEigenschaften->GrenzeMinX = aktuellerOctreeKnoten->GrenzeMinX;
			visEigenschaften->GrenzeMaxX = aktuellerOctreeKnoten->GrenzeMaxX;
			visEigenschaften->GrenzeMinY = aktuellerOctreeKnoten->GrenzeMinY;
			visEigenschaften->GrenzeMaxY = aktuellerOctreeKnoten->GrenzeMaxY;
			visEigenschaften->GrenzeMinZ = aktuellerOctreeKnoten->GrenzeMinZ;
			visEigenschaften->GrenzeMaxZ = aktuellerOctreeKnoten->GrenzeMaxZ;
			VisibilityInformationen.push_back(visEigenschaften);

			// Buffer erstellen
			irr::scene::SMeshBuffer* meshBuffer = new irr::scene::SMeshBuffer();
			// hinzufuegen
			mesh->addMeshBuffer(meshBuffer);
			meshBuffer->drop(); // droppen, da nun am Mesh haengend -> wird beim Loeschen des Meshs mit geloescht
			unsigned int bufferVertices = 0;
			unsigned int bufferIndizes = 0;

			// erstmal fuer den worst-case reservieren
			meshBuffer->Vertices.set_used(MaxAnzahlVertices); 
			meshBuffer->Indices.set_used(24*MaxAnzahlVertices);	// max je Vertex: 12 Flaechen * 2 Dreiecke (worst case)

			// SweepEbenen initialisieren: speichern nun Indices der Vertexe
			// Index > MaxAnzahlVertices --> Index nicht gesetzt
			for (unsigned int j=aktuellerOctreeKnoten->GrenzeMinY; j<=aktuellerOctreeKnoten->GrenzeMaxY+1; j++)
				for (unsigned int k=aktuellerOctreeKnoten->GrenzeMinZ; k<=aktuellerOctreeKnoten->GrenzeMaxZ+1; k++)
				{
					GlobaleSweepEbene[aktuelleSweepEbene1][j][k] = MaxAnzahlVertices+1;
					GlobaleSweepEbene[aktuelleSweepEbene2][j][k] = MaxAnzahlVertices+1;
				}
			aktuelleSweepEbene1=0;
			aktuelleSweepEbene2=1;

			// Umwandlung per Sweep:
			// ueber X-Achse sweepen
			for (unsigned int i=aktuellerOctreeKnoten->GrenzeMinX; i<=aktuellerOctreeKnoten->GrenzeMaxX; i++)
			{
				// YZ-Ebene	ist SweepEbene	
				for (unsigned int j=aktuellerOctreeKnoten->GrenzeMinY; j<=aktuellerOctreeKnoten->GrenzeMaxY; j++)
					for (unsigned int k=aktuellerOctreeKnoten->GrenzeMinZ; k<=aktuellerOctreeKnoten->GrenzeMaxZ; k++)
					{
						// Wenn aktueller Voxel auf 1 und Nachbarn auf 0 -> Dreiecke benoetigt
						if (VoxelRaum->Voxel[i][j][k] == 1)
						{		
							// Test entlang X-Achse
							if (VoxelRaum->Voxel[i-1][j][k]==0)
							{
								// 4 Eckpunke hinzufuegen (falls jeweils noch nicht vorhanden) & Indices merken
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j+1,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j+1,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j+1,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j+1,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}

								// 2 Dreiecke hinzufuegen: Reihenfolge der Vertices gegen den Uhrzeigersinn bzgl. rechtshaendigem System, also im UZS fuer Irrlicht-System
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1];
																
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1];								
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1];								
							}

							if (VoxelRaum->Voxel[i+1][j][k]==0)
							{
								// 4 Eckpunke hinzufuegen (falls jeweils noch nicht vorhanden) & Indices merken
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j+1,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j+1,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j+1,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j+1,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}

								// 2 Dreiecke hinzufuegen: Reihenfolge der Vertices gegen den Uhrzeigersinn bzgl. rechtshaendigem System, also im UZS fuer Irrlicht-System
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1];								
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k];								
								
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1];																								
							}

							// Test entlang Y-Achse
							if (VoxelRaum->Voxel[i][j-1][k]==0)
							{
								// 4 Eckpunke hinzufuegen (falls jeweils noch nicht vorhanden) & Indices merken
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}

								// 2 Dreiecke hinzufuegen: Reihenfolge der Vertices gegen den Uhrzeigersinn bzgl. rechtshaendigem System, also im UZS fuer Irrlicht-System
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1];	
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1];								

								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j][k];																	
							}

							if (VoxelRaum->Voxel[i][j+1][k]==0)
							{
								// 4 Eckpunke hinzufuegen (falls jeweils noch nicht vorhanden) & Indices merken
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j+1,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j+1,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j+1,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j+1,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j+1,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j+1,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j+1,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j+1,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}

								// 2 Dreiecke hinzufuegen: Reihenfolge der Vertices gegen den Uhrzeigersinn bzgl. rechtshaendigem System, also im UZS fuer Irrlicht-System
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1];													

								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k];	
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1];																
							}

							// Test entlang Z-Achse
							if (VoxelRaum->Voxel[i][j][k-1]==0)
							{
								// 4 Eckpunke hinzufuegen (falls jeweils noch nicht vorhanden) & Indices merken
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j+1,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j+1,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j+1,k));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j+1,k,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}

								// 2 Dreiecke hinzufuegen: Reihenfolge der Vertices gegen den Uhrzeigersinn bzgl. rechtshaendigem System, also im UZS fuer Irrlicht-System
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k];	
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k];																				

								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k];									
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j][k];	
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k];															
							}

							if (VoxelRaum->Voxel[i][j][k+1]==0)
							{
								// 4 Eckpunke hinzufuegen (falls jeweils noch nicht vorhanden) & Indices merken
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i,j+1,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i,j+1,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}
								if (GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1] > MaxAnzahlVertices)
								{	
									irr::video::S3DVertex& v = meshBuffer->Vertices[bufferVertices];
									GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1] = bufferVertices++;
									// Koordinaten des Vertex berechnen
									v.Pos.set(BerechneVertexKoordinaten(i+1,j+1,k+1));
									// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speichrung als Textur-Koordinaten
									v.TCoords.set(BerechneVertexEigenschaften(i+1,j+1,k+1,aktuellerOctreeKnoten->GrenzeMinX, aktuellerOctreeKnoten->GrenzeMinY, aktuellerOctreeKnoten->GrenzeMinZ,
										aktuellerOctreeKnoten->GrenzeMaxX, aktuellerOctreeKnoten->GrenzeMaxY, aktuellerOctreeKnoten->GrenzeMaxZ));
								}

								// 2 Dreiecke hinzufuegen: Reihenfolge der Vertices gegen den Uhrzeigersinn bzgl. rechtshaendigem System, also im UZS fuer Irrlicht-System
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j+1][k+1];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1];
								
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene1][j][k+1];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j+1][k+1];
								meshBuffer->Indices[bufferIndizes++] = GlobaleSweepEbene[aktuelleSweepEbene2][j][k+1];									
							}

						} // ENDE: Wenn aktueller Voxel auf 1

					} // ENDE: YZ-Ebene

				// genutzten Bereich der SweepEbene loeschen
				for (unsigned int j=aktuellerOctreeKnoten->GrenzeMinY; j<=aktuellerOctreeKnoten->GrenzeMaxY+1; j++)
					for (unsigned int k=aktuellerOctreeKnoten->GrenzeMinZ; k<=aktuellerOctreeKnoten->GrenzeMaxZ+1; k++)
						GlobaleSweepEbene[aktuelleSweepEbene1][j][k] = MaxAnzahlVertices+1;
					
				// SweepEbenen vertauschen
				tempInt = aktuelleSweepEbene1;
				aktuelleSweepEbene1 = aktuelleSweepEbene2;
				aktuelleSweepEbene2 = tempInt;

			} // ENDE: ueber X-Achse sweepen

			// finale Werte des Meshbuffers errechnen
			meshBuffer->Vertices.reallocate(bufferVertices);
			meshBuffer->Indices.reallocate(bufferIndizes);
			meshBuffer->recalculateBoundingBox();

		} // ENDE: Umwandlung dieses Knotens

		// naechster Knoten der Queue
		umwandlungsQueue.pop();
	}

	// finale Werte des Meshes errechen
	mesh->recalculateBoundingBox();
	// Mesh merken
	MeshVoxel.push_back(mesh);

	// Normalen berechnen
	Zeitmesser->Start(1);
	NormalenBerechnen();

	std::cout << "[Dreiecksmesh]: Umwandlung des Voxelraums in Dreiecksnetz abgeschlossen. Anzahl Meshbuffer = "  << mesh->getMeshBufferCount() << std::endl;
	std::cout << "[Dreiecksmesh]: Benoetigte Zeit fuer Umwandlung: ";
	Zeitmesser->Stop(0);
	
	std::cout << "==> davon fuer Normalenberechnung: ";
	Zeitmesser->Stop(1);
}

// Finale Vertex-Koordinaten berechnen, dabei eventuell verwackeln und glaetten
irr::core::vector3d<irr::f32> DunGen::CDreiecksMesh::BerechneVertexKoordinaten(unsigned int _X, unsigned int _Y, unsigned int _Z)
{
	if (!Verwackeln)
		return irr::core::vector3d<irr::f32>(_X,_Y,_Z);

	// deterministisches Verwackeln: gleicher Input -> gleicher Output
	// wichtig damit einzelne Mesh-Teile an den Raendern zusammenpassen
	ZufallsGenerator->SetzeZufallsSeed(ZufallsSeed + _X + (CVoxelRaum::VoxelraumX+1)*_Y + (CVoxelRaum::VoxelraumX+1)*(CVoxelRaum::VoxelraumY+1)*_Z); // Symmetrie vermeiden
	
	double deltaX, deltaY, deltaZ;

	// wenn ein umgebender Voxel mit 10 markiert ist (Andockvoxel), dann handelt es sich um einen Andockvertex
	bool andockVertex = ((VoxelRaum->Voxel[_X-1][_Y][_Z]+VoxelRaum->Voxel[_X][_Y-1][_Z]+VoxelRaum->Voxel[_X][_Y][_Z-1]
		+VoxelRaum->Voxel[_X-1][_Y-1][_Z]+VoxelRaum->Voxel[_X-1][_Y][_Z-1]+VoxelRaum->Voxel[_X][_Y-1][_Z-1]
		+VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]+VoxelRaum->Voxel[_X][_Y][_Z])>8);
	
	// nur Glaetten wenn kein Andockvertex: somit alle umgebenden Voxel mit 0 oder 1 belegt
	if (Glaetten && !andockVertex)
	{
		// Index fuer die Modifikator-Look-Up-Tabelle berechnen
		unsigned int indexModifikatortabelle = (VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]) | ((VoxelRaum->Voxel[_X][_Y-1][_Z-1])<<1)
			| ((VoxelRaum->Voxel[_X-1][_Y][_Z-1])<<2) | ((VoxelRaum->Voxel[_X][_Y][_Z-1])<<3)
			| ((VoxelRaum->Voxel[_X-1][_Y-1][_Z])<<4) | ((VoxelRaum->Voxel[_X][_Y-1][_Z])<<5)
			| ((VoxelRaum->Voxel[_X-1][_Y][_Z])<<6) | ((VoxelRaum->Voxel[_X][_Y][_Z])<<7);

		// Modifikatoren auslesen:
		SVertexModifikatoren modifikatoren = VertexModifikatoren[indexModifikatortabelle];		

		// Verwacklen im entsprechenden Bereich
		// X-Richtung
		if (modifikatoren.RichtungX == 0)
			deltaX = -VerwacklungsStaerke + 2*VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
		else if (modifikatoren.RichtungX > 0)
			deltaX = VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
		else
			deltaX = -VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();

		// Y-Richtung
		if (modifikatoren.RichtungY == 0)
			deltaY = -VerwacklungsStaerke + 2*VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
		else if (modifikatoren.RichtungY > 0)
			deltaY = VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
		else
			deltaY = -VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();

		// Z-Richtung
		if (modifikatoren.RichtungZ == 0)
			deltaZ = -VerwacklungsStaerke + 2*VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
		else if (modifikatoren.RichtungZ > 0)
			deltaZ = VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
		else
			deltaZ = -VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
	}
	else
	{
		// Verwackeln im Bereich -VerwacklungsStaerke und +VerwacklungsStaerke
		deltaX = -VerwacklungsStaerke + 2*VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
		deltaY = -VerwacklungsStaerke + 2*VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
		deltaZ = -VerwacklungsStaerke + 2*VerwacklungsStaerke * ZufallsGenerator->GeneriereZufallsWert01();
	}

	// Clamping: Positionsanpassung um ineinander-geschobenen Dreiecken vorzubeugen
	double absDeltaX = abs(deltaX);
	double absDeltaY = abs(deltaY);
	double absDeltaZ = abs(deltaZ);

	// Fall berechnen, der hier vorliegt und passend verfahren
	if ((absDeltaX+absDeltaY)>MaxClampDistanz) // XY-Ebene clampen
	{
		if ((absDeltaX+absDeltaZ)>MaxClampDistanz) // XZ-Ebene clampen	
		{
			if ((absDeltaY+absDeltaZ)>MaxClampDistanz) // YZ-Ebene clampen
			{
				// alle 3 Ebenen -> alle Koordinaten nomalisieren
				double normalisierungsFaktor = std::max(std::max(absDeltaX+absDeltaY,absDeltaX+absDeltaZ),absDeltaY+absDeltaZ);
				deltaX *= MaxClampDistanz/normalisierungsFaktor;
				deltaY *= MaxClampDistanz/normalisierungsFaktor;
				deltaZ *= MaxClampDistanz/normalisierungsFaktor;
			}
			else
			{
				// 2 Ebenen: XY, XZ -> gemeinsame Koordinate clampen
				double absNeu = std::min(MaxClampDistanz - absDeltaY, MaxClampDistanz - absDeltaZ);
				deltaX =  (deltaX>0) ? absNeu : -absNeu;
			}
		}
		else
		{
			if ((absDeltaY+absDeltaZ)>MaxClampDistanz) // YZ-Ebene clampen
			{
				// 2 Ebenen: XY, YZ -> gemeinsame Koordinate clampen
				double absNeu = std::min(MaxClampDistanz - absDeltaX, MaxClampDistanz - absDeltaZ);
				deltaY =  (deltaY>0) ? absNeu : -absNeu;
			}
			else
			{
				// 1 Ebene: XY -> 2 Koordinaten normalisieren
				double normalisierungsFaktor = absDeltaX+absDeltaY;
				deltaX *= MaxClampDistanz/normalisierungsFaktor;
				deltaY *= MaxClampDistanz/normalisierungsFaktor;
			}
		}
	}
	else
	{
		if ((absDeltaX+absDeltaZ)>MaxClampDistanz) // XZ-Ebene clampen	
		{
			if ((absDeltaY+absDeltaZ)>MaxClampDistanz) // YZ-Ebene clampen
			{
				// 2 Ebenen: XZ, YZ -> gemeinsame Koordinate clampen
				double absNeu = std::min(MaxClampDistanz - absDeltaX, MaxClampDistanz - absDeltaY);
				deltaZ =  (deltaZ>0) ? absNeu : -absNeu;
			}
			else
			{
				// 1 Ebene: XZ -> 2 Koordinaten normalisieren
				double normalisierungsFaktor = absDeltaX+absDeltaZ;
				deltaX *= MaxClampDistanz/normalisierungsFaktor;
				deltaZ *= MaxClampDistanz/normalisierungsFaktor;
			}
		}
		else
		{
			if ((absDeltaY+absDeltaZ)>MaxClampDistanz) // YZ-Ebene clampen
			{
				// 1 Ebene: YZ -> 2 Koordinaten normalisieren
				double normalisierungsFaktor = absDeltaY+absDeltaZ;
				deltaY *= MaxClampDistanz/normalisierungsFaktor;
				deltaZ *= MaxClampDistanz/normalisierungsFaktor;
			}
			// else { /* 0 Ebenen -> nichts zu tun */ }			
		}
	}

	// Ergebnis berechnen
	irr::core::vector3d<irr::f32> ergebnis;
	ergebnis.X = _X + deltaX;
	ergebnis.Y = _Y + deltaY;
	ergebnis.Z = _Z + deltaZ;
		
	return ergebnis;
}

// Berechnet die Eigenschaften des Vertex: Grenzvertex, Andockvertex -> Speicherung als Tetxur-Koordinaten
irr::core::vector2d<irr::f32> DunGen::CDreiecksMesh::BerechneVertexEigenschaften(unsigned int _X, unsigned int _Y, unsigned int _Z,
			unsigned int _GrenzeXmin, unsigned int _GrenzeYmin, unsigned int _GrenzeZmin,
			unsigned int _GrenzeXmax, unsigned int _GrenzeYmax, unsigned int _GrenzeZmax)
{
	return irr::core::vector2d<irr::f32>(IstVertexGrenzvertex(_X,_Y,_Z,_GrenzeXmin, _GrenzeYmin,_GrenzeZmin,_GrenzeXmax,_GrenzeYmax,_GrenzeZmax),
		IstVertexAndockvertex(_X,_Y,_Z));
}

// Testet ob Vertex einen Grenzvertex darstellt (-1.0 kein Grenzvertex, 1.0 Grenzvertex), fuer Speicherung als Texture.X
inline double DunGen::CDreiecksMesh::IstVertexGrenzvertex(unsigned int _X, unsigned int _Y, unsigned int _Z,
		unsigned int _GrenzeXmin, unsigned int _GrenzeYmin, unsigned int _GrenzeZmin,
		unsigned int _GrenzeXmax, unsigned int _GrenzeYmax, unsigned int _GrenzeZmax)
{
	// Vertex wird geteilt von: Voxel[X-1,Y-1,Z-1] bis Voxel[X,Y,Z] (8 Voxel insgesamt)

	// Ueberpruefung von 4 Voxeln im angrenzenden Gebiet
	if (_X == _GrenzeXmin)
	{
		// ist ein Voxel = 1 und ein anderer (angrenzender, mit gleichem Bezugsvertex) = 0?
		if (VoxelRaum->Voxel[_X-1][_Y][_Z]==1)
			if (VoxelRaum->Voxel[_X][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y-1][_Z]==1)
			if (VoxelRaum->Voxel[_X][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y][_Z-1]==1)
			if (VoxelRaum->Voxel[_X][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==1)
			if (VoxelRaum->Voxel[_X][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z]==0)
				return 1.0;
	}
	// Ueberpruefung von 4 Voxeln im angrenzenden Gebiet
	else if (_X == _GrenzeXmax+1)
	{
		if (VoxelRaum->Voxel[_X][_Y][_Z]==1)
			if (VoxelRaum->Voxel[_X-1][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X][_Y-1][_Z]==1)
			if (VoxelRaum->Voxel[_X-1][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X][_Y][_Z-1]==1)
			if (VoxelRaum->Voxel[_X-1][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X][_Y-1][_Z-1]==1)
			if (VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z]==0)
				return 1.0;
	}

	// Ueberpruefung von 4 Voxeln im angrenzenden Gebiet
	if (_Y == _GrenzeYmin)
	{
		if (VoxelRaum->Voxel[_X][_Y-1][_Z]==1)
			if (VoxelRaum->Voxel[_X-1][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y-1][_Z]==1)
			if (VoxelRaum->Voxel[_X][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X][_Y-1][_Z-1]==1)
			if (VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==1)
			if (VoxelRaum->Voxel[_X][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z]==0)
				return 1.0;
	}

	// Ueberpruefung von 4 Voxeln im angrenzenden Gebiet
	else if (_Y == _GrenzeYmax+1)
	{
		if (VoxelRaum->Voxel[_X][_Y][_Z]==1)
			if (VoxelRaum->Voxel[_X-1][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y][_Z]==1)
			if (VoxelRaum->Voxel[_X][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X][_Y][_Z-1]==1)
			if (VoxelRaum->Voxel[_X-1][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y][_Z-1]==1)
			if (VoxelRaum->Voxel[_X][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z]==0)
				return 1.0;
	}

	// Ueberpruefung von 4 Voxeln im angrenzenden Gebiet
	if (_Z == _GrenzeZmin)
	{
		if (VoxelRaum->Voxel[_X][_Y][_Z-1]==1)
			if (VoxelRaum->Voxel[_X-1][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y][_Z-1]==1)
			if (VoxelRaum->Voxel[_X][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X][_Y-1][_Z-1]==1)
			if (VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==1)
			if (VoxelRaum->Voxel[_X][_Y-1][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z-1]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z]==0)
				return 1.0;
	}

	// Ueberpruefung von 4 Voxeln im angrenzenden Gebiet
	else if (_Z == _GrenzeZmax+1)
	{
		if (VoxelRaum->Voxel[_X][_Y][_Z]==1)
			if (VoxelRaum->Voxel[_X-1][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y][_Z]==1)
			if (VoxelRaum->Voxel[_X][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X][_Y-1][_Z]==1)
			if (VoxelRaum->Voxel[_X-1][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X][_Y-1][_Z-1]==0)
				return 1.0;

		if (VoxelRaum->Voxel[_X-1][_Y-1][_Z]==1)
			if (VoxelRaum->Voxel[_X][_Y-1][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y][_Z]==0 ||
				VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]==0)
				return 1.0;
	}

	// kein Grenzvertex
	return -1.0;
}

// Testet ob Vertex einen Vertex an einer Andockstelle darstellt (-1.0 kein Andockvertex, 1.0 Andockvertex), fuer Speicherung als Texture.Y-Koordinate
inline double DunGen::CDreiecksMesh::IstVertexAndockvertex(unsigned int _X, unsigned int _Y, unsigned int _Z)
{
	// wenn ein umgebender Voxel mit 10 markiert ist (Andockvoxel), dann handelt es sich um einen Andockvertex
	if ((VoxelRaum->Voxel[_X-1][_Y][_Z]+VoxelRaum->Voxel[_X][_Y-1][_Z]+VoxelRaum->Voxel[_X][_Y][_Z-1]
		+VoxelRaum->Voxel[_X-1][_Y-1][_Z]+VoxelRaum->Voxel[_X-1][_Y][_Z-1]+VoxelRaum->Voxel[_X][_Y-1][_Z-1]
		+VoxelRaum->Voxel[_X-1][_Y-1][_Z-1]+VoxelRaum->Voxel[_X][_Y][_Z])>8)
		return 1.0;

	// sonst kein Andockvertex
	return -1.0;
}

// Berechnung der Normalen
void DunGen::CDreiecksMesh::NormalenBerechnen()
{
	std::cout << "[Dreiecksmesh]: Berechne Normalen..." << std::endl;
	std::cout << "1. Roh-Normalen werden berechnet..." << std::endl;
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Roh-Normalen berechnen und speichern
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Grund-Funktionsweise aus CMeshManipulator.cpp (Irrlicht Release 1.7.3) recalculateNormals() uebernommen
	irr::scene::IMeshBuffer* meshBuffer;
	unsigned int vertexAnzahl;
	unsigned int indexAnzahl;

	// Speicher pre-allokieren/Hilfsvariablen	
	irr::core::vector3d<double> normale;
	irr::core::vector3d<double> vertex1;
	irr::core::vector3d<double> vertex2;
	irr::core::vector3d<double> vertex3;		
	irr::core::vector3d<double> tempVecD;
	
	for (unsigned int i=0; i<MeshVoxel.at(0)->getMeshBufferCount(); i++)
	{
		// Meshbuffer merken
		meshBuffer = MeshVoxel.at(0)->getMeshBuffer(i);

		// Werte auslesen
		vertexAnzahl = meshBuffer->getVertexCount();
		indexAnzahl = meshBuffer->getIndexCount();
		const irr::u16* indices = meshBuffer->getIndices();

		// alle Normalen auf 0 setzen
		for (unsigned int j=0; j<vertexAnzahl; j++)
			meshBuffer->getNormal(j).set(0.f,0.f,0.f);

		// Normalen berechnen und zusammenaddieren
		for (unsigned int j=0; j<indexAnzahl; j+=3)
		{
			// Normale des aktuellen Dreiecks berechnen
			const irr::core::vector3df& v1 = meshBuffer->getPosition(indices[j+0]);
			const irr::core::vector3df& v2 = meshBuffer->getPosition(indices[j+1]);
			const irr::core::vector3df& v3 = meshBuffer->getPosition(indices[j+2]);
			vertex1 = irr::core::vector3d<double>(v1.X,v1.Y,v1.Z);
			vertex2 = irr::core::vector3d<double>(v2.X,v2.Y,v2.Z);
			vertex3 = irr::core::vector3d<double>(v3.X,v3.Y,v3.Z);

			normale = (vertex2-vertex1).crossProduct(vertex3-vertex1);			

			// Gewicht der Normale in jedem der 3 Eck-Vertices des Dreiecks bestimmen
			if (NormalenWichtungsmethode == ENormalenWichtungsmethode::_NORMALENWICHTUNG_FLAECHENINHALT_)
			{
				// Flaecheninhalt des Dreiecks = halbe Flaeche des Parallelogramms welches von (P2-P1) und (P3-P1) aufgepannt wird
				// tempVecD.X = (vertex2-vertex1).crossProduct(vertex3-vertex1).getLength() * 0.5; tempVecD.Y = tempVecD.X; tempVecD.Z = tempVecD.X;
				// normale.normalize();

				// durch das Weglassen der Normalisierung ist die Wichtung per Flaecheninhalt gegeben
				// Faktor 0.5 ist ueberall identisch, ist also irrelevant (letztlich wird also mit dem Flaecheninhalt des Parallelogramms gewichtet)
				tempVecD.set(1.0,1.0,1.0);				
			}
			else if (NormalenWichtungsmethode == ENormalenWichtungsmethode::_NORMALENWICHTUNG_WINKEL_)
			{
				normale.normalize();
				tempVecD = berechneWinkelGewicht(vertex1,vertex2,vertex3);
			}
			else
			{
				normale.normalize();
				tempVecD.set(1.0,1.0,1.0);
			}
			
			// Normale mit entsprechendem Gewicht aufaddieren
			vertex1 = tempVecD.X * normale;
			vertex2 = tempVecD.Y * normale;
			vertex3 = tempVecD.Z * normale;

			meshBuffer->getNormal(indices[j+0]) += irr::core::vector3df(vertex1.X,vertex1.Y,vertex1.Z);
			meshBuffer->getNormal(indices[j+1]) += irr::core::vector3df(vertex2.X,vertex2.Y,vertex2.Z);
			meshBuffer->getNormal(indices[j+2]) += irr::core::vector3df(vertex3.X,vertex3.Y,vertex3.Z);
		}
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Normalen an den Grenzvertices zusammenfassen
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::cout << "2. Normalen an den Grenzvertices zusammenfassen: Tabelle wird aufgebaut..." << std::endl;

	unsigned int HashX, HashZ;
	irr::core::vector3df tempVec;

	for (unsigned int i=0; i<MeshVoxel.at(0)->getMeshBufferCount(); i++)
	{
		// Meshbuffer merken
		meshBuffer = MeshVoxel.at(0)->getMeshBuffer(i);
		vertexAnzahl = meshBuffer->getVertexCount();
		
		for (unsigned int j=0; j<vertexAnzahl; j++)
			// wenn Grenzvertex:
			if (meshBuffer->getTCoords(j).X > 0.0)
			{
				// Informationen zum Vertex merken
				SVertexInformationen neueVertexInformation;
				tempVec = meshBuffer->getNormal(j);
				neueVertexInformation.Normale = irr::core::vector3d<double>(tempVec.X,tempVec.Y,tempVec.Z);
				tempVec = meshBuffer->getPosition(j);
				neueVertexInformation.Y = d2i(tempVec.Y);
				
				// Adresse des Vertex merken
				neueVertexInformation.Adresse.MeshbufferID = i;
				neueVertexInformation.Adresse.VertexID = j;

				// alles in Hash-Tabelle speichern
				HashX = d2i(tempVec.X);
				HashZ = d2i(tempVec.Z);

				if (HashTabelle[HashX][HashZ] == NULL)
					HashTabelle[HashX][HashZ] = new std::list<SVertexInformationen>();

				HashTabelle[HashX][HashZ]->push_front(neueVertexInformation);
			}
	}

	// Speicher pre-allokieren/Hilfsvariablen
	unsigned int aktuellesY;
	std::queue<SVertexAdresse> zuAktualisierendeVertices;
	SVertexAdresse aktuelleVertexAdresse;

	std::cout << "2. Normalen an den Grenzvertices zusammenfassen: Tabelle wird verarbeitet..." << std::endl;
	// Hashtabellen weiterverarbeiten
	for (unsigned int i=0; i<CVoxelRaum::VoxelraumX+1; i++)
		for (unsigned int j=0; j<CVoxelRaum::VoxelraumY+1; j++)
			if (HashTabelle[i][j] != NULL)
			{
				// Hashtabelle nach Y sortieren
				HashTabelle[i][j]->sort();

				// Hasheintraege nun zusammenfassen
				aktuellesY = HashTabelle[i][j]->front().Y;
				normale = irr::core::vector3d<double>(0.0,0.0,0.0);
				
				// durch Liste laufen:
				for (std::list<SVertexInformationen>::iterator it=HashTabelle[i][j]->begin() ; it != HashTabelle[i][j]->end(); it++)
				{
					// wenn neuer Vertex (= neues Y) -> alte Normale uebertragen
					if (aktuellesY!=(*it).Y)
					{
						// Normale an alle zugehoerigen Adressen speichern (normalisiert wird spaeter)
						while (!zuAktualisierendeVertices.empty())
						{
							aktuelleVertexAdresse = zuAktualisierendeVertices.front();
							MeshVoxel.at(0)->getMeshBuffer(aktuelleVertexAdresse.MeshbufferID)->getNormal(aktuelleVertexAdresse.VertexID) = irr::core::vector3df(normale.X,normale.Y,normale.Z);
							zuAktualisierendeVertices.pop();
						}
						// neues Y merken
						aktuellesY = (*it).Y;
						// Normale wieder auf 0
						normale = irr::core::vector3d<double>(0.0,0.0,0.0);
					}
					// Werte zusammenfassen
					normale += (*it).Normale;
					zuAktualisierendeVertices.push((*it).Adresse);
				}

				// letzte Werte uebertragen:
				// Normale an alle zugehoerigen Adressen speichern (normalisiert wird spaeter)
				while (!zuAktualisierendeVertices.empty())
				{
					aktuelleVertexAdresse = zuAktualisierendeVertices.front();
					MeshVoxel.at(0)->getMeshBuffer(aktuelleVertexAdresse.MeshbufferID)->getNormal(aktuelleVertexAdresse.VertexID) = irr::core::vector3df(normale.X,normale.Y,normale.Z);
					zuAktualisierendeVertices.pop();
				}

				// Hasheintraege an dieser Stelle der Hashtabelle loeschen da abgearbeitet
				delete HashTabelle[i][j];
				HashTabelle[i][j] = NULL;
			}
			

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// alle Normalen normalisieren
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::cout << "3. Normalen werden normalisiert..." << std::endl;
	for (unsigned int i=0; i<MeshVoxel.at(0)->getMeshBufferCount(); i++)
	{
		// Meshbuffer merken
		meshBuffer = MeshVoxel.at(0)->getMeshBuffer(i);
		vertexAnzahl = meshBuffer->getVertexCount();
		
		// fuer jeden Vertex des Meshbuffers: Normale normalisieren
		for (unsigned int j=0; j<vertexAnzahl; j++)
			meshBuffer->getNormal(j).normalize();
	}

}