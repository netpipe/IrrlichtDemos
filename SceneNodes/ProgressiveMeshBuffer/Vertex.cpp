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

#include "Vertex.h"
#include "Triangle.h"

using namespace irr;

Vertex::Vertex(irr::u32 id, irr::core::vector3df position, irr::core::vector3df normal, irr::video::SColor color, irr::core::vector2df textureCoordinates) {
	this->id = id;
	this->position = position;
	this->normal = normal;
	this->color = color;
	this->textureCoordinates = textureCoordinates;
	active = true;

	faces = std::list<Triangle*>();

	// Init quadric
	for (irr::u32 i = 0; i < 16; i++) {
		quadric[i] = -1;
	}
}

Vertex::~Vertex() {
	faces.clear();
}

irr::u32 Vertex::getId() const {
	return id;
}

void Vertex::setId(irr::u32 id) {
	this->id = id;
}

irr::core::vector3df Vertex::getPosition() const {
	return position;
}

void Vertex::setPosition(irr::core::vector3df position) {
	this->position = position;
}

irr::core::vector3df Vertex::getNormal() const {
	return normal;
}

irr::video::SColor Vertex::getColor() const {
	return color;
}

void Vertex::setColor(irr::video::SColor color) {
	this->color = color;
}

irr::core::vector2df Vertex::getTextureCoordinates() const {
	return textureCoordinates;
}

void Vertex::setTextureCoordinates(irr::core::vector2df textureCoordinates) {
	this->textureCoordinates = textureCoordinates;
}

bool Vertex::isActive() const {
	return active;
}

void Vertex::setActive(bool active) {
	this->active = active;
}

std::list<Triangle*> Vertex::getFaces() const {
	return faces;
}

void Vertex::addFace(Triangle* triangle) {
	if (triangle && triangle->isActive()) {
		bool add = true;

		// Check if face already exists for this vertex
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

void Vertex::removeFace(Triangle* triangle) {
	for (std::list<Triangle*>::iterator it = faces.begin(); it != faces.end(); it++) {
		Triangle* t = *it;
		if (triangle == t) {
			faces.erase(it);
			break;
		}
	}
}

irr::core::matrix4 Vertex::getQuadric() const {
	return quadric;
}

void Vertex::setQuadric(irr::core::matrix4 quadric) {
	this->quadric = quadric;
}

void Vertex::calcQuadric(bool useTriangleArea) {
	for (irr::u32 i = 0; i < 16; i++) {
		quadric[i] = 0;
	}

	for (std::list<Triangle*>::iterator it = faces.begin(); it != faces.end(); it++) {
		Triangle* t = *it;

		if (t->isActive()) {
			irr::f32 triangleArea = 1;
			if (useTriangleArea) {
				triangleArea = t->getArea();
				quadricTriangleArea += triangleArea;
			}

			const irr::core::vector3df normal = t->getNormal();
			const irr::f32 a = normal.X;
			const irr::f32 b = normal.Y;
			const irr::f32 c = normal.Z;
			const irr::f32 d = normal.dotProduct(t->getVertex1()->getPosition());

			quadric[0] += triangleArea * a * a;
			quadric[1] += triangleArea * a * b;
			quadric[2] += triangleArea * a * c;
			quadric[3] += triangleArea * a * d;

			quadric[4] += triangleArea * b * a;
			quadric[5] += triangleArea * b * b;
			quadric[6] += triangleArea * b * c;
			quadric[7] += triangleArea * b * d;

			quadric[8] += triangleArea * c * a;
			quadric[9] += triangleArea * c * b;
			quadric[10] += triangleArea * c * c;
			quadric[11] += triangleArea * c * d;

			quadric[12] += triangleArea * d * a;
			quadric[13] += triangleArea * d * b;
			quadric[14] += triangleArea * d * c;
			quadric[15] += triangleArea * d * d;
		}
	}
}

irr::f32 Vertex::getQuadricTriangleArea() const {
	return quadricTriangleArea;
}

void Vertex::setQuadricTriangleArea(irr::f32 quadricTriangleArea) {
	this->quadricTriangleArea = quadricTriangleArea;
}

void Vertex::print() const {
	if (active) {
		printf("[%d  - ACTIVE]", id);
	} else {
		printf("[%d  - INACTIVE]", id);
	}
}

bool Vertex::isOrphan() const {
	return (faces.size() == 0);
}

/**
 * Vertex2TCoords
 */
Vertex2TCoords::Vertex2TCoords(u32 id, core::vector3df position, core::vector3df normal, video::SColor color, core::vector2df textureCoordinates, core::vector2df textureCoordinates2) :
		Vertex(id, position, normal, color, textureCoordinates) {
	this->textureCoordinates2 = textureCoordinates2;
}

Vertex2TCoords::~Vertex2TCoords() {
}

core::vector2df Vertex2TCoords::getTextureCoordinates2() const {
	return textureCoordinates2;
}

void Vertex2TCoords::setTextureCoordinates2(core::vector2df textureCoordinates2) {
	this->textureCoordinates2 = textureCoordinates2;
}

/**
 * VertexTangets
 */
VertexTangets::VertexTangets(u32 id, core::vector3df position, core::vector3df normal, video::SColor color, core::vector2df textureCoordinates, core::vector3df tangent, core::vector3df binormal) :
		Vertex(id, position, normal, color, textureCoordinates) {
	this->tangent = tangent;
	this->binormal = binormal;
}

VertexTangets::~VertexTangets() {
}

core::vector3df VertexTangets::getTangent() const {
	return tangent;
}

core::vector3df VertexTangets::getBinormal() const {
	return binormal;
}
