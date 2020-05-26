#ifndef _MATH_FUNC_
#define _MATH_FUNC_

#include <vector>

class MathFunc {
public:
    MathFunc() {}
    ~MathFunc(){}
public:
    float cal_sin(float x);
    char* bigval_multiply(const char* szValue1, const char* szValue2);    
    std::vector<std::vector<double>> getModel(const int& n, const double& sigma); 
};

#endif