/*-----------------------------------------------------------------------------*
| headerfile random.h                                                          |
|                                                                              |
| version 1.10                                                                 |
| date: (05.04.2007)                                                           |
|                                                                              |
| author:  Michal Švantner                                                     |
|                                                                              |
| structure for finding random number                                          |
*-----------------------------------------------------------------------------*/

#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <time.h>

struct Random
{
   //--- CONSTRUCTOR ---
   Random()
   {
      time_t seconds;
      time(&seconds);
      srand((unsigned int) seconds);
   }
   
   //--- DESTRUCTOR ---
   ~Random(){}
   
   //--- FIND RANDOM NUMBER BETWEEN 0 AND NUMBER ---
   int operator () (int num)
   {
      return rand() % (num + 1);
   }
   
   //--- FIND RANDOM BETWEEN TWO NUMBERS ---
   int operator () (int num1, int num2)
   {
      return rand() % (num2 - num1 + 1) + num1;
   }
} rnd;
#endif
