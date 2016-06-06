#ifndef __IRRBOX__
#define __IRRBOX__


class IrrBox : public scene::ISceneNode 
{ 
protected:
   core::aabbox3d<f32> Box; 
   video::S3DVertex Vertices[24]; 
   video::SMaterial Material; 
  
public: 
    
   IrrBox (scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, float x, float y, float z);
   virtual void OnPreRender(); 
   virtual void render(); 
   virtual const core::aabbox3d<f32>& getBoundingBox() const { return Box; } 
   virtual s32 getMaterialCount() { return 1; } 
   virtual video::SMaterial& getMaterial(s32 i) { return Material; } 
}; 


class MachineBox : public IrrBox
{ 
   public: 
   MachineBox (scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, float x, float y, float z);
}; 


#endif