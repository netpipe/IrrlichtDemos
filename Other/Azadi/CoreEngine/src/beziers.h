#ifndef BEZIERS_H__
#define BEZIERS_H__

/* thinking about the interface:
   <bezier patch> := list of <bezier curve>
   <bezier curve> := array of coordinates

   bezier_patch generate_bezier(<path points>, <tangents>, <steps>) ?
   how'd we get the tangents?

   bezier_patch generate_bezier(<path points>, <steps>) + morph direct lines to 'evade' things?
   - that would be difficult, would have to move points along some line until either the patch works,
     or we have to find another path :S

   or, first find an actually working path, and use the directions from one path point to another as tangent?
   and if not possible, generate direct connection lines?

   // function:
   t + (1 - t) = 1    | raise to the power of 3, and use each part as point

   P1*t^3 + P2*3*t^2*(1-t) + P3*3*t*(1-t)^2 + P4*(1-t)^3 = Pnew
 */

// for dummies:

#include "vector.h"
#include <vector>
#include <cmath>

#define StepsByStepsize(stepsize, distance) (distance/stepsize)

extern void CreateBezier(Math::Vector3<float> from, Math::Vector3<float> to, // path points from, to
		  Math::Vector3<float> tFrom, Math::Vector3<float> tTo, // tangents at from and to
		  unsigned int steps, // detail, the # of steps
		  std::vector<Math::Vector3<float> > *out);

#endif
