#include "ray.h"

Ray::Ray() : origin(0.0f, 0.0f, 0.0f, 1.0f), direction(0.0f, 0.0f, 0.0f, 0.0f) { }

Ray::Ray(const Vector3& origin, const Vector3& dir, float distanceMax) : origin(origin), distanceMax(distanceMax) {
	direction = dir;
	direction.w = 0.0f;
}

Ray Ray::normalized()
{
	return Ray(origin, direction.normalized());
}

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