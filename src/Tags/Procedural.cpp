#include "Procedural.hpp"

#include "BSDF.hpp"
#include "Emitter.hpp"
#include "Scene.hpp"
#include "Shape.hpp"
#include "Texture.hpp"
#include <cstdlib>

RNG::RNG(int seed) : seed(seed) 
{
}

float RNG::NextFloat(float min, float max)
{
    return min + ((float) NextInt() / (float) RAND_MAX) * (max - min);
}

int RNG::NextInt()
{
    srand(seed);
    return seed = rand();
}

ProceduralPtr Procedural::Generate(Variant variant, RNG &rng, std::vector<Variant> exclude)
{
    ProceduralPtr p = nullptr;
    switch (variant)
    {
        case Variant::Shape:
        {
            uint64_t x = uint64_t(Variant::ShapeBegin) 
                + (rng.NextInt() % uint64_t(Variant::ShapeCount));
            return Generate((Variant) x, rng);
        }
        case Variant::Sphere:
            p = std::make_shared<Sphere>();
            break;
//        case Variant::ObjMesh:
//            p = std::make_shared<ObjMesh>();
//            break;
        case Variant::Cylinder:
            p = std::make_shared<Cylinder>();
            break;
//        case Variant::BumpBSDF:
//            p = std::make_shared<BumpMapBSDF>();
//            break;
//        case Variant::BumpMapTexture:
//            p = std::make_shared<BumpMapTexture>();
//            break;
        case Variant::BSDF:
        {
            sample_bsdf_variant:
            uint64_t x = uint64_t(Variant::BSDFBegin) 
                + (rng.NextInt() % uint64_t(Variant::BSDFCount));
            for (auto v : exclude)
                if (x == (uint64_t) v)
                    goto sample_bsdf_variant;
            return Generate((Variant) x, rng);
        }
        case Variant::AreaEmitter:
        {
            p = std::make_shared<AreaEmitter>();
            break;
        }
//        case Variant::EnvironmentEmitter:
//        {
//            p = std::make_shared<EnvironmentEmitter>();
//            break;
//        }
        case Variant::Dielectric:
            p = std::make_shared<DielectricBSDF>();
            break;
        case Variant::Diffuse:
            p = std::make_shared<TwoSidedBSDF>(std::make_shared<Diffuse>());
            break;
        case Variant::ThinDielectric:
            p = std::make_shared<ThinDielectricBSDF>();
            break;
        case Variant::RoughDielectric:
            p = std::make_shared<RoughDielectricBSDF>();
            break;
        case Variant::Conductor:
            p = std::make_shared<TwoSidedBSDF>(std::make_shared<ConductorBSDF>());
            break;
        case Variant::RoughConductor:
            p = std::make_shared<TwoSidedBSDF>(std::make_shared<RoughConductorBSDF>());
            break;
        case Variant::Plastic:
            p = std::make_shared<TwoSidedBSDF>(std::make_shared<PlasticBSDF>());
            break;
        case Variant::RoughPlastic:
            p = std::make_shared<TwoSidedBSDF>(std::make_shared<RoughPlasticBSDF>());
            break;
        case Variant::Scene:
            p = std::make_shared<Scene>();
            break;
        default:
            std::abort();
    }
    p->Generate(rng);
    return p;
}