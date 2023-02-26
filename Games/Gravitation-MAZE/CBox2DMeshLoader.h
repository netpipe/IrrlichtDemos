#include <IMeshLoader.h>
#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifndef __CBOX2DMESHLOADER
  #define __CBOX2DMESHLOADER

class CBox2DMeshLoader : public IMeshLoader {
  private:
    IVideoDriver *m_pDriver;

  public:
    CBox2DMeshLoader(IVideoDriver *pDriver);
    virtual ~CBox2DMeshLoader();
   // bool isALoadableFileExtension(const irr::c8 *fileName) const;
 	bool isALoadableFileExtension(const io::path& filename) const;

    virtual IAnimatedMesh *createMesh(io::IReadFile *file);
};

#endif
