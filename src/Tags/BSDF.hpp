#pragma once

#include "Procedural.hpp"
#include "Tag.hpp"
#include "Types.hpp"

extern Schema BSDFSchema;

struct BSDF : public Procedural
{
    virtual void Generate(RNG &rng) override;
};

extern Schema RoughBSDFSchema;

struct RoughBSDF : public BSDF
{
    std::string distribution;
    ::Variant alpha, alpha_u, alpha_v;
    
    virtual void Generate(RNG &rng) override;
};

extern Schema DiffuseSchema;

struct Diffuse : public BSDF, SchemaWrapper<DiffuseSchema>
{
    Spectrum reflectance;

    virtual void Generate(RNG &rng) override final;
};

extern Schema TwoSidedBSDFSchema;

struct TwoSidedBSDF : public BSDF, SchemaWrapper<TwoSidedBSDFSchema>
{
    TagPtr bsdf;

    virtual void Generate(RNG &rng) override final;

    TwoSidedBSDF(TagPtr bsdf = nullptr);
};

extern Schema DielectricBSDFSchema;

struct DielectricBSDF : public BSDF, SchemaWrapper<DielectricBSDFSchema>
{
    Float int_ior;
    Float ext_ior;

    virtual void Generate(RNG &rng) override final;
};

//---
extern Schema ThinDielectricBSDFSchema;

struct ThinDielectricBSDF : public BSDF, SchemaWrapper<ThinDielectricBSDFSchema>
{
    Float int_ior;
    Float ext_ior;

    virtual void Generate(RNG &rng) override final;
};

extern Schema RoughDielectricBSDFSchema;

struct RoughDielectricBSDF : public RoughBSDF, SchemaWrapper<RoughDielectricBSDFSchema>
{
    Float int_ior;
    Float ext_ior;

    virtual void Generate(RNG &rng) override final;
};

extern Schema ConductorBSDFSchema;

struct ConductorBSDF : public BSDF, SchemaWrapper<ConductorBSDFSchema>
{
    std::string material;

    virtual void Generate(RNG &rng) override final;
};

extern Schema RoughConductorBSDFSchema;

struct RoughConductorBSDF : public RoughBSDF, SchemaWrapper<RoughConductorBSDFSchema>
{
    std::string material;

    virtual void Generate(RNG &rng) override final;
};

extern Schema PlasticBSDFSchema;

struct PlasticBSDF : public BSDF, SchemaWrapper<PlasticBSDFSchema>
{
    Spectrum diffuse_reflectance;
    ::Variant int_ior, ext_ior;

    virtual void Generate(RNG &rng) override final;
};

extern Schema RoughPlasticBSDFSchema;

struct RoughPlasticBSDF : public RoughBSDF, SchemaWrapper<RoughPlasticBSDFSchema>
{
    Spectrum diffuse_reflectance;
    ::Variant int_ior, ext_ior;

    virtual void Generate(RNG &rng) override final;
};

#if 0
extern Schema BumpMapBSDFSchema;

struct BumpMapBSDF : public BSDF, SchemaWrapper<BumpMapBSDFSchema>
{
    TagPtr texture;
    TagPtr bsdf;
    Float scale;

    virtual void Generate(RNG &rng) override final;
};
#endif
