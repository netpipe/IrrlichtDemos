#include <os.h>
#include <coreutil.h>
#include "skin.h"

namespace irr
{
namespace scene
{
namespace quake3
{

static const char NODRAW[] = "textures/common/nodraw";

video::ITexture * getNoDrawTexture(video::IVideoDriver * driver)
{
    video::ITexture *Tex;

    if ( (Tex = driver->getTexture(NODRAW)) == NULL )
    {
        os::Printer::log ("Creating NODRAW texture", NODRAW, ELL_INFORMATION);
        Tex = driver->addTexture(core::dimension2d<s32>(32,32), NODRAW);
        c8 * T = (c8 *) Tex->lock();
        if (T)
        {
            memset(T, 0, (Tex->getPitch() * Tex->getSize().Height) );
            Tex->unlock();
            Tex->regenerateMipMapLevels();
        }
        else
        {
            Tex->unlock();
        }
    }

    return Tex;
}

void getV1V2(const core::stringc v, core::stringc & v1, core::stringc & v2)
{
    v1 = ""; v2 = "";

    s32 d;
    if ((d = v.findFirst(',')) < 0) {
       v1 = v;
    }
    else {
        if (d > 0)
           v1 = v.subString(0,d);
        if ((u32)d < v.size()-1)
           v2 = v.subString(d+1, v.size());
    }
}

bool newTag(const core::stringc & v1, const core::stringc & v2, core::stringc & tag)
{
    tag = "";

 // Is it a tag?
    if (v1 != "" && v2 == "") {
       if ( v1.subString(0,4).equals_ignore_case("tag_") ) {
          tag = v1;
          return true;
       }
    }

    return false;
}


bool newTexel(const core::stringc & v1, const core::stringc & v2, STexel & texel)
{
   texel = STexel();

// Is it a texel?
   if (v1 != "" && v2 != "") {
      core::stringc name;
      s32 p;
      if ( (p = v2.findLast('/')) != -1 || (p = v2.findLast('\\')) != -1 )
         name = v2.subString(p+1, v2.size());
      else
         name = v2;

      if (name != "") {
          texel = STexel(v1, name, v2);
          return true;
      }
   }

   return false;
}

void Skin::parse()
{
    getScript()->setAllowLineBreaks(true);

    while ( getScript()->nextToken() != ETK_TOKEN_EOF )
    {
       switch ( getScript()->token().getType() )
       {
           case ETK_TOKEN_TOKEN: // If
           {
              const core::stringc & v = getScript()->token().getValue();

           // Extract two sides of token (separator ",").
              core::stringc v1, v2;
              getV1V2(v, v1, v2);

           // Is it a texel?
              STexel texel;
              if ( newTexel(v1, v2, texel) ) {
                 Texels.push_back( texel ); // It's a skin.
                 continue;
              }

           // Is it a tag?
              core::stringc tag;
              if ( newTag(v1, v2, tag) ) {
                 Tags.push_back(tag);
                 continue; // It's a tag.
              }

           // Bad token.
              Texels.clear();
              Tags.clear();
              os::Printer::log ("Wrong token in skin file, in line", core::stringc( getScript()->line() ).c_str(), ELL_ERROR);
              return;
           }

           case ETK_TOKEN_EOL: // Empty lines.
              continue;

           default:
           {
              Texels.clear();
              Tags.clear();
              os::Printer::log ("Wrong token type in skin file, in line", core::stringc( getScript()->line() ).c_str(), ELL_ERROR);
              return;
           }

       } // switch ( ... )

    } // while ( ... )

    Texels.sort();
    Tags.sort();
}

void Skin::applyTo(IQ3ModelMesh * mesh)
{
//    IMeshBuffer* mb;
    IQ3Surface* mb;
    u32 i;

    if (Driver)
    {
        for (i = 0; i < Texels.size(); i++)
        {
            if (( mb = mesh->getSurface(Texels[i].mesh.c_str()) ) != NULL)
            {
                video::ITexture * tx = NULL;

                if (Texels[i].fullName != NODRAW)
                {
                    tx = Driver->getTexture(Texels[i].fullName.c_str());
                    if (tx == NULL)
                    {
                        core::stringc filename;
                        filename = cutFilenameExtension(filename, Texels[i].fullName);

                        core::stringc extension;
                        extension = getFileNameExtension(extension, Texels[i].fullName);

                        filename += extension.equals_ignore_case(".jpg") ? ".tga" : ".jpg";
                        tx = Driver->getTexture(filename.c_str());
                    }
                }

                if (tx == NULL)
                {
                    tx = getNoDrawTexture(Driver);
                    mb->getMaterial().MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
                    os::Printer::log("Apply texture NODRAW in mesh", Texels[i].mesh.c_str(), ELL_INFORMATION);
                }
                mb->getMaterial().setTexture(0, tx);
            } // if (( mb = mesh->getMeshBuffer
        } // for
    } // if (Driver)

    for (i = 0; i < mesh->getMeshBufferCount(); i++)
    {
        mb = mesh->getSurface(i);
        if (mb->getMaterial().getTexture(0) == NULL)
            mb->getMaterial().MaterialType = video::EMT_TRANSPARENT_ADD_COLOR;
    }
}

} // end namespace quake3
} // end namespace scene
} // end namespace irr

