#ifndef MINECRAP_VEC2_H
#define MINECRAP_VEC2_H

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
};

#endif