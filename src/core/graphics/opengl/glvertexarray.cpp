#include "../vertexarray.h"

#include <glad/gl.h>

namespace idrs
{
    VertexArray::VertexArray()
    {
    }

    VertexArray::VertexArray(const Primitive prim, const Usage usage, const u32 count) :
        m_prim(prim),
        m_usage(usage)
    {
        m_vertices.reserve(count);
    }

    void VertexArray::setVertexAttrib(const u32 location, const u32 num, const VertexAttribType type, const bool norm, const std::uintptr_t offset)
    {
        GLenum _type = getAttribType(type);

        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, num, _type, norm, sizeof(Vertex), (void*)offset);
    }

    void VertexArray::generate()
    {
        glGenVertexArrays(1, &m_vao);
        glGenBuffers(1, &m_vbo);
        glGenBuffers(1, &m_ebo);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  m_ebo);
    }

    void VertexArray::update()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), usage());
        
        if (m_updateEbo)
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(u32), m_indices.data(), usage());
            m_updateEbo = false;
        }
    }

    void VertexArray::append(const Vertex &v)
    {
        m_vertices.emplace_back(v);
    }

    void VertexArray::append(const VertexArray &arr)
    {
        m_vertices.insert(m_vertices.end(), arr.begin(), arr.end());
        m_indices.insert(m_indices.end(), arr.m_indices.begin(), arr.m_indices.end());
        m_updateEbo = true;

        // if (m_indices.size() <= 6)
        // {
        //     return;
        // }

        // u32 numSprites = (m_indices.size() / 6) - 1;

        // /* Increase the appended vertices' index buffer numbers by 4 (vertices per sprite) */
        // for (u32 i = 6; i > 0; i--)
        // {
        //     u32 index = m_indices.size() - i;
        //     m_indices[index] = m_indices[index] + (arr.count() * numSprites);
        // }
    }

    void VertexArray::append(const std::initializer_list<u32> &indices)
    {
        m_indices.insert(m_indices.end(), indices.begin(), indices.end());
        m_updateEbo = true;
    }

    void VertexArray::clear()
    {
        m_vertices.clear();
        m_indices.clear();
    }

    void VertexArray::destroy() const
    {
        glDeleteVertexArrays(1, &m_vao);
        glDeleteBuffers(1, &m_vbo);
        glDeleteBuffers(1, &m_ebo);
    }

    void VertexArray::resize(const u32 size)
    {
        m_vertices.resize(size);
        m_updateEbo = true;
    }

    const u32 VertexArray::count() const
    {
        return m_vertices.size();
    }

    const u32 VertexArray::primitive() const
    {
        switch (m_prim)
        {
            case Primitive::Point:         return GL_POINTS;           break;
            case Primitive::Line:          return GL_LINES;            break;
            case Primitive::LineStrip:     return GL_LINE_STRIP;       break;
            case Primitive::LineLoop:      return GL_LINE_LOOP;        break;
            case Primitive::Triangle:      return GL_TRIANGLES;        break;
            case Primitive::TriangleStrip: return GL_TRIANGLE_STRIP;   break;
            case Primitive::TriangleFan:   return GL_TRIANGLE_FAN;     break;
        }
        return -1;
    }

    const u32 VertexArray::usage() const
    {
        switch (m_usage)
        {
            case Usage::Stream:  return GL_STREAM_DRAW;  break;
            case Usage::Static:  return GL_STATIC_DRAW;  break;
            case Usage::Dynamic: return GL_DYNAMIC_DRAW; break;
        }
        return -1;
    }

    const u32 VertexArray::getAttribType(const VertexAttribType type) const
    {
        switch (type)
        {
            case VertexAttribType::Byte:    return GL_BYTE; break;
            case VertexAttribType::UByte:   return GL_UNSIGNED_BYTE; break;
            case VertexAttribType::Short:   return GL_SHORT; break;
            case VertexAttribType::UShort:  return GL_UNSIGNED_SHORT; break;
            case VertexAttribType::Int:     return GL_INT; break;
            case VertexAttribType::UInt:    return GL_UNSIGNED_INT; break;
            case VertexAttribType::Float:   return GL_FLOAT; break;
        }
        return -1;
    }

    const u32 VertexArray::id() const
    {
        return m_vao;
    }

    Vertex& VertexArray::operator[](u32 index)
    {
        return m_vertices[index];
    }

    const Vertex& VertexArray::operator[](u32 index) const
    {
        return m_vertices[index];
    }

    std::vector<Vertex>::iterator VertexArray::begin()
    {
        return m_vertices.begin();
    }
    std::vector<Vertex>::const_iterator VertexArray::begin() const
    {
        return m_vertices.begin();
    }

    std::vector<Vertex>::iterator VertexArray::end()
    {
        return m_vertices.end();
    }
    std::vector<Vertex>::const_iterator VertexArray::end() const
    {
        return m_vertices.end();
    }
}