#pragma once

#include "idrs_math.h"

namespace idrs
{
    struct ProjectionMatrixState
    {
        f32 x;
        f32 w;
        f32 y;
        f32 h;
        f32 zNear;
        f32 zFar;
    };

    class View2D
    {
    public:
        View2D() = default;
        View2D(const FRect &rect, const f32 zNear, const f32 zFar);

        void move(const Vec2f &offset);
        void rotate(const f32 zOffset);
        void setPosition(const Vec2f &pos);
        void setRotation(const f32 zAngle);
        void setViewport(const u32 x, const u32 y, const u32 w, const u32 h);
        void setViewport(const FRect &rect);

        const FRect getViewport() const;
        const Mat4 &projection() const;
        const Mat4 &view() const;
        
    private:
        Mat4 m_projection;
        Mat4 m_view;
        ProjectionMatrixState m_state;
        f32  m_roll;

    private:
        void update();
    };
}