// Copyright NVIDIA Corporation 2002-2005
// TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THIS SOFTWARE IS PROVIDED
// *AS IS* AND NVIDIA AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES, EITHER EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL NVIDIA OR ITS SUPPLIERS
// BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE USE OF OR INABILITY TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES 

#pragma once

#include "nvutil/Handle.h" // Handle<T> definition

// required declaration
namespace nvmath
{ 
  class Trafo;
}

namespace nvsg
{
// Object types
class Object;
  // cameras ...
  class Camera;
    class ParallelCamera;
    class PerspectiveCamera;
  // nodes ...
  class Node;
    // ... groups
    class Group;
      class Billboard;
      class LOD;
      class Transform;
        class AnimatedTransform;
      class Switch;
    // ... lights
    class LightSource;
      class DirectedLight;
      class PointLight;
      class SpotLight;
    // ... geometry
    class GeoNode;
    // ... volume
    class VolumeNode;
  class VolumeBrick;

  // drawables
  class Drawable;
    // ... primitive sets
    class PrimitiveSet;
      // ... independent
      class IndependentPrimitiveSet;
        class Quads;
          class AnimatedQuads;
        class Triangles;
          class AnimatedTriangles;
          class SkinnedTriangles;
        class Lines;
        class Points;
      // ... meshed
      class MeshedPrimitiveSet;
        class QuadMeshes;
      // ... stripped
      class StrippedPrimitiveSet;
        class QuadStrips;
        class TriFans;
        class TriStrips;
        class LineStrips;
    // ... shapes
    class Shape;

  // vertex attributes
  class VertexAttributeSet;

  // states
  class StateAttribute;
    class CgFx;
    class CgFxEffect;
    class Material;
    class TextureAttribute;
    class TextureAttributeItem;
    class BlendAttribute;
    class DepthAttribute;
    class PrimitiveAttribute;
      class FaceAttribute;
      class LineAttribute;
      class PointAttribute;
  class StateSet;
  class StateVariant;
  class StatePass;

  // animations
  template<typename T> class Animation;
    template<typename T> class FramedAnimation;
    template<typename T> class InterpolatedAnimation;
      template<typename T> class LinearInterpolatedAnimation;

  // highlight objects
  class HighlightObject;
    class HighlightBox;
    class HighlightEdges;

  // maps
  class Map;
    class Bitmap;
  class MapObject;
    class MapArray;
    class MapElement;
    class MapMatrix;
  class MapList;

// additionally required declarations
class CullData;
class Scene;
class ViewState;
class TextureImage;
class TextureImageInfo;
class VolumeTexture;
class VNVector;
typedef std::vector<unsigned int> IndexSet;
class IndexMesh;

// Handle types
typedef nvutil::Handle<Object> ObjectHandle;
typedef nvutil::Handle<Camera, ObjectHandle> CameraHandle;
typedef nvutil::Handle<ParallelCamera, CameraHandle> ParallelCameraHandle;
typedef nvutil::Handle<PerspectiveCamera, CameraHandle> PerspectiveCameraHandle;
typedef nvutil::Handle<Node,ObjectHandle> NodeHandle;
typedef nvutil::Handle<Group,NodeHandle> GroupHandle;
typedef nvutil::Handle<Billboard,GroupHandle> BillboardHandle;
typedef nvutil::Handle<LOD,GroupHandle> LODHandle;
typedef nvutil::Handle<Transform,GroupHandle> TransformHandle;
typedef nvutil::Handle<AnimatedTransform,TransformHandle> AnimatedTransformHandle;
typedef nvutil::Handle<Switch,GroupHandle> SwitchHandle;
typedef nvutil::Handle<LightSource,ObjectHandle> LightSourceHandle;
typedef nvutil::Handle<DirectedLight,LightSourceHandle> DirectedLightHandle;
typedef nvutil::Handle<PointLight,LightSourceHandle> PointLightHandle;
typedef nvutil::Handle<SpotLight,LightSourceHandle> SpotLightHandle;
typedef nvutil::Handle<GeoNode,NodeHandle> GeoNodeHandle;
typedef nvutil::Handle<Drawable,ObjectHandle> DrawableHandle;
typedef nvutil::Handle<PrimitiveSet,DrawableHandle> PrimitiveSetHandle;
typedef nvutil::Handle<IndependentPrimitiveSet,PrimitiveSetHandle> IndependentPrimitiveSetHandle;
typedef nvutil::Handle<Quads,IndependentPrimitiveSetHandle> QuadsHandle;
typedef nvutil::Handle<AnimatedQuads,QuadsHandle> AnimatedQuadsHandle;
typedef nvutil::Handle<Triangles,IndependentPrimitiveSetHandle> TrianglesHandle;
typedef nvutil::Handle<AnimatedTriangles,TrianglesHandle> AnimatedTrianglesHandle;
typedef nvutil::Handle<SkinnedTriangles,TrianglesHandle> SkinnedTrianglesHandle;
typedef nvutil::Handle<Lines,IndependentPrimitiveSetHandle> LinesHandle;
typedef nvutil::Handle<Points,IndependentPrimitiveSetHandle> PointsHandle;
typedef nvutil::Handle<MeshedPrimitiveSet,PrimitiveSetHandle> MeshedPrimitiveSetHandle;
typedef nvutil::Handle<QuadMeshes,MeshedPrimitiveSetHandle> QuadMeshesHandle;
typedef nvutil::Handle<StrippedPrimitiveSet,PrimitiveSetHandle> StrippedPrimitiveSetHandle;
typedef nvutil::Handle<QuadStrips,StrippedPrimitiveSetHandle> QuadStripsHandle;
typedef nvutil::Handle<TriFans,StrippedPrimitiveSetHandle> TriFansHandle;
typedef nvutil::Handle<TriStrips,StrippedPrimitiveSetHandle> TriStripsHandle;
typedef nvutil::Handle<LineStrips,StrippedPrimitiveSetHandle> LineStripsHandle;
typedef nvutil::Handle<Shape,DrawableHandle> ShapeHandle;
typedef nvutil::Handle<VertexAttributeSet,ObjectHandle> VertexAttributeSetHandle;
typedef nvutil::Handle<StateAttribute,ObjectHandle> StateAttributeHandle;
typedef nvutil::Handle<CgFx,StateAttributeHandle> CgFxHandle;
typedef nvutil::Handle<CgFxEffect> CgFxEffectHandle;
typedef nvutil::Handle<Material,StateAttributeHandle> MaterialHandle;
typedef nvutil::Handle<TextureAttribute,StateAttributeHandle> TextureAttributeHandle;
typedef nvutil::Handle<TextureAttributeItem,ObjectHandle> TextureAttributeItemHandle;
typedef nvutil::Handle<BlendAttribute,StateAttributeHandle> BlendAttributeHandle;
typedef nvutil::Handle<DepthAttribute,StateAttributeHandle> DepthAttributeHandle;
typedef nvutil::Handle<PrimitiveAttribute,StateAttributeHandle> PrimitiveAttributeHandle;
typedef nvutil::Handle<FaceAttribute,PrimitiveAttributeHandle> FaceAttributeHandle;
typedef nvutil::Handle<LineAttribute,PrimitiveAttributeHandle> LineAttributeHandle;
typedef nvutil::Handle<PointAttribute,PrimitiveAttributeHandle> PointAttributeHandle;
typedef nvutil::Handle<StateSet,ObjectHandle> StateSetHandle;
typedef nvutil::Handle<StateVariant,ObjectHandle> StateVariantHandle;
typedef nvutil::Handle<StatePass,ObjectHandle> StatePassHandle;
typedef nvutil::Handle<Animation<VNVector>, ObjectHandle> VNVectorAnimationHandle;
typedef nvutil::Handle<FramedAnimation<VNVector>, VNVectorAnimationHandle> FramedVNVectorAnimationHandle;
typedef nvutil::Handle<InterpolatedAnimation<VNVector>, VNVectorAnimationHandle> InterpolatedVNVectorAnimationHandle;
typedef nvutil::Handle<LinearInterpolatedAnimation<VNVector>, InterpolatedVNVectorAnimationHandle> LinearInterpolatedVNVectorAnimationHandle;
typedef nvutil::Handle<Animation<nvmath::Trafo>, ObjectHandle> TrafoAnimationHandle;
typedef nvutil::Handle<FramedAnimation<nvmath::Trafo>, TrafoAnimationHandle> FramedTrafoAnimationHandle;
typedef nvutil::Handle<InterpolatedAnimation<nvmath::Trafo>, TrafoAnimationHandle> InterpolatedTrafoAnimationHandle;
typedef nvutil::Handle<LinearInterpolatedAnimation<nvmath::Trafo>, InterpolatedTrafoAnimationHandle> LinearInterpolatedTrafoAnimationHandle;
typedef nvutil::Handle<HighlightObject, ObjectHandle> HighlightObjectHandle;
typedef nvutil::Handle<HighlightBox, HighlightObjectHandle> HighlightBoxHandle;
typedef nvutil::Handle<HighlightEdges, HighlightObjectHandle> HighlightEdgesHandle;
typedef nvutil::Handle<Map, ObjectHandle> MapHandle;
typedef nvutil::Handle<Bitmap, MapHandle> BitmapHandle;
typedef nvutil::Handle<MapObject, ObjectHandle> MapObjectHandle;
typedef nvutil::Handle<MapArray, MapObjectHandle> MapArrayHandle;
typedef nvutil::Handle<MapElement, MapObjectHandle> MapElementHandle;
typedef nvutil::Handle<MapMatrix, MapObjectHandle> MapMatrixHandle;
typedef nvutil::Handle<MapList, ObjectHandle> MapListHandle;
typedef nvutil::Handle<TextureImage> TextureImageHandle;
typedef nvutil::Handle<VolumeTexture,TextureImageHandle> VolumeTextureHandle;
typedef nvutil::Handle<Scene> SceneHandle;
typedef nvutil::Handle<ViewState> ViewStateHandle;
typedef nvutil::Handle<VolumeNode,NodeHandle> VolumeNodeHandle;
typedef nvutil::Handle<VolumeBrick,ObjectHandle> VolumeBrickHandle;

}
