#pragma once

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
        Image(u32 w, u32 h, u32 bpp);
        ~Image();

        void load(const std::string &path);
        void write(const std::string &dir, const std::string &name, Format fmt);
        void setPixel(u32 x, u32 y, Colour clr);
        Colour getPixel(u32 x, u32 y);

        //void: resize

        const u32 getWidth();
        const u32 getHeight();
        const u32 getBPP();

        u8 *data();

    private:
        u32 m_width;
        u32 m_height;
        u32 m_bpp;
        u8 *m_data;
    };
}
