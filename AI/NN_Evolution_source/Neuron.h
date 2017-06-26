
class Evolver;
class NeuralNetwork;

class Neuron
{
	friend class NeuralNetwork;
	
	// connection weights
	// for each neuron that inputs to this one
	std::vector<float> weights;
	
	float threshold;	// a.k.a. "bias"
						// could be stored as an extra weight
						// but seems clearer to have it separate
	
	float randScale;
	
	float output;
	
	float sigmoid(float a)
	{
		return 1.f / ( 1.f + pow(M_E, -a) );
	}
	
public:
	void init(int inputNeuronCount, float randScale)
	{
		// need a weight for the connection of each other neuron to this
		weights.assign(inputNeuronCount, 0.f);
		
		this->randScale = randScale;
		
		randomise();
	}
	
	// randomise all weights and the threshold
	void randomise()
	{
		threshold = fRand2()*randScale;
		
		for (int i = 0; i < weights.size(); i ++)
			weights[i] = fRand2()*randScale;
	}
	
	float getOutput()
	{
		return output;
	}
	
	// used when in the input layer
	// just passes the input on
	void process(float input)
	{
		output = input;
	}
	
	// used when in the hidden or output layer(s)
	void process(std::vector<Neuron> &inputNeurons)
	{
		output = 0.f;
		
		// sum the inputs
		for (int i = 0; i < inputNeurons.size(); i ++)
			output += inputNeurons[i].output * weights[i];
		
		// bias
		output -= threshold;
		
		// through "logistic sigmoid", smoothes the output
		output = sigmoid(output);
	}
};
