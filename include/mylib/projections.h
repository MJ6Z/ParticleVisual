#ifndef PROJECTIONS_H
    #define PROJECTIONS_H
#endif

#include<cmath>
#include<vector>
#include<iostream>
#include<SFML/Graphics.hpp>

struct Vec3 { float x, y, z; };

namespace projections {
    //3D → 2D projection using Euler-angle rotations followed by an orthographic projection
    inline sf::Vector2f eulerAngle(const Vec3 &p, const Vec3 angle, float zoom) {
        float cosX = std::cos(angle.x), sinX = std::sin(angle.x);
        float y1 = cosX * p.y - sinX * p.z;
        float z1 = sinX * p.y + cosX * p.z;

        float cosY = std::cos(angle.y), sinY = std::sin(angle.y);
        float x2 = cosY * p.x + sinY * z1;
        float z2 = -sinY * p.x + cosY * z1;

        float cosZ = std::cos(angle.z), sinZ = std::sin(angle.z);
        float x3 = cosZ * x2 - sinZ * y1;
        float y3 = sinZ * x2 + cosZ * y1;

        return sf::Vector2f(x3 * 200 * zoom + 400, y3 * 200 * zoom + 300);
    }
}
