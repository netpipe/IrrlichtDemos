// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef _RANDOM_H
#define _RANDOM_H

// ------------------------------------------------------------
// Get random numbers.
// A random generator will be initialized once at program start
// with the system time for all RandomGenerator objects.
class RandomGenerator
{
public:
    inline long int GetNumber() const;

    // including from_ and to_
    int GetNumberInRange( const int from_, const int to_ ) const;
    float GetNumberInRange( const float from_, const float to_ ) const;

    // returns numbers from 0 to range_-1
    // The full range of numbers is 0.5f*range_*(range_+1)
    // The chance for each number is the percentage of range_-number / full_range
    // That means 0 has highest chance and higher numbers have a lower chance
    int LinearSlope(const int range_) const;

    void Test();
};

#endif // _RANDOM_H
