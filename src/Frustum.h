#ifndef MINECRAP_FRUSTUM_H
#define MINECRAP_FRUSTUM_H

#include "Vec.h"
#include "Plane.h"

class Frustum {
    enum {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGHT,
        ZNEAR,
        ZFAR
    };

    Vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
    float angle, ratio, znear, zfar, tang;
    float nearWidth, nearHeight, farWidth, farHeight;

    Plane planes[6];

    public:

        enum {OUTSIDE, INTERSECT, INSIDE};

        Frustum();
        ~Frustum();

        void setFrustum(float *m);

        void render();

        int sphereInFrustum(Vec3 &p, float radius);
};

#endif
