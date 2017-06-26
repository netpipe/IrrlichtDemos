
class Evolver;

// Selector factory is the createSelector method of Evolver
enum E_SELECTOR_TYPE
{
	EST_ROULETTE_WHEEL = 0, // starts at 0 so it can be used as an index
	EST_TOURNAMENT
};

// base class for GA selectors
// NOTE: Evolver's population should not be modified after the instancing of a selector.
// (as the selector may gather and cache population "statistics" in its constructor)
class Selector
{
protected:
	Evolver *evolver;
	
public:
	Selector(Evolver *e)
	{
		this->evolver = e;
	}
	
	virtual NeuralNetwork select() = 0;
};

// Performs fitness proportionate (roulette wheel) selection
class RouletteWheelSelector : public Selector
{
	float totalFitness; // sum of all fitnesses
	
public:
	RouletteWheelSelector(Evolver *e)
		: Selector(e)
	{
		totalFitness = 0.f;
		
		std::vector<NeuralNetwork> &pop = e->getPopulation();
		
		for (int i = 0; i < pop.size(); i ++)
			totalFitness += pop[i].getFitness();
	}
	
	NeuralNetwork select()
	{
		std::vector<NeuralNetwork> &population = evolver->getPopulation();
		
		float position = fRand() * totalFitness;
		
		// now find which individual is at that position
		
		float f = 0.f;
		
		for (int i = 0; i < population.size(); i ++)
		{
			f += population[i].getFitness();
			
			if (f >= position)
				return population[i];
		}
		
		// Should never reach here (unless rounding errors or something?)
		// so just return a random individual
		printf("Note: RouletteWheelSelector::select: roulette wheel failed\n");
		return population[ rand() % population.size() ];
	}
};

// note: an individual can be added to the tournament multiple times.
class TournamentSelector : public Selector
{
	int tSize;
	
public:
	TournamentSelector(Evolver *e, int tSize)
		: Selector(e)
	{
		if (tSize < 2)
			throw (const char *)"Error: TournamentSelector::TournamentSelector: tournament size must be > 1";
		
		if (tSize > e->getPopulation().size())
			tSize = e->getPopulation().size();
		
		this->tSize = tSize;
	}
	
	NeuralNetwork select()
	{
		std::vector<NeuralNetwork> &population = evolver->getPopulation();
		std::vector<NeuralNetwork *> combatants;
		
		for (int i = 0; i < tSize; i ++)
			combatants.push_back( &population[rand() % population.size()] );
		
		// find fittest in tournament
		NeuralNetwork *fittest = combatants[0];
		
		for (int i = 0; i < combatants.size(); i ++)
		{
			if (combatants[i]->getFitness() > fittest->getFitness())
				fittest = combatants[i];
		}
		
		return *fittest;
	}
};
