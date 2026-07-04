#pragma once

#include <string>
#include <filesystem>

#include "typedefs.h"

namespace idrs
{
    class Texture
    {
    public:
        enum class WrapMode
        {
            Repeat = 0,
            MirroredRepeat,
            ClampToEdge,
            ClampToBorder
        };

        enum class Filter
        {
            Nearest = 0,
            Linear
        };

        enum class MipMapFilter
        {
            NearestNearest = 0,
            LinearNearest,
            NearestLinear,
            LinearLinear
        };

    public:
        Texture() = default;
        Texture(const WrapMode wrapMode, const Filter filter);
        Texture(const WrapMode wrapMode, const Filter filter, const MipMapFilter mipMapFilter);

        void load(const std::string &path, const bool flip = true, const bool srgb = false);
        void create(const u32 w, const u32 h, const u32 bpp, const u8 *data);
        void bind() const;
        void destroy();

        void copyTo(u8 *buf);
        void copyFrom(const u32 w, const u32 h, const u32 bpp, const u8 *data);
        void createFrom(const u32 w, const u32 h, const u32 bpp, const u8 *data, const bool srgb = false);

        /* Setters for texture parameters */
        
        const u32 getWidth() const;
        const u32 getHeight() const;
        const u32 getBPP() const;

        const u32 id() const;

    private:
        u32 m_width = 0;
        u32 m_height = 0;
        u32 m_bpp = 0;
        u32 m_id = 0;

        WrapMode m_wrapMode = WrapMode::Repeat;
        Filter m_filter = Filter::Nearest;
        MipMapFilter m_mmFilter = MipMapFilter::LinearNearest;

        bool m_hasMipMaps = false;
        bool m_isSrgb = false;

    private:

        const u32 getWrapMode() const;
        const u32 getFilter() const;
        const u32 getMipMapFilter() const;
    };
}