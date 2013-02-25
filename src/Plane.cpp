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
    Vec3 aux1, aux2;

    aux1 = p1 - p2;
    aux2 = p3 - p2;

    normal = aux1 * aux2;
    normal.normalize();
    point.copy(p2);
    d = -(normal.dotProduct(point));
}

float Plane::distance(const Vec3 &p) {
    return (d + normal.dotProduct(p));
}