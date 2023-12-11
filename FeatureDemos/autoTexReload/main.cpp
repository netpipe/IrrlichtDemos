/*
  Copyright (C) 2011 Daniel Sudmann

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Daniel Sudmann suddani@googlemail.com
*/
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include <irrlicht.h>

class TextureReloader
{
public:
    TextureReloader(irr::video::IVideoDriver* d, irr::io::IFileSystem* s)
    {
        driver = d;
        system = s;
    }
    void reloadTexturesOnChange(void)
    {
        for (unsigned texId = 0; texId<driver->getTextureCount(); ++texId)
        {
            irr::video::ITexture* texture = driver->getTextureByIndex(texId);

            irr::core::stringc texfile = texture->getName().getPath();

            if (!system->existFile(texfile)) ///cannot reload memory textures bc i dunno the filename.
                continue;

            time_t cur = getChangedTime(texfile.c_str());

            irr::core::map<irr::video::ITexture*, time_t>::Node* node = ChangeTimes.find(texture);
            if (!node) ///Continue if the file is new and therefor freshly loaded
            {
                ChangeTimes[texture] = cur;
                continue;
            }

            if (cur != ChangeTimes[texture])
            {
                ChangeTimes[texture] = cur;

                irr::io::IReadFile* file = system->createAndOpenFile(texfile.c_str());
                irr::video::IImage* image = driver->createImageFromFile(file);
                file->drop();

                irr::video::ITexture* texdummy = driver->addTexture("#dummyTextureDataForReloading", image);
                image->drop();

                if (texture->getSize() == texdummy->getSize() && texture->getPitch() == texdummy->getPitch() && texture->getColorFormat() == texdummy->getColorFormat())
                {
                    void* target = texture->lock();
                    const void* source = texdummy->lock();
                    memcpy(target, source, texture->getSize().Height*texture->getPitch());
                    texture->unlock();
                    texdummy->unlock();
                    driver->removeTexture(texdummy);
                    texture->regenerateMipMapLevels();
                }
            }
        }
    }

    time_t getChangedTime(const char* filename)
    {
        struct stat buf;
        int result = stat(filename, &buf);
        if (result >= 0)
            return buf.st_mtime;
        return 0;
    }
protected:
    irr::video::IVideoDriver* driver;
    irr::io::IFileSystem* system;
    irr::core::map<irr::video::ITexture*, time_t> ChangeTimes;
};

int main(void)
{
    irr::IrrlichtDevice* device = irr::createDevice(irr::video::EDT_OPENGL);

    irr::video::IVideoDriver* driver = device->getVideoDriver();
    irr::scene::ISceneManager* smgr = device->getSceneManager();

    irr::scene::ISceneNode* box = smgr->addCubeSceneNode();
    box->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    box->setMaterialTexture(0, driver->getTexture("tex.png"));

    TextureReloader reloader(driver, device->getFileSystem());

    smgr->addCameraSceneNode(0, irr::core::vector3df(10), irr::core::vector3df(0));

    irr::u32 time = device->getTimer()->getTime();
    while (device->run())
    {
        irr::u32 curtime = device->getTimer()->getTime();
        if (time < curtime)
        {
            time = curtime+1000;
            reloader.reloadTexturesOnChange();
        }
        driver->beginScene();
        smgr->drawAll();
        driver->endScene();
    }
    device->drop();
    return 0;
}
