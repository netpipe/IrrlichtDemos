
#include <irrlicht.h>

namespace irr{
namespace scene{

class circleclass : public ISceneNode{
  private:
    core::array<video::S3DVertex> lstVertices;
    core::array<u16> lstIndices;
    core::aabbox3df Box;
    video::SMaterial Material;
    video::IVideoDriver* Driver;

  public:
    circleclass(core::vector3df p1, core::vector3df p2, f32 r, f32 k, ISceneNode* parent, ISceneManager* smgr, s32 id=-1);
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const core::aabbox3d<f32>& getBoundingBox() const;
    virtual u32 getMaterialCount();
    virtual video::SMaterial& getMaterial(u32 i);
    virtual void setMaterial(video::SMaterial newMaterial);
    void createCircle(core::vector3df p1, core::vector3df p2, f32 r, f32 k);
};

}} // namespaces 
