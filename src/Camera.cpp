#include "Camera.h"
#include <cmath>

Vec3 WORLD_XAXIS(1.0f, 0.0f, 0.0f);
Vec3 WORLD_YAXIS(0.0f, 1.0f, 0.0f);
Vec3 WORLD_ZAXIS(0.0f, 0.0f, 1.0f);

Camera::Camera()
{
    eye.set(0.0f, 0.0f, 0.0f);
    xAxis.set(1.0f, 0.0f, 0.0f);
    yAxis.set(0.0f, 1.0f, 0.0f);
    zAxis.set(0.0f, 0.0f, 1.0f);
    viewDirection.set(0.0f, 0.0f, -1.0f);
    accumPitchDegrees = 0.0f;
    currentVelocity.set(0.0f, 0.0f, 0.0f);
    velocity.set(0.0f, 0.0f, 0.0f);
    acceleration.set(0.0f, 0.0f, 0.0f);
    viewMatrix.identity();
}

Camera::~Camera()
{
}

void Camera::move(float dx, float dy, float dz)
{
    Vec3 eye = this->eye;
    Vec3 forward;

    if(CVarUtils::GetCVar<bool>("flying")) {
        forward = viewDirection;
    } else {
        forward = Vec3::crossProduct(WORLD_YAXIS, xAxis);
        forward.normalize();
    }

    eye += xAxis * dx;
    eye += WORLD_YAXIS * dy;
    eye += forward * dz;

    setPosition(eye);
}

void Camera::move(const Vec3 &displacement)
{
    move(displacement.x, displacement.y, displacement.z);
}

void Camera::move(const Vec3 &direction, const Vec3 &amount)
{
    eye.x += direction.x * amount.x;
    eye.y += direction.y * amount.y;
    eye.z += direction.z * amount.z;

    updateViewMatrix(false);
}

void Camera::setPosition(const Vec3 &position)
{
    eye = position;
    updateViewMatrix(false);
}

void Camera::rotate(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    if(CVarUtils::GetCVar<bool>("flying")) {
        rotateFlight(headingDegrees, pitchDegrees, rollDegrees);
    } else {
        rotateFirstPerson(headingDegrees, pitchDegrees);
    }
    updateViewMatrix(true);
}

void Camera::rotateFlight(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    Matrix4 rotation;
    // rotate around Y axis
    if (headingDegrees != 0.0f) {
        rotation.rotate(yAxis, headingDegrees);
        xAxis = xAxis * rotation;
        zAxis = zAxis * rotation;
    }

    // rotate around X axis
    if (pitchDegrees != 0.0f) {
        rotation.rotate(xAxis, pitchDegrees);
        yAxis = yAxis * rotation;
        zAxis = zAxis * rotation;
    }

    // rotate around Z axis
    if (rollDegrees != 0.0f) {
        rotation.rotate(zAxis, rollDegrees);
        xAxis = xAxis * rotation;
        yAxis = yAxis * rotation;
    }
}

void Camera::rotateFirstPerson(float headingDegrees, float pitchDegrees)
{
    accumPitchDegrees += pitchDegrees;

    if (accumPitchDegrees > 90.0f) {
        pitchDegrees = 90.0f - (accumPitchDegrees - pitchDegrees);
        accumPitchDegrees = 90.0f;
    }
    if (accumPitchDegrees < -90.0f) {
        pitchDegrees = -90.0f - (accumPitchDegrees - pitchDegrees);
        accumPitchDegrees = -90.0f;
    }

    Matrix4 rotation;

    // rotate around Y axis
    if (headingDegrees != 0.0f) {
        rotation.rotate(WORLD_YAXIS, headingDegrees);
        xAxis = xAxis * rotation;
        zAxis = zAxis * rotation;
    }

    // rotate around X axis
    if (pitchDegrees != 0.0f) {
        rotation.rotate(xAxis, pitchDegrees);
        yAxis = yAxis * rotation;
        zAxis = zAxis * rotation;
    }
}

void Camera::updateViewMatrix(bool orthogonalize)
{
    if (orthogonalize) {
        zAxis.normalize();
        yAxis = Vec3::crossProduct(zAxis, xAxis);
        yAxis.normalize();

        xAxis = Vec3::crossProduct(yAxis, zAxis);
        xAxis.normalize();

        viewDirection = -zAxis;
    }

	viewMatrix[0]  = xAxis.x;
	viewMatrix[1]  = yAxis.x;
	viewMatrix[2]  = zAxis.x;
	viewMatrix[3]  = 0;

	viewMatrix[4]  = xAxis.y;
	viewMatrix[5]  = yAxis.y;
	viewMatrix[6]  = zAxis.y;
	viewMatrix[7]  = 0;

	viewMatrix[8]  = xAxis.z;
	viewMatrix[9]  = yAxis.z;
	viewMatrix[10] = zAxis.z;
	viewMatrix[11] = 0;

	viewMatrix[12] = -xAxis.dotProduct(eye);
	viewMatrix[13] = -yAxis.dotProduct(eye);
	viewMatrix[14] = -zAxis.dotProduct(eye);
	viewMatrix[15] = 1.0f;
}

Matrix4 Camera::Perspective(float fovy, float aspect, float zNear, float zFar) {
    if (aspect > 1.0f)
        fovy /= aspect;

    float depth = zFar - zNear;
    float f     = 1.0f/tan(fovy*PI/360);

    Matrix4 M;
    M[0]  = f / aspect;
    M[5]  = f;
    M[10] = -(zFar + zNear) / depth;
    M[14] = -2 * (zFar*zNear) / depth;
    M[11] = -1.0f;

    return M;
}

Matrix4 Camera::HorizontalPerspective(float fovx, float aspect, float zNear, float zFar)
{
    // Construct a projection matrix based on the horizontal field of view
    // 'fovx' rather than the more traditional vertical field of view 'fovy'.

    float e = 1.0f / tanf(fovx * DEGREES_TO_RADIANS / 2.0f);
    float aspectInv = 1.0f / aspect;
    float fovy = 2.0f * atanf(aspectInv / e);
    float xScale = 1.0f / tanf(0.5f * fovy);
    float yScale = xScale / aspectInv;

    Matrix4 projection;

    projection[0] = xScale;
    projection[1] = 0.0f;
    projection[2] = 0.0f;
    projection[3] = 0.0f;

    projection[4] = 0.0f;
    projection[5] = yScale;
    projection[6] = 0.0f;
    projection[7] = 0.0f;

    projection[8] = 0.0f;
    projection[9] = 0.0f;
    projection[10] = (zFar + zNear) / (zNear - zFar);
    projection[11] = (2.0f * zFar * zNear) / (zNear - zFar);

    projection[12] = 0.0f;
    projection[13] = 0.0f;
    projection[14] = -1.0f;
    projection[15] = 0.0f;

    /*
    m_fovx = fovx;
    m_aspectRatio = aspect;
    m_znear = znear;
    m_zfar = zfar;
    */

    return projection;
}

#define EPSILON 1e-6f;

bool closeEnough(float a, float b) {
    return fabsf((a - b) / ((b == 0.0f) ? 1.0f : b)) < EPSILON;
}

void Camera::updatePosition(const Vec3 &direction, float elapsed)
{
    if (currentVelocity.lengthSq() != 0.0f) {
        Vec3 displacement = (currentVelocity * elapsed) +
            (acceleration * 0.5f * elapsed * elapsed);

        if (direction.x == 0.0f && closeEnough(currentVelocity.x, 0.0f))
            displacement.x = 0.0f;

        if (direction.y == 0.0f && closeEnough(currentVelocity.y, 0.0f))
            displacement.y = 0.0f;

        if (direction.z == 0.0f && closeEnough(currentVelocity.z, 0.0f))
            displacement.z = 0.0f;

        move(displacement.x, displacement.y, displacement.z);
    }
    updateVelocity(direction, elapsed);
}

void Camera::updateVelocity(const Vec3 &direction, float elapsed)
{
    // Updates the camera's velocity based on the supplied movement direction
    // and the elapsed time (since this method was last called). The movement
    // direction is in the range [-1,1].

    if (direction.x != 0.0f)
    {
        // Camera is moving along the x axis.
        // Linearly accelerate up to the camera's max speed.

        currentVelocity.x += direction.x * acceleration.x * elapsed;

        if (currentVelocity.x > velocity.x)
            currentVelocity.x = velocity.x;
        else if (currentVelocity.x < -velocity.x)
            currentVelocity.x = -velocity.x;
    }
    else
    {
        // Camera is no longer moving along the x axis.
        // Linearly decelerate back to stationary state.

        if (currentVelocity.x > 0.0f)
        {
            if ((currentVelocity.x -= acceleration.x * elapsed) < 0.0f)
                currentVelocity.x = 0.0f;
        }
        else
        {
            if ((currentVelocity.x += acceleration.x * elapsed) > 0.0f)
                currentVelocity.x = 0.0f;
        }
    }

    if (direction.y != 0.0f)
    {
        // Camera is moving along the y axis.
        // Linearly accelerate up to the camera's max speed.

        currentVelocity.y += direction.y * acceleration.y * elapsed;

        if (currentVelocity.y > velocity.y)
            currentVelocity.y = velocity.y;
        else if (currentVelocity.y < -velocity.y)
            currentVelocity.y = -velocity.y;
    }
    else
    {
        // Camera is no longer moving along the y axis.
        // Linearly decelerate back to stationary state.

        if (currentVelocity.y > 0.0f)
        {
            if ((currentVelocity.y -= acceleration.y * elapsed) < 0.0f)
                currentVelocity.y = 0.0f;
        }
        else
        {
            if ((currentVelocity.y += acceleration.y * elapsed) > 0.0f)
                currentVelocity.y = 0.0f;
        }
    }

    if (direction.z != 0.0f)
    {
        // Camera is moving along the z axis.
        // Linearly accelerate up to the camera's max speed.

        currentVelocity.z += direction.z * acceleration.z * elapsed;

        if (currentVelocity.z > velocity.z)
            currentVelocity.z = velocity.z;
        else if (currentVelocity.z < -velocity.z)
            currentVelocity.z = -velocity.z;
    }
    else
    {
        // Camera is no longer moving along the z axis.
        // Linearly decelerate back to stationary state.

        if (currentVelocity.z > 0.0f)
        {
            if ((currentVelocity.z -= acceleration.z * elapsed) < 0.0f)
                currentVelocity.z = 0.0f;
        }
        else
        {
            if ((currentVelocity.z += acceleration.z * elapsed) > 0.0f)
                currentVelocity.z = 0.0f;
        }
    }
}
