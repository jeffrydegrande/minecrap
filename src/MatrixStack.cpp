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

PushStack::PushStack(MatrixStack &stack):
    stack(stack)
{
    stack.push();
}

PushStack::~PushStack() {
    stack.pop();
}
