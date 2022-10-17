#pragma once

#include "Procedural.hpp"
#include "Tag.hpp"
#include "Types.hpp"

extern Schema ShapeSchema;

struct Shape : public Procedural 
{
    Transform transform;

    TagPtr bsdf;
    TagPtr emitter;

    virtual void Generate(RNG &rng) override;
};

extern Schema SphereSchema;

struct Sphere : public Shape, SchemaWrapper<SphereSchema>
{
    Point center;
    Float radius;

    virtual void Generate(RNG &rng) override final;
};

extern Schema CylinderSchema;

struct Cylinder : public Shape, SchemaWrapper<CylinderSchema>
{
    Point p0, p1;
    Float radius;

    virtual void Generate(RNG &rng) override final;
};

extern Schema ObjMeshSchema;

struct ObjMesh : public Shape, SchemaWrapper<ObjMeshSchema>
{
    std::string filename;

    virtual void Generate(RNG &rng) override final;
};