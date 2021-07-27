#pragma once
#include "../Shape.h"

class Cone : public Shape{
private:
    Vector3 _center;
    float _radius;
    float _height;
public:
    Cone(const Vector3& rotation, const Vector3& center, float radius, float height, const Material& mat = Material());
    virtual ~Cone();
    virtual Intersection DoesIntersect(const Ray& ray);
    virtual Ray GetNormal(const Vector3& p, const Vector3& o)const;
};

