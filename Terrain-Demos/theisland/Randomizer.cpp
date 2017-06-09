// our Randomizer is not really os specific, so we
// code one for all, which should work on every platform the same,
// which is desireable.

#include "Randomizer.h"

namespace irr
{
	namespace core
	{
		irr::s32 Randomizer::seed = 0x0f0f0f0f;

		//! generates a pseudo random number
		irr::s32 Randomizer::rand()
		{
			const irr::s32 m = 2147483399;	// a non-Mersenne prime
			const irr::s32 a = 40692;		// another spectral success story
			const irr::s32 q = m/a;
			const irr::s32 r = m%a;		// again less than q

			seed = a * (seed%q) - r* (seed/q);
			if (seed<0) seed += m;

			return seed;
		}

		//! resets the randomizer
		void Randomizer::reset()
		{
			seed = 0x0f0f0f0f;
		}
	}
}