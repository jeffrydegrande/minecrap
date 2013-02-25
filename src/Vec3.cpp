#include "Vec3.h"

#include <math.h>

Vec3::Vec3(): x(0), y(0), z(0)
{
}

Vec3::Vec3(float x, float y, float z): x(x), y(y), z(z)
{
}

Vec3::Vec3(const Vec3 &v) {
    x = v.x;
    y = v.y;
    z = v.z;
}

Vec3 Vec3::operator+(const Vec3 &vector) {
	return Vec3(x + vector.x, y + vector.y, z + vector.z);
}

Vec3 & Vec3::operator +=(const Vec3 &vector) {
	x += vector.x;
	y += vector.y;
	z += vector.z;

	return *this;
}

Vec3 Vec3::operator -(const Vec3 &vector) {
    Vec3 res;
    res.x = x - vector.x;
    res.y = y - vector.y;
    res.z = z - vector.z;
	return res;
}

Vec3 & Vec3::operator -=(const Vec3 &vector) {
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;

	return *this;
}

Vec3 Vec3::operator *(float scalar) {
	return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 & Vec3::operator *=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vec3 Vec3::operator *(int scalar) {
	return Vec3(x * scalar, y * scalar, z * scalar);
}

Vec3 & Vec3::operator *=(int scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}


float Vec3::length() const {
	return sqrt(x*x + y*y + z*z);
}

float Vec3::dotProduct(const Vec3 &other) {
	return Vec3::dotProduct(*this, other);
}

float Vec3::dotProduct(const Vec3 &v1, const Vec3 &v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 Vec3::operator*(const Vec3 &other) {
    return Vec3::crossProduct(*this, other);
}

Vec3 Vec3::crossProduct(const Vec3 &other) {
	return Vec3::crossProduct(*this, other);
}

Vec3 Vec3::crossProduct(const Vec3 &v1, const Vec3 &v2) {
	// UxV = (UyVz - UzVy, UzVx - UxVz, UxVy - UyVx)
	Vec3 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
	return result;
}

void Vec3::normalize() {
    float l = this->length();
    this->x = x / l;
    this->y = y / l;
    this->z = z / l;
}

void Vec3::copy(const Vec3 &v) {
    x = v.x;
    y = v.y;
    z = v.z;
}