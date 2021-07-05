#pragma once
#include "Vector3.h"

#define RAY_DISTANCE_MIN 0.0001f
#define RAY_DISTANCE_MAX 1.0e30f

struct Ray
{
	Vector3 origin;
	Vector3 direction;
	float distanceMax;

	Ray();
	Ray(const Ray& other);
	Ray(const Vector3& origin, const Vector3& direction, float distanceMax = RAY_DISTANCE_MAX);

	Vector3 Compute(float distance) const;

	Ray& operator = (const Ray& other);
};