// Copyright (C) 2012 Asst.Prof. Dr. Arsa Tangchitsomkit
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

// PSK Mesh loader
// File format designed by Unreal Tourament (c) EpicGames

#define _IRR_COMPILE_WITH_UNREAL_LOADER_

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_UNREAL_LOADER_

#include "CUnrealMeshFileLoader.h"

#include "IVideoDriver.h"
#include "IFileSystem.h"

namespace irr
{
namespace scene
{

// A bone: an orientation, and a position, all relative to their parent.
struct JointPos
{
	core::quaternion orientation;
	core::vector3df  position;     //  
	
	f32      length;       //  For collision testing / debugging drawing.  (unused)
	f32      xSize;
	f32      ySize;
	f32      zSize;
};


// Binary animation info format - used to organize raw animation keys into FAnimSeqs on rebuild
// Similar to MotionChunkDigestInfo..
struct AnimInfoBinary
{
	char  name[64];     // Animation's name
	char  group[64];    // Animation's group name	
	
	int   totalBones;           // TotalBones * NumRawFrames is number of animation keys to digest.
	
	int   rootInclude;          // 0 none 1 included 	(unused)
	int   keyCompressionStyle;  // Reserved: variants in tradeoffs for compression.
	int   keyQuotum;            // Max key quotum for compression	
	f32	  keyReduction;			// desired 
	f32   trackTime;			// explicit - can be overridden by the animation rate
	f32   animRate;				// frames per second.
	int   startBone;            // - Reserved: for partial animations (unused)
	int   firstRawFrame;        //
	int   numRawFrames;         // NumRawFrames and AnimRate dictate tracktime...
};


// File header structure. 
struct ChunkHeader
{
	char chunkID[20];	// String ID of up to 19 chars (usually zero-terminated)
	int  typeFlag;      // Flags/reserved
	int  dataSize;      // Size per struct following;
	int  dataCount;     // Number of structs/
};

// Raw data material.
struct Material
{
	char materialName[64];
	int  textureIndex;  // Texture index ('multiskin index')
	u32  polyFlags;     // ALL poly's with THIS material will have this flag.
	int  auxMaterial;   // Reserved: index into another material, eg. detailtexture/shininess/whatever.
	u32  auxFlags;      // Reserved: auxiliary flags 
	int  lodBias;       // Material-specific lod bias (unused)
	int  lodStyle;      // Material-specific lod style (unused)
};


// Raw data bone.
struct Bone
{
	char     name[64];     //
	u32      flags;        // Reserved.
	int      numChildren;  // Children  (not used.)
	int      parentIndex;  // 0/NULL if this is the root bone.  
	JointPos bonePos;      // Reference position.
};


// Raw data bone influence.
struct RawBoneInfluence // Just weight, vertex, and Bone, sorted later.
{
	f32	  weight;
	int   pointIndex;
	int   boneIndex;
};

// An animation key.
struct QuatAnimKey
{
	core::vector3df    position;     // Relative to parent.
	core::quaternion   orientation;  // Relative to parent.
	f32      time;         // The duration until the next key (end key wraps to first...)
};

// Vertex with texturing info, akin to Hoppe's 'Wedge' concept - import only.
struct Vertex
{
	u16 pointIndex;	  // Index into the 3d point table.
	core::vector2df uv; // Texture U, V coordinates.
	u8  matIndex;     // At runtime, this one will be implied by the face that's pointing to us.
	u8  reserved;     // Top secret.
};

// Points: regular FVectors 
struct Point
{	
	core::vector3df  point; 
};

// Textured triangle.
struct Triangle
{
	u16  wedgeIndex[3];	  // Point to three vertices in the vertex list.
	u8   matIndex;	      // Materials can be anything.
	u8   auxMatIndex;     // Second material (unused).
	u32  smoothingGroups; // 32-bit flag for smoothing groups.
};

struct psk_t
{
	ChunkHeader			pskHeader;
	ChunkHeader			pointHeader;
	ChunkHeader			vertexHeader;
	ChunkHeader			faceHeader;
	ChunkHeader			materialHeader;
	ChunkHeader			boneHeader;
	ChunkHeader			influenceHeader;

	core::array<Point>		points;
	core::array<Vertex>		vertexs;
	core::array<Triangle>	faces;
	core::array<Material>	materials;
	core::array<Bone>		bones;
	core::array<RawBoneInfluence> influences;
};

struct psa_t
{
	ChunkHeader			psaHeader;
	ChunkHeader			boneHeader;
	ChunkHeader			animateHeader;
	ChunkHeader			keyHeader;

	core::array<Bone>				bones;
	core::array<AnimInfoBinary>		animates;
	core::array<QuatAnimKey>		keys;
};

typedef struct vertex_link
{
	vertex_link( s32 bfid = -1, s32 vid = -1 )
	{
		buffer_id = bfid;
		vertex_id = vid;
		plinkd = 0;
	}

	void operator =(const struct vertex_link& other)
	{
		buffer_id = other.buffer_id;
		vertex_id = other.vertex_id;
	}

	//! The == operator is provided so that CFileList can slowly search the list!
	bool operator ==(const struct vertex_link& other) const
	{
		if ( buffer_id == other.buffer_id &&
			 vertex_id == other.vertex_id )
			return true;

		return false;
	}
	bool isOk()
	{
		return vertex_id!=-1?true:false;
	}

	s32 buffer_id;
	s32 vertex_id;

	core::array<vertex_link>* plinkd;
}vertex_link;

static psk_t* load_psk( io::IReadFile* f )
{
	psk_t* psk = new psk_t( );

	f->read( &psk->pskHeader, sizeof(ChunkHeader) );

	// point
	f->read( &psk->pointHeader, sizeof(ChunkHeader) );
	psk->points.reallocate( psk->pointHeader.dataCount );
	psk->points.set_used( psk->pointHeader.dataCount );
	f->read( psk->points.pointer(), psk->pointHeader.dataCount*psk->pointHeader.dataSize );

	// vertex
	f->read( &psk->vertexHeader, sizeof(ChunkHeader) );
	psk->vertexs.reallocate( psk->vertexHeader.dataCount );
	psk->vertexs.set_used( psk->vertexHeader.dataCount );
	f->read( psk->vertexs.pointer(), psk->vertexHeader.dataCount*psk->vertexHeader.dataSize );

	// faces
	f->read( &psk->faceHeader, sizeof(ChunkHeader) );
	psk->faces.reallocate( psk->faceHeader.dataCount );
	psk->faces.set_used( psk->faceHeader.dataCount );
	f->read( psk->faces.pointer(), psk->faceHeader.dataCount*psk->faceHeader.dataSize );
	for( u32 i = 0; i < psk->faces.size(); ++i )
	{
		u16 x = psk->faces[i].wedgeIndex[0];
		psk->faces[i].wedgeIndex[0] = psk->faces[i].wedgeIndex[2];
		psk->faces[i].wedgeIndex[2] = x;
	}

	// maretial
	f->read( &psk->materialHeader, sizeof(ChunkHeader) );
	psk->materials.reallocate( psk->materialHeader.dataCount );
	psk->materials.set_used( psk->materialHeader.dataCount );
	f->read( psk->materials.pointer(), psk->materialHeader.dataCount*psk->materialHeader.dataSize );

	// bone
	f->read( &psk->boneHeader, sizeof(ChunkHeader) );
	psk->bones.reallocate( psk->boneHeader.dataCount );
	psk->bones.set_used( psk->boneHeader.dataCount );
	f->read( psk->bones.pointer(), psk->boneHeader.dataCount*psk->boneHeader.dataSize );
	if( !psk->bones.empty() )
	{
		psk->bones[0].bonePos.orientation.X *= -1;
		psk->bones[0].bonePos.orientation.Y *= -1;
		psk->bones[0].bonePos.orientation.Z *= -1;
	}

	// influence
	f->read( &psk->influenceHeader, sizeof(ChunkHeader) );
	psk->influences.reallocate( psk->influenceHeader.dataCount );
	psk->influences.set_used( psk->influenceHeader.dataCount );
	f->read( psk->influences.pointer(), psk->influenceHeader.dataCount*psk->influenceHeader.dataSize );

	return psk;
}

static psa_t* load_psa( io::IReadFile* f )
{
	psa_t* psa = new psa_t( );

	f->read( &psa->psaHeader, sizeof(ChunkHeader) );

	// bone
	f->read( &psa->boneHeader, sizeof(ChunkHeader) );
	psa->bones.reallocate( psa->boneHeader.dataCount );
	psa->bones.set_used( psa->boneHeader.dataCount );
	f->read( psa->bones.pointer(), psa->boneHeader.dataCount*psa->boneHeader.dataSize );
	if( !psa->bones.empty() )
	{
		psa->bones[0].bonePos.orientation.X *= -1;
		psa->bones[0].bonePos.orientation.Y *= -1;
		psa->bones[0].bonePos.orientation.Z *= -1;
	}

	// animate
	f->read( &psa->animateHeader, sizeof(ChunkHeader) );
	psa->animates.reallocate( psa->animateHeader.dataCount );
	psa->animates.set_used( psa->animateHeader.dataCount );
	f->read( psa->animates.pointer(), psa->animateHeader.dataCount*psa->animateHeader.dataSize );

	// key
	f->read( &psa->keyHeader, sizeof(ChunkHeader) );
	psa->keys.reallocate( psa->keyHeader.dataCount );
	psa->keys.set_used( psa->keyHeader.dataCount );
	f->read( psa->keys.pointer(), psa->keyHeader.dataCount*psa->keyHeader.dataSize );
	for (u32 i = 0; i < psa->keys.size(); ++i)
	{			
		if( !(i%psa->bones.size()) )
		{
			psa->keys[i].orientation.X *= -1;
			psa->keys[i].orientation.Y *= -1;
			psa->keys[i].orientation.Z *= -1;
		}
	}

	return psa;
}

static s32 addv( psk_t* psk, core::array<vertex_link>& vlink, core::array<vertex_link>& plink, SSkinMeshBuffer* smb, u16 vertex_index, u16 buffer_id )
{
	u16 ivx = vertex_index;

	if( vlink[ivx].isOk() )
		return vlink[ivx].vertex_id;

	video::S3DVertex v;
	v.Color = video::SColor( 255,255,255,255 );
	v.Normal= core::vector3df(0);

	u32 pix = psk->vertexs[ivx].pointIndex;
	memcpy( &v.Pos.X, &psk->points[pix].point.X, sizeof(core::vector3df) );
	memcpy( &v.TCoords.X, &psk->vertexs[ivx].uv.X, sizeof(core::vector2df) );

	smb->Vertices_Standard.push_back( v );
	u32 slot = smb->Vertices_Standard.size( )-1;
	vlink[ivx] = vertex_link( buffer_id, slot );
	if( !plink[psk->vertexs[ivx].pointIndex].isOk() )
		plink[psk->vertexs[ivx].pointIndex] = vertex_link( buffer_id, slot );
	else
		plink[psk->vertexs[ivx].pointIndex].plinkd->push_back( vertex_link(buffer_id, slot) );

	return slot;
}

static bool psk_to_irr( psk_t* psk, SSkinnedMesh* AnimatedMesh )
{
	core::array<vertex_link> vlink;
	core::array<vertex_link> plink;

	vlink.reallocate( psk->vertexHeader.dataCount );	
	vlink.set_used( psk->vertexHeader.dataCount );
	memset( vlink.pointer(), -1, sizeof(vertex_link)*psk->vertexHeader.dataCount );
	plink.reallocate( psk->pointHeader.dataCount );
	plink.set_used( psk->pointHeader.dataCount );
	memset( plink.pointer(), -1, sizeof(vertex_link)*psk->pointHeader.dataCount );
	for( u32 i = 0; i < plink.size(); ++i )
		plink[i].plinkd = new core::array<vertex_link>();

	// meshbuffer
	for( u32 i = 0; i < psk->materials.size(); ++i )
		AnimatedMesh->addMeshBuffer( );

	// vertices & indices
	for( u32 i = 0; i < psk->faces.size(); ++i )
	{
		Triangle face = psk->faces[i];
		s32 nr = face.matIndex;
		scene::SSkinMeshBuffer* smb = reinterpret_cast<scene::SSkinMeshBuffer*>(AnimatedMesh->getMeshBuffer(nr));
		if( !smb )
			continue;

		s32 id;
		id = addv( psk, vlink, plink, smb, face.wedgeIndex[0], nr );
		smb->Indices.push_back( id );
		id = addv( psk, vlink, plink, smb, face.wedgeIndex[1], nr );
		smb->Indices.push_back( id );
		id = addv( psk, vlink, plink, smb, face.wedgeIndex[2], nr );
		smb->Indices.push_back( id );

	}

	for( u32 i = 0; i < psk->materials.size(); ++i )
	{
		scene::SSkinMeshBuffer* smb = reinterpret_cast<scene::SSkinMeshBuffer*>(AnimatedMesh->getMeshBuffer(i));

		for( u32 j = 0; j < smb->getIndexCount(); j+=3 )
		{
			video::S3DVertex* v[3] = {  smb->getVertex(smb->Indices[j+0]),
										smb->getVertex(smb->Indices[j+1]),
										smb->getVertex(smb->Indices[j+2]) };
			core::plane3df p(v[0]->Pos, v[1]->Pos, v[2]->Pos );
			v[0]->Normal += p.Normal;
			v[1]->Normal += p.Normal;
			v[2]->Normal += p.Normal;
		}

		// final normalize
		for ( u32 j = 0; j<smb->getVertexCount(); ++j )
			smb->getVertex(j)->Normal.normalize();
	}

	// create joint
	core::array<u32> parentid;
	for( u32 i = 0; i < psk->bones.size(); ++i )
	{
		Bone bone = psk->bones[i];

		scene::ISkinnedMesh::SJoint* joint = AnimatedMesh->addJoint( );

		parentid.push_back( bone.parentIndex );
		joint->Name = bone.name;
		joint->Animatedscale = core::vector3df(1);
		memcpy( &joint->Animatedposition.X, &bone.bonePos.position.X, sizeof(core::vector3df) );
		memcpy( &joint->Animatedrotation.X, &bone.bonePos.orientation.X, sizeof(core::quaternion) );

		//Build LocalMatrix:
		core::matrix4 positionMatrix;
		positionMatrix.setTranslation( joint->Animatedposition );
		core::matrix4 scaleMatrix;
		scaleMatrix.setScale( joint->Animatedscale );
		core::matrix4 rotationMatrix = joint->Animatedrotation.getMatrix();

		joint->LocalMatrix = positionMatrix * rotationMatrix * scaleMatrix;	
	} // for i

	//find parent of every joint
	for (u32 jointnum=0; jointnum<AnimatedMesh->getAllJoints().size(); ++jointnum)
	{
		for (u32 j2=0; j2<AnimatedMesh->getAllJoints().size(); ++j2)
		{
			if (jointnum != j2 && parentid[jointnum] == j2 )
			{
				AnimatedMesh->getAllJoints()[j2]->Children.push_back(AnimatedMesh->getAllJoints()[jointnum]);
				break;
			}
		}
	}

	// influence
	for( u32 i = 0; i < psk->influences.size(); ++i )
	{
		RawBoneInfluence influence = psk->influences[i];
		vertex_link point = plink[influence.pointIndex];
		scene::ISkinnedMesh::SJoint* joint = AnimatedMesh->getAllJoints()[influence.boneIndex];
		scene::ISkinnedMesh::SWeight* weight = AnimatedMesh->addWeight( joint );
		weight->strength = influence.weight;
		weight->buffer_id = point.buffer_id;
		weight->vertex_id = point.vertex_id;

		for( u32 j = 0; j < plink[influence.pointIndex].plinkd->size(); ++j )
		{
			scene::ISkinnedMesh::SJoint* joint = AnimatedMesh->getAllJoints()[influence.boneIndex];
			scene::ISkinnedMesh::SWeight* weight = AnimatedMesh->addWeight( joint );
			weight->strength = influence.weight;
			weight->buffer_id = (*plink[influence.pointIndex].plinkd)[j].buffer_id;
			weight->vertex_id = (*plink[influence.pointIndex].plinkd)[j].vertex_id;
		}
	} // for i

	for( u32 i = 0; i < plink.size(); ++i )
		if(plink[i].plinkd) 
			delete plink[i].plinkd;

	return true;
}

static bool psa_to_irr( psa_t* psa, SSkinnedMesh* AnimatedMesh )
{
	// create joint
	core::array<u32> parentid;
	for( u32 i = 0; i < psa->bones.size(); ++i )
	{
		Bone bone = psa->bones[i];

		scene::ISkinnedMesh::SJoint* joint = AnimatedMesh->addJoint( );

		parentid.push_back( bone.parentIndex );
		joint->Name = bone.name;
		joint->Animatedscale = core::vector3df(1);
		memcpy( &joint->Animatedposition.X, &bone.bonePos.position.X, sizeof(core::vector3df) );
		memcpy( &joint->Animatedrotation.X, &bone.bonePos.orientation.X, sizeof(core::quaternion) );

		//Build LocalMatrix:
		core::matrix4 positionMatrix;
		positionMatrix.setTranslation( joint->Animatedposition );
		core::matrix4 scaleMatrix;
		scaleMatrix.setScale( joint->Animatedscale );
		core::matrix4 rotationMatrix = joint->Animatedrotation.getMatrix();

		joint->LocalMatrix = positionMatrix * rotationMatrix * scaleMatrix;	
	} // for i

	//find parent of every joint
	for (u32 jointnum=0; jointnum<AnimatedMesh->getAllJoints().size(); ++jointnum)
	{
		for (u32 j2=0; j2<AnimatedMesh->getAllJoints().size(); ++j2)
		{
			if (jointnum != j2 && parentid[jointnum] == j2 )
			{
				AnimatedMesh->getAllJoints()[j2]->Children.push_back(AnimatedMesh->getAllJoints()[jointnum]);
				break;
			}
		}
	}

	for( u32 a = 0; a < psa->animates.size(); ++a )
	{
		AnimInfoBinary anim = psa->animates[a];
		if( !a )
			AnimatedMesh->setAnimationSpeed( anim.animRate );		
		s32 end = anim.firstRawFrame+anim.numRawFrames;
		AnimatedMesh->addAnimation( core::stringc(anim.name), anim.firstRawFrame, end, anim.animRate );
		for( s32 i = anim.firstRawFrame; i < end; ++i )
		{
			for( u32 j = 0; j < AnimatedMesh->getJointCount(); ++j )
			{
				scene::ISkinnedMesh::SJoint* joint = AnimatedMesh->getAllJoints()[j];
				scene::ISkinnedMesh::SPositionKey* pk = AnimatedMesh->addPositionKey( joint );
				scene::ISkinnedMesh::SRotationKey* rk = AnimatedMesh->addRotationKey( joint );

				int f = j + (i*AnimatedMesh->getJointCount());
				QuatAnimKey key = psa->keys[f];

				pk->frame = (f32)i;
				memcpy( &pk->position.X, &key.position.X, sizeof(core::vector3df) );

				rk->frame = (f32)i;
				memcpy( &rk->rotation.X, &key.orientation.X, sizeof(core::quaternion) );
			}
		}
	}

	return true;
}

//! Constructor
CPSKMeshFileLoader::CPSKMeshFileLoader(scene::ISceneManager* smgr, ILogger* log)
: SceneManager(smgr), AnimatedMesh(0), Log(log)
{
	#ifdef _DEBUG
	setDebugName("CPSKMeshFileLoader");
	#endif
}


//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool CPSKMeshFileLoader::isALoadableFileExtension(const io::path& filename) const
{
	return core::hasFileExtension ( filename, "psk" );
}


//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh* CPSKMeshFileLoader::createMesh(io::IReadFile* f)
{
	if (!f)
		return 0;

	#ifdef _DEBUG
	if( Log )
	{			
		Log->log( (core::stringw("Loading ")+f->getFileName()).c_str() );
	}
	#endif

	AnimatedMesh = new SSkinnedMesh( SceneManager->createSkinnedMesh() );

	psk_t* psk = load_psk( f );
	if ( psk )
	{
		psk_to_irr( psk, AnimatedMesh );

		#ifdef _DEBUG
		if( Log )
		{
			Log->log( (core::stringw("numPoints: ")+core::stringw(psk->points.size())).c_str() );
			Log->log( (core::stringw("numVertexs: ")+core::stringw(psk->vertexs.size())).c_str() );
			Log->log( (core::stringw("numFaces: ")+core::stringw(psk->faces.size())).c_str() );
			Log->log( (core::stringw("numMaterials: ")+core::stringw(psk->materials.size())).c_str() );
			Log->log( (core::stringw("numBones: ")+core::stringw(psk->bones.size())).c_str() );
			Log->log( (core::stringw("numWeights: ")+core::stringw(psk->influences.size())).c_str() );

			Log->log( "Bones Info" );
			for( u32 i = 0; i < psk->bones.size(); ++i )
			{
				core::stringw ainfo;
				ainfo = core::stringw(i+1);
				ainfo += core::stringw(".) ");
				ainfo += psk->bones[i].name;
				Log->log( ainfo.c_str() );
			}
		}
		#endif

		delete psk;

		AnimatedMesh->finalize();
	}
	else
	{
		AnimatedMesh->drop();
		AnimatedMesh = 0;
	}

	return AnimatedMesh;
}

//! Constructor
CPSAMeshFileLoader::CPSAMeshFileLoader(scene::ISceneManager* smgr, ILogger* log)
: SceneManager(smgr), AnimatedMesh(0), Log(log)
{
	#ifdef _DEBUG
	setDebugName("CPSKMeshFileLoader");
	#endif
}


//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool CPSAMeshFileLoader::isALoadableFileExtension(const io::path& filename) const
{
	return core::hasFileExtension ( filename, "psa" );
}


//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh* CPSAMeshFileLoader::createMesh(io::IReadFile* f)
{
	if (!f)
		return 0;

	#ifdef _DEBUG
	if( Log )
	{			
		Log->log( (core::stringw("Loading ")+f->getFileName()).c_str() );
	}
	#endif

	AnimatedMesh = new SSkinnedMesh( SceneManager->createSkinnedMesh() );

	psa_t* psa = load_psa( f );
	if ( psa )
	{
		psa_to_irr( psa, AnimatedMesh );
		#ifdef _DEBUG
		if( Log )
		{			
			Log->log( (core::stringw("numBones: ")+core::stringw(psa->bones.size())).c_str() );
			Log->log( (core::stringw("numAnimations: ")+core::stringw(psa->animates.size())).c_str() );
			Log->log( (core::stringw("numKeyFrames: ")+core::stringw(psa->keys.size())).c_str() );

			Log->log( "Bones Info" );
			for( u32 i = 0; i < psa->bones.size(); ++i )
			{
				core::stringw ainfo;
				ainfo = core::stringw(i+1);
				ainfo += core::stringw(".) ");
				ainfo += psa->bones[i].name;
				Log->log( ainfo.c_str() );
			}

			Log->log( "Animations Info" );
			for( u32 i = 0; i < AnimatedMesh->getAnimationCount(); ++i )
			{
				core::stringw ainfo;
				ainfo = core::stringw(i+1);
				ainfo += core::stringw(".) ");
				ainfo += AnimatedMesh->getAnimations()[i].Name;
				ainfo += " Begin: ";
				ainfo += AnimatedMesh->getAnimations()[i].Begin;
				ainfo += " End: ";
				ainfo += AnimatedMesh->getAnimations()[i].End;
				ainfo += " Fps: ";
				ainfo += AnimatedMesh->getAnimations()[i].Fps;
				Log->log( ainfo.c_str() );
			}
		}
		#endif
		delete psa;

		AnimatedMesh->finalize();
	}
	else
	{
		AnimatedMesh->drop();
		AnimatedMesh = 0;
	}

	return AnimatedMesh;
}

} // end namespace scene
} // end namespace irr


#endif // _IRR_COMPILE_WITH_UNREAL_LOADER_

