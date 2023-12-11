#include "main.h"

#ifndef __CProjectiveTextures__
#define __CProjectiveTextures__

class CProjectiveTextures : public IShaderConstantSetCallBack, ISceneNode
{
public:

	// Constructor / Destructor.
	CProjectiveTextures(ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& lookat = core::vector3df(0,0,0));

	~CProjectiveTextures();

	//! Sets the look at target of the projector
	virtual void setTarget(const vector3df& pos);

	//! Gets the current look at target of the projector
	virtual const vector3df& getTarget() const;

	//! Sets the field of view (Default: PI / 3.5f)
	virtual void setFOV(f32 f);

	//! Gets the field of view (Default: PI / 3.5f)
	virtual f32 getFOV();

	//! Returns the axis aligned bounding box of this node
	virtual const aabbox3d<f32>& getBoundingBox() const;

	//! Render
	virtual void render();

	virtual void OnSetConstants(IMaterialRendererServices* services,s32 userData);

	ITexture* texture;
	array<IMeshSceneNode*> nodeArray;

private:

	int projTex;
	SMaterial projMat;

protected:

	void recalculateProjectionMatrix();
	void recalculateViewArea();

	vector3df Target;

	f32 Fovy;	// Field of view, in radians.

	SViewFrustum ViewArea;

};

#endif
