#ifndef _GRASS_LOADER_H_
#define _GRASS_LOADER_H_

#include <irrlicht.h>

using namespace irr;

namespace GrassLoader
{

class GrassQuadTreeNode
{
        GrassQuadTreeNode* children[4];
    public:
        virtual GrassQuadTreeNode** getChildren() {return children;}
        virtual void updateCulling(scene::ICameraSceneNode* cam)
        {
            cam->updateAbsolutePosition();
            cam->render();
        }
};

class GrassQuadTreeRoot
{
        core::array<GrassQuadTreeNode*> nodes;
    public:
        void addNode(GrassQuadTreeNode* node) {nodes.push_back(node);}
        GrassQuadTreeNode** getNodes() {return nodes.pointer();}
};

template <typename T>
class GrassQuadTreeLeaf : public scene::ISceneNode, GrassQuadTreeNode
{
        u32 LoDLevels;
        core::array<f32> LoDDistances;
        core::aabbox3df bbox;
        T* Verts;
        u32 vCount;
        u16* Indices[6];
        u32 Icount[6];
        video::SMaterial material;
    public:
        virtual GrassQuadTreeNode** getChildren() {return NULL;}
		virtual const core::aabbox3d<float>& getBoundingBox() const {return bbox;}
        GrassQuadTreeLeaf(scene::ISceneNode* parent, scene::ISceneManager* mgr,
                            T* verts, u32 vcount,
                            u16* indices0, u32 icount0,
                            u16* indices1, u32 icount1,
                            u16* indices2, u32 icount2,
                            u16* indices3, u32 icount3,
                            u16* indices4, u32 icount4,
                            u16* indices5, u32 icount5,
                            u32 levels )
                : scene::ISceneNode(parent,mgr), LoDLevels(levels), vCount(vcount)
        {
            for (u32 i=0; i<levels; i++)
                LoDDistances.push_back(-1.f);
            Icount[0] = icount0;
            Icount[1] = icount1;
            Icount[2] = icount2;
            Icount[3] = icount3;
            Icount[4] = icount4;
            Icount[5] = icount5;
            Verts = new T[vCount];
            memcpy(Verts,verts,vCount*sizeof(T));
            for (u32 i=0; i<vCount; i++)
                bbox.addInternalPoint(Verts[i].Pos);
            Indices[0] = new u16[Icount[0]];
            memcpy(Indices[0],indices0,Icount[0]*2);
            if (indices1)
            {
                Indices[1] = new u16[Icount[1]];
                memcpy(Indices[1],indices1,Icount[1]*2);
            }
            if (indices2)
            {
                Indices[2] = new u16[Icount[2]];
                memcpy(Indices[2],indices2,Icount[2]*2);
            }
            if (indices3)
            {
                Indices[3] = new u16[Icount[3]];
                memcpy(Indices[3],indices1,Icount[3]*2);
            }
            if (indices4)
            {
                Indices[4] = new u16[Icount[4]];
                memcpy(Indices[4],indices4,Icount[4]*2);
            }
            if (indices5)
            {
                Indices[5] = new u16[Icount[5]];
                memcpy(Indices[5],indices5,Icount[5]*2);
            }
        }
        ~GrassQuadTreeLeaf()
        {
            if (Verts)
                delete [] Verts;
            for (u32 i=0; i<6; i++)
            {
                if (Indices[i])
                    delete [] Indices[0];
            }
        }
        virtual void setLoDDistances(core::array<f32> distances)
        {
            LoDDistances=distances;
        }
        virtual video::SMaterial& getMaterial() {return material;}
		virtual void OnRegisterSceneNode()
		{
		    SceneManager->registerNodeForRendering(this);
		    scene::ISceneNode::OnRegisterSceneNode();
		}
        virtual void render()
        {
            video::IVideoDriver* driver = SceneManager->getVideoDriver();
            scene::ICameraSceneNode* cam = SceneManager->getActiveCamera();
            if (!driver||!cam)
                return;
            driver->setTransform(video::ETS_WORLD,AbsoluteTransformation);
            driver->setMaterial(material);
            core::vector3df camPos = cam->getAbsolutePosition();
            core::aabbox3df tbbox = getTransformedBoundingBox();
            if (!tbbox.isPointInside(camPos))
            {
                core::vector3df edges[8];
                f32 smallest_dist = cam->getFarValue()*cam->getFarValue();
                tbbox.getEdges(edges);
                for (u32 i=0; i<8; i++)
                {
                    edges[i] -= camPos;
                    f32 temp_dist = edges[i].getLengthSQ();
                    if (temp_dist<smallest_dist)
                        smallest_dist = temp_dist;
                }
                smallest_dist = sqrtf(smallest_dist);
                for (u32 i=0; i<LoDDistances.size()&&i<LoDLevels&&i<6; i++)
                {
                    if (LoDDistances[i]<0.f)
                        break;
                    if (!Indices[i])
                        break;
                    if (smallest_dist<LoDDistances[i])
                    {
                        driver->drawIndexedTriangleList(Verts,vCount/u32(pow(2.f,i)),Indices[i],Icount[i]/3);
                        break;
                    }
                }
            }
            else
                driver->drawIndexedTriangleList(Verts,vCount,Indices[0],Icount[0]/3);
        }
};


void loadGrass(const io::path &filename, scene::ISceneManager* smgr,
                f32 LoD0, f32 LoD1, f32 LoD2, f32 LoD3, f32 LoD4, f32 LoD5);

}

#endif
