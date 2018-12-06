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
#include "IrrRocketFileInterface.h"
#include <IFileSystem.h>
#include <IReadFile.h>

IrrRocketFileInterface::IrrRocketFileInterface(irr::io::IFileSystem* fileSystem) : FileSystem(fileSystem)
{
    //ctor
}

IrrRocketFileInterface::~IrrRocketFileInterface()
{
    //dtor
}

Rocket::Core::FileHandle IrrRocketFileInterface::Open(const Rocket::Core::String& path)
{
    //printf("File open: %s\n", path.CString());
    return (Rocket::Core::FileHandle)FileSystem->createAndOpenFile(path.CString());
}

void IrrRocketFileInterface::Close(Rocket::Core::FileHandle file)
{
    //printf("File close\n");
    if (!file)
        return;
    irr::io::IReadFile* rfile = (irr::io::IReadFile*)file;
    rfile->drop();
}

size_t IrrRocketFileInterface::Read(void* buffer, size_t size, Rocket::Core::FileHandle file)
{
    //printf("File read\n");
    if (!file)
        return 0;
    irr::io::IReadFile* rfile = (irr::io::IReadFile*)file;
    return rfile->read(buffer, size);
}

bool IrrRocketFileInterface::Seek(Rocket::Core::FileHandle file, long offset, int origin)
{
    //printf("File seek: %i : %i\n", offset, origin);
    if (!file)
        return false;
    irr::io::IReadFile* rfile = (irr::io::IReadFile*)file;
    if (origin == SEEK_SET)
        return rfile->seek(offset, false);
    else if (origin == SEEK_CUR)
        return rfile->seek(offset, true);
    //printf("Seek end--------------------------------------------\n");
    rfile->seek(rfile->getSize(), false);
    return rfile->seek(offset, true);
}

size_t IrrRocketFileInterface::Tell(Rocket::Core::FileHandle file)
{
    //printf("File tell\n");
    if (!file)
        return 0;
    irr::io::IReadFile* rfile = (irr::io::IReadFile*)file;
    return rfile->getPos();
}
