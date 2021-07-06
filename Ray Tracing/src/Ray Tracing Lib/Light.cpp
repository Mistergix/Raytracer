#include "Light.h"

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
    return (position - p);
}

Vector3 Light::GetVectorFromLight(const Vector3& p) const
{
    return (p - position);
}
