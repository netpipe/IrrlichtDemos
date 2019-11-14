#include <iostream>
#include "liquidbodyexample.h"

using namespace std;

int main()
{
    CLiquidbodyExample* example = new CLiquidbodyExample();
    example->runExample();

    delete example;
    return 0;
}
