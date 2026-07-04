#pragma once

#include <idcore/window/window.h>
#include <idcore/graphics/view2d.h>

namespace idrs
{
    class MouseUtils
    {
    public:
        static Vec2f getCursorWorldPos(Window &window, View2D &view);
    };
}

