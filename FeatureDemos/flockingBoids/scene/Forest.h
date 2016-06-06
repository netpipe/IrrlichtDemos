#ifndef FOREST_H_
#define FOREST_H_

#include <irrlicht.h>

enum E_TREE_TYPE
{
	ETT_BLEECH = 0,
	ETT_JUBAEA,
	ETT_BUGAINVILLIER,
	ETT_FLAMBOYANT,
	ETT_TAXUS,
	ETT_CYCAS,
};

class Forest
{

public:

	Forest(irr::IrrlichtDevice* const device, const irr::scene::ITerrainSceneNode* const terrain, const irr::core::array<irr::core::aabbox3df>& exceptions);

	~Forest();

	void addTree(const irr::core::vector3df& position, const E_TREE_TYPE& type);

	void randomlyPlaceVegetation(const irr::u32 numTrees);

	void removeTree();

	inline const irr::u32 getNumberOfTrees() const { return this->trees.size(); }

private:

	irr::IrrlichtDevice* const device;

	const irr::scene::ITerrainSceneNode* const terrain;

	irr::core::vector2d<irr::f32> terrainSize;

	irr::core::array<irr::core::aabbox3df> exceptions;

	struct Tree
	{
		E_TREE_TYPE type;

		irr::core::stringw fileName;

		irr::core::dimension2d<irr::f32> dimension;

		irr::f32 yOffset;
	};

	irr::core::array<Tree> treePool;

	irr::core::array<irr::scene::IBillboardSceneNode*> trees;

	bool isLegalPlace(const irr::core::vector3df& pos) const;

	const irr::core::vector3df makeRandomPos(const E_TREE_TYPE& type) const;
};

#endif /* FOREST_H_ */
