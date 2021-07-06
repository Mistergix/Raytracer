#pragma once
#include "Ray.h"
#include "Material.h"

struct RayHit;

struct Intersection {
    bool intersect;
    float distance;

    Intersection(bool intersect, float distance);
};

class Shape {
    public:
        Shape();
        bool Intersect(RayHit& rayHit);
        virtual Intersection DoesIntersect(const Ray& ray) = 0;
        virtual ~Shape() {}
        void SetColor(const Color& c);
        void SetMaterial(const Material& mat);
        Material GetMaterial(const Vector3& p)const;
        virtual Ray GetNormal(const Vector3& p, const Vector3& o)const = 0;
    protected :
        Color color;
        Material material;
};