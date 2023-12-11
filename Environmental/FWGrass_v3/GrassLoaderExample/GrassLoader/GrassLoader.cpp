#include "GrassLoader.h"

using namespace irr;

namespace GrassLoader
{

class SimpleBuffer
{
    void* buffer;
    u32 chunk;
    size_t usedsize;
    size_t size;
    public:
    SimpleBuffer(u32 chunkSize)
    : chunk(chunkSize), usedsize(0)
    {
        buffer = malloc(chunk);
        size = chunk;
    }
    u8* getBuffer() {return ((u8*)buffer);}
    size_t getUsedSize() {return usedsize;}
    void append(const void * ptr, size_t count)
    {
        if (size<(count+usedsize))
        {
            size = ((count+usedsize)/chunk)*chunk+chunk;
            buffer = realloc(buffer,size);
        }
        memcpy(((c8*)buffer)+usedsize, ptr, count);
        usedsize += count;
    }
    ~SimpleBuffer()
    {
        free(buffer);
    }
};


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "zlib.h"

#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 262144

/* report a zlib or i/o error */
void zerr(int ret)
{
    fputs("zpipe: ", stderr);
    switch (ret) {
    case Z_ERRNO:
        if (ferror(stdin))
            fputs("error reading stdin\n", stderr);
        if (ferror(stdout))
            fputs("error writing stdout\n", stderr);
        break;
    case Z_STREAM_ERROR:
        fputs("invalid compression level\n", stderr);
        break;
    case Z_DATA_ERROR:
        fputs("invalid or incomplete deflate data\n", stderr);
        break;
    case Z_MEM_ERROR:
        fputs("out of memory\n", stderr);
        break;
    case Z_VERSION_ERROR:
        fputs("zlib version mismatch!\n", stderr);
    }
}

int inf(FILE *source, SimpleBuffer &dest)
{
    int ret;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    /* decompress until deflate stream ends or end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)inflateEnd(&strm);
            return Z_ERRNO;
        }
        if (strm.avail_in == 0)
            break;
        strm.next_in = in;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;     /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                (void)inflateEnd(&strm);
                return ret;
            }
            have = CHUNK - strm.avail_out;
            dest.append(out, have);
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (ret != Z_STREAM_END);

    /* clean up and return */
    (void)inflateEnd(&strm);
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void loadGrass(const io::path &filename, scene::ISceneManager* smgr,
                f32 LoD0, f32 LoD1, f32 LoD2, f32 LoD3, f32 LoD4, f32 LoD5)
{
    core::array<f32> LoDDistances(6);
    LoDDistances[0] = LoD0;
    LoDDistances[1] = LoD1;
    LoDDistances[2] = LoD2;
    LoDDistances[3] = LoD3;
    LoDDistances[4] = LoD4;
    LoDDistances[5] = LoD5;
    LoDDistances.set_used(6);
    if (!smgr)
        return;
    FILE* pFile = fopen(filename.c_str(),"r");
    if (!pFile)
    {
        printf("Bad File - cannot open.\n");
        return;
    }
    SimpleBuffer buff(CHUNK);
    int res = inf(pFile,buff);
    if (res!=Z_OK)
    {
        printf("ZIP decompression failed\n");
        return;
    }
    fclose(pFile);
    core::stringc header = (c8*)buff.getBuffer();
    if (!(header=="FrontWarriorGrassFile\n"))
    {
        printf("Bad File - header doesnt match.\n");
        return;
    }
    u32 position = 23;
    u8* seeker = buff.getBuffer()+23;
    if (*((u32*)seeker)==0xfdb97531)
        printf("Endian: OK!\n");
    else
    {
        printf("Endian: BAD!\n");
        return;
    }
    seeker+=4;
    core::vector3df MinEdge = *((core::vector3df*)seeker);
    seeker+=12;
    core::vector3df MaxEdge = *((core::vector3df*)seeker);
    seeker+=12;
    core::vector2df scale = *((core::vector2df*)seeker);
    seeker+=8;
    u8 upAxis = *seeker;
    seeker++;

    u8 grassVTs = *seeker;
    printf("Number of Grass VTs: %u\n\n",u32(grassVTs));
    seeker++;
    position = 61;
    for (u8 i=0; i<grassVTs&&position<buff.getUsedSize(); i++)
    {
        core::stringc path = seeker;
        if (path.size()==0)
        {
            seeker++;
            position++;
            continue;
        }
        video::ITexture* texture = smgr->getVideoDriver()->getTexture(path.c_str());
        seeker += path.size()+1;
        f32 QuadScale = *((f32*)seeker);
        seeker +=4;
        u32 indexCount = *((u32*)seeker);
        seeker +=4;
        position += path.size()+9;
        printf("Index Count: %u\n",indexCount);
        u32 xcount = floorf(QuadScale/scale.X)+1;
        u32 ycount = floorf(QuadScale/scale.Y)+1;
        printf("Quads: %u x %u \n",xcount,ycount);
        for (u32 j=0; j<(xcount*ycount); j++)
        {
            u32 vcount = *((u32*)seeker);
            seeker +=4;
            u32 indexCountQuad = *((u32*)seeker);
            seeker +=4;
            u32 plantCount = indexCountQuad/indexCount;
            u32 plantCountOrig = plantCount;
            position += 8;
            if (plantCount==0)
                continue;
            video::S3DVertex2TCoords* verts = (video::S3DVertex2TCoords*)seeker;
            seeker += sizeof(video::S3DVertex2TCoords)*vcount;
            u16* indices0 = (u16*)seeker;
            seeker += indexCountQuad*2;
            position += sizeof(video::S3DVertex2TCoords)*vcount+2*indexCountQuad;
            u16* indices1 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices1 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            u16* indices2 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices2 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            u16* indices3 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices3 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            u16* indices4 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices4 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            u16* indices5 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices5 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            GrassQuadTreeLeaf<video::S3DVertex2TCoords> *leaf = new GrassQuadTreeLeaf<video::S3DVertex2TCoords>(smgr->getRootSceneNode(), smgr,
                                            verts,vcount,
                                            indices0,indexCountQuad,
                                            indices1,plantCountOrig/2*indexCount,
                                            indices2,plantCountOrig/4*indexCount,
                                            indices3,plantCountOrig/8*indexCount,
                                            indices4,plantCountOrig/16*indexCount,
                                            indices5,plantCountOrig/32*indexCount,6);
            leaf->setLoDDistances(LoDDistances);
            //leaf->getMaterial().Lighting = false;
            leaf->getMaterial().BackfaceCulling = false;
            leaf->getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
            leaf->getMaterial().setTexture(0,texture);
        }
    }
    u8 otherVTs = *seeker;
    printf("Number of Other VTs: %u\n\n",u32(otherVTs));
    seeker++;
    position++;
    for (u8 i=0; i<otherVTs&&position<buff.getUsedSize(); i++)
    {
        core::stringc path = seeker;
        if (path.size()==0)
        {
            seeker++;
            position++;
            continue;
        }
        video::ITexture* texture = smgr->getVideoDriver()->getTexture(path.c_str());
        seeker += path.size()+1;
        f32 QuadScale = *((f32*)seeker);
        seeker +=4;
        u32 indexCount = *((u32*)seeker);
        seeker +=4;
        position += path.size()+9;
        printf("Index Count: %u\n",indexCount);
        u32 xcount = floorf(QuadScale/scale.X)+1;
        u32 ycount = floorf(QuadScale/scale.Y)+1;
        printf("Quads: %u x %u \n",xcount,ycount);
        for (u32 j=0; j<(xcount*ycount); j++)
        {
            u32 vcount = *((u32*)seeker);
            seeker +=4;
            u32 indexCountQuad = *((u32*)seeker);
            seeker +=4;
            video::S3DVertexTangents* verts = (video::S3DVertexTangents*)seeker;
            seeker += sizeof(video::S3DVertexTangents)*vcount;
            u16* indices0 = (u16*)seeker;
            seeker += indexCountQuad*2;
            position += 8+sizeof(video::S3DVertexTangents)*vcount+2*indexCountQuad;
            u32 plantCount = indexCountQuad/indexCount;
            u32 plantCountOrig = plantCount;
            if (plantCount==0)
                continue;
            u16* indices1 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices1 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            u16* indices2 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices2 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            u16* indices3 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices3 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            u16* indices4 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices4 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            u16* indices5 = NULL;
            plantCount /= 2;
            if (plantCount!=0)
            {
                indices5 = (u16*)seeker;
                seeker += 2*plantCount*indexCount;
                position += 2*plantCount*indexCount;
            }
            GrassQuadTreeLeaf<video::S3DVertexTangents> *leaf = new GrassQuadTreeLeaf<video::S3DVertexTangents>(smgr->getRootSceneNode(), smgr,
                                            verts,vcount,
                                            indices0,indexCountQuad,
                                            indices1,plantCountOrig/2*indexCount,
                                            indices2,plantCountOrig/4*indexCount,
                                            indices3,plantCountOrig/8*indexCount,
                                            indices4,plantCountOrig/16*indexCount,
                                            indices5,plantCountOrig/32*indexCount,6);
            leaf->setLoDDistances(LoDDistances);
            //leaf->getMaterial().Lighting = false;
            leaf->getMaterial().BackfaceCulling = false;
            leaf->getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
            leaf->getMaterial().setTexture(0,texture);
        }
    }


    return;
}

}
