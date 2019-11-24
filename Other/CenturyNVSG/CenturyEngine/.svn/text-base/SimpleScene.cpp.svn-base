#include "./SimpleScene.h"

#include "nvmath/nvmath.h"
#include "nvmath/Vec3f.h"
#include "nvsg/Transform.h"
#include "nvsg/Node.h"
#include "nvsg/Face.h"
#include "nvsg/Material.h"
#include "nvsg/Triangles.h"
#include "nvsg/GeoNode.h"
#include "nvsg/FaceAttribute.h"
#include "nvsg/StateSet.h"
#include "nvsg/Scene.h"

#include "nvutil/DbgNew.h" // this must be the last include

using namespace nvsg;
using namespace nvmath;
using namespace nvutil;

SceneHandle * SimpleSceneFactory::create()
{  
  SceneHandle * sceneHdl = CreateHandle(SceneHandle);
  WritableObject<Scene> theScene(sceneHdl);
  
  theScene->setBackColor(Vec3f(1,0,0)); 

  // Create the root node and apply a rotation.
  // Note: 
  // We do not increment the ref count here, 
  // because we throw the root into the scene 
  // via setRoot(), and setRoot() will increment 
  // it for us.
  Trafo rootTrafo;
  rootTrafo.setOrientation( Quatf(Vec3f(0.0f, 1.0f, 0.0f ), float(PI_QUARTER)) );
  WritableObject<Transform> pRoot( CreateHandle(TransformHandle) );
  pRoot->setTrafo( rootTrafo );

  // positioning the shapes 
  Trafo shapeTrafo;
  shapeTrafo.setOrientation( Quatf(Vec3f(1.0f, 0.0f, 0.0f ), float(PI_QUARTER)) );
  WritableObject<Transform> pShapeTrafo( CreateHandle(TransformHandle) );
  pShapeTrafo->setTrafo( shapeTrafo );

  Trafo shapeTrafo2;
  shapeTrafo2.setOrientation( Quatf(Vec3f(0.0f, 1.0f, 0.0f ), float(PI_QUARTER)) );
  WritableObject<Transform> pShapeTrafo2( CreateHandle(TransformHandle) );
  pShapeTrafo2->setTrafo( shapeTrafo2 );

  Trafo shape2Trafo;
  shape2Trafo.setOrientation( Quatf(Vec3f(1.0f, 0.0f, 0.0f ), 0.0f) );
  shape2Trafo.setTranslation( Vec3f( -1.0f, 0.0f, 0.0f ) ); 
  WritableObject<Transform> pShape2Trafo( CreateHandle(TransformHandle) );
  pShape2Trafo->setTrafo( shape2Trafo );

  Trafo shape2Trafo2;
  shape2Trafo2.setOrientation( Quatf(Vec3f(0.0f, 1.0f, 0.0f ), float(PI_QUARTER)) );
  WritableObject<Transform> pShape2Trafo2( CreateHandle(TransformHandle) );
  pShape2Trafo2->setTrafo( shape2Trafo2 );

  Trafo shape3Trafo;
  shape3Trafo.setOrientation( Quatf(Vec3f(1.0f, 0.0f, 0.0f ), 0.0f) );
  shape3Trafo.setTranslation( Vec3f( 1.0f, 0.0f, 0.0f ) ); 
  WritableObject<Transform> pShape3Trafo( CreateHandle(TransformHandle) );
  pShape3Trafo->setTrafo( shape3Trafo );

  // setup vertices and faces  
  Vec3f vertices[8] = { Vec3f(-0.2f, -0.2f,  0.2f)
    , Vec3f(-0.2f,  0.2f,  0.2f)
    , Vec3f( 0.2f,  0.2f,  0.2f)
    , Vec3f( 0.2f, -0.2f,  0.2f)
    , Vec3f( 0.2f, -0.2f, -0.2f)
    , Vec3f( 0.2f,  0.2f, -0.2f)
    , Vec3f(-0.2f,  0.2f, -0.2f)
    , Vec3f(-0.2f, -0.2f, -0.2f) };

  Face3 faces[12]     = { {0,2,1}, {2,0,3}
  , {3,5,2}, {5,3,4}
  , {4,6,5}, {6,4,7}
  , {7,1,6}, {1,7,0}
  , {0,4,3}, {4,0,7}
  , {1,5,6}, {5,1,2} };

  // calculating face normals
  Vec3f v[36];
  Vec3f n[36];
  Face3 f[12];
  Vec3f v0, v1, v2, d0, d1, fn;
  for ( int kf=0, kv=0; kf<12; kf++, kv+=3 )
  {
    v0 = vertices[faces[kf][0]];
    v1 = vertices[faces[kf][1]];
    v2 = vertices[faces[kf][2]];

    d0 = v1 - v0;
    d1 = v2 - v0;

    fn = d0^d1;
    fn.normalize();

    f[kf][0] = kv;
    f[kf][1] = kv+1;
    f[kf][2] = kv+2;

    v[kv]    = v0;
    v[kv+1]  = v1;
    v[kv+2]  = v2;

    n[kv]    = fn; 
    n[kv+1]  = fn; 
    n[kv+2]  = fn; 
  }

  // Create a VertexAttributeSet with vertices and normals
  WritableObject<VertexAttributeSet> pVAS( CreateHandle(VertexAttributeSetHandle) );
  pVAS->setVertices( v, 36 );
  pVAS->setNormals( n, 36 );

  // Create a PrimitiveSet, in this case triangles.
  WritableObject<Triangles> pShape( CreateHandle(TrianglesHandle) );
  pShape->setVertexAttributeSet( getHandle<VertexAttributeSetHandle>(pVAS) );
  pShape->setFaces(f, 12);

  // Create a material.
  WritableObject<Material> pMaterial( CreateHandle(MaterialHandle) );
  pMaterial->setAmbientColor( Vec3f(0.f, 0.f, 1.f) );
  pMaterial->setDiffuseColor( Vec3f(0.4f, 0.4f, 0.4f) );
  pMaterial->setEmissiveColor( Vec3f(0.f, 0.f, 0.f) );
  pMaterial->setSpecularColor( Vec3f(1.f, 1.f, 1.f) );
  pMaterial->setSpecularExponent( 10.f );
  pMaterial->setOpacity( 1.0f );

  // Create a face attribute. The face attribute is a state attribute 
  // that determines how the following faces will be rendered.
  FaceAttributeHandle * pFaceAttribute = CreateHandle(FaceAttributeHandle);

  // Create state set. Add material and render mode.
  WritableObject<StateSet> pStateSet( CreateHandle(StateSetHandle) );
  pStateSet->addAttribute( getHandle<MaterialHandle>(pMaterial) );
  pStateSet->addAttribute( pFaceAttribute );

  // Setup the whole scene graph
  WritableObject<GeoNode> pGeoNode( CreateHandle(GeoNodeHandle) );
  pGeoNode->addGeometry( getHandle<DrawableHandle>( pShape ), getHandle<StateSetHandle>( pStateSet ) );

  GeoNodeHandle * pGeoNode2 = getHandle<GeoNodeHandle>(pGeoNode)->clone();

  // Note: 
  // AddChild() will increment the reference count 
  // of the added node.
  pShapeTrafo2->addChild( getHandle<NodeHandle>( pGeoNode ) );
  pShapeTrafo->addChild( getHandle<NodeHandle>( pShapeTrafo2 ) );
  pShape2Trafo2->addChild( pGeoNode2 );
  pShape2Trafo->addChild( getHandle<NodeHandle>( pShape2Trafo2 ) );
  pShape3Trafo->addChild( pGeoNode2 );
  pRoot->addChild( getHandle<NodeHandle>( pShapeTrafo ) );
  pRoot->addChild( getHandle<NodeHandle>( pShape2Trafo ) );
  pRoot->addChild( getHandle<NodeHandle>( pShape3Trafo ) );

  theScene->setRootNode( getHandle<NodeHandle>( pRoot ) );

  return sceneHdl;
}


