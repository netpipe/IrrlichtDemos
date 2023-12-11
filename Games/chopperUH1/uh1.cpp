#include "uh1.h"
using namespace irr;

float rotSpeed = 0.1f;
Uh1SceneNode::Uh1SceneNode
(
        scene::ISceneNode* parent,
        scene::ISceneManager* smgr,
        s32 id,
        const core::vector3df& position,
        const core::vector3df& rotation,
        const core::vector3df& scale
)
:       scene::IMeshSceneNode(parent ? parent : smgr->getRootSceneNode(), smgr, 0, position, rotation, scale),
        Smgr(smgr),
        RootEmpty(0),
        Body(0),
        BodyMesh(0),
        Interior(0),
        MainRotor(0),
        TailRotorEmpty(0),
        TailRotor(0),
        TailRotorOffset(-4.271f, 1.555f, 0.473f),
        MainRotorOffset(4.767f, 1.0f, 0.0f),
        //MainRotorOffset(0.0f),
        MainBladeEmpty(0),
        TailBladeEmpty(0),
        TailBladeEmptyOffset(0.0f),
        Blade(0)
{
        Smgr->getFileSystem()->addFileArchive(UH1_MODEL_DIR);
        // parent-child relationship:
        // root -> body -> main rotor empty -> main rotor -> main blade empty -> main blades
        //              -> tail rotor empty -> tail rotor -> tail blade empty -> tail blades
        // root -> interior
        // setup root: for correcting orientation of model!
        RootEmpty = Smgr->addEmptySceneNode(Parent);
        // setup body:
        BodyMesh = Smgr->getMesh("Models/uh1.obj");
        Body = Smgr->addMeshSceneNode(BodyMesh, RootEmpty);
        // workaround to correct for yaw-, roll- and pitch-controls:
        Body->setRotation(core::vector3df(0.0f, -90.0f, 0.0f));
        //Body->setDebugDataVisible(scene::EDS_BBOX);
        //printf("Body->getMaterialCount()=%u\n", Body->getMaterialCount());
        for (u32 n=0; n<Body->getMaterialCount(); ++n)
        {
                Body->getMaterial(n).setFlag(video::EMF_LIGHTING, false);
                Body->getMaterial(n).NormalizeNormals = true;
        }
        BodyMaterial = Body->getMaterial(0);
        Box = Body->getBoundingBox();
        // setup interior:
        Interior = Smgr->addMeshSceneNode(Smgr->getMesh("Models/Interior/interior.obj"), RootEmpty);
        Interior->setRotation(core::vector3df(0.0f, -90.0f, 0.0f));
        Interior->setVisible(false);
        // setup main rotor:
        MainRotorEmpty = Smgr->addEmptySceneNode(Body);
        MainRotorEmpty->setRotation(core::vector3df(0.0f, 0.0f, -5.0f));
        MainRotor = Smgr->addMeshSceneNode(Smgr->getMesh("Models/MainRotor/mainrotor.obj"), MainRotorEmpty);
        for (u32 n=0; n<MainRotor->getMaterialCount(); ++n)
        {
                MainRotor->getMaterial(n).setFlag(video::EMF_LIGHTING, false);
                MainRotor->getMaterial(n).NormalizeNormals = true;
        }
        MainRotor->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        Box.addInternalBox(MainRotor->getBoundingBox());
        this->setMainRotorOffset(MainRotorOffset);
        ////////////////////////////////////////////////////////////////////////////
        // setup tail rotor:
        TailRotorEmpty = Smgr->addEmptySceneNode(Body);
        TailRotorEmpty->setRotation(core::vector3df(90.0f, 0.0f, 0.0f));
        TailRotor = Smgr->addMeshSceneNode(Smgr->getMesh("Models/TailRotor/tailrotor.obj"), TailRotorEmpty);
        TailRotor->getMaterial(0).setFlag(video::EMF_LIGHTING, false);
        TailRotor->getMaterial(0).NormalizeNormals = true;
        TailRotor->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
        Box.addInternalBox(TailRotor->getBoundingBox());
        this->setTailRotorOffset(TailRotorOffset);
        ////////////////////////////////////////////////////////////////////////////
        // setup tail blades:
        TailBladeEmpty = Smgr->addEmptySceneNode(TailRotor);
        //TailBladeEmpty->setPosition(core::vector3df(3.047f, 0.811f, 0.0f));
        TailBladeEmpty->setPosition(core::vector3df(0.0f, -0.1f, 0.0f));
        for (u32 n=0; n<2; ++n)
        {
                Blade = Smgr->addMeshSceneNode(Smgr->getMesh("Models/TailRotor/blade.obj"), TailBladeEmpty);
                //Blade->setRotation(core::vector3df(0.0f, 0.0f, 0.0f));
                Blade->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                //Blade->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                Blade->getMaterial(0).setFlag(video::EMF_LIGHTING, false);
                Blade->getMaterial(0).NormalizeNormals = true;
                Box.addInternalBox(Blade->getBoundingBox());
                if (n==1)
                        Blade->setRotation(core::vector3df(0.0f, 180.0f, 0.0f));
        }
        ////////////////////////////////////////////////////////////////////////////
        // setup main blades:
        MainBladeEmpty = Smgr->addEmptySceneNode(MainRotor);
        //MainBladeEmpty->setPosition(core::vector3df(0.0f, -0.1f, 0.0f));
        for (u32 n=0; n<2; ++n)
        {
                Blade = Smgr->addMeshSceneNode(Smgr->getMesh("Models/MainRotor/blade.obj"), MainBladeEmpty);
                //Blade->setRotation(core::vector3df(0.0f, 0.0f, 0.0f));
                Blade->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
                //Blade->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
                Blade->getMaterial(0).setFlag(video::EMF_LIGHTING, false);
                Blade->getMaterial(0).NormalizeNormals = true;
                Box.addInternalBox(Blade->getBoundingBox());
                if (n==1)
                        Blade->setRotation(core::vector3df(0.0f, 180.0f, 0.0f));
        }
        //this->getMaterial(0).setFlag(video::EMF_LIGHTING, false);
}
Uh1SceneNode::~Uh1SceneNode()
{
}
void Uh1SceneNode::setPosition(const core::vector3df& newpos)
{
        if (!RootEmpty)
                return;
        RootEmpty->setPosition(newpos);
}
core::vector3df Uh1SceneNode::getAbsolutePosition() const
{
        if (!RootEmpty)
                return core::vector3df(0.0f);
        return RootEmpty->getAbsolutePosition();
}
void Uh1SceneNode::setMesh(scene::IMesh* mesh)
{
}
scene::IMesh* Uh1SceneNode::getMesh(void)
{
        return BodyMesh;
}
void Uh1SceneNode::setReadOnlyMaterials(bool readonly)
{
}
bool Uh1SceneNode::isReadOnlyMaterials() const
{
        return true;
}
void Uh1SceneNode::OnRegisterSceneNode()
{
        if (IsVisible)
        {
                SceneManager->registerNodeForRendering(Body);
                SceneManager->registerNodeForRendering(MainRotor);
                SceneManager->registerNodeForRendering(TailRotor);
        }
        scene::IMeshSceneNode::OnRegisterSceneNode();
}
void Uh1SceneNode::render()
{
}
//FIXME:
const core::aabbox3d<f32>& Uh1SceneNode::getBoundingBox() const
{
        return Box;
}
u32 Uh1SceneNode::getMaterialCount() const
{
        return 3;
}
video::SMaterial& Uh1SceneNode::getMaterial(u32 i)
{
        if (i == 0)
                return BodyMaterial;
        else if (i == 1)
                return MainRotorMaterial;
        else if (i == 2)
                return TailRotorMaterial;
        else
                return BodyMaterial;
}
void Uh1SceneNode::OnAnimate(u32 timeMs)
{
}
core::matrix4 Uh1SceneNode::getRelativeTransformation() const
{
        return Body->getRelativeTransformation();
}

void Uh1SceneNode::update(u32 deltaTime)
{
        f32 rotRate = 180.0f; // degs per second:
        f32 deltaRot = rotRate * (f32)deltaTime * rotSpeed;
        core::vector3df rot1 = TailRotor->getRotation();
        rot1.Y -= deltaRot;
        TailRotor->setRotation(rot1);
        core::vector3df rot2 = MainRotor->getRotation();
        //rot2.Y += deltaRot;
        rot2.Y -= deltaRot;
        MainRotor->setRotation(rot2);
}
void Uh1SceneNode::setTailRotorOffset(const core::vector3df& offset)
{
        TailRotorOffset = offset;
        //TailRotor->setPosition(offset);
        TailRotorEmpty->setPosition(offset);
}
void Uh1SceneNode::setMainRotorOffset(const core::vector3df& offset)
{
        MainRotorOffset = offset;
        //MainRotor->setPosition(offset);
        MainRotorEmpty->setPosition(offset);
}
void Uh1SceneNode::setTailBladeEmptyOffset(const core::vector3df& offset)
{
        TailBladeEmptyOffset = offset;
        TailBladeEmpty->setPosition(offset);
}
