#include "BSDF.hpp"

Schema BSDFSchema = 
{
    .baseName = "bsdf"
};

void BSDF::Generate(RNG &rng)
{
}

Schema RoughBSDFSchema =
{
    .parent = &BSDFSchema,
    .attributes = 
    {
        { Type::String, "distribution", GETTER(RoughBSDF, distribution) },
        { Type::Variant, "alpha", GETTER(RoughBSDF, alpha) },
        { Type::Variant, "alpha_u", GETTER(RoughBSDF, alpha_u) },
        { Type::Variant, "alpha_v", GETTER(RoughBSDF, alpha_v) }
    }
};

void RoughBSDF::Generate(RNG &rng)
{
    distribution = "beckmann";
    alpha = ::Variant((Float) rng.NextFloat(0.01f, 0.1f));
}

Schema DiffuseSchema = 
{
    .parent = &BSDFSchema,
    .typeName = "diffuse",
    .attributes = 
    {
        { Type::Spectrum, "reflectance", GETTER(Diffuse, reflectance) }
    }
};

void Diffuse::Generate(RNG &rng)
{
    BSDF::Generate(rng);

    reflectance = Spectrum(RGB(0.f, 0.f, 0.f));
    for (int c = 0; c < 3; ++c)
        std::get<RGB>(reflectance)[c] = rng.NextFloat();
}

Schema TwoSidedBSDFSchema = 
{
    .parent = &BSDFSchema,
    .typeName = "twosided",
    .attributes =
    {
        { Type::Tag, "bsdf", GETTER(TwoSidedBSDF, bsdf) }
    }
};

void TwoSidedBSDF::Generate(RNG &rng)
{
    if (!bsdf) 
    {
        bsdf = TagPtr(new Diffuse);
    }
    std::dynamic_pointer_cast<Procedural>(bsdf)->Generate(rng);
}

TwoSidedBSDF::TwoSidedBSDF(TagPtr bsdf) :
    bsdf(bsdf) {}

Schema DielectricBSDFSchema =
{
    .parent = &BSDFSchema,
    .typeName = "dielectric",
    .attributes = 
    {
        { Type::Float, "int_ior", GETTER(DielectricBSDF, int_ior) },
        { Type::Float, "ext_ior", GETTER(DielectricBSDF, ext_ior) },
    }
};

void DielectricBSDF::Generate(RNG &rng)
{
    BSDF::Generate(rng);

    int_ior = rng.NextFloat(1.31f, 2.65f);
    ext_ior = 1.000277f;
}

Schema ThinDielectricBSDFSchema =
{
    .parent = &BSDFSchema,
    .typeName = "thindielectric",
    .attributes = 
    {
        { Type::Float, "int_ior", GETTER(ThinDielectricBSDF, int_ior) },
        { Type::Float, "ext_ior", GETTER(ThinDielectricBSDF, ext_ior) },
    }
};

void ThinDielectricBSDF::Generate(RNG &rng)
{
    BSDF::Generate(rng);

    int_ior = rng.NextFloat(1.31f, 2.65f);
    ext_ior = 1.000277f;
}

Schema RoughDielectricBSDFSchema = 
{
    .parent = &RoughBSDFSchema,
    .typeName = "roughdielectric",
    .attributes = 
    {
        { Type::Float, "int_ior", GETTER(RoughDielectricBSDF, int_ior) },
        { Type::Float, "ext_ior", GETTER(RoughDielectricBSDF, ext_ior) },
    }
};

void RoughDielectricBSDF::Generate(RNG &rng)
{
    RoughBSDF::Generate(rng);

    int_ior = rng.NextFloat(1.31f, 2.65f);
    ext_ior = 1.000277f;
}

constexpr const char* conductor_materials[] = 
{
    "a-C",
    "Ag",
    "Al",
    "AlAs",
    "AlSb",
    "Au",
    "Be",
    "Cr",
    //"CsI",
    "Cu",
    //"Cu20",
    "CuO",
    //"d-C",
    "Hg",
    "HgTe",
    "Ir",
    "K",
    "Li",
    "MgO",
    "Mo",
    "Na_palik",
    "Nb",
    //"Ni",
    "Rh",
    "Se",
    "SiC",
    //"SnTe",
    "Ta",
    "Te",
    //"ThF4",
    "TiC",
    "TiN",
    "TiO2",
    "VC",
    "V_palik",
    "VN",
    "W",
};

Schema ConductorBSDFSchema = 
{
    .parent = &BSDFSchema,
    .typeName = "conductor",
    .attributes = 
    {
        { Type::String, "material", GETTER(ConductorBSDF, material) }
    }
};

void ConductorBSDF::Generate(RNG &rng)
{
    BSDF::Generate(rng);

    material = conductor_materials[rng.NextInt() % std::size(conductor_materials)];
}

Schema RoughConductorBSDFSchema = 
{
    .parent = &RoughBSDFSchema,
    .typeName = "roughconductor",
    .attributes = 
    {
        { Type::String, "material", GETTER(RoughConductorBSDF, material) }
    }
};

void RoughConductorBSDF::Generate(RNG &rng)
{
    RoughBSDF::Generate(rng);

    material = conductor_materials[rng.NextInt() % std::size(conductor_materials)];
}

Schema PlasticBSDFSchema = 
{
    .parent = &BSDFSchema,
    .typeName = "plastic",
    .attributes = 
    {
        { Type::Spectrum, "diffuse_reflectance", GETTER(PlasticBSDF, diffuse_reflectance) },
        { Type::Variant, "int_ior", GETTER(PlasticBSDF, diffuse_reflectance) },
        { Type::Variant, "ext_ior", GETTER(PlasticBSDF, diffuse_reflectance) },
    }
};

void PlasticBSDF::Generate(RNG &rng)
{
    BSDF::Generate(rng);

    int_ior = ::Variant(rng.NextFloat(1.31f, 1.81f));
    ext_ior = ::Variant(1.000277f);
}

Schema RoughPlasticBSDFSchema = 
{
    .parent = &RoughBSDFSchema,
    .typeName = "roughplastic",
    .attributes = 
    {
        { Type::Spectrum, "diffuse_reflectance", GETTER(RoughPlasticBSDF, diffuse_reflectance) },
        { Type::Variant, "int_ior", GETTER(RoughPlasticBSDF, diffuse_reflectance) },
        { Type::Variant, "ext_ior", GETTER(RoughPlasticBSDF, diffuse_reflectance) },
    }
};

void RoughPlasticBSDF::Generate(RNG &rng)
{
    RoughBSDF::Generate(rng);
    
    int_ior = ::Variant(rng.NextFloat(1.31f, 1.81f));
    ext_ior = ::Variant(1.000277f);
}

#if 0
Schema BumpMapBSDFSchema = 
{
    .parent = &BSDFSchema,
    .typeName = "bumpmap",
    .attributes = 
    {
        { Type::Tag, "bsdf", GETTER(BumpMapBSDF, bsdf) },
        { Type::Tag, "texture", GETTER(BumpMapBSDF, texture) },
        { Type::Float, "scale", GETTER(BumpMapBSDF, scale) }
    }
};

void BumpMapBSDF::Generate(RNG &rng)
{
    BSDF::Generate(rng);

    bsdf = std::dynamic_pointer_cast<Tag>(Procedural::Generate(
        Procedural::Variant::BSDF, rng));
    
    texture = std::dynamic_pointer_cast<Tag>(Procedural::Generate(
        Procedural::Variant::BumpMapTexture, rng));
    
    scale = 0.5f;
}
#endif
