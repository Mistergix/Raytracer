#include "RayHit.h"

RayHit::RayHit() : ray(), distance(RAY_DISTANCE_MAX), shape(NULL) { }

RayHit::RayHit(const RayHit& other) : ray(other.ray), distance(other.distance), shape(other.shape) { }

RayHit::RayHit(const Ray& ray) : ray(ray), distance(ray.distanceMax), shape(NULL) { }

RayHit& RayHit::operator =(const RayHit& other)
{
	ray = other.ray;
	distance = other.distance;
	shape = other.shape;
	return *this;
}

Vector3 RayHit::HitPoint() const
{
	return ray.Compute(distance);
}

bool RayHit::Hit() const
{
	return (shape != NULL);
}

Vector3 RayHit::Direction() const {
	return ray.direction;
}

Vector3 RayHit::Origin() const {
	return ray.origin;
}

