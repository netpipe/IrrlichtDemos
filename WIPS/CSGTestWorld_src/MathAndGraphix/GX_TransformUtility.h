
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef GX_TRANSFORMUTILITY_H
#define GX_TRANSFORMUTILITY_H

#include "ML_Transform.h"

void gxApplyTransformToOpenGL(mlTransform & trn);
void gxMakeLookAtTransform(mlTransform * trn, const mlVector3D & vPos, const mlVector3D & vLookAt);

#endif // GX_TRANSFORMUTILITY_H
