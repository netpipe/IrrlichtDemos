/*
 * Edge.h
 *
 * Tank @ War Project
 * October 2011
 */

#ifndef EDGE_H_
#define EDGE_H_

#include <irrlicht.h>
#include <list>
#include "Vertex.h"

class Edge {
private:
	Vertex* from;
	Vertex* to;
	irr::f32 cost;
	std::list<Triangle*> faces;
public:
	/**
	 * Constructor
	 * @param from vertex
	 * @param to vertex
	 */
	Edge(Vertex* from, Vertex* to);

	/**
	 * Destructor
	 */
	virtual ~Edge();

	/**
	 * Get vertex from
	 * @return from vertex
	 */
	Vertex* getVertexFrom() const;

	/**
	 * Get vertex to
	 * @return to vertex
	 */
	Vertex* getVertexTo() const;

	/**
	 * Get cost to collapse this edge
	 * @return cost
	 */
	irr::f32 getCost() const;

	/**
	 * Set edge collapse cost
	 * @param cost
	 */
	void setCost(irr::f32 cost);

	/**
	 * Check if this edge is a border
	 * Normally there will be two faces / triangles in commom, but if there is only one, then this Edge is a border
	 */
	bool isBorder() const;

	/**
	 * Get faces
	 * @return faces
	 */
	std::list<Triangle*> getFaces() const;

	/**
	 * Add all in common faces of to and from vertex
	 */
	void updateFaces();

	/**
	 * Add a face
	 * @param triangle
	 */
	void addFace(Triangle* triangle);

	/**
	 * Remove a face
	 * @param triangle
	 */
	void removeFace(Triangle* triangle);

	/**
	 * Return true if this edge has vertex
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
	 * Checks if this edge is valid
	 * Has 2 different vertices
	 * Both vertices are still active
	 * @return valid
	 */
	bool isValid() const;

	/**
	 * Print edge info
	 */
	void print() const;

	/**
	 * less than operator, used to sort the Edges
	 */
	bool operator<(const Edge& other) const {
		return (getCost() < other.getCost());
	}

	/**
	 * Equal operator, used to sort the Edges
	 */
	bool operator==(const Edge& other) const {
		return (getCost() == other.getCost());
	}
};

#endif /* EDGE_H_ */
