#pragma once

#include "math.hpp"
#include "colour.h"

namespace idrs
{
    struct Vertex
    {
        Vec3f position;
        Vec3f normal;
        Vec2f uv;
    };
}