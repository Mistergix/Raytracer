#include "Plane.h"

Plane::Plane(const Vector3& position, const Vector3& normal, const Material& material) : position(position), normal(normal) {
    SetMaterial(material);
}
Plane::~Plane () {}

Intersection Plane::DoesIntersect(const Ray& ray){
    float dotProduct = dot(ray.direction, normal);

    if(dotProduct == 0.0f) { return Intersection(false, 0.0f); }

    float distance = dot(position - ray.origin, normal) / dotProduct;

    return Intersection(true, distance);
}

Ray Plane::GetNormal(const Vector3& p, const Vector3& o) const
{
    return Ray(p, normal);
}
