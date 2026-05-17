#pragma once

#include "strdmath.h"
#include "colour.h"

namespace strd
{
    struct Vertex
    {
        Vec3f position;
        Vec3f normal;
        Vec2f uv;
    };
}