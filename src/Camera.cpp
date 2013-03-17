#include "Camera.h"
#include "Console.h"
#include <cmath>

Vec3 WORLD_XAXIS(1.0f, 0.0f, 0.0f);
Vec3 WORLD_YAXIS(0.0f, 1.0f, 0.0f);
Vec3 WORLD_ZAXIS(0.0f, 0.0f, 1.0f);


Camera::Camera()
{
    accumPitchDegrees = 0.0f;
    eye.set(0.0f, 0.0f, 0.0f);
    xAxis.set(1.0f, 0.0f, 0.0f);
    yAxis.set(0.0f, 1.0f, 0.0f);
    zAxis.set(0.0f, 0.0f, 1.0f);
    viewDirection.set(0.0f, 0.0f, -1.0f);
    viewMatrix.identity();
}

Camera::~Camera()
{
}

Vec3 Camera::move(float dx, float dy, float dz) const
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
    return eye;
}

Vec3 Camera::move(const Vec3 &displacement) const
{
    return move(displacement.x, displacement.y, displacement.z);
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

    return projection;
}

float Camera::getViewDirectionInDegrees() const {
    float angle = Vec2::angle( Vec2(viewDirection.x, viewDirection.z),
                               Vec2(0.0f, 1.0f));
    if (angle < 0)
        angle += 360.0f;
    if (angle > 360.0f)
        angle -= 360.0f;
    return angle;
}
