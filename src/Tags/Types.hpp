#pragma once

#include <eigen3/Eigen/Dense>
#include <memory>
#include <variant>
#include <vector>

using Float = float;
using Point = Eigen::Vector3f;
using RGB = Eigen::Vector3f;
using Transform = Eigen::Matrix4f;

class Tag;
using TagPtr = std::shared_ptr<Tag>;
using TagVector = std::vector<TagPtr>;

using Spectrum = std::variant<std::monostate, RGB, TagPtr>;
using Variant = std::variant<std::monostate, Float, TagPtr, std::string>;

enum class Type
{
    Tag,
    TagVector,
    Float,
    Point,
    Spectrum,
    Transform,
    String,
    Variant,

    Count
};