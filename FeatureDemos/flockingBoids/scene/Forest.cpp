#include "Forest.h"



Forest::Forest(irr::IrrlichtDevice* const device, const irr::scene::ITerrainSceneNode* const terrain, const irr::core::array<irr::core::aabbox3df>& exceptions) :
	device(device), terrain(terrain), exceptions(exceptions)
{
	//store terrain size
	const irr::core::aabbox3df& tb = terrain->getBoundingBox();
	this->terrainSize.X = tb.getExtent().X;
	this->terrainSize.Y = tb.getExtent().Z;

	//add some trees to the pool

	Tree bleech;
	bleech.type = ETT_BLEECH;
	bleech.fileName = "media/images/trees/Bleech.png";
	bleech.dimension.Width = 768;
	bleech.dimension.Height = 1024;
	bleech.yOffset = 384.0f;
	treePool.push_back(bleech);

	Tree jubaea;
	jubaea.type = ETT_JUBAEA;
	jubaea.fileName = "media/images/trees/Greenhouse-Palm-jubaea01.png";
	jubaea.dimension.Width = 1024;
	jubaea.dimension.Height = 1024;
	jubaea.yOffset = 500.0f;
	this->treePool.push_back(jubaea);

	Tree bugainvillier;
	bugainvillier.type = ETT_BUGAINVILLIER;
	bugainvillier.fileName = "media/images/trees/Bougainvillier.png";
	bugainvillier.dimension.Width = 1024;
	bugainvillier.dimension.Height = 1024;
	bugainvillier.yOffset = 480.0f;
	this->treePool.push_back(bugainvillier);

	Tree flamboyant;
	flamboyant.type = ETT_FLAMBOYANT;
	flamboyant.fileName = "media/images/trees/Flamboyant.png";
	flamboyant.dimension.Width = 1024;
	flamboyant.dimension.Height = 768;
	flamboyant.yOffset = 300.0f;
	this->treePool.push_back(flamboyant);

	Tree taxus;
	taxus.type = ETT_TAXUS;
	taxus.fileName = "media/images/trees/taxus.png";
	taxus.dimension.Width = 1024;
	taxus.dimension.Height = 1024;
	taxus.yOffset = 512.0f;
	this->treePool.push_back(taxus);

	Tree cycas;
	cycas.type = ETT_CYCAS;
	cycas.fileName = "media/images/trees/cycas.png";
	cycas.dimension.Width = 1024;
	cycas.dimension.Height = 768;
	cycas.yOffset = 350.0f;
	this->treePool.push_back(cycas);
}

Forest::~Forest()
{
	for (irr::u32 i = 0; i< this->trees.size(); ++i)
		this->trees[i]->remove();
}

void Forest::addTree(const irr::core::vector3df& position, const E_TREE_TYPE& type)
{
	irr::scene::IBillboardSceneNode* const tree = this->device->getSceneManager()->addBillboardSceneNode(
		0, this->treePool[type].dimension, position);
	tree->setMaterialType(irr::video::EMT_TRANSPARENT_ALPHA_CHANNEL);
	tree->setMaterialTexture(0, device->getVideoDriver()->getTexture(this->treePool[type].fileName));
	tree->grab();

	this->trees.push_back(tree);
}

void Forest::randomlyPlaceVegetation(const irr::u32 numTrees)
{
	//initialize random seed
	srand(device->getTimer()->getTime());

	irr::u32 i;
	for (i = 0; i < numTrees; ++i)
	{
		const irr::u32 type = rand()%6;

		irr::core::vector3df pos(makeRandomPos((E_TREE_TYPE)type));

		if (exceptions.size() > 0)
		{
			if (!isLegalPlace(pos))
			{
				//make new pos
				while (!isLegalPlace(pos))
				{
					pos = makeRandomPos((E_TREE_TYPE)type);
				}
			}
		}

		switch(type)
		{
			case 0:	addTree(pos, ETT_BLEECH); break;
			case 1:	addTree(pos, ETT_JUBAEA); break;
			case 2:	addTree(pos, ETT_BUGAINVILLIER); break;
			case 3:	addTree(pos, ETT_FLAMBOYANT); break;
			case 4:	addTree(pos, ETT_TAXUS); break;
			case 5:	addTree(pos, ETT_CYCAS); break;
		}
	}
}

void Forest::removeTree()
{
	const irr::u32 numTrees = this->trees.size();
	if (numTrees >= 1)
	{
		this->trees[numTrees-1]->remove();
		this->trees.erase(numTrees-1);
	}
}

bool Forest::isLegalPlace(const irr::core::vector3df& pos) const
{
	bool legal = true;

	irr::u32 i;
	for (i = 0; i < exceptions.size(); ++i)
	{
		if (exceptions[i].isPointInside(pos))
		{
			legal = false;
			break;
		}
	}

	return legal;
}

const irr::core::vector3df Forest::makeRandomPos(const E_TREE_TYPE& type) const
{
	const irr::f32 xPos = (rand()%(irr::s32)this->terrainSize.X);
	const irr::f32 zPos = (rand()%(irr::s32)this->terrainSize.Y);
	const irr::f32 yPos = (this->terrain->getHeight(xPos, zPos) + this->treePool[type].yOffset);

	const irr::core::vector3df pos(xPos, yPos, zPos);

	return pos;
}
