// Copyright (C) 2002-2005 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h
//
// Modified by Michael Zeilfelder to allow loading a modified obj format which has 2 textures (one for lightmap)

#include "CLMOMeshFileLoader.h"
#include "logging.h"
#include "fast_atof.h"
#include "IVideoDriver.h"
#include "SMesh.h"
#include <SMeshBuffer.h>
#include <SAnimatedMesh.h>
#include "IReadFile.h"
#include "IAttributes.h"
#include <ctype.h>
#include <string.h>
#include <assert.h>

namespace irr
{
namespace scene
{

static const u32 WORD_BUFFER_LENGTH = 512;

//! Constructor
CLMOMeshFileLoader::CLMOMeshFileLoader(scene::ISceneManager* smgr, io::IFileSystem* fs, ITimer* timer)
: SceneManager(smgr), FileSystem(fs), Timer(timer), Mesh(0), mIsLightmap(false)
{
	if (FileSystem)
		FileSystem->grab();
}



//! destructor
CLMOMeshFileLoader::~CLMOMeshFileLoader()
{
	if (FileSystem)
		FileSystem->drop();

	if (Mesh)
		Mesh->drop();
}



//! returns true if the file maybe is able to be loaded by this class
//! based on the file extension (e.g. ".bsp")
bool CLMOMeshFileLoader::isALoadableFileExtension(const io::path& filename) const
{
	return core::hasFileExtension ( filename, "obj" ) || core::hasFileExtension ( filename, "lmo" );
}



//! creates/loads an animated mesh from the file.
//! \return Pointer to the created mesh. Returns 0 if loading failed.
//! If you no longer need the mesh, you should call IAnimatedMesh::drop().
//! See IReferenceCounted::drop() for more information.
IAnimatedMesh* CLMOMeshFileLoader::createMesh(io::IReadFile* file)
{
	if (!file)
		return 0;

#if IRR_SVN>=4710
	if ( getMeshTextureLoader() )
		getMeshTextureLoader()->setMeshFile(file);
#endif

	u32 beginTime = Timer->getRealTime();

	if (core::hasFileExtension ( file->getFileName(), "lmo" ) )
    {
        LOG.Debug("using lightmap\n");
        mIsLightmap = true;
    }
    else
    {
        mIsLightmap = false;
    }

	const long filesize = file->getSize();
	if (!filesize)
		return 0;

	if (Mesh)
	{
		Mesh->drop();
		Mesh = NULL;
	}
    Mesh = new SMesh();

	core::array<core::vector3df> vertexBuffer;
	core::array<core::vector3df> normalsBuffer;
	core::array<core::vector2df> textureCoordBuffer;
	SObjGroup * pCurrGroup = 0;
	SObjMtl * pCurrMtl = 0;
	u32 smoothingGroup=0;

    std::string infoStr("createMesh: ");
    infoStr += core::stringc(file->getFileName()).c_str();
    infoStr += "\n";
    LOG.Debug(infoStr);

	// ********************************************************************
	// Patch to locate the file in the same folder as the .obj.
	// If you load the file as "data/some.obj" and mtllib contains
	// "mtlname test.mtl" (as usual), the loading will fail. Instead it
	// must look for data/test.tml. This patch does exactly that.
	//
	// patch by mandrav@codeblocks.org
	// ********************************************************************
	core::stringc obj_fullname = file->getFileName();
	core::stringc obj_relpath = "";
	s32 pathend = obj_fullname.findLast('/');
	if (pathend == -1)
		pathend = obj_fullname.findLast('\\');
	if (pathend != -1)
		obj_relpath = obj_fullname.subString(0, pathend + 1);
	// ********************************************************************
	// end of mtl folder patch
	// ********************************************************************

	c8* pBuf = new c8[filesize];
	file->read((void*)pBuf, filesize);
	const c8* const bufEnd = pBuf+filesize;
	LOG.DebugLn("Time file-read: ", core::stringc(Timer->getRealTime() - beginTime).c_str());

	// Process obj information
	const c8* bufPtr = pBuf;
    while(bufPtr && (bufPtr-pBuf<filesize))
	{
		switch(bufPtr[0])
		{
        case 'l':   // lmo - lightmap texture. Should belong in material and is only here, cuz i'm lazy
        {
        	c8 wordBuffer[WORD_BUFFER_LENGTH];
            bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
            mLightmapTexture = wordBuffer;
            break;
        }

		case 'm':	// mtllib (material)
		{
			c8 name[WORD_BUFFER_LENGTH];
			bufPtr = goAndCopyNextWord(name, bufPtr, WORD_BUFFER_LENGTH, bufEnd);

			readMTL(name, obj_relpath);
			LOG.DebugLn("Time read mtl: ", core::stringc(Timer->getRealTime() - beginTime).c_str());
			break;
		}

		case 'v':               // v, vn, vt
			switch(bufPtr[1])
			{
			case ' ':          // vertex
				{
					core::vector3df vec;
					bufPtr = readVec3(bufPtr, vec, bufEnd);
					vertexBuffer.push_back(vec);
				}
				break;

			case 'n':           // normal
				{
					core::vector3df vec;
					bufPtr = readVec3(bufPtr, vec, bufEnd);
					normalsBuffer.push_back(vec);
				}
				break;

			case 't':           // texcoord
				{
					core::vector2df vec;
					bufPtr = readUV(bufPtr, vec, bufEnd);
					textureCoordBuffer.push_back(vec);
				}
				break;

			default:
				break;
			}
			break;

		case 'g':               // group
			// get name of group
			{
				c8 grp[WORD_BUFFER_LENGTH];
				c8 lineBuffer[WORD_BUFFER_LENGTH];
				copyLine(lineBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
				goAndCopyNextWord(grp, lineBuffer, WORD_BUFFER_LENGTH, bufEnd);

				pCurrGroup = findOrAddGroup(grp);
				if ( 0 != pCurrMtl )
				{
					// Set meshbuffer's material to current active material
					if ( mIsLightmap )
					{
					    pCurrGroup->MeshbufferLM->Material = pCurrMtl->Material;
					}
					else
					{
                        pCurrGroup->Meshbuffer->Material = pCurrMtl->Material;
					}
				}
			}
			break;

		case 's': // smoothing can be a group or off (equiv. to 0)
			{
				c8 smooth[WORD_BUFFER_LENGTH];
				bufPtr = goAndCopyNextWord(smooth, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
#ifdef _IRR_DEBUG_OBJ_LOADER_
	os::Printer::log("Loaded smoothing group start",smooth);
#endif
				if (core::stringc("off")==smooth)
					smoothingGroup=0;
				else
					smoothingGroup=core::strtol10(smooth, 0);
			}
			break;

		case 'u':				// usemtl
			if ( 0 != pCurrGroup )
			{
				c8 matName[WORD_BUFFER_LENGTH];
				matName[0] = 0;
				// if there's a group being built up now,
				// get name of material
				bufPtr = goAndCopyNextWord(matName, bufPtr, WORD_BUFFER_LENGTH, bufEnd);

				// create a new group
				core::stringc groupNew = pCurrGroup->name;
				groupNew.append(matName);
				pCurrGroup = findOrAddGroup(matName);

				// retrieve the material
				pCurrMtl = findMtl( matName );
				if ( pCurrMtl )
				{
					// if material is present, assign it to current meshbuffer
					if ( mIsLightmap )
					{
					    pCurrGroup->MeshbufferLM->Material = pCurrMtl->Material;
					}
					else
					{
                        pCurrGroup->Meshbuffer->Material = pCurrMtl->Material;
					}
				}
				else
				{
				    if ( matName[0] )
				    {
				        LOG.InfoLn("could not find material", matName);
				    }
                    else
                    {
                        LOG.Info("missing material name\n");
                    }
                }
			}
			break;

		case 'f':               // face
			if ( pCurrGroup )
			{
			    core::array<s32> vertexIndices;
				c8 vertexWord[WORD_BUFFER_LENGTH];	// for retriving vertex data
				video::S3DVertex v;
				v.Color = video::SColor(0xffffffff);
				video::S3DVertex2TCoords v2t;
				v2t.Color = video::SColor(0xffffffff);
				s32 currentVertexCount = mIsLightmap ? pCurrGroup->MeshbufferLM->Vertices.size() : pCurrGroup->Meshbuffer->Vertices.size();
				u32 facePointCount = 0;	// number of vertices in this face

				// Assign vertex colour to the current active material's diffuse colour
				if ( pCurrMtl)
				{
				    if ( mIsLightmap )
                        v2t.Color = pCurrMtl->Material.DiffuseColor;
                    else
                        v.Color =  pCurrMtl->Material.DiffuseColor;
				}

				// get all vertices data in this face (current line of obj file)
				c8 faceBuffer[WORD_BUFFER_LENGTH];
				copyLine(faceBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
				const c8 * pLinePtr = faceBuffer;

				// read in all vertices
				pLinePtr = goNextWord(pLinePtr, bufEnd);
				while (0 != pLinePtr[0])
				{
				    // Array to communicate with retrieveVertexIndices()
                    // sends the buffer sizes and gets the actual indices
                    // if index not set returns -1
                    s32 Idx[4];
                    Idx[0]=vertexBuffer.size();
                    Idx[1]=textureCoordBuffer.size();
                    Idx[2]=normalsBuffer.size();
                    Idx[3]=-1;

					// read in next vertex's data
					copyWord(vertexWord, pLinePtr, WORD_BUFFER_LENGTH, bufEnd);
					// this function will also convert obj's 1-based index to c++'s 0-based index
					retrieveVertexIndices(vertexWord, Idx, bufEnd);
					if ( mIsLightmap )
					{
                        if ( -1 != Idx[0] )
                        {
                            v2t.Pos = vertexBuffer[Idx[0]];
                        }
                        if ( -1 != Idx[1] )
                        {
                            v2t.TCoords = textureCoordBuffer[Idx[1]];
                        }
                        if ( -1 != Idx[2] )
                        {
                            v2t.Normal = normalsBuffer[Idx[2]];
                        }
                        if ( -1 != Idx[3] )
                        {
                            v2t.TCoords2 = textureCoordBuffer[Idx[3]];
                        }

                        pCurrGroup->MeshbufferLM->Vertices.push_back(v2t);
                    }
					else
					{
                        if ( -1 != Idx[0] )
                        {
                            v.Pos = vertexBuffer[Idx[0]];
                        }
                        if ( -1 != Idx[1] )
                        {
                            v.TCoords = textureCoordBuffer[Idx[1]];
                        }
                        if ( -1 != Idx[2] )
                        {
                            v.Normal = normalsBuffer[Idx[2]];
                        }

                        pCurrGroup->Meshbuffer->Vertices.push_back(v);
					}
					++facePointCount;

					// go to next vertex
					pLinePtr = goNextWord(pLinePtr, bufEnd);
				}

                core::array<u16> * indices = NULL;
                if ( mIsLightmap )
                {
                    indices = &(pCurrGroup->MeshbufferLM->Indices);
                }
                else
                {
                    indices = &(pCurrGroup->Meshbuffer->Indices);
                }

//                // Add indices for first 3 vertices
                indices->push_back( currentVertexCount );
                indices->push_back( ( facePointCount - 1 ) + currentVertexCount );
                indices->push_back( ( facePointCount - 2 ) + currentVertexCount );
                // Add indices for subsequent vertices
                for ( u32 i = 0; i < facePointCount - 3; ++i )
                {
                    indices->push_back( currentVertexCount );
                    indices->push_back( ( facePointCount - 2 - i ) + currentVertexCount );
                    indices->push_back( ( facePointCount - 3 - i ) + currentVertexCount );
                }
			}

			break;
        case '#':               // comment
		default:
			break;
		}	// end switch(bufPtr[0])
        bufPtr = goNextLine(bufPtr, bufEnd);
	}	// end while(bufPtr && (bufPtr-pBuf<filesize))

	// Combine all the groups (meshbuffers) into the mesh
	for ( u32 m = 0; m < groups.size(); m++ )
	{
	    LOG.DebugLn("group: ", groups[m]->name.c_str());
	    if ( mIsLightmap )
	    {
            if ( groups[m]->MeshbufferLM->getIndexCount() > 0 )
            {
//                fprintf(stderr, "indices: %d vertices: %d\n", groups[m]->MeshbufferLM->getIndexCount(), groups[m]->MeshbufferLM->getVertexCount() );

                groups[m]->MeshbufferLM->recalculateBoundingBox();
                Mesh->addMeshBuffer( groups[m]->MeshbufferLM );
            }
	    }
	    else
	    {
            if ( groups[m]->Meshbuffer->getIndexCount() > 0 )
            {
  //              fprintf(stderr, "indices: %d vertices: %d\n", groups[m]->Meshbuffer->getIndexCount(), groups[m]->Meshbuffer->getVertexCount() );

                groups[m]->Meshbuffer->recalculateBoundingBox();
                Mesh->addMeshBuffer( groups[m]->Meshbuffer );
            }
	    }
	}
	LOG.DebugLn("Time obj read&parsed: ", core::stringc(Timer->getRealTime() - beginTime).c_str());

	// Create the Animated mesh if there's anything in the mesh
	SAnimatedMesh* animMesh = 0;
    if ( 0 != Mesh->getMeshBufferCount() )
    {
        Mesh->recalculateBoundingBox();
        animMesh = new SAnimatedMesh();
        animMesh->Type = EAMT_OBJ;
        animMesh->addMesh(Mesh);
        animMesh->recalculateBoundingBox();
    }

	// Clean up the allocate obj file contents
	delete [] pBuf;
	// more cleaning up
	cleanUp();

	if ( Mesh )
	{
        Mesh->drop();
        Mesh = 0;
	}

	LOG.DebugLn("Time CLMOMeshFileLoader::createMesh sum: ", core::stringc(Timer->getRealTime() - beginTime).c_str());

    return animMesh;
}

const c8* CLMOMeshFileLoader::readTextures(const c8* bufPtr, const c8* const bufEnd, SObjMtl* currMaterial, const io::path& relPath)
{
	u8 type=0; // map_Kd - diffuse color texture map
	// map_Ks - specular color texture map
	// map_Ka - ambient color texture map
	if ((!strncmp(bufPtr,"map_bump",8)) || (!strncmp(bufPtr,"bump",4)))
		type=1; // normal map
	else if (!strncmp(bufPtr,"map_d",5))
		type=2; // opactity map
	else if (!strncmp(bufPtr,"map_refl",8))
		type=3; // reflection map
	// extract new material's name
	c8 textureNameBuf[WORD_BUFFER_LENGTH];
	bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);

	//f32 bumpiness = 6.0f;
	bool clamp = false;
	// handle options
	while (textureNameBuf[0]=='-')
	{
		if (!strncmp(bufPtr,"-bm",3))
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			currMaterial->Material.MaterialTypeParam=core::fast_atof(textureNameBuf);
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			continue;
		}
		else
		if (!strncmp(bufPtr,"-blendu",7))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-blendv",7))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-cc",3))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-clamp",6))
			bufPtr = readBool(bufPtr, clamp, bufEnd);
		else
		if (!strncmp(bufPtr,"-texres",7))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-type",5))
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		else
		if (!strncmp(bufPtr,"-mm",3))
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
		}
		else
		if (!strncmp(bufPtr,"-o",2)) // texture coord translation
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			// next parameters are optional, so skip rest of loop if no number is found
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!core::isdigit(textureNameBuf[0]))
				continue;
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!core::isdigit(textureNameBuf[0]))
				continue;
		}
		else
		if (!strncmp(bufPtr,"-s",2)) // texture coord scale
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			// next parameters are optional, so skip rest of loop if no number is found
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!core::isdigit(textureNameBuf[0]))
				continue;
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!core::isdigit(textureNameBuf[0]))
				continue;
		}
		else
		if (!strncmp(bufPtr,"-t",2))
		{
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			// next parameters are optional, so skip rest of loop if no number is found
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!core::isdigit(textureNameBuf[0]))
				continue;
			bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
			if (!core::isdigit(textureNameBuf[0]))
				continue;
		}
		// get next word
		bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	}

	if ((type==1) && (core::isdigit(textureNameBuf[0])))
	{
		currMaterial->Material.MaterialTypeParam=core::fast_atof(textureNameBuf);
		bufPtr = goAndCopyNextWord(textureNameBuf, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	}
	if (clamp)
		currMaterial->Material.setFlag(video::EMF_TEXTURE_WRAP, video::ETC_CLAMP);

	video::ITexture * texture = getTexture(textureNameBuf, relPath, true);
	if ( texture )
	{
		if (type==0)
			currMaterial->Material.setTexture(0, texture);
		else if (type==1 && !mIsLightmap)
		{
			// TODO: never got that working
//			SceneManager->getVideoDriver()->makeNormalMapTexture(texture, bumpiness);
//			currMaterial->Material.setTexture(1, texture);
//			currMaterial->Material.MaterialType=video::EMT_PARALLAX_MAP_SOLID;
//			currMaterial->Material.MaterialTypeParam=0.035f;
		}
		else if (type==2)
		{
			currMaterial->Material.setTexture(0, texture);
			if ( !mIsLightmap )
			{
				currMaterial->Material.MaterialType=video::EMT_TRANSPARENT_ADD_COLOR;
			}
		}
		else if (type==3)
		{
//						currMaterial->Material.Textures[1] = texture;
//						currMaterial->Material.MaterialType=video::EMT_REFLECTION_2_LAYER;
		}
		// Set diffuse material colour to white so as not to affect texture colour
		// Because Maya set diffuse colour Kd to black when you use a diffuse colour map
		// But is this the right thing to do?
		currMaterial->Material.DiffuseColor.set(
			currMaterial->Material.DiffuseColor.getAlpha(), 255, 255, 255 );
	}
	return bufPtr;
}

void CLMOMeshFileLoader::readMTL(const c8* fileName, const io::path& relPath)
{
    std::string strInfo("read material ");
    strInfo += fileName;
    strInfo += "\n";
    LOG.Debug(strInfo);

	io::path realFile ( fileName );

	const s32 WORD_BUFFER_LENGTH = 255;

	io::IReadFile * pMtlReader;
	if (FileSystem->existFile(realFile))
		pMtlReader = FileSystem->createAndOpenFile(realFile.c_str());
	else
	{
		// try to read in the relative path, the .obj is loaded from
		io::path r2 = relPath + realFile;
		pMtlReader = FileSystem->createAndOpenFile(r2.c_str());
	}
	if (!pMtlReader)	// fail to open and read file
		return;

#if IRR_SVN>=4710
	if ( getMeshTextureLoader() )
	{
		getMeshTextureLoader()->setMaterialFile(pMtlReader);
		if ( SceneManager->getParameters()->existsAttribute(OBJ_TEXTURE_PATH) )
			getMeshTextureLoader()->setTexturePath(SceneManager->getParameters()->getAttributeAsString(OBJ_TEXTURE_PATH));
	}
#endif

	const long filesize = pMtlReader->getSize();
	if (!filesize)
		return;

	c8* pBuf = new c8[filesize];
	pMtlReader->read((void*)pBuf, filesize);
	const c8* pBufEnd = pBuf+filesize;

	SObjMtl * pCurrMaterial = 0;

	const c8* pBufPtr = pBuf;
	while(pBufPtr && (pBufPtr-pBuf<filesize))
	{
		switch(pBufPtr[0])
		{
        case 'n':               // newmtl
			{
				// if there's an existing material, store it first
				if ( pCurrMaterial )
				{
					materials.push_back( *pCurrMaterial );
					delete pCurrMaterial;
				}

				// extract new material's name
				c8 mtlNameBuf[WORD_BUFFER_LENGTH];
				pBufPtr = goAndCopyNextWord(mtlNameBuf, pBufPtr, WORD_BUFFER_LENGTH, pBufEnd);

				pCurrMaterial = new SObjMtl;

				pCurrMaterial->name = mtlNameBuf;
//				pCurrMaterial->material.AmbientColor = video::SColor(255, (s32)(0.2*255.f), (s32)(0.2*255.f), (s32)(0.2*255.f));
				pCurrMaterial->Material.AmbientColor = video::SColor(255, 255, 255, 255);
//				pCurrMaterial->Material.DiffuseColor = video::SColor(255, (s32)(0.8*255.f), (s32)(0.8*255.f), (s32)(0.8*255.f));
                pCurrMaterial->Material.DiffuseColor = video::SColor(255, 255, 255, 255);
				pCurrMaterial->Material.SpecularColor = video::SColor(255,0,0,0);
				pCurrMaterial->Material.Shininess = 20.f;
                pCurrMaterial->Material.MaterialTypeParam = 0.02f;  // just a value that works for me
                pCurrMaterial->Material.EmissiveColor = video::SColor(255,0,0,0);  // this should be the correct value

                if ( mIsLightmap )
                {
                    core::stringc dummy(mLightmapTexture);
                    pCurrMaterial->Material.setTexture(1, getTexture(mLightmapTexture.c_str(), relPath, true) );
                    pCurrMaterial->Material.MaterialType = video::EMT_LIGHTMAP_LIGHTING_M2;

//                    pCurrMaterial->Material.MaterialType = video::EMT_LIGHTMAP;
//                    pCurrMaterial->Material.MaterialType = video::EMT_LIGHTMAP_ADD;
//                    pCurrMaterial->Material.MaterialType = video::EMT_LIGHTMAP_M2;
//                    pCurrMaterial->Material.MaterialType = video::EMT_LIGHTMAP_M4;
//                    pCurrMaterial->Material.MaterialType = video::EMT_LIGHTMAP_LIGHTING;
//                    pCurrMaterial->Material.MaterialType = video::EMT_LIGHTMAP_LIGHTING_M2;
//                    pCurrMaterial->Material.MaterialType = video::EMT_LIGHTMAP_LIGHTING_M4;
                }
			}
            break;
        case 'i':		// illum - illuminationSRC_ALPHA
            if ( pCurrMaterial )
            {
                const u32 COLOR_BUFFER_LENGTH = 16;
                c8 illumStr[COLOR_BUFFER_LENGTH];

                pBufPtr = goAndCopyNextWord(illumStr, pBufPtr, COLOR_BUFFER_LENGTH, pBufEnd);
                pCurrMaterial->illumination = (c8)atol(illumStr);
            }
            break;

        case 'N':
			if ( pCurrMaterial )
			{
			    switch(pBufPtr[1])
			    {
			        case 's': // Ns - shininess
			        {
                        const u32 COLOR_BUFFER_LENGTH = 16;
                        c8 nsStr[COLOR_BUFFER_LENGTH];

                        pBufPtr = goAndCopyNextWord(nsStr, pBufPtr, COLOR_BUFFER_LENGTH, pBufEnd);
                        f32 shininessValue = core::fast_atof(nsStr);

                        // wavefront shininess is from [0, 1000], so scale for OpenGL
                        shininessValue *= 0.128f;
                        pCurrMaterial->Material.Shininess = shininessValue;
                        break;
			        }
				    case 'i': // Ni - refraction index
					{
						c8 tmpbuf[WORD_BUFFER_LENGTH];
						pBufPtr = goAndCopyNextWord(tmpbuf, pBufPtr, WORD_BUFFER_LENGTH, pBufEnd);
					}
				break;
			    }
			}

            break;
        case 'K':
            if ( pCurrMaterial )
			{
				switch(pBufPtr[1])
				{
				case 'd':		// Kd = diffuse
					{
					    if ( pCurrMaterial->illumination != 4 )
                            pBufPtr = readColor(pBufPtr, pCurrMaterial->Material.DiffuseColor, pBufEnd);
					}
					break;

				case 's':		// Ks = specular
					{
						pBufPtr = readColor(pBufPtr, pCurrMaterial->Material.SpecularColor, pBufEnd);
					}
					break;

				case 'a':		// Ka = ambience
					{
					    video::SColor col;
					    if ( pCurrMaterial->illumination == 4 )
					    {
					        pBufPtr=readColor(pBufPtr, col, pBufEnd);
					        if ( col.getRed() != 0 || col.getGreen() != 0 || col.getBlue() != 0 )
					        {
					            pCurrMaterial->Material.AmbientColor = video::SColor(255,0,0,0);
					            pCurrMaterial->Material.EmissiveColor = col;
					        }
					    }
                      else
                        {
                            pBufPtr=readColor(pBufPtr, pCurrMaterial->Material.AmbientColor, pBufEnd);
                        }
					}
					break;
				case 'e':		// Ke = emissive
					{
						pBufPtr=readColor(pBufPtr, pCurrMaterial->Material.EmissiveColor, pBufEnd);
					}
					break;

				default:
					break;
				}	// end switch(pBufPtr[1])
			}	// end case 'K': if ( 0 != pCurrMaterial )...
            break;

        case 'b':           // bump
		case 'm':			// map_Kd - diffuse texture map
			if ( pCurrMaterial )
			{
				pBufPtr=readTextures(pBufPtr, pBufEnd, pCurrMaterial, relPath);
			}
			break;

        case 'd':				// d - transparency
			if ( 0 != pCurrMaterial )
			{
				const u32 COLOR_BUFFER_LENGTH = 16;
				c8 dStr[COLOR_BUFFER_LENGTH];

				pBufPtr = goAndCopyNextWord(dStr, pBufPtr, COLOR_BUFFER_LENGTH, pBufEnd);
				f32 dValue = core::fast_atof(dStr);

				pCurrMaterial->Material.DiffuseColor.setAlpha( (s32)(dValue * 255) );
				if (dValue<1.0f && !mIsLightmap)
                    pCurrMaterial->Material.MaterialType = irr::video::EMT_TRANSPARENT_VERTEX_ALPHA;
			}

            break;

		case 'T':
			if ( pCurrMaterial )
			{
				switch ( pBufPtr[1] )
				{
				case 'f':		// Tf - Transmitivity
					const s32 COLOR_BUFFER_LENGTH = 16;
					c8 redStr[COLOR_BUFFER_LENGTH];
					c8 greenStr[COLOR_BUFFER_LENGTH];
					c8 blueStr[COLOR_BUFFER_LENGTH];

					pBufPtr = goAndCopyNextWord(redStr,   pBufPtr, COLOR_BUFFER_LENGTH, pBufEnd);
					pBufPtr = goAndCopyNextWord(greenStr, pBufPtr, COLOR_BUFFER_LENGTH, pBufEnd);
					pBufPtr = goAndCopyNextWord(blueStr,  pBufPtr, COLOR_BUFFER_LENGTH, pBufEnd);

					f32 transparency = ( core::fast_atof(redStr) + core::fast_atof(greenStr) + core::fast_atof(blueStr) ) / 3;

					pCurrMaterial->Material.DiffuseColor.setAlpha( (s32)(transparency * 255) );
					if (transparency < 1.0f)
						pCurrMaterial->Material.MaterialType = video::EMT_TRANSPARENT_VERTEX_ALPHA;	// TODO: needs testing - this didn't work yet in h-craft
				}
			}

            break;

        case 'A': // (alphablending), NOT an original .mtl parameter, but useful in Irrlicht so i added it to the format
            {
                const s32 VALUE_BUFFER_LENGTH = 16;
                c8 aStr[VALUE_BUFFER_LENGTH];

                pBufPtr = goAndCopyNextWord(aStr, pBufPtr, VALUE_BUFFER_LENGTH, pBufEnd);
                pCurrMaterial->Material.MaterialTypeParam = core::fast_atof(aStr);
            }
            break;

        default:               // comments or not recognised
			break;
		}	// end switch(pBufPtr[0])
        pBufPtr = goNextLine(pBufPtr, pBufEnd);
	}	// end while (pBufPtr)

	// end of file. if there's an existing material, store it
	if ( pCurrMaterial )
	{
		materials.push_back( *pCurrMaterial );
		delete pCurrMaterial;
		pCurrMaterial = 0;
    }

	delete [] pBuf;
	pMtlReader->drop();
}

//! Read RGB color
const c8* CLMOMeshFileLoader::readColor(const c8* bufPtr, video::SColor& color, const c8* const bufEnd)
{
	const u32 COLOR_BUFFER_LENGTH = 16;
	c8 colStr[COLOR_BUFFER_LENGTH];

	color.setAlpha(255);
	bufPtr = goAndCopyNextWord(colStr, bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
	color.setRed((s32)(core::fast_atof(colStr) * 255.0f));
	bufPtr = goAndCopyNextWord(colStr,   bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
	color.setGreen((s32)(core::fast_atof(colStr) * 255.0f));
	bufPtr = goAndCopyNextWord(colStr,   bufPtr, COLOR_BUFFER_LENGTH, bufEnd);
	color.setBlue((s32)(core::fast_atof(colStr) * 255.0f));
	return bufPtr;
}


//! Read 3d vector of floats
const c8* CLMOMeshFileLoader::readVec3(const c8* bufPtr, core::vector3df& vec, const c8* const bufEnd)
{
	const u32 WORD_BUFFER_LENGTH = 256;
	c8 wordBuffer[WORD_BUFFER_LENGTH];

	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.X=-core::fast_atof(wordBuffer); // change handedness
	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.Y=core::fast_atof(wordBuffer);
	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.Z=core::fast_atof(wordBuffer);
	return bufPtr;
}


//! Read 2d vector of floats
const c8* CLMOMeshFileLoader::readUV(const c8* bufPtr, core::vector2df& vec, const c8* const bufEnd)
{
	const u32 WORD_BUFFER_LENGTH = 256;
	c8 wordBuffer[WORD_BUFFER_LENGTH];

	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.X=core::fast_atof(wordBuffer);
	bufPtr = goAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.Y=1-core::fast_atof(wordBuffer); // change handedness
	return bufPtr;
}


//! Read boolean value represented as 'on' or 'off'
const c8* CLMOMeshFileLoader::readBool(const c8* bufPtr, bool& tf, const c8* const bufEnd)
{
	const u32 BUFFER_LENGTH = 8;
	c8 tfStr[BUFFER_LENGTH];

	bufPtr = goAndCopyNextWord(tfStr, bufPtr, BUFFER_LENGTH, bufEnd);
	tf = strcmp(tfStr, "off") != 0;
	return bufPtr;
}

CLMOMeshFileLoader::SObjMtl * CLMOMeshFileLoader::findMtl(c8* pMtlName)
{
	for (u32 i = 0; i < materials.size(); ++i)
	{
		if ( materials[i].name == pMtlName )
		{
			return &materials[i];
		}
	}
	return 0;
}


CLMOMeshFileLoader::SObjGroup* CLMOMeshFileLoader::addGroup(c8* pGroupName)
{
	SObjGroup * group = new SObjGroup();
	group->name = pGroupName;
	if ( mIsLightmap )
	{
	    group->MeshbufferLM = new SMeshBufferLightMap();
	}
	else
	{
        group->Meshbuffer = new SMeshBuffer();
	}

	groups.push_back(group);
	return group;
}


CLMOMeshFileLoader::SObjGroup * CLMOMeshFileLoader::findGroup(c8* pGroupName)
{
	for (u32 i = 0; i < groups.size(); ++i)
	{
		if ( groups[i]->name == pGroupName )
		{
			return groups[i];
		}
	}
	return 0;
}


CLMOMeshFileLoader::SObjGroup * CLMOMeshFileLoader::findOrAddGroup(c8* pGroupName)
{
	SObjGroup * pGroup = findGroup( pGroupName );
	if ( 0 != pGroup )
	{
		// group found, return it
		return pGroup;
	}
	// group not found, create a new group
	return addGroup( pGroupName );
}


const c8* CLMOMeshFileLoader::goFirstWord(const c8* buf, const c8* pBufEnd)
{
	s32 i = 0;
	// skip pass non-printable characters
	while(buf[i] && (buf[i]==' ' || buf[i]=='\n' || buf[i]=='\r' || buf[i]=='\t'))
	{
		if ( &(buf[i]) == pBufEnd )
		{
			// end of buffer check
			return 0;
		}
		++i;
	}

	return &buf[i];
}


const c8* CLMOMeshFileLoader::goNextWord(const c8* buf, const c8* pBufEnd)
{
	if (!buf)
		return 0;

	s32 i = 0;
	// look for non-printable characters
	while(buf[i])
	{
		if ( &(buf[i]) == pBufEnd )
		{
			// end of buffer check
			return 0;
		}
		if (buf[i]==' ' || buf[i]=='\n' || buf[i]=='\r' || buf[i]=='\t')
			break;

		++i;
	}

	// look for printable character after non-printable character
	const c8* nextWord = goFirstWord(&buf[i], pBufEnd);
	// if can't find, return null
	return nextWord == buf ? 0 : nextWord;
}


const c8* CLMOMeshFileLoader::goNextLine(const c8* buf, const c8* pBufEnd)
{
    if ( !buf )
        return 0;
	// Have to use pointer to pointer in parameter or else string address won't be changed
	s32 i=0;
	// look for newline characters
	while(buf[i])
	{
		if ( &(buf[i]) == pBufEnd )
		{
			// end of buffer check
			return 0;
		}
		if (buf[i]=='\n' || buf[i]=='\r')
		{
			// found it, so skip pass it
			break;
		}

		++i;
	}
	do
	{
	    ++i;
	    if ( pBufEnd == buf+i )
	    {
	        // check for end of buffer
	        return (c8*)pBufEnd;
	    }
	}
	while ( isspace(buf[i]));

	return &buf[i];
}


void CLMOMeshFileLoader::copyWord(c8* outBuf, const c8* inBuf, s32 outBufLength, const c8* pBufEnd)
{
    if ( !outBuf )
        return;
	if (!inBuf )
	{
	    *outBuf = 0;
		return;
	}

	s32 i = 0;
	while(inBuf[i])
	{
		if (inBuf[i]==' ' || inBuf[i]=='\n' || inBuf[i]=='\r' || inBuf[i]=='\t' || &(inBuf[i]) == pBufEnd || i>=outBufLength)
			break;
        outBuf[i] = inBuf[i];
		++i;
	}

	if (i < outBufLength )
    {
        outBuf[i] = '\0';
    }
    else
    {
        LOG.ErrorLn("CLMOMeshFileLoader::copyWord, word too long for buffer", NULL);
        outBuf[outBufLength-1] = '\0';
    }
}


void CLMOMeshFileLoader::copyLine(c8* outBuf, const c8* inBuf, s32 outBufLength, const c8* pBufEnd)
{
    if ( !outBuf )
        return;
	if (!inBuf )
	{
	    *outBuf = 0;
		return;
	}

	s32 i = 0;
	while(inBuf[i])
	{
		if (inBuf[i]=='\n' || inBuf[i]=='\r' || &(inBuf[i]) == pBufEnd || i>=outBufLength )
			break;
        outBuf[i] = inBuf[i];
		++i;
	}

    if (i < outBufLength )
    {
        outBuf[i] = '\0';
    }
    else
    {
        LOG.ErrorLn("CLMOMeshFileLoader::copyLine, line too long for buffer", NULL);
        outBuf[outBufLength-1] = '\0';
    }
}


const c8* CLMOMeshFileLoader::goAndCopyNextWord(c8* outBuf, const c8* inBuf, s32 outBufLength, const c8* pBufEnd)
{
	inBuf = goNextWord(inBuf, pBufEnd);
	copyWord(outBuf, inBuf, outBufLength, pBufEnd);
	return inBuf;
}

bool CLMOMeshFileLoader::retrieveVertexIndices(c8* pVertexData, s32* pIdx, const c8* pBufEnd)
{
    #define VERT_NUM_BUF_SIZE 16
	c8 word[VERT_NUM_BUF_SIZE] = "";
	const c8* pChar = goFirstWord(pVertexData, pBufEnd);
	u32 idxType = 0;		// 0 = posIdx, 1 = texcoordIdx, 2 = normalIdx, 3 = texCoordIdx2 for .lmo files
	s32 index;

	u32 i = 0;
	while ( pChar != pBufEnd )
	{
		if ( (isdigit(*pChar)) || (*pChar == '-') )
		{
			// build up the number
			if ( i < VERT_NUM_BUF_SIZE-1 )  // -1 because adding 0 won't be enough anymore
			{
                word[i++] = *pChar;
			}
			else
			{
			    LOG.ErrorLn("CLMOMeshFileLoader::retrieveVertexIndices, VERT_NUM_BUF_SIZE too small", NULL);
			    return false;
			}
		}
		else if ( *pChar == '/' || *pChar == '\0' )
		{
			// number is completed. Convert and store it
			word[i] = '\0';
			// if no number was found index will become 0 and later on -1 by decrement
			index = atoi( word );
			if (index<0)
				index += pIdx[idxType];
			else
				--index;
			pIdx[idxType] = index;

			// reset the word
			word[0] = '\0';
			i = 0;

			// go to the next kind of index type
			if (*pChar == '/')
			{
			    ++idxType;
				if ( (!mIsLightmap && idxType > 2 ) || idxType > 3 )
				{
					// error checking, shouldn't reach here unless file is wrong
					idxType = 0;
				}
			}
			else if (*pChar == '\0')
			{
				// set all missing values to disable (=-1)
				while (++idxType < 3)
					pIdx[idxType]=-1;
                if ( mIsLightmap && idxType == 2 )
                {
                    idxType = 3;
                    pIdx[idxType]=-1;
                }

				++pChar;
				break; // while
			}
		}

		// go to the next char
		++pChar;
	}

	return true;
}

void CLMOMeshFileLoader::cleanUp()
{
	materials.clear();
	for (u32 i = 0; i < groups.size(); ++i )
	{
	    if ( groups[i]->Meshbuffer )
	    {
            groups[i]->Meshbuffer->drop();
            groups[i]->Meshbuffer = NULL;
	    }
	    if ( groups[i]->MeshbufferLM )
	    {
	        groups[i]->MeshbufferLM->drop();
            groups[i]->MeshbufferLM = NULL;
	    }
		delete groups[i];
	}
	groups.clear();
}

video::ITexture * CLMOMeshFileLoader::getTexture(const c8* textureName, const io::path& relPath, bool checkAlternatives)
{
	if ( !textureName || textureName[0] == 0)
		return NULL;

#if IRR_SVN>=4710
	if ( getMeshTextureLoader() )
	{
		video::ITexture * texture = getMeshTextureLoader()->getTexture(textureName);
		if ( !texture )
		{
			LOG.DebugLn("LMO failed loading texture: ", textureName);
		}
		return texture;
	}
	else
	{
#ifdef _IRR_ANDROID_PLATFORM_
		LOG.DebugLn("LMO Missing meshtextureloader (bad on Android, doesn't matter on PC) to load: ", textureName);
#endif
	}
#endif

	io::path texname(textureName);
	texname.replace('\\', '/');

	video::ITexture * texture = 0;
    io::path textureFromTexPath( SceneManager->getParameters()->getAttributeAsString(OBJ_TEXTURE_PATH) );
    if ( textureFromTexPath.size() )
    {
        textureFromTexPath += '/';
        textureFromTexPath += texname;
    }
	if ( FileSystem->existFile(textureFromTexPath) )
        texture = SceneManager->getVideoDriver()->getTexture( textureFromTexPath );
	else if (FileSystem->existFile(texname)	)
		texture = SceneManager->getVideoDriver()->getTexture(texname);
	else
	{
		// try to read in the relative path, the .obj is loaded from
		io::path relName(relPath + texname);
		if (FileSystem->existFile(relName))
			texture = SceneManager->getVideoDriver()->getTexture( relName );
	}

	if ( !texture && checkAlternatives )
	{
		// hack because lazy artist didn't want to fix materials when deciding to use another fileformat for textures.
		if ( core::hasFileExtension(texname, "jpg" ) )
		{
			u32 size = texname.size();
			texname[size-3] = 't';
			texname[size-2] = 'g';
			texname[size-1] = 'a';
			texture = getTexture(texname.c_str(), relPath, false);
		}
		else if ( core::hasFileExtension(texname, "tga") )
		{
			u32 size = texname.size();
			texname[size-3] = 'j';
			texname[size-2] = 'p';
			texname[size-1] = 'g';
			texture = getTexture(texname.c_str(), relPath, false);
		}
	}

	return texture;
}


} // end namespace scene
} // end namespace irr

