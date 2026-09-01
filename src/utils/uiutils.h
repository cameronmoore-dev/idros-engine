#pragma once

#include "core/math.hpp"

namespace idrs
{
    class UIUtils
    {
    public:
        static bool isPointWithin(s32 x, s32 y, FRect &bounds);
    };
}

