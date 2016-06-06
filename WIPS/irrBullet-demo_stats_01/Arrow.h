#pragma once

#include <irrlicht.h>

using namespace irr;

#define NUM_ARROW_SIDES 6
#define ARROW_TRANSPARENCY 128
#define BLUE		video::SColor(ARROW_TRANSPARENCY, 0, 0, 255)
#define RED			video::SColor(ARROW_TRANSPARENCY, 255, 0, 0)
#define GREEN		video::SColor(ARROW_TRANSPARENCY, 0, 255, 0)
#define WHITE		video::SColor(ARROW_TRANSPARENCY, 255, 255, 255)
#define YELLOW		video::SColor(ARROW_TRANSPARENCY, 255, 255, 0)
#define	LIGHTBLUE	video::SColor(ARROW_TRANSPARENCY, 0, 255, 255)
#define	GREY		video::SColor(ARROW_TRANSPARENCY, 153, 153, 153)
#define	PURPLE		video::SColor(ARROW_TRANSPARENCY, 255, 0, 255)
#define	SAND		video::SColor(ARROW_TRANSPARENCY, 255, 255, 204)
#define	BLACK		video::SColor(ARROW_TRANSPARENCY, 0, 0, 0)
 
class CArrow
{
private:
	scene::SMesh * m_pMesh;
	scene::SAnimatedMesh * m_pAnimMesh;
	scene::ISceneNode* m_pNode;
	video::SColor m_Color;
	bool m_bHead;
	bool m_bTransparent;
	bool m_bHasInitialised;
	s32 m_NumSides;
	f32 m_Width;

private:
	void SetMesh(f32 length);

public:
	CArrow(void);
	CArrow(video::SColor color, f32 width=0.1f, bool head=true, bool transparent=false, s32 numsides=NUM_ARROW_SIDES);
	void Init(void);
	virtual ~CArrow(void);
	void SetLength(f32 length);
	void SetPosition(const core::vector3df& pos);
	void SetPosition(f32 x, f32 y, f32 z);
	void SetRotation(const core::vector3df& rot);
	void SetRotation(f32 x, f32 y, f32 z);
	void SetTarget(const core::vector3df& dir);
	void SetTarget(f32 x, f32 y, f32 z);
	void SetEndpoints(const core::vector3df& start, const core::vector3df& end);
	void Cleanup(void);
};
