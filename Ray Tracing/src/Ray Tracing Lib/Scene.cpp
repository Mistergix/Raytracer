#include "Scene.h"

Scene::Scene() { }
Scene::~Scene() { }

void Scene::AddShape(Shape* shape){ 
    shape->index = NbShapes();
    shapes.push_back(shape); 
}

void Scene::AddLight(Light* light)
{
    lights.push_back(light);
}

bool Scene::Intersect(RayHit& rayHit, bool useShadow){
    bool doesIntersect = false;
    for(auto iterator = shapes.begin(); iterator != shapes.end(); ++iterator){
        Shape *shape = *iterator;
        if(shape-> Intersect(*this, rayHit, useShadow)){ doesIntersect = true; }
    }

    return doesIntersect;
}

Color Scene::GetAmbiant() const
{
    return ambiant;
}

void Scene::SetAmbiant(const Color& c)
{
    ambiant = c;
}

Color Scene::GetBackground() const
{
    return background;
}

void Scene::SetBackground(const Color& c)
{
    background = c;
}

int Scene::NbLights() const
{
    return lights.size();
}

const Light* Scene::GetLight(int index) const
{
    return lights[index];
}

int Scene::NbShapes() const
{
    return shapes.size();
}

 Shape* Scene::GetShape(int index)  const
{
    return shapes[index];
}
