#pragma once

#include "typedefs.h"

namespace idrs
{
    class Framebuffer
    {
    public:
        enum class Target
        {
            Read = 0,
            Write,
            Both
        };

    public:
        Framebuffer() = default;
        Framebuffer(const u32 width, const u32 height);

        void bind(const Target target = Target::Both);
        void unbind(const Target target = Target::Both);
        void destroy();
        
        const u32 id() const;
        const u32 textureID() const;

    private:
        u32 m_textureId = 0;
        u32 m_id = 0;
    };
}