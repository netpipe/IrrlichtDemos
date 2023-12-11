// Copyright (C) 2010 randomMesh
// This file is part of the Sierpinksi demo.
// For conditions of distribution and use, see copyright notice in main.cpp

#include "CSierpinskiSceneNode.h"
#include <ISceneManager.h>
#include <ICameraSceneNode.h>
#include <SViewFrustum.h>

namespace irr
{
namespace scene
{

CSierpinskiSceneNode::CSierpinskiSceneNode(const f32 size, const u32 initialIterations,
	const video::SColor& topColor, const video::SColor& bottomColor, ISceneNode* parent, ISceneManager* mgr, s32 id)
:	ISierpinskiSceneNode(parent, mgr, id),
 	CurrentIteration(initialIterations), StartTetrahedron(0)
{
	Material.Lighting = false;

	/* make first regular tetrahedron for iteration 0
	 *
	 *      0
	 * 	    .
	 *     /|\             Front view
	 *    / | \
	 * 1 /__3__\ 2
	 *
	 *
	 *
	 *      3
	 *      .
	 *     /|\             Top view
	 *   1//_\\2
	 *
	 */

	const f32 r = size*sqrt(3)/sqrt(2)/2; //compute radius for 'size' edge length
	const f32 m_sqrt1 = sqrt(2.0);
	const f32 m_sqrt2 = m_sqrt1/sqrt(3.0);
	const f32 m_r1 = -r/3.0;

	video::S3DVertex vertices[4];
	vertices[0].Pos = core::vector3df(0.0f, r, 0.0f);
	vertices[1].Pos = core::vector3df( r*m_sqrt1*2.0/3.0, m_r1,  0.0);
	vertices[2].Pos = core::vector3df(-r*m_sqrt1    /3.0, m_r1,  r*m_sqrt2);
	vertices[3].Pos = core::vector3df(-r*m_sqrt1    /3.0, m_r1, -r*m_sqrt2);
	vertices[0].Color = topColor;
	vertices[1].Color = bottomColor;
	vertices[2].Color = bottomColor;
	vertices[3].Color = bottomColor;

	const u16 indices[6] = {  0, 1, 2, 3, 0, 1 }; //triangle strip

	StartTetrahedron = new Tetrahedron(vertices, 4, indices, 6, 4);

	Box.reset(vertices[0].Pos);
	for (s32 i = 1; i < 4; ++i)
		Box.addInternalPoint(vertices[i].Pos);

	setIteration(initialIterations);
}

CSierpinskiSceneNode::~CSierpinskiSceneNode()
{
	core::list<const Tetrahedron* const>::ConstIterator it = Tetrahedra.begin();
	const core::list<const Tetrahedron* const>::ConstIterator& end = Tetrahedra.end();
	for (; it != end; ++it)
		delete (*it);

	delete StartTetrahedron;
}

void CSierpinskiSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

void CSierpinskiSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->setMaterial(Material);

	const core::aabbox3df& camBox = SceneManager->getActiveCamera()->getViewFrustum()->getBoundingBox();

	core::list<const Tetrahedron* const>::ConstIterator it = Tetrahedra.begin();
	const core::list<const Tetrahedron* const>::ConstIterator& end = Tetrahedra.end();
	for (; it != end; ++it)
	{
		const Tetrahedron* const tetrahedron = (*it);

		if (camBox.intersectsWithBox(tetrahedron->getBox()))
		{
			driver->drawVertexPrimitiveList(
				tetrahedron->getVertices(), tetrahedron->getVertexCount(), tetrahedron->getIndices(), tetrahedron->getPrimitiveCount(),
				video::EVT_STANDARD, EPT_TRIANGLE_STRIP);

			if (DebugDataVisible & irr::scene::EDS_BBOX)
				driver->draw3DBox(tetrahedron->getBox(), video::SColor(255, 0, 255, 255));
		}
	}


	if (DebugDataVisible & EDS_BBOX)
		driver->draw3DBox(Box, video::SColor(255, 0, 0, 128));
}

IMesh* CSierpinskiSceneNode::getMeshForIteration(const u32 iteration)
{
	return 0;
}

void CSierpinskiSceneNode::setIteration(const u32 iteration)
{
	core::list<const Tetrahedron* const>::ConstIterator it = Tetrahedra.begin();
	const core::list<const Tetrahedron* const>::ConstIterator& end = Tetrahedra.end();
	for (; it != end; ++it)
		delete (*it);

	Tetrahedra.clear();

	Tetrahedra.push_back(new Tetrahedron(
		StartTetrahedron->getVertices(),	StartTetrahedron->getVertexCount(),
		StartTetrahedron->getIndices(), StartTetrahedron->getIndexCount(), StartTetrahedron->getPrimitiveCount()));

	CurrentIteration = 0;

	for (u32 i = 0; i < iteration; ++i)
		iterate(true);
}

void CSierpinskiSceneNode::iterate(const bool forward)
{
	if (forward)
	{
		irr::video::S3DVertex vertices[4];
		const Tetrahedron* newTetrahedron1 = 0;
		const Tetrahedron* newTetrahedron2 = 0;
		const Tetrahedron* newTetrahedron3 = 0;
		const Tetrahedron* newTetrahedron4 = 0;

		irr::core::list<const Tetrahedron* const>::Iterator it = Tetrahedra.begin();
		const irr::core::list<const Tetrahedron* const>::ConstIterator& end = Tetrahedra.end();
		for (; it != end;)
		{
			const Tetrahedron* const oldTetraheder = (*it);
			const irr::video::S3DVertex* const originalVertices = oldTetraheder->getVertices();

			const irr::core::vector3df& center1 = (originalVertices[0].Pos + originalVertices[1].Pos)*0.5;
			const irr::core::vector3df& center2 = (originalVertices[0].Pos + originalVertices[2].Pos)*0.5;
			const irr::core::vector3df& center3 = (originalVertices[1].Pos + originalVertices[2].Pos)*0.5;
			const irr::core::vector3df& center4 = (originalVertices[1].Pos + originalVertices[3].Pos)*0.5;
			const irr::core::vector3df& center5 = (originalVertices[2].Pos + originalVertices[3].Pos)*0.5;
			const irr::core::vector3df& center6 = (originalVertices[0].Pos + originalVertices[3].Pos)*0.5;

			//create 4 new initiators
			const video::SColor& topColor = originalVertices[0].Color;
			const video::SColor& bottomColor = originalVertices[1].Color;
			const irr::u16* const indices = oldTetraheder->getIndices();

			//left
			vertices[0].Pos = center1;
			vertices[0].Color = topColor;
			vertices[1].Pos = originalVertices[1].Pos;
			vertices[1].Color = bottomColor;
			vertices[2].Pos = center3;
			vertices[2].Color = bottomColor;
			vertices[3].Pos = center4;
			vertices[3].Color = bottomColor;
			newTetrahedron1 = new Tetrahedron(vertices, 4, indices, 6, 4);

			//right
			vertices[0].Pos = center2;
			vertices[0].Color = topColor;
			vertices[1].Pos = center3;
			vertices[1].Color = bottomColor;
			vertices[2].Pos = originalVertices[2].Pos;
			vertices[2].Color = bottomColor;
			vertices[3].Pos = center5;
			vertices[3].Color = bottomColor;
			newTetrahedron2 = new Tetrahedron(vertices, 4, indices, 6, 4);

			//rear
			vertices[0].Pos = center6;
			vertices[0].Color = topColor;
			vertices[1].Pos = center4;
			vertices[1].Color = bottomColor;
			vertices[2].Pos = center5;
			vertices[2].Color = bottomColor;
			vertices[3].Pos = originalVertices[3].Pos;
			vertices[3].Color = bottomColor;
			newTetrahedron3 = new Tetrahedron(vertices, 4, indices, 6, 4);

			//top
			vertices[0].Pos = originalVertices[0].Pos;
			vertices[0].Color = topColor;
			vertices[1].Pos = center1;
			vertices[1].Color = bottomColor;
			vertices[2].Pos = center2;
			vertices[2].Color = bottomColor;
			vertices[3].Pos = center6;
			vertices[3].Color = bottomColor;
			newTetrahedron4 = new Tetrahedron(vertices, 4, indices, 6, 4);

			//remove old initator
			delete oldTetraheder;
			it = Tetrahedra.erase(it);

			//add new (to front since we are still in the loop)
			Tetrahedra.push_front(newTetrahedron1);
			Tetrahedra.push_front(newTetrahedron2);
			Tetrahedra.push_front(newTetrahedron3);
			Tetrahedra.push_front(newTetrahedron4);
		}

		++CurrentIteration;
	}
	else
	{
		if (CurrentIteration > 0)
			setIteration(--CurrentIteration);
	}
}

CSierpinskiSceneNode::Tetrahedron::Tetrahedron(
	const irr::video::S3DVertex vertices[], const unsigned int numVertices,
	const unsigned short indices[], const unsigned int numIndices, const unsigned int numPrimitives) :
	vertices(0), indices(0), numVertices(numVertices), numIndices(numIndices), numPrimitives(numPrimitives)
{
	this->vertices = new irr::video::S3DVertex[numVertices];
	this->indices = new unsigned short[numIndices];

	memcpy(this->vertices, vertices, sizeof(irr::video::S3DVertex)*numVertices);
	memcpy(this->indices, indices, sizeof(unsigned short)*numIndices);

	box.reset(vertices[0].Pos);
	for (irr::s32 i = 1; i < 4; ++i)
		box.addInternalPoint(vertices[i].Pos);
}

CSierpinskiSceneNode::Tetrahedron::~Tetrahedron()
{
	delete[] vertices;
	delete[] indices;
}

}
}
