#include <iostream>
#include "collisionexample.h"

using namespace std;

int main()
{
    CCollisionExample* example = new CCollisionExample();
    example->runExample();

    delete example;
    return 0;
}
