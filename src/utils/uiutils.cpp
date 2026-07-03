#include "uiutils.h"

namespace idrs
{
    bool UIUtils::isPointWithin(s32 x, s32 y, FRect &bounds)
    {
        if (x < bounds.x || x > bounds.x + bounds.w) return false;
        if (y < bounds.y || y > bounds.y + bounds.h) return false;
        return true;
    }
}

