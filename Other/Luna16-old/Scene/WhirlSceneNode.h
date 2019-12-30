#ifndef WHIRL_H_
#define WHIRL_H_

#include <ISceneNode.h>
#include <S3DVertex.h>

namespace irr
{
namespace scene
{

/**
 * Based off of the code found here: http://lesson.ifdef.jp/009.html
 */
class WhirlSceneNode : public irr::scene::ISceneNode
{

public:

	WhirlSceneNode(const u32 num, ISceneNode* const parent, ISceneManager* const mgr, s32 id, const f32 distance = 2.5f);

	virtual void OnRegisterSceneNode();

	virtual void OnAnimate(u32 timeMs);

	virtual void render();

	virtual const core::aabbox3d<f32>& getBoundingBox() const;

	virtual u32 getMaterialCount() const;

	virtual video::SMaterial& getMaterial(u32 i);

private:

	video::S3DVertex vertices[4];

	u16 indices[6];

	video::SMaterial Material;

	core::aabbox3d<f32> Box;

	u32 num;

	struct Star
	{
		video::SColor color;
		f32 dist;
		f32 angle;
	};

	core::array<Star> stars;
};

#endif /* WHIRL_H_ */

} // end namespace scene
} // end namespace irr
