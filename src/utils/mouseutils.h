#pragma once

#include <idlai/window/window.h>
#include <idlai/graphics/view2d.h>

namespace idrs
{
    class MouseUtils
    {
    public:
        static Vec2f getCursorWorldPos(Window &window, View2D &view);
    };
}

