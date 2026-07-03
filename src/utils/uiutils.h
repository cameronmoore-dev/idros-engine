#pragma once

#include <idlai/math.hpp>
#include <idlai/typedefs.h>

namespace idrs
{
    class UIUtils
    {
    public:
        static bool isPointWithin(s32 x, s32 y, FRect &bounds);
    };
}

