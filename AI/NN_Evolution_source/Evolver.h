
#define TOURNAMENT_SIZE 2

class Evolver
{
	std::vector<NeuralNetwork> population;
	int neuronsPerNetwork;
	
	int popSize;
	int eliteCount;
	
	E_SELECTOR_TYPE selectorType;
	float probCrossover;
	float probMutate;
	float mutateScale;
	
	// count of the generations
	int generation;
	
	// utility method
	template <class T>
	void swap(T &a, T &b)
	{
		T temp = a;
		a = b;
		b = temp;
	}
	
	// Selector factory method
	Selector *createSelector(E_SELECTOR_TYPE type)
	{
		switch (type)
		{
		case EST_ROULETTE_WHEEL:
			return new RouletteWheelSelector(this);
			
		case EST_TOURNAMENT:
			return new TournamentSelector(this, TOURNAMENT_SIZE);
			
		default:
			return NULL;
		}
	}
	
	// sorts the population by fitness (for elitism)
	// not the fastest sort method, but this area is not a bottleneck
	void sortPop()
	{
		for (int i = 0; i < population.size(); i ++)
		{
			int best = i;
			
			for (int j = i; j < population.size(); j ++)
			{
				if (population[j].getFitness() > population[best].getFitness())
					best = j;
			}
			
			swap(population[best], population[i]);
		}
	}
	
	// single point crossover applied to two neural networks
	// NNs should be topologically equal
	void crossover(NeuralNetwork &n1, NeuralNetwork &n2)
	{
		// For crossover, the entire contents of the network is needed as an array.
		std::vector<float> data1 = n1.getData();
		std::vector<float> data2 = n2.getData();
		
		int i = 1 + rand() % (data1.size()-1);
		
		for (; i < data1.size(); i ++)
			swap(data1[i], data2[i]);
		
		n1.setData(data1);
		n2.setData(data2);
	}
	
	// chromosome is floating point so bit flipping is not easy
	// instead, mutation just deviates a weight by a random amount.
	void mutate(NeuralNetwork &nn)
	{
		std::vector<float> data = nn.getData();
		
		for (int i = 0; i < data.size(); i ++)
		{
			if (fRand() < probMutate)
				data[i] += fRand2()*mutateScale;
		}
		
		nn.setData(data);
	}
	
public:

	// a seed NN is passed to the evolver which specifies the desired number of neurons and neuron layers etc.
	// its weights etc are randomised though.
	Evolver(NeuralNetwork nn, int popSize, int eliteCount,
		E_SELECTOR_TYPE selectorType,
		float probCrossover, // e.g. 0.6-0.95 (from course notes)
		float probMutate, // e.g. 0.01
		float mutateScale // max amount by which a weight is mutated. e.g. 10 for a range of -10 to 10.
		)
	{
		this->popSize = popSize;
		this->eliteCount = eliteCount;
		this->selectorType = selectorType;
		this->probCrossover = probCrossover;
		this->probMutate = probMutate;
		this->mutateScale = mutateScale;
		
		generation = 0;
		
		// Count the total neurons in this network topology (needed in crossover)
		neuronsPerNetwork = nn.size();
		
		// Create and randomise an initial population
		// using the topology of the "seed" network
		for (int i = 0; i < popSize; i ++)
		{
			population.push_back(nn);
			population[i].randomise();
		}
	}
	
	std::vector<NeuralNetwork> &getPopulation()
	{
		return population;
	}
	
	int getGenCount()
	{
		return generation;
	}
	
	// set the fitness of a population member...
	// alternatively the population can be got with getPopulation and the
	// fitness set directly
	void setFitness(int i, float fitness)
	{
		population[i].setFitness(fitness);
	}
	
	NeuralNetwork &getFittest()
	{
		if (population.size() == 0)
			throw (const char *)"Error: NeuralNetwork::getFittest: population of zero size";
		
		NeuralNetwork &fittest = population[0];
		
		for (int i = 1; i < population.size(); i ++)
		{
			if (population[i].getFitness() > fittest.getFitness())
				fittest = population[i];
		}
		
		return fittest;
	}
	
	// creates a new generation
	// neural network population should have been rated (with setFitness) before calling this.
	void step()
	{
		// note: if the population size is odd numbered, will create popsize+1 children
		// and throw away the last...
		
		// sort the population for elitism
		sortPop();
		
		std::vector<NeuralNetwork> newPopulation;
		
		Selector *selector = createSelector(selectorType);
		
		int i = 0;
		
		// put a number of elite unchanged into the new population
		for (; i < eliteCount && i < popSize; i ++)
			newPopulation.push_back(population[i]);
		
		// form the rest of the population using selection
		for (; i < popSize; i += 2)
		{
			// randomly select two parents (based on fitness...)
			NeuralNetwork n1 = selector->select();
			NeuralNetwork n2 = selector->select();
			
			// if crossover probability; apply crossover (at random point)
			if (fRand() < probCrossover)
				crossover(n1,n2);
			
			// so now there are two children
			
			// mutate children (for each gene location, if mutation probability; flip bit)
			// (actually bits aren't flipped, as the genes are floating point.
			// they are simply randomly deviated)
			mutate(n1);
			mutate(n2);
			
			// add children to population!
			newPopulation.push_back(n1);
			
			if (i < popSize-1) // only add the second child if there's room (popSize might be odd numbered)
				newPopulation.push_back(n2);
		}
		
		delete selector;
		
		population = newPopulation;
		
		generation ++;
	}
};
