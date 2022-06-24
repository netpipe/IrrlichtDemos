#ifndef __C_Q3MODELSCENENODE_H_INCLUDED__
#define __C_Q3MODELSCENENODE_H_INCLUDED__

#include <IDummyTransformationSceneNode.h>
#include "IQ3ModelSceneNode.h"


namespace irr
{
namespace scene
{

class CQ3ModelSceneNode : public IQ3ModelSceneNode
{
public:


    CQ3ModelSceneNode(IQ3ModelMesh          *mesh,
                      ISceneNode            *parent,
                      ISceneManager         *mgr,
                      s32                    id       = -1,
                      const core::vector3df &position = core::vector3df(0,0,0),
                      const core::vector3df &rotation = core::vector3df(0,0,0),
                      const core::vector3df &scale    = core::vector3df(1.0f, 1.0f, 1.0f)
                      );


    virtual ~CQ3ModelSceneNode();

 // ISceneNode.
    virtual video::SMaterial& getMaterial(u32 i);

	virtual u32 getMaterialCount() const;

    const core::aabbox3d<f32>& getBoundingBox() const {	return Box; }

    void OnRegisterSceneNode();

    void render();


 // IQ3ModelSceneNode.
    virtual void setCurrentFrame(u32 frame);

    virtual void OnAnimate(u32 timeMs);

    virtual bool setFrameLoop(s32 begin, s32 end);

    virtual void setAnimationSpeed(f32 framesPerSecond);

    virtual void setLoopMode(bool playAnimationLooped);

    virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(s32 id=-1, bool zfailmethod=true, f32 infinity=10000.0f);

    virtual ISceneNode* getJointNode(const c8* tagName);

    virtual ISceneNode* getJointNode(EMD3_TAG_TYPE type);

    virtual ISceneNode* getJointNode(u32 index);

	virtual ISceneNode* addJointToNode(IQ3ModelSceneNode* jointNode);

    virtual f32 getFrameNr() const { return CurrentFrameNr; }

    virtual s32 getStartFrame() const { return StartFrame; }

	virtual s32 getEndFrame() const { return EndFrame; }

	virtual bool isLoopedMode() { return Looping; }

	virtual void setAnimationEndCallback(IAnimationEndCallBack* callback=0);

	virtual void setMesh(IQ3ModelMesh* mesh);

	virtual IAnimatedMesh* getMesh(void) { return (IAnimatedMesh*) Mesh; }

	virtual IQ3ModelMesh* getQ3Mesh(void) { return Mesh; }

    virtual void setRenderFromIdentity( bool On ) {	RenderFromIdentity=On; }

    virtual void setAnimationConfig(quake3::Animation * animation);

    virtual void setAnimation(quake3::EMD3_MODEL_ANIMATION_TYPE animationType);

	virtual quake3::EMD3_MODEL_ANIMATION_TYPE getAnimation() const { return AnimationType; }

	virtual void setSkin(quake3::Skin * skin);

	virtual void setAnimation(quake3::Animation * animations);

	virtual void setShader(u32 materialIndex, const quake3::SShader * shader, io::IFileSystem * fileSystem);

	virtual void setShaderFromList(quake3::ShadersParser * shaders, io::IFileSystem * fileSystem);


protected:
    virtual void updateMaterials();

    virtual void dropMaterials();

    virtual void normalRender(video::IVideoDriver* driver);

    virtual void debugRender(video::IVideoDriver* driver);

private:
    struct SJoinChildSceneNode
    {
        IDummyTransformationSceneNode* Node;
        u32 Index;
    };

    struct SQ3ModelTextures
    {
        //SQ3ModelTextures() : surface(NULL), shader(NULL) {}
        SQ3ModelTextures() : shader(NULL) {}

        //SQ3ModelTextures(quake3::Shader *pshader) : surface(NULL), shader(pshader) {}
        SQ3ModelTextures(quake3::Shader *pshader) : shader(pshader) {}

//        scene::IQ3Surface *surface;
        quake3::Shader    *shader;
    };


    s32 buildFrameNr( u32 timeMs);

	core::array<SQ3ModelTextures> Materials;
	core::aabbox3d<f32> Box;
	IQ3ModelMesh* Mesh;
	s32 BeginFrameTime;
	s32 StartFrame, EndFrame, CurrentFrameNr;
	f32 FramesPerSecond;
	bool Looping, FirstLoop;
	s32 LoopFrames;
	IAnimationEndCallBack* LoopCallBack;
	bool RenderFromIdentity;

	IShadowVolumeSceneNode* Shadow;

	core::array<SJoinChildSceneNode> JointChildSceneNodes;
	IQ3ModelSceneNode* Bone;

	quake3::EMD3_MODEL_ANIMATION_TYPE AnimationType;
	quake3::Animation * Animations;
	quake3::Skin* Skined;
	f32 TimeAbs;
};


} // namespace irr
} // namespace scene

#endif // __C_Q3MODELSCENENODE_H_INCLUDED__

