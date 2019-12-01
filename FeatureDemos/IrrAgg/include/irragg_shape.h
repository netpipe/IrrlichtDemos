// (C) 2019 Nicolaus Anderson
// See license.txt for details.

#ifndef IRRAGG_SHAPE_H
#define IRRAGG_SHAPE_H

#include "irragg_types.h"

namespace irr {
namespace vecg {

struct IShape
{
	virtual ~IShape() {}

	//! Return how the polygon must be drawn (indices start at zero)
	virtual PathMotion  getPathMotion(size_t) const = 0;

	//! Return the number of points in the polygon
	virtual size_t  getPathStepCount() const = 0;

	//operator IShape& () {
	//	return (IShape&)*this;
	//}
};

} // end namespace vecg
} // end namespace irr

#endif
