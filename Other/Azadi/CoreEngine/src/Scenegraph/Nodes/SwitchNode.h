#ifndef SWITCHNODE_H
#define SWITCHNODE_H

#include "../BaseNode.h"

namespace SceneGraphNodes
{
    class SwitchNode : public BaseNode
    {
        public:
            SwitchNode(const bool &flag = false);
            ~SwitchNode();

            void execute();
        private:
            bool flag;
    };
};

#endif
