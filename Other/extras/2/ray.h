#ifndef RAY_H
#define RAY_H

#include "vector.h"

namespace Geometry
{
	struct Ray
	{
		Math::Vector3<float> point;
		Math::Vector3<float> vector;

		Math::Vector3<float> at ( const float &pos )
		{
			return Math::Vector3<float> ( point + ( vector * pos ) );
		}

		float distanceFromPoint ( const Math::Vector3<float> &rhs )
		{
			return sqrt ( Math::vSquared ( rhs - point ) - ( Math::vSquared ( ( rhs - point ) * vector ) / Math::vSquared ( vector ) ) );
		}
	};
};

#endif

