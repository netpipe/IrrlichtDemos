/*
 Copyright (C) 2011-2012 Thijs Ferket (RdR)

 This software is provided 'as-is', without any express or implied
 warranty.  In no event will the authors be held liable for any damages
 arising from the use of this software.

 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it
 freely, subject to the following restrictions:

 1. The origin of this software must not be misrepresented; you must not
 claim that you wrote the original software. If you use this software
 in a product, an acknowledgment in the product documentation would be
 appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be
 misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

#include "ProgressiveMeshBuffer.h"

static const irr::u32 BOUNDARY_WEIGHT = 1000;

namespace irr {
namespace scene {

ProgressiveMeshBuffer::ProgressiveMeshBuffer(irr::scene::IMeshBuffer* meshBuffer, SimplificationAlgorithm algorithm) {
	this->algorithm = algorithm;
	this->meshBuffer = meshBuffer;
	this->meshBuffer->grab();

	// Create vertices
	if (meshBuffer->getVertexType() == video::EVT_STANDARD) {
		video::S3DVertex* verts = (video::S3DVertex*) meshBuffer->getVertices();
		for (irr::u32 i = 0; i < meshBuffer->getVertexCount(); i++) {
			vertices.push_back(new Vertex(i, verts[i].Pos, verts[i].Normal, verts[i].Color, verts[i].TCoords));
		}
	}
	// Vertex with two texture coordinates
	else if (meshBuffer->getVertexType() == video::EVT_2TCOORDS) {
		video::S3DVertex2TCoords* verts = (video::S3DVertex2TCoords*) meshBuffer->getVertices();
		for (irr::u32 i = 0; i < meshBuffer->getVertexCount(); i++) {
			vertices.push_back(new Vertex2TCoords(i, verts[i].Pos, verts[i].Normal, verts[i].Color, verts[i].TCoords, verts[i].TCoords2));
		}
	}
	// Vertex with a tangent and binormal vector
	else if (meshBuffer->getVertexType() == video::EVT_TANGENTS) {
		video::S3DVertexTangents* verts = (video::S3DVertexTangents*) meshBuffer->getVertices();
		for (irr::u32 i = 0; i < meshBuffer->getVertexCount(); i++) {
			vertices.push_back(new VertexTangets(i, verts[i].Pos, verts[i].Normal, verts[i].Color, verts[i].TCoords, verts[i].Tangent, verts[i].Binormal));
		}
	}

	// Create triangles
	u16* indices = meshBuffer->getIndices();
	for (irr::u32 i = 0; i < meshBuffer->getIndexCount(); i += 3) {
		Triangle* triangle = new Triangle(vertices[indices[i]], vertices[indices[i + 1]], vertices[indices[i + 2]]);
		triangles.push_back(triangle);
	}

	// Calculate quadric for all vertices
	if (algorithm == QUADRICTRI) {
		calculateQuadric(true);
	} else if (algorithm == QUADRIC) {
		calculateQuadric(false);
	}

	// Create edges
	createEdges();

	// Calculate edge costs
	calculateEdgeCost();

#ifdef _DEBUG
	// Print some info
	printf("Creating ProgressiveMeshBuffer [%s] [Triangles: %d  Vertices: %d  Indices: %d  Edges: %d ]\n", getAlgorithmString().c_str(), triangles.size(), vertices.size(), meshBuffer->getIndexCount(), edges.size());
#endif
}

ProgressiveMeshBuffer::~ProgressiveMeshBuffer() {
	meshBuffer->drop();

	// Delete all Edges
	for( irr::u32 i = 0; i < edges.size(); ++i ) {
		Edge* e = edges[i];
		delete e;
	}
	edges.clear();

	// Delete all Triangles
	for( irr::u32 i = 0; i < triangles.size(); ++i ) {
		Triangle* t = triangles[i];
		delete t;
	}
	triangles.clear();

	// Delete all Vertices
	for( irr::u32 i = 0; i < vertices.size(); ++i ) {
		Vertex* v = vertices[i];
		delete v;
	}
	vertices.clear();
}

const irr::core::array<Triangle*>& ProgressiveMeshBuffer::getTriangles() const {
	return triangles;
}

const irr::core::array<Vertex*>& ProgressiveMeshBuffer::getVertices() const {
	return vertices;
}

IMeshBuffer* ProgressiveMeshBuffer::getCurrentMeshBuffer() {
	scene::IDynamicMeshBuffer* mb = new scene::CDynamicMeshBuffer(meshBuffer->getVertexType(), irr::video::EIT_16BIT);

	// Add vertices to buffer
	irr::u32 vertexIndex = 0;
	if (meshBuffer->getVertexType() == video::EVT_STANDARD) {
		for( irr::u32 i = 0; i < vertices.size(); ++i ) {
			Vertex* v = vertices[i];
			if (v->isOrphan()) {
				v->setActive(false);
			} else if (v && v->isActive()) {
				v->setId(vertexIndex); // Change ID
				mb->getVertexBuffer().push_back(video::S3DVertex(v->getPosition(), v->getNormal(), v->getColor(), v->getTextureCoordinates()));
				vertexIndex++;
			}
		}
	}
	// Vertex with two texture coordinates
	else if (meshBuffer->getVertexType() == video::EVT_2TCOORDS) {
		for( irr::u32 i = 0; i < vertices.size(); ++i ) {
			Vertex2TCoords* v = (Vertex2TCoords*) vertices[i];
			if (v->isOrphan()) {
				v->setActive(false);
			} else if (v && v->isActive()) {
				v->setId(vertexIndex); // Change ID
				mb->getVertexBuffer().push_back(video::S3DVertex2TCoords(v->getPosition(), v->getNormal(), v->getColor(), v->getTextureCoordinates(), v->getTextureCoordinates2()));
				vertexIndex++;
			}
		}
	}
	// Vertex with a tangent and binormal vector
	else if (meshBuffer->getVertexType() == video::EVT_TANGENTS) {
		for( irr::u32 i = 0; i < vertices.size(); ++i ) {
			VertexTangets* v = (VertexTangets*) vertices[i];
			if (v->isOrphan()) {
				v->setActive(false);
			} else if (v && v->isActive()) {
				v->setId(vertexIndex); // Change ID
				mb->getVertexBuffer().push_back(video::S3DVertexTangents(v->getPosition(), v->getNormal(), v->getColor(), v->getTextureCoordinates(), v->getTangent(), v->getBinormal()));
				vertexIndex++;
			}
		}
	}

	// Add indices to buffer
	for( irr::u32 i = 0; i < triangles.size(); ++i ) {
		const Triangle* t = triangles[i];
		if (t && t->isActive()) {

#ifdef _DEBUG
			if (!t->getVertex(0)->isActive() || !t->getVertex(1)->isActive() || !t->getVertex(2)->isActive()) {
				t->print();
				continue;
			}
#endif

			mb->getIndexBuffer().push_back(t->getVertex(0)->getId());
			mb->getIndexBuffer().push_back(t->getVertex(1)->getId());
			mb->getIndexBuffer().push_back(t->getVertex(2)->getId());
		}
	}

#ifdef _DEBUG
	printf("Building MeshBuffer [ Triangles: %d  Vertices: %d  Indices: %d  Edges: %d ]\n", mb->getIndexBuffer().size() / 3, mb->getVertexBuffer().size(), mb->getIndexBuffer().size(), edges.size());
#endif

	// Set mesh buffer material
	mb->getMaterial() = meshBuffer->getMaterial();

	// Recalculate bounding box
	mb->recalculateBoundingBox();

	return mb;
}

void ProgressiveMeshBuffer::calculateQuadric(bool useTriangleArea) {
	for( irr::u32 i = 0; i < vertices.size(); ++i ) {
		Vertex* vertex = vertices[i];

		// Calculate quadric
		vertex->calcQuadric(useTriangleArea);
	}
}

void ProgressiveMeshBuffer::createEdges() {
	irr::core::map<irr::core::stringw, Edge*> uniqueEdges;
	for( irr::u32 i = 0; i < triangles.size(); ++i ) {
		const Triangle* t = triangles[i];

		// Add 3 edges
		for (irr::u32 j = 0; j < 3; j++) {
			Vertex* vertexFrom = 0;
			Vertex* vertexTo = 0;

			if (j == 0) {
				vertexFrom = t->getVertex(0);
				vertexTo = t->getVertex(1);
			} else if (j == 1) {
				vertexFrom = t->getVertex(1);
				vertexTo = t->getVertex(2);
			} else {
				vertexFrom = t->getVertex(2);
				vertexTo = t->getVertex(0);
			}

			// Create id string
			irr::core::stringw string = "";
			if (vertexFrom->getId() < vertexTo->getId()) {
				string += vertexFrom->getId();
				string += ";";
				string += vertexTo->getId();
			} else {
				string += vertexTo->getId();
				string += ";";
				string += vertexFrom->getId();
			}

			// If edge is unique add it to the list			
			irr::core::map<irr::core::stringw, Edge*>::Node* n = uniqueEdges.find( string );
			if( !n )
			{
				Edge* edge = new Edge(vertexFrom, vertexTo);
				uniqueEdges.insert(string, edge);
				edges.push_back(edge);
			}
		} // for j
	}
}

void ProgressiveMeshBuffer::calculateEdgeCost() {
	// Calculate edge cost for each edge
	for( irr::u32 i = 0; i < edges.size(); ++i ) {
		Edge* edge = edges[i];
		calculateEdgeCost(edge);
	}
}

void ProgressiveMeshBuffer::calculateEdgeCost(Edge* edge) {
	switch (algorithm) {
	case RANDOM:
		calculateRandomEdgeCost(edge);
		break;
	case SHORTEST:
		calculateShortEdgeCost(edge);
		break;
	case MELAX:
		calculateMelaxEdgeCost(edge);
		break;
	case QUADRIC:
	case QUADRICTRI:
		calculateQuadricEdgeCost(edge);
		break;
	default:
		break;
	};
}

void ProgressiveMeshBuffer::calculateRandomEdgeCost(Edge* edge) {
	// Calculate random edge cost
	const f32 cost = f32(rand() % edges.size());
	edge->setCost(cost);
}

void ProgressiveMeshBuffer::calculateShortEdgeCost(Edge* edge) {
	// Calculate the length of the edge
	const f32 edgeLength = (edge->getVertexTo()->getPosition() - edge->getVertexFrom()->getPosition()).getLength();
	edge->setCost(edgeLength);
}

void ProgressiveMeshBuffer::calculateMelaxEdgeCost(Edge* edge) {
	// Calculate the collapse edge how much different will the model change, i.e. the 'error'
	// Calculate the length of the edge
	const f32 edgeLength = (edge->getVertexTo()->getPosition() - edge->getVertexFrom()->getPosition()).getLength();
	f32 curvature = 0;

	const irr::core::array<Triangle*> facesEdge = edge->getFaces();
	const irr::core::array<Triangle*> facesFrom = edge->getVertexFrom()->getFaces();

	// Use the triangle facing most away from the sides to determine our curvature term
	for (irr::u32 i  = 0; i < facesFrom.size(); ++i) {
		const Triangle* t1 = facesFrom[i];
		f32 minCurve = 1;

		for (irr::u32 i2  = 0; i2 < facesEdge.size(); ++i2) {
			const Triangle* t2 = facesEdge[i2];

			// Use dot product of face normals
			const f32 dotProduct = t1->getNormal().dotProduct(t2->getNormal());
			minCurve = core::min_(minCurve, (1 - dotProduct) / 2.0f);
		}
		curvature = core::max_(curvature, minCurve);
	}

	const f32 cost = edgeLength * curvature;
	edge->setCost(cost);
}

void ProgressiveMeshBuffer::calculateQuadricEdgeCost(Edge* edge) {
	// Apply border penalties
	if (edge->isBorder()) {
		applyBorderPenalties(edge);
	}

	const core::matrix4 quadric1 = edge->getVertexFrom()->getQuadric();
	const core::matrix4 quadric2 = edge->getVertexTo()->getQuadric();
	const core::matrix4 quadricSum = quadric1 + quadric2;

	f32 triangleArea = 0;
//	if (algorithm == QUADRICTRI) {
//		triangleArea = edge->getVertexFrom()->getQuadricTriangleArea() + edge->getVertexTo()->getQuadricTriangleArea();
//	}

	Vertex* v = edge->getVertexTo(); // Calculate ~v

	const f32 cost = calcQuadricError(quadricSum, v, triangleArea);
	edge->setCost(cost);
}

void ProgressiveMeshBuffer::applyBorderPenalties(Edge* edge) {
	core::vector3df pos1 = edge->getVertexFrom()->getPosition();
	core::vector3df pos2 = edge->getVertexTo()->getPosition();
	core::vector3df distance = pos1 - pos2;
	core::vector3df normal = edge->getFaces()[0]->getNormal();
	core::vector3df abc = distance.crossProduct(normal);
	abc.normalize();

	f32& a = abc.X;
	f32& b = abc.Y;
	f32& c = abc.Z;

	f32 d = -(abc.dotProduct(pos1));

	core::matrix4 quadricConstraint;
	quadricConstraint[0] = BOUNDARY_WEIGHT * a * a;
	quadricConstraint[1] = BOUNDARY_WEIGHT * a * b;
	quadricConstraint[2] = BOUNDARY_WEIGHT * a * c;
	quadricConstraint[3] = BOUNDARY_WEIGHT * a * d;

	quadricConstraint[4] = BOUNDARY_WEIGHT * b * a;
	quadricConstraint[5] = BOUNDARY_WEIGHT * b * b;
	quadricConstraint[6] = BOUNDARY_WEIGHT * b * c;
	quadricConstraint[7] = BOUNDARY_WEIGHT * b * d;

	quadricConstraint[8] = BOUNDARY_WEIGHT * c * a;
	quadricConstraint[9] = BOUNDARY_WEIGHT * c * b;
	quadricConstraint[10] = BOUNDARY_WEIGHT * c * c;
	quadricConstraint[11] = BOUNDARY_WEIGHT * c * d;

	quadricConstraint[12] = BOUNDARY_WEIGHT * d * a;
	quadricConstraint[13] = BOUNDARY_WEIGHT * d * b;
	quadricConstraint[14] = BOUNDARY_WEIGHT * d * c;
	quadricConstraint[15] = BOUNDARY_WEIGHT * d * d;

	// Now add the constraint quadric to the quadrics for both of the vertices.
	core::matrix4 quadric1 = edge->getVertexFrom()->getQuadric();
	core::matrix4 quadric2 = edge->getVertexTo()->getQuadric();

	quadric1 += quadricConstraint;
	quadric2 += quadricConstraint;

	edge->getVertexFrom()->setQuadric(quadric1);
	edge->getVertexTo()->setQuadric(quadric2);
}

f32 ProgressiveMeshBuffer::calcQuadricError(core::matrix4 quadric, Vertex* v, f32 triangleArea) {
	f32 cost = 0;

	// 1st, consider vertex v a 1x4 matrix: [v.x v.y v.z 1]
	// Multiply it by the Qsum 4x4 matrix, resulting in a 1x4 matrix

	f32 result[4];

	const core::vector3df pos = v->getPosition();

	result[0] = pos.X * quadric[0] + pos.Y * quadric[4] + pos.Z * quadric[8] + 1 * quadric[12];
	result[1] = pos.X * quadric[1] + pos.Y * quadric[5] + pos.Z * quadric[9] + 1 * quadric[13];
	result[2] = pos.X * quadric[2] + pos.Y * quadric[6] + pos.Z * quadric[10] + 1 * quadric[14];
	result[3] = pos.X * quadric[3] + pos.Y * quadric[7] + pos.Z * quadric[11] + 1 * quadric[15];

	// Multiply this 1 x 4 matrix by the vertex v transpose (a 4 x 1 matrix).
	// This is just the dot product.

	cost = result[0] * pos.X + result[1] * pos.Y + result[2] * pos.Z + result[3] * 1;

	if (triangleArea != 0) {
		cost /= triangleArea;
	}

	return cost;
}

void ProgressiveMeshBuffer::contract(Edge* edge) {
	if (!edge->isValid()) {
		printf("Edge invalid!\n");
		return;
	}

#ifdef _DEBUG
	//printf("[Edges: %d] Contracting ", edges.size());
	//edge->print();
#endif

	// Set from vertex in active
	edge->getVertexFrom()->setActive(false);

	// Get center position v // TODO use better algorithm for position
	if (algorithm == QUADRIC || algorithm == QUADRICTRI) {
		core::vector3df newPos = (edge->getVertexFrom()->getPosition() + edge->getVertexTo()->getPosition()) * 0.5f;
		core::vector2df centerTextureCoordinates = (edge->getVertexFrom()->getTextureCoordinates() + edge->getVertexTo()->getTextureCoordinates()) * 0.5f;

		// Replace position on vertex to
		edge->getVertexTo()->setPosition(newPos);

		// Set texture coordinates on vertex to
		edge->getVertexTo()->setTextureCoordinates(centerTextureCoordinates);
	}

	// Recalculate quadric for vertex
	if (algorithm == QUADRIC || algorithm == QUADRICTRI) {
		edge->getVertexTo()->setQuadric(edge->getVertexTo()->getQuadric() + edge->getVertexFrom()->getQuadric());
	}

	// Replace all vertex from with vertex to
	const irr::core::array<Triangle*> facesFrom = edge->getVertexFrom()->getFaces();
	for (irr::u32 i  = 0; i < facesFrom.size(); ++i) {
		Triangle* t = facesFrom[i];
		t->replaceVertex(edge->getVertexFrom(), edge->getVertexTo());
	}

	// Delete edge faces/triangles
	const irr::core::array<Triangle*> faces = edge->getFaces();
	for (irr::u32 i  = 0; i < faces.size(); ++i) {
		Triangle* t = faces[i];

		// Delete triangle
		t->setActive(false);
		s32 id = triangles.linear_search( t );
		if( id != -1 )
			triangles.erase(id);
		delete t;
	}

	// Recalculate quadric for vertex to
	if (algorithm == QUADRICTRI) {
		edge->getVertexTo()->calcQuadric(true);
	} else if (algorithm == QUADRIC) {
		edge->getVertexTo()->calcQuadric(false);
	}

	irr::core::map<irr::core::stringw, Edge*> uniqueEdges;

	// Replace vertex in all edges which have vertex from and update cost
	for (irr::u32 i  = 0; i < edges.size(); ++i) {

		Edge* e = edges[i];

		// Recalculate edge cost and update faces
		if (e->hasVertex(edge->getVertexFrom()) || e->hasVertex(edge->getVertexTo())) {
			if (e->hasVertex(edge->getVertexFrom())) {
				e->replaceVertex(edge->getVertexFrom(), edge->getVertexTo());
			} else {
				e->updateFaces();
			}

			// Delete edge is its not valid anymore
			if (!e->isValid()) {
				edges.erase(i);
				//next = false;
				delete e;
				--i;
			}
			// Check if edge is unique
			else {
				// Get id string
				irr::core::stringw string = e->getIdString();

				// If edge is unique add it to the list
				irr::core::map<irr::core::stringw, Edge*>::Node* n = uniqueEdges.find( string );
				if( !n )
				{
					uniqueEdges.insert(string, 0);

					// Recalculate edge cost
					calculateEdgeCost(e);
				}
				// Delete edge because its not unique
				else {
					edges.erase(i);
					//next = false;
					delete e;
					--i;
				}
			}
		}
	}

	// Delete edge
	delete edge;
}

void ProgressiveMeshBuffer::contract(irr::u32 numberOfEdges) {
	for (irr::u32 i = 0; i < numberOfEdges && triangles.size() > 0; ++i) {
		//Edge* edgeIt = edges[0];
		Edge* edge = edges[0];

		// Find edge with lowest cost
		u32 edgeIt = 0;
		for (irr::u32 j  = 0; j < edges.size(); ++j) {
			Edge* e = edges[j];
			if (e->getCost() < edge->getCost()) {
				edge = e;
				edgeIt = j;
			}
		}
		edges.erase(edgeIt);

		// Contract edge
		contract(edge);
	}
}

void ProgressiveMeshBuffer::contractTillTriangleCount(irr::u32 numberOfTriangles) {
	while (triangles.size() > numberOfTriangles) {
		contract(1);
	}
}

ProgressiveMeshBuffer::SimplificationAlgorithm ProgressiveMeshBuffer::getAlgorithm() const {
	return algorithm;
}

core::stringc ProgressiveMeshBuffer::getAlgorithmString() const {
	core::stringc algorithmString = "";
	switch (algorithm) {
	case RANDOM:
		algorithmString = "Random";
		break;
	case SHORTEST:
		algorithmString = "Shortest Edge";
		break;
	case MELAX:
		algorithmString = "Melax";
		break;
	case QUADRIC:
		algorithmString = "Quadric";
		break;
	case QUADRICTRI:
		algorithmString = "Quadric Weighted by Triangle Area";
		break;
	default:
		break;
	};
	return algorithmString;
}

}
}