#pragma once

#include <vector>
#include <memory>

#include "Procedural.hpp"
#include "Tag.hpp"

extern Schema SceneSchema;

struct Scene : Procedural, SchemaWrapper<SceneSchema>
{
    TagVector children;

    virtual void Generate(RNG &rng) override final;
};