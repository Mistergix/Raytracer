#include "Plane.h";

Plane::Plane(const Vector3& position, const Vector3& normal, const Color& color) : position(position), normal(normal) {
    SetColor(color);
}
Plane::~Plane () {}

Intersection Plane::DoesIntersect(const Ray& ray){
    float dotProduct = dot(ray.direction, normal);

    if(dotProduct == 0.0f) { return Intersection(false, 0.0f); }

    float distance = dot(position - ray.origin, normal) / dotProduct;

    return Intersection(true, distance);
}
