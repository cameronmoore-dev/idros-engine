#include "../framebuffer.h"

#include <glad/gl.h>

#include <cstdio>

namespace idrs
{
    Framebuffer::Framebuffer(const u32 width, const u32 height)
    {
        glGenFramebuffers(1, &m_id);
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);

        glGenTextures(1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureId, 0);

        u32 rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            printf("Framebuffer did not complete\n");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::bind(const Target target)
    {
        switch (target)
        {
            case Target::Read:  glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id); break;
            case Target::Write: glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id); break;
            case Target::Both:  glBindFramebuffer(GL_FRAMEBUFFER, m_id); break;
        }
    }

    void Framebuffer::unbind(const Target target)
    {
        switch (target)
        {
            case Target::Read:  glBindFramebuffer(GL_READ_FRAMEBUFFER, 0); break;
            case Target::Write: glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); break;
            case Target::Both:  glBindFramebuffer(GL_FRAMEBUFFER, 0); break;
        }
    }

    void Framebuffer::destroy()
    {
        glDeleteFramebuffers(1, &m_id);
    }

    const u32 Framebuffer::id() const
    {
        return m_id;
    }

    const u32 Framebuffer::textureID() const
    {
        return m_textureId;
    }
}