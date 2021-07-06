#pragma once
#include "Shape.h"
#include "Light.h"
#include <vector>

class Scene {
    public:
        Scene();
        virtual ~Scene();
        void AddShape(Shape* shape);
        void AddLight(Light* light);
        bool Intersect(RayHit& rayHit, bool useShadow);
        Color GetAmbiant() const;
        void SetAmbiant(const Color& c);
        Color GetBackground() const;
        void SetBackground(const Color& c);
        int NbLights() const;
        const Light* GetLight(int index) const;
        int NbShapes() const;
        Shape* GetShape(int index) const;
        float shadowFactor;
    protected :
        Color ambiant;
        Color background;
        std::vector<Shape*> shapes;
        std::vector<Light*> lights;
};