#include "Frustum.h"
#include "minecrap.h"
#include "Plane.h"

#include <cmath>

Frustum::Frustum() {}
Frustum::~Frustum() {}

#define m(row,col)  m[col*4+row-5]

void Frustum::setFrustum(float *m) {
    planes[ZNEAR].setCoefficients(
                 m(3,1) + m(4,1),
                 m(3,2) + m(4,2),
                 m(3,3) + m(4,3),
                 m(3,4) + m(4,4));
    planes[ZFAR].setCoefficients(
                -m(3,1) + m(4,1),
                -m(3,2) + m(4,2),
                -m(3,3) + m(4,3),
                -m(3,4) + m(4,4));
    planes[BOTTOM].setCoefficients(
                 m(2,1) + m(4,1),
                 m(2,2) + m(4,2),
                 m(2,3) + m(4,3),
                 m(2,4) + m(4,4));
    planes[TOP].setCoefficients(
                -m(2,1) + m(4,1),
                -m(2,2) + m(4,2),
                -m(2,3) + m(4,3),
                -m(2,4) + m(4,4));
    planes[LEFT].setCoefficients(
                 m(1,1) + m(4,1),
                 m(1,2) + m(4,2),
                 m(1,3) + m(4,3),
                 m(1,4) + m(4,4));
    planes[RIGHT].setCoefficients(
                -m(1,1) + m(4,1),
                -m(1,2) + m(4,2),
                -m(1,3) + m(4,3),
                -m(1,4) + m(4,4));
}

#undef M

void Frustum::render() {

    #define POINT(p)   glVertex3f(p.x, p.y, p.z)
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLineWidth(2);
    glBegin(GL_LINES);
        // near
        POINT(ntl); POINT(ntl);
        POINT(ntr); POINT(nbr);
        POINT(nbr); POINT(nbl);
        POINT(nbl); POINT(ntl);
        // far
        POINT(ftl); POINT(ftl);
        POINT(ftr); POINT(fbr);
        POINT(fbr); POINT(fbl);
        POINT(fbl); POINT(ftl);

        // top plane
        POINT(ftl); POINT(ntl);
        POINT(ftr); POINT(ntr);
        // bottom
        POINT(fbl); POINT(nbl);
        POINT(fbr); POINT(nbr);
    glEnd();
    glLineWidth(1);
    glPopMatrix();
    #undef POINT
}


int Frustum::sphereInFrustum(Vec3 &p, float radius) {
    int result = INSIDE;
    float distance;

    for (int i=0; i< 6; i++) {
        distance = planes[i].distance(p);
        if (distance < -radius)
            return OUTSIDE;
        else if (distance < radius)
            result = INTERSECT;
    }
    return result;
}
