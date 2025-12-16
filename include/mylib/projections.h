#ifndef PROJECTIONS_H
    #define PROJECTIONS_H
#endif

#include <cmath>
#include <SFML/Graphics.hpp>

// Carteasian vector.
struct Vec3 {
    float x, y, z;
};

// Define vector addition.
inline Vec3 operator+(const Vec3& a, const Vec3& b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

namespace projections{

    //Euler-angle orthographic projection
    inline sf::Vector2f eulerAngle(const Vec3& p, const Vec3 angle, float zoom){

        //Rotation about x
        float cosX = std::cos(angle.x);
        float sinX = std::sin(angle.x);

        float y1 =  cosX * p.y - sinX * p.z;
        float z1 =  sinX * p.y + cosX * p.z;

        //Rotation about y
        float cosY = std::cos(angle.y);
        float sinY = std::sin(angle.y);

        float x2 =  cosY * p.x + sinY * z1;
        float z2 = -sinY * p.x + cosY * z1;

        //Rotation about z
        float cosZ = std::cos(angle.z);
        float sinZ = std::sin(angle.z);

        float x3 = cosZ * x2 - sinZ * y1;
        float y3 = sinZ * x2 + cosZ * y1;

        //Scale into pixels & project ignoring depth.
        return {x3 * 200.0f * zoom + 400.0f, y3 * 200.0f * zoom + 300.0f};
    }
}

