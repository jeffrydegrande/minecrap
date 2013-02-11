#ifndef MINECRAP_VEC3_H
#define MINECRAP_VEC3_H

class Vec3
{

public:

	float x;
	float y;
	float z;

	Vec3();
	Vec3(float x, float y, float z);

	Vec3 operator +(const Vec3 &);
	Vec3 & operator +=(const Vec3 &);
	Vec3 operator -(const Vec3 &);
	Vec3 & operator -=(const Vec3 &);
	Vec3 operator *(float scalar);
	Vec3 & operator *=(float scalar);
	Vec3 operator *(int scalar);
	Vec3 & operator *=(int scalar);

	float dotProduct(const Vec3 &);
	static float dotProduct(const Vec3 &, const Vec3 &);

	
	Vec3 crossProduct(const Vec3 &);
	static Vec3 crossProduct(const Vec3 &, const Vec3 &);

	Vec3 normalize();
	static Vec3 normalize(const Vec3 &);

	float length() const;
};

#endif
