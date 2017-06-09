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

#include "Edge.h"
#include "Triangle.h"

Edge::Edge(Vertex* from, Vertex* to) {
	this->from = from;
	this->to = to;
	cost = 0;

	faces = std::list<Triangle*>();

	updateFaces();
}

Edge::~Edge() {
	faces.clear();
}

Vertex* Edge::getVertexFrom() const {
	return from;
}

Vertex* Edge::getVertexTo() const {
	return to;
}

irr::f32 Edge::getCost() const {
	return cost;
}

void Edge::setCost(irr::f32 cost) {
	this->cost = cost;
}

std::list<Triangle*> Edge::getFaces() const {
	return faces;
}

void Edge::updateFaces() {
	faces.clear();
	// Add faces
	std::list<Triangle*> facesFrom = from->getFaces();
	for (std::list<Triangle*>::iterator it = facesFrom.begin(); it != facesFrom.end(); it++) {
		Triangle* t1 = *it;
		if (t1 && t1->isActive()) {
			std::list<Triangle*> facesTo = to->getFaces();
			for (std::list<Triangle*>::iterator it2 = facesTo.begin(); it2 != facesTo.end(); it2++) {
				Triangle* t2 = *it2;
				if (t1 == t2) {
					faces.push_back(t1);
				}
			}
		}
	}
}

void Edge::addFace(Triangle* triangle) {
	if (triangle && triangle->isActive()) {
		bool add = true;

		// Check if face already exists for this edge
		for (std::list<Triangle*>::iterator it = faces.begin(); it != faces.end(); it++) {
			Triangle* t = *it;
			if (triangle == t) {
				add = false;
				break;
			}
		}
		if (add) {
			faces.push_back(triangle);
		}
	}
}

void Edge::removeFace(Triangle* triangle) {
	for (std::list<Triangle*>::iterator it = faces.begin(); it != faces.end(); it++) {
		Triangle* t = *it;
		if (triangle == t) {
			faces.erase(it);
			break;
		}
	}
}

bool Edge::isBorder() const {
	return (faces.size() == 1);
}

bool Edge::hasVertex(Vertex* v) const {
	return (v == from || v == to);
}

void Edge::replaceVertex(Vertex* from, Vertex* to) {
	// Replace from with to vertex
	if (this->from == from) {
		this->from = to;
	} else if (this->to == from) {
		this->to = to;
	}

	// Check faces
	updateFaces();
}

bool Edge::isValid() const {
	// One of the vertexes active
	if (!from || !to) {
		return false;
	}
	if (!from->isActive() || !to->isActive()) {
		return false;
	}
	if (from == to) {
		return false;
	}
	if (faces.size() == 0) {
		return false;
	}
	return true;
}

void Edge::print() const {
	printf("Edge: ");
	from->print();
	printf(" -> ");
	to->print();
	printf(" Cost: %f Faces: %d\n", cost, faces.size());
}
