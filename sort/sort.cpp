#include "sort.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    /* code */
    // int a[9] = {5,2,8,4,9,1,3,6,7}; 
    int a[100] = {0};
    Util::GlobalFunc::random_integers(1000,1,a, 100);
    Util::HeapSort hpSort(Util::HeapSort::SHIFT_UP);
    hpSort.SortFunc(a,100);
    for(int& i : a)
    {
        std::cout<<i<<std::endl;
    }
    return 0;
}
