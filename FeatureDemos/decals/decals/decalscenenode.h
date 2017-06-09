#include <irrlicht.h>
#include <time.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

class DecalSceneNode : public scene::ISceneNode
{
protected:
   core::aabbox3d<f32> Box;
   video::S3DVertex Vertices[4];
   video::SMaterial Material;

public:

DecalSceneNode(scene::ISceneNode* parent, scene::ISceneManager* mgr,
                video::ITexture* image,float size);

      vector3df pointA;
      vector3df pointB;
      vector3df pointC;
      float posA;
      float posB;
      float posC;
      vector3df trinormal;
      float basesize;

       IVideoDriver* driver;

       line3d<f32> line1;
       vector3df v1;
       vector3df v2;
       line3d<f32> line2;
       vector3df v3;

       vector3df squarepA;
       vector3df squarepB;
       vector3df squarepC;
       vector3df squarepD;



      time_t c_time;
      time_t d_time;
      double lifeSpan;

   void   Setup(triangle3df tri,   vector3df intersection);

void setLifeSpan(double seconds);

void OnRegisterSceneNode();

void render();

const core::aabbox3d<f32>& getBoundingBox() const;

};
