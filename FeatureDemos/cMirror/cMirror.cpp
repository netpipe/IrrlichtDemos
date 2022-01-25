#include "cMirror.h"
#include "irrlicht.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

using namespace std;

	cMirror::~cMirror(){
		ReflectTexture->drop();
		Overlay->drop();
		ReflectCamera->drop();
   }

	cMirror::cMirror(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IrrlichtDevice* myDevice, scene::ICameraSceneNode* Camera, video::ITexture * mirrorOverlay) : scene::ISceneNode(parent, mgr, id)
   {
      DefaultCamera = Camera;
      W=25;
      H=25;
      video::IVideoDriver* driver = myDevice->getVideoDriver();
      if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) //If the driver can handle Render-To-Target, then...
      {


		  ReflectTexture = driver->addRenderTargetTexture(core::dimension2d<u32>(1024,1024));
         this->setMaterialTexture(0, ReflectTexture); // set material of mirror to render target and settings
         this->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
         this->setMaterialFlag(video::EMF_LIGHTING, false);
         this->setMaterialFlag(video::EMF_WIREFRAME,false);
         if(mirrorOverlay!=0) // If the mirror has an extra texture, apply it.
         {
            HasExtraTexture=true;
			Overlay = SceneManager->addCubeSceneNode(50);
			Overlay->setMaterialTexture(0,mirrorOverlay);
            Overlay->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			Overlay->setRotation(getRotation());
			Overlay->setScale(vector3df(getScale().X,getScale().Y,0.01f));
			Overlay->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            Overlay->setMaterialFlag(video::EMF_LIGHTING, false);
         }
		 else
			HasExtraTexture=false;
         this->setMaterialType(video::EMT_SOLID);
         ReflectCamera = SceneManager->addCameraSceneNode(0, this->getPosition(),  // add dynamic camera..
         DefaultCamera->getPosition()); //Looking at the camera. This alows you to align the near-
                                        //view plane directly with the mirror plane.
      }
      else
      {
         //TODO: add error message
      }
         Vertices[0] = video::S3DVertex(-H,-W,0, 1,1,0, video::SColor(255,255,255,255), 1, 1);
         Vertices[1] = video::S3DVertex(-H,W,0, 1,0,0, video::SColor(255,255,255,255), 1, 0);
         Vertices[2] = video::S3DVertex(H,-W,0, 0,1,1, video::SColor(255,255,255,255), 0, 1);
         Vertices[3] = video::S3DVertex(H,W,0, 0,0,1, video::SColor(255,255,255,255), 0, 0);

		//C++ computation of cos/sin/tan can be slow, so precompute values and
     //create a lookup table.
     for(int i = 0; i<360; i++){
        Facos[i]=(f32)acos((float)i);
     }
      Box.reset(Vertices[0].Pos);
      for (s32 i=1; i<4; ++i)
         Box.addInternalPoint(Vertices[i].Pos);

		vector3df rotate = getRotation();
		UpVector = vector3df(0,1,0);

		core::matrix4 m;
		m.setRotationDegrees(rotate);
		m.rotateVect(UpVector);
		// Get a vector on the surface of the mirror

		 vector3df mirrorV1 = Vertices[0].Pos - Vertices[3].Pos;
         mirrorV1.rotateYZBy(rotate.X, core::vector3df(0, 0, 0));
         mirrorV1.rotateXZBy(-rotate.Y, core::vector3df(0, 0, 0));
         mirrorV1.rotateXYBy(rotate.Z, core::vector3df(0, 0, 0));

         // Get another vector on the surface of the mirror
         vector3df mirrorV2 = Vertices[2].Pos - Vertices[1].Pos;
         mirrorV2.rotateYZBy(rotate.X, core::vector3df(0, 0, 0));
         mirrorV2.rotateXZBy(-rotate.Y, core::vector3df(0, 0, 0));
         mirrorV2.rotateXYBy(rotate.Z, core::vector3df(0, 0, 0));

         // Use the above 2 vectors to find the mirror's Normal
         MirrorNormal = mirrorV1.crossProduct(mirrorV2);

         // Normalize the Normal (to a length of 1).
         MirrorNormal = MirrorNormal.normalize();
   }

	inline void cMirror::localVectorRotation(vector3df & vector, const vector3df & rotation)
	{
	   quaternion q;
	   matrix4 m;
	   vector3df forward = vector3df(0,0,1);
	   vector3df up = vector3df(0,1,0);
	   vector3df right = vector3df(1,0,0);

	   (void)q.fromAngleAxis(DEGTORAD*rotation.X, right);
	   m= q.getMatrix();
	   m.rotateVect(vector);
	   m.rotateVect(up);
	   m.rotateVect(forward);

	   (void)q.fromAngleAxis(DEGTORAD*rotation.Y, up);
	   m=q.getMatrix();
	   m.rotateVect(vector);
	   m.rotateVect(forward);

	   (void)q.fromAngleAxis(DEGTORAD*rotation.Z, forward);
	   m=q.getMatrix();
	   m.rotateVect(vector);
	}

   void cMirror::reflect() //Does everything required to reflect the image.
      {           //All put into one function to reduce funtion calls.
		  if(!IsVisible){
			 return;
		  }
		  //check to see if this is the front of the mirror. This mirror is
         //one sided, do to the fact that in most games, mirrors will not be
         //floating in the middle of the air, but attached to another suface.
         //Also, mirror calculations can be stopped to increase FPS when you
         //dont need it.
		  f32 myAngle, dP;
         core::vector3df reflectionVector,
            sightVector;
         bool oldFrontSide;
         static bool frontSide = true;
		 sightVector = DefaultCamera->getPosition()-getPosition();
         myAngle = angle(sightVector, MirrorNormal);
         oldFrontSide = frontSide;
         if (myAngle < 90)
            frontSide = false;
         else
            frontSide = true;
         video::IVideoDriver* driver = SceneManager->getVideoDriver();
         if (!frontSide)
         {
            driver->setRenderTarget(ReflectTexture, true, true, video::SColor(0,0,0,0));
            driver->setRenderTarget(0);
            return;
         }


         // Calculate the "dot product" - used to find the reflection
         //vector below
         dP = 2.0f * MirrorNormal.dotProduct(sightVector);
         // Standard formula for a reflection vector!
         reflectionVector = core::vector3df(dP, dP, dP) * MirrorNormal - sightVector;
         //Set the lenght of the reflection vector to the length of
         //the matching sight vector. Used in camera placement.
		 reflectionVector.setLength(sightVector.getLength());


         //get half the length between the 2 cameras to find
         //the near clipping plane.
         f32 length2 = len2(DefaultCamera->getPosition(),
            ReflectCamera->getPosition())/2;

         //create_glfrustumLH creates the view frustum from the camera to the
         //mirror (remeber reflection camera is actually pointing to the
         //original camera) But create_glfrustumLH requires fixing.
         ReflectCamera->setPosition(getPosition()-reflectionVector);
         ReflectCamera->setUpVector(UpVector);

         //Target the default camera.
         ReflectCamera->setTarget(DefaultCamera->getPosition());

         //Rotate the reflection vector to offset the rotation to the
         //mirror.
		 localVectorRotation(reflectionVector,-getRotation());

		 //Create the view frustum. W= half the width of the mirror. For scalability,
         // times this by getScale().X. for H, multiply it by getScale().Y.
         // reflecionVector is the center of the mirror from the camera.
         ReflectCamera->setProjectionMatrix(create_glfrustumLH((reflectionVector.X/2-
            (W/2*getScale().X)),(reflectionVector.X/2+(W/2*getScale().X))
            ,reflectionVector.Y/-2-(H/2*getScale().Y),
            reflectionVector.Y/-2+(H/2*getScale().Y),length2/2,
            DefaultCamera->getFarValue()));


		 if(HasExtraTexture)
			 Overlay->setVisible(false);

         //render the image onto the mirror.
         SceneManager->setActiveCamera(ReflectCamera); // set reflection camera as active camera
		 driver->setRenderTarget(ReflectTexture, true, true, video::SColor(0,0,0,0)); // set render target texture (the mirror's texture)
         this->setVisible(false); // make mirror invisible (so it doesn't get painted over)
         driver->clearZBuffer();

		 SceneManager->drawAll(); // draw whole scene into render buffer
		 //Reset everything
		 if(HasExtraTexture)
			 Overlay->setVisible(true);
         driver->setRenderTarget(0); // set back old render target
         this->setVisible(true); // make the mirror visible again
         SceneManager->setActiveCamera(DefaultCamera);
   }

   // function used to create the view matrix. Provided by FreakNigh.
   matrix4 cMirror::create_glfrustumLH(f32 left, f32 right, f32 bottom, f32 top, f32 nearZ, f32 farZ)
   {
      f32 x = (2.0F*nearZ) / (right-left);
      f32 y = (2.0F*nearZ) / (top-bottom);
      f32 a = (right+left) / (right-left);
      f32 b = (top+bottom) / (top-bottom);
      f32 c = -(farZ+nearZ) / ( farZ-nearZ);
      f32 d = -(2.0F*farZ*nearZ) / (farZ-nearZ);

      matrix4 m;
      m.makeIdentity();
   #define M(row,col)  m[col*4+row]
      M(0,0) = x;     M(0,1) = 0.0F;  M(0,2) = a;      M(0,3) = 0.0F;
      M(1,0) = 0.0F;  M(1,1) = y;     M(1,2) = b;      M(1,3) = 0.0F;
      M(2,0) = 0.0F;  M(2,1) = 0.0F;  M(2,2) = -c;      M(2,3) = d;
      M(3,0) = 0.0F;  M(3,1) = 0.0F;  M(3,2) = 1.0F;  M(3,3) = 0.0F;
   #undef M

      return m;
   }
