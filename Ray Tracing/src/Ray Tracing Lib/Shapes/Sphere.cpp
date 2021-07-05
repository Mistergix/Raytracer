#include "Sphere.h";

Sphere::Sphere(const Vector3& center, float radius, const Color& color) : center(center), radius(radius) {
    SetColor(color);
}

Sphere::~Sphere() { }

Intersection Sphere::DoesIntersect(const Ray& ray){
    Ray localRay = ray;
    localRay.origin -= center;

    float a = localRay.direction.magnitudeSquared();
    float b = 2 * dot(localRay.direction, localRay.origin);
    float c = localRay.origin.magnitudeSquared() - square(radius);

    float delta = square(b) - 4 * a * c;

    if(delta < 0.0f){return Intersection(false, 0.0f);}


    float sqrDelta = std::sqrt(delta);
    float distance1 = (-b - sqrDelta) / (2 * a);
    float distance2 = (-b + sqrDelta) / (2 * a);

    if(distance1 > RAY_DISTANCE_MIN && distance1 < ray.distanceMax) { return Intersection(true, distance1); }
    else if (distance2 > RAY_DISTANCE_MIN && distance2 < ray.distanceMax) { return Intersection(true, distance2); }
    
    
    return Intersection(false, 0.0f);
}