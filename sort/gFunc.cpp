#include "sort.h"
#include <iostream>
#include <random>

using namespace Util;

//index
int GlobalFunc::iParent(int index)
{
    return (index -1) >=0? (index -1) :0;
}
int GlobalFunc::iLeftChild(int index)
{
    return 2*index +1;
}
int GlobalFunc::iRightChild(int index)
{
    return 2*index +2;
}

int GlobalFunc::random_integers(int max, int min, int data[], int count)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator
    std::uniform_int_distribution<> distr(min, max); // define the range

    for(int n=0; n<count; ++n)
        data[n] = distr(eng);

    return 0;
}