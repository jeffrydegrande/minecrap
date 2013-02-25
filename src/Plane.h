#ifndef MINECRAP_PLANE_H
#define MINECRAP_PLANE_H

#include "Vec3.h"

class Plane {
    Vec3 normal, point;
    float d;

    public:
        Plane(Vec3 &p1, Vec3 &p2, Vec3 &p3);
        Plane();
        ~Plane();

        void setPoints(Vec3 &p1, Vec3 &p2, Vec3 &p3);
        void render();

        void setCoefficients(float a, float b, float c, float d);
        float distance(const Vec3 &p);

};
#endif
