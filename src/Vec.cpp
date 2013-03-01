#include "Vec.h"
#include <math.h>

Vec2::Vec2(): x(0), y(0)
{
}

Vec2::Vec2(float x, float y): x(x), y(y)
{
}

Vec2::Vec2(int x, int y): x((float)x), y((float)y)
{
}

Vec2 Vec2::operator+(const Vec2 &vector) {
	return Vec2(x + vector.x, y + vector.y);
}

Vec2 & Vec2::operator +=(const Vec2 &vector) {
	x += vector.x;
	y += vector.y;
	return *this;
}

Vec2 Vec2::operator -(const Vec2 &vector) {
	return Vec2(x - vector.x, y - vector.y);
}

Vec2 & Vec2::operator -=(const Vec2 &vector) {
	x -= vector.x;
	y -= vector.y;
	return *this;
}



float Vec2::length() const {
	return sqrt(x*x + y*y);
}


Vec2 Vec2::normalize() {
	return Vec2::normalize(*this);
}

Vec2 Vec2::normalize(const Vec2 &v) {
	float length = v.length();
	Vec2 result;
	result.x = v.x / length;
	result.y = v.y / length;
	return result;
}


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

void Vec3::set(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
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


Vec4::Vec4(): x(0), y(0), z(0), w(0)
{
}

Vec4::Vec4(float x, float y, float z, float w): x(x), y(y), z(z), w(w)
{
}

Vec4::Vec4(const Vec4 &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

void Vec4::set(float x, float y, float z, float w) {
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vec4 Vec4::operator+(const Vec4 &v) {
	return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

Vec4 & Vec4::operator +=(const Vec4 &v) {
	x += v.x;
	y += v.y;
	z += v.z;
    w += v.w;

	return *this;
}

Vec4 Vec4::operator -(const Vec4 & v) {
    Vec4 res;
    res.x = x - v.x;
    res.y = y - v.y;
    res.z = z - v.z;
    res.w = w - v.w;
	return res;
}

Vec4 & Vec4::operator -=(const Vec4 &v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
    w -= v.w;

	return *this;
}

Vec4 Vec4::operator *(float scalar) {
	return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vec4 & Vec4::operator *=(float scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
    w *= scalar;
	return *this;
}

Vec4 Vec4::operator *(int scalar) {
	return Vec4(x * scalar, y * scalar, z * scalar, w * scalar);
}

Vec4 & Vec4::operator *=(int scalar) {
	x *= scalar;
	y *= scalar;
	z *= scalar;
    w *= scalar;
	return *this;
}


float Vec4::length() const {
	return sqrt(x*x + y*y + z*z + w*w);
}

float Vec4::dotProduct(const Vec4 &other) {
	return Vec4::dotProduct(*this, other);
}

float Vec4::dotProduct(const Vec4 &v1, const Vec4 &v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

Vec4 Vec4::operator*(const Vec4 &other) {
    return Vec4::crossProduct(*this, other);
}

Vec4 Vec4::crossProduct(const Vec4 &other) {
	return Vec4::crossProduct(*this, other);
}

Vec4 Vec4::crossProduct(const Vec4 &v1, const Vec4 &v2) {
	// UxV = (UyVz - UzVy, UzVx - UxVz, UxVy - UyVx)
	Vec4 result;
	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;
    result.w = v1.w;

	return result;
}

void Vec4::normalize() {
    float l = this->length();
    this->x = x / l;
    this->y = y / l;
    this->z = z / l;
    this->w = w / l;
}

void Vec4::copy(const Vec4 &v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}
