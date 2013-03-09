#ifndef MINECRAP_CAMERA_H
#define MINECRAP_CAMERA_H

#include "Matrix.h"
#include "Vec.h"

class Camera {
    Vec3 eye;

    Vec3 xAxis;
    Vec3 yAxis;
    Vec3 zAxis;
    Vec3 viewDirection;

    Matrix4 viewMatrix;

    float accumPitchDegrees;

    public:
        Camera();
        ~Camera();

        const Vec3 &getPosition() const;
        const Matrix4 &getViewMatrix() const;

        static Matrix4 Perspective(float fovy, float aspect, float zNear, float zFar);
        static Matrix4 HorizontalPerspective(float fovx, float aspect, float zNear, float zFar);

        Vec3 move(float dx, float dy, float dz) const;
        Vec3 move(const Vec3 &displacement) const;

        void rotate(float headingDegrees, float pitchDegrees, float rollDegrees);

        void setPosition(const Vec3 &position);

        void setViewDirection(const Vec3 &direction);

        const Vec3 &getViewDirection() const;

        float getViewDirectionInDegrees() const;

    private:
        void rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees);
        void rotateFirstPerson(float headingDegrees, float pitchDegrees);

        void updateViewMatrix(bool orthogonalize);
};

inline const Vec3 &Camera::getPosition() const 
{ return eye; }

inline const Matrix4 &Camera::getViewMatrix() const 
{ return viewMatrix; }

inline void Camera::setViewDirection(const Vec3 &direction)
{ this->viewDirection = direction; }

inline const Vec3 &Camera::getViewDirection() const
{ return this->viewDirection; }


#endif
