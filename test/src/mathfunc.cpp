#include "mathfunc.h"

float MathFunc::cal_sin(float x)
{
    float v1=x;
    bool v3=false;
    float r=x;
    int Total=50;

    for( int i=3; i<=Total; i+=2)
    {       
        v1 = v1 * x*x/((i-1) *i);
        if( v3) r += v1;
        else r -= v1;
        v3 = !v3;
    }

    return r;
}
