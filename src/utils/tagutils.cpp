#include "tagutils.h"

#include <string_view>
#include <string.h>

namespace idrs
{
    void TagUtils::set(Tag &tag, const char* str)
    {
        strcpy(tag.string, str);
        tag.hash = std::hash<std::string_view>{}(str);
    }

    bool TagUtils::compare(Tag &a, Tag &b)
    {
        return (a.hash == b.hash);
    }

    bool TagUtils::compare(Tag &tag, const char *str)
    {
        u32 hash = std::hash<std::string_view>{}(str);
        return (tag.hash == hash);
    }
}
