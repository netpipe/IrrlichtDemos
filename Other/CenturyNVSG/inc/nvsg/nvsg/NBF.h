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
/** @file */

#include "nvsgcommon.h"
#include "nvutil/BitMask.h"

// NBF is independent of packing. However, this forces
// the compile-time asserts below to fire on inconsistencies
#pragma pack(push, 1)

// convenient aliases for used built-in types
typedef char                    byte_t;   //!< Specifies an 8-bit signed type.
typedef unsigned char           ubyte_t;  //!< Specifies an 8-bit unsigned type.
typedef int                     int_t;    //!< Specifies a 32-bit signed integer type.
typedef unsigned int            uint_t;   //!< Specifies a 32-bit unsigned integer type.
#ifdef LINUX
# if __WORDSIZE == 64           // defined indirectly through stdint.h
// avoid a conflict with GNU stdint.h on Linux64
// note: long is a 64-bit type on Linux64, while it is 32bit on Win64
// Linux64
typedef unsigned long           uint64_t; //!< Specifies a 64-bit unsigned integer type.
# else
// Linux32
typedef unsigned long long      uint64_t; //!< Specifies a 64-bit unsigned integer type.
# endif
#else
// Win32 and Win64
typedef unsigned long long      uint64_t; //!< Specifies a 64-bit unsigned integer type.
#endif
typedef float                   float2_t[2]; //!< Specifies a 2-component float vector.
typedef float                   float3_t[3]; //!< Specifies a 3-component float vector.
typedef float                   float4_t[4]; //!< Specifies a 4-component float vector.

// convenient macro to add padding bits
#define PADDING(n) PADDING_i(n,__LINE__) 
#define PADDING_i(n,l) PADDING_ii(n,l)
#define PADDING_ii(n,l) ubyte_t padding##l[n] 

// NBF version
const ubyte_t NBF_VER_MAJOR  =  0x22; //!< NBF major version number
const ubyte_t NBF_VER_MINOR  =  0x01; //!< NBF version compatibility level
const ubyte_t NBF_VER_BUGFIX =  0x00; //!< NBF version bugfix level
  
// constants specifying a certain byte order
const ubyte_t NBF_LITTLE_ENDIAN = 0x00; //!< Specifies little endian byte order
const ubyte_t NBF_BIG_ENDIAN =  0x01;   //!< Specifies big endian byte order

// convenient helper types

//! The str_t structure specifies how a string is stored in a .NBF file.
struct str_t 
{ 
  uint_t      numChars;           //!< Specifies the number of characters in the actual string,
                                  //!< not including the terminating null character.
  uint_t      chars;              //!< Specifies the file offset to the string characters.
};
NVSG_CTASSERT_MODULO_BYTESIZE(str_t,4);

//! The texCoordSet_t structure specifies how a texture coordinate set is stored in a .NBF file.
/** Texture coordinate sets, in this context, need to be considered in conjunction with NBFVertexAttributeSet objects. */
struct texCoordSet_t 
{
  uint_t      numTexCoords;       //!< Specifies the number of texture coordinates contained in the actual set.
  uint_t      coordDim;           //!< Specifies the dimension, in terms of float, of the contained texture coordinates.
                                  //!< Texture coordinates can be either one, two, three, or four dimensional.
  uint_t      texCoords;          //!< Specifies the file offset to the contained texture coordinates. 
};
NVSG_CTASSERT_MODULO_BYTESIZE(texCoordSet_t,4);

//! The geometrySet_t structure specifies how a geometry set is stored in a .NBF file.
/** Geometry sets, in this context, need to be considered in conjunction with NBFGeoNode objects. */
struct geometrySet_t
{
  uint_t      stateSet;           //!< Specifies the file offset to the corresponding NBFStateSet object.
  uint_t      numDrawables;       //!< Specifies the number of this geometry's NBFDrawable objects.
  uint_t      drawables;          //!< Specifies the file offset to the offsets to the NBFDrawable objects. 
};
NVSG_CTASSERT_MODULO_BYTESIZE(geometrySet_t,4);

//! The indexSet_t structure specifies how an index set is stored in a .NBF file.
/** Index sets, in this context, need to be considered in conjunction with NBFStrippedPrimitiveSet objects. */
struct indexSet_t
{
  uint_t      numIndices;         //!< Specifies the number of indices in the actual index set.
  uint_t      indices;            //!< Specifies the file offset to the indices. As specified by the NBF format,
                                  //!< a index is a 32-bit unsigned integer value.
};
NVSG_CTASSERT_MODULO_BYTESIZE(indexSet_t,4);

//! The meshSet_t structure specifies how an mesh set is stored in a .NBF file.
/** Mesh sets, in this context, need to be considered in conjunction with NBFMeshedPrimitiveSet objects. */
struct meshSet_t
{
  uint_t      width;              //!< Specifies the width of the mesh
  uint_t      height;             //!< Specifies the height of the mesh
  uint_t      indices;            //!< Specifies the file offset to the indices. As specified by the NBF format,
  //!< a index is a 32-bit unsigned integer value.
};
NVSG_CTASSERT_MODULO_BYTESIZE(indexSet_t,4);   //!< test size of struct

//! The texBinding_t structure specifies how a texture binding is stored in a .NBF file.
/** Texture bindings, in this context, need to be considered in conjunction with 
  * NBFTextureAttribute and NBFTextureAttributeItem objects. */
struct texBinding_t
{
  uint_t      texUnit;            //!< Specifies the texture unit where the actual NBFTextureAttributeItem object is bound to.
  uint_t      texAttribItem;      //!< Specifies the file offset to the corresponding NBFTextureAttributeItem object.
};
NVSG_CTASSERT_MODULO_BYTESIZE(texBinding_t,4);

//! The texImage_t structure specifies how a texture image is stored in a .NBF file.
/** Texture images are considered in conjunction with NBFTextureAttributeItem objects. */
struct texImage_t
{
  uint_t      flags;              //!< Creation flags.
  str_t       file;               //!< Specifies the filename of the image file in case the image is from a file.
  // the following are only relevant in case the image is not from a file but from a image data lump.
  uint_t      width;              //!< Specifies the width of the texture in pixels.
  uint_t      height;             //!< Specifies the height of the texture in pixels.
  uint_t      depth;              //!< Specifies the depth of the texture in pixels.
  PADDING(12);                    //!< Padding bits to ensure offset of scene is on a 4-byte boundary, regardless of packing.
  uint_t      pixelFormat;        //!< Specifies the format of the pixel data. 
  uint_t      dataType;           //!< Specifies the type of the pixel data.
  uint_t      pixels;             //!< Specifies the file offset to the raw pixel data.
};
NVSG_CTASSERT_MODULO_BYTESIZE(texImage_t,4);

//! The trafo_t structure specifies how a transformation is stored in a .NBF file.
struct trafo_t
{
  float4_t    orientation;        //!< Specifies the orientational part of the transformation.
  float3_t    scaling;            //!< Specifies the scaling part of the transformation.
  float3_t    translation;        //!< Specifies the translational part of the transformation.
  float3_t    center;             //!< Specifies the center of rotation of the transformation.
  float4_t    scaleOrientation;   //!< Specifies the scale orientational part of the transformation.
};
NVSG_CTASSERT_MODULO_BYTESIZE(trafo_t,4);

//! The tweak_t structure specifies how a tweakable parameter is stored in a .NBF file.
/** Tweakables need to be considered in conjunction with NBFCgFx objects. */
struct tweak_t
{
  str_t       name;               //!< Specifies the tweakable's name as it appears in the effect source.

  // type flags
  enum {
    TYPE_STRUCT = BIT0
  , TYPE_ARRAY  = BIT1
  , TYPE_FLOAT  = BIT2
  , TYPE_INT    = BIT3
  , TYPE_BOOL   = BIT4
  , TYPE_STRING = BIT5
  };
  uint_t      type;
  uint_t      numValues;          /**!< Specifies the number of values stored. 
                                        For a float4-typed parameter, for example, this is 4. */ 
  union
  {
    uint_t    floats;             /**!< Specifies the file offset to a non-string value. 
                                      Note that this offset is not a valid offset for string types!
                                      Non-string values internally always are stored in terms of floats. */
    str_t     str;                //!< Specifies the string value in case of a string parameter. 
  };
 
  // the following is relevant only for arrays and structs (nesting tweaks) 
  uint_t   numTweaks;         /**!< In case of an array parameter this specifies the array size.
                                   In case of a struct parameter this specifies the number of members. */
  uint_t   tweaks;            //!< Specifies the file offset to either the array elements or struct members.
};
NVSG_CTASSERT_MODULO_BYTESIZE(tweak_t,4);

//! The sampler_t structure specifies how a sampler parameter is stored in a .NBF file. 
/** Samplers need to be considered in conjunction with NBFCgFx objects. */
struct sampler_t
{
  str_t       name;               //!< Specifies the sampler's name as it appears in the effect source.
  uint_t      texImg;             //!< Specifies the file offset to the corresponding texture image.
};

//! The bone_t structure specifies how a bone is stored in a .NBF file. 
/** Bones need to be considered in conjunction with the skin_t structure. */
struct bone_t
{
  float       weight;             //!< Specifies a weight factor to be applied to the initial vertices and normals of the
                                  //!< actual skin.
  uint_t      animation;          //!< Specifies the file offset to the animation to be applied to the initial vertices 
                                  //!< and normals of the actual skin.
};
NVSG_CTASSERT_MODULO_BYTESIZE(bone_t,4);

//! The skin_t structure specifies how a skin is stored in a .NBF file.
/** Skins need to be considered in conjunction with NBFSkinnedTriangles objects. */
struct skin_t
{
  uint_t      numBones;           //!< Specifies the number of bones describing the actual skin.
  uint_t      bones;              //!< Specifies the file offset to the bones describing the actual skin.
                                  //!< The way bones are stored in a .NBF file is specified by the bone_t structure. 
  float3_t    vertex;             //!< Specifies the initial vertex for the actual skin.
  float3_t    normal;             //!< Specifies the initial normal for the actual skin.
};
NVSG_CTASSERT_MODULO_BYTESIZE(skin_t,4);

//! The keyVariant_t structure specifies how a pair of a VariantKey and a StateVariant is stored in a .NBF file.
struct keyVariant_t
{
  uint_t      key;                //!< Specifies the key of this pair
  uint_t      variant;            //!< Specifies the offset to an NBFStateVariant in the .NBF file.
};
NVSG_CTASSERT_MODULO_BYTESIZE(keyVariant_t,4);

//! The plane_t structure specifies how a pair of a bool and a Plane3f is stored in a .NBF file.
struct plane_t
{
  uint_t      active;             //!< Specifies if this plane is active
  float3_t    normal;             //!< Specifies the normal of the plane
  float       offset;             //!< Specifies the offset of the plane
};

//! Unique NBF Object Codes
/** Each concrete NBFObject type is assigned to a unique NBF object code. 
  * This code is a 32-bit unsigned integer value, stored at offset 0, of each concrete NBFObject.
  * The purpose of the unique 'per-object' code is to provide a Load-Time Type Information (LTTI) 
  * to resolve concrete NBFObjects while loading NBF files. */   
enum
{
  // scene object 
  NBF_UNKNOWN                     = 0x00000000  //!< Unknown, and hence, invalid object code.
  // animation objects (0x100 - 0x1FF)
, NBF_TRAFO_ANIMATION           = 0x00000100
, NBF_VNVECTOR_ANIMATION
  // framed animations
, NBF_FRAMED_ANIMATION          = 0x00000120
, NBF_FRAMED_TRAFO_ANIMATION                  //!< Identifies an NBFFramedTrafoAnimation object.
, NBF_FRAMED_VNVECTOR_ANIMATION               //!< Identifies an NBFFramedVNVectorAnimation object.
  // interpolated animations 
, NBF_INTERPOLATED_ANIMATION    = 0x00000140
, NBF_LINEAR_INTERPOLATED_TRAFO_ANIMATION     //!< Identifies an NBFInterpolatedTrafoAnimation object.
, NBF_LINEAR_INTERPOLATED_VNVECTOR_ANIMATION  //!< Identifies an NBFInterpolatedVNVectorAnimation object.
  // camera objects (0x200 - 0x2FF)
, NBF_CAMERA                    = 0x00000200
, NBF_MONO_CAMERA
, NBF_JITTER_CAMERA
, NBF_SIMPLE_CAMERA
, NBF_PARALLEL_CAMERA                         //!< Identifies an NBFParallelCamera object.
, NBF_PERSPECTIVE_CAMERA                      //!< Identifies an NBFPerspectiveCamera object.
, NBF_STEREO_CAMERA  
  // drawable objects (0x300 - 0x3FF) 
, NBF_DRAWABLE                  = 0x00000300
, NBF_VERTEX_ATTRIBUTE_SET                    //!< Identifies an NBFVertexAttributeSet object.
, NBF_TRIANGLES                               //!< Identifies an NBFIndependentPrimitiveSet object.
, NBF_ANIMATED_TRIANGLES                      //!< Identifies an NBFAnimatedIndependents object.
, NBF_SKINNED_TRIANGLES                       //!< Identifies an NBFSkinnedTriangles object.
, NBF_TRISTRIPS                               //!< Identifies an NBFStrippedPrimitiveSet object.
, NBF_QUADS                                   //!< Identifies an NBFIndependentPrimitiveSet object (support since v0.3).
, NBF_ANIMATED_QUADS                          //!< Identifies an NBFAnimatedIndependents object (support since v0.4)
, NBF_QUADSTRIPS                              //!< Identifies an NBFStrippedPrimitiveSet object (support since v0.4)
, NBF_LINES                                   //!< Identifies an NBFIndependentPrimitiveSet object (support since v0.5)
, NBF_LINESTRIPS                              //!< Identifies an NBFStrippedPrimitiveSet object (support since v0.5)
, NBF_TRIFANS                                 //!< Identifies an NBFTriFans object (support since v0.6)
, NBF_POINTS                                  //!< Identifies an NBFPoints object (support since v0.6)
, NBF_QUADMESHES                              //!< Identifies an NBFQuadMeshes object
  // node objects (0x400 - 0x4FF)
, NBF_NODE                      = 0x00000400
, NBF_GEO_NODE                                //!< Identifies a NBFGeoNode object.
, NBF_GROUP                                   //!< Identifies a NBFGroup object.
, NBF_LOD                                     //!< Identifies a NBFLOD object.
, NBF_SWITCH                                  //!< Identifies a NBFSwitch object.
, NBF_TRANSFORM                               //!< Identifies a NBFTransform object.
, NBF_ANIMATED_TRANSFORM                      //!< Identifies a NBFAnimatedTransform object.
, NBF_LIGHT_SOURCE
, NBF_DIRECTED_LIGHT                          //!< Identifies an NBFDirectedLight object.
, NBF_POINT_LIGHT                             //!< Identifies an NBFPointLight object.
, NBF_SPOT_LIGHT                              //!< Identifies an NBFSpotLight object.
, NBF_BILLBOARD                               //!< Identifies an NBFBillboard object.
, NBF_VOLUME_NODE                             //!< Identifies an NBFVolumeNode object.
  // state set objects (0x500 - 0x5FF)
, NBF_STATE_SET                 = 0x00000500  //!< Identifies an NBFStateSet object.
, NBF_STATE_VARIANT                           //!< Identifies an NBFStateVariant object.
, NBF_STATE_PASS                              //!< Identifies an NBFStatePass object.
  // state attribute objects (0x600 - 0x6FF)
, NBF_STATE_ATTRIBUTE           = 0x00000600
, NBF_CGFX                                    //!< Identifies an NBFCgFx object.
, NBF_MATERIAL                                //!< Identifies an NBFMaterial object.
, NBF_FACE_ATTRIBUTE                          //!< Identifies an NBFFaceAttribute object.
, NBF_TEXTURE_ATTRIBUTE                       //!< Identifies an NBFTextureAttribute object.
, NBF_TEXTURE_ATTRIBUTE_ITEM                  //!< Identifies an NBFTextureAttributeItem object.
, NBF_LINE_ATTRIBUTE                          //!< Identifies an NBFLineAttribute object.
, NBF_POINT_ATTRIBUTE                         //!< Identifies an NBFPointAttribute object.
, NBF_BLEND_ATTRIBUTE                         //!< Identifies an NBFBlendAttribute object.
, NBF_DEPTH_ATTRIBUTE                         //!< Identifies an NBFDepthAttribute object.
  // custom objects (>=0x700)
, NBF_CUSTOM_OBJECT                           //!< Custom objects must not have codes lower than this.
};

//! The NBFHeader structure represents the NBF header format.
/** The NBFHeader structure is the primary location where NBF specifics are stored.\n
  * For a valid NBF file, the NBFHeader structure is stored at file offset 0. Note that,
  * except for the NBFHeader object, a file offset of 0 indicates an invalid file offset!\n
  * This structure mainly serves as validation and compatibility checks for verification 
  * purposes. It also maintains the file offset to the contained NBFScene object, which  
  * represents a scene in the context of computer graphics. */ 
struct NBFHeader
{
  // signature
  byte_t      signature[4];       //!< A 4-byte signature identifying the file as a valid NBF file. The bytes are "#NBF".
  // NBF version
  ubyte_t     nbfMajorVersion;    //!< Specifies the major part of the NBF version used to save the file.
  ubyte_t     nbfMinorVersion;    //!< Specifies the minor part (compatibility level) of the NBF version used to save the file.
  ubyte_t     nbfBugfixLevel;     //!< Specifies the bugfix level of the NBF version used to save the file. This is optional
                                  //!< information, as a bugfix level does not influence compatibility issues, and hence 
                                  //!< must not be taken into account for compatibility checks.
  // NVSG version
  ubyte_t     nvsgMajorVersion;   //!< Specifies the major part of the NVSG version the content of this file is compatible to.
  ubyte_t     nvsgMinorVersion;   //!< Specifies the minor part of the NVSG version the content of this file is compatible to.
  ubyte_t     nvsgBugfixLevel;    //!< Specifies the bugfix level of the NVSG version. This is optional information, as a 
                                  //!< bugfix level does not influence compatibility issues, and hence must not be taken 
                                  //!< into account for compatibility checks.
  // Reserved bytes 
  ubyte_t     reserved[16];       //!< Reserved bytes for future extensions.
  // Date
  ubyte_t     dayLastModified;    //!< Specifies the day (1-31) of last modification.
  ubyte_t     monthLastModified;  //!< Specifies the month (1-12) of last modification.
  ubyte_t     yearLastModified[2]; //!< Specifies the year of last modification.
  // Time stamp
  ubyte_t     secondLastModified; //!< Specifies the second (0-59) of last modification. 
  ubyte_t     minuteLastModified; //!< Specifies the minute (0-59) of last modification.
  ubyte_t     hourLastModified;   //!< Specifies the hour (0-23) of last modification.
  // endianess
  ubyte_t     byteOrder;          //!< Specifies the byte order used to save the contained data.
                                  //!< A value of 0 implies little-endian byte order, a value of 1 implies big-endian byte order.
                                  //!< It is more convenient to use the symbolic constants NBF_LITTLE_ENDIAN and NBF_BIG_ENDIAN here.
  PADDING(2);                     //!< Padding bits to ensure offset of scene is on a 4-byte boundary, regardless of packing.
  // scene object
  uint_t      scene;              //!< Specifies the file offset to the contained NBFScene object.
  // optional view state
  uint_t      viewState;          //!< Specifies the file offset to an optional NBFViewState object. 
                                  //!< An offset of 0 indicates that no NBFViewState object is available in this file. 
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFHeader,4);

//! The NBFScene structure represents a scene in the context of computer graphics.
/** A valid NBF file always contains one - and only one - NBFScene object. 
  * The file offset to this NBFScene object is specified within the NBFHeader structure. */
struct NBFScene
{
  float3_t    ambientColor;             //!< Specifies the global ambient color to be used for rendering.
  float3_t    backColor;                //!< Specifies the scene's RGB background color used with rendering.
  uint_t      numCameras;               //!< Specifies the number of the scene's NBFCamera objects.
  uint_t      cameras;                  //!< Specifies the file offset to the offsets of the scene's NBFCamera objects.
  uint_t      numCameraAnimations;      //!< Specifies the number of the scene's NBFTrafoAnimation objects (camera animations). 
  uint_t      cameraAnimations;         //!< Specifies the offset to the offsets of the scene's NBFTrafoAnimation objects (camera animations).
  uint_t      numberOfAnimationFrames;  //!< For animated scenes, this specifies the number of animation frames.
  uint_t      root;                     //!< Specifies the file offset to the scene's root node, which always is of a NBFNode derived type.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFScene,4);

//! The NBFViewState represents an optional view state used to render the scene.
/** The file offset to an optional NBFViewState object is specified within the NBFHeader structure. */
struct NBFViewState
{
  uint_t      objectCode;           //!< Specifies the object code of the actual object. The object code is unique per object type! 
  uint_t      camera;               //!< Specifies the offset to the NBFCamera object to be used for viewing.
  ubyte_t     isJitter;             //!< Indicates whether the view is in jitter mode.
  ubyte_t     isStereo;             //!< Indicates whether the view is in stereo mode.
  ubyte_t     isStereoAutomatic;    //!< Indicates whether eye distance is automatically adjusted in stereo mode.
  PADDING(1);                       //!< Padding bits to ensure offset of jitters is on a 4-byte boundary, regardless of packing.
  uint_t      numJitters;           //!< Specifies the number of jitter values available.  
  uint_t      jitters;              //!< Specifies the offset to the float2_t jitter values used with jitter mode on. 
  float       stereoAutomaticFactor;//!< Specifies the automatic eye distance adjustment factor in stereo mode.
  float       stereoEyeDistance;    //!< Specifies the stereo eye distance used if the view is in stereo mode.
  float       targetDistance;       //!< Specifies the target distance to the projection plane.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFViewState,4);

//! The NBFObject structure represents general object data. 
struct NBFObject
{
  uint_t      objectCode;         //!< Specifies the object code of the actual object. The object code is unique per object type! 
  ubyte_t     isShared;           //!< Indicates whether the data of the actual object is shared among different objects.
                                  //!< A value of 1 indicates that this object's data is shared, whereas a value of 0 indicates
                                  //!< that this object's data is not shared.
  PADDING(3);                     //!< Padding bits to ensure offset of objectDataID is on a 8-byte boundary, regardless of packing.
  uint64_t    objectDataID;       //!< A unique 64-bit value to identify shared object data while loading.
  uint_t      sourceObject;       //!< Specifies the file offset to the source object in case of data sharing. 
                                  //!< A file offset of 0 always indicates that no source object is available for the actual object.
  str_t       objectName;         //!< Specifies an optional name for the actual object.
  // NOTE: Because of the uint64_t member objectDataID above, which is 8-byte aligned, 
  // we need to ensure the size of NBFObject is fixed - that is, independent of whatever
  // the compilers actual packing value might be! We achieve this by making the size of 
  // NBFObject a multiple of 8 bytes (see compile time assert below).  
  PADDING(4);                     //!< Padding bits to ensure the size of NBFObject is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFObject,8);

//! The NBFTrafoAnimation represents an animation that will be applied to trafo_t objects.
/** A NBFTrafoAnimation serves as base class only and needs to be considered in conjunction
  * with either NBFFramedAnimation or NBFInterpolatedAnimation.\n
  * Valid object codes for a NBFTrafoAnimation are NBF_FRAMED_TRAFO_ANIMATION, 
  * and NBF_LINEAR_INTERPOLATED_TRAFO_ANIMATION. Further concrete object codes valid for
  * NBFTrafoAnimation objects are subject to future extensions of the NBF format. */
struct NBFTrafoAnimation : public NBFObject
{
  uint_t      trafos;             //!< Specifies the file offset to the trafo_t objects the animation will be applied to.
                                  //!< The actual number of trafo_t objects is specified by the corresponding animation
                                  //!< type, which is either NBFFramedAnimation or NBFInterpolatedAnimation.
  PADDING(4);        //!< Padding bits to ensure the size of NBFTrafoAnimation is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFTrafoAnimation,8);

//! The NBFVNVectorAnimation represents an animation that will be applied to VNVector objects.
/** A NBFVNVectorAnimation serves as base class only and needs to be considered in conjunction 
  * with either NBFFramedAnimation or NBFInterpolatedAnimation.\n
  * Concrete object codes valid for a NBFVNVectorAnimation are NBF_FRAMED_VNVECTOR_ANIMATION, 
  * and NBF_LINEAR_INTERPOLATED_VNVECTOR_ANIMATION. Further concrete object codes valid for
  * NBFVNVectorAnimation objects are subject to future extensions of the NBF format. */
struct NBFVNVectorAnimation : public NBFObject
{
  uint_t      numVertices;        //!< Specifies the number of vertices per vertex set. Each vertex set of 
                                  //!< the actual animation is specified to have the same count of vertices.
  uint_t      vertexSets;         //!< Specifies the file offset to the pre-transformed vertex sets for this animation. 
                                  //!< The vertex sets are stored adjoined to each other in contiguous memory.\n
                                  //!< The actual number of vertex sets, as well as normal sets, is specified by
                                  //!< the corresponding animation type, which is either NBFFramedAnimation or
                                  //!< NBFInterpolatedAnimation.
  uint_t      numNormals;         //!< Specifies the number of normals per normal set. Each normal set of 
                                  //!< the actual animation is specified to have the same count of normals.
  uint_t      normalSets;         //!< Specifies the file offset to the pre-transformed normal sets for this animation.
                                  //!< The normal sets are stored adjoined to each other in contiguous memory.\n
                                  //!< The actual number of normal sets, as well as vertex sets, is specified by
                                  //!< the corresponding animation type, which is either NBFFramedAnimation or
                                  //!< NBFInterpolatedAnimation.
                                  
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFVNVectorAnimation,8);

//! The NBFFramedAnimation structure represents a framed animation.
/** A NBFFramedAnimation serves as base class only and needs to be considered in conjunction
  * with either NBFTrafoAnimation or NBFVNVectorAnimation.\n
  * Concrete object codes valid for a NBFFramedAnimation are NBF_FRAMED_TRAFO_ANIMATION and
  * NBF_FRAMED_VNVECTOR_ANIMATION. Further concrete object codes valid for NBFFramedAnimation 
  * objects are subject to future extensions of the NBF format. */
struct NBFFramedAnimation
{
  uint_t      numFrames;          //!< Specifies the number of animation frames.\n 
                                  //!< In conjunction with a NBFTrafoAnimation, this corresponds to the number of
                                  //!< trafo_t objects stored with a NBFTrafoAnimation.\n
                                  //!< In conjunction with a NBFVNVectorAnimation, this corresponds to the number of
                                  //!< vertex/normal sets stored with a NBFVNVectorAnimation.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFFramedAnimation,4);

//! The NBFFramedTrafoAnimation structure represents a framed animation that will be applied to trafo_t objects.
/** A NBFFramedTrafoAnimation is a concrete animation type. It publicly inherits from NBFTrafoAnimation and
  * NBFFramedAnimation. The object code for a NBFFramedTrafoAnimation is NBF_FRAMED_TRAFO_ANIMATION. */ 
struct NBFFramedTrafoAnimation : public NBFTrafoAnimation
                               , public NBFFramedAnimation
{
  PADDING(4);         //!< Padding bits to ensure the size of NBFFramedTrafoAnimation is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFFramedTrafoAnimation,8);

//! The NBFFramedVNVectorAnimation structure represents a framed animation that will be applied to VNVector objects.
/** A NBFFramedVNVectorAnimation is a concrete animation type. It publicly inherits from NBFVNVectorAnimation and
  * NBFFramedAnimation. The object code for a NBFFramedVNVectorAnimation is NBF_FRAMED_VNVECTOR_ANIMATION. */ 
struct NBFFramedVNVectorAnimation : public NBFVNVectorAnimation
                                  , public NBFFramedAnimation
{
  PADDING(4);         //!< Padding bits to ensure the size of NBFFramedVNVectorAnimation is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFFramedVNVectorAnimation,8);

//! The NBFInterpolatedAnimation structure represents a interpolated animation.
/** A NBFInterpolatedAnimation serves as base class only and needs to be considered in conjunction
  * with either NBFTrafoAnimation or NBFVNVectorAnimation.\n
  * Concrete object codes valid for a NBFInterpolatedAnimation are NBF_LINEAR_INTERPOLATED_TRAFO_ANIMATION and
  * NBF_LINEAR_INTERPOLATED_VNVECTOR_ANIMATION. Further concrete object codes valid for NBFInterpolatedAnimation 
  * objects are subject to future extensions of the NBF format. */
struct NBFInterpolatedAnimation
{
  uint_t      numKeys;            //!< Specifies the number of key frames
  uint_t      keys;               //!< Specifies the file offset to the key frames. As specified in the NBF format,
                                  //!< a key is a 32-bit unsigned integer value.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFInterpolatedAnimation,4);

//! The NBFInterpolatedTrafoAnimation structure represents a interpolated animation that will be applied to trafo_t objects.
/** A NBFInterpolatedTrafoAnimation is a concrete animation type. It publicly inherits from NBFTrafoAnimation and
  * NBFInterpolatedAnimation.\n 
  * The object code valid for a NBFInterpolatedTrafoAnimation is NBF_LINEAR_INTERPOLATED_TRAFO_ANIMATION. Further object codes
  * valid for NBFInterpolatedTrafoAnimation objects are subject to future extensions of the NBF format. */
struct NBFInterpolatedTrafoAnimation : public NBFTrafoAnimation
                                     , public NBFInterpolatedAnimation
{
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFInterpolatedTrafoAnimation,8);

//! The NBFInterpolatedVNVectorAnimation structure represents a interpolated animation that will be applied to VNVector objects.
/** A NBFInterpolatedVNVectorAnimation is a concrete animation type. It publicly inherits from NBFVNVectorAnimation and
  * NBFInterpolatedAnimation.\n 
  * The object code valid for a NBFInterpolatedVNVectorAnimation is NBF_LINEAR_INTERPOLATED_VNVECTOR_ANIMATION. Further object codes
  * valid for NBFInterpolatedVNVectorAnimation objects are subject to future extensions of the NBF format. */
struct NBFInterpolatedVNVectorAnimation : public NBFVNVectorAnimation
                                        , public NBFInterpolatedAnimation
{
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFInterpolatedVNVectorAnimation,8);

//! The NBFCamera represents a camera.
/** A NBFCamera serves as base class only.\n
  * Concrete object codes valid for a NBFCamera are NBF_PARALLEL_CAMERA
  * and NBF_PERSPECTIVE_CAMERA. Further object codes valid for a NBFCamera
  * are subject to future extensions for the NBF format. */
struct NBFCamera : public NBFObject
{
  uint_t      numHeadLights;    //!< Specifies the number of headlights attached.
  uint_t      headLights;       //!< Specifies the file offset to the offsets to the attached headlight objects.
                                //!< Headlights are of type NBFLightSource. 
  float3_t    upVector;         //!< Specifies the camera's normalized up vector.
  float3_t    position;         //!< Specifies the actual position of camera in world space.
  float3_t    direction;        //!< Specifies the normalized direction for the camera to look along.
  float       farDist;          //!< Specifies the distance from the actual camera position to the far clipping plane.
  float       nearDist;         //!< Specifies the distance from the actual camera position to the near clipping plane.
  float       focusDist;        //!< Specifies the distance to the projection plane.
  ubyte_t     isAutoClipPlanes; //!< Indicates if automatic generation of clipping planes is enabled.
  PADDING(3);      //!< Padding bits to ensure offset of windowSize is on a 4-byte boundary, regardless of packing
  float2_t    windowSize;       //!< Specifies the world-relative size of the viewing window. Whereas the x-component of 
                                //!< of the vector specifies the width, and the y-component of the vector specifies the height.
  float2_t    windowOffset;     //!< Specifies the world-relative offset from the viewing reference point to the center 
                                //!< of the viewing window.
  PADDING(4);        //!< Padding bits to ensure the size of NBFTrafoAnimation is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFCamera,8);

//! The NBFParallelCamera represents a parallel camera.
/** A NBFParallelCamera is a concrete camera type. 
  * The object code for a NBFParallelCamera is NBF_PARALLEL_CAMERA. */
struct NBFParallelCamera : public NBFCamera
{
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFParallelCamera,8);

//! The NBFPerspectiveCamera represents a perspective camera.
/** A NBFPerspectiveCamera is a concrete camera type. 
  * The object code for a NBFPerspectiveCamera is NBF_PERSPECTIVE_CAMERA. */
struct NBFPerspectiveCamera : public NBFCamera
{
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFPerspectiveCamera,8);

//! The NBFDrawable represents any kind of a drawable object.
/** A NBFDrawable serves a base class only.\n
  * Concrete object codes valid for a NBFDrawable are NBF_TRIANGLES, NBF_ANIMATED_TRIANGLES, 
  * NBF_SKINNED_TRIANGLES, NBF_TRIFANS, and NBF_TRISTRIPS. Further object codes valid for a NBFDrawable
  * are subject to future extensions of the NBF format. */
struct NBFDrawable : public NBFObject
{
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFDrawable,8);

//! The NBFVertexAttributeSet structure represents a set of vertex attributes.
/** A NBFVertexAttributeSet maintains a full set of geometry
  * data (vertex attributes) that is used by all topology classes. */
struct NBFVertexAttributeSet : public NBFObject
{
  uint_t      numVertices;        //!< Specifies the number of contained vertices. 
  uint_t      vertices;           //!< Specifies the file offset to the vertices. Vertices are stored as float3_t.
  uint_t      numNormals;         //!< Specifies the number of contained normals.
  uint_t      normals;            //!< Specifies the file offset to the normals. Normals are stored as float3_t.
  uint_t      numTexCoordsSets;   //!< Specifies the number of contained texture coordinate sets.
  uint_t      texCoordsSets;      //!< Specifies the file offset to the texture coordinate sets. 
                                  //!< Texture coordinate sets are stored using the texCoordSet_t type.
  uint_t      numColors;          //!< Specifies the number of contained primary colors.
  uint_t      colorDim;           //!< Specifies the dimension, in terms of float, used for the primary colors.
                                  //!< Colors can be either three or four dimensional. 
  uint_t      colors;             //!< Specifies the file offset to the primary colors. In conformity to the color's dimension,
                                  //!< colors are stored as float3_t or float4_t respectively.
  uint_t      numSecondaryColors; //!< Specifies the number of contained secondary colors.
  uint_t      secondaryColorDim;  //!< Specifies the dimension, in terms of float, used for the secondary colors.
                                  //!< Colors can be either three or four dimensional. 
  uint_t      secondaryColors;    //!< Specifies the file offset to the secondary colors. In conformity to the color's dimension,
                                  //!< colors are stored as float3_t or float4_t respectively.
  uint_t      numFogCoords;       //!< Specifies the number of contained fog coordinates.
  uint_t      fogCoords;          //!< Specifies the file offset to the fog coordinates. Fog coordinates always are one 
                                  //!< dimensional, and are stored as 32-bit floating point values.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFVertexAttributeSet,8);

//! The NBFPrimitiveSet structure represents a geometry with an NBFVertexAttributeSet.
/** A NBFPrimitiveSet holds the offset to an NBFVertexAttributeSet. */
struct NBFPrimitiveSet : public NBFDrawable
{
  uint_t      vertexAttributeSet; //!< Specifies the file offset to the vertex attribute set.
  PADDING(4);                     //!< Padding bits to ensure the size of NBFAnimatedIndependents is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFPrimitiveSet,8);

//! The NBFIndependentPrimitiveSet structure represents a set of independent primitives.
/** A NBFIndependentPrimitiveSet is an abstract topology class derived from NBFPrimitiveSet.
  * It is used with NBF_LINES, NBF_POINTS, NBF_QUADS, and NBF_TRIANGLES. */
struct NBFIndependentPrimitiveSet : public NBFPrimitiveSet
{
  uint_t      numIndices;         //!< Specifies the number of contained indices
  uint_t      indices;            //!< Specifies the file offset to the Independent objects.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFIndependentPrimitiveSet,8);

//! The NBFMeshedPrimitiveSet structure represents a mesh set.
/** A NBFMeshedPrimitiveSet is an abstract topology class derived from NBFPrimitiveSet. 
* Valid object codes for a NBFMeshedPrimitiveSet are NBF_QUADMESHES. */
struct NBFMeshedPrimitiveSet : public NBFPrimitiveSet
{
  uint_t      numMeshes;          //!< Specifies the number of meshes
  uint_t      meshes;             //!< Specifies the file offset to the meshes. 
  //!< Strips are stored as indexSet_t objects.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFMeshedPrimitiveSet,8);   //!< test size of struct

//! The NBFStrippedPrimitiveSet structure represents a strip set.
/** A NBFStrippedPrimitiveSet is an abstract topology class derived from NBFPrimitiveSet. 
  * Valid object codes for a NBFStrippedPrimitiveSet are NBF_TRIFANS, NBF_TRISTRIPS, and NBF_QUADSTRIPS. */
struct NBFStrippedPrimitiveSet : public NBFPrimitiveSet
{
  uint_t      numStrips;          //!< Specifies the number of strips
  uint_t      strips;             //!< Specifies the file offset to the strips. 
                                  //!< Strips are stored as indexSet_t objects.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFStrippedPrimitiveSet,8);

//! The NBFAnimatedIndependents structure represents animated independent primitives.
/** A NBFAnimatedIndependents is an abstract topology class derived from NBFIndependentPrimitiveSet. 
  * The object code for a NBFAnimatedIndependents is NBF_ANIMATED_QUADS and NBF_ANIMATED_TRIANGLES. */
struct NBFAnimatedIndependents : public NBFIndependentPrimitiveSet
{
  uint_t      animation;          //!< Specifies the file offset to the NBFVNVectorAnimation object 
                                  //!< to be applied to the independent faces.
  PADDING(4);        //!< Padding bits to ensure the size of NBFAnimatedIndependents is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFAnimatedIndependents,8);

//! The NBFSkinnedTriangles structure represents skin animated triangles.
/** A NBFSkinnedTriangles is a concrete topology class derived from NBFIndependentPrimitiveSet. 
* The object code for a NBFSkinnedTriangles is NBF_SKINNED_TRIANGLES. */
struct NBFSkinnedTriangles : public NBFIndependentPrimitiveSet
{
  uint_t      numSkins;           //!< Specifies the number of contained skins
  uint_t      skins;              //!< Specifies the file offset to the corresponding skin_t objects.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFSkinnedTriangles,8);

//! The NBFNode structure represents a general node.
/** A NBFNode serves as base class only. Concrete object codes valid for a NBFNode are
  * NBF_GEO_NODE, NBF_LOD, NBF_SWITCH, NBF_TRANSFORM, NBF_ANIMATED_TRANSFORM,
  * NBF_DIRECTED_LIGHT, NBF_POINT_LIGHT, NBF_SPOT_LIGHT, and NBF_VOLUME_NODE. Further concrete
  * object codes valid for a NBFNode are subject to future extensions of the NBF format. */
struct NBFNode : public NBFObject
{
  str_t       annotation;         //<! Specifies an optional annotation string.
  PADDING(6);                     //!< Padding bits to ensure the size of NBFStateAttribute is a multiple of 4, regardless of packing.
  ubyte_t     systemHints;        //!< Specifies the system hints on this Node.
  ubyte_t     userHints;          //!< Specifies the user hints on this Node.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFNode,8);

//! The NBFGeoNode structure represents a geometry node.
/** The object code for a NBFGeoNode is NBF_GEO_NODE. */
struct NBFGeoNode : public NBFNode
{
  uint_t      numGeometrySets;    //<! Specifies the number of contained geometry sets.
  uint_t      geometrySets;       //<! Specifies the file offset to the geometry sets.
                                  //<! Geometry sets are stored as geometrySet_t objects.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFGeoNode,8);

//! The NBFVolumeNode structure represents a volume node.
/** The object code for a NBFVolumeNode is NBF_VOLUME_NODE. */
struct NBFVolumeNode : public NBFNode
{
  str_t       bricksName;         //!< Specifies the name of the top level brick.
  uint_t      levelToDisplay;     //!< Specifies the level to display.
  uint_t      shader;             //!< Specifies the file offset to the shader.
  str_t       volumeName;         //!< Specifies the name of the volume dataset.
  float       voxelLimitFraction; //!< Specifies the voxel limit fraction
};

//! The NBFGroup structure represents a group node.
/** A NBFGroup serves as base class only. Concrete object codes valid for a NBFGroup are
  * NBF_LOD, NBF_SWITCH, NBF_TRANSFORM, and NBF_ANIMATED_TRANSFORM. Further concrete object
  * codes valid for a NBFGroup are subject to future extensions of the NBF format. */
struct NBFGroup : public NBFNode
{
  uint_t      numChildren;        //!< Specifies the number of maintained children.
  uint_t      children;           //!< Specifies the file offset to the offsets to the maintained children.
                                  //!< NBFGroup's children always are of NBFNode-derived types.
  uint_t      numClipPlanes;      //!< Specifies the number of clipping planes.
  uint_t      clipPlanes;         //!< Specifies the file offset to the clipping planes
  uint_t      numLightSource;     //!< Specifies the number of light sources.
  uint_t      lightSources;       //!< Specifies the fie offset to the offsets to the light sources.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFGroup,8);

//! The NBFLOD structure represents a 'Level Of Detail' group node.
/** The object code for a NBFLOD is NBF_LOD. */
struct NBFLOD : public NBFGroup
{
  float3_t    center;             //!< Specifies the center point used for distance calculations.
  uint_t      numRanges;          //!< Specifies the number of contained ranges.
  uint_t      ranges;             //!< Specifies the file offset to the ranges. 
                                  //!< Ranges are stored as 32-bit floating point numbers.
  PADDING(4);        //!< Padding bits to ensure the size of NBFLOD is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFLOD,8);

//! The NBFSwitch structure represents a switch group node.
/** The object code for a NBFSwitch is NBF_SWITCH. */
struct NBFSwitch : public NBFGroup
{
  uint_t      numActiveChildren;  //!< Specifies the number of children that are currently switched on.
  uint_t      activeChildren;     //!< Specifies the offset to indices to the active children.
                                  //!< Indices are stored as 32-bit unsigned integers.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFSwitch,8);

//! The NBFBillboard structure represents a billboard group node.
/** The object code for a NBFBillboard is NBF_BILLBOARD. */
struct NBFBillboard: public NBFGroup
{
  float3_t  rotationAxis;
  ubyte_t   viewerAligned;
  PADDING(3);        //!< Padding bits to ensure the size of NBFBillboard is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFBillboard,8);

//! The NBFTransform structure represents a transform group node.
/** The object code for a NBFTransform is NBF_TRANSFORM. */
struct NBFTransform : public NBFGroup
{
  trafo_t             trafo;
  PADDING(4);        //!< Padding bits to ensure the size of NBFTransform is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFTransform,8);

//! The NBFAnimatedTransform structure represents an animated transform group node.
/** The object code for a NBFAnimatedTransform is NBF_ANIMATED_TRANSFORM. */
struct NBFAnimatedTransform : public NBFTransform
{
  uint_t      animation;          //!< Specifies the file offset to the NBFTrafoAnimation object
                                  //!< to be applied to the transform group node.
  PADDING(4);        //!< Padding bits to ensure the size of NBFAnimatedTransform is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFAnimatedTransform,8);

//! The NBFLightSource structure represents a light source node.
/** A NBFLightSource serves as base class only. Concrete object codes valid for 
  * a NBFLightSource are NBF_DIRECTED_LIGHT, NBF_POINT_LIGHT, and NBF_SPOT_LIGHT. */
struct NBFLightSource : public NBFObject
{
  float       intensity;          //!< Specifies the light intensity.
  float3_t    ambientColor;       //!< Specifies the ambient color term of the light.
  float3_t    diffuseColor;       //!< Specifies the diffuse color term of the light.
  float3_t    specularColor;      //!< Specifies the specular color term of the light.
  ubyte_t     castShadow;         //!< flag that determines if this light source creates shadows.
  PADDING(3);        //!< Padding bits to ensure the offset of the next struct member is on a 4-byte boundary.
  uint_t      animation;          //!< Specifies the file offset to an optional NBFTrafoAnimation object
                                  //!< to be applied to the light transform (orientation and translation).
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFLightSource,8);

// The NBFDirectedLight structure represents a directed light source.
/** The object code for a NBFDirectedLight is NBF_DIRECTED_LIGHT. */
struct NBFDirectedLight : public NBFLightSource
{
  float3_t direction; //!< Specifies the direction of the light source.
  PADDING(4);
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFDirectedLight,8);

// The NBFPointLight structure represents a point light source.
/** The object code for a NBFPointLight is NBF_POINT_LIGHT. */
struct NBFPointLight : public NBFLightSource
{
  float3_t position;    //!< Specifies the position of the light source.
  float3_t attenuation; //!< Specifies the attenuation factors of the point light.
                        //!< The x-component of the vector specifies the constant term of the attenuation,
                        //!< the y-component of the vector specifies the linear term of the attenuation, and
                        //!< the z-component of the vector specifies the quadratic term of the attenuation.
};                        
NVSG_CTASSERT_MODULO_BYTESIZE(NBFPointLight,8);

// The NBFSpotLight structure represents a spot light.
/** The object code for a NBFSpotLight is NBF_SPOT_LIGHT. */
struct NBFSpotLight : public NBFLightSource
{
  float3_t position;        //!< Specifies the position of the light source.
  float3_t direction;       //!< Specifies the direction of the light source.
  float3_t attenuation;     //!< Specifies the attenuation factors of the spot light.
                            //!< The x-component of the vector specifies the constant term of the attenuation,
                            //!< the y-component of the vector specifies the linear term of the attenuation, and
                            //!< the z-component of the vector specifies the quadratic term of the attenuation.
  float    cutoffAngle;     //!< Specifies the angle between the axis of the cone, the light is emitted to, and
                            //!< a ray along the edge of the cone.
  float    falloffExponent; //!< Controls the intensity distribution inside the cone, the light is mitted to.
  PADDING(4);               //!< Padding bits to ensure the size of NBFSpotLight is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFSpotLight,8);

//! The NBFStateSet structure represents a set of pairs of VariantKeys and StateVariants.
/** The object code for a NBFStateSet is NBF_STATE_SET. */
struct NBFStateSet : public NBFObject
{
  uint_t activeKey;               //!< Specifies the currently active VariantKey
  uint_t numStateVariants;        //!< Specifies the number of contained pairs of VariantKey and StateVariant.
  uint_t keyStateVariantPairs;    //!< Specifies the file offset to the offsets to the keyVariant_t objects
  PADDING(4);               //!< Padding bits to ensure the size of NBFStateSet is a multiple of 8, regardless of packing.    
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFStateSet,8);

//! The NBFStateVariant structure represents a set of StatePasses
/** The object code for a NBFStateVariant is NBF_STATE_VARIANT. */
struct NBFStateVariant : public NBFObject
{
  uint_t      numStatePasses;     //!< Specifies the number of contained state passes.
  uint_t      statePasses;        //!< Specifies the file offset to the offsets to the NBFStatePass objects
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFStateVariant,8);

//! The NBFStatePass structure represents a set of heterogeneous NBFStateAttributes.
/** The object code for a NBFStatePass is NBF_STATE_PASS. */
struct NBFStatePass : public NBFObject
{
  uint_t      numStateAttribs;    //!< Specifies the number of contained state attributes.
  uint_t      stateAttribs;       //!< Specifies the file offset to the offsets to the NBFStateAttribute objects
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFStatePass,8);

//! The NBFStateAttribute structure represents a state attribute.
/** A NBFStateAttribute serves a base class only. Concrete object codes valid for a NBFStateAttribute
  * are NBF_BLEND_ATTRIBUTE, NBF_CGFX, NBF_MATERIAL, NBF_FACE_ATTRIBUTE, and NBF_TEXTURE_ATTRIBUTE. */
struct NBFStateAttribute : public NBFObject
{
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFStateAttribute,8);

//! The NBFBlendAttribute structure represents a blend function.
/** The object code for a NBFBlendAttribute is NBF_BLEND_ATTRIBUTE. */
struct NBFBlendAttribute : public NBFStateAttribute
{
  ubyte_t     sourceFunction;       //!< Specifies the source blending function
  ubyte_t     destinationFunction;  //!< Specifies the destination blending function
  PADDING(6);                       //!< Padding bits to ensure size of NBFBlendAttribute is a multiple of 8
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFBlendAttribute,8);

//! The NBFCgFx structure represents an effect attribute.
/** The object code for a NBFCgFx is NBF_CGFX. */
struct NBFCgFx : public NBFStateAttribute
{
  PADDING(3);
  ubyte_t     fromFile;           /**!< Specifies whether the effect's source is a file. 
                                       If this is false, the source member below is interpreted as a code string.*/
  str_t       source;             //!< Holds either the filename of an effect file or a source code string. 
  uint_t      numTweaks;          //!< Specifies the number of tweakable effect parameters stored.
  uint_t      tweaks;             /**!< Specifies the file offset to the tweakable effect paramters. 
                                        Tweakables in the NBF format are stored as tweak_t objects. */
  uint_t      numSamplers;        //!< Specifies the number of sampler parameters stored. 
  uint_t      samplers;           /**!< Specifies the file offset t the sampler parameters.
                                       Samplers parameters a stored as sampler_t objects. */
  str_t       currTech;           //!< Specifies the name of the current technique used with rendering.
  PADDING(4); //!< Padding bits to ensure the size of NBFCgFx is a multiple of 8, regardless of packing.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFCgFx,8);

//! The NBFDepthAttribute structure represents depth buffer settings.
/** The object code for a NBFDepthAttribute is NBF_DEPTH_ATTRIBUTE. */
struct NBFDepthAttribute : public NBFStateAttribute
{
  ubyte_t     depthFunction;        //!< Specifies the depth function
  ubyte_t     depthTestingEnabled;  //!< Specifies if depth testing is enabled
  ubyte_t     depthWritingEnabled;  //!< Specifies if depth writing is enabled
  PADDING(5);                       //!< Padding bits to ensure size of NBFBlendAttribute is a multiple of 8
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFDepthAttribute,8);

//! The NBFFaceAttribute structure represents a render mode.
/** The object code for a NBFFaceAttribute is NBF_FACE_ATTRIBUTE. */
struct NBFFaceAttribute : public NBFStateAttribute
{
  ubyte_t     cullMode;             //!< Specifies the face culling mode.
  ubyte_t     polygonOffsetEnabled; //!< Specifies if polygon offset should be enabled for rendering.
  PADDING(2);                       //!< Padding bits to ensure offset of polygonOffsetFactor is on a 4-byte boundary, regardless of packing
  float       polygonOffsetFactor;  //!< Specifies the scale factor to be used to create a variable depth offset
                                    //!< for a polygon.
  float       polygonOffsetUnits;   //!< Specifies a unit that is multiplied by a render API implementation-specific
                                    //!< value to create a constant depth offset.
  ubyte_t     twoSidedLighting;     //!< Specifies if two-sided lighting should be enabled for rendering.
  ubyte_t     frontFaceMode;        //!< Specifies the face mode (points/lines/faces) for front faces.
  ubyte_t     backFaceMode;         //!< Specifies the face mode (points/lines/faces) for back faces.
  PADDING(1);                       //!< Padding bits to ensure the size of NBFFaceAttribute is a multiple of 8, regardless of packing.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFFaceAttribute,8);

//! The NBFLineAttribute structure represents a render mode.
/** The object code for a NBFLineAttribute is NBF_LINE_ATTRIBUTE. */
struct NBFLineAttribute : public NBFStateAttribute
{
  ubyte_t     antiAliasing;         //!< Specifies if the lines are anti aliased.
  PADDING(3);                       //!< Padding bits ensure offset of next elements is on a 4-byte boundary, regardless of packing
  uint_t      stippleFactor;        //!< Specifies the stipple factor
  uint_t      stipplePattern;       //!< Specifies the stipple pattern
  float       width;                //!< Specifies the line width
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFLineAttribute,8);

//! The NBFMaterial structure represents a material.
/** The object code for a NBFMaterial is NBF_MATERIAL. */
struct NBFMaterial : public NBFStateAttribute
{
  float3_t    frontAmbientColor;      //!< Specifies the ambient part of the front material color. 
  float3_t    frontDiffuseColor;      //!< Specifies the diffuse part of the front material color.
  float3_t    frontEmissiveColor;     //!< Specifies the emissive part of the front material color.
  float       frontOpacity;           //!< Specifies the opacity of the front material.
  float3_t    frontSpecularColor;     //!< Specifies the specular part of the front material color.
  float       frontSpecularExponent;  //!< Specifies the specular exponent of the front material color.
  float3_t    backAmbientColor;       //!< Specifies the ambient part of the back material color. 
  float3_t    backDiffuseColor;       //!< Specifies the diffuse part of the back material color.
  float3_t    backEmissiveColor;      //!< Specifies the emissive part of the back material color.
  float       backOpacity;            //!< Specifies the opacity of the back material.
  float3_t    backSpecularColor;      //!< Specifies the specular part of the back material color.
  float       backSpecularExponent;   //!< Specifies the specular exponent of the back material color.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFMaterial,8);

//! The NBFPointAttribute structure represents a render mode.
/** The object code for a NBFPointAttribute is NBF_POINT_ATTRIBUTE. */
struct NBFPointAttribute : public NBFStateAttribute
{
  ubyte_t     antiAliasing;         //!< Specifies if the points are anti aliased.
  PADDING(3);                       //!< Padding bits ensure offset of next elements is on a 4-byte boundary, regardless of packing
  float       size;                 //!< Specifies the point size
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFPointAttribute,8);

//! The NBFTextureAttribute structure represents a texture attribute.
/** The object code for a NBFTextureAttribute is NBF_TEXTURE_ATTRIBUTE. */
struct NBFTextureAttribute : public NBFStateAttribute
{
  uint_t      numBindings;        //!< Specifies the number of contained texture bindings.
  uint_t      bindings;           //!< Specifies the file offset to the texture bindings. Texture bindings in
                                  //!< the NBF format are stored as texBinding_t objects.
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFTextureAttribute,8);

//! The NBFTextureAttributeItem structure represents a single texture object.
/** The object code for a NBFTextureAttributeItem is NBF_TEXTURE_ATTRIBUTE_ITEM. 
  * A NBFTextureAttributeItem needs to be consider in conjunction with a NBFTextureAttribute, 
  * which specifies the binding of texture objects. */
struct NBFTextureAttributeItem : public NBFObject
{
  uint_t        texImg;             //!< Specifies the file offset to the texture image object
  float4_t      texEnvColor;        //!< Specifies the texture environment color.
  uint_t        texEnvMode;         //!< Specifies the texture environment mode for the actual texture object. 
                                    //!< Valid modes are TEM_REPLACE, TEM_MODULATE, TEM_DECAL, TEM_BLEND, and TEM_ADD.
  uint_t        texEnvScale;        //!< Specifies the texture environment scale used with rasterization 
  uint_t        texWrapS;           //!< Specifies the wrap parameter for texture coordinate s. 
                                    //!< Valid values are TWM_REPEAT, TWM_CLAMP, and TWM_MIRROR
  uint_t        texWrapT;           //!< Specifies the wrap parameter for texture coordinate t.
                                    //!< Valid values are TWM_REPEAT, TWM_CLAMP, and TWM_MIRROR
  uint_t        texWrapR;           //!< Specifies the wrap parameter for texture coordinate r.
                                    //!< Valid values are TWM_REPEAT, TWM_CLAMP, and TWM_MIRROR
  uint_t        minFilter;          //!< Specifies the filter used with minimizing. 
                                    //!< Valid values are TFM_MIN_NEAREST, TFM_MIN_BILINEAR, and TFM_MIN_TRILINEAR.
  uint_t        magFilter;          //!< Specifies the filter used with magnifying.
                                    //!< Valid values are TFM_MAX_NEAREST, and TFM_MAZ_BILINEAR.
  float4_t      texBorderColor;     //!< Specifies the texture border RGBA color.
  trafo_t       trafo;              //!< Specifies the texture transformation
  uint_t        texGenMode[4];      //!< Specifies the texture coordinate generation modes
  float4_t      texGenPlane[2][4];  //!< Specifies the texture coordinate generation planes
  PADDING(4);   //!< Padding bits to ensure the size of NBFTextureAttributeItem is a multiple of 8, regardless of packing.        
};
NVSG_CTASSERT_MODULO_BYTESIZE(NBFTextureAttributeItem,8);

#pragma pack(pop)

#undef __NVSG_CTASSERT_PREFIX
