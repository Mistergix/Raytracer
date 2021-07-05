#include "RayTracerCamera.h"
#include <cmath>

PerspectiveCamera::PerspectiveCamera(Vector3 origin, Vector3 target, Vector3 up2, float fov, float ratio) : origin(origin) {
    forward = (target - origin).normalized();
    right = cross(forward, up2).normalized();
    up = cross(right, forward).normalized();

    height = tan(fov);
    width = height * ratio;
}

Ray PerspectiveCamera::MakeRay(Vector3 point) const {
    Vector3 direction = forward + point.x * width * right + point.y * height * up;
    return Ray(origin, direction.normalized());
}