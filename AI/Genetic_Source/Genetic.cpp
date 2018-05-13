#include "Genetic.h"
#include <math.h>

/**
* Chromosome constructor.
*/
Chromosome::Chromosome(float nangle, double npower)
{
    angle=nangle;
    power=npower;
    fitness=-1000;
    ftime=-1;
}

/**
* @return Returns the X coordinated of the projectile at the time T.
*/
double Chromosome::xaxis(double x0, double v0, float angle, double t)
{
    return x0+v0*cos(angle)*t;
}

/**
* @return Returns the Y coordinated of the projectile at the time T.
*/
double Chromosome::yaxis(double y0, double v0, float angle, double t)
{
    float gravity=10;
    return y0+v0*sin(angle)*t-(gravity*t*t)/2;
}

/**
* Degrees to Radians..
*/
double Chromosome::toRadians(double angle)
{
    return (angle*PI)/180;
}

/**
* @return Distance between two points.
*/
double Chromosome::getDistance(double x1,double y1, double x2, double y2)
{
    return sqrt( (x2-x1)*(x2-x1)+(y2-y1)*(y2-y1) );
}

/**
* Fitness Function
*/
double Chromosome::calculateFitness(double distanceFromTarget)
{
    vector3df v0=vector3df(distanceFromTarget,0,0);
    double d=getDistance(v0.X,v0.Y,0,0);

    float t=0;
    float oldd=0;

    do
    {
        t+=0.01;

        double X=xaxis(0,power,toRadians(-angle),t);
        double Y=yaxis(0,power,toRadians(-angle),t);
        double Z=0;
        v0.X-=X;
        v0.Y+=Y;

        oldd=d;
        d = getDistance(v0.X,v0.Y,0,0);

    }while(d<oldd);

    fitness=oldd;
    ftime=t;

    return oldd;
}

/**
* Return the time of the best distance from target.
*/
float Chromosome::getFitnessTime()
{
    return ftime;
}

/**
* Return Fitness.
*/
double Chromosome::getFitness()
{
    return fitness;
}

double Chromosome::getPower()
{
    return power;
}

float Chromosome::getAngle()
{
    return angle;
}

/**
* Set new values to the chromosome 'n reset fitness.
*/
void Chromosome::setValues(float nangle, double npower)
{
    angle=nangle;
    power=npower;
    fitness=-1000;
    ftime=-1;
}

irr::core::stringc Chromosome::toString()
{
    irr::core::stringc str="{angle:";
    str+=-angle;
    str+=" power:";
    str+=power;
    str+=" fitness:";
    str+=fitness;
    str+="}";
    return str;

}

///============================================================================

Genetic::Genetic(float crossRate,float mutRate)
{
    this->crossoverRate=crossRate;
    this->mutationRate=mutRate;
    srand ( time(NULL) );
}

void Genetic::redefineRates(float crossRate,float mutRate)
{
    this->crossoverRate=crossRate;
    this->mutationRate=mutRate;
}

double Genetic::frand(double low, double high)
{
    double temp;

    /* swap low & high around if the user makes no sense */
    if (low > high)
    {
        temp = low;
        low = high;
        high = temp;
    }

    /* calculate the random number & return it */
    temp = (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (high - low) + low;
    return temp;
}

void Genetic::generatePopulation(int size)
{
    ///TODO: testar "garbage collector manual"
    for (it=population.begin(); it!=population.end(); ++it)
    {
        Chromosome* c=*it;
        delete c;
    }

    population.clear();

    for (int i=0;i<size;i++)
    {
        population.push_back(new Chromosome(frand(-180,180),frand(0,15)));
    }
}

void Genetic::calculateFitness(double distance)
{
    for (it=population.begin(); it!=population.end(); ++it)
    {
        Chromosome* c=*it;
        c->calculateFitness(distance);
    }
}

void Genetic::printPopulation()
{
    for (it=population.begin(); it!=population.end(); ++it)
    {
        Chromosome* c=*it;
        cout << " " << c->toString().c_str() << endl;
    }
    cout << endl;
}

void Genetic::sortPopulation()
{
    population.sort(SortProcess());
}

Chromosome* Genetic::getChromosome(int index)
{
    std::list<Chromosome*>::iterator iterat;

    if (index+1>population.size()) return NULL;

    iterat=population.begin();
    Chromosome* c=NULL;
    for(int count = 0;count<index;count++)
    {
        ++iterat;
        c=*iterat;
    }

    return c;
}

Chromosome* Genetic::getRandomChromosome()
{
    Chromosome* c;
    do
    {
        int x=rand()%(population.size()-1);
        c=getChromosome(x);
    }while(c==NULL);
    return c;
}

std::list<Chromosome*> Genetic::select(int rate)
{
    std::list<Chromosome*> selected;

    for (int i=0;i<rate;i++)
    {
        Chromosome* c=getRandomChromosome();
        selected.push_back(c);
    }

    return selected;
}

void Genetic::crossover(Chromosome* c1,Chromosome* c2)
{
    float tempAngle=c1->getAngle();
    c1->setValues(c2->getAngle(),c1->getPower());
    c2->setValues(tempAngle,c2->getPower());
}

void Genetic::mutation(std::list<Chromosome*> l)
{
    std::list<Chromosome*>::iterator iterat;
    for (iterat=l.begin(); iterat!=l.end(); ++iterat)
    {
        Chromosome* c=*iterat;
        c->setValues(frand(-180,180),c->getPower());
    }
}

Chromosome* Genetic::execute(double distanceFromTarget,int populationSize,int totalGenerations, bool show)
{
    system("CLS");
    if (show) cout << "INITIAL POPULATION____________________________" << endl;
    generatePopulation(populationSize);
    calculateFitness(distanceFromTarget);
    sortPopulation();
    if (show) printPopulation();

    for (int i=0;i<totalGenerations;i++)
    {
        if (show) cout << "GENERATION "<< i+1 <<"____________________________" << endl;
        calculateFitness(distanceFromTarget);
        sortPopulation();
        if (show) printPopulation();

        for (int i=10;i<population.size()*crossoverRate;i++)
        {
            Chromosome* c1=getRandomChromosome();
            Chromosome* c2=getRandomChromosome();
            crossover(c1,c2);
            c1->calculateFitness(distanceFromTarget);
            c2->calculateFitness(distanceFromTarget);
        }

        calculateFitness(distanceFromTarget);
        sortPopulation();


        std::list<Chromosome*> selectedToMutate=select(population.size()*mutationRate);
        mutation(selectedToMutate);

        calculateFitness(distanceFromTarget);
        sortPopulation();

        if (show) printPopulation();
    }
    if (show) cout <<"BEST CHROMOSOME:____________________________" << endl;
    it = population.begin();
    Chromosome* winner=*it;
    if (show) cout << winner->toString().c_str() << endl;

    return winner;
}


