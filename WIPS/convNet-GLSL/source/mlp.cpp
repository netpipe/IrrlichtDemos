
#include "../header/mlp.h"
#include <assert.h>
#include <fstream>

mlp::mlp(int numIn, int numHid, int numOut, int nCells)			//default constructor for a binomial classifier
{
	numInputNode = numIn;
	numOutputNode = numOut;
	numHiddenLayer = numHid;	

	numCell = new unsigned int[numHiddenLayer + 2];
	numCell[0] = numInputNode;
	numCell[numHiddenLayer +1] = numOutputNode;
	for(int i = 1; i < numHiddenLayer +1; i++)
	  numCell[i] = nCells;

	ioValues = new float* [numHiddenLayer +2];
	for(int i = 0; i < numHiddenLayer+2; i++)
	  ioValues[i] = new float[numCell[i]];

	errGradient = new float* [numHiddenLayer +2];
	for(int i = 0; i < numHiddenLayer +2; i++)
	  errGradient[i] = new float[numCell[i]];

	hiddenLayerObj = new hiddenLayer* [numHiddenLayer+1];

	weights = new float** [numHiddenLayer +1];
 	biasWeights = new float* [numHiddenLayer +1];

 	int numCell_cur, numCell_prev;
	std::ifstream wtFile("weights.txt");
	std::ifstream biasFile("bias.txt");

	for(int i = 0; i < numHiddenLayer; i++)			//all HiddenLayers and +1 for output layer as well
	{
		srand(time(NULL));
		numCell_cur = numCell[i+1];
		numCell_prev = numCell[i];

 		weights[i] = new float* [numCell_cur];
		biasWeights[i] = new float[numCell_cur];

 		for(int j = 0; j < numCell_cur; j++)
		{
 		 weights[i][j] = new float[numCell_prev];
			for(int k = 0; k < numCell_prev; k++)
			 wtFile>>weights[i][j][k];
		 biasFile>>biasWeights[i][j];
		}

	  hiddenLayerObj[i] = new hiddenLayer(numCell_prev, numCell_cur, weights[i], biasWeights[i], ioValues[i], ioValues[i+1]);
	}

		//for output Layer as hidden Layer
		numCell_cur = numCell[numHiddenLayer+1];
		numCell_prev = numCell[numHiddenLayer];

 		weights[numHiddenLayer] = new float* [numCell_cur];
		biasWeights[numHiddenLayer] = new float[numCell_cur];

 		for(int j = 0; j < numCell_cur; j++)
		{
 		 weights[numHiddenLayer][j] = new float[numCell_prev];
			for(int k = 0; k < numCell_prev; k++)
			 wtFile>>weights[numHiddenLayer][j][k];
		 biasFile>>biasWeights[numHiddenLayer][j];
		}

  	hiddenLayerObj[numHiddenLayer] = new hiddenLayer(numCell[numHiddenLayer], numCell[numHiddenLayer+1], weights[numHiddenLayer], biasWeights[numHiddenLayer], ioValues[numHiddenLayer], ioValues[numHiddenLayer+1], false);
	
	labelPtr = new float[numOutputNode];	
	for(int i = 0; i < numOutputNode; i++)	labelPtr[i] = 0.0;
}


float mlp::learnFromMLP(float thresholdValue, int label, int imageID)
{
  unsigned int iterationCount = 0;
  float evalAccuracy = 0.0;			//assume zero accuracy initially
	
	for(int i = 0; i < numHiddenLayer+1; i++){
		hiddenLayerObj[i]->forwardPassLayer(); }

	evalAccuracy = 1.0;
	for(int i = 0; i < numOutputNode; i++)
	{
		errGradient[numHiddenLayer+1][i] = (labelPtr[i] - ioValues[numHiddenLayer+1][i]);
		if(isnan(errGradient[numHiddenLayer+1][i]) == 1) 
		{
				std::cout<<" label "<<label;
			std::cout<<std::endl<<" label "<<labelPtr[i]<<" io "<<ioValues[numHiddenLayer+1][i];
		}
		assert(isnan(errGradient[numHiddenLayer+1][i]) != 1);
		evalAccuracy *= pow(ioValues[numHiddenLayer+1][i], labelPtr[i]);
	}

	
	//std::cout<<"label "<<label<<" eval "<<evalAccuracy<<" diff "<<(labelPtr[label] - ioValues[numHiddenLayer+1][label])<<std::endl;
	//evalAccuracy is the maximum value of probability


	for(int i = numHiddenLayer; i >= 0; i--)
		hiddenLayerObj[i]->backwardPassLayer(errGradient[i], errGradient[i+1]);
//	 std::cout<<std::endl<<" ID "<<imageID<<" eval Accuracy "<<evalAccuracy<<std::endl;

/*
	if(isnan(evalAccuracy))
	{
		std::cout<<std::endl<<" Label "<<label<<std::endl;
	std::cout<<" after forward pass "<<std::endl;
	for(int i = 0; i < numHiddenLayer+2; i++)
	{
		for(int j = 0; j < numCell[i]; j++)
		{
			std::cout<<" for cell i = "<<i<<" j = "<<j<<"  val is "<<ioValues[i][j]<<std::endl;		

		}

	}



	std::cout<<" after backward pass "<<std::endl;
	for(int i = 0; i < numHiddenLayer + 2; i++)
	{
		for(int j = 0; j < numCell[i]; j++)
		{
			std::cout<<" for cell i = "<<i<<" j = "<<j<<"  val is "<<errGradient[i][j]<<std::endl;		

		}

	}
	}
*/

	return (labelPtr[label] - ioValues[numHiddenLayer+1][label]);
}

int mlp::testMLP()
{
	std::cout<<std::endl<<" testing procedure "<<std::endl;
	for(int i = 0; i < numHiddenLayer+1; i++)
	hiddenLayerObj[i]->forwardPassLayer();

	//display output
	std::cout<<" after forward pass "<<std::endl;
	float maxVal = ioValues[numHiddenLayer+1][0];
	int label = 0;

	for(int j = 1; j < numCell[numHiddenLayer]; j++)
	 {
		if(maxVal < ioValues[numHiddenLayer+1][j])
		{
			maxVal = ioValues[numHiddenLayer+1][j];
			label = j;
		}
	 }
 return label;

}

mlp::~mlp()
{

	//save weights to file
	std::ofstream wtFile("weights.txt");
	std::ofstream biasFile("bias.txt");
	for(int i = 0; i < numHiddenLayer+1; i++)
	{
		for(int j = 0; j < numCell[i+1]; j++)
		{
			for(int k = 0; k < numCell[i]; k++)
				wtFile<<weights[i][j][k]<<' ';
			biasFile<<biasWeights[i][j]<<' ';
		}
	}

	for(int i = 0; i < numHiddenLayer+1; i++)
		delete[] weights[i];
	delete[] biasWeights;
	delete labelPtr;
	delete numCell;
	delete[] ioValues;
	delete[] errGradient;
	delete[] hiddenLayerObj;

}
