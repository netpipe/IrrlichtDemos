
#include "../header/hiddenLayer.h"
#include <assert.h>

hiddenLayer::hiddenLayer(unsigned int numCell_prev, unsigned int numCell_cur, float** weight, float* biasWeight, float* inputVal, float* outputVal, bool isSigmoid)
{
 weights = weight;
 biasWeights = biasWeight;

 cellObj = new cell* [numCell_cur];
 for(int i = 0; i < numCell_cur; i++)
	cellObj[i] = new cell(numCell_prev, weights[i], inputVal, &(outputVal[i]), biasWeights[i], isSigmoid);

 hiddenLayer::numCell_prev = numCell_prev;
 hiddenLayer::numCell_cur = numCell_cur;
 hiddenLayer::inputVal = inputVal;
 hiddenLayer::outputVal = outputVal;
 hiddenLayer::isHiddenLayer = isSigmoid;
}

void hiddenLayer::forwardPassLayer()
{
	//forward pass for all cells
	for(int i = 0; i < numCell_cur; i++)
	 cellObj[i]-> forwardPassCell();
/*
	if(!isHiddenLayer) 			//if this layer is hidden Layer, then use softmax function
	{
		float cumulativeSum = 0.0;
		for(int i = 0; i < numCell_cur; i++)
		 cumulativeSum += exp(outputVal[i]);
		for(int i = 0; i < numCell_cur; i++)
		{
		 if(isinf(cumulativeSum))
			{
				for(int j = 0; j < numCell_cur; j++)
				 std::cout<<" out "<<outputVal[j]<<" expOut"<<exp(outputVal[j]);
			outputVal[i] = 0.0;
			}
		 else outputVal[i] = exp(outputVal[i]) / cumulativeSum;
		 if(isnan(outputVal[i]) == 1) std::cout<<" Softcsum "<<cumulativeSum;
		 assert(isnan(outputVal[i]) != 1);
		}		
	}
*/

	//using log scale
	float maxOut = outputVal[0];
	float cumulativeSum = 0.0;
	if(!isHiddenLayer)
	{
		for(int i = 1; i < numCell_cur; i++)
		{
			if(maxOut < outputVal[i]) maxOut = outputVal[i];
		}
		for(int i = 0; i < numCell_cur; i++)
			cumulativeSum += exp(outputVal[i] - maxOut);
		for(int i = 0; i < numCell_cur; i++)
		{
			outputVal[i] = exp(outputVal[i] - maxOut - log(cumulativeSum));
		 if(isnan(outputVal[i]) == 1) std::cout<<" Softcsum "<<cumulativeSum;
		}

	}

}

void hiddenLayer::backwardPassLayer(float* errGradient_prev, float* errGradient_cur)
{
 float p;
	for(int j = 0; j < numCell_prev; j++)
	{
		errGradient_prev[j] = 0.0;
		for(int i = 0; i < numCell_cur; i++)
		 errGradient_prev[j] += errGradient_cur[i] * weights[i][j];

		//sigmoid gradient s(1-s)
		errGradient_prev[j] *= inputVal[j] * (1 - inputVal[j]);
		//ReLU gradient
		// p = (inputVal[j] == 0.0 ? 0.0 : 1.0);
		//errGradient_prev[j] *= p;
	}
//use errGradient_cur to update weights
 float stepSize = 1.0;
	for(int i = 0; i < numCell_cur; i++)
	{
		for(int j = 0; j < numCell_prev; j++)
		{
		  if(isnan(errGradient_cur[i]))
			{
				for(int i = 0; i < numCell_cur; i++)
				std::cout<<" "<<errGradient_cur[i]<<std::endl;
				for(int i = 0; i < numCell_prev; i++)
				std::cout<<" "<<errGradient_prev[i]<<std::endl;

			}
		  assert(isnan(errGradient_cur[i]) != 1); 
		  weights[i][j] += stepSize * (errGradient_cur[i] * inputVal[j]);
		}
		biasWeights[i] += stepSize * errGradient_cur[i];
	}
}


hiddenLayer::~hiddenLayer()
{
 //delete biasWeights;
 //delete[] weights;
 delete[] cellObj;
}


