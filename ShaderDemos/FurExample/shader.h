#ifndef SHADER_H
#define SHADER_H

#include "global.h"

class MyShaderCallBack : public IShaderConstantSetCallBack
{
public:

	void OnSetConstants(IMaterialRendererServices* services, s32 userData);
    f32 fFurLength;
    f32 fLayer;
    f32 fMaxLength;
};

class CFurSceneNode : public ISceneNode
{

    public:
        //SceneNode functions
        CFurSceneNode(IAnimatedMesh* meshSELF, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position = core::vector3df(0,0,0),
			const core::vector3df& rotation = core::vector3df(0,0,0),
			const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f))
			: ISceneNode(parent, mgr, id, position, rotation, scale), meshSelf(meshSELF){Init();}

        ~CFurSceneNode();

        virtual void OnRegisterSceneNode();
        virtual void render();

        virtual const core::aabbox3d<f32>& getBoundingBox() const;
	    virtual u32 getMaterialCount() const;
        virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue);
	    virtual video::SMaterial& getMaterial(u32 i);
        virtual void setMaterialTexture(u32 textureLayer, video::ITexture* texture);
        virtual void setAnimationSpeed(u32 speed);
        virtual void setFrameLoop(u32 first, u32 last);\
        virtual f32  getFrameNr();


        //Custom
        void Init();
        void setLayers(u32 Layer);
        void setFurLength(f32 Length);
        IAnimatedMeshSceneNode* getNode();

    private:
        f32 fFurLength;
        u32 iLayers;
        IAnimatedMesh* meshSelf;
        IAnimatedMeshSceneNode* nodeSelf;
};

extern void InitShaders();
#endif
