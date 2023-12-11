
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#include "ML_TransformTest.h"

const mlTransformWithScale 		mlTransformWithScaleIdentity;

mlTransformWithScale::
mlTransformWithScale()
:	m_rotation(1.0f, 0.0f, 0.0f, 0.0f),
	m_translation(0.0f, 0.0f, 0.0f),
	m_matrix(	mlVector3D(1.0f, 0.0f, 0.0f),
				mlVector3D(0.0f, 1.0f, 0.0f),
				mlVector3D(0.0f, 0.0f, 1.0f),
				mlVector3D(0.0f, 0.0f, 0.0f)	),
	m_scale(1.0f, 1.0f, 1.0f),
	m_matrix_valid(true)
{
}

mlTransformWithScale::
mlTransformWithScale(const mlQuaternion &rotation, const mlVector3D &translation, const mlVector3D &scale)
:	m_rotation(rotation),
	m_translation(translation),
	m_scale(scale),
	m_matrix_valid(false)
{
}

void mlTransformWithScale::
TransformSelf(const mlTransformWithScale &t)
{
	m_rotation = t.m_rotation * m_rotation;

	m_translation = VectorScale(t.m_scale, (t.m_rotation.TransformVector(m_translation))) + t.m_translation;

	m_scale = VectorScale(m_scale, t.m_scale);

	m_matrix_valid = false;
}

void mlTransformWithScale::
Interpolate(mlFloat alpha, const mlTransformWithScale &from, const mlTransformWithScale &to)
{
	m_rotation = mlQuaternionInterpolate(alpha, from.m_rotation, to.m_rotation);

	m_translation = mlInterpolate(alpha, from.m_translation, to.m_translation);
	m_scale       = mlInterpolate(alpha, from.m_scale, to.m_scale);
	m_matrix_valid = false;
}

void mlTransformWithScale::
Invert()
{
	m_rotation.Invert();

	if (m_scale != mlVector3D(1.0f, 1.0f, 1.0f))
	{
		m_scale = InvertScaleVector(m_scale);
		m_translation = VectorScale(m_rotation.TransformVector(m_translation), -m_scale);
	}
	else
	{
		m_translation = -m_rotation.TransformVector(m_translation);
	}

	m_matrix_valid = false;
}


mlVector3D mlTransformWithScale::
TransformPoint(const mlVector3D &in) const
{
	// TEST: Is this equivalent to the array version?

	return m_rotation.TransformVector(VectorScale(in, m_scale)) + m_translation;
}

mlVector3D mlTransformWithScale::
TransformPointInverse(const mlVector3D &in) const
{
	// TEST: Is this equivalent to the array version?

	UpdateMatrixIfNecessary();

	mlVector3D scaleSquared = VectorScale(m_scale, m_scale);
	mlVector3D inverseScaleSquared = InvertScaleVector(scaleSquared);

	return VectorScale(m_matrix.TransformPointByTranspose(in), inverseScaleSquared);
}

mlVector3D mlTransformWithScale::
TransformVector(const mlVector3D &in) const
{
	// TEST: Is this equivalent to the array version?

	return m_rotation.TransformVector(VectorScale(in, m_scale));
}

mlVector3D mlTransformWithScale::
TransformVectorInverse(const mlVector3D &in) const
{
	// TEST: Is this equivalent to the array version?

	UpdateMatrixIfNecessary();

	mlVector3D scaleSquared = VectorScale(m_scale, m_scale);
	mlVector3D inverseScaleSquared = InvertScaleVector(scaleSquared);
	
	return VectorScale(m_matrix.TransformVectorByTranspose(in), inverseScaleSquared);
}

void mlTransformWithScale::
TransformVectors(mlVector3D outArray[], const mlVector3D inArray[], int vectorCount) const
{
	UpdateMatrixIfNecessary();
	m_matrix.TransformVectors(outArray, inArray, vectorCount);
}

void mlTransformWithScale::
TransformVectorsInverse(mlVector3D outArray[], const mlVector3D inArray[], int vectorCount) const
{
	UpdateMatrixIfNecessary();
	
	mlMatrix3x4 inverseMatrix;
	mlMatrixTranspose(inverseMatrix, m_matrix);
	
	if (m_scale != mlVector3D(1.0f, 1.0f, 1.0f))
	{	
		mlVector3D scaleSquared = VectorScale(m_scale, m_scale);
		mlVector3D inverseScaleSquared = InvertScaleVector(scaleSquared);

		inverseMatrix.I *= inverseScaleSquared.x;
		inverseMatrix.J *= inverseScaleSquared.y;
		inverseMatrix.K *= inverseScaleSquared.z;
	}
	
	inverseMatrix.TransformVectors(outArray, inArray, vectorCount);
}

void mlTransformWithScale::
TransformPoints(mlVector3D outArray[], const mlVector3D inArray[], int pointCount) const
{
	UpdateMatrixIfNecessary();
	m_matrix.TransformPoints(outArray, inArray, pointCount);
}

void mlTransformWithScale::
TransformPointsInverse(mlVector3D outArray[], const mlVector3D inArray[], int pointCount) const
{
	UpdateMatrixIfNecessary();
	
	mlMatrix3x4 inverseMatrix;
	mlMatrixTranspose(inverseMatrix, m_matrix);

	if (m_scale != mlVector3D(1.0f, 1.0f, 1.0f))
	{
		mlVector3D scaleSquared = VectorScale(m_scale, m_scale);
		mlVector3D inverseScaleSquared = InvertScaleVector(scaleSquared);

		inverseMatrix.I *= inverseScaleSquared.x;
		inverseMatrix.J *= inverseScaleSquared.y;
		inverseMatrix.K *= inverseScaleSquared.z;
		inverseMatrix.T = VectorScale(inverseMatrix.T, inverseScaleSquared);
	}
	
	inverseMatrix.TransformPoints(outArray, inArray, pointCount);
}

void mlTransformWithScale::UpdateMatrixAlways() const
{
	m_matrix.SetRotation(m_rotation);
	m_matrix.SetTranslation(m_translation);

	if (m_scale != mlVector3D(1.0f, 1.0f, 1.0f))
	{
		m_matrix.I *= m_scale.x;
		m_matrix.J *= m_scale.y;
		m_matrix.K *= m_scale.z;
	}

	m_matrix_valid = true;
}

mlVector3D mlTransformWithScale::GetTranslation() const
{
	return m_translation;
}

mlQuaternion mlTransformWithScale::GetRotation() const
{
	return m_rotation;
}

mlVector3D	mlTransformWithScale::GetScale() const
{
	return m_scale;
}

void mlTransformWithScale::SetTranslation(const mlVector3D & translation)
{
	m_translation = translation;
	m_matrix_valid = false;
}

void mlTransformWithScale::SetRotation(const mlQuaternion & rotation)
{
	m_rotation = rotation;
	m_matrix_valid = false;
}

void mlTransformWithScale::SetScale(const mlVector3D & scale)
{
	m_scale = scale;
	m_matrix_valid = false;
}


void mlTransformWithScale::ApplyTranslation(const mlVector3D & translation)
{
	m_translation += translation;
	m_matrix_valid = false;
}

void mlTransformWithScale::ApplyRotation(const mlQuaternion & rotation)
{
	m_rotation = rotation * m_rotation;
	m_matrix_valid = false;
}

void mlTransformWithScale::ApplyScale(const mlVector3D & scale)
{
	m_scale = VectorScale(m_scale, scale);
	m_matrix_valid = false;
}

void mlTransformWithScale::SetMatrix(const mlMatrix3x4 & matrix)
{	
	mlMatrix3x3 rotationMatrix(matrix.I, matrix.J, matrix.K);
	
	rotationMatrix.I.Normalise();
	rotationMatrix.J.Normalise();
	rotationMatrix.K.Normalise();
	 
	m_rotation = mlQuaternionFromRotationMatrix(rotationMatrix);
	m_translation = matrix.T;
	m_scale.x = matrix.I.Magnitude();
	m_scale.y = matrix.J.Magnitude();
	m_scale.z = matrix.K.Magnitude();
	m_matrix_valid = false;
}

void mlTransformWithScale::Normalise()
{
	m_rotation.Normalise();
}

void mlCombineTransform(mlTransformWithScale &AtoC, const mlTransformWithScale &AtoB, const mlTransformWithScale &BtoC)
{
	if (&BtoC==&AtoC) {
		mlTransformWithScale result(AtoB);
		result.TransformSelf(BtoC);
		AtoC = result;
	} else {

		if (&AtoB!=&AtoC)
			AtoC = AtoB;
			
		AtoC.TransformSelf(BtoC);
	}
}

void mlCombineTransformInverse(mlTransformWithScale &AtoC, const mlTransformWithScale &AtoB, const mlTransformWithScale &CtoB)
{
	mlTransformWithScale BtoC = CtoB;
	BtoC.Invert();
	
	mlCombineTransform(AtoC, AtoB, BtoC);
}

mlTransformWithScale	mlTransformWithScaleUtility::TransformFromReflection(const mlVector3D & pointOnPlane, const mlVector3D & planeNormalNormalised)
{
	mlTransformWithScale pointToOrigin;
	mlTransformWithScale originToPoint;
	originToPoint.SetTranslation(pointOnPlane);
	pointToOrigin.SetTranslation(-pointOnPlane);
	
	mlQuaternion rotation;
	rotation.x = planeNormalNormalised.x;
	rotation.y = planeNormalNormalised.y;
	rotation.z = planeNormalNormalised.z;
	rotation.w = 0.0f;
	
	mlTransformWithScale rotationScale(rotation, mlVector3DZero, mlVector3D(-1.0f, -1.0f, -1.0f));
	
	mlTransformWithScale result = pointToOrigin;
	result.TransformSelf(rotationScale);
	result.TransformSelf(originToPoint);
	
	return result;
}

void mlTransformWithScaleUtility::TransformFromPointForwardUp(mlTransformWithScale & trnTransform, const mlVector3D & vPosition, const mlVector3D & vForward, const mlVector3D & vUp)
{
	// Translation

	trnTransform.SetTranslation(vPosition);

	// Rotation

	mlVector3D vForwardNormalised = vForward;
	vForwardNormalised.Normalise();
	mlVector3D vUpNormalised = vUp;
	vUpNormalised.Normalise();

	mlQuaternion rotLookAt = mlQuaternionFromDirection(vForwardNormalised, vUpNormalised);
	trnTransform.SetRotation(rotLookAt);
}
