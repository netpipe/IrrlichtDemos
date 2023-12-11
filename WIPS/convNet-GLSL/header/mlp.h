
#ifndef MLP_H
#define MLP_H

#include <iostream>
#include "hiddenLayer.h"

class mlp{

private:
	unsigned int numInputNode, numOutputNode, numHiddenLayer;
	unsigned int* numCell;						//number of neurons or cells in each layer
	float** ioValues;						//collection of all input and output Values
	hiddenLayer** hiddenLayerObj;					//array of pointers to hidden Layers
	float** errGradient;
	float* labelPtr;
	float*** weights;
	float** biasWeights; 

public:
	mlp(int numIn, int numHid, int numOut, int nCells);
	float* getPtrToInput() {return ioValues[0];}
	float* getPtrToInputGradient() { return errGradient[0]; } 
	float* getPtrToLabel() {return labelPtr;}
	float learnFromMLP(float thresholdValue, int label, int imageID);
	int testMLP();
	~mlp();


};


#endif
