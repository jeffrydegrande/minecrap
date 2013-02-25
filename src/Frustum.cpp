#include "Frustum.h"
#include "minecrap.h"

#include <cmath>

Frustum::Frustum() {}
Frustum::~Frustum() {}

#define POINT(p)   glVertex3f(p.x, p.y, p.z)

void Frustum::updatePerspective(
        float angle, float ratio, float near, float far)
{
    this->ratio = ratio;
    this->angle = angle;
    this->near  = near;
    this->far   = far;

    tang = (float)tan(angle * DEGREES_TO_RADIANS * 0.5);

    nearHeight = near * tang;
    nearWidth  = nearHeight * ratio;
    farHeight  = far * tang;
    farWidth   = farHeight * ratio;
}

void Frustum::updateCamera(Vec3 &eye, Vec3 &center, Vec3 &up)
{
    Vec3 dir, nc, fc, X, Y, Z;

    Z = eye - center;
    Z.normalize();

    X = up * Z;
    X.normalize();

    Y = Z * X;

    nc = (eye - Z) * this->near;
    fc = (eye - Z) * this->far;

    ntl = nc + Y * nearHeight - X * nearWidth;
    ntr = nc + Y * nearHeight + X * nearWidth;
    nbl = nc - Y * nearHeight - X * nearWidth;
    nbr = nc + Y * nearHeight + X * nearWidth;

    ftl = fc + Y * farHeight - X * farWidth;
    ftr = fc + Y * farHeight + X * farWidth;
    fbl = fc - Y * farHeight - X * farWidth;
    fbr = fc + Y * farHeight + X * farWidth;

    planes[TOP].setPoints(ntr, ntl, ftl);
    planes[BOTTOM].setPoints(nbl, nbr, fbr);
    planes[LEFT].setPoints(ntl, nbl, fbl);
    planes[RIGHT].setPoints(nbr, ntr, fbr);
    planes[NEAR].setPoints(ntl, ntr, nbr);
    planes[FAR].setPoints(ftr, ftl, fbl);
}


void Frustum::render() {
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
