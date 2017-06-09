//Irrlicht source
//taken from os::Randomizer
#ifndef _Randomizer_
#define _Randomizer_

#include "include.h"

namespace irr
{
	namespace core
	{
		class Randomizer
		{
		public:

			//! resets the randomizer
			static void reset();

			//! generates a pseudo random number
			static irr::s32 rand();

		private:

			static irr::s32 seed;
		};
	}
}

#endif