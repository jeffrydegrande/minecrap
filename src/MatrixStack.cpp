#include "MatrixStack.h"
#include "minecrap.h"
#include "Matrix.h"
#include "Camera.h"

#include <cmath>

MatrixStack::MatrixStack()
    :currentMatrix()
{
}

MatrixStack::MatrixStack(const Matrix4 & matrix)
    :currentMatrix(matrix)
{
}

void MatrixStack::set(const Matrix4 &matrix)
{
    currentMatrix = matrix;
}

void MatrixStack::Perspective(float fovy, float aspect, float zNear, float zFar)
{
    currentMatrix *= Camera::Perspective(fovy, aspect, zNear, zFar);
}

void MatrixStack::apply(const Matrix4 &matrix) {
    currentMatrix *= matrix;
}

void MatrixStack::push()
{
    stack.push(currentMatrix);
}

void MatrixStack::pop()
{
    currentMatrix = stack.top();
    stack.pop();
}

const Matrix4 &MatrixStack::top() const
{
    return currentMatrix;
}

void MatrixStack::translate(const Vec3 &offset) {
    currentMatrix.translate(offset);
}

void MatrixStack::scale(const Vec3 &v) {
    currentMatrix.scale(v);
}

void MatrixStack::scale(float v) {
    currentMatrix.scale(v);
}

void MatrixStack::rotateX(float degs) {
    currentMatrix.rotateX(degs);
}

void MatrixStack::rotateY(float degs) {
    currentMatrix.rotateY(degs);
}

void MatrixStack::rotateZ(float degs) {
    currentMatrix.rotateZ(degs);
}

void MatrixStack::identity()
{
    currentMatrix.identity();
}

PushStack::PushStack(MatrixStack &stack):
    stack(stack)
{
    stack.push();
}

PushStack::~PushStack() {
    stack.pop();
}
