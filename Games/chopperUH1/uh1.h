#ifndef _UH1_SCENE_NODE_H_
#define _UH1_SCENE_NODE_H_
#include <irrlicht.h>
using namespace irr;
using namespace scene;
#ifndef UH1_SCENE_NODE_PROPERTIES
#define UH1_SCENE_NODE_PROPERTIES
#define UH1_MODEL_DIR "/home/smso/downloads/irrlicht/aircraft/UH-1/Models/"
#endif
class Uh1SceneNode: public scene::IMeshSceneNode
{
        public:

                	virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh=0,
		s32 id=-1, bool zfailmethod=true, f32 infinity=1000.0f) {};

        Uh1SceneNode
        (
                scene::ISceneNode* parent,
                scene::ISceneManager* smgr,
                s32 id = -1,
                const core::vector3df& position = core::vector3df(0.0f),
                const core::vector3df& rotation = core::vector3df(0.0f),
                const core::vector3df& scale = core::vector3df(1.0f)
        );

        virtual ~Uh1SceneNode();
        virtual void setPosition(const core::vector3df& newpos);
        virtual core::vector3df getAbsolutePosition() const;
        virtual void setMesh(scene::IMesh* mesh);
        virtual scene::IMesh* getMesh(void);
        virtual void setReadOnlyMaterials(bool readonly);
        virtual bool isReadOnlyMaterials() const;
        virtual void OnRegisterSceneNode();
        virtual void render();
        virtual const core::aabbox3d<f32>& getBoundingBox() const;
        virtual u32 getMaterialCount() const;
        virtual video::SMaterial& getMaterial(u32 i);
        virtual void OnAnimate(u32 timeMs);
        virtual core::matrix4 getRelativeTransformation() const;
        void drawDownwardLinesFromBB();
        void drawVerticalLine();
        core::vector3df getMatrixRotation(const core::matrix4& mx);
        void update(u32 deltaTime);
        bool isVisible() const
        {
                return Body->isVisible();
        }

virtual int addShadowVolumeSceneNode() {return 1;};

        // setters + getters:
        //void setModel(Ka50Model* model) { Model = model; }

        scene::ISceneNode* getRootEmpty() const { return RootEmpty; }
        scene::IMeshSceneNode* getBody() const { return Body; }
        scene::IMeshSceneNode* getInterior() const { return Interior; }
        scene::IMeshSceneNode* getMainRotor() const { return MainRotor; }
        scene::ISceneNode* getTailRotorEmpty() const { return TailRotorEmpty; }
        scene::ISceneNode* getTailRotor() const { return TailRotor; }
        core::vector3df getTailRotorOffset() const { return TailRotorOffset; }
        void setTailRotorOffset(const core::vector3df& offset);
        core::vector3df getMainRotorOffset() const { return MainRotorOffset; }
        void setMainRotorOffset(const core::vector3df& offset);
        core::vector3df getTailBladeEmptyOffset() const { return TailBladeEmptyOffset; }
        void setTailBladeEmptyOffset(const core::vector3df& offset);
        protected:

        private:
        //IrrlichtDevice* _device;
        scene::ISceneManager* Smgr;
        scene::ISceneNode* RootEmpty;
        scene::IMeshSceneNode* Body;
        scene::IMesh* BodyMesh;
        scene::IMeshSceneNode* Interior;
        scene::ISceneNode* MainRotorEmpty;
        scene::IMeshSceneNode* MainRotor;
        core::vector3df MainRotorOffset;
        scene::ISceneNode* TailRotorEmpty;
        scene::IMeshSceneNode* TailRotor;
        core::vector3df TailRotorOffset;
        scene::ISceneNode* MainBladeEmpty;
        scene::ISceneNode* TailBladeEmpty;
        core::vector3df TailBladeEmptyOffset;

        scene::IMeshSceneNode* Blade;
        core::aabbox3d<f32> Box;
        video::SMaterial BodyMaterial;
        video::SMaterial MainRotorMaterial;
        video::SMaterial TailRotorMaterial;
};
#endif // _UH1_SCENE_NODE_H_
