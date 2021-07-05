#include "RayTracerCamera.h"
#include <cmath>

PerspectiveCamera::PerspectiveCamera(Vector3 origin, Vector3 target, Vector3 up2, float fov, float ratio) : origin(origin) {
    forward = (target - origin).normalized();
    right = cross(up2, forward).normalized();
    up = cross(right, forward).normalized();

    forward.Print();
    right.Print();
    up.Print();
    
    height = tan(fov);
    width = height * ratio;

    std::cout << "Width and height " << width << " " << height << std::endl;
}

Ray PerspectiveCamera::CreateRay(Vector3 point) const {
    Vector3 x = (point.x * width) * right;
    Vector3 y = (point.y * up) * height;
    Vector3 direction = forward + x;
    direction += y;
    direction = direction.normalized();

   // std::cout << "Dir is ";
    //direction.Print();
    return Ray(origin, direction);
}

void PerspectiveCamera::SetPosition(Vector3 pos)
{
    origin = pos;
}
