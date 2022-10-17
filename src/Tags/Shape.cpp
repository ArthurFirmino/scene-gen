#include "Shape.hpp"

#include <cstddef>
#include <memory>
#include <Eigen/Geometry>
#include <filesystem>

namespace fs = std::filesystem;

extern fs::path rootDir;
extern bool sSceneFirstShape;

Schema ShapeSchema = 
{
    .baseName = "shape",
    .attributes = 
    {
        { Type::Transform, "transform", GETTER(Shape, transform) },
        { Type::Tag, "bsdf", GETTER(Shape, bsdf) },
        { Type::Tag, "emitter", GETTER(Shape, emitter) }
    }
};

void Shape::Generate(RNG &rng)
{
    transform = Eigen::Affine3f(Eigen::Translation3f(
        rng.NextFloat(-10.f, 10.f), 
        rng.NextFloat(-10.f, 10.f), 
        rng.NextFloat(0.f, 10.f)
    )).matrix();

    transform *= Eigen::Affine3f(Eigen::AngleAxisf(
        rng.NextFloat(-3.14159, 3.14159),
        Eigen::Vector3f(
            rng.NextFloat(), rng.NextFloat(), rng.NextFloat()
        ).normalized()
    )).matrix();

    bool isCylinder = dynamic_cast<Cylinder*>(this) != nullptr;
    std::vector<Variant> exclude = { Variant::Dielectric, Variant::ThinDielectric, Variant::RoughDielectric };

    // if (!sSceneFirstShape && !isCylinder && rng.NextFloat() < 0.3f)
    //    bsdf = std::dynamic_pointer_cast<Tag>(Procedural::Generate(
    //         Procedural::Variant::BumpBSDF, rng));
    // else
    {
        bsdf = std::dynamic_pointer_cast<Tag>(Procedural::Generate(
                Procedural::Variant::BSDF, rng, isCylinder ? exclude : std::vector<Variant>()));
        if (sSceneFirstShape || rng.NextFloat() < 0.4f)
            emitter = std::dynamic_pointer_cast<Tag>(Procedural::Generate(
                    Procedural::Variant::AreaEmitter, rng));
    }
}

Schema SphereSchema = 
{
    .parent = &ShapeSchema,
    .typeName = "sphere",
    .attributes = 
    {
        { Type::Float, "radius", GETTER(Sphere, radius) },
        { Type::Point, "center", GETTER(Sphere, center) },
    }
};

void Sphere::Generate(RNG &rng)
{ 
    Shape::Generate(rng);

    radius = rng.NextFloat(1.f, 5.f); // [1, 5]
    center = Point(0.f, 0.f, 0.f);
}

Schema CylinderSchema = 
{
    .parent = &ShapeSchema,
    .typeName = "cylinder",
    .attributes = 
    {
        { Type::Point, "p0", GETTER(Cylinder, p0) },
        { Type::Point, "p1", GETTER(Cylinder, p1) },
        { Type::Float, "radius", GETTER(Cylinder, radius) }
    }
};

void Cylinder::Generate(RNG &rng)
{
    Shape::Generate(rng);

    p0 = p1 = Point(0.f, 0.f, 0.f);
    p1.z() = rng.NextFloat(1.f, 5.f);
    radius = rng.NextFloat(1.f, 5.f);
}

Schema ObjMeshSchema = 
{
    .parent = &ShapeSchema,
    .typeName = "ply",
    .attributes = 
    {
        { Type::String, "filename", GETTER(ObjMesh, filename) }
    }
};

void ObjMesh::Generate(RNG &rng)
{
    Shape::Generate(rng);

    auto objects_directory = rootDir;
    objects_directory += "/resources/objects";
    static std::vector<std::string> objects;
    if (objects.empty())
    {
        for (const auto& object : 
            fs::directory_iterator(objects_directory))
        {
            objects.push_back(object.path().u8string());
        }
    }
    if (!objects.empty())
    {
        int index = rng.NextInt() % objects.size();
        filename = objects[index];
    }

    if (filename.find("bunny.ply") != std::string::npos)
    {
        transform *= Eigen::Affine3f(Eigen::Scaling(3.f)).matrix();
    }
    else if (filename.find("dragon.ply") != std::string::npos)
    {
        transform *= Eigen::Affine3f(Eigen::Scaling(3.f)).matrix();
    }
    else if (filename.find("indonesian.ply") != std::string::npos)
    {
        transform *= Eigen::Affine3f(Eigen::Scaling(0.05f)).matrix();
    }
    else if (filename.find("serapis.ply") != std::string::npos)
    {
        transform *= Eigen::Affine3f(Eigen::Scaling(0.05f)).matrix();
    }
}
