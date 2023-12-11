/**
 * DecalSceneNode.h
 *
 * Tank @ War Project
 * October 2011
 */

#ifndef DECAL_SCENE_NODE_H_
#define DECAL_SCENE_NODE_H_

#include <irrlicht.h>
#include <time.h>

namespace irr {
namespace scene {

#ifdef MAKE_IRR_ID
const int DECAL_SCENE_NODE_ID = MAKE_IRR_ID('d','e','c','l');
#else
const int DECAL_SCENE_NODE_ID = 'decl';
#endif // MAKE_IRR_ID

#if defined _DEBUG
#define SHOW_DEBUG_BOX false
#else
#define SHOW_DEBUG_BOX false
#endif

class DecalSceneNode: public scene::ISceneNode {
private:
	scene::IMesh* mesh;
	video::ITexture* texture;
	core::aabbox3df box;
	video::SMaterial material;
	f32 distance;
	f32 lifetime;
	time_t creationTime;
	bool fadeOut;
	f32 fadeOutTime;
	u32 startFadeOutTimeMs;

public:
	/**
	 * Constructor
	 * @param parent
	 * @param smgr
	 * @param mesh
	 * @param texture
	 * @param position
	 */
	DecalSceneNode(scene::ISceneNode* parent, scene::ISceneManager* smgr, scene::IMesh* mesh, video::ITexture* texture, const core::vector3df position = core::vector3df(0, 0, 0));

	/**
	 * Destructor
	 */
	virtual ~DecalSceneNode();

	/**
	 * On register scene node
	 */
	virtual void OnRegisterSceneNode();

	/**
	 * Render
	 */
	virtual void render();

	/**
	 * On Animate
	 * @param timeMs
	 */
	virtual void OnAnimate(u32 timeMs);

	/**
	 * Get AA bounding box
	 */
	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	/**
	 * Get type
	 * @return type
	 */
	virtual scene::ESCENE_NODE_TYPE getType() const;

	/**
	 * Get material count
	 * @return 1
	 */
	virtual u32 getMaterialCount() const;

	/**
	 * Get material
	 * @param i
	 * @return material
	 */
	virtual video::SMaterial& getMaterial(u32 i);

	/**
	 * Get decal mesh
	 * @return mesh
	 */
	scene::IMesh* getMesh();

	/**
	 * Get decal texture
	 * @return texture
	 */
	video::ITexture* getTexture();

	/**
	 * Get life time in seconds
	 * @return lifeTime
	 */
	f32 getLifetime();

	/**
	 * Set life time in seconds
	 * @param lifeTime
	 */
	void setLifetime(const f32 lifeTime);

	/**
	 * Get max viewing distance
	 * @return distance
	 */
	f32 getDistance();

	/**
	 * Set max viewing distance
	 * @param distance
	 */
	void setDistance(const f32 distance);

	/**
	 * Set fade out
	 * @param fadeOut
	 * @param time
	 */
	void setFadeOut(const bool fadeOut, const irr::f32 time = 1);
};

} // end namespace scene

} // end namespace irr

#endif /* DECAL_SCENE_NODE_H_ */
