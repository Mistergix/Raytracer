#pragma once
#include "Ray.h"
#include "Color.h"

class Light {
	public:
	Light(const Vector3& pos, const Color& id, const Color& is);
	Ray GetRayToLight(const Vector3& p)const;
	Ray GetRayFromLight(const Vector3& p)const;
	Vector3 GetVectorToLight(const Vector3& p)const;
	Vector3 GetVectorFromLight(const Vector3& p)const;
	Color id;
	Color is;
	Vector3 position;
};