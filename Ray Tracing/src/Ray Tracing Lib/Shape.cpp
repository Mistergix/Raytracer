#pragma once
#include "Shape.h"

Intersection::Intersection(bool intersect, float distance) : intersect(intersect), distance(distance) {}

bool Shape::Intersect(RayHit& rayHit) {

    Intersection intersection = DoesIntersect(rayHit.ray);

    if(! intersection.intersect) {return false;}

    float distance = intersection.distance;

    if(distance <= RAY_DISTANCE_MIN || distance >= rayHit.ray.distanceMax){
        return false;
    }

    rayHit.distance = distance;
    rayHit.shape = this;
    rayHit.color = color;

    return true;
}

void Shape::SetColor(const Color& c){
    color = c;
}