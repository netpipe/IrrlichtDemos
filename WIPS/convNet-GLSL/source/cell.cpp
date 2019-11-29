
#include "../header/cell.h"
#include <assert.h>
#include <time.h>

cell::cell(unsigned int numCell_prev, float* weights, float* inputVal, float* outputVal, float biasWeight, bool isSigmoid)
{
	cell::numCell_prev = numCell_prev;
	cell::weights = weights;
	cell::inputVal = inputVal;
	cell::outputVal = outputVal;
	cell::isSigmoid = isSigmoid;
	cell::biasWeight = biasWeight;	

/*
	//initialize weights
	srand(time(NULL));
	for(int i = 0; i < numCell_prev; i++)
		weights[i] = (float)(rand()%50) / 100.0;
	srand(time(NULL));
	cell::biasWeight = (float)(rand()%50) / 100.0;
*/

}

float cell::sigmoid(float varX)
{
	float res;
	res = 1/(1+exp(-varX));
	if(isnan(res)) std::cout<<" res "<<res;
	if(isnan(varX)) std::cout<<" X "<<varX;
	return res;
	

}

float cell::ReLU(float varX)
{
	float p =  (varX > 0.0) ? varX:0.0;
	return p;
}

void cell::forwardPassCell()
{
	float cumulativeSum = 0.0;
	for(int i = 0; i < numCell_prev; i++)
	 cumulativeSum += weights[i] * inputVal[i];
	//std::cout<<" cSum "<<cumulativeSum<<std::endl;
	cumulativeSum += biasWeight;
	if(isnan(cumulativeSum)) 
		std::cout<<" csum "<<cumulativeSum;
	assert(isnan(cumulativeSum) != 1);
	if(isSigmoid) *outputVal = sigmoid(cumulativeSum);
	else if(!isSigmoid) *outputVal = cumulativeSum;

}


