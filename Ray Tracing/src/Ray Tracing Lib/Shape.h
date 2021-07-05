#pragma once
#include "Ray.h"
#include "RayHit.h"
#include "Vector3.h"

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
    protected :
        Color color;
};