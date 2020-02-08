// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include "helper_math.h"
#include <cmath>
//#include <cassert>

namespace ExtMath
{

double acos_save(double val_)
{
//    assert( val_ < 1.1 && val_ > -1.1 );

    if ( val_ > 1.f )
        val_ = 1.f;
    if ( val_ < -1.f )
        val_ = -1.f;
    return acos(val_);
}

double asin_save(double val_)
{
	if ( val_ > 1.f )
        val_ = 1.f;
    if ( val_ < -1.f )
        val_ = -1.f;
    return asin(val_);
}

// round float to a certain precision
float RoundPrec(float val, int precision)
{
	int m = pow((double)10, (double)precision);
	val *= m;
	val = floorf( val + 0.5f );
	val /= m;
	return val;
}

}; // namespace ExtMath
