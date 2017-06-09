/*
 * Vertex.h
 *
 * Tank @ War Project
 * October 2011
 */

#ifndef VERTEX_H_
#define VERTEX_H_

#include <irrlicht.h>
#include <list>

class Triangle;

class Vertex {
private:
	irr::u32 id;
	irr::core::vector3df position;
	irr::core::vector3df normal;
	irr::video::SColor color;
	irr::core::vector2df textureCoordinates;
	bool active; // false if vertex has been removed
	std::list<Triangle*> faces; // triangles of which this vertex is part of
	irr::core::matrix4 quadric; // Used for Quadric error cost.
	irr::f32 quadricTriangleArea; // summed area of triangles used to computer quadrics

public:
	/**
	 * Constructor
	 * @param id
	 * @param position
	 * @param normal
	 * @param color
	 * @param textureCoordinates
	 */
	Vertex(irr::u32 id, irr::core::vector3df position, irr::core::vector3df normal, irr::video::SColor color = irr::video::SColor(255, 255, 255, 255), irr::core::vector2df textureCoordinates = irr::core::vector2df(0, 0));

	/**
	 * Destructor
	 */
	virtual ~Vertex();

	/**
	 * Get vertex id
	 * @return id
	 */
	irr::u32 getId() const;

	/**
	 * Set vertex id
	 * @param id
	 */
	void setId(irr::u32 id);

	/**
	 * Get vertex position
	 * @return position
	 */
	irr::core::vector3df getPosition() const;

	/**
	 * Set vertex position
	 * @param position
	 */
	void setPosition(irr::core::vector3df position);

	/**
	 * Get normal
	 * @return normal
	 */
	irr::core::vector3df getNormal() const;

	/**
	 * Get vertex color
	 * @return color
	 */
	irr::video::SColor getColor() const;

	/**
	 * set vertex color
	 * @param color
	 */
	void setColor(irr::video::SColor color);

	/**
	 * get vertex texture coordinates
	 * @return textureCoordinates
	 */
	irr::core::vector2df getTextureCoordinates() const;

	/**
	 * Set vertex texture coordinates
	 * @param textureCoordinates
	 */
	void setTextureCoordinates(irr::core::vector2df textureCoordinates);

	/**
	 * Return true if vertex is active
	 * @return active
	 */
	bool isActive() const;

	/**
	 * Set active
	 * @param active
	 */
	void setActive(bool active);

	/**
	 * Get faces
	 * @return faces
	 */
	std::list<Triangle*> getFaces() const;

	/**
	 * Add face
	 * @param triangle
	 */
	void addFace(Triangle* triangle);

	/**
	 * Remove face
	 * @param triangle
	 */
	void removeFace(Triangle* triangle);

	/**
	 * Get quadric
	 * @return quadric
	 */
	irr::core::matrix4 getQuadric() const;
	/**
	 * Set quadric
	 * @param quadric
	 */
	void setQuadric(irr::core::matrix4 quadric);

	/**
	 * Calculate quadric
	 * @param useTriangleArea
	 */
	void calcQuadric(bool useTriangleArea);

	/**
	 * Get quadric triangle area
	 * @return quadricTriangleArea
	 */
	irr::f32 getQuadricTriangleArea() const;

	/**
	 * Set quadric triangle area
	 * @param quadricTriangleArea
	 */
	void setQuadricTriangleArea(irr::f32 quadricTriangleArea);

	/**
	 * Print vertex info
	 */
	void print() const;

	/**
	 * Check if vertex is orphan
	 * @return bool
	 */
	bool isOrphan() const;
};

class Vertex2TCoords: public Vertex {
private:
	irr::core::vector2df textureCoordinates2;
public:
	/**
	 * Constructor
	 * @param id
	 * @param position
	 * @param normal
	 * @param color
	 * @param textureCoordinates
	 * @param textureCoordinates2
	 */
	Vertex2TCoords(irr::u32 id, irr::core::vector3df position, irr::core::vector3df normal, irr::video::SColor color = irr::video::SColor(255, 255, 255, 255), irr::core::vector2df textureCoordinates = irr::core::vector2df(), irr::core::vector2df textureCoordinates2 = irr::core::vector2df());

	/**
	 * Destructor
	 */
	virtual ~Vertex2TCoords();

	/**
	 * Get vertex texture coordinates 2
	 * @return textureCoordinates2
	 */
	irr::core::vector2df getTextureCoordinates2() const;

	/**
	 * Set vertex texture coordinates 2
	 * @param textureCoordinates2
	 */
	void setTextureCoordinates2(irr::core::vector2df textureCoordinates2);
};

class VertexTangets: public Vertex {
private:
	irr::core::vector3df tangent;
	irr::core::vector3df binormal;

public:
	/**
	 * Constructor
	 * @param id
	 * @param position
	 * @param normal
	 * @param color
	 * @param textureCoordinates
	 * @param tangent
	 * @param binormal
	 */
	VertexTangets(irr::u32 id, irr::core::vector3df position, irr::core::vector3df normal, irr::video::SColor color = irr::video::SColor(255, 255, 255, 255), irr::core::vector2df textureCoordinates = irr::core::vector2df(), irr::core::vector3df tangent = irr::core::vector3df(),
			irr::core::vector3df binormal = irr::core::vector3df());

	/**
	 * Destructor
	 */
	virtual ~VertexTangets();

	/**
	 * Get tanget
	 * @return tangent
	 */
	irr::core::vector3df getTangent() const;

	/**
	 * Get binormal
	 * @return binnormal
	 */
	irr::core::vector3df getBinormal() const;
};

#endif /* VERTEX_H_ */
