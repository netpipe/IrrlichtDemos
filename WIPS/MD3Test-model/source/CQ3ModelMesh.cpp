// Copyright (C) 2003-2004 Fabio Concas
//  as an extension of the "Irrlicht Engine"
//  which is Copyright (C) 2002-2003 Nikolaus Gebhardt

#include <SColor.h>
#include <os.h>
#include "IReadFile.h"
#include <string.h>
#include "CQ3ModelMesh.h"

namespace irr
{
namespace scene
{

#define NODRAW "textures/common/nodraw"

//! Constructor
CQ3ModelMesh::CQ3ModelMesh () :
    FrameCount (0), Frame (0), TagCount (0), TagIndex(0), Tags(NULL)
{
	#ifdef _DEBUG
	this->IAnimatedMesh::setDebugName("CQ3ModelMesh IAnimatedMesh");
	this->IMesh::setDebugName("CQ3ModelMesh IMesh");
	#endif
}

//! Destructor
CQ3ModelMesh::~CQ3ModelMesh()
{
    clear();
}

void CQ3ModelMesh::clear()
{
    MeshName = "";
    FrameCount = 0;
    Frame = 0;
    BoxList.clear();
    TagCount = 0;
    TagIndex = 0;

    if (Tags) {
       delete[] Tags;
       Tags = NULL;
    }

    Surfaces.clear();
}

void CQ3ModelMesh::setFrameCount(u32 frameCount)
{
    FrameCount = frameCount;
	BoxList.clear();
	BoxList.reallocate(FrameCount);
}

void CQ3ModelMesh::setTagCount(u32 tagCount)
{
    if (Tags != NULL)
    {
        delete [] Tags;
		Tags = NULL;
		TagCount = 0;
		TagIndex = 0;
	}

	Tags = new tags_t[tagCount];

	if (Tags != NULL)
	{
	    TagCount = tagCount;
        u32 i;
        for (i = 0; i < TagCount; i++)
        {
            Tags[i].Name = "";
            Tags[i].Data.reallocate(FrameCount);
        }
    }
}

void CQ3ModelMesh::addTag(const c8 * name, const tag_t & tag)
{
    if (TagCount == 0)
       return;

	core::stringc tname;
	tname = name;
	tname.make_lower();

    if (TagCount == 1)
    {
        if (Tags[0].Data.size() == 0)
           Tags[0].Name = tname;
        Tags[0].Data.push_back(tag);
        return;
    }

    if (Tags[TagIndex].Name == tname)
    {
        Tags[TagIndex].Data.push_back(tag);
        return;
    }

    TagIndex = 0;
    for (TagIndex = 0; TagIndex < TagCount; TagIndex++)
    {
        if (Tags[TagIndex].Name == tname)
        {
            Tags[TagIndex].Data.push_back(tag);
            return;
        }

        if (Tags[TagIndex].Name == "")
        {
            Tags[TagIndex].Name = tname;
            Tags[TagIndex].Data.push_back(tag);
            return;
        }
    }
}

void CQ3ModelMesh::setSurfacesCount(u32 surfacesCount)
{
    Surfaces.clear();
    Surfaces.reallocate(surfacesCount);
    if (Surfaces.pointer() == NULL)
       Surfaces.clear();
}

CQ3Surface* CQ3ModelMesh::newSurface(const c8 * name, u32 numIndices, u32 numVertices)
{
    Surfaces.push_back( new CQ3Surface(name, &BoxList, &Frame, numIndices, numVertices) );
    return Surfaces[Surfaces.size()-1];
}


} // end namespace scene
} // end namespace irr

