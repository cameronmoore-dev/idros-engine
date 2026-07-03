#pragma once

#include "ecs/components.h"

namespace idrs
{
    class TextUtils
    {
    public:
        static void setup(UIText &text);
        static void setText(UIText &text, const std::string &str);
        static void updateDrawPosition(UIText &text, const Vec3f &position);

    private:
        static void appendVertexIndices(UIText &text);
        static void setVertexPositions(UIText &text, const Vec3f &position);
        static void setVertexUVs(UIText &text);
    };
}

