// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#ifndef NN_H
#define NN_H

#include "compile_config.h"

#if defined(NEURAL_AI)

#include <vector>
#include <assert.h>
#include <math.h>

// Notes:
// sigmoid activation function could have activation response param by dividing by arp like that: exp(-sum/arp)

// TODO: weights are called "input" and "hidden". Bias is called "hidden" and "output". That's wrong. Should be same name both times

class NeuralNet
{
public:
    NeuralNet();
    ~NeuralNet();

    void Run();

    bool Save(const char *filename_);
    bool Load(const char *filename_);

    void SetInputValue(int neuron_, float val_)     { assert(neuron_ < (int)mInputLayer.size()); mInputLayer[neuron_] = val_; }
    float GetOutputValue(int neuron_)               { return mOutputLayer[neuron_]; }

    void Initialize(int inputSize_, int hiddenSize_, int outputSize_, float absTreshholds_= 0.5f);

    void RandomizeWeights(float min_=-1.f, float max_=1.f);
	unsigned int GetNumWeights();
    void SetHiddenBias(float val_=-1.f);
    void SetOutputBias(float val_=0.f);
    void MutateInputWeights(float probability_=0.01f, float min_=-1.f, float max_=1.f, float mutateRange_= 0.1f, bool mutateBias_=true);
    void MutateHiddenWeights(float probability_=0.01f, float min_=-1.f, float max_=1.f, float mutateRange_= 0.1f, bool mutateBias_=true);

    // probability for each neuron of next layer to cross it's input weights completely or cross each individual weight
    void CrossInputWeights(float probability_, const NeuralNet &source_, bool crossIndividualWeights_, bool crossBias_=true);
    void CrossHiddenWeights(float probability_, const NeuralNet &source_, bool crossIndividualWeights_, bool crossBias_=true);

    void Print();

    const std::vector<float>& GetInputLayer() const { return mInputLayer; }

    float CalcOutputError(const std::vector<float> &trainingValues_);

// older stuff which might be useful for other network layouts
//    void MutateInputWeightsBiased(float min_=-1.f, float max_=1.f, float mutateRange_= 0.1f, float weightbias_ = 0.1f);
//    void MutateHiddenWeightsBiased(float min_=-1.f, float max_=1.f, float mutateRange_= 0.1f, float weightbias_ = 0.1f);
//    void SetBoostWinnerWeights(float val_)  { mBoostWinnerWeights = val_; }
//    void SetAllHiddenActivations(int val_);
//    void ModifyWeightsForWinners(float min_=-1.f, float max_=1.f, float mutateRange_= 0.1f, float weightbias_ = 0.1f);
//    void SetWeightBias(float val_);
//    void SetWeightBiasToWeights();
//    void SetHiddenWeightBiasForOutputNeuron(int neuron_, float bias_);
//    void RandomizeWeightBias(float min_=-1.f, float max_=1.f);
//    void ModifyWinnerWeightBiasToWeights(float modify_=0.25);    // modify_ between 0 and 1
//    void ModifyWeightBiasToWeights(float modify_=0.25);

protected:
    void MutateFloatVector(std::vector<float> &values_, float probability_=0.01f, float min_=-1.f, float max_=1.f, float mutateRange_= 0.1f);
	inline float Sigmoid(float val_)	{ return 1.f / (1.f + exp(-val_)); }
	inline float SigmoidP(float val_)	{ float r=Sigmoid(val_); return r * (1.f - r); }

    void CrossHiddenBias(float probability_, const NeuralNet &source_);
    void CrossOutputBias(float probability_, const NeuralNet &source_);
	// Should be no longer needed, TODO: check if it can be removed
    void MutateHiddenBias(float probability_=0.01f, float min_=-1.f, float max_=1.f, float mutateRange_= 0.1f);
    void MutateOutputBias(float probability_=0.01f, float min_=-1.f, float max_=1.f, float mutateRange_= 0.1f);

private:
    std::vector<float> mInputLayer;
    std::vector<float> mInputWeights;
    std::vector<float> mHiddenBias;
    std::vector<float> mHiddenLayer;
    std::vector<float> mHiddenWeights;
    std::vector<float> mOutputLayer;
    std::vector<float> mOutputBias;

// older stuff which might be useful for other network layouts
//    float mBoostWinnerWeights;
//    std::vector<int> mHiddenActivations;
//    std::vector<float> mHiddenTresholds;

//    std::vector<float> mInputWeightsBias;   // used for some test, basically a value towards weights are mutated which changes slower
//    std::vector<float> mHiddenWeightsBias;  // used for some test, basically a value towards weights are mutated which changes slower
};

#endif // NEURAL_AI

#endif // NN_H
