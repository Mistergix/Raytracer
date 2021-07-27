#include "Square.h"

Square::Square(const Vector3& rotation, const Vector3& center, float size, const Material& mat)
{
	entity.scale(size);
	entity.rotate(rotation);
	entity.translate(center);
	SetMaterial(mat);
}

Square::~Square()
{
}

Intersection Square::DoesIntersect(const Ray& ray)
{
    Ray lr = entity.globalToLocal(ray).normalized();
    if (lr.direction.z<0.0001 && lr.direction.z>-0.0001)return Intersection(false, 0);
    if (lr.direction.z * lr.origin.z > 0)return Intersection(false, 0);
    float t = -lr.origin.z / lr.direction.z;
    Vector3 p = lr.Compute(t);
    if (p.x < -1 || p.x>1 || p.y < -1 || p.y>1)return Intersection(false, 0);
    Vector3 impact = entity.localToGlobal(p);
    return Intersection(true, (impact - ray.origin).magnitude());
}

Ray Square::GetNormal(const Vector3& p, const Vector3& o) const
{
	Vector3 lp = entity.globalToLocal(p);
	Vector3 lo = entity.globalToLocal(o);
	Vector3 v(0, 0, 0);
	v.z = -1.0;

	if (lo.x<1 && lo.x>-1 && lo.y<1 && lo.y>-1 && lo.z == 0)
		return entity.localToGlobal(Ray(lp, -v)).normalized();
	return entity.localToGlobal(Ray(lp, v)).normalized();
}

Vector3 Square::GetTextureCoordinates(const Vector3& p) const
{
	Vector3 lp = entity.globalToLocal(p);
	return Vector3(lp.x / 2 + 0.5, lp.y / 2 + 0.5, 0.0);
}
