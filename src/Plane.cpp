#include "Plane.h"
#include "minecrap.h"

Plane::Plane(Vec3 &p1, Vec3 &p2, Vec3 &p3) {
    setPoints(p1, p2, p3);
}

Plane::Plane() {
}

Plane::~Plane() {
}

void Plane::setPoints(Vec3 &p1, Vec3 &p2, Vec3 &p3) {
    (void)p1;
    (void)p2;
    (void)p3;

    /*
    Vec3 aux1, aux2;

    aux1 = p1 - p2;
    aux2 = p3 - p2;

    normal = aux1 * aux2;
    normal.normalize();
    point.copy(p2);
    d = -(normal.dotProduct(point));
    */
}

float Plane::distance(const Vec3 &p) {
    return (d + normal.dotProduct(p));
}

void Plane::setCoefficients(float a, float b, float c, float d) {
	// set the normal vector
	normal.set(a,b,c);
	//compute the lenght of the vector
	float l = normal.length();
	// normalize the vector
	normal.set(a/l,b/l,c/l);
	// and divide d by th length as well
	this->d = d/l;
}
