#include "../texture.h"

#include <glad/gl.h>
#include <stb_image/stb_image.h>

namespace idrs
{
    Texture::Texture(const WrapMode wrapMode, const Filter filter) : 
        m_wrapMode(wrapMode),
        m_filter(filter)
    {
    }

    Texture::Texture(const WrapMode wrapMode, const Filter filter, const MipMapFilter mipMapFilter) :
        m_wrapMode(wrapMode),
        m_filter(filter),
        m_mmFilter(mipMapFilter),
        m_hasMipMaps(true)
    {
    }

    void Texture::load(const std::string &path, const bool flip, const bool srgb)
    {
        stbi_set_flip_vertically_on_load(flip);

        s32 w, h, c;
        u8 *data = stbi_load(path.c_str(), &w, &h, &c, 0);
        if (!data)
        {
            printf("FAILED TO LOAD TEXTURE IMAGE!\n");
            printf("PATH: %s\n", path.c_str());
            return;
        }

        m_isSrgb = srgb;
        create(w, h, c, data);
        stbi_image_free(data);
    }

    void Texture::create(const u32 w, const u32 h, const u32 bpp, const u8 *data)
    {
        m_width  = w;
        m_height = h;
        m_bpp    = bpp;

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, getWrapMode());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, getWrapMode());

        if (!m_hasMipMaps)  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getFilter());
        else                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, getMipMapFilter());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, getFilter());

        GLenum iformat = 0;
        GLenum format = 0;
        switch (bpp)
        {
            case 1:
            {
                iformat = GL_R8;
                format  = GL_RED;
            } break;

            case 3:
            {
                iformat = m_isSrgb ? GL_SRGB8 : GL_RGB8;
                format  = GL_RGB;
            } break;

            case 4:
            {
                iformat = m_isSrgb ? GL_SRGB8_ALPHA8 : GL_RGBA8;
                format  = GL_RGBA;
            } break;

            default: break;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, iformat, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        if (m_hasMipMaps)
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }

    void Texture::bind() const
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_id);
    }

    void Texture::destroy()
    {
        glDeleteTextures(1, &m_id);
    }

    void Texture::copyTo(u8 *buf)
    {
        GLenum f = 0;
        switch (m_bpp)
        {
            case 1: f = GL_RED;  break;
            case 3: f = GL_RGB;  break;
            case 4: f = GL_RGBA; break;
        }

        glBindTexture(GL_TEXTURE_2D, m_id);
        glGetTexImage(GL_TEXTURE_2D, 0, f, GL_UNSIGNED_BYTE, buf);
    }

    void Texture::copyFrom(const u32 w, const u32 h, const u32 bpp, const u8 *data)
    {
        /* 
         * NOTE: Do some checks to find if the replacement image is within the old textures memory bounds.
         *       If it is: replace the region of the current texture with input image data.
         *       Other wise fully overwrite with the new image data.
         */

        // GLenum format;
        // switch (bpp)
        // {
        //     case 1: format = GL_RED; break;
        //     case 3: format = GL_RGB; break;
        //     case 4: format = GL_RGBA; break;
        //     default: break;
        // }
        
        // glBindTexture(GL_TEXTURE_2D, m_id);
        // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, format, GL_UNSIGNED_BYTE, data);
        // glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
    }

    void Texture::createFrom(const u32 w, const u32 h, const u32 bpp, const u8 *data, const bool srgb)
    {
        if (!data)
        {
            return;
        }

        m_isSrgb = srgb;
        create(w, h, bpp, data);
    }

    const u32 Texture::getWrapMode() const
    {
        switch (m_wrapMode)
        {
            case WrapMode::Repeat:          return GL_REPEAT; break;
            case WrapMode::MirroredRepeat:  return GL_MIRRORED_REPEAT; break;
            case WrapMode::ClampToEdge:     return GL_CLAMP_TO_EDGE; break;
            case WrapMode::ClampToBorder:   return GL_CLAMP_TO_BORDER; break;
        }
        return -1;
    }

    const u32 Texture::getFilter() const
    {
        switch (m_filter)
        {
            case Filter::Nearest: return GL_NEAREST; break;
            case Filter::Linear:  return GL_LINEAR;  break;
        }
        return -1;
    }

    const u32 Texture::getMipMapFilter() const
    {
        switch (m_mmFilter)
        {
            case MipMapFilter::NearestNearest: return GL_NEAREST_MIPMAP_NEAREST; break;
            case MipMapFilter::LinearNearest:  return GL_LINEAR_MIPMAP_NEAREST; break;
            case MipMapFilter::NearestLinear:  return GL_NEAREST_MIPMAP_LINEAR; break;
            case MipMapFilter::LinearLinear:   return GL_LINEAR_MIPMAP_LINEAR; break;
        }
        return -1;
    }

    const u32 Texture::getWidth() const
    {
        return m_width;
    }

    const u32 Texture::getHeight() const
    {
        return m_height;
    }

    const u32 Texture::getBPP() const
    {
        return m_bpp;
    }

    const u32 Texture::id() const
    {
        return m_id;
    }
}