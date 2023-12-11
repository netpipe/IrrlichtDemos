#include "Jungle.h"
#include "JungleChunk.h"
#include <irrlicht.h>

namespace irr
{
    namespace scene
    {
        namespace jungleScene
        {
            Jungle::Jungle(u32 dimension, u32 chunkSize, u32 treeDimension, u8 verticesDetail, ITerrainSceneNode* heightRef, ISceneManager* mgr, s32 id):
                scene::ISceneNode(0, mgr, id)
            {
                //ctor
                Box.reset(core::vector3df(-2000,-2000,-2000));
                Box.addInternalPoint(core::vector3df(2000,2000,2000));

                refNode = heightRef;

                setAutomaticCulling(EAC_OFF);

                //calculating configuration
                CHUNK_SIZE = chunkSize;
                TREE_DIMENSION = treeDimension;
                u32 totalPossibleTreeRooted = dimension / treeDimension;
                WORLD_SIZE = totalPossibleTreeRooted / CHUNK_SIZE;
                CHUNK_RENDER_DIMENSION = f32(dimension) / f32(WORLD_SIZE);
                NUM_VERTICES = verticesDetail;
                normalScale = 500.0f;

                chunks = new JungleChunk*[WORLD_SIZE];
                for(u8 i = 0; i < WORLD_SIZE; i++)
                {
                    chunks[i] = new JungleChunk[WORLD_SIZE];
                }//j

                //set chunks positions
                for(u8 x = 0 ; x < WORLD_SIZE; x ++)
                {
                    for(u8 y = 0 ; y < WORLD_SIZE; y ++)
                    {
                        chunks[x][y].X = x * CHUNK_RENDER_DIMENSION;
                        chunks[x][y].Y = 0;
                        chunks[x][y].Z = y * CHUNK_RENDER_DIMENSION;
                        chunks[x][y].heightRef = heightRef;
                        chunks[x][y].jungle = this;
                        chunks[x][y].setup();
                    }
                }
            }

            Jungle::~Jungle()
            {
                //dtor
                //dtor
                for (u8 i = 0; i < WORLD_SIZE; ++i)
                {
                    delete [] chunks[i];
                }
                delete [] chunks;
            }

            void Jungle::reset()
            {
                for(u8 x = 0 ; x < WORLD_SIZE; x ++)
                {
                    for(u8 y = 0 ; y < WORLD_SIZE; y ++)
                    {
                        if(chunks[x][y].isRenderable())
                        {
                            chunks[x][y].reset();
                        }
                    }
                }
            }

            bool Jungle::treeExistAt(core::vector3df position)
            {
                //
                position /= this->getScale();
                s32 idx = s32(position.X / CHUNK_RENDER_DIMENSION);
                s32 idy = s32(position.Z / CHUNK_RENDER_DIMENSION);
                if(idx < 0)return false;
                if(idy < 0)return false;
                if(idx >= WORLD_SIZE)return false;
                if(idy >= WORLD_SIZE)return false;
                return chunks[idx][idy].treeExistAt(position);
            }

            void Jungle::OnRegisterSceneNode()
            {
                if (IsVisible)
                {
                    SceneManager->registerNodeForRendering(this);
                }
                ISceneNode::OnRegisterSceneNode();
            }

            void Jungle::OnAnimate(u32 timeMs)
            {
                // animating jungle if necessary
                for(u8 x = 0 ; x < WORLD_SIZE; x ++)
                {
                    for(u8 y = 0 ; y < WORLD_SIZE; y ++)
                    {
                        chunks[x][y].update(timeMs);
                    }
                }
            }

            void Jungle::render()
            {
                video::IVideoDriver * driver = SceneManager->getVideoDriver();

                ICameraSceneNode* cam = SceneManager->getActiveCamera();
                driver->setTransform(video::ETS_VIEW, cam->getViewMatrix());
                driver->setTransform(video::ETS_PROJECTION, cam->getProjectionMatrix());
                driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

                core::vector3df camPos = SceneManager->getActiveCamera()->getAbsolutePosition();
                f32 far = SceneManager->getActiveCamera()->getFarValue();
                camPos.Y = 0;

                //start rendering
                for(u8 x = 0 ; x < WORLD_SIZE; x ++)
                {
                    for(u8 y = 0 ; y < WORLD_SIZE; y ++)
                    {
                        f32 dist = camPos.getDistanceFrom(
                                        core::vector3df(chunks[x][y].X + 16,
                                                        chunks[x][y].Y + 16,
                                                        chunks[x][y].Z + 16));
                        if(dist > far)
                            continue;
                        //v_chunks[x][y].render(driver);
                        driver->setMaterial(Material);
                        chunks[x][y].renderBark(driver);
                        driver->setMaterial(MaterialLeaf);
                        chunks[x][y].renderLeaf(driver);
                    }
                }
            }

            void Jungle::addTreeAt(core::vector3df position,
                                            u8 segment_count,
                                            f32 min_rotation,
                                            f32 max_rotation,
                                            f32 branch_length,
                                            u8 branch_count,
                                            f32 max_radius,
                                            u8 ground_root_count,
                                            f32 leaf_width,
                                            f32 leaf_length,
                                            u8 leaf_segment,
                                            f32 leaf_stiffness,
                                            u8 leaf_type,
                                            u8 bark_type,
                                            u8 seed,
                                            bool instant)
            {
                position /= this->getScale();
                s32 idx = s32(position.X / CHUNK_RENDER_DIMENSION);
                s32 idy = s32(position.Z / CHUNK_RENDER_DIMENSION);
                if(idx < 0)return;
                if(idy < 0)return;
                if(idx >= WORLD_SIZE)return;
                if(idy >= WORLD_SIZE)return;
                s32 age = 0;
                if(instant)age = 10000;
                chunks[idx][idy].growTreeAt(position,
                                            segment_count,
                                            min_rotation,
                                            max_rotation,
                                            branch_length,
                                            branch_count,
                                            max_radius,
                                            ground_root_count,
                                            bark_type,
                                            leaf_width,
                                            leaf_length,
                                            leaf_segment,
                                            leaf_stiffness,
                                            leaf_type,
                                            seed,
                                            age);//(position, segment_count, min_rotation, max_rotation, branch_length, branch_count, seed);
            }
        }
    }
}
