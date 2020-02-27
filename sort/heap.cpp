#include "sort.h"

using namespace Util;

#define SWAP(a,b)  {a^=b;b^=a;a^=b;}

HeapSort::HeapSort(HeapType eType)
{
    this->mHeapType = eType;
}

int HeapSort::ShiftDown(int array[], int start, int end)
{
    int result = 0;
    int root = start;
    while (GlobalFunc::iLeftChild(root) <= end)
    {
        int child = GlobalFunc::iLeftChild(root);
        int swap = root;
        if(child <= end && array[swap] < array[child])
            swap = child;
        if(child+1<=end && array[swap] < array[child+1])
            swap = child +1;
        
        if(swap == root)
            return result;
        else
        {
            SWAP(array[root],array[swap]);
            root = swap;
        }
    }

    return result;
}

int HeapSort::ShiftUp(int array[], int start, int end)
{
    int result = 0;

    int child = end;
    int parent = GlobalFunc::iParent(end);
    while (parent > start)
    {
        parent = GlobalFunc::iParent(child);
        if( array[parent] < array[child])
        {
            SWAP(array[parent], array[child]);
            child = parent;
        }
        else
            return result;

    }
    return result;
}



int HeapSort::Heapify(int array[],int count)
{
    int result = 0;
    if( this->mHeapType == HeapType::SHIFT_DOWN)
    {
        int start = GlobalFunc::iParent(count -1);
        while (start >= 0)
        {
            ShiftDown(array,start,count -1);
            start = start -1;
        }
    }

    if( this->mHeapType == HeapType::SHIFT_UP)
    {
        int end = 1;
        while (end < count)
        {
            ShiftUp(array,0,end);
            end = end + 1;
        }
    }

    return result;
}

int HeapSort::SortFunc(int array[], int count)
{
    int result = 0;
    if( this->mHeapType == SHIFT_DOWN)  
    {
        Heapify(array,count);
        int end = count -1;
        while (end > 0)
        {
            SWAP(array[end],array[0]);
            end = end -1;      
            ShiftDown(array,0,end);
        }
    }    
    if( this->mHeapType == SHIFT_UP)
    {
        int end = 0;
        while (end < count )
        {                       
            ShiftUp(array,0,end);    
            end = end +1;                
        }
    }
   

    return result;
}