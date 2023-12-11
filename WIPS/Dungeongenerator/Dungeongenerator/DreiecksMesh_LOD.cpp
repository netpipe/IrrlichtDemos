// Implementierung der Klasse DreiecksMesh
#include "DreiecksMesh.h"

// ======================================================
// Reduktion
// ======================================================

void DunGen::CDreiecksMesh::BerechneNeueDetailstufe()
{
	std::cout << "[Dreiecksmesh]: Starte Reduktion des Dreiecksnetzes auf Detailstufe " << MeshVoxel.size() << "..." << std::endl;
	Zeitmesser->Start(0);

	// neuen Mesh erzeugen
	irr::scene::SMesh* mesh = new irr::scene::SMesh();

	// Index-Tabelle der neu erzeugten Vertices
	int vertexAdresse[MaxAnzahlVertices];

	// zum Zaehlen der Reduktion
	unsigned int anzahlVerticesMeshAlt = 0;
	unsigned int anzahlVerticesMeshNeu = 0;
	unsigned int anzahlDreieckeMeshAlt = 0;
	unsigned int anzahlDreieckeMeshNeu = 0;

	// Speicher pre-allokieren
	int index1, index2, index3;
	irr::scene::IMeshBuffer* originalMeshBuffer;
	irr::scene::SMeshBuffer* reduzierterMeshBuffer;
	unsigned int bufferVertices;
	unsigned int bufferIndizes;

	irr::core::vector3df tempVecF;
	int anzahlDoppelte;
	int tempIndex;

	// fuer alle Meshbuffer des Elternmeshes (eine Detailstufe hoeher)
	for (unsigned int i=0; i<MeshVoxel.back()->getMeshBufferCount(); i++)
	{
		// Vertex-Adressen-Tabelle re-initialisieren
		for (unsigned int j=0; j<MaxAnzahlVertices; j++)
		{
			// Adressen unbelegt, da noch keine reduzierten Vertices vorliegen
			vertexAdresse[j] = -1;

			// Flags loeschen
			VerticesFlagsOriginal[j] = MaxAnzahlVertices+1;
			VerticesFlagsReduziert[j] = MaxAnzahlVertices+1;
		}
		originalMeshBuffer = MeshVoxel.back()->getMeshBuffer(i);
		
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// 0. Adjazenzlisten aufbauen
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		for (unsigned int j=0; j<originalMeshBuffer->getVertexCount(); j++)
			Adjazenzlisten[j]->clear();
		for (unsigned int j=0; j<originalMeshBuffer->getIndexCount(); j+=3)
		{
			// Eck-Indices auslesen
			index1 = originalMeshBuffer->getIndices()[j];
			index2 = originalMeshBuffer->getIndices()[j+1];
			index3 = originalMeshBuffer->getIndices()[j+2];

			// Adjazenzlisten befuellen: per Halbkantenrichtung, andere Richtung durch Nachbardreiecke
			Adjazenzlisten[index1]->push_back(index2);
			Adjazenzlisten[index2]->push_back(index3);
			Adjazenzlisten[index3]->push_back(index1);
		}
		
		// neuen Meshbuffer generieren: fuer reduzierten Mesh
		std::cout << "Buffer "  << i;
		reduzierterMeshBuffer = new irr::scene::SMeshBuffer();
		mesh->addMeshBuffer(reduzierterMeshBuffer);
		reduzierterMeshBuffer->drop(); // droppen, da nun am Mesh haengend -> wird beim Loeschen des Meshs mit geloescht
		bufferVertices = 0;
		bufferIndizes = 0;

		// erstmal fuer den worst-case reservieren
		reduzierterMeshBuffer->Vertices.set_used(originalMeshBuffer->getVertexCount()); 
		reduzierterMeshBuffer->Indices.set_used(originalMeshBuffer->getIndexCount());

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// 1. alle Dreiecke des aktuellen Meshbuffers betrachten: Collapse-Schritt
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		// alle Dreiecke betrachten
		for (unsigned int j=0; j<originalMeshBuffer->getIndexCount(); j+=3)
			// alle 3 Kanten des Dreiecks betrachten		
			for (unsigned int k=0; k<3; k++)
			{
				// Eck-Indices auslesen
				if (k==0)
				{	
					index1 = originalMeshBuffer->getIndices()[j];
					index2 = originalMeshBuffer->getIndices()[j+1];
				}
				else if (k==1)
				{	
					index1 = originalMeshBuffer->getIndices()[j];
					index2 = originalMeshBuffer->getIndices()[j+2];
				}
				else
				{	
					index1 = originalMeshBuffer->getIndices()[j+1];
					index2 = originalMeshBuffer->getIndices()[j+2];
				}				
			
				// --------------------------------------
				// Kante durchpruefen:
				// --------------------------------------
				if ((vertexAdresse[index1] + vertexAdresse[index2]										// noch kein collapsed Vertex in Kante?
				+ originalMeshBuffer->getTCoords(index1).X + originalMeshBuffer->getTCoords(index2).X	// kein Grenz-Vertex in Kante?
				+ originalMeshBuffer->getTCoords(index1).Y + originalMeshBuffer->getTCoords(index2).Y)	// kein Andock-Vertex in Kante?
																							< -5.5)		// -6 wenn alle Bedingungen erfuellt, -5 oder hoeher sonst
				{		
					// Testen welche Kante collapsed wird:
					// fuer alle Nachbarvertices von Vertex1
					for (unsigned int k=0;k<Adjazenzlisten[index1]->size(); k++)
					{
						tempIndex = Adjazenzlisten[index1]->at(k);
						if (vertexAdresse[tempIndex]<0)
						{	// Original-Vertex markieren
							VerticesFlagsOriginal[tempIndex] = index1;
						}
						else
						{
							// neuen Vertex markieren
							tempIndex = vertexAdresse[tempIndex];
							VerticesFlagsReduziert[tempIndex] = index1;
						}
					}

					anzahlDoppelte = 0;
					// fuer alle Nachbarvertices von Vertex2
					for (unsigned int k=0;k<Adjazenzlisten[index2]->size(); k++)
					{
						tempIndex = Adjazenzlisten[index2]->at(k);
						if (vertexAdresse[tempIndex]<0)
						{	// Original-Vertex: Vertex passend markiert? -> gemeinsamer Nachbar
							if (VerticesFlagsOriginal[tempIndex] == index1)
								anzahlDoppelte++;
						}
						else
						{	// reduzierter Vertex: Vertex passend markiert? -> gemeinsamer Nachbar
							tempIndex = vertexAdresse[tempIndex];
							if (VerticesFlagsReduziert[tempIndex] == index1)
								anzahlDoppelte++;
						}
					}

					if (anzahlDoppelte<3)	// weniger als 3 gemeinsame Nachbarn (sonst nicht-mannigfaltiger Mesh als Resultat)
					{
						// -> Triangle-Collapse: Dreieck wird zu Vertex collapsed
						// Index des neuen Vertex speichern
						vertexAdresse[index1] = bufferVertices;
						vertexAdresse[index2] = bufferVertices;
												
						irr::video::S3DVertex& v = reduzierterMeshBuffer->Vertices[bufferVertices++];
						// neue Position: Durchschitt der 2 Ausgangspositionen
						tempVecF = (originalMeshBuffer->getPosition(index1) + originalMeshBuffer->getPosition(index2))/2;
						v.Pos.set(tempVecF);
						// Texturkoordinaten bleiben erhalten, Normalen werden im Nachhinein berechnet
						v.TCoords.set(-1.0,-1.0);
					}
				}

			} // ENDE: 1. Collapse-Schritt

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// 2. alle Vertices des aktuellen Meshbuffers betrachten: nicht-collapsed Vertices uebernehmen
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		for (unsigned int j=0; j<originalMeshBuffer->getVertexCount(); j++)
			if (vertexAdresse[j]==-1)	// ein Original-Vertex muss uebernommen werden
			{
				// Index des neuen Vertex speichern
				vertexAdresse[j] = bufferVertices;
				
				irr::video::S3DVertex& v = reduzierterMeshBuffer->Vertices[bufferVertices++];
				// Werte uebernehmen
				v.Pos.set(originalMeshBuffer->getPosition(j));
				v.Normal.set(originalMeshBuffer->getNormal(j));
				v.TCoords.set(originalMeshBuffer->getTCoords(j));
			}

		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// 3. alle Dreiecke des aktuellen Meshbuffers betrachten: Dreiecke filtern und bei Bedarf uebernehmen
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
			
		for (unsigned int j=0; j<originalMeshBuffer->getIndexCount(); j+=3)
		{
			// original Eck-Indices auslesen
			index1 = originalMeshBuffer->getIndices()[j];
			index2 = originalMeshBuffer->getIndices()[j+1];
			index3 = originalMeshBuffer->getIndices()[j+2];

			// neue Indices auslesen
			index1 = vertexAdresse[index1];
			index2 = vertexAdresse[index2];
			index3 = vertexAdresse[index3];

			// wenn kein degeneriertes Dreieck: uebernehmen
			// also muessen alle Ecken paarweise unterschiedlich sein
			if ((index1 != index2) && (index1 != index3) && (index2 != index3))
			{
				reduzierterMeshBuffer->Indices[bufferIndizes++] = index1;
				reduzierterMeshBuffer->Indices[bufferIndizes++] = index2;
				reduzierterMeshBuffer->Indices[bufferIndizes++] = index3;
			}
		}

		// finale Werte des reduzierten Meshbuffers errechnen
		reduzierterMeshBuffer->Vertices.reallocate(bufferVertices);
		reduzierterMeshBuffer->Indices.reallocate(bufferIndizes);
		reduzierterMeshBuffer->recalculateBoundingBox();

		std::cout << ": Vertices: " << originalMeshBuffer->getVertexCount() << " -> " << reduzierterMeshBuffer->getVertexCount();
		std::cout << " , Dreiecke: " << originalMeshBuffer->getIndexCount()/3 << " -> " << reduzierterMeshBuffer->getIndexCount()/3 << std::endl;

		anzahlVerticesMeshAlt += originalMeshBuffer->getVertexCount();
		anzahlVerticesMeshNeu += reduzierterMeshBuffer->getVertexCount();
		anzahlDreieckeMeshAlt += originalMeshBuffer->getIndexCount()/3;
		anzahlDreieckeMeshNeu += reduzierterMeshBuffer->getIndexCount()/3;

	} // END: fuer alle Meshbuffer des Original-Meshes

	// finale Werte des reduzierten Meshes errechen
	mesh->recalculateBoundingBox();
	// neu erzeugten Mesh speichern
	MeshVoxel.push_back(mesh);

	std::cout << "Gesamtzahl: Vertices: " << anzahlVerticesMeshAlt << " -> " << anzahlVerticesMeshNeu;
	std::cout << " , Dreiecke: " << anzahlDreieckeMeshAlt << " -> " << anzahlDreieckeMeshNeu << std::endl;

	// Berechnung der Normalen der neu-erzeugten Detailstufe
	BerechneNeueDetailstufeNormalen(); 
	
	std::cout << "[Dreiecksmesh]: Reduktion des Dreiecksnetzes abgeschlossen." << std::endl;
	std::cout << "[Dreiecksmesh]: Benoetigte Zeit fuer aktuelle Detailstufe: ";
	Zeitmesser->Stop(0);
}

void DunGen::CDreiecksMesh::BerechneNeueDetailstufeNormalen()
{
	std::cout << "[Dreiecksmesh]: Berechnung der Normalen ...";

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Normalen berechnen und speichern
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
	
	// fuer alle Meshbuffer des zu letzt erzeugten Meshes
	for (unsigned int i=0; i<MeshVoxel.back()->getMeshBufferCount(); i++)
	{
		// Meshbuffer merken
		meshBuffer = MeshVoxel.back()->getMeshBuffer(i);

		// Werte auslesen
		vertexAnzahl = meshBuffer->getVertexCount();
		indexAnzahl = meshBuffer->getIndexCount();
		const irr::u16* indices = meshBuffer->getIndices();

		// alle Normalen auf 0 setzen
		for (unsigned int j=0; j<vertexAnzahl; j++)
		  // wenn kein Grenzvertex
			if (meshBuffer->getTCoords(j).X < 0.0)
				meshBuffer->getNormal(j).set(0.f,0.f,0.f);

		// Normalen berechnen und zusammenaddieren
		for (unsigned int j=0; j<indexAnzahl; j+=3)
		{
			// Normale des aktuellen Dreiecks berechnen
			const irr::core::vector3df& v1 = meshBuffer->getPosition(indices[j]);
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

			// nur fuer Nicht-Grenzvertices aendern
			if (meshBuffer->getTCoords(indices[j]).X < 0.0)
				meshBuffer->getNormal(indices[j]) += irr::core::vector3df(vertex1.X,vertex1.Y,vertex1.Z);
			if (meshBuffer->getTCoords(indices[j+1]).X < 0.0)
				meshBuffer->getNormal(indices[j+1]) += irr::core::vector3df(vertex2.X,vertex2.Y,vertex2.Z);
			if (meshBuffer->getTCoords(indices[j+2]).X < 0.0)
				meshBuffer->getNormal(indices[j+2]) += irr::core::vector3df(vertex3.X,vertex3.Y,vertex3.Z);
		}

		// Normalisieren
		for (unsigned int j=0; j<vertexAnzahl; j++)
			// wenn kein Grenzvertex
			if (meshBuffer->getTCoords(j).X < 0.0)
				meshBuffer->getNormal(j).normalize();
	
	} // ENDE: fuer alle Meshbuffer

	std::cout << " abgeschlossen." << std::endl;
}
