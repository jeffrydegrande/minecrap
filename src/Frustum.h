#ifndef MINECRAP_FRUSTUM_H
#define MINECRAP_FRUSTUM_H

#include "Vec3.h"
#include "Plane.h"

class Frustum {

    enum {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGHT,
        NEAR,
        FAR
    };

    Vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
    float angle, ratio, near, far, tang;
    float nearWidth, nearHeight, farWidth, farHeight;

    Plane planes[6];

    public:

        enum {OUTSIDE, INTERSECT, INSIDE};

        Frustum();
        ~Frustum();

        void updatePerspective(
                float angle, float ratio, float near, float far);

        void updateCamera(Vec3 &eye, Vec3 &center, Vec3 &up);

        void render();

        int sphereInFrustum(Vec3 &p, float radius);
};

#endif
