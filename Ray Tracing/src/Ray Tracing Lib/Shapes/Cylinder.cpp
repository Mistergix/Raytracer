#include "Cylinder.h"

Cylinder::Cylinder(const Vector3& rotation, const Vector3& center, float size, const Material& mat)
{
	entity.scale(size);
	entity.rotate(rotation);
	entity.translate(center);
	SetMaterial(mat);
}

Cylinder::~Cylinder()
{
}

Intersection Cylinder::DoesIntersect(const Ray& ray) {
	Ray r = entity.globalToLocal(ray).normalized();

	float a = r.direction.x * r.direction.x + r.direction.z * r.direction.z;
	float b = 2 * (r.direction.x * r.origin.x + r.direction.z * r.origin.z);
	float c = r.origin.x * r.origin.x + r.origin.z * r.origin.z - 1.0;
	float delta = b * b - 4 * a * c;

	if (delta < 0) return Intersection(false, 0.0f);

	float t;
	if (delta < 0.0001) {
		t = -b / (2 * a);
	}
	else {
		t = (-b - sqrt(delta)) / (2 * a);
		if (t < 0)t = (-b + sqrt(delta)) / (2 * a);
	}
	if (t < 0) return Intersection(false, 0.0f);

	Vector3 p = r.origin + t * r.direction;
	Vector3 impact = entity.localToGlobal(p);
	return Intersection(true, (impact - ray.origin).magnitude());
}

Ray Cylinder::GetNormal(const Vector3& p, const Vector3& o) const
{
	Vector3 lp = entity.globalToLocal(p);
	Vector3 lo = entity.globalToLocal(o);
	if ((lo - Vector3(0, lo.y, 0)).magnitude() > 1)
		return entity.localToGlobal(Ray(lp, Vector3(lp.x, 0, lp.z))).normalized();
	return entity.localToGlobal(Ray(lp, Vector3(-lp.x, 0, -lp.z))).normalized();
}