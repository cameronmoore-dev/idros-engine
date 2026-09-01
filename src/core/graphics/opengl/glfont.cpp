#include "../font.h"

#include <glad/gl.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <cassert>

namespace idrs
{
    Font::Font(const std::string &path, const u32 fontSize) :
        m_path(path),
        m_fontSize(fontSize)
    {
        create();
    }

    Font::~Font()
    {
        m_glyphAtlas.destroy();
    }

    void Font::load(const std::string &path, const u32 fontSize)
    {
        m_path = path;
        m_fontSize = fontSize;
        create();
    }

    void Font::create()
    {
        m_glyphs.clear();
        m_glyphAtlas.destroy();

        FT_Library ft;
        FT_Face face;
        FT_Init_FreeType(&ft);
        FT_New_Face(ft, m_path.c_str(), 0, &face);

        FT_Set_Pixel_Sizes(face, 0, m_fontSize);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        m_approxSize = (12 * m_fontSize);
        m_glyphAtlas.create(m_approxSize, m_approxSize, sizeof(u8), nullptr);
        loadAscii(face);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        // Image img(m_approxSize, m_approxSize, 1);
        // glBindTexture(GL_TEXTURE_2D, m_glyphAtlas.id());
        // glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_UNSIGNED_BYTE, img.data());

        // img.write("./", "atlas", Image::Format::PNG);
    }

    void Font::setFontSize(const u32 size)
    {
        m_fontSize = size;
        create();
    }

    const std::map<char, Glyph> &Font::getGlyphMap() const
    {
        return m_glyphs;
    }

    const Texture &Font::getGlyphAtlas() const
    {
        return m_glyphAtlas;
    }

    const u32 Font::getFontSize() const
    {
        return m_fontSize;
    }

    void Font::loadAscii(const void *f)
    {
        FT_Face face = (FT_Face)(f);

        u32 accumWidth = 0;
        u32 h = 0;
        for (u32 i = 0; i < 128; i++)
        {
            assert(!FT_Load_Char(face, i, FT_LOAD_RENDER));

            Glyph g;
            g.pos.x     = accumWidth;
            g.pos.y     = m_fontSize * h;
            g.size.x    = face->glyph->bitmap.width;
            g.size.y    = face->glyph->bitmap.rows;
            g.advance   = face->glyph->advance.x;
            g.bearing.x = face->glyph->bitmap_left;
            g.bearing.y = face->glyph->bitmap_top;

            m_glyphs.insert(std::pair<char, Glyph>(i, g));

            accumWidth += face->glyph->bitmap.width + k_pixelPaddingWidth;
            if (accumWidth + face->glyph->bitmap.width > m_approxSize)
            {
                accumWidth = 0;
                h++;
            }
            glTexSubImage2D(GL_TEXTURE_2D, 0, g.pos.x, g.pos.y, g.size.x, g.size.y, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
        }
    }
}