#ifndef MINECRAP_AABOX_H
#define MINECRAP_AABOX_H

#include "Vec.h"

class AABox {
    Vec3 corner;
    float x;
    float y;
    float z;

    public:

        AABox(Vec3 & corner, float x, float y, float z);
        AABox();
        ~AABox();

        void setBox(Vec3 &corner, float x, float y, float z);

        Vec3 getPositiveVertex(const Vec3 &normal) const;
        Vec3 getNegativeVertex(const Vec3 &normal) const;
};

#endif
