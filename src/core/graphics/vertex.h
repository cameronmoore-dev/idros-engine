#pragma once

#include "core/math.hpp"
#include "core/graphics/colour.h"

namespace idrs
{
    struct Vertex
    {
        Vec3f position;
        Vec3f normal;
        Vec2f uv;
    };
}