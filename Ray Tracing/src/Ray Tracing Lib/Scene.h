#include "Shape.h"
#include <vector>

class Scene {
    public:
        Scene();
        virtual ~Scene();
        void AddShape(Shape* shape);
        bool Intersect(RayHit& rayHit);
    protected :
        std::vector<Shape*> shapes;
};