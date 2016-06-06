// blindside's neuralnetworking example demo.
#ifndef ANN_INCLUDE_H
#define ANN_INCLUDE_H

#include <vector>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

using namespace std;

const float learningRate = 0.05f;

class Ann
{
public:
   class Link;
   class BackLink;
   class INode;
   class ITrainer
   {
   public:
      virtual void train(std::vector<float>& inputs,
         std::vector<float>& outputs, Ann* ann) = 0;
   };

   Ann(size_t numberOfWorkLayers = 2, size_t numberOfWorkers = 8, size_t numberOfInputs = 3,
      size_t numberOfOutputs = 3);
   ~Ann();

   void setTrainer(Ann::ITrainer* trainerIn)
   {
      trainer = trainerIn;
   }

   void backPropagate(size_t iterations);

   void randomizeInputs();

   void setInput(const std::vector<float>& input);

   std::vector<float> getOutput();

   std::vector<float> getInput();

   void saveNeuralMap(const char* filename);

   bool loadNeuralMap(const char* filename);
private:
   Ann::ITrainer* trainer;
   
   std::vector<Ann::INode*> inputLayer;
   std::vector<std::vector<Ann::INode*> > workLayers;
   std::vector<Ann::INode*> outputLayer;

   // Sizes
   size_t inputLayerSize;
   size_t workLayersSize;
   size_t workLayerSize;
   size_t outputLayerSize;

   // Used while training.
   std::vector<float> inputValues;
   std::vector<float> outputValues;
};


class Ann::Link
{
public:
   Ann::Link(Ann::INode* nodeLink) : pointer(nodeLink)
   {
      // Initialise weight to a random value between [-2.0, 2.0]
      weight = (rand()%400) * 0.01f - 2.0f;
   }

   float weight;
   Ann::INode* pointer;   
};



class Ann::BackLink
{
public:
   Ann::BackLink(Ann::INode* nodeLink, float* weightIn)
      : pointer(nodeLink), weight(weightIn) {}

   float* weight;
   Ann::INode* pointer;   
};



class Ann::INode
{
public:
   virtual float getOutput() = 0;
   virtual void calculateDelta() {};
   virtual void updateFreeValues() {};

   std::vector<Ann::Link>& getLinks()
   {
      return links;
   }   

   float delta;
   float bias;
   std::vector<Ann::BackLink> backLinks;

protected:
   std::vector<Ann::Link> links;
};



class BasicNode : public Ann::INode
{
public:
   BasicNode(std::vector<Ann::INode*>& inputs)
   {
      bias = (rand()%100) * 0.01f;

      size_t inputsSize = inputs.size();
      links.reserve(inputsSize);

      for(size_t i = 0;i < inputsSize;++i)
      {
         Ann::Link currentLink(inputs[i]);
         links.push_back(currentLink);

         inputs[i]->backLinks.push_back(Ann::BackLink(this, &links[i].weight));
      }
   }

   virtual float getOutput()
   {
      float netValue = bias;

      // Summing function.
      size_t linksSize = links.size();
      for(size_t i = 0;i < linksSize;++i)
         netValue += links[i].pointer->getOutput() * links[i].weight;

      // Transfer function.
      netValue = 1.0f / (1.0f + exp(-netValue));

      return netValue;
   }

   virtual void calculateDelta()
   {
      float errorFactor = 0.0f;
      size_t backLinksSize = backLinks.size();
      for(size_t i = 0;i < backLinksSize;++i)
         errorFactor += (*backLinks[i].weight) * backLinks[i].pointer->delta;

      float currentOutput = getOutput();
      delta = currentOutput * (1.0f - currentOutput) * errorFactor;
   }

   virtual void updateFreeValues()
   {
      bias = bias + learningRate * delta;

      size_t currentLinksSize = links.size();
      for(size_t g = 0;g < currentLinksSize;++g)
         links[g].weight = links[g].weight + learningRate
            * links[g].pointer->getOutput() * delta;
   }
};

class InputNode : public Ann::INode
{
public:
   InputNode(float inputValue) : value(inputValue)   {}

   virtual float getOutput()
   {
      return value;
   }

   float value;
};

Ann::Ann(size_t numberOfWorkLayers, size_t numberOfWorkers, size_t numberOfInputs, size_t numberOfOutputs)
: trainer(0)
{
   // Seed rand.
   srand(clock());

   for(size_t i = 0;i < numberOfInputs;++i)
      inputLayer.push_back(new InputNode(0));

   inputLayerSize = inputLayer.size();
   
   // Push back the input layer.
   workLayers.push_back(inputLayer);

   for(size_t g = 0;g < numberOfWorkLayers;++g)
   {
      std::vector<Ann::INode*> workLayer;

      for(size_t i = 0;i < numberOfWorkers;++i)
         workLayer.push_back(new BasicNode(workLayers[g]));

      workLayers.push_back(workLayer);
   }

   workLayerSize = numberOfWorkers;
   workLayersSize = workLayers.size();
   
   for(size_t i = 0;i < numberOfOutputs;++i)
      outputLayer.push_back(new BasicNode(workLayers[workLayersSize - 1]));

   outputLayerSize = outputLayer.size();
}

void Ann::randomizeInputs()
{
   for(size_t i = 0;i < inputLayerSize;++i)
      ((InputNode*)inputLayer[i])->value = inputValues[i] = (rand()%100) * 0.01f;
}

void Ann::backPropagate(size_t iterations)
{
   for(size_t l = 0;l < iterations;++l)
   {
      // Reset output and input values.
      inputValues.resize(inputLayerSize);
      outputValues.resize(outputLayerSize);

      // Training is performed here.
      if(trainer)   
         trainer->train(inputValues, outputValues, this);

      // Update input values.
      for(size_t i = 0;i < inputLayerSize;++i)
         ((InputNode*)inputLayer[i])->value = inputValues[i];

      // Here we backpropagate and calculate the error values:
      for(size_t i = 0;i < outputLayerSize;++i)
      {
         float outputValue = outputLayer[i]->getOutput();
         float outputEFactor = outputValues[i] - outputValue;
         outputLayer[i]->delta = outputValue * (1.0f - outputValue) * outputEFactor;
      }

      // We only go down to level 1 as we don't need to calculate
      // the input node's deltas/weights/biases.
      for(size_t i = workLayersSize - 1;i >= 1;--i)
      {
         for(size_t g = 0;g < workLayerSize;++g)
         {
            workLayers[i][g]->calculateDelta();
            workLayers[i][g]->updateFreeValues();
         }
      }

      for(size_t i = 0;i < outputLayerSize;++i)
         outputLayer[i]->updateFreeValues();
   }
}

void Ann::setInput(const std::vector<float>& input)
{
   size_t inputSize = input.size();

   // Update input values.
   for(size_t i = 0;i < inputLayerSize && i < inputSize;++i)
      ((InputNode*)inputLayer[i])->value = input[i];
}

std::vector<float> Ann::getOutput()
{
   std::vector<float> output;

   for(size_t i = 0;i < outputLayerSize;++i)
      output.push_back(outputLayer[i]->getOutput());

   return output;
}

std::vector<float> Ann::getInput()
{
   std::vector<float> input;

   for(size_t i = 0;i < inputLayerSize;++i)
      input.push_back(inputLayer[i]->getOutput());

   return input;
}

Ann::~Ann()
{
   for(size_t i = 0;i < inputLayerSize;++i)
      delete inputLayer[i];
   
   for(size_t g = 1;g < workLayersSize;++g)
      for(size_t i = 0;i < workLayerSize;++i)
         delete (workLayers[g])[i];

   for(size_t i = 0;i < outputLayerSize;++i)
      delete outputLayer[i];
}

void Ann::saveNeuralMap(const char* filename)
{
   FILE* file = fopen(filename, "wb");

   fwrite(&workLayersSize, sizeof(size_t), 1, file);
   fwrite(&workLayerSize, sizeof(size_t), 1, file);
   fwrite(&inputLayerSize, sizeof(size_t), 1, file);
   fwrite(&outputLayerSize, sizeof(size_t), 1, file);

   for(size_t g = 1;g < workLayersSize;++g)
   {
      for(size_t i = 0;i < workLayerSize;++i)
      {
         fwrite(&workLayers[g][i]->bias, sizeof(float), 1, file);

         size_t linksSize = workLayers[g][i]->getLinks().size();
         for(size_t j = 0;j < linksSize;++j)
            fwrite(&workLayers[g][i]->getLinks()[j].weight, sizeof(float), 1, file);
      }
   }
   
   for(size_t i = 0;i < outputLayerSize;++i)
   {
      fwrite(&outputLayer[i]->bias, sizeof(float), 1, file);

      size_t linksSize = outputLayer[i]->getLinks().size();
      for(size_t j = 0;j < linksSize;++j)
         fwrite(&outputLayer[i]->getLinks()[j].weight, sizeof(float), 1, file);
   }
   
   fclose(file);
}

bool Ann::loadNeuralMap(const char* filename)
{
   FILE* file = fopen(filename, "rb");

   size_t fWorkLayersSize = 0;
   size_t fWorkLayerSize = 0;
   size_t fInputLayersSize = 0;
   size_t fOutputLayersSize = 0;

   if(!file)
   {
      printf("\nAnn: Error loading Neural Map, IO error.");
      return false;
   }

   fread(&fWorkLayersSize, sizeof(size_t), 1, file);
   fread(&fWorkLayerSize, sizeof(size_t), 1, file);
   fread(&fInputLayersSize, sizeof(size_t), 1, file);
   fread(&fOutputLayersSize, sizeof(size_t), 1, file);

   if(   fWorkLayersSize != workLayersSize ||
      fWorkLayerSize != workLayerSize ||
      fInputLayersSize != inputLayerSize ||
      fOutputLayersSize != outputLayerSize)
   {
      printf("\nAnn: Error loading Neural Map, dimensions do not match.");
      fclose(file);
      return false;
   }

   for(size_t g = 1;g < workLayersSize;++g)
   {
      for(size_t i = 0;i < workLayerSize;++i)
      {
         fread(&workLayers[g][i]->bias, sizeof(float), 1, file);

         size_t linksSize = workLayers[g][i]->getLinks().size();
         for(size_t j = 0;j < linksSize;++j)
            fread(&workLayers[g][i]->getLinks()[j].weight, sizeof(float), 1, file);
      }
   }
   
   for(size_t i = 0;i < outputLayerSize;++i)
   {
      fread(&outputLayer[i]->bias, sizeof(float), 1, file);

      size_t linksSize = outputLayer[i]->getLinks().size();
      for(size_t j = 0;j < linksSize;++j)
         fread(&outputLayer[i]->getLinks()[j].weight, sizeof(float), 1, file);
   }
   
   fclose(file);

   return true;
}

#endif

// Copyright Ahmed Hilali 2008 (C)
