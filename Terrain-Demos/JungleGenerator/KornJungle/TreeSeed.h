#ifndef TREESEED_H
#define TREESEED_H

#include <irrlicht.h>

namespace irr
{
    namespace scene
    {
        namespace jungleScene
        {

            class TreeSeed
            {
                public:
                    u8 segment_count;
                    u8 bark_type;
                    f32 min_rotation;
                    f32 max_rotation;
                    f32 branch_length;
                    u8 branch_count;
                    u8 seed;
                    u16 age;
                    u32 time;
                    f32 Y;
                    f32 max_radius;
                    f32 leaf_width;
                    f32 leaf_length;
                    f32 leaf_segment;
                    f32 leaf_stiffness;
                    u8 leaf_type;
                    u8 ground_root_count;
                    bool active;
                    //

                    TreeSeed();
                    virtual ~TreeSeed();
                protected:
                private:


            };
        }
    }
}

#endif // TREESEED_H
