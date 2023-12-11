/*
  IWindGenerator.h - thank you Klasker :)

  http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=10762&start=15
*/

#ifndef IWINDGENERATOR_H_INCLUDED
#define IWINDGENERATOR_H_INCLUDED

#include <irrlicht.h>
#include <cstdlib>
 
namespace irr
{
namespace scene
{

  class IWindGenerator : public IReferenceCounted
  {
    public:
      virtual void setStrength( f32 strength )     = 0;
      virtual void setRegularity( f32 regularity ) = 0;
      virtual f32 getStrength()                    = 0;
      virtual f32 getRegularity()                  = 0;
      virtual core::vector2df getWind( const core::vector3df& position, u32 timeMs ) = 0;
  };

  IWindGenerator* createWindGenerator(f32 strength=3.0f, f32 regularity=1.0f);

} // scene
} // irr

#endif
