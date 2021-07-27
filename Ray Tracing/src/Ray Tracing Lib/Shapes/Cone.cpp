#include "Cone.h"

Cone::Cone(const Vector3& rotation, const Vector3& center, float radius, float height, const Material& mat) {
    _center = center;
    _radius = radius;
    _height = height;
    entity.scale(radius);
    entity.rotate(rotation);
    entity.translate(center);
    SetMaterial(mat);
}

Cone::~Cone() {}

Intersection Cone::DoesIntersect(const Ray& ray) {
    Vector3 pos = ray.origin;
    Vector3 dir = ray.direction;
    float A = pos.x - _center.x;
    float B = pos.z - _center.z;
    float D = _height - pos.y + _center.y;

    float tan = (_radius / _height) * (_radius / _height);

    float a = (dir.x * dir.x) + (dir.z * dir.z) - (tan * (dir.y * dir.y));
    float b = (2 * A * dir.x) + (2 * B * dir.z) + (2 * tan * D * dir.y);
    float c = (A * A) + (B * B) - (tan * (D * D));

    float delta = b * b - 4 * (a * c);
    if (fabs(delta) < 0.001) return Intersection(false, 0.0f);
    if (delta < 0.0) return Intersection(false, 0.0f);

    float t1 = (-b - sqrt(delta)) / (2 * a);
    float t2 = (-b + sqrt(delta)) / (2 * a);
    float t;

    if (t1 > t2) t = t2;
    else t = t1;

    float r = pos.y + t * dir.y;

    //if ((r > _center.y) and (r < _center.y + _height)) return t;
    Vector3 p = pos + t * dir;
    Vector3 impact = entity.localToGlobal(p);
    if ((r > _center.y) and (r < _center.y + _height)) return Intersection(true, (impact - ray.origin).magnitude());
    else return Intersection(false, 0.0f);
}

Ray Cone::GetNormal(const Vector3& p, const Vector3& o)const {

    /*float r = sqrt((p.x - _center.x) * (p.x - _center.x) + (p.z - _center.z) * (p.z - _center.z));
    Vector3 n = Vector3(p.x - _center.x, r * (_radius / _height), p.z - _center.z);
    Ray ray = Ray(p, n).normalized(); //On tente des trucs
    return ray;*/
    Vector3 lp = entity.globalToLocal(p);
    Vector3 lo = entity.globalToLocal(o);
    if ((lo - Vector3(0, lo.y, 0)).magnitude() > 1)
        return entity.localToGlobal(Ray(lp, Vector3(lp.x, 0, lp.z))).normalized();
    return entity.localToGlobal(Ray(lp, Vector3(-lp.x, 0, -lp.z))).normalized();
}