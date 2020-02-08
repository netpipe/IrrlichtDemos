// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "helper_irr.h"
#include "helper_math.h"
#include "globals.h"
#include <vector>
#include <algorithm>

using namespace irr;

namespace
{
	template <class TSortHelper>
	static bool SortByOldIndex(const TSortHelper& a, const TSortHelper&b )
	{
		return a.oldIndex < b.oldIndex;
	}

	template <class TSortHelper>
	static bool SortByHash(const TSortHelper& a, const TSortHelper&b )
	{
		return a.hash < b.hash;
	}

	template <class TSortHelper>
	void makeHash(TSortHelper& a)
	{
		// Ensure similar vertices have a similar hash (very different vertices can also have a similar hash, but similar vertices should not have very different hashes)
		a.hash =  a.vertex->Pos.X + a.vertex->Pos.Y + a.vertex->Pos.Z + a.vertex->Normal.X + a.vertex->Normal.Y + a.vertex->Normal.Z + a.vertex->TCoords.X + a.vertex->TCoords.Y;
	}

	struct SortVertex
	{
		SortVertex() : vertex(0), hash(0.f), oldIndex(0), newIndex(0), hasNewIndex(false) {}
		irr::video::S3DVertex* vertex;
		float hash;
		irr::u32 oldIndex;
		irr::u32 newIndex;
		bool hasNewIndex;
	};

	struct SortVertex2T
	{
		SortVertex2T() : vertex(0), hash(0.f), oldIndex(0), newIndex(0), hasNewIndex(false) {}
		irr::video::S3DVertex2TCoords* vertex;
		float hash;
		irr::u32 oldIndex;
		irr::u32 newIndex;
		bool hasNewIndex;
	};

	struct SortVertexTangents
	{
		SortVertexTangents() : vertex(0), hash(0.f), oldIndex(0), newIndex(0), hasNewIndex(false) {}
		irr::video::S3DVertexTangents* vertex;
		float hash;
		irr::u32 oldIndex;
		irr::u32 newIndex;
		bool hasNewIndex;
	};
};

namespace ExtIrr
{

// ok, i have no idea of this, but seems to work
void AlignToUpVector(irr::core::matrix4 &mat_, const irr::core::matrix4 &oldMat_, const irr::core::vector3df &newUp_, float interpolate_)
{
    SmoothAlignToVector(mat_, oldMat_, core::vector3df(0, 1, 0), newUp_, interpolate_);
}

void SmoothAlignToVector(irr::core::matrix4 &newMat_, const irr::core::matrix4 &oldMat_, const irr::core::vector3df &oldVec_, const irr::core::vector3df &newVec_, float interpolate_)
{
    core::vector3df oldVector(oldVec_);
    oldMat_.rotateVect(oldVector);
    float dot = oldVector.dotProduct(newVec_);
    core::vector3df axis( oldVector.crossProduct(newVec_) );
    core::quaternion quatTarget(axis.X, axis.Y, axis.Z, 1+dot);
    quatTarget.normalize();
    core::quaternion quatSource;
    core::quaternion quatRot;
    quatRot.slerp(quatSource, quatTarget, interpolate_);
    core::vector3df newRot;
    quatRot.toEuler(newRot);
    newRot *= core::RADTODEG;
    newMat_.setRotationDegrees(newRot);

    newMat_ = newMat_*oldMat_;
}

float GetClosestPointOnLineWithDelta(const irr::core::line3df &line_, const irr::core::vector3df &point_, irr::core::vector3df & target_)
{
    core::vector3df c(point_ - line_.start);
    core::vector3df v(line_.end - line_.start);
    float d = v.getLength();
    v /= d;
    float t = v.dotProduct(c);

    if (t < 0.0)
    {
        target_ = line_.start;
        return 0.f;
    }
    if (t > d)
    {
        target_ = line_.end;
        return 1.f;
    }

    v *= t;
    target_ = line_.start + v;

    return t/d;
}

void CalcYForPointOnLine(const irr::core::line3df &line_, irr::core::vector3df &point_)
{
    irr::core::vector3df startXZ(line_.start);
    irr::core::vector3df endXZ(line_.end);
    startXZ.Y = 0.f;
    endXZ.Y = 0.f;
    point_.Y = 0.f;
    float lenXZ = (endXZ-startXZ).getLength();
    if ( lenXZ == 0.f )
    {
        point_.Y = line_.start.Y;
    }
    else
    {
        float delta = (point_-startXZ).getLength() / lenXZ;
        point_.Y = line_.start.Y + delta*(line_.end.Y-line_.start.Y);
    }
}

float GetLengthXZ(const irr::core::vector3df &vec_)
{
    return sqrt(vec_.X*vec_.X + vec_.Z*vec_.Z);
}

// copy the rotation part of a matrix (from source_ to target_)
void SetMatrixRotation(irr::core::matrix4 &target_, const irr::core::matrix4 &source_)
{
    target_[0] = source_[0];
    target_[1] = source_[1];
    target_[2] = source_[2];

    target_[4] = source_[4];
    target_[5] = source_[5];
    target_[6] = source_[6];

    target_[8] = source_[8];
    target_[9] = source_[9];
    target_[10] = source_[10];
}

// get rotation degrees without crashing on floating point precission problems
irr::core::vector3df GetRotationDegreesSave(const irr::core::matrix4 &mat_)
{
    f64 Y = -ExtMath::asin_save(mat_(0,2));
    f64 C = cos(Y);
    Y *= (f32)irr::core::RADTODEG;

    f64 rotx, roty, X, Z;

    if (fabs(C)>0.0005f)
    {
        rotx = mat_(2,2) / C;
        roty = mat_(1,2)  / C;
        X = atan2( roty, rotx ) * (f32)irr::core::RADTODEG;
        rotx = mat_(0,0) / C;
        roty = mat_(0,1) / C;
        Z = atan2( roty, rotx ) * (f32)irr::core::RADTODEG;
    }
    else
    {
        X  = 0.0f;
        rotx = mat_(1,1);
        roty = -mat_(1,0);
        Z  = atan2( roty, rotx ) * (f32)irr::core::RADTODEG;
    }

    // fix values that get below zero
    // before it would set (!) values to 360
    // that where above 360:
    if (X < 0.00) X += 360.00;
    if (Y < 0.00) Y += 360.00;
    if (Z < 0.00) Z += 360.00;

    return irr::core::vector3df((f32)X,(f32)Y,(f32)Z);
}

// return true if both vectors show in the same direction
bool IsSameDir(const irr::core::vector3df& v1, const irr::core::vector3df& v2)
{
	return (v1+v2).getLengthSQ() > (v1-v2).getLengthSQ();
}

// return true if both vectors show in the same direction
bool IsSameDir(const irr::core::vector2df& v1, const irr::core::vector2df& v2)
{
	return (v1+v2).getLengthSQ() > (v1-v2).getLengthSQ();
}

bool ReadFile(irr::io::IFileSystem * fs, const irr::c8 * fileName, irr::core::array<irr::c8>& targetBuffer, bool nullTerminated)
{
	if ( !fs )
		return false;

	irr::io::IReadFile* file = fs->createAndOpenFile(fileName);
	if (!file )
	{
		return false;
	}

	// Get the file size, so we can pre-allocate the array.
	long length = file->getSize();
	if ( length == 0 )	// Strange case, but good to handle up front.
	{
		file->drop();
		return false;
	}

	if ( nullTerminated )
	{
		targetBuffer.set_used(length+1);	// takes only u32 and so will mess up with huge files
		targetBuffer[0] = 0;
	}
	else
	{
		targetBuffer.set_used(length);
	}

	// No checks because Irrlicht interface (at least in this version) is really strange returning s32 as size instead of long (so long files count as fail?)
	file->read(targetBuffer.pointer(), length);
	file->drop();

	if ( nullTerminated )
	{
		targetBuffer[length] = 0;
	}

	return true;
}

template <typename TBuffer, typename TVertex, typename TVertexSortHelper>
static void AddWeldedMeshBuffer(irr::scene::SMesh* clone, const scene::IMeshBuffer* const mb)
{
	u32 vertexCount = mb->getVertexCount();
	u32 indexCount = mb->getIndexCount();
	const u16* oldIndices = mb->getIndices();

	TBuffer* buffer = new TBuffer();
	buffer->BoundingBox = mb->getBoundingBox();
	buffer->Material = mb->getMaterial();

	if (vertexCount > 0 )
	{
		// reserve buffers (just for speed)
		buffer->Vertices.reallocate(vertexCount);
		buffer->Indices.reallocate(indexCount);

		// pointer to old vertices
		TVertex* oldVertices = (TVertex*)mb->getVertices();

		// create an sort buffer
		std::vector<TVertexSortHelper> sortedVertexBuffer;
		sortedVertexBuffer.resize((size_t)vertexCount);
		for (u32 i=0; i < vertexCount; ++i)
		{
			sortedVertexBuffer[i].vertex = oldVertices+i;
			sortedVertexBuffer[i].oldIndex = i;
			makeHash(sortedVertexBuffer[i]);
		}

		// sort it so similar vertices are together (different vertices can also be together, but similar are never far apart)
		std::sort(sortedVertexBuffer.begin(), sortedVertexBuffer.end(), SortByHash<TVertexSortHelper>);

		// Find the new indices - only different vertices get their own indices
		// Push the new vertices into the new meshbuffer
		const float maxHashDiff = 0.01f;
		u32 newIndex = 0;
		for (u32 i=0; i < vertexCount; ++i)
		{
			if (!sortedVertexBuffer[i].hasNewIndex)
			{
				sortedVertexBuffer[i].newIndex = newIndex;
				sortedVertexBuffer[i].hasNewIndex = true;
				buffer->Vertices.push_back(*sortedVertexBuffer[i].vertex);

				for (u32 k=i+1; k < vertexCount; ++k)
				{
					if ( !core::equals(sortedVertexBuffer[i].hash, sortedVertexBuffer[k].hash, maxHashDiff) )
					{
						break;
					}
                    if ( !sortedVertexBuffer[k].hasNewIndex && *sortedVertexBuffer[i].vertex == *sortedVertexBuffer[k].vertex )
                    {
						sortedVertexBuffer[k].newIndex = newIndex;
						sortedVertexBuffer[k].hasNewIndex = true;
                    }
				}
				++newIndex;
			}
		}

		// Sort by the old index so the old index-buffer can find it quickly
		std::sort(sortedVertexBuffer.begin(), sortedVertexBuffer.end(), SortByOldIndex<TVertexSortHelper>);

		for (u32 i=0; i < indexCount; i += 3)
		{
			u16 a = sortedVertexBuffer[oldIndices[i]].newIndex;
			u16 b = sortedVertexBuffer[oldIndices[i+1]].newIndex;
			u16 c = sortedVertexBuffer[oldIndices[i+2]].newIndex;

			// degenerate?
			if (a == b || b == c || a == c)
				continue;

			buffer->Indices.push_back(a);
			buffer->Indices.push_back(b);
			buffer->Indices.push_back(c);
		}
	}

	clone->addMeshBuffer(buffer);
	buffer->drop();
}

irr::scene::IMesh* CreateMeshWelded(irr::scene::IMesh *mesh)
{
	scene::SMesh* clone = new scene::SMesh();
	clone->BoundingBox = mesh->getBoundingBox();

	for (u32 b=0; b<mesh->getMeshBufferCount(); ++b)
	{
		const scene::IMeshBuffer* const mb = mesh->getMeshBuffer(b);

		switch(mb->getVertexType())
		{
			case video::EVT_STANDARD:
			{
				AddWeldedMeshBuffer<scene::SMeshBuffer, video::S3DVertex, SortVertex>(clone, mb);
				break;
			}
			case video::EVT_2TCOORDS:
			{
				AddWeldedMeshBuffer<scene::SMeshBufferLightMap, video::S3DVertex2TCoords, SortVertex2T>(clone, mb);
				break;
			}
			case video::EVT_TANGENTS:
			{
				AddWeldedMeshBuffer<scene::SMeshBufferTangents, video::S3DVertexTangents, SortVertexTangents>(clone, mb);
				break;
			}
		}
	}

	return clone;
}

template <typename TBuffer, typename TVertex>
static void CreateSubMeshBufferForNormal(irr::scene::SMesh* clone, const scene::IMeshBuffer* const mb, const irr::core::vector3df& targetNormal, float minDot)
{
	u32 vertexCount = mb->getVertexCount();
	u32 indexCount = mb->getIndexCount();
	const u16* oldIndices = mb->getIndices();
	const TVertex* oldVertices = (TVertex*)mb->getVertices();

	TBuffer* buffer = new TBuffer();
	buffer->BoundingBox = mb->getBoundingBox();
	buffer->Material = mb->getMaterial();

		// reserve buffers (just for speed)
	buffer->Vertices.reallocate(vertexCount);
	buffer->Indices.reallocate(indexCount);

	// Vertices can be kept (some might not be needed anymore, but that doesn't matter)
	memcpy((void*)buffer->Vertices.pointer(), (void*)oldVertices, sizeof(TVertex) * vertexCount);
	buffer->Vertices.set_used(vertexCount);

	for ( u32 i=0; i<indexCount; i+=3 )
	{
		irr::core::triangle3df triangle(oldVertices[oldIndices[i]].Pos, oldVertices[oldIndices[i+1]].Pos, oldVertices[oldIndices[i+2]].Pos);
		core::vector3df normal(triangle.getNormal());
		normal.normalize();
		if ( normal.dotProduct(targetNormal) >= minDot)
		{
			buffer->Indices.push_back(oldIndices[i]);
			buffer->Indices.push_back(oldIndices[i+1]);
			buffer->Indices.push_back(oldIndices[i+2]);
		}
	}

	clone->addMeshBuffer(buffer);
	buffer->drop();
}

irr::scene::IMesh* CreateSubMeshForNormal(irr::scene::IMesh *mesh, const irr::core::vector3df& targetNormal, float minDot)
{
	scene::SMesh* clone = new scene::SMesh();
	clone->BoundingBox = mesh->getBoundingBox();

	for (u32 b=0; b<mesh->getMeshBufferCount(); ++b)
	{
		const scene::IMeshBuffer* const mb = mesh->getMeshBuffer(b);

		switch(mb->getVertexType())
		{
			case video::EVT_STANDARD:
			{
				CreateSubMeshBufferForNormal<scene::SMeshBuffer, video::S3DVertex>(clone, mb, targetNormal, minDot);
				break;
			}
			case video::EVT_2TCOORDS:
			{
				CreateSubMeshBufferForNormal<scene::SMeshBufferLightMap, video::S3DVertex2TCoords>(clone, mb, targetNormal, minDot);
				break;
			}
			case video::EVT_TANGENTS:
			{
				CreateSubMeshBufferForNormal<scene::SMeshBufferTangents, video::S3DVertexTangents>(clone, mb, targetNormal, minDot);
				break;
			}
		}
	}

	return clone;
}

}; // namespace ExtIrr
