#ifndef MINECRAP_MATRIX_STACK_H
#define MINECRAP_MATRIX_STACK_H

#include "Matrix.h"
#include <stack>
#include <vector>

class MatrixStack {
    public:
        void Perspective(float fovy, float aspect, float zNear, float zFar);

        MatrixStack();
        MatrixStack(const Matrix4 &matrix);

        void push();
        void pop();
        const Matrix4 &top() const;

        void apply(const Matrix4 &matrix);
        void set(const Matrix4 &matrix);

        void translate(const Vec3 &offset);
        void scale(const Vec3 &v);
        void scale(float v);

        void rotateX(float degs);
        void rotateY(float degs);
        void rotateZ(float degs);


        void identity();

    private:

        std::stack<Matrix4, std::vector<Matrix4> > stack;
        Matrix4 currentMatrix;

};

class PushStack {
    public:
        PushStack(MatrixStack &stack);
        ~PushStack();

    private:
        PushStack(const PushStack&);
        PushStack &operator=(const PushStack &);
        MatrixStack &stack;
};

#endif
