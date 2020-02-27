#include "pointdis.h"
#include <math.h>

namespace geo_distance
{

template <class T>
inline double dot(const T& u, const T& v)
{   
    return  1.0*((u).x * (v).x + (u).y * (v).y + (u).z * (v).z);
}

template <class T>
inline double norm(const T& v)
{
    return sqrt(dot(v,v) * 1.0);
}

template <class T>
inline double d(const T& u, const T& v)
{
    return norm(u-v);
}

 template<class T>
inline  Vector operator*(const Vector& v, const T& f )  
{
    Vector r;
    r.x = f * v.x;
    r.y = f * v.y;
    r.z = f * v.z; 
    return r;
}

int PLDistance::closest2D_Point_to_Line( Point P[], int n, Line L)
{
    return 0;
}

//formula: |w-(w.u)u|
float PLDistance::dist_Point_to_Line( Point P, Line L)
{
    Vector v = L.p1 - L.p0;
    Vector w = P - L.p0;    
    double b =  dot(w,v)   / (norm(v) * norm(v)); 
    return d(w,v*b);
}

float PLDistance::dist_Point_to_Segment( Point P, Segment S)
{
   return 0;
}


}