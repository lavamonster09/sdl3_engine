//
// Created by oliver on 05/08/2025.
//
#include "common.h"
#include <cmath>

Vec3::Vec3(const float X, const float Y, const float Z) {
    x = X;
    y = Y;
    z = Z;
}

Vec3 Vec3::operator+(Vec3 const &obj) const {
    return Vec3{
        x + obj.x,
        y + obj.y,
        z + obj.z
    };
}

Vec3 Vec3::operator+(float const a) const {
    return Vec3{
        x + a,
        y + a,
        z + a
    };
}

Vec3 Vec3::operator-(Vec3 const &obj) const {
    return Vec3{
        x - obj.x,
        y - obj.y,
        z - obj.z
    };
}

Vec3 Vec3::operator-(float const a) const {
    return Vec3{
        x - a,
        y - a,
        z - a
    };
}

float Vec3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}


Vec3 Vec3::normalise() const {
    return Vec3{
        x / this->magnitude(),
        y / this->magnitude(),
        z / this->magnitude()
    };
}

Vec3 Vec3::cross(Vec3 a) {
    return Vec3{
        (y * a.z) - (a.y * z),
        -((x * a.z) - (a.x * z)),
        (x * a.y) - (a.x * y)
    };
}
