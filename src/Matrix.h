#ifndef MINECRAP_MATRIX_H
#define MINECRAP_MATRIX_H

#include "Vec3.h"

class Plane;
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
        Matrix4(float v[16]);

        float & operator()(int row, int column);

        static Matrix4 Multiply(const Matrix4 &am, const Matrix4 &bm);
	    Matrix4 operator *(const Matrix4 &m);
        void multiply(const Matrix4 &);


        void loadIdentity();
        void invertPt(const Vec3 & from, Vec3 &to);
        void rotateX(float degs);
        void rotateY(float degs);
        void rotateZ(float degs);
        void inverse(const Matrix4 &m);

        float *data();
};

#endif
