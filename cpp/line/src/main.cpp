#include <iostream>
#include "pointdis.h"
#include "ft.h"

using namespace geo_distance;
int main() {
    // Point pt(150,250);
    // Line line;
    // line.p0 = Point(100, 200);
    // line.p1 = Point(150, 300);

    // PLDistance pl;
    // float dis = pl.dist_Point_to_Line(pt,line);

    // std::cout << "return," <<dis<< std::endl;  
    std::complex<int> ins[5];
    std::complex<double> out[5];
 
    for( int i=0; i<5; i++)
    {
        ins[i].real(i);
        ins[i].imag(2*i);

        out[i].real(0);
        out[i].imag(1);
    }

    util::DFTUtil df;
    df.dft1(ins,5,out);

    for( int i=0; i<5; i++)
    {      
        std::cout<<"out["<<i<<"] "<< out[i].real() <<" "<<out[i].imag()<<"i"<<std::endl;
    }

    std::cin.ignore();
}
