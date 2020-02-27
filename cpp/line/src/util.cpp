#include "util.h"

namespace geo_distance 
{
    Vector& Vector::operator=(const Vector& other)
    {
            if (this != &other)
            {
                this->x = other.x;
                this->y = other.y;
                this->z = other.z;
            }
            return *this;
    }

    Vector Vector::operator-(const Vector &other) const
    {
        Vector v;
        v.x = this->x - other.x;
        v.y = this->y - other.y;
        v.z = this->z - other.z;
        return v;
    }
  

    Vector Point::operator+(const Point& point) const
    {
        Vector v;
        v.x = this->x + point.x;
        v.y = this->y + point.y;
        v.z = this->z + point.z;
        return v;
    }

    Vector Point::operator-(const Point &point) const
    {
        Vector v;
        v.x = this->x - point.x;
        v.y = this->y - point.y;
        v.z = this->z - point.z;
        return v;
    }

}