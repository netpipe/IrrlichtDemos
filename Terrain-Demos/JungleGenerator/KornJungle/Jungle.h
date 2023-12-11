#ifndef JUNGLE_H
#define JUNGLE_H

#include <irrlicht.h>
#include "JungleChunk.h"


namespace irr
{
    namespace scene
    {
        namespace jungleScene
        {
            class Jungle : public ISceneNode
            {
                public:
                    // the size of the world, determines how many chunk
                    // build the world
                    u32 WORLD_SIZE;

                    // chunk dimension,
                    // means each jungle chunk contains
                    // CHUNK_SIZE * CHUNK_SIZE trees
                    u32 CHUNK_SIZE;

                    // chunk size
                    // real chunk size for generating
                    f32 CHUNK_RENDER_DIMENSION;

                    // tree spherical detail
                    // the more the better
                    // but may hit maximum primitive count
                    // if too many used
                    u8 NUM_VERTICES;

                    f32 TREE_DIMENSION;

                    f32 normalScale;

                    Jungle(u32 dimension,
                           u32 chunkSize,
                           u32 treeDimension,
                           u8 verticesDetail,
                           ITerrainSceneNode* heightRef, ISceneManager* mgr, s32 id);
                    virtual ~Jungle();

                    //! pre render
                    virtual void OnRegisterSceneNode();

                    //! Animate grass
                    virtual void OnAnimate(u32 timeMs);

                    //! render
                    virtual void render();

                    //!material count
                    virtual u32 getMaterialCount() const {return 2;}

                    //! get material
                    virtual video::SMaterial& getMaterial(u32 i)
                    {
                        if(i == 0)
                            return Material;
                        return MaterialLeaf;
                    }

                    //!bounding box
                    virtual const core::aabbox3d<f32>& getBoundingBox() const { return Box; }

                    // reset world
                    virtual void reset();

                    //
                    virtual bool treeExistAt(core::vector3df position);


                    virtual void addTreeAt(core::vector3df position,
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
                                            bool instant = true);

                protected:
                private:
                    ITerrainSceneNode* refNode;
                    video::SMaterial Material;
                    video::SMaterial MaterialLeaf;
                    core::aabbox3d<f32> Box;
                    JungleChunk** chunks;
            };
        }
    }
}

#endif // JUNGLE_H
