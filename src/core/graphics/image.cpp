#include "image.h"

#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image/stb_image_write.h>

namespace idrs
{
    Image::Image(u32 w, u32 h, u32 bpp) : 
        m_width(w),
        m_height(h),
        m_bpp(bpp),
        m_data(nullptr)
    {
    }

    Image::~Image()
    {
        delete(m_data);
    }

    void Image::load(const std::string &path)
    {
        stbi_set_flip_vertically_on_load(1);

        s32 w, h, c;
        u8 *data = stbi_load(path.c_str(), &w, &h, &c, 0);
        if (!data)
        {
            printf("FAILED TO LOAD IMAGE!\n");
            printf("PATH: %s\n", path.c_str());
            return;
        }

        m_width = w;
        m_height = h;
        m_bpp = c;

        if (!m_data)
        {
            m_data = new u8[m_width * m_height * m_bpp];
        }

        std::memcpy(m_data, data, m_width * m_height * m_bpp);
        stbi_image_free(data);
    }

    void Image::write(const std::string &dir, const std::string &name, Format fmt)
    {
        std::string path = dir + "/" + name;

        /* NOTE: Check for non-existent directory (create or return) using std::filesystem */

        stbi_flip_vertically_on_write(1);

        switch (fmt)
        {
            case Format::PNG:
            {
                path += ".png";
                stbi_write_png(path.c_str(), m_width, m_height, m_bpp, m_data, m_width * m_bpp);
            } break;

            case Format::TGA: 
            {
                path += ".tga";
                stbi_write_tga(path.c_str(), m_width, m_height, m_bpp, m_data);
            } break;

            case Format::BMP: 
            {
                path += ".bmp";
                stbi_write_bmp(path.c_str(), m_width, m_height, m_bpp, m_data);
            } break;

            default: break;
        }
    }

    void Image::setPixel(u32 x, u32 y, Colour clr)
    {
        u32 index = x + y * (m_width * m_bpp);

        switch (m_bpp)
        {
            case 1:
            {
                m_data[index] = clr.r;
            } break;

            case 3:
            {
                m_data[index] = clr.r;
                m_data[index + 1] = clr.g;
                m_data[index + 2] = clr.b;
            } break;

            case 4:
            {
                m_data[index] = clr.r;
                m_data[index + 1] = clr.g;
                m_data[index + 2] = clr.b;
                m_data[index + 3] = clr.a;
            } break;

            default: break;
        }
    }

    Colour Image::getPixel(u32 x, u32 y)
    {
        u32 index = x + y * (m_width * m_bpp);

        switch (m_bpp)
        {
            case 1: return { m_data[index] }; break;
            case 3: return { m_data[index], m_data[index + 1], m_data[index + 2] }; break;
            case 4: return { m_data[index], m_data[index + 1], m_data[index + 2], m_data[index + 3] }; break;
        }
        return { 255, 0, 255, 255 };
    }

    const u32 Image::getWidth()
    {
        return m_width;
    }

    const u32 Image::getHeight()
    {
        return m_height;
    }

    const u32 Image::getBPP()
    {
        return m_bpp;
    }

    u8 *Image::data()
    {
        return m_data;
    }
}
