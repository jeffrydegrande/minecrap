#ifndef MINECRAP_MATRIX_H
#define MINECRAP_MATRIX_H

#include "minecrap.h"
#include "Vec.h"

class Plane;
class Matrix4 {

    float m[16];


    public:
        Matrix4();
        Matrix4(float v[16]);

        static Matrix4 load(GLenum matrix);

        float & operator()(int row, int column);

        static Matrix4 Multiply(const Matrix4 &am, const Matrix4 &bm);
	    Matrix4 operator *(const Matrix4 &m);
        void multiply(const Matrix4 &);


        void loadIdentity();
        void invertPt(const Vec3 & from, Vec3 &to);
        void transformVector(Vec3 &to);
        void rotate(const float &angle, Vec3 &axis);
        void rotateX(float degs);
        void rotateY(float degs);
        void rotateZ(float degs);
        void inverse(const Matrix4 &m);

        float *value_ptr();

        void print();
};

#endif
