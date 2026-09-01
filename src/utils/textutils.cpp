#include "textutils.h"

#include "core/graphics/vertexarray.h"
#include "core/graphics/glyph.h"

#define VERTICES_PER_GLYPH 4

namespace idrs
{
    void TextUtils::setup(UIText &text)
    {
        text.vertices.generate();
        text.vertices.setVertexAttrib(0, 2, VertexArray::VertexAttribType::Float, false, offsetof(Vertex, position));
        text.vertices.setVertexAttrib(1, 2, VertexArray::VertexAttribType::Float, false, offsetof(Vertex, uv));
        text.vertices.resize(text.text.size() * VERTICES_PER_GLYPH);
        appendVertexIndices(text);
    }

    void TextUtils::setText(UIText &text, const std::string &str)
    {
        text.text = str;
        text.vertices.clear();
        text.vertices.resize(text.text.size() * VERTICES_PER_GLYPH);
        appendVertexIndices(text);
    }

    void TextUtils::updateDrawPosition(UIText &text, const Vec3f &position)
    {
        setVertexPositions(text, position);
        setVertexUVs(text);
    }

    void TextUtils::appendVertexIndices(UIText &text)
    {
        for (u32 i = 0; i < text.text.size(); i++)
        {
            u32 idx = i * VERTICES_PER_GLYPH;
            text.vertices.append
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

    void TextUtils::setVertexPositions(UIText &text, const Vec3f &position)
    {
        u32 accumTextWidth = 0;
        u32 height = 0;
        u32 idx = 0;
        for (const char c : text.text)
        {
            Glyph g = text.font->getGlyphMap().at(c);

            /* If a new line character is found: skip the character and move each new glyphs position down by the font size (height) */
            if (c == '\n')
            {
                accumTextWidth = 0;

                height += text.font->getFontSize() + text.charPadding.y;
                idx += VERTICES_PER_GLYPH;
                continue;
            }

            f32 x = (position.x + accumTextWidth) + g.bearing.x;
            f32 y = (position.y - (g.size.y - g.bearing.y)) - height;
            f32 w = g.size.x;
            f32 h = g.size.y;

            text.vertices[idx + 0].position = { x, y };
            text.vertices[idx + 1].position = { x + w, y };
            text.vertices[idx + 2].position = { x, y + h };
            text.vertices[idx + 3].position = { x + w, y + h };

            accumTextWidth += ((g.advance >> 6) + text.charPadding.x);
            idx += VERTICES_PER_GLYPH;
        }

        text.vertices.update();
    }

    void TextUtils::setVertexUVs(UIText &text)
    {
        f32 texSize = 12 * text.font->getFontSize();
        u32 idx = 0;
        for (const char c : text.text)
        {
            Glyph g = text.font->getGlyphMap().at(c);

            /* 
            * NOTE: Because there's a mismatch in how Freetype and OpenGL handle texture coords,
            *       the texture coords for each glyph have to be flipped vertically
            */
            f32 px = (f32)g.pos.x / texSize;
            f32 py = ((f32)g.pos.y + g.size.y) / texSize;
            f32 nx = ((f32)g.pos.x + g.size.x) / texSize;
            f32 ny = (f32)g.pos.y / texSize;

            text.vertices[idx + 0].uv = { px, py };
            text.vertices[idx + 1].uv = { nx, py };
            text.vertices[idx + 2].uv = { px, ny };
            text.vertices[idx + 3].uv = { nx, ny };

            idx += VERTICES_PER_GLYPH;
        }

        text.vertices.update();
    }
}

