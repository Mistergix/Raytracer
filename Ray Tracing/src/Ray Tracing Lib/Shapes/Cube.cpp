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
	return Ray();
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
