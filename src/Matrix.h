#ifndef MINECRAP_MATRIX_H
#define MINECRAP_MATRIX_H

#include "minecrap.h"
#include "Vec.h"

class Plane;
class Matrix4 {

    // friend Vec3 operator*(const Vec3 &lhs, const Matrix4 &rhs);

    float m[16];

    public:
        Matrix4();
        Matrix4(float v[16]);

        static Matrix4 Load(GLenum matrix);
        static Matrix4 Perspective(float fovy, float ratio, float zNear, float zFar);

        float & operator()(int row, int column);

        static Matrix4 Multiply(const Matrix4 &am, const Matrix4 &bm);
	    Matrix4 operator *(const Matrix4 &m);
        Matrix4 operator *=(const Matrix4 &m);

        Vec4 operator *(const Vec4 &m);
        Vec4 operator *(Vec4 &m) const;

        const float &operator[](size_t i) const;
        float &operator[](size_t i);

        void multiply(const Matrix4 &);

        void identity();
        void invertPt(const Vec3 & from, Vec3 &to);
        void transformVector(Vec3 &to);
        void translate(float x, float y, float z);
        void translate(const Vec3 &v);

        void scale(const Vec3 &v);
        void scale(float v);

        void rotate(const Vec3 &axis, float angle);
        void rotateX(float degs);
        void rotateY(float degs);
        void rotateZ(float degs);
        void inverse(const Matrix4 &m);

        float *value_ptr();

        void print();
};

class Matrix3 {
    float m[9];

    public:
        Matrix3();
        Matrix3(float v[9]);
        Matrix3(Matrix4 &m);
        Matrix3(const Matrix4 &m);

        void identity();

        float *value_ptr();

        void print();
};

//////////////////////////////////////////////////////////////////
// Matrix4 inline operations
//////////////////////////////////////////////////////////////////

inline Vec3 operator*(const Vec3 &lhs, const Matrix4 &rhs) {
    return Vec3((lhs.x * rhs[0]) + (lhs.y * rhs[4]) + (lhs.z * rhs[8]),
                (lhs.x * rhs[1]) + (lhs.y * rhs[5]) + (lhs.z * rhs[9]),
                (lhs.x * rhs[2]) + (lhs.y * rhs[6]) + (lhs.z * rhs[10]));
}


inline const float &Matrix4::operator[](size_t i) const {
    return m[i];
}

inline float &Matrix4::operator[](size_t i) {
    return m[i];
}

#endif
