
#ifndef CELL_H
#define CELL_H

#include <iostream>
#include <cmath>
#include <cstdlib>

class cell{
 private:
 unsigned int numCell_prev;
 float* weights;
 float biasWeight;
 float* inputVal;
 float* outputVal;
 bool isSigmoid;

 public:
	cell(unsigned int, float*, float*, float*, float, bool);
	void forwardPassCell();
	float sigmoid(float varX);
	float ReLU(float varX);

};

#endif
