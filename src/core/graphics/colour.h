#pragma once

#include "core/typedefs.h"
#include "core/math.hpp"

namespace idrs
{
    struct Colour
    {
        u8 r = 255;
        u8 g = 255;
        u8 b = 255;
        u8 a = 255;
    };

    namespace colour
    {
        inline const Vec4f normalizeColour(Colour colour)
        {
            Vec4f c;
            c.x = colour.r / 255.0f;
            c.y = colour.g / 255.0f;
            c.z = colour.b / 255.0f;
            c.w = colour.a / 255.0f;

            return c;
        }

        inline constexpr Colour black    = { 0, 0, 0, 255 };
        inline constexpr Colour white    = { 255, 255, 255, 255 };
        inline constexpr Colour red      = { 255, 0, 0, 255 };
        inline constexpr Colour green    = { 0, 255, 0, 255 };
        inline constexpr Colour blue     = { 0, 0, 255, 255 };
        inline constexpr Colour yellow   = { 255, 255, 0, 255 };
        inline constexpr Colour magenta  = { 255, 0, 255, 255 };
        inline constexpr Colour cyan     = { 0, 255, 255, 255 };
    }
}
