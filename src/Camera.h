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

    Vec3 acceleration;
    Vec3 currentVelocity;
    Vec3 velocity;

    float distanceTraveled;

    Matrix4 viewMatrix;

    float accumPitchDegrees;

    public:
        Camera();
        ~Camera();

        const Vec3 &getPosition() const;
        const Matrix4 &getViewMatrix() const;

        static Matrix4 Perspective(float fovy, float aspect, float zNear, float zFar);
        static Matrix4 HorizontalPerspective(float fovx, float aspect, float zNear, float zFar);

        void move(float dx, float dy, float dz);
        void move(const Vec3 &direction, const Vec3 &amount);
        void move(const Vec3 &displacement);

        void rotate(float headingDegrees, float pitchDegrees, float rollDegrees);

        void updatePosition(const Vec3 &direction, float elapsed);
        void setPosition(const Vec3 &position);

        void setAcceleration(const Vec3 &acceleration);
        void setVelocity(const Vec3 &velocity);
        void setCurrentVelocity(const Vec3 &velocity);
        void setCurrentVelocity(float x, float y, float z);
        void setViewDirection(const Vec3 &direction);

        const Vec3 &getAcceleration() const;
        const Vec3 &getVelocity() const;
        const Vec3 &getCurrentVelocity() const;
        const Vec3 &getViewDirection() const;
        const float &getDistanceTraveled() const;

    private:
        void rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees);
        void rotateFirstPerson(float headingDegrees, float pitchDegrees);

        void updateVelocity(const Vec3 &direction, float elapsed);
        void updateViewMatrix(bool orthogonalize);
};

inline const Vec3 &Camera::getPosition() const 
{ return eye; }

inline const Matrix4 &Camera::getViewMatrix() const 
{ return viewMatrix; }

inline void Camera::setAcceleration(const Vec3 &acceleration) 
{ this->acceleration = acceleration; }

inline void Camera::setVelocity(const Vec3 &velocity) 
{ this->velocity = velocity; }

inline void Camera::setCurrentVelocity(const Vec3 &velocity) 
{ this->currentVelocity = velocity; }

inline void Camera::setCurrentVelocity(float x, float y, float z)
{ this->currentVelocity.set(x, y, z); }

inline void Camera::setViewDirection(const Vec3 &direction)
{ this->viewDirection = direction; }

inline const Vec3 &Camera::getAcceleration() const 
{ return this->acceleration; }

inline const Vec3 &Camera::getVelocity() const 
{ return this->velocity; }

inline const Vec3 &Camera::getCurrentVelocity() const 
{ return this->currentVelocity; }

inline const Vec3 &Camera::getViewDirection() const
{ return this->viewDirection; }

inline const float &Camera::getDistanceTraveled() const
{ return this->distanceTraveled; }

#endif
