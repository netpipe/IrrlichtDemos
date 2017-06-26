
class Evolver;

// feed forward (possibly multilayer) perceptron
// layers (at least two!) must be added before use
// each neuron is connected to all others in the next layer
class NeuralNetwork
{
	float randScale;
	
	// neuron layers
	std::vector<std::vector<Neuron> > layers;
	
	float fitness; // how well this NN solves the problem. used by the GA.
	
	// set the number of weights and initialise weights and thresholds
	// in a vector of neurons
	// inputNeuronCount should == the number of neurons in the previous layer
	void initLayer(std::vector<Neuron> &layer, int inputNeuronCount)
	{
		for (int i = 0; i < layer.size(); i ++)
			layer[i].init(inputNeuronCount, randScale);
	}
	
public:

	// Default constructor required so NeuralNetwork can be instanced as a class member
	// (in Vehicle)
	NeuralNetwork()
	{
		randScale = 1.f;
	}
	
	NeuralNetwork(float randScale)
	{
		this->randScale = randScale;
	}
	
	// Get NN as an array. For mutation, crossover etc.
	std::vector<float> getData()
	{
		std::vector<float> data;
		
		for (int i = 0; i < layers.size(); i ++)
		{
			for (int j = 0; j < layers[i].size(); j ++)
			{
				Neuron &n = layers[i][j];
				
				for (int k = 0; k < n.weights.size(); k ++)
					data.push_back(n.weights[k]);
				
				data.push_back(n.threshold);
			}
		}
		
		return data;
	}
	
	// set NN from array
	void setData(const std::vector<float> &data)
	{
		int dataIndex = 0;
		
		for (int i = 0; i < layers.size(); i ++)
		{
			for (int j = 0; j < layers[i].size(); j ++)
			{
				Neuron &n = layers[i][j];
				
				for (int k = 0; k < n.weights.size(); k ++)
					n.weights[k] = data[dataIndex++];
				
				n.threshold = data[dataIndex++];
			}
		}
	}
	
	float getFitness()
	{
		return fitness;
	}
	
	void setFitness(float f)
	{
		fitness = f;
	}
	
	// randomise all neurons
	void randomise()
	{
		for (int i = 0; i < layers.size(); i ++)
		{
			for (int j = 0; j < layers[i].size(); j ++)
			{
				layers[i][j].randomise();
			}
		}
	}
	
	// the total number of neurons in this network
	// not used enough to necessitate caching the result...
	int size()
	{
		int count = 0;
		
		for (int i = 0; i < layers.size(); i ++)
			count += layers[i].size();
		
		return count;
	}
	
	// adds on to the end
	// only intended to be used when initialising the network
	void addLayer(int neuronCount)
	{
		std::vector<Neuron> newLayer(neuronCount);
		
		// initialise each neuron, telling it how many other neurons (in previous layer) connect to it
		if (layers.size() > 0)
			initLayer(newLayer, (layers.end()-1)->size());
		else
			initLayer(newLayer, 0); // first layer, so no neurons in previous layer
		
		layers.push_back(newLayer);
	}
	
	// number of inputs should be the same as the number of neurons in the first layer
	void process(std::vector<float> &inputs, std::vector<float> &outputs)
	{
		if (layers.size() == 0)
		{
			printf("Error: NeuralNetwork::process: no neuron layers.\n");
			return;
		}
		
		if (inputs.size() != layers[0].size())
		{
			printf("Error: NeuralNetwork::process: incorrect number of inputs given\n");
			return;
		}
		
		for (int i = 0; i < layers.size(); i ++)
		{
			for (int j = 0; j < layers[i].size(); j ++)
			{
				if (i == 0) // input layer, so input is a single value not a list of neurons
					layers[i][j].process(inputs[j]);
				else
					layers[i][j].process(layers[i-1]);
			}
		}
		
		// Then set the outputs
		
		std::vector<Neuron> &outputLayer = layers.back();
		
		outputs.assign(outputLayer.size(), 0.f);
		
		for (int i = 0; i < outputLayer.size(); i ++)
			outputs[i] = outputLayer[i].getOutput();
	}
};
