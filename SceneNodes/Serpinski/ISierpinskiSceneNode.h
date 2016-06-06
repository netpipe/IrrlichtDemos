// Copyright (C) 2010 randomMesh
// This file is part of the Sierpinksi demo.
// For conditions of distribution and use, see copyright notice in main.cpp

#ifndef ISIERPINSKISCENENODE_H_
#define ISIERPINSKISCENENODE_H_

#include <ISceneNode.h>

namespace irr
{
namespace scene
{

class IMesh;

class ISierpinskiSceneNode : public ISceneNode
{

public:

	ISierpinskiSceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id)
		: ISceneNode(parent, mgr, id) {}

	virtual IMesh* getMeshForIteration(const u32 iteration) = 0;

	//! get current iteration
	virtual u32 getIteration() const = 0;

	//! set current iteration
	virtual void setIteration(const u32 iteration) = 0;

	virtual void iterate(const bool forward) = 0;

	virtual u32 getNumTetrahedra() const = 0;
};

}
}

#endif /* ISIERPINSKISCENENODE_H_ */
