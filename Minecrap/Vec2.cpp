#include "Vec2.h"

#include <math.h>

Vec2::Vec2(): x(0), y(0)
{
}

Vec2::Vec2(float x, float y): x(x), y(y)
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