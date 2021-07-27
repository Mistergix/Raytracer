#pragma once
#include "../Shape.h"

class Triangle : public Shape {
public:
    Triangle(const Vector3& rotation, const Vector3& center, float size, Vector3 vertex0, Vector3 vertex1, Vector3 vertex2, const Material& mat = Material());
    virtual ~Triangle();
    virtual Intersection DoesIntersect(const Ray& ray);
    virtual Ray GetNormal(const Vector3& p, const Vector3& o)const;
    virtual Vector3 GetTextureCoordinates(const Vector3& p)const;
protected :
    Vector3 vertex0, vertex1, vertex2;
};