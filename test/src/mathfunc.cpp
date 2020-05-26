#include "mathfunc.h"
#include <cmath>
#include <cstring>

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
//e.g. "123", then arr[]={3,2,1};
int toIntegerArr(const char* szValue, int arr[])
{
    const char* p = szValue;
    int size = std::strlen(szValue);
    int i =0;
    for(i = size; i > 0; --i)
    {
        arr[size - i] = p[i -1] - '0';
    }   
    return size;
}

char* toStr(int arr[], int len)
{
    int l = 0;
    int i = 0;
    char *p;

    //cal valid length of arr
    for(i=len -1; i >0; --i) 
        if(arr[i] != 0)
        {
            l = i;
            break;
        }
    
    p = new char[l + 1];
    p[l] = '\0';
    for( i =l; i >= 0; --i)
        p[l-i] = arr[i] + '0';
    return p;
}
// i.e.
//  1234567
//X     234
//-------------
//  4938268
char* MathFunc::bigval_multiply(const char* szValue1, const char* szValue2)
{
    const int NUM = 100;
    int arr1[NUM] = {0};
    int arr2[NUM] = {0};
    int rs1[ NUM +1] = {0};
    int rs[NUM * NUM] = {0};
    int i=0, j=0, len1 =0, len2 =0;

    len1 = toIntegerArr(szValue1, arr1);
    len2 = toIntegerArr(szValue2, arr2);

    for(j=0; j < len2; ++j)
    {
        for(i =0; i < len1; ++i)
        {
            rs1[i] += arr1[i] * arr2[j];
            while( rs1[i] >= 10)
            {
                rs1[i] -= 10;
                rs1[i+1] += 1;
            }            
        }

        //calculate the sum        
        for(i=0; i < len1 +1; ++i)
        {
            rs[i +j] = rs[i +j] + rs1[i];
            if( rs[i +j] >= 10)
            {
                rs[i +j] -= 10;
                rs[i +j+1] += 1;
            }
            rs1[i] =0;
        }
    }

    return toStr(rs, NUM * NUM);
}


std::vector<std::vector<double>> MathFunc::getModel(const int& n, const double& sigma)
{
    double pi = 3.1415926;
    double weight = 1/ ( 2 * pi * sigma * sigma);

    std::vector<std::vector<double>> model(n,std::vector<double>(n,0.0));
    for( int i =0; i < n; ++i)
        for( int j =0; j < n; ++j)
        {
            model[i][j] = weight * std::exp( -((i - n/2)*(i - n/2) + (j - n/2)*(j - n/2))/ (2 * sigma * sigma) );
        }
    return model;
}