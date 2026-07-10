#pragma once

#include "ecs/components.h"

namespace idrs
{
    class TagUtils
    {
    public:
        static void set(Tag &tag, const char* str);
        static bool compare(Tag &a, Tag &b);
        static bool compare(Tag &tag, const char *str);
    };
}

