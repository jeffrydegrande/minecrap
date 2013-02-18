#ifndef MINECRAP_MATRIX_H
#define MINECRAP_MATRIX_H

#include "Vec3.h"

class Matrix4 {

    float _11;
    float _12;
    float _13;
    float _14;
    float _21;
    float _22;
    float _23;
    float _24;
    float _31;
    float _32;
    float _33;
    float _34;
    float _41;
    float _42;
    float _43;
    float _44;

    public:
        Matrix4();

        void loadIdentity();
        void multiply(const Matrix4 &);
        void invertPt(const Vec3 & from, Vec3 &to);
        void rotateX(float degs);
        void rotateY(float degs);
        void rotateZ(float degs);

};

#endif
