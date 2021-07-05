#include "Scene.h"

Scene::Scene() { }
Scene::~Scene() { }

void Scene::AddShape(Shape* shape){ shapes.push_back(shape); }

bool Scene::Intersect(RayHit& rayHit){
    bool doesIntersect = false;
    for(auto iterator = shapes.begin(); iterator != shapes.end(); ++iterator){
        Shape *shape = *iterator;
        if(shape-> Intersect(rayHit)){ doesIntersect = true; }
    }

    return doesIntersect;
}