#include "circle.h"

namespace idrs
{
    Circle::Circle(Colour clr, u32 radius, u32 numSides) :
        m_colour(clr),
        m_radius(radius),
        m_numSides(numSides),
        m_va(VertexArray::Primitive::TriangleFan, VertexArray::Usage::Dynamic, numSides)
    {
        fillArray();
    }

    void Circle::fillArray()
    {
        auto randomU8Int = [](const u8 min, const u8 max) -> u8
        {
            return rand() % (max - min + 1) + min;
        };

        u32 numVerts = m_numSides + 2;
        m_va.resize(numVerts);

        Vertex v;
        v.position = { m_pos.x, m_pos.y };
        // v.colour = m_colour;
        m_va[0] = v;

        for (u32 i = 1; i < numVerts; i++)
        {
            const f32 radians = i * doublePi / m_numSides;
            f32 x = m_pos.x + m_radius * std::cos(radians);
            f32 y = m_pos.y + m_radius * std::sin(radians);

            Vertex v;
            v.position = { x, y };
            // v.colour = { randomU8Int(0, 255), randomU8Int(0, 255), randomU8Int(0, 255), 255 };
            m_va[i] = v;
        }

        m_va.update();
    }

    void Circle::update(double dt)
    {
        float dx = m_vel.x * m_speed * dt;
        float dy = m_vel.y * m_speed * dt;

        m_pos.x += dx;
        m_pos.y += dy;

        for (u32 i = 0; i < m_va.count(); i++)
        {
            m_va[i].position.x += dx;
            m_va[i].position.y += dy;
        }

        m_va.update();
    }

    const VertexArray Circle::vertices() const
    {
        return m_va;
    }

    void Circle::keepInBounds(f32 sw, f32 sh)
    {
        if ((m_pos.x + m_radius > sw) || (m_pos.x - m_radius < 0.f))
        {
            m_vel.x = -m_vel.x;
        }
        if ((m_pos.y + m_radius > sh) || (m_pos.y - m_radius < 0.f))
        {
            m_vel.y = -m_vel.y;
        }
    }
}