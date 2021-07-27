#include "Light.h"

Light::Light() : position(Vector3()), id(Color(255,255,255)), is(Color(255,255,255))
{
}

Light::Light(const Vector3& pos, const Color& id, const Color& is) : position(pos), id(id), is(is)
{
}

Ray Light::GetRayToLight(const Vector3& p) const
{
    Vector3 dir = GetVectorToLight(p);
    return Ray(p, dir);
}

Ray Light::GetRayFromLight(const Vector3& p) const
{
    Vector3 dir = GetVectorFromLight(p);
    return Ray(position, dir);
}

Vector3 Light::GetVectorToLight(const Vector3& p) const
{
    Vector3 dir = (position - p);
    dir.w = 0.0f;
    return dir.normalized();
}

Vector3 Light::GetVectorFromLight(const Vector3& p) const
{
    Vector3 dir = (p - position);
    dir.w = 0.0f;
    return dir.normalized();
}
