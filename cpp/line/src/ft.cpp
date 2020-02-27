#include "ft.h"
#include <math.h>

namespace util
{
    //dft formular sigma n{0,N-1} sigma m{0,M-1} f[m,n] * e^ (-j*2T*(m*i/N,n*j?M))
    void DFTUtil::dft1(const std::complex<int> ins[], int len, std::complex<double> out[] )
    {
        double w= 2* GLVAR::PI/len;
        for ( int i=0; i < len; i++)
        {
            double real =0;
            double imag =0;
            for( int j=0; j< len; j++)
            {
                double theta= w * i *j;   
                real += ins[j].real() * cos(theta)  + ins[j].imag() * sin(theta);
                imag += ins[j].imag() * cos(theta)  - ins[j].real() * sin(theta);
            
            }
            out[i].real(real);
            out[i].imag(imag);
        }
            
    }
}