#ifndef TELEPORTERSCENENODE_H_
#define TELEPORTERSCENENODE_H_

#include <ISceneNode.h>

class Demo;

namespace irr
{
namespace scene
{

class ISceneManager;
class WhirlSceneNode;
class IBillboardTextSceneNode;

class TeleporterSceneNode : public irr::scene::ISceneNode
{

public:

	TeleporterSceneNode(
		const Demo* const demo,
		const core::aabbox3d<f32>& placeA,
		const core::aabbox3d<f32>& placeB,
		const bool viceversa = true,
		const s32 id = -1);

	~TeleporterSceneNode();

	virtual u32 getMaterialCount() const;


	virtual void OnRegisterSceneNode();

	virtual void OnAnimate(u32 timeMs);

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual video::SMaterial& getMaterial(u32 i);


	void addNodeToWatchList(irr::scene::ISceneNode* const node);

	void removeNodeFromWatchList(const irr::scene::ISceneNode* const node);

private:

	const Demo* const demo; //needed for the timer

	core::list<ISceneNode*> nodesToWatch;

	core::aabbox3df placeA;

	core::aabbox3df placeB;

	bool viceversa;

	bool dontTeleport;

	f32 dontTeleportTimer;

	WhirlSceneNode* whirl[2];

	IBillboardTextSceneNode* text[2];

	video::SMaterial Material;

	core::aabbox3d<f32> Box;

	void teleport(ISceneNode* const node, const irr::core::vector3df& target);
};

} // end namespace scene
} // end namespace irr

#endif /* TELEPORTERSCENENODE_H_ */
