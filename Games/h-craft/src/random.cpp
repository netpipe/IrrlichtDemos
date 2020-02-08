// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "random.h"

// ------------------------------------------------------------
class InitRandomGenerators
{
public:
    InitRandomGenerators();
};

InitRandomGenerators g_randomGeneratorSingleton;

// ------------------------------------------------------------
// initialize random generators with system time
InitRandomGenerators::InitRandomGenerators()
{
    time_t seconds;

    seconds = time( (time_t*) 0 );
    seconds += 1437372017;	// so it's not the usual number range
#ifdef __GNUWIN32__
    srand( (unsigned int)seconds );
#else
    srandom( (unsigned int)seconds );
#endif
}

// ------------------------------------------------------------
long int RandomGenerator::GetNumber() const
{
#ifdef __GNUWIN32__
    return rand();
#else
    return random();
#endif
}

// ------------------------------------------------------------
int RandomGenerator::GetNumberInRange( const int from_, const int to_ ) const
{
    return (GetNumber() % (to_+1-from_)) + from_;
}

float RandomGenerator::GetNumberInRange( const float from_, const float to_ ) const
{
    float range = to_-from_;
    return ((float)GetNumber() / RAND_MAX) * range + from_;
}

// The full range of numbers is 0.5f*range_*(range_+1)
// The chance for each number is the percentage of range_-number / fullrange
int RandomGenerator::LinearSlope(const int range_) const
{
    int fullRange = static_cast<int>(0.5f*range_*(range_+1.f));
    int value = GetNumberInRange( 0, fullRange );

    int result = 0;
    while ( value > range_-result )
    {
        value -= range_-result;
        ++result;
    }

    return result;
}

void RandomGenerator::Test()
{
    int res1[20];
    for ( int i=0; i < 20; ++i )
    {
        res1[i] = 0;
    }
    for ( int i=0; i < 10000; ++i )
    {
        int val = 0;
        val = LinearSlope(17);
        if ( val < 0 )
            res1[18]++;
        else if ( val > 17 )
            res1[19]++;
        else
            res1[val] ++;
    }
    for ( int i=0; i < 20; ++i )
    {
        fprintf(stderr, "%d: %d \n", i, res1[i]);
    }

}
