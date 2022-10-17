#pragma once

#include "Procedural.hpp"
#include "Tag.hpp"
#include "Types.hpp"

extern Schema TextureSchema;

struct Texture : public Procedural
{
    std::string filename;

    virtual void Generate(RNG &rng) override;
};

extern Schema BumpMapTextureSchema;

struct BumpMapTexture : public Texture, SchemaWrapper<BumpMapTextureSchema>
{

    virtual void Generate(RNG &rng) override final;
};