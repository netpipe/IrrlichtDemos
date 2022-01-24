
//by omaremad
#include <irrlicht.h>
#include <iostream>

using namespace irr;
#include <pthread.h>

scene::ISceneManager* smgr;
//a struct to carry arguemnts across threads
struct arguments
{
    c8* name;
    scene::IAnimatedMesh* oldMesh;
    scene::IAnimatedMesh** smesh;
    core::aabbox3d<f32>* sBox;
    scene::ISceneManager* ssmgr;
    volatile bool* switching;

};
//lod struct for setting distances and meshes
struct lodLevel
{
    c8* meshName;
    float distance;
};

arguments argt;

//allows one streamer thread at a time.
volatile bool switching=false;

void isswitching(bool in)
{

    switching=in;

};

class CStrreamedLODSceneNode : public scene::ISceneNode
{
    pthread_t thread;

    core::aabbox3d<f32> Box;
    video::SMaterial Material;
    scene::IAnimatedMesh* mesh;
    scene::IAnimatedMesh* streamedMesh;
    scene::ISceneManager* mgr;

    int currentLOD;

    core::array<lodLevel> levels;
    volatile bool switchMeshes;
    bool ready;

public:

    CStrreamedLODSceneNode(scene::ISceneNode* parent, scene::ISceneManager* smgr, s32 id)
            : scene::ISceneNode(parent, smgr, id)
    {
        mgr=smgr;
        Material.Wireframe = false;
        Material.Lighting = false;
        //Material.MaterialType=video::EMT_LIGHTMAP;
        mesh=mgr->getMesh("2.obj");
        mesh->grab();


        lodLevel Low;
        Low.meshName="2.obj";
        Low.distance=0;

        lodLevel High;
        High.meshName="1.obj";
        High.distance=500;

        levels.push_back(Low);
        levels.push_back(High);
        currentLOD=0;
        Box=mesh->getMesh(0)->getBoundingBox();
        switchMeshes=false;
        ready=true;

    }

    virtual ~CStrreamedLODSceneNode()
    {
        mesh->drop();
    }

    virtual void OnRegisterSceneNode()
    {
        if (IsVisible)
        {
            SceneManager->registerNodeForRendering(this);
        }

        ISceneNode::OnRegisterSceneNode();
    }


    virtual void render()
    {
        //LOD logic
        handleLOD();
        video::IVideoDriver* driver = SceneManager->getVideoDriver();

        driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);

        //If there is freshly streamed data use it instead of current data
        if (switchMeshes)
            switchData();

        for (int i =0; i<mesh->getMesh(0)->getMeshBufferCount();i++)
        {
            mesh->getMesh(0)->getMeshBuffer(i)->getMaterial().Lighting=false;
            driver->setMaterial(mesh->getMesh(0)->getMeshBuffer(i)->getMaterial());
            driver->drawMeshBuffer(mesh->getMesh(0)->getMeshBuffer(i));
        }

    }


    virtual const core::aabbox3d<f32>& getBoundingBox() const
    {
        return Box;
    }

    virtual u32 getMaterialCount() const
    {
        return 1;
    }

    virtual video::SMaterial& getMaterial(u32 i)
    {
        return Material;
    }

    void handleLOD()
    {
        //NB makesure your mesh is centered around the origin, if not use the bounding box
        //centre instead of getPosition()
        core::vector3df camPos= mgr->getActiveCamera()->getPosition();
        float distance = core::vector3df(camPos-getPosition()).getLength();
        int requiredLevel=0;

        for (int i=0; i<levels.size();i++)
        {
            if (distance>levels[i].distance)
            {
                requiredLevel=i;

            }

        }
        if (requiredLevel!=currentLOD)
        {
            //If failed then another node instance is streaming.
            //Streaming will happen the consequent frame since currentLOD is still != requiredLevel
            if (streamMesh(levels[requiredLevel].meshName))
                currentLOD=requiredLevel;

        }

    }

    bool streamMesh(c8* name)
    {


        argt.name = name;
        argt.oldMesh = mesh;
        argt.smesh = &streamedMesh;
        argt.sBox= &Box;
        argt.ssmgr = this->mgr;
        argt.switching=&switchMeshes;

        //while (CStrreamedLODSceneNode::switching){};
        if (!switching)
        {
            ready = false;
            this->mgr->getMesh(name);
            pthread_create( &thread, NULL, callback, (void*)&argt);
            return true;
        }
        else return false;

    }

    void switchData()
    {


        mesh->drop();

        if (mesh->getReferenceCount()==1)
        {

            for (int i =0; i<mesh->getMesh(0)->getMeshBufferCount();i++)
            {
                //clean out unsed textures (by layer)
                for (int t=0;t<4;t++)
                {
                    video::ITexture* tex=mesh->getMesh(0)->getMeshBuffer(i)->getMaterial().getTexture(t);
                    if (tex)
                    {
                        int cnt=tex->getReferenceCount();
                        if (cnt==1)
                            mgr->getVideoDriver()->removeTexture(mesh->getMesh(0)->getMeshBuffer(i)->getMaterial().getTexture(t));
                    }
                }
            }
            mgr->getMeshCache()->removeMesh(mesh);
        }

        mesh=streamedMesh;

        switchMeshes=false;

        ready = true;

    }


    static void* callback(void* iarg)
    {
        isswitching(true);
        arguments ar;
        ar=*(arguments*)iarg;
        printf("Streaming\n");

        scene::IAnimatedMesh* tempMesh = ar.ssmgr->getMesh(ar.name);
        *ar.smesh=tempMesh;

        tempMesh->grab();

        isswitching(false);
        *ar.switching=true;

        if (!pthread_detach( pthread_self()) )
            return NULL;
    }


};

int main()
{
    // let user select driver type

    video::E_DRIVER_TYPE driverType;

    printf("Please select the driver you want for this example:\n"\
           " (c) OpenGL 1.5\n"\
           " (d) Software Renderer\n (e) Burning's Software Renderer\n"\
           " (f) NullDevice\n (otherKey) exit\n\n");

    char i;
    std::cin >> i;

    switch (i)
    {

    case 'c':
        driverType = video::EDT_OPENGL;
        break;
    case 'd':
        driverType = video::EDT_SOFTWARE;
        break;
    case 'e':
        driverType = video::EDT_BURNINGSVIDEO;
        break;
    case 'f':
        driverType = video::EDT_NULL;
        break;
    default:
        return 0;
    }

    // create device

    IrrlichtDevice *device =
        createDevice(driverType, core::dimension2d<u32>(640, 480), 32, false);

    if (device == 0)
        return 1; // could not create selected driver.

    // create engine and camera

    device->setWindowCaption(L"Streamed io and LOD -Omaremad");

    video::IVideoDriver* driver = device->getVideoDriver();
    smgr = device->getSceneManager();

    smgr->addCameraSceneNodeFPS(0)->setPosition(core::vector3df(100,100,100));


    CStrreamedLODSceneNode *myNode =
        new CStrreamedLODSceneNode(smgr->getRootSceneNode(), smgr,4);

    CStrreamedLODSceneNode *myNode2 =
        new CStrreamedLODSceneNode(smgr->getRootSceneNode(), smgr,5);




    myNode2->setScale(core::vector3df(50,50,50));
    myNode->setScale(core::vector3df(50,50,50));


    u32 frames=0;
    while (device->run())
    {
        driver->beginScene(true, true, video::SColor(0,100,0,100));

        smgr->drawAll();

        driver->endScene();
        if (++frames==100)
        {
            core::stringw str = L"Irrlicht Engine [";
            str += driver->getName();
            str += L"] FPS: ";
            str += (s32)driver->getFPS();

            device->setWindowCaption(str.c_str());
            frames=0;
        }
    }


    myNode->drop();
    myNode2->drop();
    device->drop();
    pthread_exit(0);
    return 0;
}

