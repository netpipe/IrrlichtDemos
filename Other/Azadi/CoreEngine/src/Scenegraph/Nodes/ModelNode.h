#ifndef MODELNODE_H
#define MODELNODE_H

#include "../BaseNode.h"
#include "../../ModelFactory/ModelFactory.h"

namespace SceneGraphNodes
{
    class ModelNode : public BaseNode
    {
        public:
            ModelNode(const unsigned long int &ID);
            ~ModelNode();

            void execute();
        private:
            unsigned long int modelID;
    };
};

#endif
