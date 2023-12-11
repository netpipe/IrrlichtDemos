// Copyright (C) 2010 randomMesh
// This file is part of the Sierpinksi demo.
// For conditions of distribution and use, see copyright notice in main.cpp

#ifndef CSIERPINSKISCENENODE_H_
#define CSIERPINSKISCENENODE_H_

#include "ISierpinskiSceneNode.h"

namespace irr
{
namespace video
{
class S3DVertex;
}

namespace scene
{

/*
	 rules for each Tetrahedra

		//center1 = find center of line v0 and v1
		//center2 = find center of line v0 and v2
		//center3 = find center of line v1 and v2
		//center4 = find center of line v1 and v3
		//center5 = find center of line v2 and v3
		//center6 = find center of line v0 and v3

		//make 4 new Tetrahedra
			//left
				//newV0 = center1
				//newV1 = v1
				//newV2 = center3
				//newV3 = center4
			//right
				//newV0 = center2
				//newV1 = center3
				//newV2 = v2
				//newV3 = center5
			//rear
				//newV0 = center6
				//newV1 = center4
				//newV2 = center5
				//newV3 = v3
			//top
				//newV0 = v0
				//newV1 = center1
				//newV2 = center2
				//newV3 = center6

		//replace old Tetrahedron with the new ones
 */
class CSierpinskiSceneNode : public ISierpinskiSceneNode
{

public:

	CSierpinskiSceneNode(const f32 size, const u32 initialIterations, const video::SColor& topColor, const video::SColor& bottomColor,
		ISceneNode* parent, ISceneManager* mgr, s32 id);

	//! destructor
	virtual ~CSierpinskiSceneNode();

	//! frame
	virtual void OnRegisterSceneNode();

	//! renders the node.
	virtual void render();

	//! returns the axis aligned bounding box of this node
	virtual const core::aabbox3d<f32>& getBoundingBox() const { return Box; }

	//! returns the material based on the zero based index i. To get the amount
	//! of materials used by this scene node, use getMaterialCount().
	//! This function is needed for inserting the node into the scene hirachy on a
	//! optimal position for minimizing renderstate changes, but can also be used
	//! to directly modify the material of a scene node.
	virtual video::SMaterial& getMaterial(u32 i) { return Material; }

	//! returns amount of materials used by this scene node.
	virtual u32 getMaterialCount() const { return 1; }

	virtual IMesh* getMeshForIteration(const u32 iteration);

	//! get current iteration
	virtual u32 getIteration() const { return CurrentIteration; }

	//! set current iteration
	virtual void setIteration(const u32 iteration);

	virtual void iterate(const bool forward);

	virtual u32 getNumTetrahedra() const { return Tetrahedra.size(); }

private:

	core::aabbox3df Box;

	irr::video::SMaterial Material;

	u32 CurrentIteration;

	class Tetrahedron
	{

	public:

		Tetrahedron(
			const irr::video::S3DVertex vertices[], const unsigned int numVertices,
			const unsigned short indices[], const unsigned int numIndices,
			const unsigned int numPrimitives);

		~Tetrahedron();

		inline const irr::video::S3DVertex* const getVertices() const { return this->vertices; }

		inline const unsigned short* const getIndices() const { return this->indices; }

		inline unsigned int getVertexCount() const { return numVertices; }

		inline unsigned int getIndexCount() const { return this->numIndices; }

		inline const unsigned int getPrimitiveCount() const { return this->numPrimitives; }

		inline const irr::core::aabbox3df& getBox() const { return this->box; }

	private:

		irr::video::S3DVertex* vertices;

		unsigned short* indices;

		unsigned int numVertices;

		unsigned int numIndices;

		unsigned int numPrimitives;

		irr::core::aabbox3df box;
	};

	core::list<const Tetrahedron* const> Tetrahedra;

	Tetrahedron* StartTetrahedron;
};

}
}

#endif /* CSIERPINSKISCENENODE_H_ */
