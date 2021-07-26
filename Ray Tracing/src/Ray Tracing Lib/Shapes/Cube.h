#pragma once
#include "../Shape.h"

class Cube : public Shape {
public:
    Cube(const Vector3& center, float size, const Material& mat = Material());
    virtual ~Cube();
    virtual Intersection DoesIntersect(const Ray& ray);
    virtual Ray GetNormal(const Vector3& p, const Vector3& o)const;
    float interSide(const Ray& r, int dim, float offset)const;
protected:
    float size;
};
