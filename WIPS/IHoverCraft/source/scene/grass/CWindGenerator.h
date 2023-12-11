/*
  CWindGenerator.h - thank you Klasker :)
  http://irrlicht.sourceforge.net/phpBB2/viewtopic.php?t=10762&start=15
*/

#include "IWindGenerator.h"

namespace irr
{
namespace scene
{

  class CWindGenerator : public IWindGenerator
  {
    private:
      f32 Strength;
      f32 Regularity;
 
    public:
      void setStrength( f32 strength );
      f32  getStrength();
      
      void setRegularity( f32 regularity );
      f32  getRegularity();

      core::vector2df getWind( const core::vector3df& position, u32 timeMs );
  }; 

  void CWindGenerator::setStrength    ( f32 strength   ) { Strength   = strength;   }
  f32  CWindGenerator::getStrength()  { return Strength; }
  void CWindGenerator::setRegularity  ( f32 regularity ) { Regularity = regularity; }
  f32  CWindGenerator::getRegularity(){ return Regularity; }

  f32 rndGenerator(s32 x)         
  {
    x = (x<<13) ^ x;
    return ( (f32)(1.0 - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0));
  }

  f32 cosInterpolater(f32 a, f32 b, f32 x)
  {
    f32 ft = x * 3.1415927f;
    f32 f = (1 - cos(ft)) * 0.5f;

    return  a*(1-f) + b*f;
  }

  f32 windSmoother(s32 x)
  {
    return rndGenerator(x)/2  +  rndGenerator(x-1)/4  +  rndGenerator(x+1)/4;
  }

  f32 noiseInterpolate( f32 x )
  {
    s32 intX = (int)(x);
    f32 fracX = x - intX;

    f32 v1 = windSmoother(intX);
    f32 v2 = windSmoother(intX + 1);

    return cosInterpolater(v1 , v2 , fracX);
  }

  f32 noise( f32 x )
  {
    f32 total = 0;
    f32 p = 0.50;
    
    s32 n = 4;
    
    f32 frequency = 1;
    f32 amplitude = 1;
 
    for ( s32 i=0; i<n; i++ )
    {   
      total += noiseInterpolate(x * frequency) * amplitude;
   
      frequency = frequency + frequency;
      amplitude *= p;
    }
 
    return total;
  }

  core::vector2df CWindGenerator::getWind( const core::vector3df& position, u32 timeMs )
  {
    f32 seed = (timeMs + position.X*7*cos(timeMs/120000.0f) + position.Z*7*sin(timeMs/120000.0f))/ 1000.0f;
    f32 dir = 2*core::PI*noise( seed / Regularity );
    f32 amp = Strength*sin( seed );
 
    return core::vector2df( amp*cos(dir), amp*sin(dir) );
  } 

  IWindGenerator* createWindGenerator( f32 strength, f32 regularity )
  {
    CWindGenerator* wind = new CWindGenerator();
    wind->setStrength(strength);
    wind->setRegularity(regularity);
    return wind;
  } 

} // namespace scene
} // namespace irr

