// Written by Michael Zeilfelder, please check licenseHCraft.txt for the zlib-style license text.

#if defined(NEURAL_AI)

#include "nn.h"
#include "random.h"
#include "streaming.h"

NeuralNet::NeuralNet()
{
//    mBoostWinnerWeights = 0.f;
}

NeuralNet::~NeuralNet()
{
}

void NeuralNet::Print()
{
//    fprintf(stderr, "mBoostWinnerWeights: %f\n", mBoostWinnerWeights);
    fprintf(stderr, "mInputLayer: ");
    for ( unsigned int i=0; i<mInputLayer.size(); ++i )
        fprintf(stderr, "%f ", mInputLayer[i]);
    fprintf(stderr, "\nmInputWeights: ");
    for ( unsigned int i=0; i<mInputWeights.size(); ++i )
        fprintf(stderr, "%f ", mInputWeights[i]);
//    fprintf(stderr, "\nmInputWeightsBias: ");
//    for ( unsigned int i=0; i<mInputWeightsBias.size(); ++i )
//        fprintf(stderr, "%f ", mInputWeightsBias[i]);
    fprintf(stderr, "\nmHiddenLayer: ");
    for ( unsigned int i=0; i<mHiddenLayer.size(); ++i )
        fprintf(stderr, "%f ", mHiddenLayer[i]);
    fprintf(stderr, "\nmHiddenBias: ");
    for ( unsigned int i=0; i<mHiddenBias.size(); ++i )
        fprintf(stderr, "%f ", mHiddenBias[i]);
    fprintf(stderr, "\nmHiddenWeights: ");
    for ( unsigned int i=0; i<mHiddenWeights.size(); ++i )
        fprintf(stderr, "%f ", mHiddenWeights[i]);
//    fprintf(stderr, "\nmHiddenWeightsBias: ");
//    for ( unsigned int i=0; i<mHiddenWeightsBias.size(); ++i )
//        fprintf(stderr, "%f ", mHiddenWeightsBias[i]);
//    fprintf(stderr, "\nmHiddenTresholds: ");
//    for ( unsigned int i=0; i<mHiddenTresholds.size(); ++i )
//        fprintf(stderr, "%f ", mHiddenTresholds[i]);
//    fprintf(stderr, "\nmHiddenActivations: ");
//    for ( unsigned int i=0; i<mHiddenActivations.size(); ++i )
//        fprintf(stderr, "%d ", mHiddenActivations[i]);
    fprintf(stderr, "\nmOutputLayer: ");
    for ( unsigned int i=0; i<mOutputLayer.size(); ++i )
        fprintf(stderr, "%f ", mOutputLayer[i]);
    fprintf(stderr, "\n");
}

bool NeuralNet::Save(const char *filename_)
{
    if ( !filename_ || !strlen(filename_) )
        return false;

    FileArchive ar;
    if ( !ar.Open(filename_, "wb") )
        return false;

    ar.Write(mInputLayer.size());
    for ( unsigned int i=0; i < mInputLayer.size(); ++i )
        ar.Write(mInputLayer[i]);

    ar.Write(mInputWeights.size());
    for ( unsigned int i=0; i < mInputWeights.size(); ++i )
        ar.Write(mInputWeights[i]);

    ar.Write(mHiddenBias.size());
    for ( unsigned int i=0; i < mHiddenBias.size(); ++i )
        ar.Write(mHiddenBias[i]);

    ar.Write(mHiddenLayer.size());
    for ( unsigned int i=0; i < mHiddenLayer.size(); ++i )
        ar.Write(mHiddenLayer[i]);

    ar.Write(mHiddenWeights.size());
    for ( unsigned int i=0; i < mHiddenWeights.size(); ++i )
        ar.Write(mHiddenWeights[i]);

    ar.Write(mOutputLayer.size());
    for ( unsigned int i=0; i < mOutputLayer.size(); ++i )
        ar.Write(mOutputLayer[i]);

    ar.Write(mOutputBias.size());
    for ( unsigned int i=0; i < mOutputBias.size(); ++i )
        ar.Write(mOutputBias[i]);

    return true;
}

bool NeuralNet::Load(const char *filename_)
{
    if ( !filename_ || !strlen(filename_) )
        return false;

    FileArchive ar;
    if ( !ar.Open(filename_, "rb") )
        return false;

    unsigned int size=0;

    ar.Read(size);
    mInputLayer.resize(size, 0.f);
    for ( unsigned int i=0; i<size; ++i )
        ar.Read(mInputLayer[i]);

    ar.Read(size);
    mInputWeights.resize(size, 0.f);
    for ( unsigned int i=0; i<size; ++i )
        ar.Read(mInputWeights[i]);

    ar.Read(size);
    mHiddenBias.resize(size, 0.f);
    for ( unsigned int i=0; i<size; ++i )
        ar.Read(mHiddenBias[i]);

    ar.Read(size);
    mHiddenLayer.resize(size, 0.f);
    for ( unsigned int i=0; i<size; ++i )
        ar.Read(mHiddenLayer[i]);

    ar.Read(size);
    mHiddenWeights.resize(size, 0.f);
    for ( unsigned int i=0; i<size; ++i )
        ar.Read(mHiddenWeights[i]);

    ar.Read(size);
	mOutputLayer.resize(size, 0.f);
    for ( unsigned int i=0; i<size; ++i )
        ar.Read(mOutputLayer[i]);

    ar.Read(size);
    mOutputBias.resize(size, 0.f);
    for ( unsigned int i=0; i<size; ++i )
        ar.Read(mOutputBias[i]);

    return true;
}


void NeuralNet::Run()
{
    for ( unsigned int h=0; h < mHiddenLayer.size(); ++h )
    {
        float val = 0.f;
        for ( unsigned int i=0; i < mInputLayer.size(); ++i)
        {
            val += mInputLayer[i] * mInputWeights[h*mInputLayer.size()+i];
        }
        val += mHiddenBias[h]*-1.f;
	    mHiddenLayer[h] = Sigmoid(val);
    }

    for ( unsigned int p=0; p < mOutputLayer.size(); ++p )
    {
        float val = 0.f;
        for ( unsigned int h=0; h < mHiddenLayer.size(); ++h )
        {
            val += mHiddenLayer[h] * mHiddenWeights[p*mHiddenLayer.size()+h];
        }
        val += mOutputBias[p]*-1.f;
	    mOutputLayer[p] = Sigmoid(val);
    }


// winner takes it all
//    int winner = 0;
//    for ( unsigned int h=1; h < mHiddenLayer.size(); ++h )
//    {
//        if ( mHiddenLayer[h] > mHiddenLayer[winner] )
//        {
//            winner = h;
//        }
//    }
//        ++mHiddenActivations[winner];
//    for ( unsigned int p=0; p < mOutputLayer.size(); ++p )
//    {
//        if ( mHiddenLayer[winner] >= mHiddenTresholds[winner] )
//            mOutputLayer[p] = mHiddenWeights[p*mHiddenTresholds.size()+winner];
//    }
//    if ( mBoostWinnerWeights != 0.f )
//    {
//        int winners = 0;
//        for ( unsigned int i=0; i < mInputLayer.size(); ++i)
//        {
//            if ( mInputWeights[winner*mInputLayer.size()+i] > mHiddenTresholds[winner] )
//            {
//                ++winners;
//            }
//        }
//        int losers = mInputLayer.size()-winners;
//        for ( unsigned int i=0; i < mInputLayer.size(); ++i)
//        {
//            if ( mInputWeights[winner*mInputLayer.size()+i] > mHiddenTresholds[winner] )
//            {
//                mInputWeights[winner*mInputLayer.size()+i] += mBoostWinnerWeights / winners;
//            }
//            else
//            {
//                mInputWeights[winner*mInputLayer.size()+i] -= mBoostWinnerWeights / losers;
//            }
//        }
//    }
}

void NeuralNet::Initialize(int inputSize_, int hiddenSize_, int outputSize_, float absTreshholds_)
{
    mInputLayer.resize(inputSize_, 0.f);
    mHiddenLayer.resize(hiddenSize_, 0.f);
    mHiddenBias.resize(hiddenSize_, 0.f);
//    mHiddenTresholds.resize(hiddenSize_, absTreshholds_);
//    mHiddenActivations.resize(hiddenSize_, 0);
    mOutputLayer.resize(outputSize_, 0.f);
    mOutputBias.resize(outputSize_, 0.f);
    mInputWeights.resize(inputSize_*hiddenSize_, 0.f);
//    mInputWeightsBias.resize(inputSize_*hiddenSize_, 0.f);
    mHiddenWeights.resize(hiddenSize_*outputSize_, 0.f);
//    mHiddenWeightsBias.resize(hiddenSize_*outputSize_, 0.f);
}

unsigned int NeuralNet::GetNumWeights()
{
	return mInputWeights.size() + mHiddenWeights.size();
}

void NeuralNet::RandomizeWeights(float min_, float max_)
{
    RandomGenerator randGen;

    for ( unsigned int i=0; i < mInputWeights.size(); ++i )
        mInputWeights[i] = randGen.GetNumberInRange( min_, max_ );
    for ( unsigned int i=0; i < mHiddenWeights.size(); ++i )
        mHiddenWeights[i] = randGen.GetNumberInRange( min_, max_ );
}

void NeuralNet::SetHiddenBias(float val_)
{
    for ( unsigned int i=0; i < mHiddenBias.size(); ++i )
        mHiddenBias[i] = val_;
}

void NeuralNet::SetOutputBias(float val_)
{
    for ( unsigned int i=0; i < mOutputBias.size(); ++i )
        mOutputBias[i] = val_;
}

void NeuralNet::MutateFloatVector(std::vector<float> &values_, float probability_, float min_, float max_, float mutateRange_)
{
    RandomGenerator randGen;

    for ( unsigned int i=0; i < values_.size(); ++i )
    {
        if ( randGen.GetNumberInRange(0.f, 1.f) < probability_ )
        {
            float min = values_[i]-0.5f*mutateRange_;
            float max = values_[i]+0.5f*mutateRange_;
            if ( min < min_ )
                min = min_;
            if ( max > max_ )
                max = max_;
            values_[i] = randGen.GetNumberInRange( min, max );
        }
    }
}

void NeuralNet::MutateHiddenBias(float probability_, float min_, float max_, float mutateRange_)
{
    MutateFloatVector(mHiddenBias, probability_, min_, max_, mutateRange_);
}

void NeuralNet::MutateOutputBias(float probability_, float min_, float max_, float mutateRange_)
{
    MutateFloatVector(mOutputBias, probability_, min_, max_, mutateRange_);
}

void NeuralNet::MutateInputWeights(float probability_, float min_, float max_, float mutateRange_, bool mutateBias_)
{
    MutateFloatVector(mInputWeights, probability_, min_, max_, mutateRange_);
    if ( mutateBias_ )
        MutateFloatVector(mHiddenBias, probability_, min_, max_, mutateRange_);
}

void NeuralNet::MutateHiddenWeights(float probability_, float min_, float max_, float mutateRange_, bool mutateBias_)
{
    MutateFloatVector(mHiddenWeights, probability_, min_, max_, mutateRange_);
    if ( mutateBias_ )
        MutateFloatVector(mOutputBias, probability_, min_, max_, mutateRange_);
}

void NeuralNet::CrossInputWeights(float probability_, const NeuralNet &source_, bool crossIndividualWeights_, bool crossBias_)
{
    RandomGenerator randGen;
    if ( crossIndividualWeights_ )
    {
        for ( unsigned int i=0; i < mInputWeights.size(); ++i)
        {
            if ( randGen.GetNumberInRange(0.f, 1.f) < probability_ )
            {
                mInputWeights[i] = source_.mInputWeights[i];
            }
        }
        if ( crossBias_ )
            CrossHiddenBias(probability_, source_);
    }
    else
    {
        for ( unsigned int h=0; h < mHiddenLayer.size(); ++h )
        {
            if ( randGen.GetNumberInRange(0.f, 1.f) < probability_ )
            {
                for ( unsigned int i=0; i < mInputLayer.size(); ++i)
                {
                    mInputWeights[h*mInputLayer.size()+i] = source_.mInputWeights[h*mInputLayer.size()+i];
                }
                if ( crossBias_ )
                    mHiddenBias[h] = source_.mHiddenBias[h];
            }
        }
    }
}

void NeuralNet::CrossHiddenWeights(float probability_, const NeuralNet &source_, bool crossIndividualWeights_, bool crossBias_)
{
    RandomGenerator randGen;
    if ( crossIndividualWeights_ )
    {
        for ( unsigned int i=0; i < mHiddenWeights.size(); ++i)
        {
            if ( randGen.GetNumberInRange(0.f, 1.f) < probability_ )
            {
                mHiddenWeights[i] = source_.mHiddenWeights[i];
            }
        }
        if ( crossBias_ )
            CrossOutputBias(probability_, source_);
    }
    else
    {
        for ( unsigned int p=0; p < mOutputLayer.size(); ++p )
        {
            if ( randGen.GetNumberInRange(0.f, 1.f) < probability_ )
            {
                for ( unsigned int h=0; h < mHiddenLayer.size(); ++h )
                {
                    mHiddenWeights[p*mHiddenLayer.size()+h] = source_.mHiddenWeights[p*mHiddenLayer.size()+h];
                }
                if ( crossBias_ )
                    mOutputBias[p] = source_.mOutputBias[p];
            }
        }
    }
}

void NeuralNet::CrossHiddenBias(float probability_, const NeuralNet &source_)
{
    RandomGenerator randGen;
    for ( unsigned int i=0; i < mHiddenBias.size(); ++i )
    {
        if ( randGen.GetNumberInRange(0.f, 1.f) < probability_ )
        {
            mHiddenBias[i] = source_.mHiddenBias[i];
        }
    }
}

void NeuralNet::CrossOutputBias(float probability_, const NeuralNet &source_)
{
    RandomGenerator randGen;
    for ( unsigned int i=0; i < mOutputBias.size(); ++i )
    {
        if ( randGen.GetNumberInRange(0.f, 1.f) < probability_ )
        {
            mOutputBias[i] = source_.mOutputBias[i];
        }
    }
}

float NeuralNet::CalcOutputError(const std::vector<float> &trainingValues_)
{
    if ( trainingValues_.size() != mOutputLayer.size() )
    {
        assert(!"NeuralNet::CalcOutputError trainingValues_.size() != mOutputLayer.size()");
    	fprintf(stderr, "trainingValues_: %d mOutputLayer: %d", trainingValues_.size(), mOutputLayer.size() );
        return 0.f;
    }
    float error = 0.f;

    for ( unsigned int i=0; i < mOutputLayer.size(); ++i )
    {
        float e = mOutputLayer[i]-trainingValues_[i];
        e *= e;
        error += e;
    }
    error *= 0.5f;
    return error;
}

//void NeuralNet::SetAllHiddenActivations(int val_)
//{
//    for ( unsigned int i=0; i < mHiddenActivations.size(); ++i )
//    {
//        mHiddenActivations[i] = val_;
//    }
//}

//void NeuralNet::SetWeightBias(float val_)
//{
//    for ( unsigned int i=0; i < mInputWeightsBias.size(); ++i )
//    {
//        mInputWeightsBias[i] = val_;
//    }
//    for ( unsigned int i=0; i < mHiddenWeightsBias.size(); ++i )
//    {
//        mHiddenWeightsBias[i] = val_;
//    }
//}

//void NeuralNet::SetWeightBiasToWeights()
//{
//    for ( unsigned int i=0; i < mInputWeightsBias.size(); ++i )
//    {
//        mInputWeightsBias[i] = mInputWeights[i];
//    }
//    for ( unsigned int i=0; i < mHiddenWeightsBias.size(); ++i )
//    {
//        mHiddenWeightsBias[i] = mHiddenWeights[i];
//    }
//}

//void NeuralNet::RandomizeWeightBias(float min_, float max_)
//{
//    RandomGenerator randGen;
//
//    for ( unsigned int i=0; i < mInputWeightsBias.size(); ++i )
//    {
//        mInputWeightsBias[i] = randGen.GetNumberInRange( min_, max_ );
//    }
//    for ( unsigned int i=0; i < mHiddenWeightsBias.size(); ++i )
//    {
//        mHiddenWeightsBias[i] = randGen.GetNumberInRange( min_, max_ );
//    }
//}

//void NeuralNet::SetHiddenWeightBiasForOutputNeuron(int neuron_, float bias_)
//{
//    for ( unsigned int i=0; i < mHiddenLayer.size(); ++i )
//    {
//        mHiddenWeightsBias[neuron_*mHiddenLayer.size()+i] = bias_;
//    }
//}

//void NeuralNet::MutateInputWeightsBiased(float min_, float max_, float mutateRange_, float bias_)
//{
//    RandomGenerator randGen;
//
//    for ( unsigned int i=0; i < mInputWeights.size(); ++i )
//    {
//        float min = mInputWeights[i]-0.5f*mutateRange_;
//        float max = mInputWeights[i]+0.5f*mutateRange_;
//        if ( mInputWeights[i] < mInputWeightsBias[i] )
//            max += bias_;
//        else if ( mInputWeights[i] > mInputWeightsBias[i] )
//            min -= bias_;
//        if ( min < min_ )
//            min = min_;
//        if ( max > max_ )
//            max = max_;
//        mInputWeights[i] = randGen.GetNumberInRange( min, max );
//    }
//}

//void NeuralNet::MutateHiddenWeightsBiased(float min_, float max_, float mutateRange_, float bias_)
//{
//    RandomGenerator randGen;
//
//    for ( unsigned int i=0; i < mHiddenWeights.size(); ++i )
//    {
//        float min = mHiddenWeights[i]-0.5f*mutateRange_;
//        float max = mHiddenWeights[i]+0.5f*mutateRange_;
//        if ( mHiddenWeights[i] < mHiddenWeightsBias[i] )
//            max += bias_;
//        else if ( mHiddenWeights[i] > mHiddenWeightsBias[i] )
//            min -= bias_;
//        if ( min < min_ )
//            min = min_;
//        if ( max > max_ )
//            max = max_;
//        mHiddenWeights[i] = randGen.GetNumberInRange( min, max );
//    }
//}

//void NeuralNet::ModifyWeightBiasToWeights(float modify_)
//{
//    for ( unsigned int i=0; i < mInputWeightsBias.size(); ++i)
//    {
//        mInputWeightsBias[i] = mInputWeights[i]*modify_ + mInputWeightsBias[i] * (1-modify_);
//    }
//    for ( unsigned int i=0; i < mHiddenWeightsBias.size(); ++i )
//    {
//        mHiddenWeightsBias[i] = mHiddenWeights[i]*modify_ + mHiddenWeightsBias[i] * (1-modify_);
//    }
//}

//void NeuralNet::ModifyWinnerWeightBiasToWeights(float modify_)
//{
//    for ( unsigned int h = 0; h < mHiddenActivations.size(); ++h )
//    {
//        if ( mHiddenActivations[h] )
//        {
//            for ( unsigned int i=0; i < mInputLayer.size(); ++i)
//            {
//                int index = h*mInputLayer.size()+i;
//                mInputWeightsBias[index] = mInputWeights[index]*modify_ + mInputWeightsBias[index] * (1-modify_);
//            }
//            for ( unsigned int i=0; i < mOutputLayer.size(); ++i )
//            {
//                int index = h*mOutputLayer.size()+i;
//                mHiddenWeightsBias[index] = mHiddenWeights[index]*modify_ + mHiddenWeightsBias[index] * (1-modify_);
//            }
//        }
//    }
//}

//void NeuralNet::ModifyWeightsForWinners(float min_, float max_, float mutateRange_, float bias_)
//{
//    RandomGenerator randGen;
//
//    for ( unsigned int h = 0; h < mHiddenActivations.size(); ++h )
//    {
//        if ( mHiddenActivations[h] )
//        {
//            for ( unsigned int i=0; i < mInputLayer.size(); ++i)
//            {
//                int index = h*mInputLayer.size()+i;
//                float min = mInputWeights[index]-0.5f*mutateRange_;
//                float max = mInputWeights[index]+0.5f*mutateRange_;
//
//                if ( mInputWeights[index] < mInputWeightsBias[index] )
//                    max += bias_;
//                else if ( mInputWeights[index] > mInputWeightsBias[index] )
//                    min -= bias_;
//
//                if ( min < min_ )
//                    min = min_;
//                if ( max > max_ )
//                    max = max_;
//                mInputWeights[index] = randGen.GetNumberInRange( min, max );
//            }
//            for ( unsigned int i=0; i < mOutputLayer.size(); ++i )
//            {
//                int index = i*mHiddenTresholds.size()+h;
//                float min = mHiddenWeights[index]-0.5f*mutateRange_;
//                float max = mHiddenWeights[index]+0.5f*mutateRange_;
//
//                if ( mHiddenWeights[index] < mHiddenWeightsBias[index] )
//                    max += bias_;
//                else if ( mHiddenWeights[index] > mHiddenWeightsBias[index] )
//                    min -= bias_;
//
//                if ( min < min_ )
//                    min = min_;
//                if ( max > max_ )
//                    max = max_;
//                mHiddenWeights[index] = randGen.GetNumberInRange( min, max );
//            }
//        }
//    }
//}

#endif // NEURAL_AI
