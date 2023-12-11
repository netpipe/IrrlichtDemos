#include <irrlicht.h>
#include "GrassLoader/GrassLoader.h"

using namespace irr;

//the modified main() function which allows to input parameters like the path to the input XML
int main(int argc,char** argv)
{
    IrrlichtDevice* device = createDevice(video::EDT_OPENGL,core::dimension2du(1024,768));
    // Here I retrieve the handle to the VideoDriver interface which draws everything
    video::IVideoDriver* driver = device->getVideoDriver();
    // The scene manager loads models and draws the scene correctly (does frustum culling etc.)
    scene::ISceneManager* smgr =
     device->getSceneManager();


    //Here we load the arguments, specifying where the file is
    core::stringc filePath = "./output.grs";
    if (argc>0) //If there are any arguments
    {
        core::stringc temp = core::stringc(argv[1]);
        if (temp.size()>4) //has a file extension
        {
            if (device->getFileSystem()->existFile(temp))
                filePath=temp;
        }
    }
    scene::ICameraSceneNode* cam = smgr->addCameraSceneNodeFPS(0,32.f,0.0125);
    //Add the directory where the grass textures are
    device->getFileSystem()->addFolderFileArchive("../../GrassGen/vegData/");
    GrassLoader::loadGrass(filePath,smgr,
                            16.f,8.f,36.f,
                            40.f,43.f,46.f);

    //We load the terrain too
    scene::IMesh* mesh = smgr->getMesh("../../GrassGen/vegData/terrain.ms3d");
    if (mesh)
    {
        mesh->getMeshBuffer(0)->getMaterial().setTexture(0,driver->getTexture("./grassfloor.jpg"));
        //mesh->getMeshBuffer(0)->getMaterial().Lighting = false;
        smgr->addMeshSceneNode(mesh);
    }

    scene::ILightSceneNode* light = smgr->addLightSceneNode(0,core::vector3df(0,1.f,0));
    light->getLightData().Position = core::vector3df(0.3,1.f,0.3).normalize();
    light->getLightData().AmbientColor = video::SColorf(0.4,0.4,0.4,1.f);
    light->setLightType(video::ELT_DIRECTIONAL);

    int lastFPS = -1;
    while (device->run())
    {
        //This flushes the whole window with a fully opaque greyish color RGB = (200,200,200)
        //It also enables us to draw
        driver->beginScene(true,true,video::SColor(255,200,200,200));
        smgr->drawAll();
        driver->endScene(); // Presents the buffer in the window - on Screen

        int fps = driver->getFPS();
        if (lastFPS != fps)
        {
            core::stringw str = L"Front Warrior GrassGenerator Usage Example - FPS:";
            str += fps;
            str += " Trigs:";
            str += driver->getPrimitiveCountDrawn();

            device->setWindowCaption(str.c_str());
            lastFPS = fps;
        }
    }

    device->closeDevice();
    device->drop();
    printf("Still Alive.\n");

    return 0;
}
