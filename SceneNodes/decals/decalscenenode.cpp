
/*
bullethole/decal class
by Armen138

before creating an instance of this class, i assume you got:
1. the triangle you hit with your weapon
2. the coordinates(intersection point) you hit the triangle at
3. the texture you want to display on the decal
4. the size you want the bullethole to be(you'll have to experiment a little with this one)

these are the last 4 arguments for the constructor.

this will create a square decal displaying your texture on the wall you hit
with a weapon shooting in a straight line.
*/ 
#include <irrlicht.h>
#include "decalscenenode.h"
#include "time.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

DecalSceneNode::DecalSceneNode(ISceneNode* parent, ISceneManager* mgr,  ITexture* image, float size): ISceneNode(parent, mgr, 0)
{

        IVideoDriver* driver = SceneManager->getVideoDriver();
        basesize=size/2;
        time(&c_time);
        lifeSpan=0;

        Material.Wireframe = false;
        Material.Lighting = false;
        Material.setTexture(0, image);
   //   Material.MaterialTypeParam  = 0.01f; //suposedly this will feather the falloff
        Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
        //Material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL;

        //setMaterialFlag(video::EMF_LIGHTING, false);
        //this->setMaterialType( video::EMT_TRANSPARENT_ALPHA_CHANNEL  );
        //this->setMaterialTexture(0, image );
        //this->setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL);
}

void DecalSceneNode::Setup(triangle3df tri,
            vector3df intersection){

    SColor color(255,255,255,255);


    pointA=tri.pointA;
      pointB=tri.pointB;
      pointC=tri.pointC;


      posA=intersection.X;
      posB=intersection.Y;
      posC=intersection.Z;

      trinormal=tri.getNormal();
      trinormal=trinormal.normalize();

       line3d<f32> line1(pointA, pointB);
       vector3df v1 = line1.getVector().normalize();
       vector3df v2 = line1.getClosestPoint( pointC );
       line3d<f32> line2(v2,pointC);
       vector3df v3 = line2.getVector().normalize();

       vector3df squarepA=(v1*-basesize)+trinormal*1+(v3*-basesize);
       vector3df squarepB=squarepA+(v1*basesize*2);
       vector3df squarepC=squarepA+(v1*basesize*2)+(v3*basesize*2);
       vector3df squarepD=squarepA+(v3*basesize*2);


       squarepA=squarepA+vector3df(posA,posB,posC);
       squarepB=squarepB+vector3df(posA,posB,posC);
       squarepC=squarepC+vector3df(posA,posB,posC);
       squarepD=squarepD+vector3df(posA,posB,posC);

       Vertices[0] = S3DVertex(squarepA.X,squarepA.Y,squarepA.Z, 1,0,0,color,1,0);
       Vertices[1] = S3DVertex(squarepB.X,squarepB.Y,squarepB.Z, 1,0,0,color,1,1);
       Vertices[2] = S3DVertex(squarepC.X,squarepC.Y,squarepC.Z, 1,0,0,color,0,1);
       Vertices[3] = S3DVertex(squarepD.X,squarepD.Y,squarepD.Z, 1,0,0,color,0,0);

       Box.reset(Vertices[0].Pos);
       for (s32 i=1; i<4; ++i)
       {
           Box.addInternalPoint(Vertices[i].Pos);
       }
    }

void DecalSceneNode::setLifeSpan(double seconds){
lifeSpan=seconds;
}

void DecalSceneNode::OnRegisterSceneNode()
{
if(lifeSpan>0)
{
if(difftime(time(NULL),c_time) > lifeSpan)
SceneManager->addToDeletionQueue(this);
}


if (IsVisible)
SceneManager->registerNodeForRendering(this);
ISceneNode::OnRegisterSceneNode();
}

void DecalSceneNode::render()
{
u16 indices[] = { 0,1,2, 0,2,3};
IVideoDriver* driver = SceneManager->getVideoDriver();
driver->setMaterial(Material);
driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 2);
}

const core::aabbox3d<f32>& DecalSceneNode::getBoundingBox() const
{
return Box;
}

