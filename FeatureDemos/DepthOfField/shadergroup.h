#ifndef SHADERGROUP_H
#define SHADERGROUP_H
#include "shadermaterial.h"
#include <irrlicht.h>
#include <list>
using std::list;
using namespace irr;


class ShaderGroup
{
    public:
        float sampleDist0;
        float sampleDist1;
        float range;
        float focus;
        float distanceScale;

        ShaderGroup(IrrlichtDevice* m_device, scene::ISceneManager* m_smgr);
        virtual ~ShaderGroup();
        int render();
        void add(scene::ISceneNode* node);

    protected:
    private:
        IrrlichtDevice* device;
        scene::ISceneManager* smgr;
        video::IGPUProgrammingServices*  gpu;
        scene::ISceneNode* postProcessorNode;
        video::ITexture* rt;
        ShaderMaterial* distance;
        ShaderMaterial* blur0;
        ShaderMaterial* blur1;
        ShaderMaterial* combine;
        list <scene::ISceneNode*> shaded;
};

#endif // SHADERGROUP_H
