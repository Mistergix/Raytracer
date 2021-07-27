#pragma once
#include "Vector3.h"
#include "Ray.h"
#include "Matrix.h"
# define M_PI 3.14159265358979323846
class Entity
{
protected:
    Matrix trans;
    Matrix transInv;
public:
    Entity();

    void translate(Vector3 transV);
    void scale(float f);
    void rotateX(float deg); // Rotation sur l'axe X
    void rotateY(float deg); // Rotation sur l'axe Y
    void rotateZ(float deg); // Rotation sur l'axe Z
    void rotate(Vector3 deg); // Rotation sur tous les axes

    Vector3 localToGlobal(const Vector3& v) const;
    Vector3 globalToLocal(const Vector3& v) const;
    Ray localToGlobal(const Ray& r) const;
    Ray globalToLocal(const Ray& r) const;

    void Print() const;

};

