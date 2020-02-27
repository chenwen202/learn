#include "mathfunc.h"

namespace processing
{
void ImgProcFunc::equalization(const unsigned char* src,  int cols , int rows,unsigned char* dst)
{    
    if(eq_.size() != LEVEL ) eq_.resize(LEVEL);
    cal_equalising(src,cols,rows);
    int len = cols * rows;
    for(int i=0; i< len; i++)
        dst[i] = std::round(eq_[src[i]]);

}

void ImgProcFunc::cal_equalising(const unsigned char* src,  int cols , int rows)
{
    int len = cols * rows;
    int num =0;
    float* p = eq_.data();
    int tag[LEVEL]={0};
    int sig[LEVEL]={0};

    for(int i=0; i< len; i++)
    {       
        tag[src[i]] +=1;
    }

    for(int i=0; i< LEVEL; i++)
    {
        if( 0 == i) num =0;
        else    num = sig[i-1];
        sig[i] = num + tag[i];
        p[i] = (LEVEL-1) *(sig[i]) *1.0f/len;
     }

}

}


