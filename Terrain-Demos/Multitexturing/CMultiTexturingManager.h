#ifndef CMULTITEXTURINGMANAGER_H_INCLUDED
#define CMULTITEXTURINGMANAGER_H_INCLUDED

#include <irrlicht.h>
using namespace irr;

class CMultiTexturingManager: public video::IShaderConstantSetCallBack
{
public:
	struct STexturePass
	{
	    video::ITexture *splat_texture;
	    video::ITexture *red_texture;
	    video::ITexture *green_texture;
	    video::ITexture *blue_texture;
	};

private:
	struct SMultiTextureNode
	{
	    //The textured node
	    scene::ISceneNode *Node;
	    core::array<STexturePass*> array_Passes;
	};

public:
	CMultiTexturingManager(scene::ISceneManager* psmgr);

	~CMultiTexturingManager();

	void drawAll();

	virtual void OnSetConstants(video::IMaterialRendererServices* services,s32 userData);

	bool addNode(scene::ISceneNode *node);

	bool removeNode(scene::ISceneNode *node);

	STexturePass *addPass(scene::ISceneNode *node);
	STexturePass *addPass(scene::ISceneNode *node, video::ITexture *splat, video::ITexture *red, video::ITexture *green, video::ITexture *blue);
	STexturePass *addPass(scene::ISceneNode *node, STexturePass *pass);

	bool removePass(scene::ISceneNode *node, u32 layer);
	bool removePass(scene::ISceneNode *node, STexturePass *pass);

private:
    scene::ISceneManager* smgr;

    video::IVideoDriver* driver;

	core::array<SMultiTextureNode> array_Nodes;

    int shaderMaterial;

};

#endif // CMULTITEXTURINGMANAGER_H_INCLUDED
