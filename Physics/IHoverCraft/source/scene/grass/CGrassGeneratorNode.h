#ifndef GRASSGENERATOR_H_
#define GRASSGENERATOR_H_

#include "CGrassPatchSceneNode.h"

class Game;

namespace irr
{
namespace scene
{

class CGrassGeneratorNode : public irr::scene::ISceneNode
{
private:

    ///An array of grass patch nodes.
    irr::core::array<irr::scene::CGrassPatchSceneNode*> grassNodes;

	//material settings
	irr::core::array<irr::video::E_MATERIAL_TYPE> modes;
	irr::u32 currentMode;

	//texture settings
	irr::video::ITexture* tex1;
	irr::video::ITexture* tex2;
    bool tex; //which texture to use;

    
    irr::core::aabbox3d<irr::f32> Box;

    irr::u32 frameCount;

    ///True if node automatically should spawn grass near the camera
    bool autoSpawn;
    
public:

	/**
	 * Constructor.
	 * \param device A pointer to the Irrlicht device.
	 */
	CGrassGeneratorNode(irr::IrrlichtDevice* device, bool autoSpawn = false);

	virtual void OnRegisterSceneNode();

	virtual void render();

	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

	virtual void addChild(irr::scene::ISceneNode* child);

	virtual void setScale(const core::vector3df& scale);

	virtual void setRotation(const core::vector3df& rotation);
	/**
	 * Adds grass patches to a terrain.
	 * \param wind A pointer to IWindGenerator.
	 * \param terrain A pointer to ITerrainSceneNode.
	 * \param heightMap Use the terrain height map.
	 * \param textureMap .
	 * \param grassMap .
	 */
	const bool addGrassToTerrain(Game* game,
		irr::scene::IWindGenerator* wind,
		irr::scene::ITerrainSceneNode* terrain,
		irr::video::IImage* heightMap,
		irr::video::IImage* textureMap,
		irr::video::IImage* grassMap);

	const void removeAllGrass();

	///Toggle between two textures.
	const void swapTextures();

	/**
	 * Cycle through E_MATERIAL_TYPE modes.
	 * \param forward True if search direction is forward.
	 */
	const void cycleModes(const bool forward);


	//setter

	const void decreaseDrawDistance(const irr::u32 dist) const;

	const void increaseDrawDistance(const irr::u32 dist) const;

	const void decreaseWindResolution(const irr::u32 delta = 1) const;

	const void increaseWindResolution(const irr::u32 delta = 1) const;

	const void decreaseRefreshDelay() const;

	const void increaseRefreshDelay() const;

	const void decreaseMaxDensity(const irr::u32 delta = 50) const;

	const void increaseMaxDensity(const irr::u32 delta = 50) const;


	//getter

	//inline irr::core::array<irr::scene::CGrassPatchSceneNode*>& getGrassNodes() { return this->grassNodes; }
	
	inline const irr::f32 getDrawDistance() const
	{
		return (this->grassNodes.size() > 0) ? this->grassNodes[0]->getDrawDistance() : 0.0f;
	}

	inline const irr::u32 getMaxDensity() const
	{
		return (this->grassNodes.size() > 0) ? this->grassNodes[0]->getMaxDensity() : 0;
	}

	inline const irr::u32 getWindResolution() const
	{
		return (this->grassNodes.size() > 0) ? this->grassNodes[0]->getWindRes() : 0;
	}

	inline const irr::u32 getRefreshDelay() const
	{
		return (this->grassNodes.size() > 0) ? this->grassNodes[0]->getRefreshDelay() : 0; 
	}
};

} // end namespace scene
} // end namespace irr

#endif /*GRASSGENERATOR_H_*/
