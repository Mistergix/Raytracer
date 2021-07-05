#include "../Shape.h";

class Sphere : public Shape {
    public:
        Sphere(const Vector3& center, float radius, const Color& color = Color());
        virtual ~Sphere();
        virtual Intersection DoesIntersect(const Ray& ray);
        void SetCenter(const Vector3& center);
    protected :
        Vector3 center;
	    float radius;
};