#ifndef _IMG_PROC_DFT_H_
#define _IMG_PROC_DFT_H_
#include <complex>

namespace util
{
    class GLVAR
    {
        public:
        constexpr static double PI=3.1415926;
    };
    class DFTUtil
    {
        public:
        void dft1(const std::complex<int> ins[], int len, std::complex<double> out[]);  
    };
}

#endif //_IMG_PROC_DFT_H_

