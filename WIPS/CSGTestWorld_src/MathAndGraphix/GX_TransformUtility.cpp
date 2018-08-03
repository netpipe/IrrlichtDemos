
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "GX_TransformUtility.h"

#include "GL/glut.h"

void gxApplyTransformToOpenGL(mlTransform & trn)
{
	mlTransform transform = trn;

	mlMatrix4x4 mat(transform.GetMatrix());

	mlFloat * pFloats = reinterpret_cast<mlFloat*>(&mat);

	glMultMatrixf(pFloats);
}

void gxMakeLookAtTransform(mlTransform * trn, const mlVector3D & vPos, const mlVector3D & vLookAt)
{
	trn->SetTranslation(vPos);

	mlVector3D vForward = vLookAt - vPos;

	vForward.Normalise();

	mlQuaternion rotLookAt = mlQuaternionFromDirection(vForward, mlVector3D(0.0f, 1.0f, 0.0f));

	trn->SetRotation(rotLookAt);
}
