#ifndef __SKIN_H_INCLUDED__
#define __SKIN_H_INCLUDED__

#include <irrArray.h>
#include <IVideoDriver.h>
#include "iparser.h"
#include "IQ3ModelMesh.h"


namespace irr
{
namespace scene
{
namespace quake3
{

struct STexel
{
    bool operator == ( const STexel &other ) const { return mesh == other.mesh; }

    bool operator < ( const STexel &other ) const { return mesh < other.mesh; }

    STexel()
    {
    }

    STexel(const core::stringc meshName)
        : mesh(meshName)
    {
        mesh.make_lower();
    }

    STexel(const c8 * meshName)
        : mesh(meshName)
    {
        mesh.make_lower();
    }

    STexel(const core::stringc meshName, core::stringc texelName, core::stringc texelPath)
        : mesh(meshName), name(texelName), fullName(texelPath)
    {
        mesh.make_lower();
        name.make_lower();
        fullName.make_lower();
    }

    core::stringc mesh;
    core::stringc name;
    core::stringc fullName;
};

typedef core::array <STexel> texels_t;

typedef core::array <core::stringc> tags_t;

class Skin : public IParser<STexel>
{
public:
    Skin(Script & script, video::IVideoDriver * driver = NULL) : IParser<STexel>(&script), Driver(driver) { parse(); }

    Skin(Script * script, video::IVideoDriver * driver = NULL) : IParser<STexel>(script), Driver(driver) { parse(); }

    Skin(io::IReadFile* file, video::IVideoDriver * driver = NULL, u32 size=0, s32 offset = 0)
          : IParser<STexel>( new Script(file, size, offset) ), Driver(driver) { parse(); }

    Skin(const c8* name, const c8* script, video::IVideoDriver * driver = NULL, u32 size=0, u32 offset = 0)
          : IParser<STexel>( new Script(name, script, size, offset) ), Driver(driver) { parse(); }

    ~Skin() {}

    virtual u32 size() const { return Texels.size(); }

    virtual s32 find(const c8 * name) const { return Texels.binary_search(STexel(name), 0, Texels.size()-1); }

    virtual const STexel * get(u32 index) const
    {
        return ( index < Texels.size() ? &Texels[index] : NULL );
    }

    const texels_t & texels() const { return Texels; }

    const tags_t & tags() const { return Tags; }

    s32 findTag(const c8 * tag) const { return Tags.binary_search(tag, 0, Tags.size()-1); }

    void applyTo(IQ3ModelMesh * mesh);

protected:
    virtual void parse();

private:
    texels_t Texels;
    tags_t Tags;
    video::IVideoDriver * Driver;
};

} // end namespace quake3
} // end namespace scene
} // end namespace irr

#endif


