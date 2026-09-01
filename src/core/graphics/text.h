#pragma once

#include "core/graphics/vertexarray.h"
#include "core/graphics/font.h"

namespace idrs
{
    class Text
    {
    public:
        Text();
        Text(const std::string &text);
        ~Text();

        void draw();
        void setText(const std::string &text);
        void setFont(Font &font);
        void setPosition(const Vec2f &pos);
        void setPosition(const f32 x, const f32 y);
        void setCharacterPadding(const Vec2f &padding);
        void setCharacterPadding(const f32 hor, const f32 ver);

        const std::string &getTextString() const;
        const Font &getFont() const;
        const Vec2f &getPosition() const;
        const Vec2f &getCharacterPadding() const;

        void updateFont();

    private:
        VertexArray m_arr;
        std::string m_text;
        Font *m_font = nullptr;
        Vec2f m_position;
        Vec2f m_padding;

        const u32 k_verticesPerGlyph = 4;

    private:
        void setupBuffers();
        void setVertexPositions();
        void setVertexUVs();
        void appendVertexIndices();
    };
}