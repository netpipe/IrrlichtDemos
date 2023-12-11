#include "JungleChunk.h"
#include "TreeSeed.h"
#include "Jungle.h"

namespace irr
{
    namespace scene
    {
        namespace jungleScene
        {
            JungleChunk::JungleChunk()
            {
                //ctor
            }

            void JungleChunk::setup()
            {
                renderable = false;
                needUpdate = true;
                seeds = new TreeSeed*[jungle->CHUNK_SIZE];
                for(u8 i = 0; i < jungle->CHUNK_SIZE; i++)
                {
                    seeds[i] = new TreeSeed[jungle->CHUNK_SIZE];
                }//i
            }

            JungleChunk::~JungleChunk()
            {
                //dtor
                //dtor
                for (u8 i = 0; i < jungle->CHUNK_SIZE; ++i)
                {
                    delete [] seeds[i];
                }
                delete [] seeds;
                indexBuffer.clear();
                vertexBuffer.clear();
            }

            void JungleChunk::growTreeAt(core::vector3df position,
                                            u8 segment_count,
                                            f32 min_rotation,
                                            f32 max_rotation,
                                            f32 branch_length,
                                            u8 branch_count,
                                            f32 max_radius,
                                            u8 ground_root_count,
                                            u8 bark_type,
                                            f32 leaf_width,
                                            f32 leaf_length,
                                            u8 leaf_segment,
                                            f32 leaf_stiffness,
                                            u8 leaf_type,
                                            u8 seed,
                                            u16 age)
            {

                u16 rx = u16((position.X - X) / jungle->TREE_DIMENSION);
                u16 ry = u16((position.Z - Z) / jungle->TREE_DIMENSION);

                if(seeds[rx][ry].active)return;

                needUpdate = true;

                seeds[rx][ry].segment_count = segment_count;
                seeds[rx][ry].min_rotation = min_rotation;
                seeds[rx][ry].max_rotation = max_rotation;
                seeds[rx][ry].branch_length = branch_length;
                seeds[rx][ry].branch_count = branch_count;
                seeds[rx][ry].bark_type = bark_type;
                seeds[rx][ry].max_radius = max_radius;
                seeds[rx][ry].ground_root_count = ground_root_count;
                seeds[rx][ry].leaf_width = leaf_width;
                seeds[rx][ry].leaf_length = leaf_length;
                seeds[rx][ry].leaf_segment = leaf_segment;
                seeds[rx][ry].leaf_stiffness = leaf_stiffness;
                seeds[rx][ry].leaf_type = leaf_type;
                seeds[rx][ry].seed = seed;
                seeds[rx][ry].age = age;
                seeds[rx][ry].active = true;
                seeds[rx][ry].Y = position.Y;

                renderable = true;
            }

            void JungleChunk::update(u32 timeMs)
            {
                if(!needUpdate) return;
                // updating geometri and growwing trees
                needUpdate = false;
                clearBuffer();
                for (u8 x = 0; x < jungle->CHUNK_SIZE; ++x)
                {
                    for (u8 y = 0; y < jungle->CHUNK_SIZE; ++y)
                    {
                        if(seeds[x][y].active)
                        {
                            if(seeds[x][y].time < 0 )
                            {
                                seeds[x][y].time = timeMs;
                            }
                            else
                            {
                                seeds[x][y].age += (timeMs - seeds[x][y].time)/1000;
                            }

                            //printf("growing %d\n", seeds[x][y].age);

                            // build tree geom
                            // tree generating algorithm goes here
                            srand(seeds[x][y].seed);
                            bool updateReq = createTreeGeom(
                                            core::vector3df(X + x * jungle->TREE_DIMENSION, seeds[x][y].Y, Z + y * jungle->TREE_DIMENSION),
                                            seeds[x][y].segment_count,
                                            seeds[x][y].min_rotation,
                                            seeds[x][y].max_rotation,
                                            seeds[x][y].branch_length,
                                            seeds[x][y].branch_count,
                                            seeds[x][y].max_radius,
                                            seeds[x][y].bark_type,
                                            core::vector3df(0.0f,0.0f,90.0f),
                                            seeds[x][y].leaf_width,
                                            seeds[x][y].leaf_length,
                                            seeds[x][y].leaf_segment,
                                            seeds[x][y].leaf_stiffness,
                                            seeds[x][y].leaf_type,
                                            seeds[x][y].seed,
                                            seeds[x][y].age);


                            for(u8 r = 0 ; r < seeds[x][y].ground_root_count;r++)
                            {
                                srand(seeds[x][y].seed * (r+1));
                                core::vector3df inrotation(0.0f,f32(rand()%1000)/1000 * 360,0.0f);
                                createRootGeom(core::vector3df(X + x * jungle->TREE_DIMENSION, seeds[x][y].Y, Z + y * jungle->TREE_DIMENSION),
                                            seeds[x][y].segment_count,
                                            seeds[x][y].min_rotation,
                                            seeds[x][y].max_rotation,
                                            seeds[x][y].branch_length,
                                            seeds[x][y].branch_count * 2,
                                            seeds[x][y].max_radius,
                                            inrotation,
                                            seeds[x][y].bark_type,
                                            seeds[x][y].seed,
                                            seeds[x][y].age * 0.75);
                            }

                            needUpdate = updateReq || needUpdate;
                        }
                    }
                }

                // calculate tangent and bi tangent
                for(u32 i = 0 ; i < indexBuffer.size() ; i+= 3)
                {
                    u32 id1 = indexBuffer[i];
                    u32 id2 = indexBuffer[i + 1];
                    u32 id3 = indexBuffer[i + 2];

                    core::vector3df &p1 = vertexBuffer[id1].Pos;
                    core::vector3df &p2 = vertexBuffer[id2].Pos;
                    core::vector3df &p3 = vertexBuffer[id3].Pos;

                    core::vector2df &uv1 = vertexBuffer[id1].TCoords;
                    core::vector2df &uv2 = vertexBuffer[id2].TCoords;
                    core::vector2df &uv3 = vertexBuffer[id3].TCoords;

                    core::vector3df deltaPos1 = p2 - p1;
                    core::vector3df deltaPos2 = p3 - p1;
                    core::vector2df deltaUV1 = uv2 - uv1;
                    core::vector2df deltaUV2 = uv3 - uv1;
                    f32 r = 1.0f / (deltaUV1.X * deltaUV2.Y - deltaUV1.Y * deltaUV2.X);
                    vertexBuffer[id1].Tangent = (deltaPos1 * deltaUV2.Y   - deltaPos2 * deltaUV1.Y)*r;
                    vertexBuffer[id1].Binormal = (deltaPos2 * deltaUV1.X   - deltaPos1 * deltaUV2.X)*r;


                    deltaPos1 = p1 - p2;
                    deltaPos2 = p3 - p2;
                    deltaUV1 = uv1 - uv2;
                    deltaUV2 = uv3 - uv2;
                    r = 1.0f / (deltaUV1.X * deltaUV2.Y - deltaUV1.Y * deltaUV2.X);
                    vertexBuffer[id2].Tangent = (deltaPos1 * deltaUV2.Y   - deltaPos2 * deltaUV1.Y)*r;
                    vertexBuffer[id2].Binormal = (deltaPos2 * deltaUV1.X   - deltaPos1 * deltaUV2.X)*r;

                    deltaPos1 = p2 - p3;
                    deltaPos2 = p1 - p3;
                    deltaUV1 = uv1 - uv3;
                    deltaUV2 = uv2 - uv3;
                    r = 1.0f / (deltaUV1.X * deltaUV2.Y - deltaUV1.Y * deltaUV2.X);
                    vertexBuffer[id3].Tangent = (deltaPos1 * deltaUV2.Y   - deltaPos2 * deltaUV1.Y)*r;
                    vertexBuffer[id3].Binormal = (deltaPos2 * deltaUV1.X   - deltaPos1 * deltaUV2.X)*r;
                }
            }

            bool JungleChunk::createTreeGeom(core::vector3df position,
                                            u8 segment_count,
                                            f32 min_rotation,
                                            f32 max_rotation,
                                            f32 branch_length,
                                            u8 branch_count,
                                            f32 max_radius,
                                            u8 bark_type,
                                            core::vector3df initialRotation,
                                            f32 leaf_width,
                                            f32 leaf_length,
                                            u8 leaf_segment,
                                            f32 leaf_stiffness,
                                            u8 leaf_type,
                                            u8 seed,
                                            u16 age)
            {
                f32 progress = irr::core::min_(f32(age) / 10000.0f, 1.0f); // right now every thing

                f32 current_segment_count = roundf( 1 + progress * f32(segment_count));
                f32 current_length = branch_length * progress;
                f32 segment_length = current_length / current_segment_count;

                f32 radius = max_radius * progress;

                u8 current_branch_count = irr::core::min_(branch_count, u8(current_segment_count)) * (0.5 + (rand()%1000 / 1000.0f));
                current_branch_count = u8(f32(current_branch_count) * progress + 0.5);

                core::vector3df current_position = position;
                core::vector3df current_rotation = initialRotation;


                video::SColor vcolor = video::SColor(255,255,255,255);

                u8 quota = current_branch_count;
                u32 firstRingVertexID = 0;
                u32 secondRingVertexID = 0;
                f32 startChilds = f32(segment_count) * 0.25f;

                for(f32 i = 0 ; i < current_segment_count ; i++)
                {
                    f32 v = i / current_segment_count * 4.0f;


                    secondRingVertexID = vertexBuffer.size();
                    // radius progress
                    f32 current_radius = radius * f32(current_segment_count - i) / f32(current_segment_count);

                    // create number of vertice surrounding current position
                    for(u8 j = 0 ; j < jungle->NUM_VERTICES ; j++)
                    {
                        core::vector3df cvp = core::vector3df(0.0f,0.0f,current_radius);
                        cvp.rotateYZBy(f32(j)/f32(jungle->NUM_VERTICES) * 360.0f);

                        cvp.rotateYZBy(current_rotation.X);
                        cvp.rotateXZBy(-current_rotation.Y);
                        cvp.rotateXYBy(current_rotation.Z);

                        f32 vx = cvp.X + current_position.X;
                        f32 vy = cvp.Y + current_position.Y;
                        f32 vz = cvp.Z + current_position.Z;

                        cvp = cvp.normalize();
                        f32 nx = -cvp.X * jungle->normalScale;
                        f32 ny = -cvp.Y * jungle->normalScale;
                        f32 nz = -cvp.Z * jungle->normalScale;

                        f32 u = f32(bark_type)/8.0f + f32(j % u8(f32(jungle->NUM_VERTICES) / 2.0f)) / (f32(jungle->NUM_VERTICES) / 2.0f) * 0.125;

                        vertexBuffer.push_back(video::S3DVertexTangents(vx,vy,vz,nx,ny,nz,vcolor, u,v,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f));

                    }
                    //assign indices
                    if(i > 0)
                    {
                        for(u8 j = 0 ; j < jungle->NUM_VERTICES ; j++)
                        {
                            u16 id1 = firstRingVertexID + j;
                            u16 id2 = firstRingVertexID + ((j + 1)%jungle->NUM_VERTICES);
                            u16 id3 = secondRingVertexID + j;
                            u16 id4 = secondRingVertexID + ((j + 1)%jungle->NUM_VERTICES);

                            indexBuffer.push_back(id1);
                            indexBuffer.push_back(id2);
                            indexBuffer.push_back(id3);

                            indexBuffer.push_back(id2);
                            indexBuffer.push_back(id4);
                            indexBuffer.push_back(id3);

                        }
                    }
                    u16 nseed = rand();
                    if(i >= startChilds)
                    if(quota > 0)
                    {
                        //create leaf
                        //if((rand()%1000 )> 200)
                        f32 leaf_size_factor = f32(current_segment_count - i) / f32(current_segment_count);
                            createLeaf(current_position,
                                       current_rotation,
                                       leaf_width * leaf_size_factor,
                                       leaf_length * leaf_size_factor,
                                       leaf_segment,
                                       leaf_stiffness,
                                       leaf_type);

                        if((rand()%1000 )> 100)
                        {

                            quota --;
                            f32 len = (max_rotation - min_rotation);
                            f32 crx = current_rotation.X + (min_rotation + len * (rand()%1000 / 1000.0f)) * (rand()%1000 > 500 ? 1.0f : -1.0f);
                            f32 cry = current_rotation.Y + (min_rotation + len * (rand()%1000 / 1000.0f)) * (rand()%1000 > 500 ? 1.0f : -1.0f);
                            f32 crz = current_rotation.Z + (min_rotation + len * (rand()%1000 / 1000.0f)) * (rand()%1000 > 500 ? 1.0f : -1.0f);
                            createTreeGeom(
                                current_position,
                                segment_count,
                                min_rotation,
                                max_rotation,
                                branch_length,
                                branch_count,
                                current_radius,
                                bark_type,
                                core::vector3df(crx, cry, crz),
                                leaf_width * leaf_size_factor,
                                leaf_length * leaf_size_factor,
                                leaf_segment,
                                leaf_stiffness,
                                leaf_type,
                                seed,
                                core::max_(core::min_(f32(age) * f32(current_segment_count - i) / f32(current_segment_count) - 50, 10000.0f), 0.0f));
                        }

                    }
                    core::vector3df nextPos = core::vector3df(0, segment_length, 0);
                    //current_rotation;
                    srand(nseed);
                    f32 len = (max_rotation - min_rotation);
                    current_rotation.X += (min_rotation + len * (rand()%1000 / 1000.0f)) * (rand()%1000 > 500 ? 1.0f : -1.0f);
                    current_rotation.Y += (min_rotation + len * (rand()%1000 / 1000.0f)) * (rand()%1000 > 500 ? 1.0f : -1.0f);
                    nextPos.rotateYZBy(current_rotation.X);
                    nextPos.rotateXZBy(current_rotation.Y);

                    current_position.X += nextPos.X;
                    current_position.Y += nextPos.Y;
                    current_position.Z += nextPos.Z;

                    firstRingVertexID = secondRingVertexID;

                }
                return (progress < 1.0f) ; // stops if matured
            }

            bool JungleChunk::createRootGeom(core::vector3df position,
                                            u8 segment_count,
                                            f32 min_rotation,
                                            f32 max_rotation,
                                            f32 branch_length,
                                            u8 branch_count,
                                            f32 max_radius,
                                            core::vector3df initialRotation,
                                            u8 bark_type,
                                            u8 seed,
                                            u16 age)
            {
                f32 progress = irr::core::min_(f32(age) / 10000.0f, 1.0f); // right now every thing

                f32 current_segment_count = roundf( 1 + progress * segment_count);
                f32 current_length = branch_length * progress;
                f32 segment_length = current_length / current_segment_count;

                f32 radius = max_radius * progress;

                u8 current_branch_count = irr::core::min_(branch_count, u8(current_segment_count)) * (0.5 + (rand()%1000 / 1000.0f));
                current_branch_count = u8(f32(current_branch_count) * progress + 0.5);

                core::vector3df current_position = position;
                core::vector3df current_rotation = initialRotation;

                video::SColor vcolor = video::SColor(255,255,255,255);

                u8 quota = current_branch_count;
                u32 firstRingVertexID = 0;
                u32 secondRingVertexID = 0;
                for(f32 i = 0 ; i < current_segment_count ; i++)
                {
                    f32 v = i / current_segment_count;
                    secondRingVertexID = vertexBuffer.size();
                    // radius progress
                    f32 current_radius = radius * f32(current_segment_count - i) / f32(current_segment_count);

                    // create number of vertice surrounding current position
                    for(u8 j = 0 ; j < jungle->NUM_VERTICES ; j++)
                    {
                        core::vector3df cvp = core::vector3df(0.0f,0.0f,current_radius);
                        cvp.rotateYZBy(f32(j)/f32(jungle->NUM_VERTICES) * 360.0f);

                        cvp.rotateYZBy(current_rotation.X);
                        cvp.rotateXZBy(-current_rotation.Y);
                        cvp.rotateXYBy(current_rotation.Z);

                        f32 vx = cvp.X + current_position.X;
                        f32 vy = cvp.Y + current_position.Y;
                        f32 vz = cvp.Z + current_position.Z;

                        cvp = cvp.normalize();
                        f32 nx = -cvp.X * jungle->normalScale;
                        f32 ny = -cvp.Y * jungle->normalScale;// * 20.0f;
                        f32 nz = -cvp.Z * jungle->normalScale;// * 20.0f;

                        f32 u = f32(bark_type)/8.0f + f32(j % u8(f32(jungle->NUM_VERTICES) / 2.0f)) / (f32(jungle->NUM_VERTICES) / 2.0f) * 0.128;

                        vertexBuffer.push_back(video::S3DVertexTangents(vx,vy,vz,nx,ny,nz,vcolor, u,v, 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f));

                    }

                    //assign indices
                    if(i > 0)
                    {
                        for(u8 j = 0 ; j < jungle->NUM_VERTICES ; j++)
                        {
                            u16 id1 = firstRingVertexID + j;
                            u16 id2 = firstRingVertexID + ((j + 1)%jungle->NUM_VERTICES);
                            u16 id3 = secondRingVertexID + j;
                            u16 id4 = secondRingVertexID + ((j + 1)%jungle->NUM_VERTICES);

                            indexBuffer.push_back(id1);
                            indexBuffer.push_back(id2);
                            indexBuffer.push_back(id3);

                            indexBuffer.push_back(id2);
                            indexBuffer.push_back(id4);
                            indexBuffer.push_back(id3);
                        }
                    }
                    u16 nseed = rand();
                    core::vector3df nextPos = core::vector3df(0, segment_length, 0);
                    //current_rotation;
                    srand(nseed);
                    f32 len = (max_rotation - min_rotation);
                    current_rotation.X += (min_rotation + len * (rand()%1000 / 1000.0f)) * (rand()%1000 > 500 ? 1.0f : -1.0f);
                    current_rotation.Y += (min_rotation + len * (rand()%1000 / 1000.0f)) * (rand()%1000 > 500 ? 1.0f : -1.0f);
                    nextPos.rotateYZBy(current_rotation.X);
                    nextPos.rotateXZBy(current_rotation.Y);

                    nextPos.Y -= (nextPos.Y + current_position.Y) - heightRef->getHeight(current_position.X + nextPos.X, current_position.Z + nextPos.Z);
                    //current_rotation = nextPos.getSphericalCoordinateAngles();

                    current_position.X += nextPos.X;
                    current_position.Y += nextPos.Y;
                    current_position.Z += nextPos.Z;
                    //current_position.Y = core::min_(current_position.Y, 5.0f);

                    firstRingVertexID = secondRingVertexID;

                }
                return (progress < 1.0f) ; // stops if matured
            }

            void JungleChunk::renderBark(video::IVideoDriver * driver)
            {
                //
                if(renderable)
                {
                    driver->drawIndexedTriangleList(vertexBuffer.pointer(), vertexBuffer.size(), indexBuffer.pointer(), indexBuffer.size() / 3);
                }
            }

            void JungleChunk::renderLeaf(video::IVideoDriver * driver)
            {
                //
                if(renderable)
                {
                    driver->drawIndexedTriangleList(vertexBufferLeaf.pointer(), vertexBufferLeaf.size(), indexBufferLeaf.pointer(), indexBufferLeaf.size() / 3);
                }
            }

            bool JungleChunk::treeExistAt(core::vector3df position)
            {
                //
                u16 rx = u16((position.X - X) / jungle->TREE_DIMENSION);
                u16 ry = u16((position.Z - Z) / jungle->TREE_DIMENSION);

                return seeds[rx][ry].active;
            }

            void JungleChunk::reset()
            {
                clearBuffer();
                for (u8 x = 0; x < jungle->CHUNK_SIZE; ++x)
                {
                    for (u8 y = 0; y < jungle->CHUNK_SIZE; ++y)
                    {
                        seeds[x][y].active = false;
                        seeds[x][y].time = -1;
                        seeds[x][y].age = 0;
                    }
                }
                renderable = false;
                needUpdate = false;
            }

            void JungleChunk::createLeaf(core::vector3df position, core::vector3df initialRotation, f32 wid, f32 len, u8 segment, f32 stiffness, u8 type)
            {
                //
                f32 rw = wid / 2.0f;
                f32 lenStep = len / (segment - 1);
                f32 cLen = 0;
                f32 ystep = (1.0 - stiffness) * 10.0f;
                f32 cy = 0;
                f32 v = 0;
                f32 vstep = 1.0f / (segment - 1);

                video::SColor vcolor = video::SColor(255,255,255,255);

                u16 id = vertexBufferLeaf.size() + 2;

                for(u8 i = 0 ; i < segment; i++)
                {
                    //
                    core::vector3df v1(cLen, cy, -rw);
                    v1.rotateYZBy(initialRotation.X);
                    v1.rotateXZBy(initialRotation.Y);
                    v1.rotateXYBy(initialRotation.Z);
                    v1 += position;

                    core::vector3df v2(cLen, cy, rw);
                    v2.rotateYZBy(initialRotation.X);
                    v2.rotateXZBy(initialRotation.Y);
                    v2.rotateXYBy(initialRotation.Z);
                    v2 += position;

                    vertexBufferLeaf.push_back(video::S3DVertexTangents(v1.X,v1.Y,v1.Z,0,1,0,vcolor, f32(type)/8.0f, 1.0 - v, 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f));
                    vertexBufferLeaf.push_back(video::S3DVertexTangents(v2.X,v2.Y,v2.Z,0,1,0,vcolor, f32(type)/8.0f + 0.125f, 1.0 - v, 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f));

                    if(i > 0)
                    {
                        //
                        indexBufferLeaf.push_back(id);
                        indexBufferLeaf.push_back(id + 1);
                        indexBufferLeaf.push_back(id + 3);

                        indexBufferLeaf.push_back(id + 3);
                        indexBufferLeaf.push_back(id + 2);
                        indexBufferLeaf.push_back(id);
                    }

                    id += 2;
                    v += vstep;
                    cLen += lenStep;
                    cy += ystep;
                }
            }

            void JungleChunk::clearBuffer()
            {
                indexBuffer.clear();
                vertexBuffer.clear();
                indexBufferLeaf.clear();
                vertexBufferLeaf.clear();
            }
        }
    }
}
