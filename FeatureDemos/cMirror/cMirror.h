#include "irrlicht.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

class cMirror: public scene::ISceneNode
{
	   /*
   This cMirro class is a reconstruction of Panther's version of cMirror.
   It allows for near-perfect reflections, with the exception of extreme angles.
   As the reflection is caculated for the center of the mirror, when the camera
   is close to the mirror, it distorts the rest of the image at odd or extreme
   angles. It also comes with an option for an overlay material for cracks,
   dirt, or to make it a semi-reflective surface such as marble. This class
   is completely self sufficient, to use, just create your mirror object,
   apply transformations or animators, and just before your main render in
   the main loop, use [mirror name]->reflect(); cMirror uses the default
   camera instead of an object, since reflection must be made to players
   point of view.
   */
private:
   core::aabbox3d<f32> Box;
   scene::ISceneNode * Overlay;
   video::SMaterial ReflectMaterial;
   video::ITexture * ReflectTexture;
   scene::ICameraSceneNode * ReflectCamera;
   scene::ICameraSceneNode * DefaultCamera;
   f32 W, H; //hieght and width of the mirror.
   bool HasExtraTexture;
   f32 Facos[360]; //Fast acos lookup table.
   vector3df UpVector, MirrorNormal;

public:
   video::S3DVertex Vertices[4];

   ~cMirror();//deconstructor
   cMirror(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id,
	   IrrlichtDevice* myDevice, scene::ICameraSceneNode* Camera,
	   video::ITexture * mirrorOverlay= 0); //constructor


   virtual void render()//render
	   {

      u16 indices[] = {       0,2,3, 2,1,3, 1,0,3, 2,0,1      };
       video::IVideoDriver* driver = SceneManager->getVideoDriver();

       driver->setMaterial(ReflectMaterial);
       driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
       driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 4);

   }

   virtual const core::aabbox3d<f32>& getBoundingBox() const //Bounding box
   {
      return Box;
   }

   virtual u32 getMaterialCount() const//1
   {
      return 1;
   }
   virtual void OnRegisterSceneNode()//Registers Node
   {
	   if (IsVisible){

		 //SceneManager->setActiveCamera(ReflectCamera);
		   //reflect();
         SceneManager->registerNodeForRendering(this);
		 if(HasExtraTexture)
			Overlay->setVisible(true);
	   }
	   else if(HasExtraTexture){
		   Overlay->setVisible(false);
	   }

      ISceneNode::OnRegisterSceneNode();
   }

   virtual video::SMaterial& getMaterial(u32 i)//Reflection texture
   {
      return ReflectMaterial;
   }

   inline irr::f32 len(core::vector3df v)  //vector length
   {
      return (irr::f32)sqrt((v.X * v.X) + (v.Y * v.Y) + (v.Z * v.Z));
   }

   inline irr::f32 len2(core::vector3df v,core::vector3df v2)  //Distance between
   {                                             //two vectors
      f32 dx = v.X - v2.X;
     f32 dy= v.Y -v2.Y;
     f32 dz= v.Z-v2.Z;
      return (irr::f32)sqrt((dx * dx) + (dy * dy) + (dz * dz));
   }

   f32 acos_f(f32 deg)
   {
      // Fast computation of acos
      deg = (f32)fmodf(deg,360);
      if(deg<0)
         deg+=360;

      s32 deg_int = (s32)deg;
      f32 deg_frac = deg - deg_int;

      return(Facos[deg_int] + deg_frac*(Facos[deg_int+1] - Facos[deg_int]));
   }

   irr::f32 acosd(irr::f32 input) //arc-cosine in degrees, uses f32 type (instead of "double")
   {                                     //Modified to use Fast acos instead of acos. Minute error rating
                                        //with a hit of speed.
      f32 value = acos_f(input) * RADTODEG; //Converts from radians back to degrees
      return value;
   }

   irr::f32 angle(core::vector3df v1, core::vector3df v2) //angle between two vectors
   {
      if ((len(v1) == 0) || (len(v2) == 0))
      {
         return 0; //avoid division by zero!
      } else {
         return acosd(v1.dotProduct(v2) / (len(v1) * len(v2)));
      }
   }
   inline virtual void setRotation(const core::vector3df &newpos)//sets rotation and
   {
	   if(HasExtraTexture)
			Overlay->setRotation(newpos);
	   ISceneNode::setRotation(newpos);
	   vector3df rotate = getRotation();
	   UpVector = vector3df(0,1,0);
	   core::matrix4 m;
	   m.setRotationDegrees(rotate);
	   m.rotateVect(UpVector);
	   // Get a vector on the surface of the mirror
       vector3df mirrorV1 = Vertices[0].Pos - Vertices[3].Pos;
       mirrorV1.rotateYZBy(getRotation().X, core::vector3df(0, 0, 0));
       mirrorV1.rotateXZBy(-getRotation().Y, core::vector3df(0, 0, 0));
       mirrorV1.rotateXYBy(getRotation().Z, core::vector3df(0, 0, 0));

       // Get another vector on the surface of the mirror
       vector3df mirrorV2 = Vertices[2].Pos - Vertices[1].Pos;
       mirrorV2.rotateYZBy(getRotation().X, core::vector3df(0, 0, 0));
       mirrorV2.rotateXZBy(-getRotation().Y, core::vector3df(0, 0, 0));
       mirrorV2.rotateXYBy(getRotation().Z, core::vector3df(0, 0, 0));

       // Use the above 2 vectors to find the mirror's Normal
       MirrorNormal = mirrorV1.crossProduct(mirrorV2);

       // Normalize the Normal (to a length of 1).
       MirrorNormal = MirrorNormal.normalize();
   }


   virtual void setPosition(const core::vector3df &v)//sets position
   {
	   if(HasExtraTexture)
		   Overlay->setPosition(v);
	   ISceneNode::setPosition(v);
   }

   virtual void setScale(const core::vector3df &v)//sets scale
   {
	   if(HasExtraTexture)
		   Overlay->setScale(vector3df(v.X,v.Y,0.01f));
	   ISceneNode::setScale(v);
   }

   inline void localVectorRotation(vector3df & vector,
	   const vector3df & rotation);//local space rotation

   void reflect(); //reflects current world.
   inline ICameraSceneNode * getCamera() {return ReflectCamera;}
   matrix4 create_glfrustumLH(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ);

};
