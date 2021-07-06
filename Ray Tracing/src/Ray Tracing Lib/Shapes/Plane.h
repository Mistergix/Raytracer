#pragma once
#include "../Shape.h"

class Plane : public Shape {
    public:
        Plane(const Vector3& point, const Vector3& normal, const Material& mat = Material());
        virtual ~Plane();
        virtual Intersection DoesIntersect(const Ray& ray);
        virtual Ray GetNormal(const Vector3& p, const Vector3& o)const;
    protected :
        Vector3 position;
        Vector3 normal;
};