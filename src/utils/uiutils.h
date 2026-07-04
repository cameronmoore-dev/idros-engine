#pragma once

#include <idcore/math.hpp>
#include <idcore/typedefs.h>

namespace idrs
{
    class UIUtils
    {
    public:
        static bool isPointWithin(s32 x, s32 y, FRect &bounds);
    };
}

