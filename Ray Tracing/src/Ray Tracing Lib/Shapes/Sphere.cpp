#include "Sphere.h";

Sphere::Sphere(const Vector3& rotation, const Vector3& center, float radius, const Material& mat) : center(center){
    entity.scale(radius);
    entity.rotate(rotation);
    entity.translate(center);
    SetMaterial(mat);
}

Sphere::~Sphere() { }

Intersection Sphere::DoesIntersect(const Ray& ray){

    Ray r = entity.globalToLocal(ray).normalized();
    float a = r.direction.magnitudeSquared();
    float b = 2 * dot(r.direction, r.origin);
    float c = r.origin.magnitudeSquared() - 1.0;
    float delta = b * b - 4 * a * c;

    if (delta < 0) { return Intersection(false, 0.0f); }

    float t;
    if (delta < 0.0001) {
        t = -b / (2 * a);
    }
    else {
        t = (-b - sqrt(delta)) / (2 * a);
        if (t < 0)t = (-b + sqrt(delta)) / (2 * a);
    }
    if (t < 0) { return Intersection(false, 0.0f); }

    Vector3 p = r.Compute(t);
    Vector3 impact = entity.localToGlobal(p);
    return Intersection(true, (ray.origin - impact).magnitude());
}

Ray Sphere::GetNormal(const Vector3& p, const Vector3& o) const
{
    Vector3 lp = entity.globalToLocal(p);
    Vector3 lo = entity.globalToLocal(o);
    if ((lo - Vector3::Zero()).magnitude() < 1)return entity.localToGlobal(Ray(lp, -lp)).normalized();
    return entity.localToGlobal(Ray(lp, lp)).normalized();
}

Vector3 Sphere::GetTextureCoordinates(const Vector3& p) const
{
    Vector3 Sn = (p - center).normalized();
    Vector3 Sp = entity.localToGlobal(Vector3::Up());
    Vector3 Se = entity.localToGlobal(Vector3::Forward());

    float phi = std::acos(-dot(Sn, Sp));
    float v = phi / M_PI;
    float u = 0;
    if(v < 0.0001 || v > 0.999){
        return Vector3(0, v, 0);
    }

    float theta = std::acos((dot(Se, Sn) / std::sin(phi))) / (2 * M_PI);

    if (dot(cross(Sp, Se), Sn) > 0) {
        u = theta;
    }
    else
    {
        u = 1 - theta;
    }

    return Vector3(u, v, 0);
    /*
    Vector3 lp = entity.globalToLocal(p);
    float rho = std::sqrt(dot(lp, lp));
    float theta = std::atan2(lp.y, lp.x);
    float sigma = std::acos(lp.z / rho);
    float x = -theta / (2 * M_PI) + 0.5;
    float y = sigma / M_PI;
    //std::cerr<<x<<","<<y<<std::endl;   		 	  			  	 		 
    return Vector3(x, y, 0);*/
}
