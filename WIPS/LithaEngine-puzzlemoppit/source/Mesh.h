
#include "IMesh.h"

#include "IShader.h"
#include <map>

// Utility struct
struct Pass
{
	// Create using the materials from an Irrlicht scene node.
	Pass(scene::ISceneNode *node)
	{
		for (u32 i = 0; i < node->getMaterialCount(); i ++)
			materials.push_back(node->getMaterial(i));
		
		shaders.assign(node->getMaterialCount(), NULL);
	}
	
	// Create using the materials from a scene::IMesh
	Pass(scene::IMesh *mesh)
	{
		for (u32 i = 0; i < mesh->getMeshBufferCount(); i ++)
		{
			if (scene::IMeshBuffer *mb = mesh->getMeshBuffer(i))
				materials.push_back(mb->getMaterial());
			else
			{
				materials.push_back(video::SMaterial()); // Default material. This should probably never occur.
			}
		}
		
		shaders.assign(mesh->getMeshBufferCount(), NULL);
	}
	
	~Pass()
	{
		for (u32 i = 0; i < shaders.size(); i ++)
		{
			if (shaders[i])
				shaders[i]->drop();
		}
	}
	
	std::vector<video::SMaterial> materials;
	std::vector<IShader *> shaders;
};

struct Animation
{
	s32 start,end;
	f32 fps;
	bool looped;
	bool interruptible;
	f32 transitionTime;
};

class Mesh : public IMesh
{
	scene::ISceneManager *smgr;
	
	scene::IAnimatedMeshSceneNode *meshNode;
	
	std::vector<Pass> passes;
	
	std::map<s32, Animation> animations;
	Animation *currentAnimation;
	
	// a permanent rotation to apply transparently to mesh scene node
	core::vector3df rotation;
	// a permanent translation
	core::vector3df translation;
	
	// fixes an Irrlicht bug
	//bool onFirstFrame;
	
public:
	Mesh(scene::ISceneManager *smgr, const c8 *name);
	~Mesh();
	
	core::aabbox3df GetBoundingBox();
	
	u32 GetMaterialCount();
	
	video::SMaterial &GetMaterial(u32 material, u16 pass);
	
	void SetShader(u32 material, IShader *shader, u16 pass);
	
	void SetAllShaders(IShader *shader, u16 pass);
	
	void DisableMaterial(u32 material, u16 pass);
	
	void DisableAllMaterials(u16 pass);
	
	void SetVisible(bool visible);
	
	void ReceiveRenderPosition(core::vector3df pos);
	void ReceiveRenderRotation(core::vector3df rot);
	
	u16 GetPassCount();
	
	void SetCurrentPass(u16 pass);
	
	void AddPass(const c8 *meshName);
	
	void Render(u16 pass);
	
	// an animation is: {start,end,FPS,looped?,interruptible?}
	void DefineAnimation(s32 id, u32 start, u32 end, f32 fps, bool looped, bool interruptible, f32 transitionTime);
	
	void SetAnimation(s32 id);
	
	bool IsPlayingAnimation(s32 id);
	
	void HaltAnimation();
	
	scene::IAnimatedMeshSceneNode *GetIrrlichtNode();
	
	// Mesh can be scaled. Should be taken into account by physics mesh geometry
	void Scale(const core::vector3df &scale);
	
	void Rotate(const core::vector3df &rotation);
	
	void Translate(const core::vector3df &translation);
};

