#include "Scene.hpp"

#include "Shape.hpp"

Schema SceneSchema = 
{
    .baseName = "scene",
    .attributes = 
    {
        { Type::TagVector, "children", GETTER(Scene, children) }
    }
};

bool sSceneFirstShape = false;

void Scene::Generate(RNG &rng)
{
    int numShapes = 5 + rng.NextInt() % 10; // [3, 12]
    for (int i = 0; i < numShapes; ++i)
    {
        sSceneFirstShape = i == 0;
        ProceduralPtr p = Procedural::Generate(
            Procedural::Variant::Shape, rng);
        children.push_back(std::dynamic_pointer_cast<Tag>(p));
    }
//    // Environment map
//    {
//        ProceduralPtr p = Procedural::Generate(
//            Procedural::Variant::EnvironmentEmitter, rng);
//        children.push_back(std::dynamic_pointer_cast<Tag>(p));
//    }
}