#pragma once
#include "Shape.h"
#include "RayHit.h"

Intersection::Intersection(bool intersect, float distance) : intersect(intersect), distance(distance) {}

Shape::Shape()
{
}

bool Shape::Intersect(const Scene& scene, RayHit& rayHit) {

    Intersection intersection = DoesIntersect(rayHit.ray);

    if(! intersection.intersect) {return false;}

    float distance = intersection.distance;

    if(distance <= RAY_DISTANCE_MIN || distance >= rayHit.distance){
        return false;
    }

    rayHit.distance = distance;
    rayHit.shape = this;
    rayHit.color = rayHit.GetImpactColor(scene);

    return true;
}

void Shape::SetMaterial(const Material& mat)
{
    material = mat;
}

Material Shape::GetMaterial(const Vector3& p) const
{
    return material;
}
