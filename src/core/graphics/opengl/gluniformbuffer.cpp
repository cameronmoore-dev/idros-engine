#include "../uniformbuffer.h"

#include <glad/gl.h>

namespace idrs
{
    UniformBuffer::UniformBuffer(const std::string &name, u32 bytes, u32 bindingPoint) :
        m_name(name),
        m_size(bytes),
        m_bindingpoint(bindingPoint)
    {
        glGenBuffers(1, &m_index);
        glBindBuffer(GL_UNIFORM_BUFFER, m_index);
        glBufferData(GL_UNIFORM_BUFFER, m_size, nullptr, GL_STATIC_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, m_bindingpoint, m_index);
    }

    UniformBuffer::~UniformBuffer()
    {
        glDeleteBuffers(1, &m_index);
    }

    void UniformBuffer::update(u32 offset, u32 bytes, void* value)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_index);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, bytes, value);
    }

    void UniformBuffer::setShaderBinding(Shader &shader)
    {
        u32 index = glGetUniformBlockIndex(shader.id(), m_name.c_str());
        glUniformBlockBinding(shader.id(), index, m_bindingpoint);
    }
}
