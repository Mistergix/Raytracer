#include "../Shape.h"

class Sphere : public Shape {
    public:
        Sphere(const Vector3& center, float radius, const Material& mat = Material());
        virtual ~Sphere();
        virtual Intersection DoesIntersect(const Ray& ray);
        void SetCenter(const Vector3& center);
        virtual Ray GetNormal(const Vector3& p, const Vector3& o)const;
    protected :
        Vector3 center;
	    float radius;
};