#include "cMirror.h"
#include "Irrlicht.h"
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


using namespace std;

	cMirror::~cMirror(){
      free(reflectTexture);
   }
   
	cMirror::cMirror(scene::ISceneNode* parent, scene::ISceneManager* mgr, s32 id, IrrlichtDevice* myDevice, scene::ICameraSceneNode* Camera, video::ITexture * mirrorOverlay) : scene::ISceneNode(parent, mgr, id)
   {
      defaultCamera = Camera;
      w=25;
      h=25;
      video::IVideoDriver* driver = myDevice->getVideoDriver();
      if (driver->queryFeature(video::EVDF_RENDER_TO_TARGET)) //If the driver can handle Render-To-Target, then...
      {
         
         reflectTexture = driver->addRenderTargetTexture(core::dimension2d<s32>(1024,1024));
         this->setMaterialTexture(0, reflectTexture); // set material of mirror to render target and settings
         this->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
         this->setMaterialFlag(video::EMF_LIGHTING, false);
         this->setMaterialFlag(video::EMF_WIREFRAME,false);
         if(mirrorOverlay!=0) // If the mirror has an extra texture, apply it.
         {
            hasExtraTexture=true;
			cube = SceneManager->addCubeSceneNode(50);
			cube->setMaterialTexture(0,mirrorOverlay);
            cube->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
			cube->setRotation(getRotation());
			cube->setScale(vector3df(getScale().X,getScale().Y,0.01f));
			cube->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
            cube->setMaterialFlag(video::EMF_LIGHTING, false);
         }
		 else
			hasExtraTexture=false;
         this->setMaterialType(video::EMT_SOLID);
         reflectCamera = SceneManager->addCameraSceneNode(0, this->getPosition(),  // add dynamic camera..
         defaultCamera->getPosition()); //Looking at the camera. This alows you to align the near-
                                        //view plane directly with the mirror plane.
      }
      else
      {
         //TODO: add error message
      }
      if (driver->getDriverType() == video::EDT_OPENGL)
      {
         //EDT_OPENGL requires we flip the image vertically
         Vertices[0] = video::S3DVertex(-h,w,0, 1,1,0, video::SColor(255,255,255,255), 0, 1);
         Vertices[1] = video::S3DVertex(-h,-w,0, 1,0,0, video::SColor(255,255,255,255), 0, 0);
         Vertices[2] = video::S3DVertex(h,w,0, 0,1,1, video::SColor(255,255,255,255), 1, 1);
         Vertices[3] = video::S3DVertex(h,-w,0, 0,0,1, video::SColor(255,255,255,255), 1, 0);
      }
      else
      {
         //For other drivers don't need to flip it
         Vertices[0] = video::S3DVertex(-h,-w,0, 1,1,0, video::SColor(255,255,255,255), 1, 1);
         Vertices[1] = video::S3DVertex(-h,w,0, 1,0,0, video::SColor(255,255,255,255), 1, 0);
         Vertices[2] = video::S3DVertex(h,-w,0, 0,1,1, video::SColor(255,255,255,255), 0, 1);
         Vertices[3] = video::S3DVertex(h,w,0, 0,0,1, video::SColor(255,255,255,255), 0, 0);
      }

      Box.reset(Vertices[0].Pos);
      for (s32 i=1; i<4; ++i)
         Box.addInternalPoint(Vertices[i].Pos);

     //C++ computation of cos/sin/tan can be slow, so precompute values and
     //create a lookup table for the values we need.
     for(s32 i = 0; i<360; i++){
        facos[i]=acos((f32)i);
     }
		vector3df rotate = getRotation();
		vUp = vector3df(0,1,0);
		
		core::matrix4 m;
		m.setRotationDegrees(rotate);
		m.rotateVect(vUp);  
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
         mirrorNormal = mirrorV1.crossProduct(mirrorV2);

         // Normalize the Normal (to a length of 1).
         mirrorNormal = mirrorNormal.normalize();
   }

	f32 cMirror::acos_f(f32 deg)
   {
      // Fast computation of acos
      deg = (f32)fmodf(deg,360);
      if(deg<0)
         deg+=360;

      s32 deg_int = (s32)deg;
      f32 deg_frac = deg - deg_int;

      return(facos[deg_int] + deg_frac*(facos[deg_int+1] - facos[deg_int]));
   }

   //Basic math functions, inlined because the call to the functions
   //take as much time as the functions themselves!
   inline irr::f32 cMirror::len(core::vector3df v)  //vector length
   {
      return (irr::f32)sqrt((v.X * v.X) + (v.Y * v.Y) + (v.Z * v.Z));
   }

   inline irr::f32 cMirror::len2(core::vector3df v,core::vector3df v2)  //Distance between
   {                                             //two vectors
      f32 dx = v.X - v2.X;
     f32 dy= v.Y -v2.Y;
     f32 dz= v.Z-v2.Z;
      return (irr::f32)sqrt((dx * dx) + (dy * dy) + (dz * dz));
   }
   
   
   inline irr::f32 cMirror::acosd(irr::f32 input) //arc-cosine in degrees, uses f32 type (instead of "double")
   {                                     //Modified to use Fast acos instead of acos. Minute error rating
                                        //with a hit of speed.
      f32 value = acos_f(input) * RADTODEG; //Converts from radians back to degrees
      return value;
   }

   inline irr::f32 cMirror::angle(core::vector3df v1, core::vector3df v2) //angle between two vectors
   {
      if ((len(v1) == 0) || (len(v2) == 0))
      {
         return 0; //avoid division by zero!
      } else {
         return acosd(v1.dotProduct(v2) / (len(v1) * len(v2)));
      }
   }


	inline void cMirror::localVectorRotation(vector3df & vector, const vector3df & rotation)
	{
	   quaternion MrQuaternion;
	   matrix4 MrMatrix;
	   vector3df forward = vector3df(0,0,1);
	   vector3df up = vector3df(0,1,0);
	   vector3df right = vector3df(1,0,0);

	   (void)MrQuaternion.fromAngleAxis(DEGTORAD*rotation.X, right);
	   MrQuaternion.getMatrix(MrMatrix);
	   MrMatrix.rotateVect(vector);
	   MrMatrix.rotateVect(up);
	   MrMatrix.rotateVect(forward);

	   (void)MrQuaternion.fromAngleAxis(DEGTORAD*rotation.Y, up);
	   MrQuaternion.getMatrix(MrMatrix);
	   MrMatrix.rotateVect(vector);
	   MrMatrix.rotateVect(forward);

	   (void)MrQuaternion.fromAngleAxis(DEGTORAD*rotation.Z, forward);
	   MrQuaternion.getMatrix(MrMatrix);
	   MrMatrix.rotateVect(vector);
	} 
   void cMirror::reflect() //Does everything required to reflect the image.
      {           //All put into one function to reduce funtion calls.
         f32 myAngle, dP;
         core::vector3df reflectionVector,//vector of the reflection
            sightVector;//vector of the camera to the mirror
         bool oldFrontSide;
         static bool frontSide = true;
         
         // Store the node's vector to the mirror
         sightVector = defaultCamera->getPosition()-getPosition();
         
         //check to see if this is the front of the mirror. This mirror is
         //one sided, do to the fact that in most games, mirrors will not be
         //floating in the middle of the air, but attached to another suface.
         //Also, mirror calculations can be stopped to increase FPS when you
         //dont need the mirror.
         myAngle = angle(sightVector, mirrorNormal);
         oldFrontSide = frontSide;
         if (myAngle < 90)
            frontSide = false;
         else
            frontSide = true;
         video::IVideoDriver* driver = SceneManager->getVideoDriver();
         if (!frontSide)//not the front, set render to black and exit.
         {
            driver->setRenderTarget(reflectTexture, true, true, video::SColor(0,0,0,0));
            driver->setRenderTarget(0);
            return;
         }

         // Calculate the "dot product" - used to find the reflection
         //vector below
         dP = 2.0f * mirrorNormal.dotProduct(sightVector);
         // Standard formula for a reflection vector!
         reflectionVector = core::vector3df(dP, dP, dP) * mirrorNormal - sightVector;
         //Set the lenght of the reflection vector to the length of
         //the matching sight vector. Used in camera placement.
         reflectionVector.setLength(len(sightVector));

          
         //get half the length between the 2 cameras to find
         //the near clipping plane.
         f32 length2 = len2(defaultCamera->getPosition(),
            reflectCamera->getPosition())/2;

         //create_glfrustumLH creates the view frustum from the camera to the
         //mirror (remeber reflection camera is actually pointing to the
         //original camera) But create_glfrustumLH requires fixing. 
         reflectCamera->setPosition(getPosition()-reflectionVector);
         reflectCamera->setUpVector(vUp);

         //Target the default camera.
         reflectCamera->setTarget(defaultCamera->getPosition());

         //Rotate the reflection vector to offset the rotation to the
         //mirror.
		 localVectorRotation(reflectionVector,-getRotation());

		 //Create the view frustum. w= half the width of the mirror. For scalability,
         // times this by getScale().X. for H, multiply it by getScale().Y.
         // reflecionVector is the center of the mirror from the camera.
         reflectCamera->setProjectionMatrix(create_glfrustumLH((reflectionVector.X/2-
            (w/2*getScale().X)),(reflectionVector.X/2+(w/2*getScale().X))
            ,reflectionVector.Y/-2-(h/2*getScale().Y),
            reflectionVector.Y/-2+(h/2*getScale().Y),length2/2,
            defaultCamera->getFarValue()));
         
         
		 if(hasExtraTexture)
			 cube->setVisible(false);
      
         //render the image onto the mirror.
         driver->setRenderTarget(reflectTexture, true, true, video::SColor(0,0,0,0)); // set render target texture (the mirror's texture)
         this->setVisible(false); // make mirror invisible (so it doesn't get painted over)
         SceneManager->setActiveCamera(reflectCamera); // set reflection camera as active camera
         SceneManager->drawAll(); // draw whole scene into render buffer

		 //Reset everything
		 if(hasExtraTexture)
			 cube->setVisible(true);
         driver->setRenderTarget(0); // set back old render target
         this->setVisible(true); // make the mirror visible again
         SceneManager->setActiveCamera(defaultCamera);
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