#pragma once

#include <map>
#include <string>

#include "glyph.h"
#include "texture.h"

namespace strd
{
    class Font
    {
    public:
        Font() = default;
        Font(const std::string &path, const u32 fontSize);
        ~Font();

        void setFontSize(const u32 size);

        const std::map<char, Glyph> &getGlyphMap() const;
        const Texture &getGlyphAtlas() const;
        const u32 getFontSize() const;
        
    private:
        std::map<char, Glyph> m_glyphs;
        std::string m_path;
        Texture m_glyphAtlas;
        u32 m_fontSize = 0;
        u32 m_approxSize = 0;

        const u32 k_pixelPaddingWidth = 1;
        
    private:
        void init();
        void loadAscii(const void *f);
    };
}