#pragma once
#include "Ray.h"
#include "RayHit.h"
#include "Vector3.h"
#include "Material.h"

struct Intersection {
    bool intersect;
    float distance;

    Intersection(bool intersect, float distance);
};

class Shape {
    public:
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