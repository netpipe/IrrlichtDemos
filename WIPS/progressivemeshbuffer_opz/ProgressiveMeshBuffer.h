/*
 * ProgressiveMeshBuffer.h
 *
 * ProgressiveMeshBuffer can simplify meshes using edge contraction
 * The following algorithms are implemented:
 * - Random edge contraction
 * - Shortest edge contraction
 * - Melax using the "Stan Melax" method.
 * - Quadric using the "Garland & Heckbert Quadrics" method.
 *
 * Tank @ War Project
 * October 2011
 */

#ifndef PROGRESSIVE_MESH_BUFFER_H_
#define PROGRESSIVE_MESH_BUFFER_H_

#include <irrlicht.h>
#include "Vertex.h"
#include "Triangle.h"
#include "Edge.h"

namespace irr {
namespace scene {

class ProgressiveMeshBuffer {
public:
	/**
	 * Type of simplification algorithm
	 */
	enum SimplificationAlgorithm {
		/**
		 * Random
		 */
		RANDOM,
		/**
		 * Shortest
		 */
		SHORTEST,
		/**
		 * Melax
		 */
		MELAX,
		/**
		 * Quadric
		 */
		QUADRIC,
		/**
		 * Quadric triangle
		 */
		QUADRICTRI
	};
private:
	scene::IMeshBuffer* meshBuffer;
	SimplificationAlgorithm algorithm;
	irr::core::array<Vertex*> vertices;
	irr::core::array<Triangle*> triangles;
	irr::core::array<Edge*> edges;

public:
	/**
	 * Constructor
	 * @param meshBuffer
	 * @param algorithm
	 */
	ProgressiveMeshBuffer(scene::IMeshBuffer* meshBuffer, SimplificationAlgorithm algorithm = QUADRICTRI);

	/**
	 * Destructor
	 */
	virtual ~ProgressiveMeshBuffer();

	/**
	 * Get triangles
	 * @return triangles
	 */
	const irr::core::array<Triangle*>& getTriangles() const;

	/**
	 * Get vertices
	 * @return vertices
	 */
	const irr::core::array<Vertex*>& getVertices() const;

	/**
	 * Build mesh buffer from current triangles
	 * @return mb
	 */
	scene::IMeshBuffer* getCurrentMeshBuffer();

	/**
	 * Contract number of edges
	 * @param numberOfEdges
	 */
	void contract(u32 numberOfEdges = 1);

	/**
	 * Contract edges till number of triangles is reached
	 * @param numberOfTriangles
	 */
	void contractTillTriangleCount(u32 numberOfTriangles);

	/**
	 * Get algorithm
	 * @return algorithm
	 */
	ProgressiveMeshBuffer::SimplificationAlgorithm getAlgorithm() const;

	/**
	 * Get algorithm string
	 * @return algorithmString
	 */
	core::stringc getAlgorithmString() const;

private:

	void calculateQuadric(bool useTriangleArea = true);
	/**
	 * Create a vector heap with edges
	 */
	void createEdges();

	/**
	 * Calculate all edge collapse cost
	 */
	void calculateEdgeCost();

	/**
	 * Calculate edge collapse cost for single edge
	 * @param edge
	 */
	void calculateEdgeCost(Edge* edge);

	/**
	 * Calculate edge cost randomly
	 * @param edge
	 */
	void calculateRandomEdgeCost(Edge* edge);

	/**
	 * Calculate edge cost using the "shortest edge" method.
	 * @param edge
	 */
	void calculateShortEdgeCost(Edge* edge);

	/**
	 * Calculate edge cost using the "Stan Melax" method.
	 * @param edge
	 */
	void calculateMelaxEdgeCost(Edge* edge);

	/**
	 * Calculate edge cost using the "Garland & Heckbert Quadrics" method.
	 * @param edge
	 */
	void calculateQuadricEdgeCost(Edge* edge);

	/**
	 * Apply penalty is an edge is border
	 * @param edge
	 */
	void applyBorderPenalties(Edge* edge);

	/*
	 * Calculate the quadric error if using that edge collapse algorithm.  We're calculating
	 *
	 * T
	 * v  Q v
	 *
	 * This is the vertex multiplied by the 4x4 Q matrix, multiplied by the vertex again.
	 */
	f32 calcQuadricError(core::matrix4 quadric, Vertex* v, f32 triangleArea);

	/**
	 * Contract edge
	 * @param edge
	 */
	void contract(Edge* edge);

};

} // end namespace scene

} // end namespace irr

#endif /* PROGRESSIVE_MESH_BUFFER_H_ */
