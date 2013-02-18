#include "minecrap.h"
#include "Matrix.h"
#include <cmath>

Matrix4::Matrix4()
{
    loadIdentity();
}

void Matrix4::loadIdentity() {
    _11 = _22 = _33 = _44 = 1.0f;
    _12 = _13 = _14 = 0.0f;
    _21 = _23 = _24 = 0.0f;
    _31 = _32 = _34 = 0.0f;
    _41 = _42 = _43 = 0.0f;
}

void Matrix4::invertPt(const Vec3 &from, Vec3 &to) {
    float x = from.x - _41;
    float y = from.y - _42;
    float z = from.z - _43;

    to.x = _11 * x + _12 * y + _13 * z;
    to.y = _21 * x + _22 * y + _23 * z;
    to.z = _31 * x + _32 * y + _33 * z;
}

void Matrix4::rotateX(float degs) {
    Matrix4 rot;
    float rads = degs * DEGREES_TO_RADIANS;
    rot._22 = cos(rads);
    rot._23 = sin(rads);
    rot._32 = -rot._23; // -sin
    rot._33 = rot._22;  // cos
    multiply(rot);
}

void Matrix4::rotateY(float degs) {
    Matrix4 rot;
    float rads = degs * DEGREES_TO_RADIANS;
    rot._11 = cos(rads);
    rot._13 = -sin(rads);
    rot._31 = -rot._13; // sin
    rot._33 = rot._11;  // cos
    multiply(rot);
}

void Matrix4::rotateZ(float degs) {
    Matrix4 rot;
    float rads = degs * DEGREES_TO_RADIANS;
    rot._11 = cos(rads);
    rot._12 = sin(rads);
    rot._21 = -rot._12; // -sin
    rot._22 = rot._11;  // cos
    multiply(rot);
}

void Matrix4::multiply(const Matrix4 &m) {
    float *pA = (float *)&_11;
    float *pB = (float *)&m._11;

    float pM[16];

    memset(pM, 0, sizeof(Matrix4));
    for (unsigned short i=0; i<4; i++)
        for (unsigned short j=0; j<4; j++)
            for (unsigned short k=0; k<4; k++)
                pM[4*i+j] += pA[4*i+k] * pB[4*k+j];

    memcpy(&_11, pM, sizeof(Matrix4));
}
