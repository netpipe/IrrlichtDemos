// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef HELPER_MATH_H
#define HELPER_MATH_H

namespace ExtMath
{
	// Because of rounding errors the value is sometimes slightly above allowed range (like 1.0000001)
	// acos_save does clip the values first and throws an assert if val_ is too far off
	extern double acos_save(double val_);
	extern double asin_save(double val_);

	// round float to a certain precision
	extern float RoundPrec(float val, int precision);

};

#endif // HELPER_MATH_H
