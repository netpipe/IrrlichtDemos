
// Author: Greg Santucci, 2008 and 2009
// Email: thecodewitch@gmail.com
// Web: http://createuniverses.blogspot.com/

#ifndef ML_TRANSFORMTEST_H
#define ML_TRANSFORMTEST_H

#include 	"ML_Maths.h"
#include	"ML_Vector.h"
#include	"ML_Quaternion.h"
#include 	"ML_Matrix.h"

class mlTransformWithScale 
{
public:
	mlTransformWithScale();
	
	mlTransformWithScale(const mlQuaternion & rotation, const mlVector3D & translation, const mlVector3D & scale = mlVector3D(1.0f, 1.0f, 1.0f));
	
	const mlTransformWithScale& operator=(const mlTransformWithScale& cloneFrom);
	mlTransformWithScale(const mlTransformWithScale& cloneFrom);
	
	const mlTransformWithScale& operator=(const mlMatrix3x4 & cloneFrom);
	explicit mlTransformWithScale(const mlMatrix3x4& cloneFrom);
	
	bool			operator==(const mlTransformWithScale & other) const;
	bool			IsIdentity() const;
	
	void			TransformSelf(const mlTransformWithScale &transform);
	
	void			Normalise();
	
	void			SetTranslation(const mlVector3D & translation);
	void			SetRotation(const mlQuaternion & rotation);
	void			SetScale(const mlVector3D & scale);
	void			SetMatrix(const mlMatrix3x4 & matrix);

	void			ApplyTranslation(const mlVector3D & translation);
	void			ApplyRotation(const mlQuaternion & rotation);
	void			ApplyScale(const mlVector3D & scale);
	
	void			Invert();
	
	void			Interpolate(mlFloat alpha, const mlTransformWithScale &from, const mlTransformWithScale &to);
	
	mlVector3D		TransformPoint(const mlVector3D &in) const;
	mlVector3D		TransformPointInverse(const mlVector3D &in) const;
	mlVector3D		TransformVector(const mlVector3D &in) const;
	mlVector3D		TransformVectorInverse(const mlVector3D &in) const;
	
	void			TransformPoints			(mlVector3D outArray[], const mlVector3D inArray[], int pointCount) const;
	void			TransformPointsInverse	(mlVector3D outArray[], const mlVector3D inArray[], int pointCount) const;
	void			TransformVectors		(mlVector3D outArray[], const mlVector3D inArray[], int vectorCount) const;
	void			TransformVectorsInverse	(mlVector3D outArray[], const mlVector3D inArray[], int vectorCount) const;
	
	mlVector3D		GetScale() const;
	mlQuaternion	GetRotation() const;
	mlVector3D		GetTranslation() const;
	
	const mlMatrix3x4 &	GetMatrix() const;
	
private:
	mlQuaternion				m_rotation;
	mlVector3D					m_translation;
	
	mutable mlMatrix3x4			m_matrix;
	mlVector3D					m_scale;
	
	mutable bool				m_matrix_valid;
	
	void						UpdateMatrixAlways() const;
	void						UpdateMatrixIfNecessary() const;
};

extern const mlTransformWithScale 		mlTransformWithScaleIdentity;

void		mlCombineTransform		 (mlTransformWithScale &AtoC, const mlTransformWithScale &AtoB, const mlTransformWithScale &BtoC);
void		mlCombineTransformInverse(mlTransformWithScale &AtoC, const mlTransformWithScale &AtoB, const mlTransformWithScale &CtoB);

class mlTransformWithScaleUtility
{
public:
	static mlTransformWithScale			TransformFromReflection(const mlVector3D & pointOnPlane, const mlVector3D & planeNormalNormalised);
	static void					TransformFromPointForwardUp(mlTransformWithScale & trnTransform, const mlVector3D & vPosition, const mlVector3D & vForward, const mlVector3D & vUp);
};

inline const mlTransformWithScale & mlTransformWithScale::operator=(const mlTransformWithScale & cloneFrom)
{
	m_matrix_valid = false;
	m_rotation = cloneFrom.m_rotation;
	m_translation = cloneFrom.m_translation;
	m_scale = cloneFrom.m_scale;

	return *this;
}

inline mlTransformWithScale::mlTransformWithScale(const mlTransformWithScale& cloneFrom)
{
	*this = cloneFrom;
}

inline const mlTransformWithScale & mlTransformWithScale::operator=(const mlMatrix3x4 & cloneFrom)
{
	SetMatrix(cloneFrom);
	return *this;
}

inline mlTransformWithScale::mlTransformWithScale(const mlMatrix3x4& cloneFrom)
{
	*this = cloneFrom;
}

inline void	mlTransformWithScale::UpdateMatrixIfNecessary() const
{
	if (!m_matrix_valid)
		UpdateMatrixAlways();
}

inline const mlMatrix3x4 & mlTransformWithScale::GetMatrix() const
{
	UpdateMatrixIfNecessary();
		
	return m_matrix;
}

inline bool mlTransformWithScale::operator==(const mlTransformWithScale & other) const
{
	return	(m_translation == other.m_translation) &&
			(m_scale == other.m_scale) &&
			(m_rotation == other.m_rotation);
}

inline bool	mlTransformWithScale::IsIdentity() const
{
	return	(m_translation.x == 0.0f && m_translation.y == 0.0f && m_translation.z == 0.0f) &&
			(m_scale == mlVector3D(1.0f, 1.0f, 1.0f)) &&
			(m_rotation.w == 1.0f && m_rotation.x == 0.0f && m_rotation.y == 0.0f && m_rotation.z == 0.0f);
}

inline mlVector3D VectorScale(const mlVector3D & a, const mlVector3D & b)
{
	mlVector3D c = mlVector3D(a.x * b.x, a.y * b.y, a.z * b.z);
	return c;
}

inline mlVector3D InvertScaleVector(const mlVector3D & a)
{
	mlVector3D b = mlVector3D(1.0f / a.x, 1.0f / a.y, 1.0f / a.z);
	return b;
}

#endif // ML_TRANSFORMTEST_H
