#ifndef JUNGGLECHUNK_H
#define JUNGGLECHUNK_H

#include <irrlicht.h>
#include "TreeSeed.h"

namespace irr
{
    namespace scene
    {
        namespace jungleScene
        {
            class Jungle;
            class JungleChunk
            {
                public:
                    f32 X;
                    f32 Y;
                    f32 Z;
                    Jungle* jungle;
                    ITerrainSceneNode* heightRef;
                    JungleChunk();
                    virtual ~JungleChunk();
                    virtual void setup();
                    virtual void update(u32 timeMs);
                    virtual void renderBark(video::IVideoDriver * driver);
                    virtual void renderLeaf(video::IVideoDriver * driver);
                    virtual bool isRenderable(){return renderable;}
                    virtual bool treeExistAt(core::vector3df position);
                    virtual void reset();
                    virtual void growTreeAt(core::vector3df position,
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
                                            u16 age);
                protected:
                private:
                    bool renderable;
                    bool renderableLow;
                    bool needUpdate;
                    TreeSeed** seeds;

                    core::array<video::S3DVertexTangents > vertexBuffer;
                    core::array<u16> indexBuffer;
                    core::array<video::S3DVertexTangents> vertexBufferLeaf;
                    core::array<u16> indexBufferLeaf;

                    virtual bool createTreeGeom(core::vector3df position,
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
                                            u16 age);

                    virtual bool createRootGeom(core::vector3df position,
                                            u8 segment_count,
                                            f32 min_rotation,
                                            f32 max_rotation,
                                            f32 branch_length,
                                            u8 branch_count,
                                            f32 max_radius,
                                            core::vector3df initialRotation,
                                            u8 bark_type,
                                            u8 seed,
                                            u16 age);

                    virtual void createLeaf(
                                            core::vector3df position,
                                            core::vector3df initialRotation,
                                            f32 wid,
                                            f32 len,
                                            u8 segment,
                                            f32 stiffness,
                                            u8 type);

                    virtual void clearBuffer();
            };
        }
    }
}

#endif // JUNGGLECHUNK_H
