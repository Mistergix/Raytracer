#pragma once
#include "../Shape.h"

class Sphere : public Shape {
    public:
        Sphere(const Vector3& rotation, const Vector3& center, float radius, const Material& mat = Material());
        virtual ~Sphere();
        virtual Intersection DoesIntersect(const Ray& ray);
        virtual Ray GetNormal(const Vector3& p, const Vector3& o)const;
        virtual Vector3 GetTextureCoordinates(const Vector3& p)const;
};