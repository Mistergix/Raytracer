#pragma once
#include "../Shape.h"

class Square : public Shape {
public:
    Square(const Vector3& rotation, const Vector3& center, float size, const Material& mat = Material());
    virtual ~Square();
    virtual Intersection DoesIntersect(const Ray& ray);
    virtual Ray GetNormal(const Vector3& p, const Vector3& o)const;
};
