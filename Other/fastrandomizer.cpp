  //!========XORSHIFT RANDOMIZER========
  //! A class using George Marsaglia's "Xorshift-128" algorithm.
  //! Provides very fast and quality sequence
  //! of 2^{128}-1 32-bit random numbers.
#include <iostream>
#include <stdlib.h>
#include <windows.h>
using namespace std;


 
 

class htRandomizer
{
  unsigned long seed,x,y,z,w;
public:
  htRandomizer(unsigned long state=GetTickCount()) {RndSet(state);}

  //! Randomization.
  void RndSet(unsigned long state=123456789)
  {seed=x=state,y=362436069,z=521288629,w=88675123;}

  //! More randomization.
  void RndShift() {RndSet(Rnd());}

  //! For farther restore a sequence we need to know a seed.
  unsigned long RndSeed(void) {return seed;}

  //! Returns unsigned long in [0..ULONG_MAX].
  unsigned long Rnd()
  {
    unsigned long t=(x^(x<<11)); x=y;y=z;z=w;
    return(w=(w^(w>>19))^(t^(t>>8)));
  }

  //! Returns float in [0,1)
  float Rndf()
  {
    unsigned long f=Rnd();
    f=0x3f800000U|(0x007fffffU&f);
    float val=*(float*)&f-1.f;
    return val;
  }

  //! Returns double in [0,1].
  double Rndd()
  {
    return Rnd()/(double)0xffffffffUL; //! ULONG_MAX.
  }

  //! Returns unsigned long in [num1..num2].
  unsigned long Rnd(unsigned long num1,unsigned long num2)
  {
    if(num1==num2) return num1;
    float rnd=Rndf();
    if(num2>num1)
      return num1+(unsigned long)(rnd*(num2-num1+1));
    else
      return num2+(unsigned long)(rnd*(num1-num2+1));
  }

  //! Returns int in [num1..num2].
  int Rndi(int num1,int num2)
  {
    if(num1==num2) return num1;
    float rnd=Rndf();
    if(num2>num1)
      return num1+(int)(rnd*(num2-num1+1));
    else
      return num2+(int)(rnd*(num1-num2+1));
  }

  //! Returns float in [num1..num2).
  float Rndf(float num1,float num2)
  {
    if(num1==num2) return num1;
    float rnd=Rndf();
    if(num2>num1)
      return (num2-num1)*rnd+num1;
    else
      return (num1-num2)*rnd+num2;
  }

  //! Returns double in [num1..num2].
  double Rndd(double num1,double num2)
  {
    if(num1==num2) return num1;
    double rnd=Rndd();
    if(num2>num1)
      return (num2-num1)*rnd+num1;
    else
      return (num1-num2)*rnd+num2;
  }

  //! Returns random boolean.
  bool Rndb() {return Rnd()%2;}

  //! Returns random sign.
  int  Rnds() {return Rndb()? 1:-1;}
};

unsigned int rSeed;

inline void seedRand(int seed) {
   rSeed = seed;
}

inline int genRand() {
   rSeed = (214013*rSeed+2531011);
   return (rSeed>>16)&0x7FFF;
}
 

int main() {

   int startTime = GetTickCount();

   htRandomizer htr;
   for (int y = 0; y < 1000; y++) {
      htr.RndSet(y);
      for (int x = 0; x < 1000000; x++) {
         int r = htr.Rnd();
      }
   }

   int endTime = GetTickCount();

   cout << "1st test completed in: " << endTime-startTime << "ms" << endl;

   int startTime2 = GetTickCount();

   for (int y = 0; y < 1000; y++) {
      seedRand(y);
      for (int x = 0; x < 1000000; x++) {
         int r = genRand();
      }
   }

   int endTime2 = GetTickCount();

   cout << "2nd test completed in: " << endTime2-startTime2 << "ms" << endl;

   if (endTime2-startTime2 < endTime-startTime)
      cout << "2nd ";
   else
      cout << "1st ";

   cout << "test completed " << abs((endTime2-startTime2)-(endTime-startTime)) << "ms faster." << endl;

   return 0;
};
