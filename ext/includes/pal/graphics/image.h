#pragma once

#include <cstring>
#include <string>

#include "typedefs.h"
#include "colour.h"

namespace idrs
{
    class Image
    {
    public:
        enum class Format
        {
            PNG = 0,
            TGA,
            BMP
        };

    public:
        Image() = default;
        Image(const u32 w, const u32 h, const u32 bpp);
        ~Image();

        void load(const std::string &path);
        void write(const std::string &dir, const std::string &name, const Format fmt);
        void setPixel(const u32 x, const u32 y, const Colour clr);
        Colour getPixel(const u32 x, const u32 y);

        //void: resize

        const u32 getWidth() const;
        const u32 getHeight() const;
        const u32 getBPP() const;

        u8 *data();

    private:
        u32 m_width = 0;
        u32 m_height = 0;
        u32 m_bpp = 0;
        u8 *m_data = nullptr;
    };
}