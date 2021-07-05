#include "Vector3.h"
#include "Ray.h"

class RayTracerCamera {
    public:
        virtual Ray CreateRay(Vector3 point) const = 0;
};

class PerspectiveCamera : public RayTracerCamera {
public:
    PerspectiveCamera(Vector3 origin, Vector3 target, Vector3 up, float fov, float ratio);
    virtual Ray CreateRay(Vector3 point) const;
    void SetPosition(Vector3 origin);
protected:
    Vector3 origin;
    Vector3 forward;
    Vector3 up;
    Vector3 right;

    float height, width;
};