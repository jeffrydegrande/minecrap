#include "AABox.h"

AABox::AABox()
{
    corner.x = 0.0f;
    corner.y = 0.0f;
    corner.z = 0.0f;
    this->x = x;
    this->y = y;
    this->z = z;
}

AABox::AABox(Vec3 &corner, float x, float y, float z)
{
    setBox(corner, x, y, z);
}

AABox::~AABox() {
}

void AABox::setBox(Vec3 &corner, float x, float y, float z) 
{
    this->corner.copy(corner);

    if (x < 0.0) {
        x = -x;
        this->corner.x -= x;
    }

    if (y < 0.0) {
        y = -y;
        this->corner.y -= y;
    }

    if (z < 0.0) {
        z = -z;
        this->corner.z -= z;
    }

    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3 AABox::getPositiveVertex(const Vec3 &normal) const {
    Vec3 res = corner;

    if (normal.x > 0)
        res.x += x;

    if (normal.y > 0)
        res.y += y;

    if (normal.z > 0)
        res.z += z;

    return res;
}

Vec3 AABox::getNegativeVertex(const Vec3 &normal) const {
    Vec3 res;
    if (normal.x < 0)
        res.x += x;

    if (normal.y < 0)
        res.y += y;

    if (normal.z < 0)
        res.z += z;

    return res;
}
