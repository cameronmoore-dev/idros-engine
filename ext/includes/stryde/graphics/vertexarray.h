#pragma once

#include <vector>

#include "vertex.h"

namespace strd
{
    class VertexArray
    {
    public:
        enum class Primitive
        {
            Point,
            Line,
            LineStrip,
            LineLoop,
            Triangle,
            TriangleStrip,
            TriangleFan
        };

        enum class Usage
        {
            Stream,
            Static,
            Dynamic
        };

        enum class VertexAttribType
        {
            Byte = 0,
            UByte,
            Short,
            UShort,
            Int,
            UInt,
            Float
        };

    public:
        VertexArray();
        VertexArray(const Primitive prim, const Usage usage, const u32 count);
        
        void generate();
        void update();
        void append(const Vertex &v);
        void append(const VertexArray &arr);
        void append(const std::initializer_list<u32> &indices);
        void resize(u32 size);
        void clear();
        
        void setVertexAttrib(const u32 location, const u32 num, const VertexAttribType type, const bool norm, const std::uintptr_t offset);

        /* This is for explicitly destroying the buffers */
        /* TODO: Find some better way to handle destroying buffers (dtor destroys buffers after going out of scope) */
        void destroy() const;

        const u32 count() const;
        const u32 primitive() const;
        const u32 usage() const;

        const u32 id() const;
        const std::vector<u32> &indices() const { return m_indices; }

        Vertex& operator[](u32 index);
        const Vertex& operator[](u32 index) const;

        std::vector<Vertex>::iterator begin();
        std::vector<Vertex>::const_iterator begin() const;
        std::vector<Vertex>::iterator end();
        std::vector<Vertex>::const_iterator end() const;

    private:
        std::vector<Vertex> m_vertices;
        std::vector<u32> m_indices;
        Primitive m_prim = Primitive::Triangle;
        Usage m_usage = Usage::Static;
        u32 m_vao = 0;
        u32 m_vbo = 0;
        u32 m_ebo = 0;

        bool m_updateEbo = true;

    private:
        const u32 getAttribType(const VertexAttribType type) const;
    };
}