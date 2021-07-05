#pragma once
#include "Ray.h"
#include "color.h"

#define NULL 0

class Shape;

struct RayHit
{
	Ray ray;
	float distance;
	Shape *shape;
	Color color;

	RayHit();
	RayHit(const RayHit& other);
	RayHit(const Ray& ray);

    bool Hit() const;
	Vector3 HitPoint() const;
	Vector3 Direction() const;
	Vector3 Origin() const;

	RayHit& operator = (const RayHit& other);
};