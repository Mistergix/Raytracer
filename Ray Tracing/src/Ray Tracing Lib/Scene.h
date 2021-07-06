#include "Shape.h"
#include "Light.h"
#include <vector>

class Scene {
    public:
        Scene();
        virtual ~Scene();
        void AddShape(Shape* shape);
        void AddLight(Light* light);
        bool Intersect(RayHit& rayHit);
        Color GetAmbiant() const;
        void SetAmbiant(const Color& c);
        int NbLights() const;
        const Light* GetLight(int index) const;
    protected :
        Color ambiant;
        std::vector<Shape*> shapes;
        std::vector<Light*> lights;
};