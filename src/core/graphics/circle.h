#pragma once

#include "vertexarray.h"
#include "colour.h"

namespace idrs
{
    class Circle
    {
    public:
        Circle(Colour clr, u32 radius, u32 numSides);
        
        void setVel(f32 x, f32 y)
        {
            m_vel.x = x;
            m_vel.y = y;
        }
        void setSpeed(f32 speed)
        {
            m_speed = speed;
        }
        
        void keepInBounds(f32 sw, f32 sh);
        void update(double dt);
        const VertexArray vertices() const;

    private:
        VertexArray m_va;

        Vec2f m_pos = { 640, 360 };
        Vec2f m_vel;
        Colour m_colour;
        u32 m_radius = 0;
        u32 m_numSides = 0;
        f32 m_speed = 0.f;

        float doublePi = 2.0f * 3.1415926535f;

    private:
        void fillArray();
    };
}