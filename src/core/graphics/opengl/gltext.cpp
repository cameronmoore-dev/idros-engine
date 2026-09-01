#include "../text.h"

#include <glad/gl.h>

namespace idrs
{
    Text::Text() : 
        m_arr(VertexArray::Primitive::Triangle, VertexArray::Usage::Static, 0)
    {
        setupBuffers();
    }

    Text::Text(const std::string &text) :
        m_text(text),
        m_arr(VertexArray::Primitive::Triangle, VertexArray::Usage::Static, (m_text.size() * k_verticesPerGlyph))
    {
        setupBuffers();
    }

    Text::~Text()
    {
        delete(m_font);
    }

    void Text::updateFont()
    {
        setVertexPositions();
        setVertexUVs();
    }

    void Text::draw()
    {
        m_font->getGlyphAtlas().bind();

        glBindVertexArray(m_arr.id());
        glDrawElements(m_arr.primitive(), m_arr.indices().size(), GL_UNSIGNED_INT, 0);
    }

    void Text::setFont(Font &font)
    {
        m_font = &font;
    }

    void Text::setPosition(const Vec2f &pos)
    {
        m_position = pos;
        setVertexPositions();
    }

    void Text::setPosition(const f32 x, const f32 y)
    {
        m_position = { x, y };
        setVertexPositions();
    }

    void Text::setCharacterPadding(const Vec2f &padding)
    {
        m_padding = padding;
    }

    void Text::setCharacterPadding(const f32 hor, const f32 ver)
    {
        m_padding = { hor, ver };
    }

    void Text::setText(const std::string &text)
    {
        m_text = text;
        m_arr.clear();
        m_arr.resize(m_text.size() * k_verticesPerGlyph);

        appendVertexIndices();
        setVertexPositions();
        setVertexUVs();
    }

    const std::string &Text::getTextString() const
    {
        return m_text;
    }

    const Font &Text::getFont() const
    {
        return *m_font;
    }

    const Vec2f &Text::getPosition() const
    {
        return m_position;
    }

    const Vec2f &Text::getCharacterPadding() const
    {
        return m_padding;
    }

    void Text::setupBuffers()
    {
        m_arr.generate();
        m_arr.setVertexAttrib(0, 3, VertexArray::VertexAttribType::Float, false, offsetof(Vertex, position));
        m_arr.setVertexAttrib(1, 2, VertexArray::VertexAttribType::Float, false, offsetof(Vertex, uv));
        m_arr.resize(m_text.size() * k_verticesPerGlyph);

        appendVertexIndices();
    }

    void Text::appendVertexIndices()
    {
        for (u32 i = 0; i < m_text.size(); i++)
        {
            u32 idx = i * k_verticesPerGlyph;
            m_arr.append
            ({ 
                0 + idx,
                1 + idx,
                2 + idx,
                2 + idx,
                1 + idx,
                3 + idx
            });
        }
    }

    void Text::setVertexPositions()
    {
        u32 accumTextWidth = 0;
        u32 height = 0;
        u32 idx = 0;
        for (const char c : m_text)
        {
            Glyph g = m_font->getGlyphMap().at(c);

            /* If a new line character is found: skip the character and move each new glyphs position down by the font size (height) */
            if (c == '\n')
            {
                accumTextWidth = 0;

                height += m_font->getFontSize() + m_padding.y;
                idx += k_verticesPerGlyph;
                continue;
            }

            f32 x = (m_position.x + accumTextWidth) + g.bearing.x;
            f32 y = (m_position.y - (g.size.y - g.bearing.y)) - height;
            f32 w = g.size.x;
            f32 h = g.size.y;

            m_arr[idx + 0].position = { x, y };
            m_arr[idx + 1].position = { x + w, y };
            m_arr[idx + 2].position = { x, y + h };
            m_arr[idx + 3].position = { x + w, y + h };

            accumTextWidth += ((g.advance >> 6) + m_padding.x);
            idx += k_verticesPerGlyph;
        }

        m_arr.update();
    }

    void Text::setVertexUVs()
    {
        f32 texSize = 12 * m_font->getFontSize();
        u32 idx = 0;
        for (const char c : m_text)
        {
            Glyph g = m_font->getGlyphMap().at(c);

            /* 
             * NOTE: Because there's a mismatch in how Freetype and OpenGL handle texture coords,
             *       the texture coords for each glyph have to be flipped vertically
             */
            f32 px = (f32)g.pos.x / texSize;
            f32 py = ((f32)g.pos.y + g.size.y) / texSize;
            f32 nx = ((f32)g.pos.x + g.size.x) / texSize;
            f32 ny = (f32)g.pos.y / texSize;

            m_arr[idx + 0].uv = { px, py };
            m_arr[idx + 1].uv = { nx, py };
            m_arr[idx + 2].uv = { px, ny };
            m_arr[idx + 3].uv = { nx, ny };

            idx += k_verticesPerGlyph;
        }

        m_arr.update();
    }
}
