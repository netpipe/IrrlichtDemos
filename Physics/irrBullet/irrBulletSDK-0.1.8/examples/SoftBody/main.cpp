#include <iostream>
#include "softbodyexample.h"

using namespace std;

int main()
{
    CSoftbodyExample* example = new CSoftbodyExample();
    example->runExample();

    delete example;
    return 0;
}
