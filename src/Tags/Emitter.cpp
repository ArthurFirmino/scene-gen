#include "Emitter.hpp"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

extern fs::path rootDir;

Schema EmitterSchema =
{
    .baseName = "emitter"
};

void Emitter::Generate(RNG &rng)
{

}

Schema AreaEmitterSchema = 
{
    .parent = &EmitterSchema,
    .typeName = "area",
    .attributes = 
    {
        { Type::Spectrum, "radiance", GETTER(AreaEmitter, radiance) }
    }
};

void AreaEmitter::Generate(RNG &rng)
{
    Emitter::Generate(rng);

    radiance = Spectrum(RGB());
    for (int c = 0; c < 3; ++c)
        std::get<RGB>(radiance)[c] = rng.NextFloat();
}

Schema EnvironmentEmitterSchema =
{
    .parent = &EmitterSchema,
    .typeName = "envmap",
    .attributes = 
    {
        { Type::Transform, "to_world", GETTER(EnvironmentEmitter, to_world) },
        { Type::String, "filename", GETTER(EnvironmentEmitter, filename) }
    }
};

void EnvironmentEmitter::Generate(RNG &rng)
{
    Emitter::Generate(rng);

    to_world = Eigen::Affine3f(Eigen::AngleAxisf(
        3.14159265f * 0.5f, Eigen::Vector3f(1.f, 0.f, 0.f))).matrix();
    
    auto environments_directory = rootDir;
    environments_directory += "/resources/environments";
    std::vector<std::string> environments;
    for (const auto& environment : 
        fs::directory_iterator(environments_directory))
    {
        environments.push_back(environment.path().u8string());
    }
    if (!environments.empty())
    {
        int index = rng.NextInt() % environments.size();
        filename = environments[index];
    }
}