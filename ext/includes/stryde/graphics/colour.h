#pragma once

#include "typedefs.h"

namespace strd
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
        inline const Colour black    = { 0, 0, 0, 255 };
        inline const Colour white    = { 255, 255, 255, 255 };
        inline const Colour red      = { 255, 0, 0, 255 };
        inline const Colour green    = { 0, 255, 0, 255 };
        inline const Colour blue     = { 0, 0, 255, 255 };
        inline const Colour yellow   = { 255, 255, 0, 255 };
        inline const Colour magenta  = { 255, 0, 255, 255 };
        inline const Colour cyan     = { 0, 255, 255, 255 };
    }
}