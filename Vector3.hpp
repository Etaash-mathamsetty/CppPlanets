#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include <iostream>

class Vector3 {
private:
    double x, y, z;

public:
    Vector3(double x = 0, double y = 0, double z = 0);
    Vector3(const Vector3& v);

    double getX() const;
    double getY() const;
    double getZ() const;

    void setX(double x);
    void setY(double y);
    void setZ(double z);

    Vector3 negate() const;
    Vector3 div(double scalar) const;
    Vector3 times(double scalar) const;
    Vector3 add(const Vector3& v) const;
    Vector3 minus(const Vector3& v) const;

    void print() const {
        //std::cout << getX() << " " << getY() << " " << getZ() << std::endl;
    }

    double norm() const;
    double getDist(const Vector3& v) const;
    double dot(const Vector3& v) const;
};

#endif
