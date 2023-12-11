#ifndef ROOTNODE_H
#define ROOTNODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class RootNode : public BaseNode
    {
        public:
            RootNode();
            ~RootNode();

            void execute();
    };
};

#endif
