#pragma once

#include "idrs_math.h"

namespace idrs
{
    struct ProjectionMatrixState
    {
        s32 x;
        s32 w;
        s32 y;
        s32 h;
        f32 zNear;
        f32 zFar;
    };

    class View2D
    {
    public:
        View2D() = default;
        View2D(const IRect &rect, const f32 zNear, const f32 zFar);

        void move(const Vec2i &offset);
        void rotate(const f32 zOffset);
        void setPosition(const Vec2i &pos);
        void setRotation(const f32 zAngle);
        void setViewport(const s32 x, const s32 y, const s32 w, const s32 h);
        void setViewport(const IRect &rect);

        const IRect getViewport() const;
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