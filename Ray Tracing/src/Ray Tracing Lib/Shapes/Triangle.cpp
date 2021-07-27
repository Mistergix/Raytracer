#include "Triangle.h"

Triangle::Triangle(const Vector3& rotation, const Vector3& center, float size, Vector3 vertex0, Vector3 vertex1, Vector3 vertex2, const Material& mat) : vertex0(vertex0), vertex1(vertex1), vertex2(vertex2)
{
    entity.scale(size);
    entity.rotate(rotation);
    entity.translate(center);
    SetMaterial(mat);
}

Triangle::~Triangle()
{
}

Intersection Triangle::DoesIntersect(const Ray& ray)
{
    Vector3 v0 = entity.localToGlobal(vertex0);
    Vector3 v1 = entity.localToGlobal(vertex1);
    Vector3 v2 = entity.localToGlobal(vertex2);
    const float EPSILON = 0.0000001;
    Vector3 rayVector = ray.direction;
    Vector3 rayOrigin = ray.origin;
    Vector3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = v1 - v0;
    edge2 = v2 - v0;
    h = cross(rayVector, edge2);
    a = dot(edge1, h);
    if (a > -EPSILON && a < EPSILON)
        return Intersection(false, -1.0f);    // This ray is parallel to this triangle.
    f = 1.0 / a;
    s = rayOrigin - v0;
    u = f * dot(s, h);
    if (u < 0.0 || u > 1.0)
        return Intersection(false, -1.0f);
    q = cross(s, edge1);
    v = f * dot(rayVector, q);
    if (v < 0.0 || u + v > 1.0)
        return Intersection(false, -1.0f);
    // At this stage we can compute t to find out where the intersection point is on the line.
    float t = f * dot(edge2, q);
    if (t > EPSILON) // ray intersection
    {
        return Intersection(true, t);
    }
    else // This means that there is a line intersection but not a ray intersection.
        return Intersection(false, -1.0f);
}

Ray Triangle::GetNormal(const Vector3& p, const Vector3& o) const
{
    Vector3 lp = entity.globalToLocal(p);
    Vector3 lo = entity.globalToLocal(o);
    Vector3 v(0, 0, 0);
    v.z = -1.0;

    if (lo.x<1 && lo.x>-1 && lo.y<1 && lo.y>-1 && lo.z == 0)
        return entity.localToGlobal(Ray(lp, -v)).normalized();
    return entity.localToGlobal(Ray(lp, v)).normalized();
}
