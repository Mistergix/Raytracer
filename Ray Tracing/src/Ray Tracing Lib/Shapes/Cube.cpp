#include "Cube.h"

Cube::Cube(const Vector3& rotation, const Vector3& center, float size, const Material& mat) : size(size)
{
	entity.scale(size);
	entity.rotate(rotation);
	entity.translate(center);
	SetMaterial(mat);
}

Cube::~Cube()
{
}

Intersection Cube::DoesIntersect(const Ray& ray)
{
	Ray r = entity.globalToLocal(ray).normalized();

	float mint = -1;
	float offsets[] = { -1,1 };
	for (int d = 0; d < 3; d++) {
		for (int o = 0; o < 2; o++) {
			float t = interSide(r, d, offsets[o]);
			if (t >= 0 && (mint<0 || mint>t)) {
				mint = t;
			}
		}
	}

	if (mint >= 0) {
		Vector3 impact = entity.localToGlobal(r.Compute(mint));
		return Intersection(true, (impact - ray.origin).magnitude());
	}

	return Intersection(false, 0.0f);
}

Ray Cube::GetNormal(const Vector3& p, const Vector3& o) const
{
	Vector3 lp = entity.globalToLocal(p);
	Vector3 lo = entity.globalToLocal(o);
	Vector3 v(0, 0, 0);
	if (lp.x > 0.999)v.x = 1.0;
	else if (lp.x < -0.999)v.x = -1.0;
	else if (lp.y > 0.999)v.y = 1.0;
	else if (lp.y < -0.999)v.y = -1.0;
	else if (lp.z > 0.999)v.z = 1.0;
	else if (lp.z < -0.999)v.z = -1.0;
	if (lo.x<1 && lo.x>-1 && lo.y<1 && lo.y>-1 && lo.z<1 && lo.z>-1)
		return entity.localToGlobal(Ray(lp, -v)).normalized();
	return entity.localToGlobal(Ray(lp, v)).normalized();
}

float Cube::interSide(const Ray& r, int dim, float offset) const
{
	float t = -1;
	if (r.direction.GetByDim(dim)<0.00001 && r.direction.GetByDim(dim) >-0.00001)return -1;
	t = (offset - r.origin.GetByDim(dim)) / r.direction.GetByDim(dim);
	if (t < 0)return -1;
	for (int d = 0; d < 3; d++) {
		if (d == dim)continue;
		float x = r.origin.GetByDim(d) + t * r.direction.GetByDim(d);
		if (x < -1 || x > 1)return -1;
	}
	return t;
}

Vector3 Cube::GetTextureCoordinates(const Vector3& p) const
{
	Vector3 lp = entity.globalToLocal(p);
	if (lp.x > 0.999 || lp.x < -0.999)return Vector3(lp.z / 2 + 0.5, lp.y / 2 + 0.5, 0);
	if (lp.y > 0.999 || lp.y < -0.999)return Vector3(lp.x / 2 + 0.5, lp.z / 2 + 0.5, 0);
	if (lp.z > 0.999 || lp.z < -0.999)return Vector3(lp.x / 2 + 0.5, lp.y / 2 + 0.5, 0);
	return Vector3(0, 0, 0);
}
