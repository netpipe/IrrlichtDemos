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

#include "Triangle.h"

using namespace irr;

Triangle::Triangle(Vertex* v1, Vertex* v2, Vertex* v3) {
	vertex[0] = v1;
	vertex[1] = v2;
	vertex[2] = v3;

	active = true;

	for (u32 i = 0; i < 3; i++) {
		// Add this triangle / face for each vertex
		vertex[i]->addFace(this);
	}
}

Triangle::~Triangle() {
	vertex[0]->removeFace(this);
	vertex[1]->removeFace(this);
	vertex[2]->removeFace(this);
}

Vertex* Triangle::getVertex1() const {
	return vertex[0];
}

Vertex* Triangle::getVertex2() const {
	return vertex[1];
}

Vertex* Triangle::getVertex3() const {
	return vertex[2];
}

Vertex* Triangle::getVertex(u32 index) const {
	if (index > 3) {
		index = 0;
	}
	return vertex[index];
}

bool Triangle::isActive() const {
	return active;
}

void Triangle::setActive(bool active) {
	this->active = active;
}

core::vector3df Triangle::getNormal() const {
	return (vertex[1]->getPosition() - vertex[0]->getPosition()).crossProduct(vertex[2]->getPosition() - vertex[0]->getPosition());
}

f32 Triangle::getArea() const {
	return (vertex[1]->getPosition() - vertex[0]->getPosition()).crossProduct(vertex[2]->getPosition() - vertex[0]->getPosition()).getLength() * 0.5f;
}

bool Triangle::hasVertex(Vertex* v) const {
	return (v == vertex[0] || v == vertex[1] || v == vertex[2]);
}

void Triangle::replaceVertex(Vertex* from, Vertex* to) {
	// Remove this triangle from from vertex
	from->removeFace(this);

	// Replace from with to vertex
	for (u32 i = 0; i < 3; i++) {
		if (vertex[i] == from) {
			vertex[i] = to;
			vertex[i]->addFace(this);
			break;
		}
	}
}

void Triangle::print() const {
	printf("Triangle: ");
	for (u32 i = 0; i < 3; i++) {
		vertex[i]->print();
		if (i < 2) {
			printf(" -> ");
		} else {
			printf("\n");
		}
	}
}
