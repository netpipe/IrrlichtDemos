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
    f32 Radius;
    core::vector3df Normal;
    core::vector3df Center;

  public:
    circleclass(core::vector3df center, f32 radius, u32 vertexcount, core::vector3df normal, ISceneNode* parent, ISceneManager* smgr, s32 id=-1);
    virtual void OnRegisterSceneNode();
    virtual void render();
    virtual const core::aabbox3d<f32>& getBoundingBox() const;
    virtual u32 getMaterialCount();
    virtual video::SMaterial& getMaterial(u32 i);
    virtual void setMaterial(video::SMaterial newMaterial);
    void createCircle(core::vector3df center, f32 radius, u32 vertexcount, core::vector3df normal);

};

}} // namespaces
