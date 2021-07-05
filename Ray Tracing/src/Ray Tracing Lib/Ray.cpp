#include "ray.h"

Ray::Ray() : origin(0.0f, 0.0f, 0.0f), direction() { }

Ray::Ray(const Vector3& origin, const Vector3& direction, float distanceMax) : origin(origin), direction(direction), distanceMax(distanceMax) {}

Ray::Ray(const Ray& other) : origin(other.origin), direction(other.direction), distanceMax(other.distanceMax) {}

Ray& Ray::operator =(const Ray& other)
{
	origin = other.origin;
	direction = other.direction;
	distanceMax = other.distanceMax;
	return *this;
}

Vector3 Ray::Compute(float distance) const
{
	return origin + direction * distance;
}