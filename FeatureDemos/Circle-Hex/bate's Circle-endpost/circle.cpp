/*
// EXAMPLE
vector3df p1 = vector3df(52,0,0);
vector3df p2 = vector3df(0,52,0);

circleclass* circleNode = new circleclass(p1, p2, 52, 100, smgr->getRootSceneNode(), smgr);
circleNode->drop(); 
*/


#include "circle.h"

using namespace irr;
using namespace core;
using namespace video;

namespace irr{
namespace scene{

circleclass::circleclass(core::vector3df p1, core::vector3df p2, f32 r, f32 k, ISceneNode* parent, ISceneManager* smgr, s32 id):ISceneNode(parent,smgr,id){
  Driver = SceneManager->getVideoDriver();
  Material.EmissiveColor = SColor(255, 255,0,0);
  if(Parent) Parent->addChild(this);
  updateAbsolutePosition();
  createCircle(p1, p2, r, k);
  AutomaticCullingState = EAC_FRUSTUM_BOX;
}

void circleclass::OnRegisterSceneNode(){
  if(IsVisible) SceneManager->registerNodeForRendering(this);
  ISceneNode::OnRegisterSceneNode();
}

void circleclass::render(){
  // Prep to render
  Driver->setMaterial(Material);
  Driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
  // render
  Driver->drawVertexPrimitiveList(&lstVertices[0], lstVertices.size(), &lstIndices[0], lstIndices.size() - 1, video::EVT_STANDARD, EPT_LINE_STRIP, video::EIT_16BIT);
}

const core::aabbox3d<f32>& circleclass::getBoundingBox() const{
  return Box;
}
u32 circleclass::getMaterialCount(){
  return 1;
}
video::SMaterial& circleclass::getMaterial(u32 i){
  return Material;
}
void circleclass::setMaterial(video::SMaterial newMaterial){
  Material = newMaterial;
}

void circleclass::createCircle(core::vector3df p1, core::vector3df p2, f32 r, f32 k){

lstIndices.push_back(lstVertices.size());
lstVertices.push_back( S3DVertex(p1, vector3df(0,0,0), SColor(255, 255,0,0), vector2df(0,0) ));



for(f32 i = 1; i < k; i++)
{

f32 t;


t = squareroot(
                ( pow(r,2) * pow(k,2) ) /
                  (
                    ( pow((k-i),2) * pow(p1.X,2) + 2 * (k-i) * p1.X * i * p2.X + pow(i,2) * pow(p2.X,2) ) +
                    ( pow((k-i),2) * pow(p1.Y,2) + 2 * (k-i) * p1.Y * i * p2.Y + pow(i,2) * pow(p2.Y,2) ) +
                    ( pow((k-i),2) * pow(p1.Z,2) + 2 * (k-i) * p1.Z * i * p2.Z + pow(i,2) * pow(p2.Z,2) )
                  )
              );

S3DVertex newVertex;

newVertex.Pos = t * (
                      ( ((k-i) / k) * p1 ) +
                      ( (i/k) * p2 )
                    );

newVertex.Color = SColor(255, 255,0,0);


lstIndices.push_back(lstVertices.size());
lstVertices.push_back(newVertex);


}


lstIndices.push_back(lstVertices.size());
lstVertices.push_back( S3DVertex(p2, vector3df(0,0,0), SColor(255, 255,0,0), vector2df(0,0) ));


}

}} // namespaces 
