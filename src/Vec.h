#ifndef MINECRAP_VEC_H
#define MINECRAP_VEC_H

class Vec2 {

public:
	float x;
	float y;

	Vec2();
	Vec2(float x, float y);
    Vec2(int x, int y);

	Vec2 operator +(const Vec2 &);
	Vec2 & operator +=(const Vec2 &);
	Vec2 operator -(const Vec2 &);
	Vec2 & operator -=(const Vec2 &);

	Vec2 normalize();
	static Vec2 normalize(const Vec2 &);

	float length() const;

    void print() const;
};

class Vec3
{
	friend Vec3 operator -(const Vec3 &);

public:

	float x;
	float y;
	float z;

	Vec3();
    Vec3(const Vec3 &v);
	Vec3(float x, float y, float z);

	Vec3 & operator +=(const Vec3 &);
	Vec3 & operator -=(const Vec3 &);
	Vec3 & operator *=(float scalar);
	Vec3 & operator *=(int scalar);

	Vec3 operator +(const Vec3 &) const;
	Vec3 operator -(const Vec3 &) const;
	Vec3 operator *(float scalar) const;
	Vec3 operator *(int scalar) const;

	float dotProduct(const Vec3 &);
	static float dotProduct(const Vec3 &, const Vec3 &);

    Vec3 operator*(const Vec3 &other);
	Vec3 crossProduct(const Vec3 &);
	static Vec3 crossProduct(const Vec3 &, const Vec3 &);

    void normalize();

    void copy(const Vec3 &v);
    void set(float x, float y, float z);

	float length() const;
    float lengthSq() const;
    
    float *value_ptr();

    void print() const;
};


inline Vec3 operator-(const Vec3 &v)
{
    return Vec3(-v.x, -v.y, -v.z);
}


class Vec4
{

public:

	float x;
	float y;
	float z;
    float w;

	Vec4();
    Vec4(const Vec4 &v);
	Vec4(float x, float y, float z, float w);

	Vec4 operator +(const Vec4 &);
	Vec4 & operator +=(const Vec4 &);
	Vec4 operator -(const Vec4 &);
	Vec4 & operator -=(const Vec4 &);
	Vec4 operator *(float scalar);
	Vec4 & operator *=(float scalar);
	Vec4 operator *(int scalar);
	Vec4 & operator *=(int scalar);

	float dotProduct(const Vec4 &);
	static float dotProduct(const Vec4 &, const Vec4 &);

    Vec4 operator*(const Vec4 &other);
	Vec4 crossProduct(const Vec4 &);
	static Vec4 crossProduct(const Vec4 &, const Vec4 &);

    void normalize();

    void copy(const Vec4 &v);
    void set(float x, float y, float z, float w);

	float length() const;

    float *value_ptr();

    void print() const;
};

#endif
