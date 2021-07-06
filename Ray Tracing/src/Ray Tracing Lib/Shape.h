#pragma once
#include "Ray.h"
#include "Material.h"

struct RayHit;
class Scene;

struct Intersection {
    bool intersect;
    float distance;

    Intersection(bool intersect, float distance);
};

class Shape {
    public:
        Shape();
        bool Intersect(const Scene& scene, RayHit& rayHit);
        virtual Intersection DoesIntersect(const Ray& ray) = 0;
        virtual ~Shape() {}
        void SetMaterial(const Material& mat);
        Material GetMaterial(const Vector3& p)const;
        virtual Ray GetNormal(const Vector3& p, const Vector3& o)const = 0;
    protected :
        Material material;
};