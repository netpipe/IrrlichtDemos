
#ifndef INFINITE_RUNNING_AVERAGE_H
#define INFINITE_RUNNING_AVERAGE_H

class InfiniteRunningAverage
{
	double average;
	double valueCount;
	
public:
	InfiniteRunningAverage()
	{
		Reset();
	}
	
	void Reset()
	{
		average = 0.0;
		valueCount = 1;
	}
	
	void Add(double value)
	{
		average = average * ((valueCount-1.0)/valueCount) + value * (1.0/valueCount);
		valueCount += 1.0;
	}
	
	double Get()
	{
		return average;
	}
	
	bool Valid()
	{
		// At least one value has been added.
		return valueCount > 1.1;
	}
};

#endif
