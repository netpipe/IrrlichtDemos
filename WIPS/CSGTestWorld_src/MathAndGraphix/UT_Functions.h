
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef UT_FUNCTIONS_H
#define UT_FUNCTIONS_H

#include "ML_Maths.h"
#include "ML_Vector.h"

mlFloat utSigmoid(mlFloat x, mlFloat k, mlFloat bias);
mlFloat utSigmoidDerivative(mlFloat x, mlFloat k, mlFloat bias);

mlFloat utRand(mlFloat min, mlFloat max, int granularity);

mlFloat utLatticeMagnitude(const mlVector2D & line);
mlFloat utLatticeMagnitude(const mlVector3D & line);

mlFloat utAddAndClamp(mlFloat fValue, mlFloat fToAdd, mlFloat fMin, mlFloat fMax);
mlFloat utSubtractAndClamp(mlFloat fValue, mlFloat fToSubtract, mlFloat fMin, mlFloat fMax);
mlFloat utApproachZero(mlFloat fValue, mlFloat fToDiminish);

#endif // UT_FUNCTIONS_H
