
#ifndef HIDDENLAYER_H
#define HIDDENLAYER_H

#include "cell.h"

class hiddenLayer{
 private:
	cell** cellObj;
	float** weights;
	float* biasWeights;
	unsigned int numCell_prev, numCell_cur;
	float *inputVal, *outputVal;
	bool isHiddenLayer;

 public:
	hiddenLayer(unsigned int numCell_prev, unsigned int numCell_cur, float** weight, float* biasWeight, float* inputVal, float* outputVal, bool isSigmoid = true);
	~hiddenLayer();
	void forwardPassLayer();
	void backwardPassLayer(float*, float*);

};


#endif
