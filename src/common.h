//
// Created by oliver on 05/08/2025.
//

#ifndef COMMON_H
#define COMMON_H

struct Vec3 {
    float x, y, z;

    Vec3(float X, float Y, float Z);

    Vec3 operator+(Vec3 const &obj) const;

    Vec3 operator+(float a) const;

    Vec3 operator-(Vec3 const &obj) const;

    Vec3 operator-(float a) const;

    Vec3 normalise() const;

    float magnitude() const;

    Vec3 cross(Vec3 a);
};

struct Mat4 {
    float values[16];
};


#endif //COMMON_H
