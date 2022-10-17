#pragma once

#include "Procedural.hpp"
#include "Tag.hpp"
#include "Types.hpp"

#include <string>

extern Schema EmitterSchema;

struct Emitter : public Procedural
{
    virtual void Generate(RNG &rng) override;
};

extern Schema AreaEmitterSchema;

struct AreaEmitter : public Emitter, SchemaWrapper<AreaEmitterSchema>
{
    Spectrum radiance;

    virtual void Generate(RNG &rng) override final;
};

extern Schema EnvironmentEmitterSchema;

struct EnvironmentEmitter : public Emitter, SchemaWrapper<EnvironmentEmitterSchema>
{
    std::string filename;
    Transform to_world;

    virtual void Generate(RNG &rng) override final;
};