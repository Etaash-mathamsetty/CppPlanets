#include "Vector3.hpp"

Vector3::Vector3(double x, double y, double z) : x(x), y(y), z(z) {}

Vector3::Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}

double Vector3::getX() const { return x; }
double Vector3::getY() const { return y; }
double Vector3::getZ() const { return z; }

void Vector3::setX(double x) { this->x = x; }
void Vector3::setY(double y) { this->y = y; }
void Vector3::setZ(double z) { this->z = z; }

Vector3 Vector3::negate() const { return Vector3(-x, -y, -z); }

Vector3 Vector3::div(double scalar) const {
    return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3 Vector3::times(double scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::add(const Vector3& v) const {
    return Vector3(x + v.x, y + v.y, z + v.z);
}

Vector3 Vector3::minus(const Vector3& v) const {
    return Vector3(x - v.x, y - v.y, z - v.z);
}

double Vector3::norm() const {
    return std::sqrt(x * x + y * y + z * z);
}

double Vector3::getDist(const Vector3& v) const {
    return minus(v).norm();
}

double Vector3::dot(const Vector3& v) const {
    return x * v.x + y * v.y + z * v.z;
}
