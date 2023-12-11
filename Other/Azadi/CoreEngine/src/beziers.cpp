#include "beziers.h"
#include "GUI/Console.h"

// FIXME:: I use the order: FROM, TangentFrom, TangentTo, TO
// might be 'wrong', there's no wrong though, but it might not be the best one?
// 1, and 4 are the 'strongest' ones, 2 and 3 are.. not lol

// 3rd degree basis function
// t^3 + 3*t^2*(1-t) + 3*t*(1-t)^2 + (1-t)^3 = 1
// P1*t^3 + P2*3*t^2*(1-t) + P3*3*t*(1-t)^2 + P4*(1-t)^3 = Pp
inline Math::Vector3<float> Basis3v(float t, Math::Vector3<float> *p)
{
	return ( ( p[0] * pow(t,3) ) +
		 ( p[1] * (3 * pow(t, 2) * (1 - t)) ) +
		 ( p[2] * (3 * t * pow(1-t, 2)) ) +
		 ( p[3] * pow(1-t, 3) )
		);
}
inline Math::Vector3<float> Basis3(float t, Math::Vector3<float> p1, Math::Vector3<float> p2, Math::Vector3<float> p3, Math::Vector3<float> p4)
{
	return ( ( p1 * pow(t,3) ) +
		 ( p2 * (3 * pow(t, 2) * (1 - t)) ) +
		 ( p3 * (3 * t * pow(1-t, 2)) ) +
		 ( p4 * pow(1-t, 3) )
		);
}

void CreateBezier(Math::Vector3<float> from, Math::Vector3<float> to, // path points from, to
		  Math::Vector3<float> tFrom, Math::Vector3<float> tTo, // tangents at from and to
		  unsigned int steps, // detail, the # of steps
		  std::vector<Math::Vector3<float> > *out)
{
#ifdef DEBUG
	assert(out); // FIXME::Well, passing 0 here is stupid, but asserting sucks, so what? --blub
#endif

	const register float stepsize = 1.0f/static_cast<float>(steps);

	for(unsigned int i = 0; i < steps; ++i)
		out->push_back(Basis3(i*stepsize, from, tFrom, tTo, to));
}
