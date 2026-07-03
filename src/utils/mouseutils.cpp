#include "mouseutils.h"

namespace idrs
{
    Vec2f MouseUtils::getCursorWorldPos(Window &window, View2D &view)
    {
        s32 x, y;
        window.getCursorPos(x, y);
        Vec2f worldPos = { (f32)x, (f32)y };

        return view.screenToWorld(worldPos);
    }
}

