/*
 * Triangle.h
 *
 * Tank @ War Project
 * October 2011
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <irrlicht.h>
#include "Vertex.h"
#include "Edge.h"

class Triangle {
private:
	Vertex* vertex[3];
	bool active;
public:
	/**
	 * Constructor of Triangle
	 * @param v1
	 * @param v2
	 * @param v3
	 */
	Triangle(Vertex* v1, Vertex* v2, Vertex* v3);

	/**
	 * Destructor
	 */
	virtual ~Triangle();

	/**
	 * Get vertex 1
	 * @return vertex
	 */
	Vertex* getVertex1() const;

	/**
	 * Get vertex 2
	 * @return vertex
	 */
	Vertex* getVertex2() const;

	/**
	 * Get vertex 3
	 * @return vertex
	 */
	Vertex* getVertex3() const;

	/**
	 * Get vertex by index
	 * @param index
	 * @return vertex
	 */
	Vertex* getVertex(irr::u32 index) const;

	/**
	 * Return active
	 * @return active
	 */
	bool isActive() const;

	/**
	 * Set triangle active
	 * @param active
	 */
	void setActive(bool active);

	/**
	 * Get triangle normal
	 * @return normal
	 */
	irr::core::vector3df getNormal() const;

	/**
	 * Get triangle area
	 * @return area
	 */
	irr::f32 getArea() const;

	/**
	 * Return true if this triangle has vertex
	 * @param v
	 */
	bool hasVertex(Vertex* v) const;

	/**
	 * Replace from with to vertex
	 * @param from
	 * @param to
	 */
	void replaceVertex(Vertex* from, Vertex* to);

	/**
	 * Print triangle info
	 */
	void print() const;
};

#endif /* TRIANGLE_H_ */

