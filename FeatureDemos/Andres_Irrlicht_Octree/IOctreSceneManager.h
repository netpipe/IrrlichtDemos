#ifndef IOCTRESCENEMANAGER_H
#define IOCTRESCENEMANAGER_H

#include <irrlicht.h>

#include "IOctreeNode.h"

namespace irr
{
    namespace scene
    {
        class IOctreSceneManager : public ISceneManager
        {
            public:
                IOctreSceneManager();
                virtual ~IOctreSceneManager();
            protected:
            private:
                IOctreeNode* root;
        };
    }
}

#endif // IOCTRESCENEMANAGER_H
