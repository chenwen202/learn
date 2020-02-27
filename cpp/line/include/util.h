#ifndef _MATRIX_UTIL_H_
#define _MATRIX_UTIL_H_

namespace geo_distance
{
class Vector
{
public:
    int x;
    int y;
    int z;
    Vector &operator=(const Vector &other);
    Vector operator-(const Vector &other) const;    
};
struct Point
{
    int x;
    int y;
    int z;
    Point() { set_default(); }
    Point(int xv,int yv, int zv=0) { x = xv; y = yv; z = zv; }
    void set_default()
    {
        x = 0;
        y = 0;
        z = 0;
    }
    Vector operator+(const Point &point) const;
    Vector operator-(const Point &point) const;
};
struct Line
{
    Point p0;
    Point p1;
};

struct Segment
{
    Point p0;
    Point p1;
};


} // namespace geo_distantce

#endif // _MATRIX_UTIL_H_