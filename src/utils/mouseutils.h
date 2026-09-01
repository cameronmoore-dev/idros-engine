#pragma once

#include "core/window/window.h"
#include "core/graphics/view2d.h"

namespace idrs
{
    class MouseUtils
    {
    public:
        static Vec2f getCursorWorldPos(Window &window, View2D &view);
    };
}

