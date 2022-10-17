#pragma once

#include "Types.hpp"
#include <cstdint>
#include <memory>
#include <vector>

class RNG
{
    int seed;
public:
    RNG(int seed = 0);

    float NextFloat(float min = 0.f, float max = 1.f);
    int NextInt();
};

class Procedural;
using ProceduralPtr = std::shared_ptr<Procedural>;

class Procedural
{
public:
    enum class Variant
    {
        Shape,
            Sphere,
//            ObjMesh,
            Cylinder,
        BSDF,
            Dielectric,
            Diffuse,
            ThinDielectric,
            RoughDielectric,
            Conductor,
            RoughConductor,
            Plastic,
            RoughPlastic,
//        BumpBSDF,
//        BumpMapTexture,
        Emitter,
            AreaEmitter,
//            EnvironmentEmitter,
        Scene,

        Count,

        ShapeBegin = Shape + 1,
        ShapeEnd = Cylinder + 1,
        ShapeCount = ShapeEnd - ShapeBegin,

        BSDFBegin = BSDF + 1,
        BSDFEnd = RoughPlastic + 1,
        BSDFCount = BSDFEnd - BSDFBegin,

        EmitterBegin = Emitter + 1,
        EmitterEnd = AreaEmitter + 1,
        EmitterCount = EmitterEnd - EmitterBegin,
    };
    static ProceduralPtr Generate(Variant variant, RNG &rng, std::vector<Variant> exclude = {});

    virtual void Generate(RNG &rng) = 0;
};