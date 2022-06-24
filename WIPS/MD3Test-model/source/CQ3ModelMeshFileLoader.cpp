#include <os.h>
#include "CQ3ModelMeshFileLoader.h"
#include "CQ3ModelMesh.h"
#include "pool.h"

namespace irr
{
namespace scene
{


/********************** Extracted principally from "q3files.h" ****************/

#if defined(_MSC_VER) ||  defined(__BORLANDC__) || defined (__BCPLUSPLUS__)
#	pragma pack( push, packing )
#	pragma pack( 1 )
#	define PACK_STRUCT
#elif defined( __GNUC__ )
#	define PACK_STRUCT	__attribute__((packed))
#else
#	error compiler not supported
#endif

// the maximum size of game reletive pathnames
#define	MAX_QPATH		64


#define MD3_IDENT			(('3'<<24)+('P'<<16)+('D'<<8)+'I')
#define MD3_VERSION			15

struct SMD3_Frame {
        f32 minBounds[3];
        f32 maxBounds[3];
        f32 Translate[3];
        f32 Scale;
        c8 Name[16];
} PACK_STRUCT;


struct SMD3_Tag {
        c8 Name[64];
        f32 Origin[3];
        f32 Rotation[3][3];
} PACK_STRUCT;


/*
** md3Surface_t
**
** CHUNK			SIZE
** header			sizeof( md3Surface_t )
** shaders			sizeof( md3Shader_t ) * numShaders
** triangles[0]		sizeof( md3Triangle_t ) * numTriangles
** st				sizeof( md3St_t ) * numVerts
** XyzNormals		sizeof( md3XyzNormal_t ) * numVerts * numFrames
*/

struct SMD3_Surface {
        s32 magicNumber;
        c8 Name[MAX_QPATH];
        s32 Flags;
        u32 numFrames;
        u32 numShaders;
        u32 numVertices;
        u32 numTriangles;
        u32 ofsTriangles;
        u32 ofsShaders;
        u32 ofsTexels;
        u32 ofsVertices;
        u32 Size;
} PACK_STRUCT;


struct SMD3_Shader {
        char fileName[MAX_QPATH];
        s32 Index;
} PACK_STRUCT;


struct SMD3_Triangle {
        s32 vIndex[3];
} PACK_STRUCT;


struct SMD3_Texel {
        f32 S;
        f32 T;
} PACK_STRUCT;


struct SMD3_Vertex {
        s16 X, Y, Z;
        s16 encNormal;
} PACK_STRUCT;


struct SMD3_Header {
        s32 magicNumber;
        s32 Version;
        c8  fileName[MAX_QPATH];
        s32 Flags;
        u32 numFrames;
        u32 numTags;
        u32 numSurfaces;
        u32 numSkins;
        u32 ofsFrames;
        u32 ofsTags;
        u32 ofsSurfaces;
        u32 ofsEOF;
} PACK_STRUCT;

// Default alignment
#ifdef _MSC_VER
#	pragma pack( pop, packing )
#endif

#undef PACK_STRUCT

EMD3_MODEL_TYPE calculateTypeOfModel(io::IReadFile* file)
{
    static const c8* BaseNames[9] = {"head.md3",   "upper.md3",   "lower.md3",
                                     "head_1.md3", "upper_1.md3", "lower_1.md3",
                                     "head_2.md3", "upper_2.md3", "lower_2.md3"};
    static u32 TotalBaseNames = 9;

    core::stringc name;
    s32 d;
    name = file->getFileName();
    if ((d = name.findLast('/')) != -1 || (d = name.findLast('\\')) != -1)
        name = name.subString(d+1,name.size());

    if (name.size() >= 8)
    {
        u32 i;
        for (i = 0; i < TotalBaseNames; i++)
        {
            if (name == BaseNames[i])
            {
                switch(name[0])
                {
                    case 'h': return MD3_HEAD; break;
                    case 'u': return MD3_UPPER; break;
                    case 'l': return MD3_LOWER; break;
                    default : break;
                }
                break;
            }
        }
    }

    return MD3_UNKNOWN;
}

bool loadHeader(core::Pool* pool, CQ3ModelMesh* mesh, SMD3_Header **pHeader, u32 totalFrames)
{
 // Load header.
	if (pool->available() < sizeof(SMD3_Header))
    {
		os::Printer::log ("MD3 wrong header", ELL_ERROR);
		return false;
	}
	*pHeader = (SMD3_Header*)pool->pointer();

	SMD3_Header* Header;
    Header = *pHeader;

 // Become 2 host...
#ifdef __BIG_ENDIAN__
    Header->magicNumber = os::Byteswap::byteswap(Header.magicNumber);
    Header->Version     = os::Byteswap::byteswap(Header.Version    );
    Header->Flags       = os::Byteswap::byteswap(Header.Flags      );
    Header->numFrames   = os::Byteswap::byteswap(Header.numFrames  );
    Header->numTags     = os::Byteswap::byteswap(Header.numTags    );
    Header->numSurfaces = os::Byteswap::byteswap(Header.numSurfaces);
    Header->numSkins    = os::Byteswap::byteswap(Header.numSkins   );
    Header->ofsFrames   = os::Byteswap::byteswap(Header.ofsFrames  );
    Header->ofsTags     = os::Byteswap::byteswap(Header.ofsTags    );
    Header->ofsSurfaces = os::Byteswap::byteswap(Header.ofsSurfaces);
    Header->ofsEOF      = os::Byteswap::byteswap(Header.ofsEOF     );
#endif

 // Check file header
    if (Header->magicNumber != MD3_IDENT)
    {
		os::Printer::log ("MD3 wrong IDENT", ELL_ERROR);
		return false;
	}
    if (Header->Version != MD3_VERSION)
    {
		os::Printer::log ("MD3 wrong VERSION MD3", ELL_ERROR);
		return false;
	}

 // Show information?
    if (mesh == NULL)
    {
       os::Printer::log ("Magic number", core::stringc(Header->magicNumber).c_str(), ELL_INFORMATION);
       os::Printer::log ("Version", core::stringc(Header->Version).c_str(), ELL_INFORMATION);
       os::Printer::log ("Flags", core::stringc(Header->Flags).c_str(), ELL_INFORMATION);
       os::Printer::log ("Num. frames", core::stringc(Header->numFrames).c_str(), ELL_INFORMATION);
       os::Printer::log ("Num. tags", core::stringc(Header->numTags).c_str(), ELL_INFORMATION);
       os::Printer::log ("Num. surfaces", core::stringc(Header->numSurfaces).c_str(), ELL_INFORMATION);
       os::Printer::log ("Num. skins", core::stringc(Header->numSkins).c_str(), ELL_INFORMATION);
       os::Printer::log ("Ofs. frames", core::stringc(Header->ofsFrames).c_str(), ELL_INFORMATION);
       os::Printer::log ("Ofs. tags", core::stringc(Header->ofsTags).c_str(), ELL_INFORMATION);
       os::Printer::log ("Ofs. surfaces", core::stringc(Header->ofsSurfaces).c_str(), ELL_INFORMATION);
       os::Printer::log ("Ofs. EOF", core::stringc(Header->ofsEOF).c_str(), ELL_INFORMATION);
       return true;
	}

 // Ok!
	mesh->setMeshName(Header->fileName);
	if (totalFrames == 0)
	   mesh->setFrameCount(Header->numFrames);
    else
       mesh->setFrameCount(totalFrames);

    return true;
}

bool loadBoneFrames(core::Pool* pool, CQ3ModelMesh* mesh, SMD3_Header *Header, u32 firstFrame)
{
    if (Header->numFrames == 0)
        return true;

 // Seeking...
    if (!pool->seek(Header->ofsFrames)) {
        os::Printer::log ("MD3 missing bones frames", ELL_ERROR);
        return false;
    }

 // Checking...
    u32 size;
    size = ((u32)sizeof(SMD3_Frame)) * Header->numFrames;
    if (pool->available() < size) {
        os::Printer::log ("MD3 truncated on bones frames", ELL_ERROR);
        return false;
    }

 // Show information?
    if (mesh == NULL)
    {
		return true;
	}

 // Extracting...
    SMD3_Frame *pBones;
    u32 i;

    pBones = (SMD3_Frame*) pool->pointer();
    pBones+= firstFrame;
    for (i = 0; i < mesh->getFrameCount(); i++, pBones++)
    {
        SMD3_Frame& Bones = *pBones;
     // Become 2 host...
#ifdef __BIG_ENDIAN__
        *((long*)&Bones.minBounds[0]) = os::Byteswap::byteswap(Bones.minBounds[0]);
        *((long*)&Bones.minBounds[1]) = os::Byteswap::byteswap(Bones.minBounds[1]);
        *((long*)&Bones.minBounds[2]) = os::Byteswap::byteswap(Bones.minBounds[2]);
        *((long*)&Bones.maxBounds[0]) = os::Byteswap::byteswap(Bones.maxBounds[0]);
        *((long*)&Bones.maxBounds[1]) = os::Byteswap::byteswap(Bones.maxBounds[1]);
        *((long*)&Bones.maxBounds[2]) = os::Byteswap::byteswap(Bones.maxBounds[2]);
        *((long*)&Bones.Translate[0]) = os::Byteswap::byteswap(Bones.Translate[0]);
        *((long*)&Bones.Translate[1]) = os::Byteswap::byteswap(Bones.Translate[1]);
        *((long*)&Bones.Translate[2]) = os::Byteswap::byteswap(Bones.Translate[2]);
        *((long*)&Bones.Scale)        = os::Byteswap::byteswap(Bones.Scale);
#endif

     // New box 2 mesh.
        mesh->addBone(
              core::aabbox3d<f32> ( Bones.minBounds[0],
                                    Bones.minBounds[1],
                                    Bones.minBounds[2],
                                    Bones.maxBounds[0],
                                    Bones.maxBounds[1],
                                    Bones.maxBounds[2]
                                    )
               );
    }

 // Ok!
    return true;
}

bool loadTags(core::Pool* pool, CQ3ModelMesh* mesh, SMD3_Header *Header, u32 firstFrame)
{
 // Seeking...
    if (!pool->seek(Header->ofsTags))
    {
        os::Printer::log ("MD3 missing tags", ELL_ERROR);
        return false;
    }

 // Checking...
    u32 size;
    size = ((u32)sizeof(SMD3_Tag)) * Header->numTags * Header->numFrames;
    if (pool->available() < size) {
        os::Printer::log ("MD3 truncated on tags", ELL_ERROR);
        return false;
    }

    if (Header->numTags == 0)
       return true;

 // Show information?
    if (mesh == NULL)
    {
        u32 tagCnt;
        SMD3_Tag* pTag;
        pTag = (SMD3_Tag*) pool->pointer();
        for (tagCnt = 0; tagCnt < Header->numTags; tagCnt++, pTag++)
        {
            SMD3_Tag& Tag = *pTag;
            core::stringc s;
            s = "Tag ";
            s+= tagCnt;
            s+= ": ";
            s+= Tag.Name;
            os::Printer::log(s.c_str(), ELL_INFORMATION);
        }
		return true;
	}

 // Reallocate...
    mesh->setTagCount(Header->numTags);
    if (mesh->getTagCount() == 0) {
        os::Printer::log("Out of memory on tags");
        return false;
    }

 // MD3 tags.
    SMD3_Tag* pTag;

    pTag = (SMD3_Tag*) pool->pointer();
    pTag += firstFrame;
    for (u32 frameCnt = 0; frameCnt < mesh->getFrameCount(); frameCnt++)
    {
        for (u32 tagCnt = 0; tagCnt < mesh->getTagCount(); tagCnt++, pTag++)
        {
            SMD3_Tag& Tag = *pTag;

         // Become 2 host...
#ifdef __BIG_ENDIAN__
            for ( s32 itagCnt=0; itagCnt<3; ++itagCnt )
            {
                *((long*)&Tag.Origin[itagCnt])      = os::Byteswap::byteswap(Tag.Origin[itagCnt]);
                *((long*)&Tag.Rotation[itagCnt][0]) = os::Byteswap::byteswap(Tag.Rotation[itagCnt][0]);
                *((long*)&Tag.Rotation[itagCnt][1]) = os::Byteswap::byteswap(Tag.Rotation[itagCnt][1]);
                *((long*)&Tag.Rotation[itagCnt][2]) = os::Byteswap::byteswap(Tag.Rotation[itagCnt][2]);
            }
#endif
         // MD3 Tag 2 Memory Tag.
            tag_t TD;
            // Origin.
            TD.Origin      = core::vector3df (Tag.Origin[0],      Tag.Origin[1],      Tag.Origin[2]);
            // Rotation.
            TD.Rotation[0] = core::vector3df (Tag.Rotation[0][0], Tag.Rotation[0][1], Tag.Rotation[0][2]);
            TD.Rotation[1] = core::vector3df (Tag.Rotation[1][0], Tag.Rotation[1][1], Tag.Rotation[1][2]);
            TD.Rotation[2] = core::vector3df (Tag.Rotation[2][0], Tag.Rotation[2][1], Tag.Rotation[2][2]);
            // Transformation matrix.
            // Identity.
            TD.Transformation.makeIdentity ();
            // Row 0.
            TD.Transformation (0, 0) = Tag.Rotation[0][0];
            TD.Transformation (0, 1) = Tag.Rotation[0][1];
            TD.Transformation (0, 2) = Tag.Rotation[0][2];
            TD.Transformation (0, 3) = 0.0f;
            // Row 1.
            TD.Transformation (1, 0) = Tag.Rotation[1][0];
            TD.Transformation (1, 1) = Tag.Rotation[1][1];
            TD.Transformation (1, 2) = Tag.Rotation[1][2];
            TD.Transformation (1, 3) = 0.0f;
            // Row 2.
            TD.Transformation (2, 0) = Tag.Rotation[2][0];
            TD.Transformation (2, 1) = Tag.Rotation[2][1];
            TD.Transformation (2, 2) = Tag.Rotation[2][2];
            TD.Transformation (2, 3) = 0.0f;
            // Row 3.
            TD.Transformation (3, 0) = Tag.Origin[0];
            TD.Transformation (3, 1) = Tag.Origin[1];
            TD.Transformation (3, 2) = Tag.Origin[2];
            TD.Transformation (3, 3) = 1.0f;

         // Register tag.
            mesh->addTag(Tag.Name, TD);
        }
    }
 // Ok!
    return true;
}

bool loadSurfaces(core::Pool* pool, CQ3ModelMesh* mesh, SMD3_Header *Header, u32 firstFrame)
{
    if (Header->numSurfaces == 0)
       return true;

 // Show information?
    if (mesh == NULL)
    {
        os::Printer::log ("Num. Surfaces", core::stringc(Header->numSurfaces).c_str(), ELL_INFORMATION);

        u32 srfCnt, offset;
        offset = Header->ofsSurfaces;
        for (srfCnt = 0; srfCnt < Header->numSurfaces; ++srfCnt)
        {
        // Seeking...
           if (!pool->seek(offset))
           {
               os::Printer::log ("MD3 missing surfaces", ELL_ERROR);
               return false;
           }
        // Header.
           SMD3_Surface* MeshH;
           MeshH = (SMD3_Surface*) pool->pointer();
        // Information.
           core::stringc s;
           s = "Surface ";
           s+= srfCnt;
           s+= ": ";
           s+= MeshH->Name;
           s+= ", triangles ";
           s+= MeshH->numTriangles;
           s+= ", vertices ";
           s+= MeshH->numVertices;
           os::Printer::log(s.c_str(), ELL_INFORMATION);

        // Shaders.
           SMD3_Shader *Shaders;
           pool->seek(offset + MeshH->ofsShaders);
           Shaders = (SMD3_Shader*) pool->pointer();

           u32 shdCnt;
           for (shdCnt = 0; shdCnt < MeshH->numShaders; shdCnt++, Shaders++)
           {
               s = "- Shader ";
               s+= shdCnt;
               s+= ": ";
               s+= Shaders->fileName;
               s+= Shaders->Index;
               os::Printer::log(s.c_str(), ELL_INFORMATION);
           }
           offset += MeshH->Size;
        }
		return true;
	}

 // Reallocate.
    mesh->setSurfacesCount(Header->numSurfaces);
    if (mesh->getSurfaceCount() == 0) {
        os::Printer::log("Out of memory to reserve space for the surfaces");
        return false;
    }

 // Running surfaces...
    u32 srfCnt;
    u32 h_size, offset;

    h_size = sizeof(SMD3_Surface);
    offset = Header->ofsSurfaces;
    for (srfCnt = 0; srfCnt < mesh->getSurfaceCount(); ++srfCnt)
    {
    // Seeking...
       if (!pool->seek(offset))
       {
           os::Printer::log ("MD3 missing surfaces", ELL_ERROR);
           return false;
       }

    // Checking header.
       if (pool->available() < h_size) {
           os::Printer::log ("MD3 truncated on surface header", ELL_ERROR);
           return false;
       }

    // Header.
       SMD3_Surface* MeshH;
       MeshH = (SMD3_Surface*) pool->pointer();

    // Checking data...
       // Size
       if (pool->available() < MeshH->Size)
           os::Printer::log ("MD3 truncated on surfaces data", ELL_WARNING);

       // Frames
       if (MeshH->numFrames != Header->numFrames) {
           os::Printer::log ("Mesh frame count mismatch on surface!", ELL_ERROR);
           return false;
       }

    // New Surface.
       CQ3Surface* S;
       if ( (S = mesh->newSurface(MeshH->Name, MeshH->numTriangles*3, MeshH->numVertices)) == NULL ) {
           os::Printer::log("Out of memory to create a surface");
           return false;
       }

    // Reading...
       // Shaders.
       SMD3_Shader *Shaders;
       pool->seek(offset + MeshH->ofsShaders);
       Shaders = (SMD3_Shader*) pool->pointer();

       // Read texture coordinates
       SMD3_Texel *Textures;
       pool->seek(offset + MeshH->ofsTexels);
       Textures = (SMD3_Texel*) pool->pointer();

       // Read vertexs.
       u32 vxMax;
       SMD3_Vertex* Vertex;
       pool->seek(offset + MeshH->ofsVertices);
       Vertex = (SMD3_Vertex*) pool->pointer();
       vxMax = MeshH->numVertices;
       Vertex+= (firstFrame * vxMax);

       // Vertex Frame List
       f32 Lat, Lng;
       const f32 BASENORMAL = (2.f * core::PI) / 255.f;

       for (u32 frameCnt = 0; frameCnt < mesh->getFrameCount(); ++frameCnt)
       {
        // Initialize source texture coordinates for a frame.
           SMD3_Texel* TexCoords;
           TexCoords = Textures;

        // Initialize source vertex for a frame.
           video::S3DVertex2TCoords *Vtx = (video::S3DVertex2TCoords *)S->getVertices(frameCnt);

        // Create final vertex for a frame.
           for (u32 triCnt = 0; triCnt < vxMax; ++triCnt, ++TexCoords, ++Vertex, ++Vtx)
           {
            // Color : For ever white.
               Vtx->Color = video::SColor (0xFF, 0xFF, 0xFF, 0xFF);
            // Position.
               Vtx->Pos.X = Vertex->X / 64.0f;
               Vtx->Pos.Y = Vertex->Y / 64.0f;
               Vtx->Pos.Z = Vertex->Z / 64.0f;
            // Normals.
               Lat = ((f32)(Vertex->encNormal & 0xFF       )) * BASENORMAL;
               Lng = ((f32)((Vertex->encNormal >> 8) & 0xFF)) * BASENORMAL;
               Vtx->Normal.X = cosf(Lat) * sinf(Lng);
               Vtx->Normal.X = sinf(Lat) * sin(Lng);
               Vtx->Normal.X = cosf(Lng);
            // Texture coordinates.
               Vtx->TCoords.X = TexCoords->S;
               Vtx->TCoords.Y = TexCoords->T;
            // ¿For light map...?
               Vtx->TCoords2.X = TexCoords->S;
               Vtx->TCoords2.Y = TexCoords->T;
           }
       }

       // Triangles.
       u32 trMax;
       SMD3_Triangle *Triangles;
       pool->seek(offset + MeshH->ofsTriangles);
       Triangles = (SMD3_Triangle*) pool->pointer();
       trMax = MeshH->numTriangles;

       // List of indexes
       u16* Indices = S->getIndices();
       for (u32 idxCnt = 0; idxCnt < trMax; ++idxCnt, ++Triangles)
       {
           *Indices++ = (u16) Triangles->vIndex[2];
           *Indices++ = (u16) Triangles->vIndex[1];
           *Indices++ = (u16) Triangles->vIndex[0];
       }

       S->original();

    // New surfaces offset.
       offset += MeshH->Size;
    } // End running surfaces...

// Ok!
   return true;
}

bool loadFile(io::IReadFile* file, CQ3ModelMesh* mesh, u32 firstFrame, u32 totalFrames)
{
 // Start loading mesh.
    os::Printer::log ("Loading MD3 mesh", file->getFileName(), ELL_INFORMATION);

 // Temporal memory pool.
    core::Pool *pool;

    pool = new core::Pool( file->getSize() );
    if (pool == NULL || pool->buffer() == NULL) {
        os::Printer::log ("Out of memory", ELL_ERROR);
        if (pool)
           delete pool;
        return false;
    }

 // Load all byte from file.
    s32 readed;
    if ( (readed = file->read(pool->buffer(), pool->size())) <= 0 || pool->size() != ((u32)readed) ) {
        os::Printer::log("I/O error to read file", ELL_ERROR);
        delete pool;
        return false;
    }

 // Loading...
    bool invalid;

    invalid = true;
    while ( invalid )
    {
        SMD3_Header* Header;

        if ( ! loadHeader(pool, mesh, &Header, totalFrames) )
            break;

        if ( ! loadBoneFrames(pool, mesh, Header, firstFrame) )
            break;

        if ( ! loadTags(pool, mesh, Header, firstFrame) )
            break;

        if ( ! loadSurfaces(pool, mesh, Header, firstFrame) )
            break;

        invalid = false;
    }

 // Erase pool.
    delete pool;

 // Checking result.
    if (invalid)
        mesh->clear();

 // End.
    return ( ! invalid );
}

//! Constructor
CQ3ModelMeshFileLoader::CQ3ModelMeshFileLoader(io::IFileSystem* fs, video::IVideoDriver* driver)
: FileSystem(fs), Driver(driver)
{
	if (FileSystem)
		FileSystem->grab();

	if (Driver)
		Driver->grab();
}

//! destructor
CQ3ModelMeshFileLoader::~CQ3ModelMeshFileLoader()
{
	if (FileSystem)
		FileSystem->drop();

	if (Driver)
		Driver->drop();
}

//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool CQ3ModelMeshFileLoader::isALoadableFileExtension (const c8* filename) const
{
	return (strstr(filename, ".md3"));
}

IAnimatedMesh* CQ3ModelMeshFileLoader::createMesh(io::IReadFile* file, u32 firstFrame, u32 totalFrames)
{
	IAnimatedMesh* result;
	result = NULL;

 // File is valid?
	if (file && file->seek(0))
	{
	    CQ3ModelMesh* mesh;
	    mesh = new CQ3ModelMesh();
	    if (mesh) {
	        if (loadFile(file, mesh, firstFrame, totalFrames)) {
	            mesh->setModelType( calculateTypeOfModel(file) );
	            result = (IAnimatedMesh*)mesh;
	        }
	        else
	            mesh->drop();
	    }
	    else
	       os::Printer::log("Out of memory", ELL_ERROR);
	}
	else
	{
	    if (file)
		    os::Printer::log("File for MD3 mesh with invalid state", ELL_ERROR);
        else
            os::Printer::log("File for MD3 mesh isn't open", ELL_ERROR);
		return false;
	}

	return result;
}

void CQ3ModelMeshFileLoader::showInformation(irr::io::IReadFile* file)
{
 // File is valid?
	if (file && file->seek(0))
	{
	    os::Printer::log ("MD3 file", file->getFileName(), ELL_INFORMATION);
	    os::Printer::log ("===============================================", ELL_INFORMATION);
	    switch(calculateTypeOfModel(file) )
	    {
	        case MD3_UNKNOWN: os::Printer::log ("Type", "Unknown", ELL_INFORMATION); break;
	        case MD3_HEAD   : os::Printer::log ("Type", "Head",    ELL_INFORMATION); break;
	        case MD3_UPPER  : os::Printer::log ("Type", "Upper",   ELL_INFORMATION); break;
	        case MD3_LOWER  : os::Printer::log ("Type", "Lower",   ELL_INFORMATION); break;
	        case MD3_WEAPON : os::Printer::log ("Type", "Weapon",  ELL_INFORMATION); break;
	    }

	    loadFile(file, NULL, 0, 0);
	}
	else
	{
	    if (file)
		    os::Printer::log("File for MD3 mesh with invalid state", ELL_ERROR);
        else
            os::Printer::log("File for MD3 mesh isn't open", ELL_ERROR);
	}
}



} // end namespace scene
} // end namespace irr

