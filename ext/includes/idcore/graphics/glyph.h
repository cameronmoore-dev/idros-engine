#pragma once

#include "math.hpp"

namespace idrs
{
    struct Glyph
    {
        Vec2i pos;
        Vec2i size;
        long int advance;
        struct
        {
            s32 x = 0;
            s32 y = 0;
        } bearing;
    };
}