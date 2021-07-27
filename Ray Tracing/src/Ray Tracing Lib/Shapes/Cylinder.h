#pragma once
#include "../Shape.h"

class Cylinder : public Shape {
public:
	Cylinder(const Vector3& rotation, const Vector3& center, float size, const Material& mat = Material());
	virtual ~Cylinder();
	virtual Intersection DoesIntersect(const Ray& ray);
	virtual Ray GetNormal(const Vector3& p, const Vector3& o)const;
};

